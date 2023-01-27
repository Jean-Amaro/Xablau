// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

// MIT License
//
// Copyright (c) 2023 Jean Amaro <jean.amaro@outlook.com.br>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <gtest/gtest.h>

import <array>;
import <concepts>;
import <iostream>;
import <numbers>;

import xablau.color;
import xablau.testing;

namespace xablau::unit_testing
{
	template < std::floating_point Type >
	struct ColorCIELabTypeDefinitions
	{
		using value_type = Type;
	};

	template < typename Types >
	class ColorCIELabTest : public ::testing::Test
	{
	public:
		static constexpr auto min = xablau::color::CIELab < typename Types::value_type > ::min_values();
		static constexpr auto max = xablau::color::CIELab < typename Types::value_type > ::max_values();
	};

	TYPED_TEST_SUITE_P(ColorCIELabTest);

	TYPED_TEST_P(ColorCIELabTest, Boundaries)
	{
		EXPECT_TRUE(this->min.valid_boundaries());
		EXPECT_TRUE(this->max.valid_boundaries());
	}

	REGISTER_TYPED_TEST_SUITE_P(
		ColorCIELabTest,
		Boundaries);

	using ColorCIELabTestDataTypes = std::tuple < float, double, long double >;

	using ColorCIELabTestGroup =
		xablau::testing::test_bundler < ColorCIELabTestDataTypes > ::template test_group < ColorCIELabTypeDefinitions >;

	using ColorCIELabTypes = xablau::testing::tuple_transformation < ::testing::Types, ColorCIELabTestGroup > ::types;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		Xablau,
		ColorCIELabTest,
		ColorCIELabTypes);
}
