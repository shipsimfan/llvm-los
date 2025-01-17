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

// insert_iterator

#include <iterator>

#include <vector>

using iterator = std::insert_iterator<std::vector<int> >;
static_assert(!std::indirectly_readable<iterator>);
static_assert(std::indirectly_writable<iterator, int>);
static_assert(!std::weakly_incrementable<iterator>);
static_assert(!std::input_or_output_iterator<iterator>);
static_assert(!std::sentinel_for<iterator, iterator>);
static_assert(!std::input_iterator<iterator>);
