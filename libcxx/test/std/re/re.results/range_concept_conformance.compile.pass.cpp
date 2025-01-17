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

// match_results

#include <regex>

#include <concepts>
#include <ranges>

namespace stdr = std::ranges;

static_assert(std::same_as<stdr::iterator_t<std::cmatch>, std::cmatch::iterator>);
static_assert(stdr::common_range<std::cmatch>);
static_assert(stdr::input_range<std::cmatch>);

static_assert(std::same_as<stdr::iterator_t<std::cmatch const>, std::cmatch::const_iterator>);
static_assert(stdr::common_range<std::cmatch const>);
static_assert(stdr::input_range<std::cmatch const>);
