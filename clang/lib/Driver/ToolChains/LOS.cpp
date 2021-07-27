//===--- LOS.cpp - LOS ToolChain Implementations ------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "LOS.h"
#include "CommonArgs.h"
#include "clang/Driver/Compilation.h"
#include "clang/Driver/DriverDiagnostic.h"
#include "llvm/Support/Path.h"

using namespace clang::driver;
using namespace clang;
using namespace llvm::opt;
using namespace toolchains;

void tools::los::Linker::ConstructJob(Compilation &C, const JobAction &JA,
                                      const InputInfo &Output,
                                      const InputInfoList &Inputs,
                                      const ArgList &Args,
                                      const char *LinkingOutput) const {
  const ToolChain &ToolChain = getToolChain();
  const Driver &D = ToolChain.getDriver();
  ArgStringList CmdArgs;

  CmdArgs.push_back("-z");
  CmdArgs.push_back("max-page-size=4096");

  if (!D.SysRoot.empty())
    CmdArgs.push_back(Args.MakeArgString("--sysroot=" + D.SysRoot));

  CmdArgs.push_back("-o");
  CmdArgs.push_back(Output.getFilename());

  if (!Args.hasArg(options::OPT_nostdlib, options::OPT_nostartfiles)) {
    CmdArgs.push_back(Args.MakeArgString(ToolChain.GetFilePath("crt0.o")));
    CmdArgs.push_back(Args.MakeArgString(ToolChain.GetFilePath("crti.o")));

    ToolChain.AddFilePathLibArgs(Args, CmdArgs);
    AddLinkerInputs(ToolChain, Inputs, Args, CmdArgs, JA);

    CmdArgs.push_back(Args.MakeArgString(ToolChain.GetFilePath("crtn.o")));

    ToolChain.addProfileRTLibs(Args, CmdArgs);

    if (!Args.hasArg(options::OPT_nostdlib, options::OPT_nodefaultlibs)) {
      if (D.CCCIsCXX())
        if (ToolChain.ShouldLinkCXXStdlib(Args))
          ToolChain.AddCXXStdlibLibArgs(Args, CmdArgs);

      AddRunTimeLibs(ToolChain, D, CmdArgs, Args);

      if (!Args.hasArg(options::OPT_nolibc))
        CmdArgs.push_back("-lc");

      CmdArgs.push_back("-lkernel");
    }
  }

  const char *Exec = Args.MakeArgString(ToolChain.GetLinkerPath());
  C.addCommand(std::make_unique<Command>(JA, *this, ResponseFileSupport::None(),
                                         Exec, CmdArgs, Inputs, Output));
}

LOS::LOS(const Driver &D, const llvm::Triple &Triple, const ArgList &Args)
    : ToolChain(D, Triple, Args) {
  getProgramPaths().push_back(getDriver().getInstalledDir());
  if (getDriver().getInstalledDir() != getDriver().Dir)
    getProgramPaths().push_back(getDriver().Dir);

  if (!D.SysRoot.empty()) {
    SmallString<128> P(D.SysRoot);
    llvm::sys::path::append(P, "los", "lib");
    getFilePaths().push_back(std::string(P.str()));
  } else
    getFilePaths().push_back(":1/los/lib");
}

void LOS::AddClangCXXStdlibIncludeArgs(
    const llvm::opt::ArgList &DriverArgs,
    llvm::opt::ArgStringList &CC1Args) const {
  if (DriverArgs.hasArg(options::OPT_nostdlibinc) ||
      DriverArgs.hasArg(options::OPT_nostdincxx))
    return;

  switch (GetCXXStdlibType(DriverArgs)) {
  case ToolChain::CST_Libcxx: {
    SmallString<128> P(getDriver().Dir);
    llvm::sys::path::append(P, "..", "include");
    std::string Version = detectLibcxxVersion(P);
    llvm::sys::path::append(P, "c++", Version);
    addSystemInclude(DriverArgs, CC1Args, P.str());
    break;
  }

  default:
    llvm_unreachable("invalid stdlib name");
  }
}

void LOS::AddCXXStdlibLibArgs(const llvm::opt::ArgList &Args,
                              llvm::opt::ArgStringList &CmdArgs) const {
  switch (GetCXXStdlibType(Args)) {
  case ToolChain::CST_Libcxx:
    CmdArgs.push_back("-lc++");
    break;

  case ToolChain::CST_Libstdcxx:
    llvm_unreachable("invalid stdlib name");
  }
}

void LOS::AddClangSystemIncludeArgs(const llvm::opt::ArgList &DriverArgs,
                                    llvm::opt::ArgStringList &CC1Args) const {
  const Driver &D = getDriver();

  if (DriverArgs.hasArg(options::OPT_nostdinc))
    return;

  if (DriverArgs.hasArg(options::OPT_nostdlibinc))
    return;

  if (!D.SysRoot.empty()) {
    SmallString<128> P(D.SysRoot);
    llvm::sys::path::append(P, "los", "include");
    addSystemInclude(DriverArgs, CC1Args, P.str());
  } else
    addSystemInclude(DriverArgs, CC1Args, ":1/los/include");
}

void LOS::addClangTargetOptions(const ArgList &DriverArgs,
                                ArgStringList &CC1Args,
                                Action::OffloadKind) const {
  if (!DriverArgs.hasFlag(options::OPT_fuse_init_array,
                          options::OPT_fno_use_init_array, true))
    CC1Args.push_back("-fno-use-init-array");
}

std::string LOS::ComputeEffectiveClangTriple(const ArgList &Args,
                                             types::ID InputType) const {
  llvm::Triple Triple(ComputeLLVMTriple(Args, InputType));
  return Triple.str();
}

ToolChain::RuntimeLibType LOS::GetRuntimeLibType(const ArgList &Args) const {
  if (Arg *A = Args.getLastArg(clang::driver::options::OPT_rtlib_EQ)) {
    StringRef Value = A->getValue();
    if (Value != "compiler-rt")
      getDriver().Diag(clang::diag::err_drv_invalid_rtlib_name)
          << A->getAsString(Args);
  }

  return ToolChain::RLT_CompilerRT;
}

ToolChain::CXXStdlibType LOS::GetCXXStdlibType(const ArgList &Args) const {
  if (Arg *A = Args.getLastArg(options::OPT_stdlib_EQ)) {
    StringRef Value = A->getValue();
    if (Value != "libc++")
      getDriver().Diag(diag::err_drv_invalid_stdlib_name)
          << A->getAsString(Args);
  }

  return ToolChain::CST_Libcxx;
}

Tool *LOS::buildLinker() const { return new tools::los::Linker(*this); }
