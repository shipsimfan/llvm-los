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

// template<range _Rp>
// using sentinel_t = decltype(ranges::end(declval<_Rp&>()));

#include <ranges>

#include <concepts>

#include "test_iterators.h"
#include "test_range.h"

namespace stdr = std::ranges;

static_assert(std::same_as<stdr::sentinel_t<test_range<> >, sentinel>);
static_assert(std::same_as<stdr::sentinel_t<test_range<> const>, sentinel>);
static_assert(std::same_as<stdr::sentinel_t<test_non_const_range<> >, sentinel>);
static_assert(std::same_as<stdr::sentinel_t<test_common_range<> >, input_iterator<int*> >);
static_assert(std::same_as<stdr::sentinel_t<test_common_range<> const>, input_iterator<int const*> >);
static_assert(std::same_as<stdr::sentinel_t<test_non_const_common_range<> >, input_iterator<int*> >);
