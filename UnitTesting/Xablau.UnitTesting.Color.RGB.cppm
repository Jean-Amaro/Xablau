// MIT License
//
// Copyright (c) 2023-2024 Jean Amaro <jean.amaro@outlook.com.br>
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

import std;

import xablau.color;
import xablau.testing;

namespace xablau::unit_testing
{
	template < typename Type, size_t BitsPerChannel >
	struct RGBConfigWrapper
	{
		using value_type = Type;
		static constexpr auto bits_per_channel = BitsPerChannel;
	};

	template < typename Type >
	struct ColorRGBTypeDefinitions
	{
		using value_type = typename Type::value_type;
		static constexpr auto bits_per_channel = Type::bits_per_channel;
	};

	template < typename Types >
	class ColorRGBTest : public ::testing::Test
	{
	public:
		static constexpr auto min = xablau::color::RGB < typename Types::value_type, Types::bits_per_channel > ::min_values();
		static constexpr auto max = xablau::color::RGB < typename Types::value_type, Types::bits_per_channel > ::max_values();
	};

	TYPED_TEST_SUITE_P(ColorRGBTest);

	TYPED_TEST_P(ColorRGBTest, Boundaries)
	{
		EXPECT_TRUE(this->min.valid_boundaries());
		EXPECT_TRUE(this->max.valid_boundaries());
	}

	REGISTER_TYPED_TEST_SUITE_P(
		ColorRGBTest,
		Boundaries);

	using ColorRGBTestDataTypes =
		std::tuple <
			RGBConfigWrapper < uint8_t, 1 >,
			RGBConfigWrapper < uint8_t, 4 >,
			RGBConfigWrapper < uint8_t, 8 >,
			RGBConfigWrapper < uint16_t, 16 >,
			RGBConfigWrapper < uint32_t, 32 >,

			RGBConfigWrapper < float, 1 >,
			RGBConfigWrapper < float, 8 >,
			RGBConfigWrapper < double, 1 >,
			RGBConfigWrapper < double, 8 >,
			RGBConfigWrapper < long double, 1 >,
			RGBConfigWrapper < long double, 8 > >;

	using ColorRGBTestGroup =
		xablau::testing::test_bundler < ColorRGBTestDataTypes > ::template test_group < ColorRGBTypeDefinitions >;

	using ColorRGBTypes = xablau::testing::tuple_transformation < ::testing::Types, ColorRGBTestGroup > ::types;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		Xablau,
		ColorRGBTest,
		ColorRGBTypes);
}
