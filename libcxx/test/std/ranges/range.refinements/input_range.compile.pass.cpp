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
// concept input_range;

#include <ranges>

#include "test_range.h"

namespace stdr = std::ranges;

static_assert(stdr::input_range<test_range<input_iterator> >);
static_assert(stdr::input_range<test_range<input_iterator> const>);

static_assert(stdr::input_range<test_range<cpp20_input_iterator> >);
static_assert(stdr::input_range<test_range<cpp20_input_iterator> const>);

static_assert(stdr::input_range<test_non_const_range<input_iterator> >);
static_assert(stdr::input_range<test_non_const_range<cpp20_input_iterator> >);

static_assert(!stdr::input_range<test_non_const_range<input_iterator> const>);
static_assert(!stdr::input_range<test_non_const_range<cpp20_input_iterator> const>);

static_assert(stdr::input_range<test_common_range<input_iterator> >);
static_assert(!stdr::input_range<test_common_range<cpp20_input_iterator> >);

static_assert(stdr::input_range<test_common_range<input_iterator> const>);
static_assert(!stdr::input_range<test_common_range<cpp20_input_iterator> const>);

static_assert(stdr::input_range<test_non_const_common_range<input_iterator> >);
static_assert(!stdr::input_range<test_non_const_common_range<cpp20_input_iterator> >);

static_assert(!stdr::input_range<test_non_const_common_range<input_iterator> const>);
static_assert(!stdr::input_range<test_non_const_common_range<cpp20_input_iterator> const>);
