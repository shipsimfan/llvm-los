//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17
// UNSUPPORTED: libcpp-no-concepts
// UNSUPPORTED: gcc-10
// XFAIL: msvc && clang

// path

#include "filesystem_include.h"

#include <concepts>
#include <ranges>

namespace stdr = std::ranges;

static_assert(std::same_as<stdr::iterator_t<fs::path>, fs::path::iterator>);
static_assert(stdr::common_range<fs::path>);
static_assert(stdr::input_range<fs::path>);

static_assert(std::same_as<stdr::iterator_t<fs::path const>, fs::path::const_iterator>);
static_assert(stdr::common_range<fs::path const>);
static_assert(stdr::input_range<fs::path const>);
