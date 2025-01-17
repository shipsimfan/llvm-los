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

// template<range R>
// using range_difference_t = iter_difference_t<iterator_t<R>>;

// template<range R>
// using range_value_t = iter_value_t<iterator_t<R>>;

// template<range R>
// using range_reference_t = iter_reference_t<iterator_t<R>>;

// template<range R>
// using range_rvalue_reference_t = iter_rvalue_reference_t<iterator_t<R>>;

#include <ranges>

#include <concepts>

#include "test_range.h"

namespace stdr = std::ranges;

static_assert(std::same_as<stdr::range_difference_t<test_range<> >, std::iter_difference_t<int*> >);
static_assert(std::same_as<stdr::range_value_t<test_range<> >, std::iter_value_t<int*> >);
static_assert(std::same_as<stdr::range_reference_t<test_range<> >, std::iter_reference_t<int*> >);
static_assert(std::same_as<stdr::range_rvalue_reference_t<test_range<> >, std::iter_rvalue_reference_t<int*> >);
