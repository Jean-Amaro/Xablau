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
import <numbers>;

import xablau.geometry;
import xablau.testing;

namespace xablau::unit_testing
{
	template < xablau::geometry::concepts::geometric_assignable_arithmetic Type >
	struct GeometryHypersphereTypeDefinitions
	{
		using value_type = Type;
	};

	template < typename Types >
	class GeometryHypersphereTest : public ::testing::Test
	{
	public:
		static constexpr xablau::geometry::vertex < typename Types::value_type, xablau::geometry::spatial_dimension < 0 > > vertexOrigin{};
		static constexpr auto vertexPositiveX = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(1) }));
		static constexpr auto vertexNegativeX = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(-1) }));
		static constexpr auto vertexPositiveY = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(0), typename Types::value_type(1) }));
		static constexpr auto vertexNegativeY = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(0), typename Types::value_type(-1) }));
		static constexpr auto vertexPositiveZ = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(0), typename Types::value_type(0), typename Types::value_type(1) }));
		static constexpr auto vertexNegativeZ = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(0), typename Types::value_type(0), typename Types::value_type(-1) }));

		static constexpr auto vertexPositiveDoubleX = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(2) }));
		static constexpr auto vertexNegativeDoubleX = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(-2) }));
		static constexpr auto vertexPositiveDoubleY = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(0), typename Types::value_type(2) }));
		static constexpr auto vertexNegativeDoubleY = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(0), typename Types::value_type(-2) }));
		static constexpr auto vertexPositiveDoubleZ = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(0), typename Types::value_type(0), typename Types::value_type(2) }));
		static constexpr auto vertexNegativeDoubleZ = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(0), typename Types::value_type(0), typename Types::value_type(-2) }));

		static constexpr auto hypersphereDegenerateNegative1 = xablau::geometry::make_hypersphere(vertexNegativeX, typename Types::value_type(-1));
		static constexpr auto hypersphereDegenerateZero1 = xablau::geometry::make_hypersphere(vertexOrigin, typename Types::value_type(-1));
		static constexpr auto hypersphereDegenerateNegative2 = xablau::geometry::make_hypersphere(vertexNegativeX, typename Types::value_type(0));
		static constexpr auto hypersphereDegenerateZero2 = xablau::geometry::make_hypersphere(vertexOrigin, typename Types::value_type(0));

		static constexpr auto hypersphereNegativeRadius1 = xablau::geometry::make_hypersphere(vertexNegativeX, typename Types::value_type(1));
		static constexpr auto hypersphereZeroRadius1 = xablau::geometry::make_hypersphere(vertexOrigin, typename Types::value_type(1));
		static constexpr auto hyperspherePositiveRadius1 = xablau::geometry::make_hypersphere(vertexPositiveX, typename Types::value_type(1));
		static constexpr auto hypersphereNegativeRadius2 = xablau::geometry::make_hypersphere(vertexNegativeX, typename Types::value_type(2));
		static constexpr auto hypersphereZeroRadius2 = xablau::geometry::make_hypersphere(vertexOrigin, typename Types::value_type(2));
		static constexpr auto hyperspherePositiveRadius2 = xablau::geometry::make_hypersphere(vertexPositiveX, typename Types::value_type(2));
	};

	TYPED_TEST_SUITE_P(GeometryHypersphereTest);

	TYPED_TEST_P(GeometryHypersphereTest, ComparisonOperatorLess)
	{
		EXPECT_FALSE(this->hypersphereDegenerateNegative1 < this->hypersphereDegenerateNegative1);
		EXPECT_TRUE(this->hypersphereDegenerateNegative1 < this->hypersphereDegenerateZero1);
		EXPECT_TRUE(this->hypersphereDegenerateNegative1 < this->hypersphereDegenerateNegative2);
		EXPECT_TRUE(this->hypersphereDegenerateNegative1 < this->hypersphereDegenerateZero2);
		EXPECT_TRUE(this->hypersphereDegenerateNegative1 < this->hypersphereNegativeRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateNegative1 < this->hypersphereZeroRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateNegative1 < this->hyperspherePositiveRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateNegative1 < this->hypersphereNegativeRadius2);
		EXPECT_TRUE(this->hypersphereDegenerateNegative1 < this->hypersphereZeroRadius2);
		EXPECT_TRUE(this->hypersphereDegenerateNegative1 < this->hyperspherePositiveRadius2);

		EXPECT_FALSE(this->hypersphereDegenerateZero1 < this->hypersphereDegenerateNegative1);
		EXPECT_FALSE(this->hypersphereDegenerateZero1 < this->hypersphereDegenerateZero1);
		EXPECT_TRUE(this->hypersphereDegenerateZero1 < this->hypersphereDegenerateNegative2);
		EXPECT_TRUE(this->hypersphereDegenerateZero1 < this->hypersphereDegenerateZero2);
		EXPECT_TRUE(this->hypersphereDegenerateZero1 < this->hypersphereNegativeRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateZero1 < this->hypersphereZeroRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateZero1 < this->hyperspherePositiveRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateZero1 < this->hypersphereNegativeRadius2);
		EXPECT_TRUE(this->hypersphereDegenerateZero1 < this->hypersphereZeroRadius2);
		EXPECT_TRUE(this->hypersphereDegenerateZero1 < this->hyperspherePositiveRadius2);

		EXPECT_FALSE(this->hypersphereDegenerateNegative2 < this->hypersphereDegenerateNegative1);
		EXPECT_FALSE(this->hypersphereDegenerateNegative2 < this->hypersphereDegenerateZero1);
		EXPECT_FALSE(this->hypersphereDegenerateNegative2 < this->hypersphereDegenerateNegative2);
		EXPECT_TRUE(this->hypersphereDegenerateNegative2 < this->hypersphereDegenerateZero2);
		EXPECT_TRUE(this->hypersphereDegenerateNegative2 < this->hypersphereNegativeRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateNegative2 < this->hypersphereZeroRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateNegative2 < this->hyperspherePositiveRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateNegative2 < this->hypersphereNegativeRadius2);
		EXPECT_TRUE(this->hypersphereDegenerateNegative2 < this->hypersphereZeroRadius2);
		EXPECT_TRUE(this->hypersphereDegenerateNegative2 < this->hyperspherePositiveRadius2);

		EXPECT_FALSE(this->hypersphereDegenerateZero2 < this->hypersphereDegenerateNegative1);
		EXPECT_FALSE(this->hypersphereDegenerateZero2 < this->hypersphereDegenerateZero1);
		EXPECT_FALSE(this->hypersphereDegenerateZero2 < this->hypersphereDegenerateNegative2);
		EXPECT_FALSE(this->hypersphereDegenerateZero2 < this->hypersphereDegenerateZero2);
		EXPECT_TRUE(this->hypersphereDegenerateZero2 < this->hypersphereNegativeRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateZero2 < this->hypersphereZeroRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateZero2 < this->hyperspherePositiveRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateZero2 < this->hypersphereNegativeRadius2);
		EXPECT_TRUE(this->hypersphereDegenerateZero2 < this->hypersphereZeroRadius2);
		EXPECT_TRUE(this->hypersphereDegenerateZero2 < this->hyperspherePositiveRadius2);

		EXPECT_FALSE(this->hypersphereNegativeRadius1 < this->hypersphereDegenerateNegative1);
		EXPECT_FALSE(this->hypersphereNegativeRadius1 < this->hypersphereDegenerateZero1);
		EXPECT_FALSE(this->hypersphereNegativeRadius1 < this->hypersphereDegenerateNegative2);
		EXPECT_FALSE(this->hypersphereNegativeRadius1 < this->hypersphereDegenerateZero2);
		EXPECT_FALSE(this->hypersphereNegativeRadius1 < this->hypersphereNegativeRadius1);
		EXPECT_TRUE(this->hypersphereNegativeRadius1 < this->hypersphereZeroRadius1);
		EXPECT_TRUE(this->hypersphereNegativeRadius1 < this->hyperspherePositiveRadius1);
		EXPECT_TRUE(this->hypersphereNegativeRadius1 < this->hypersphereNegativeRadius2);
		EXPECT_TRUE(this->hypersphereNegativeRadius1 < this->hypersphereZeroRadius2);
		EXPECT_TRUE(this->hypersphereNegativeRadius1 < this->hyperspherePositiveRadius2);

		EXPECT_FALSE(this->hypersphereZeroRadius1 < this->hypersphereDegenerateNegative1);
		EXPECT_FALSE(this->hypersphereZeroRadius1 < this->hypersphereDegenerateZero1);
		EXPECT_FALSE(this->hypersphereZeroRadius1 < this->hypersphereDegenerateNegative2);
		EXPECT_FALSE(this->hypersphereZeroRadius1 < this->hypersphereDegenerateZero2);
		EXPECT_FALSE(this->hypersphereZeroRadius1 < this->hypersphereNegativeRadius1);
		EXPECT_FALSE(this->hypersphereZeroRadius1 < this->hypersphereZeroRadius1);
		EXPECT_TRUE(this->hypersphereZeroRadius1 < this->hyperspherePositiveRadius1);
		EXPECT_TRUE(this->hypersphereZeroRadius1 < this->hypersphereNegativeRadius2);
		EXPECT_TRUE(this->hypersphereZeroRadius1 < this->hypersphereZeroRadius2);
		EXPECT_TRUE(this->hypersphereZeroRadius1 < this->hyperspherePositiveRadius2);

		EXPECT_FALSE(this->hyperspherePositiveRadius1 < this->hypersphereDegenerateNegative1);
		EXPECT_FALSE(this->hyperspherePositiveRadius1 < this->hypersphereDegenerateZero1);
		EXPECT_FALSE(this->hyperspherePositiveRadius1 < this->hypersphereDegenerateNegative2);
		EXPECT_FALSE(this->hyperspherePositiveRadius1 < this->hypersphereDegenerateZero2);
		EXPECT_FALSE(this->hyperspherePositiveRadius1 < this->hypersphereNegativeRadius1);
		EXPECT_FALSE(this->hyperspherePositiveRadius1 < this->hypersphereZeroRadius1);
		EXPECT_FALSE(this->hyperspherePositiveRadius1 < this->hyperspherePositiveRadius1);
		EXPECT_TRUE(this->hyperspherePositiveRadius1 < this->hypersphereNegativeRadius2);
		EXPECT_TRUE(this->hyperspherePositiveRadius1 < this->hypersphereZeroRadius2);
		EXPECT_TRUE(this->hyperspherePositiveRadius1 < this->hyperspherePositiveRadius2);

		EXPECT_FALSE(this->hypersphereNegativeRadius2 < this->hypersphereDegenerateNegative1);
		EXPECT_FALSE(this->hypersphereNegativeRadius2 < this->hypersphereDegenerateZero1);
		EXPECT_FALSE(this->hypersphereNegativeRadius2 < this->hypersphereDegenerateNegative2);
		EXPECT_FALSE(this->hypersphereNegativeRadius2 < this->hypersphereDegenerateZero2);
		EXPECT_FALSE(this->hypersphereNegativeRadius2 < this->hypersphereNegativeRadius1);
		EXPECT_FALSE(this->hypersphereNegativeRadius2 < this->hypersphereZeroRadius1);
		EXPECT_FALSE(this->hypersphereNegativeRadius2 < this->hyperspherePositiveRadius1);
		EXPECT_FALSE(this->hypersphereNegativeRadius2 < this->hypersphereNegativeRadius2);
		EXPECT_TRUE(this->hypersphereNegativeRadius2 < this->hypersphereZeroRadius2);
		EXPECT_TRUE(this->hypersphereNegativeRadius2 < this->hyperspherePositiveRadius2);

		EXPECT_FALSE(this->hypersphereZeroRadius2 < this->hypersphereDegenerateNegative1);
		EXPECT_FALSE(this->hypersphereZeroRadius2 < this->hypersphereDegenerateZero1);
		EXPECT_FALSE(this->hypersphereZeroRadius2 < this->hypersphereDegenerateNegative2);
		EXPECT_FALSE(this->hypersphereZeroRadius2 < this->hypersphereDegenerateZero2);
		EXPECT_FALSE(this->hypersphereZeroRadius2 < this->hypersphereNegativeRadius1);
		EXPECT_FALSE(this->hypersphereZeroRadius2 < this->hypersphereZeroRadius1);
		EXPECT_FALSE(this->hypersphereZeroRadius2 < this->hyperspherePositiveRadius1);
		EXPECT_FALSE(this->hypersphereZeroRadius2 < this->hypersphereNegativeRadius2);
		EXPECT_FALSE(this->hypersphereZeroRadius2 < this->hypersphereZeroRadius2);
		EXPECT_TRUE(this->hypersphereZeroRadius2 < this->hyperspherePositiveRadius2);

		EXPECT_FALSE(this->hyperspherePositiveRadius2 < this->hypersphereDegenerateNegative1);
		EXPECT_FALSE(this->hyperspherePositiveRadius2 < this->hypersphereDegenerateZero1);
		EXPECT_FALSE(this->hyperspherePositiveRadius2 < this->hypersphereDegenerateNegative2);
		EXPECT_FALSE(this->hyperspherePositiveRadius2 < this->hypersphereDegenerateZero2);
		EXPECT_FALSE(this->hyperspherePositiveRadius2 < this->hypersphereNegativeRadius1);
		EXPECT_FALSE(this->hyperspherePositiveRadius2 < this->hypersphereZeroRadius1);
		EXPECT_FALSE(this->hyperspherePositiveRadius2 < this->hyperspherePositiveRadius1);
		EXPECT_FALSE(this->hyperspherePositiveRadius2 < this->hypersphereNegativeRadius2);
		EXPECT_FALSE(this->hyperspherePositiveRadius2 < this->hypersphereZeroRadius2);
		EXPECT_FALSE(this->hyperspherePositiveRadius2 < this->hyperspherePositiveRadius2);
	}

	TYPED_TEST_P(GeometryHypersphereTest, ComparisonOperatorLessEqualTo)
	{
		EXPECT_TRUE(this->hypersphereDegenerateNegative1 <= this->hypersphereDegenerateNegative1);
		EXPECT_TRUE(this->hypersphereDegenerateNegative1 <= this->hypersphereDegenerateZero1);
		EXPECT_TRUE(this->hypersphereDegenerateNegative1 <= this->hypersphereDegenerateNegative2);
		EXPECT_TRUE(this->hypersphereDegenerateNegative1 <= this->hypersphereDegenerateZero2);
		EXPECT_TRUE(this->hypersphereDegenerateNegative1 <= this->hypersphereNegativeRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateNegative1 <= this->hypersphereZeroRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateNegative1 <= this->hyperspherePositiveRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateNegative1 <= this->hypersphereNegativeRadius2);
		EXPECT_TRUE(this->hypersphereDegenerateNegative1 <= this->hypersphereZeroRadius2);
		EXPECT_TRUE(this->hypersphereDegenerateNegative1 <= this->hyperspherePositiveRadius2);

		EXPECT_FALSE(this->hypersphereDegenerateZero1 <= this->hypersphereDegenerateNegative1);
		EXPECT_TRUE(this->hypersphereDegenerateZero1 <= this->hypersphereDegenerateZero1);
		EXPECT_TRUE(this->hypersphereDegenerateZero1 <= this->hypersphereDegenerateNegative2);
		EXPECT_TRUE(this->hypersphereDegenerateZero1 <= this->hypersphereDegenerateZero2);
		EXPECT_TRUE(this->hypersphereDegenerateZero1 <= this->hypersphereNegativeRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateZero1 <= this->hypersphereZeroRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateZero1 <= this->hyperspherePositiveRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateZero1 <= this->hypersphereNegativeRadius2);
		EXPECT_TRUE(this->hypersphereDegenerateZero1 <= this->hypersphereZeroRadius2);
		EXPECT_TRUE(this->hypersphereDegenerateZero1 <= this->hyperspherePositiveRadius2);

		EXPECT_FALSE(this->hypersphereDegenerateNegative2 <= this->hypersphereDegenerateNegative1);
		EXPECT_FALSE(this->hypersphereDegenerateNegative2 <= this->hypersphereDegenerateZero1);
		EXPECT_TRUE(this->hypersphereDegenerateNegative2 <= this->hypersphereDegenerateNegative2);
		EXPECT_TRUE(this->hypersphereDegenerateNegative2 <= this->hypersphereDegenerateZero2);
		EXPECT_TRUE(this->hypersphereDegenerateNegative2 <= this->hypersphereNegativeRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateNegative2 <= this->hypersphereZeroRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateNegative2 <= this->hyperspherePositiveRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateNegative2 <= this->hypersphereNegativeRadius2);
		EXPECT_TRUE(this->hypersphereDegenerateNegative2 <= this->hypersphereZeroRadius2);
		EXPECT_TRUE(this->hypersphereDegenerateNegative2 <= this->hyperspherePositiveRadius2);

		EXPECT_FALSE(this->hypersphereDegenerateZero2 <= this->hypersphereDegenerateNegative1);
		EXPECT_FALSE(this->hypersphereDegenerateZero2 <= this->hypersphereDegenerateZero1);
		EXPECT_FALSE(this->hypersphereDegenerateZero2 <= this->hypersphereDegenerateNegative2);
		EXPECT_TRUE(this->hypersphereDegenerateZero2 <= this->hypersphereDegenerateZero2);
		EXPECT_TRUE(this->hypersphereDegenerateZero2 <= this->hypersphereNegativeRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateZero2 <= this->hypersphereZeroRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateZero2 <= this->hyperspherePositiveRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateZero2 <= this->hypersphereNegativeRadius2);
		EXPECT_TRUE(this->hypersphereDegenerateZero2 <= this->hypersphereZeroRadius2);
		EXPECT_TRUE(this->hypersphereDegenerateZero2 <= this->hyperspherePositiveRadius2);

		EXPECT_FALSE(this->hypersphereNegativeRadius1 <= this->hypersphereDegenerateNegative1);
		EXPECT_FALSE(this->hypersphereNegativeRadius1 <= this->hypersphereDegenerateZero1);
		EXPECT_FALSE(this->hypersphereNegativeRadius1 <= this->hypersphereDegenerateNegative2);
		EXPECT_FALSE(this->hypersphereNegativeRadius1 <= this->hypersphereDegenerateZero2);
		EXPECT_TRUE(this->hypersphereNegativeRadius1 <= this->hypersphereNegativeRadius1);
		EXPECT_TRUE(this->hypersphereNegativeRadius1 <= this->hypersphereZeroRadius1);
		EXPECT_TRUE(this->hypersphereNegativeRadius1 <= this->hyperspherePositiveRadius1);
		EXPECT_TRUE(this->hypersphereNegativeRadius1 <= this->hypersphereNegativeRadius2);
		EXPECT_TRUE(this->hypersphereNegativeRadius1 <= this->hypersphereZeroRadius2);
		EXPECT_TRUE(this->hypersphereNegativeRadius1 <= this->hyperspherePositiveRadius2);

		EXPECT_FALSE(this->hypersphereZeroRadius1 <= this->hypersphereDegenerateNegative1);
		EXPECT_FALSE(this->hypersphereZeroRadius1 <= this->hypersphereDegenerateZero1);
		EXPECT_FALSE(this->hypersphereZeroRadius1 <= this->hypersphereDegenerateNegative2);
		EXPECT_FALSE(this->hypersphereZeroRadius1 <= this->hypersphereDegenerateZero2);
		EXPECT_FALSE(this->hypersphereZeroRadius1 <= this->hypersphereNegativeRadius1);
		EXPECT_TRUE(this->hypersphereZeroRadius1 <= this->hypersphereZeroRadius1);
		EXPECT_TRUE(this->hypersphereZeroRadius1 <= this->hyperspherePositiveRadius1);
		EXPECT_TRUE(this->hypersphereZeroRadius1 <= this->hypersphereNegativeRadius2);
		EXPECT_TRUE(this->hypersphereZeroRadius1 <= this->hypersphereZeroRadius2);
		EXPECT_TRUE(this->hypersphereZeroRadius1 <= this->hyperspherePositiveRadius2);

		EXPECT_FALSE(this->hyperspherePositiveRadius1 <= this->hypersphereDegenerateNegative1);
		EXPECT_FALSE(this->hyperspherePositiveRadius1 <= this->hypersphereDegenerateZero1);
		EXPECT_FALSE(this->hyperspherePositiveRadius1 <= this->hypersphereDegenerateNegative2);
		EXPECT_FALSE(this->hyperspherePositiveRadius1 <= this->hypersphereDegenerateZero2);
		EXPECT_FALSE(this->hyperspherePositiveRadius1 <= this->hypersphereNegativeRadius1);
		EXPECT_FALSE(this->hyperspherePositiveRadius1 <= this->hypersphereZeroRadius1);
		EXPECT_TRUE(this->hyperspherePositiveRadius1 <= this->hyperspherePositiveRadius1);
		EXPECT_TRUE(this->hyperspherePositiveRadius1 <= this->hypersphereNegativeRadius2);
		EXPECT_TRUE(this->hyperspherePositiveRadius1 <= this->hypersphereZeroRadius2);
		EXPECT_TRUE(this->hyperspherePositiveRadius1 <= this->hyperspherePositiveRadius2);

		EXPECT_FALSE(this->hypersphereNegativeRadius2 <= this->hypersphereDegenerateNegative1);
		EXPECT_FALSE(this->hypersphereNegativeRadius2 <= this->hypersphereDegenerateZero1);
		EXPECT_FALSE(this->hypersphereNegativeRadius2 <= this->hypersphereDegenerateNegative2);
		EXPECT_FALSE(this->hypersphereNegativeRadius2 <= this->hypersphereDegenerateZero2);
		EXPECT_FALSE(this->hypersphereNegativeRadius2 <= this->hypersphereNegativeRadius1);
		EXPECT_FALSE(this->hypersphereNegativeRadius2 <= this->hypersphereZeroRadius1);
		EXPECT_FALSE(this->hypersphereNegativeRadius2 <= this->hyperspherePositiveRadius1);
		EXPECT_TRUE(this->hypersphereNegativeRadius2 <= this->hypersphereNegativeRadius2);
		EXPECT_TRUE(this->hypersphereNegativeRadius2 <= this->hypersphereZeroRadius2);
		EXPECT_TRUE(this->hypersphereNegativeRadius2 <= this->hyperspherePositiveRadius2);

		EXPECT_FALSE(this->hypersphereZeroRadius2 <= this->hypersphereDegenerateNegative1);
		EXPECT_FALSE(this->hypersphereZeroRadius2 <= this->hypersphereDegenerateZero1);
		EXPECT_FALSE(this->hypersphereZeroRadius2 <= this->hypersphereDegenerateNegative2);
		EXPECT_FALSE(this->hypersphereZeroRadius2 <= this->hypersphereDegenerateZero2);
		EXPECT_FALSE(this->hypersphereZeroRadius2 <= this->hypersphereNegativeRadius1);
		EXPECT_FALSE(this->hypersphereZeroRadius2 <= this->hypersphereZeroRadius1);
		EXPECT_FALSE(this->hypersphereZeroRadius2 <= this->hyperspherePositiveRadius1);
		EXPECT_FALSE(this->hypersphereZeroRadius2 <= this->hypersphereNegativeRadius2);
		EXPECT_TRUE(this->hypersphereZeroRadius2 <= this->hypersphereZeroRadius2);
		EXPECT_TRUE(this->hypersphereZeroRadius2 <= this->hyperspherePositiveRadius2);

		EXPECT_FALSE(this->hyperspherePositiveRadius2 <= this->hypersphereDegenerateNegative1);
		EXPECT_FALSE(this->hyperspherePositiveRadius2 <= this->hypersphereDegenerateZero1);
		EXPECT_FALSE(this->hyperspherePositiveRadius2 <= this->hypersphereDegenerateNegative2);
		EXPECT_FALSE(this->hyperspherePositiveRadius2 <= this->hypersphereDegenerateZero2);
		EXPECT_FALSE(this->hyperspherePositiveRadius2 <= this->hypersphereNegativeRadius1);
		EXPECT_FALSE(this->hyperspherePositiveRadius2 <= this->hypersphereZeroRadius1);
		EXPECT_FALSE(this->hyperspherePositiveRadius2 <= this->hyperspherePositiveRadius1);
		EXPECT_FALSE(this->hyperspherePositiveRadius2 <= this->hypersphereNegativeRadius2);
		EXPECT_FALSE(this->hyperspherePositiveRadius2 <= this->hypersphereZeroRadius2);
		EXPECT_TRUE(this->hyperspherePositiveRadius2 <= this->hyperspherePositiveRadius2);
	}

	TYPED_TEST_P(GeometryHypersphereTest, ComparisonOperatorEqualTo)
	{
		EXPECT_TRUE(this->hypersphereDegenerateNegative1 == this->hypersphereDegenerateNegative1);
		EXPECT_FALSE(this->hypersphereDegenerateNegative1 == this->hypersphereDegenerateZero1);
		EXPECT_FALSE(this->hypersphereDegenerateNegative1 == this->hypersphereDegenerateNegative2);
		EXPECT_FALSE(this->hypersphereDegenerateNegative1 == this->hypersphereDegenerateZero2);
		EXPECT_FALSE(this->hypersphereDegenerateNegative1 == this->hypersphereNegativeRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateNegative1 == this->hypersphereZeroRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateNegative1 == this->hyperspherePositiveRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateNegative1 == this->hypersphereNegativeRadius2);
		EXPECT_FALSE(this->hypersphereDegenerateNegative1 == this->hypersphereZeroRadius2);
		EXPECT_FALSE(this->hypersphereDegenerateNegative1 == this->hyperspherePositiveRadius2);

		EXPECT_FALSE(this->hypersphereDegenerateZero1 == this->hypersphereDegenerateNegative1);
		EXPECT_TRUE(this->hypersphereDegenerateZero1 == this->hypersphereDegenerateZero1);
		EXPECT_FALSE(this->hypersphereDegenerateZero1 == this->hypersphereDegenerateNegative2);
		EXPECT_FALSE(this->hypersphereDegenerateZero1 == this->hypersphereDegenerateZero2);
		EXPECT_FALSE(this->hypersphereDegenerateZero1 == this->hypersphereNegativeRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateZero1 == this->hypersphereZeroRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateZero1 == this->hyperspherePositiveRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateZero1 == this->hypersphereNegativeRadius2);
		EXPECT_FALSE(this->hypersphereDegenerateZero1 == this->hypersphereZeroRadius2);
		EXPECT_FALSE(this->hypersphereDegenerateZero1 == this->hyperspherePositiveRadius2);

		EXPECT_FALSE(this->hypersphereDegenerateNegative2 == this->hypersphereDegenerateNegative1);
		EXPECT_FALSE(this->hypersphereDegenerateNegative2 == this->hypersphereDegenerateZero1);
		EXPECT_TRUE(this->hypersphereDegenerateNegative2 == this->hypersphereDegenerateNegative2);
		EXPECT_FALSE(this->hypersphereDegenerateNegative2 == this->hypersphereDegenerateZero2);
		EXPECT_FALSE(this->hypersphereDegenerateNegative2 == this->hypersphereNegativeRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateNegative2 == this->hypersphereZeroRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateNegative2 == this->hyperspherePositiveRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateNegative2 == this->hypersphereNegativeRadius2);
		EXPECT_FALSE(this->hypersphereDegenerateNegative2 == this->hypersphereZeroRadius2);
		EXPECT_FALSE(this->hypersphereDegenerateNegative2 == this->hyperspherePositiveRadius2);

		EXPECT_FALSE(this->hypersphereDegenerateZero2 == this->hypersphereDegenerateNegative1);
		EXPECT_FALSE(this->hypersphereDegenerateZero2 == this->hypersphereDegenerateZero1);
		EXPECT_FALSE(this->hypersphereDegenerateZero2 == this->hypersphereDegenerateNegative2);
		EXPECT_TRUE(this->hypersphereDegenerateZero2 == this->hypersphereDegenerateZero2);
		EXPECT_FALSE(this->hypersphereDegenerateZero2 == this->hypersphereNegativeRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateZero2 == this->hypersphereZeroRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateZero2 == this->hyperspherePositiveRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateZero2 == this->hypersphereNegativeRadius2);
		EXPECT_FALSE(this->hypersphereDegenerateZero2 == this->hypersphereZeroRadius2);
		EXPECT_FALSE(this->hypersphereDegenerateZero2 == this->hyperspherePositiveRadius2);

		EXPECT_FALSE(this->hypersphereNegativeRadius1 == this->hypersphereDegenerateNegative1);
		EXPECT_FALSE(this->hypersphereNegativeRadius1 == this->hypersphereDegenerateZero1);
		EXPECT_FALSE(this->hypersphereNegativeRadius1 == this->hypersphereDegenerateNegative2);
		EXPECT_FALSE(this->hypersphereNegativeRadius1 == this->hypersphereDegenerateZero2);
		EXPECT_TRUE(this->hypersphereNegativeRadius1 == this->hypersphereNegativeRadius1);
		EXPECT_FALSE(this->hypersphereNegativeRadius1 == this->hypersphereZeroRadius1);
		EXPECT_FALSE(this->hypersphereNegativeRadius1 == this->hyperspherePositiveRadius1);
		EXPECT_FALSE(this->hypersphereNegativeRadius1 == this->hypersphereNegativeRadius2);
		EXPECT_FALSE(this->hypersphereNegativeRadius1 == this->hypersphereZeroRadius2);
		EXPECT_FALSE(this->hypersphereNegativeRadius1 == this->hyperspherePositiveRadius2);

		EXPECT_FALSE(this->hypersphereZeroRadius1 == this->hypersphereDegenerateNegative1);
		EXPECT_FALSE(this->hypersphereZeroRadius1 == this->hypersphereDegenerateZero1);
		EXPECT_FALSE(this->hypersphereZeroRadius1 == this->hypersphereDegenerateNegative2);
		EXPECT_FALSE(this->hypersphereZeroRadius1 == this->hypersphereDegenerateZero2);
		EXPECT_FALSE(this->hypersphereZeroRadius1 == this->hypersphereNegativeRadius1);
		EXPECT_TRUE(this->hypersphereZeroRadius1 == this->hypersphereZeroRadius1);
		EXPECT_FALSE(this->hypersphereZeroRadius1 == this->hyperspherePositiveRadius1);
		EXPECT_FALSE(this->hypersphereZeroRadius1 == this->hypersphereNegativeRadius2);
		EXPECT_FALSE(this->hypersphereZeroRadius1 == this->hypersphereZeroRadius2);
		EXPECT_FALSE(this->hypersphereZeroRadius1 == this->hyperspherePositiveRadius2);

		EXPECT_FALSE(this->hyperspherePositiveRadius1 == this->hypersphereDegenerateNegative1);
		EXPECT_FALSE(this->hyperspherePositiveRadius1 == this->hypersphereDegenerateZero1);
		EXPECT_FALSE(this->hyperspherePositiveRadius1 == this->hypersphereDegenerateNegative2);
		EXPECT_FALSE(this->hyperspherePositiveRadius1 == this->hypersphereDegenerateZero2);
		EXPECT_FALSE(this->hyperspherePositiveRadius1 == this->hypersphereNegativeRadius1);
		EXPECT_FALSE(this->hyperspherePositiveRadius1 == this->hypersphereZeroRadius1);
		EXPECT_TRUE(this->hyperspherePositiveRadius1 == this->hyperspherePositiveRadius1);
		EXPECT_FALSE(this->hyperspherePositiveRadius1 == this->hypersphereNegativeRadius2);
		EXPECT_FALSE(this->hyperspherePositiveRadius1 == this->hypersphereZeroRadius2);
		EXPECT_FALSE(this->hyperspherePositiveRadius1 == this->hyperspherePositiveRadius2);

		EXPECT_FALSE(this->hypersphereNegativeRadius2 == this->hypersphereDegenerateNegative1);
		EXPECT_FALSE(this->hypersphereNegativeRadius2 == this->hypersphereDegenerateZero1);
		EXPECT_FALSE(this->hypersphereNegativeRadius2 == this->hypersphereDegenerateNegative2);
		EXPECT_FALSE(this->hypersphereNegativeRadius2 == this->hypersphereDegenerateZero2);
		EXPECT_FALSE(this->hypersphereNegativeRadius2 == this->hypersphereNegativeRadius1);
		EXPECT_FALSE(this->hypersphereNegativeRadius2 == this->hypersphereZeroRadius1);
		EXPECT_FALSE(this->hypersphereNegativeRadius2 == this->hyperspherePositiveRadius1);
		EXPECT_TRUE(this->hypersphereNegativeRadius2 == this->hypersphereNegativeRadius2);
		EXPECT_FALSE(this->hypersphereNegativeRadius2 == this->hypersphereZeroRadius2);
		EXPECT_FALSE(this->hypersphereNegativeRadius2 == this->hyperspherePositiveRadius2);

		EXPECT_FALSE(this->hypersphereZeroRadius2 == this->hypersphereDegenerateNegative1);
		EXPECT_FALSE(this->hypersphereZeroRadius2 == this->hypersphereDegenerateZero1);
		EXPECT_FALSE(this->hypersphereZeroRadius2 == this->hypersphereDegenerateNegative2);
		EXPECT_FALSE(this->hypersphereZeroRadius2 == this->hypersphereDegenerateZero2);
		EXPECT_FALSE(this->hypersphereZeroRadius2 == this->hypersphereNegativeRadius1);
		EXPECT_FALSE(this->hypersphereZeroRadius2 == this->hypersphereZeroRadius1);
		EXPECT_FALSE(this->hypersphereZeroRadius2 == this->hyperspherePositiveRadius1);
		EXPECT_FALSE(this->hypersphereZeroRadius2 == this->hypersphereNegativeRadius2);
		EXPECT_TRUE(this->hypersphereZeroRadius2 == this->hypersphereZeroRadius2);
		EXPECT_FALSE(this->hypersphereZeroRadius2 == this->hyperspherePositiveRadius2);

		EXPECT_FALSE(this->hyperspherePositiveRadius2 == this->hypersphereDegenerateNegative1);
		EXPECT_FALSE(this->hyperspherePositiveRadius2 == this->hypersphereDegenerateZero1);
		EXPECT_FALSE(this->hyperspherePositiveRadius2 == this->hypersphereDegenerateNegative2);
		EXPECT_FALSE(this->hyperspherePositiveRadius2 == this->hypersphereDegenerateZero2);
		EXPECT_FALSE(this->hyperspherePositiveRadius2 == this->hypersphereNegativeRadius1);
		EXPECT_FALSE(this->hyperspherePositiveRadius2 == this->hypersphereZeroRadius1);
		EXPECT_FALSE(this->hyperspherePositiveRadius2 == this->hyperspherePositiveRadius1);
		EXPECT_FALSE(this->hyperspherePositiveRadius2 == this->hypersphereNegativeRadius2);
		EXPECT_FALSE(this->hyperspherePositiveRadius2 == this->hypersphereZeroRadius2);
		EXPECT_TRUE(this->hyperspherePositiveRadius2 == this->hyperspherePositiveRadius2);
	}

	TYPED_TEST_P(GeometryHypersphereTest, ComparisonOperatorGreaterEqualTo)
	{
		EXPECT_TRUE(this->hypersphereDegenerateNegative1 >= this->hypersphereDegenerateNegative1);
		EXPECT_FALSE(this->hypersphereDegenerateNegative1 >= this->hypersphereDegenerateZero1);
		EXPECT_FALSE(this->hypersphereDegenerateNegative1 >= this->hypersphereDegenerateNegative2);
		EXPECT_FALSE(this->hypersphereDegenerateNegative1 >= this->hypersphereDegenerateZero2);
		EXPECT_FALSE(this->hypersphereDegenerateNegative1 >= this->hypersphereNegativeRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateNegative1 >= this->hypersphereZeroRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateNegative1 >= this->hyperspherePositiveRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateNegative1 >= this->hypersphereNegativeRadius2);
		EXPECT_FALSE(this->hypersphereDegenerateNegative1 >= this->hypersphereZeroRadius2);
		EXPECT_FALSE(this->hypersphereDegenerateNegative1 >= this->hyperspherePositiveRadius2);

		EXPECT_TRUE(this->hypersphereDegenerateZero1 >= this->hypersphereDegenerateNegative1);
		EXPECT_TRUE(this->hypersphereDegenerateZero1 >= this->hypersphereDegenerateZero1);
		EXPECT_FALSE(this->hypersphereDegenerateZero1 >= this->hypersphereDegenerateNegative2);
		EXPECT_FALSE(this->hypersphereDegenerateZero1 >= this->hypersphereDegenerateZero2);
		EXPECT_FALSE(this->hypersphereDegenerateZero1 >= this->hypersphereNegativeRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateZero1 >= this->hypersphereZeroRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateZero1 >= this->hyperspherePositiveRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateZero1 >= this->hypersphereNegativeRadius2);
		EXPECT_FALSE(this->hypersphereDegenerateZero1 >= this->hypersphereZeroRadius2);
		EXPECT_FALSE(this->hypersphereDegenerateZero1 >= this->hyperspherePositiveRadius2);

		EXPECT_TRUE(this->hypersphereDegenerateNegative2 >= this->hypersphereDegenerateNegative1);
		EXPECT_TRUE(this->hypersphereDegenerateNegative2 >= this->hypersphereDegenerateZero1);
		EXPECT_TRUE(this->hypersphereDegenerateNegative2 >= this->hypersphereDegenerateNegative2);
		EXPECT_FALSE(this->hypersphereDegenerateNegative2 >= this->hypersphereDegenerateZero2);
		EXPECT_FALSE(this->hypersphereDegenerateNegative2 >= this->hypersphereNegativeRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateNegative2 >= this->hypersphereZeroRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateNegative2 >= this->hyperspherePositiveRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateNegative2 >= this->hypersphereNegativeRadius2);
		EXPECT_FALSE(this->hypersphereDegenerateNegative2 >= this->hypersphereZeroRadius2);
		EXPECT_FALSE(this->hypersphereDegenerateNegative2 >= this->hyperspherePositiveRadius2);

		EXPECT_TRUE(this->hypersphereDegenerateZero2 >= this->hypersphereDegenerateNegative1);
		EXPECT_TRUE(this->hypersphereDegenerateZero2 >= this->hypersphereDegenerateZero1);
		EXPECT_TRUE(this->hypersphereDegenerateZero2 >= this->hypersphereDegenerateNegative2);
		EXPECT_TRUE(this->hypersphereDegenerateZero2 >= this->hypersphereDegenerateZero2);
		EXPECT_FALSE(this->hypersphereDegenerateZero2 >= this->hypersphereNegativeRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateZero2 >= this->hypersphereZeroRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateZero2 >= this->hyperspherePositiveRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateZero2 >= this->hypersphereNegativeRadius2);
		EXPECT_FALSE(this->hypersphereDegenerateZero2 >= this->hypersphereZeroRadius2);
		EXPECT_FALSE(this->hypersphereDegenerateZero2 >= this->hyperspherePositiveRadius2);

		EXPECT_TRUE(this->hypersphereNegativeRadius1 >= this->hypersphereDegenerateNegative1);
		EXPECT_TRUE(this->hypersphereNegativeRadius1 >= this->hypersphereDegenerateZero1);
		EXPECT_TRUE(this->hypersphereNegativeRadius1 >= this->hypersphereDegenerateNegative2);
		EXPECT_TRUE(this->hypersphereNegativeRadius1 >= this->hypersphereDegenerateZero2);
		EXPECT_TRUE(this->hypersphereNegativeRadius1 >= this->hypersphereNegativeRadius1);
		EXPECT_FALSE(this->hypersphereNegativeRadius1 >= this->hypersphereZeroRadius1);
		EXPECT_FALSE(this->hypersphereNegativeRadius1 >= this->hyperspherePositiveRadius1);
		EXPECT_FALSE(this->hypersphereNegativeRadius1 >= this->hypersphereNegativeRadius2);
		EXPECT_FALSE(this->hypersphereNegativeRadius1 >= this->hypersphereZeroRadius2);
		EXPECT_FALSE(this->hypersphereNegativeRadius1 >= this->hyperspherePositiveRadius2);

		EXPECT_TRUE(this->hypersphereZeroRadius1 >= this->hypersphereDegenerateNegative1);
		EXPECT_TRUE(this->hypersphereZeroRadius1 >= this->hypersphereDegenerateZero1);
		EXPECT_TRUE(this->hypersphereZeroRadius1 >= this->hypersphereDegenerateNegative2);
		EXPECT_TRUE(this->hypersphereZeroRadius1 >= this->hypersphereDegenerateZero2);
		EXPECT_TRUE(this->hypersphereZeroRadius1 >= this->hypersphereNegativeRadius1);
		EXPECT_TRUE(this->hypersphereZeroRadius1 >= this->hypersphereZeroRadius1);
		EXPECT_FALSE(this->hypersphereZeroRadius1 >= this->hyperspherePositiveRadius1);
		EXPECT_FALSE(this->hypersphereZeroRadius1 >= this->hypersphereNegativeRadius2);
		EXPECT_FALSE(this->hypersphereZeroRadius1 >= this->hypersphereZeroRadius2);
		EXPECT_FALSE(this->hypersphereZeroRadius1 >= this->hyperspherePositiveRadius2);

		EXPECT_TRUE(this->hyperspherePositiveRadius1 >= this->hypersphereDegenerateNegative1);
		EXPECT_TRUE(this->hyperspherePositiveRadius1 >= this->hypersphereDegenerateZero1);
		EXPECT_TRUE(this->hyperspherePositiveRadius1 >= this->hypersphereDegenerateNegative2);
		EXPECT_TRUE(this->hyperspherePositiveRadius1 >= this->hypersphereDegenerateZero2);
		EXPECT_TRUE(this->hyperspherePositiveRadius1 >= this->hypersphereNegativeRadius1);
		EXPECT_TRUE(this->hyperspherePositiveRadius1 >= this->hypersphereZeroRadius1);
		EXPECT_TRUE(this->hyperspherePositiveRadius1 >= this->hyperspherePositiveRadius1);
		EXPECT_FALSE(this->hyperspherePositiveRadius1 >= this->hypersphereNegativeRadius2);
		EXPECT_FALSE(this->hyperspherePositiveRadius1 >= this->hypersphereZeroRadius2);
		EXPECT_FALSE(this->hyperspherePositiveRadius1 >= this->hyperspherePositiveRadius2);

		EXPECT_TRUE(this->hypersphereNegativeRadius2 >= this->hypersphereDegenerateNegative1);
		EXPECT_TRUE(this->hypersphereNegativeRadius2 >= this->hypersphereDegenerateZero1);
		EXPECT_TRUE(this->hypersphereNegativeRadius2 >= this->hypersphereDegenerateNegative2);
		EXPECT_TRUE(this->hypersphereNegativeRadius2 >= this->hypersphereDegenerateZero2);
		EXPECT_TRUE(this->hypersphereNegativeRadius2 >= this->hypersphereNegativeRadius1);
		EXPECT_TRUE(this->hypersphereNegativeRadius2 >= this->hypersphereZeroRadius1);
		EXPECT_TRUE(this->hypersphereNegativeRadius2 >= this->hyperspherePositiveRadius1);
		EXPECT_TRUE(this->hypersphereNegativeRadius2 >= this->hypersphereNegativeRadius2);
		EXPECT_FALSE(this->hypersphereNegativeRadius2 >= this->hypersphereZeroRadius2);
		EXPECT_FALSE(this->hypersphereNegativeRadius2 >= this->hyperspherePositiveRadius2);

		EXPECT_TRUE(this->hypersphereZeroRadius2 >= this->hypersphereDegenerateNegative1);
		EXPECT_TRUE(this->hypersphereZeroRadius2 >= this->hypersphereDegenerateZero1);
		EXPECT_TRUE(this->hypersphereZeroRadius2 >= this->hypersphereDegenerateNegative2);
		EXPECT_TRUE(this->hypersphereZeroRadius2 >= this->hypersphereDegenerateZero2);
		EXPECT_TRUE(this->hypersphereZeroRadius2 >= this->hypersphereNegativeRadius1);
		EXPECT_TRUE(this->hypersphereZeroRadius2 >= this->hypersphereZeroRadius1);
		EXPECT_TRUE(this->hypersphereZeroRadius2 >= this->hyperspherePositiveRadius1);
		EXPECT_TRUE(this->hypersphereZeroRadius2 >= this->hypersphereNegativeRadius2);
		EXPECT_TRUE(this->hypersphereZeroRadius2 >= this->hypersphereZeroRadius2);
		EXPECT_FALSE(this->hypersphereZeroRadius2 >= this->hyperspherePositiveRadius2);

		EXPECT_TRUE(this->hyperspherePositiveRadius2 >= this->hypersphereDegenerateNegative1);
		EXPECT_TRUE(this->hyperspherePositiveRadius2 >= this->hypersphereDegenerateZero1);
		EXPECT_TRUE(this->hyperspherePositiveRadius2 >= this->hypersphereDegenerateNegative2);
		EXPECT_TRUE(this->hyperspherePositiveRadius2 >= this->hypersphereDegenerateZero2);
		EXPECT_TRUE(this->hyperspherePositiveRadius2 >= this->hypersphereNegativeRadius1);
		EXPECT_TRUE(this->hyperspherePositiveRadius2 >= this->hypersphereZeroRadius1);
		EXPECT_TRUE(this->hyperspherePositiveRadius2 >= this->hyperspherePositiveRadius1);
		EXPECT_TRUE(this->hyperspherePositiveRadius2 >= this->hypersphereNegativeRadius2);
		EXPECT_TRUE(this->hyperspherePositiveRadius2 >= this->hypersphereZeroRadius2);
		EXPECT_TRUE(this->hyperspherePositiveRadius2 >= this->hyperspherePositiveRadius2);
	}

	TYPED_TEST_P(GeometryHypersphereTest, ComparisonOperatorGreater)
	{
		EXPECT_FALSE(this->hypersphereDegenerateNegative1 > this->hypersphereDegenerateNegative1);
		EXPECT_FALSE(this->hypersphereDegenerateNegative1 > this->hypersphereDegenerateZero1);
		EXPECT_FALSE(this->hypersphereDegenerateNegative1 > this->hypersphereDegenerateNegative2);
		EXPECT_FALSE(this->hypersphereDegenerateNegative1 > this->hypersphereDegenerateZero2);
		EXPECT_FALSE(this->hypersphereDegenerateNegative1 > this->hypersphereNegativeRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateNegative1 > this->hypersphereZeroRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateNegative1 > this->hyperspherePositiveRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateNegative1 > this->hypersphereNegativeRadius2);
		EXPECT_FALSE(this->hypersphereDegenerateNegative1 > this->hypersphereZeroRadius2);
		EXPECT_FALSE(this->hypersphereDegenerateNegative1 > this->hyperspherePositiveRadius2);

		EXPECT_TRUE(this->hypersphereDegenerateZero1 > this->hypersphereDegenerateNegative1);
		EXPECT_FALSE(this->hypersphereDegenerateZero1 > this->hypersphereDegenerateZero1);
		EXPECT_FALSE(this->hypersphereDegenerateZero1 > this->hypersphereDegenerateNegative2);
		EXPECT_FALSE(this->hypersphereDegenerateZero1 > this->hypersphereDegenerateZero2);
		EXPECT_FALSE(this->hypersphereDegenerateZero1 > this->hypersphereNegativeRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateZero1 > this->hypersphereZeroRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateZero1 > this->hyperspherePositiveRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateZero1 > this->hypersphereNegativeRadius2);
		EXPECT_FALSE(this->hypersphereDegenerateZero1 > this->hypersphereZeroRadius2);
		EXPECT_FALSE(this->hypersphereDegenerateZero1 > this->hyperspherePositiveRadius2);

		EXPECT_TRUE(this->hypersphereDegenerateNegative2 > this->hypersphereDegenerateNegative1);
		EXPECT_TRUE(this->hypersphereDegenerateNegative2 > this->hypersphereDegenerateZero1);
		EXPECT_FALSE(this->hypersphereDegenerateNegative2 > this->hypersphereDegenerateNegative2);
		EXPECT_FALSE(this->hypersphereDegenerateNegative2 > this->hypersphereDegenerateZero2);
		EXPECT_FALSE(this->hypersphereDegenerateNegative2 > this->hypersphereNegativeRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateNegative2 > this->hypersphereZeroRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateNegative2 > this->hyperspherePositiveRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateNegative2 > this->hypersphereNegativeRadius2);
		EXPECT_FALSE(this->hypersphereDegenerateNegative2 > this->hypersphereZeroRadius2);
		EXPECT_FALSE(this->hypersphereDegenerateNegative2 > this->hyperspherePositiveRadius2);

		EXPECT_TRUE(this->hypersphereDegenerateZero2 > this->hypersphereDegenerateNegative1);
		EXPECT_TRUE(this->hypersphereDegenerateZero2 > this->hypersphereDegenerateZero1);
		EXPECT_TRUE(this->hypersphereDegenerateZero2 > this->hypersphereDegenerateNegative2);
		EXPECT_FALSE(this->hypersphereDegenerateZero2 > this->hypersphereDegenerateZero2);
		EXPECT_FALSE(this->hypersphereDegenerateZero2 > this->hypersphereNegativeRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateZero2 > this->hypersphereZeroRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateZero2 > this->hyperspherePositiveRadius1);
		EXPECT_FALSE(this->hypersphereDegenerateZero2 > this->hypersphereNegativeRadius2);
		EXPECT_FALSE(this->hypersphereDegenerateZero2 > this->hypersphereZeroRadius2);
		EXPECT_FALSE(this->hypersphereDegenerateZero2 > this->hyperspherePositiveRadius2);

		EXPECT_TRUE(this->hypersphereNegativeRadius1 > this->hypersphereDegenerateNegative1);
		EXPECT_TRUE(this->hypersphereNegativeRadius1 > this->hypersphereDegenerateZero1);
		EXPECT_TRUE(this->hypersphereNegativeRadius1 > this->hypersphereDegenerateNegative2);
		EXPECT_TRUE(this->hypersphereNegativeRadius1 > this->hypersphereDegenerateZero2);
		EXPECT_FALSE(this->hypersphereNegativeRadius1 > this->hypersphereNegativeRadius1);
		EXPECT_FALSE(this->hypersphereNegativeRadius1 > this->hypersphereZeroRadius1);
		EXPECT_FALSE(this->hypersphereNegativeRadius1 > this->hyperspherePositiveRadius1);
		EXPECT_FALSE(this->hypersphereNegativeRadius1 > this->hypersphereNegativeRadius2);
		EXPECT_FALSE(this->hypersphereNegativeRadius1 > this->hypersphereZeroRadius2);
		EXPECT_FALSE(this->hypersphereNegativeRadius1 > this->hyperspherePositiveRadius2);

		EXPECT_TRUE(this->hypersphereZeroRadius1 > this->hypersphereDegenerateNegative1);
		EXPECT_TRUE(this->hypersphereZeroRadius1 > this->hypersphereDegenerateZero1);
		EXPECT_TRUE(this->hypersphereZeroRadius1 > this->hypersphereDegenerateNegative2);
		EXPECT_TRUE(this->hypersphereZeroRadius1 > this->hypersphereDegenerateZero2);
		EXPECT_TRUE(this->hypersphereZeroRadius1 > this->hypersphereNegativeRadius1);
		EXPECT_FALSE(this->hypersphereZeroRadius1 > this->hypersphereZeroRadius1);
		EXPECT_FALSE(this->hypersphereZeroRadius1 > this->hyperspherePositiveRadius1);
		EXPECT_FALSE(this->hypersphereZeroRadius1 > this->hypersphereNegativeRadius2);
		EXPECT_FALSE(this->hypersphereZeroRadius1 > this->hypersphereZeroRadius2);
		EXPECT_FALSE(this->hypersphereZeroRadius1 > this->hyperspherePositiveRadius2);

		EXPECT_TRUE(this->hyperspherePositiveRadius1 > this->hypersphereDegenerateNegative1);
		EXPECT_TRUE(this->hyperspherePositiveRadius1 > this->hypersphereDegenerateZero1);
		EXPECT_TRUE(this->hyperspherePositiveRadius1 > this->hypersphereDegenerateNegative2);
		EXPECT_TRUE(this->hyperspherePositiveRadius1 > this->hypersphereDegenerateZero2);
		EXPECT_TRUE(this->hyperspherePositiveRadius1 > this->hypersphereNegativeRadius1);
		EXPECT_TRUE(this->hyperspherePositiveRadius1 > this->hypersphereZeroRadius1);
		EXPECT_FALSE(this->hyperspherePositiveRadius1 > this->hyperspherePositiveRadius1);
		EXPECT_FALSE(this->hyperspherePositiveRadius1 > this->hypersphereNegativeRadius2);
		EXPECT_FALSE(this->hyperspherePositiveRadius1 > this->hypersphereZeroRadius2);
		EXPECT_FALSE(this->hyperspherePositiveRadius1 > this->hyperspherePositiveRadius2);

		EXPECT_TRUE(this->hypersphereNegativeRadius2 > this->hypersphereDegenerateNegative1);
		EXPECT_TRUE(this->hypersphereNegativeRadius2 > this->hypersphereDegenerateZero1);
		EXPECT_TRUE(this->hypersphereNegativeRadius2 > this->hypersphereDegenerateNegative2);
		EXPECT_TRUE(this->hypersphereNegativeRadius2 > this->hypersphereDegenerateZero2);
		EXPECT_TRUE(this->hypersphereNegativeRadius2 > this->hypersphereNegativeRadius1);
		EXPECT_TRUE(this->hypersphereNegativeRadius2 > this->hypersphereZeroRadius1);
		EXPECT_TRUE(this->hypersphereNegativeRadius2 > this->hyperspherePositiveRadius1);
		EXPECT_FALSE(this->hypersphereNegativeRadius2 > this->hypersphereNegativeRadius2);
		EXPECT_FALSE(this->hypersphereNegativeRadius2 > this->hypersphereZeroRadius2);
		EXPECT_FALSE(this->hypersphereNegativeRadius2 > this->hyperspherePositiveRadius2);

		EXPECT_TRUE(this->hypersphereZeroRadius2 > this->hypersphereDegenerateNegative1);
		EXPECT_TRUE(this->hypersphereZeroRadius2 > this->hypersphereDegenerateZero1);
		EXPECT_TRUE(this->hypersphereZeroRadius2 > this->hypersphereDegenerateNegative2);
		EXPECT_TRUE(this->hypersphereZeroRadius2 > this->hypersphereDegenerateZero2);
		EXPECT_TRUE(this->hypersphereZeroRadius2 > this->hypersphereNegativeRadius1);
		EXPECT_TRUE(this->hypersphereZeroRadius2 > this->hypersphereZeroRadius1);
		EXPECT_TRUE(this->hypersphereZeroRadius2 > this->hyperspherePositiveRadius1);
		EXPECT_TRUE(this->hypersphereZeroRadius2 > this->hypersphereNegativeRadius2);
		EXPECT_FALSE(this->hypersphereZeroRadius2 > this->hypersphereZeroRadius2);
		EXPECT_FALSE(this->hypersphereZeroRadius2 > this->hyperspherePositiveRadius2);

		EXPECT_TRUE(this->hyperspherePositiveRadius2 > this->hypersphereDegenerateNegative1);
		EXPECT_TRUE(this->hyperspherePositiveRadius2 > this->hypersphereDegenerateZero1);
		EXPECT_TRUE(this->hyperspherePositiveRadius2 > this->hypersphereDegenerateNegative2);
		EXPECT_TRUE(this->hyperspherePositiveRadius2 > this->hypersphereDegenerateZero2);
		EXPECT_TRUE(this->hyperspherePositiveRadius2 > this->hypersphereNegativeRadius1);
		EXPECT_TRUE(this->hyperspherePositiveRadius2 > this->hypersphereZeroRadius1);
		EXPECT_TRUE(this->hyperspherePositiveRadius2 > this->hyperspherePositiveRadius1);
		EXPECT_TRUE(this->hyperspherePositiveRadius2 > this->hypersphereNegativeRadius2);
		EXPECT_TRUE(this->hyperspherePositiveRadius2 > this->hypersphereZeroRadius2);
		EXPECT_FALSE(this->hyperspherePositiveRadius2 > this->hyperspherePositiveRadius2);
	}

	TYPED_TEST_P(GeometryHypersphereTest, ComparisonOperatorNotEqualTo)
	{
		EXPECT_FALSE(this->hypersphereDegenerateNegative1 != this->hypersphereDegenerateNegative1);
		EXPECT_TRUE(this->hypersphereDegenerateNegative1 != this->hypersphereDegenerateZero1);
		EXPECT_TRUE(this->hypersphereDegenerateNegative1 != this->hypersphereDegenerateNegative2);
		EXPECT_TRUE(this->hypersphereDegenerateNegative1 != this->hypersphereDegenerateZero2);
		EXPECT_TRUE(this->hypersphereDegenerateNegative1 != this->hypersphereNegativeRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateNegative1 != this->hypersphereZeroRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateNegative1 != this->hyperspherePositiveRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateNegative1 != this->hypersphereNegativeRadius2);
		EXPECT_TRUE(this->hypersphereDegenerateNegative1 != this->hypersphereZeroRadius2);
		EXPECT_TRUE(this->hypersphereDegenerateNegative1 != this->hyperspherePositiveRadius2);

		EXPECT_TRUE(this->hypersphereDegenerateZero1 != this->hypersphereDegenerateNegative1);
		EXPECT_FALSE(this->hypersphereDegenerateZero1 != this->hypersphereDegenerateZero1);
		EXPECT_TRUE(this->hypersphereDegenerateZero1 != this->hypersphereDegenerateNegative2);
		EXPECT_TRUE(this->hypersphereDegenerateZero1 != this->hypersphereDegenerateZero2);
		EXPECT_TRUE(this->hypersphereDegenerateZero1 != this->hypersphereNegativeRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateZero1 != this->hypersphereZeroRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateZero1 != this->hyperspherePositiveRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateZero1 != this->hypersphereNegativeRadius2);
		EXPECT_TRUE(this->hypersphereDegenerateZero1 != this->hypersphereZeroRadius2);
		EXPECT_TRUE(this->hypersphereDegenerateZero1 != this->hyperspherePositiveRadius2);

		EXPECT_TRUE(this->hypersphereDegenerateNegative2 != this->hypersphereDegenerateNegative1);
		EXPECT_TRUE(this->hypersphereDegenerateNegative2 != this->hypersphereDegenerateZero1);
		EXPECT_FALSE(this->hypersphereDegenerateNegative2 != this->hypersphereDegenerateNegative2);
		EXPECT_TRUE(this->hypersphereDegenerateNegative2 != this->hypersphereDegenerateZero2);
		EXPECT_TRUE(this->hypersphereDegenerateNegative2 != this->hypersphereNegativeRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateNegative2 != this->hypersphereZeroRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateNegative2 != this->hyperspherePositiveRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateNegative2 != this->hypersphereNegativeRadius2);
		EXPECT_TRUE(this->hypersphereDegenerateNegative2 != this->hypersphereZeroRadius2);
		EXPECT_TRUE(this->hypersphereDegenerateNegative2 != this->hyperspherePositiveRadius2);

		EXPECT_TRUE(this->hypersphereDegenerateZero2 != this->hypersphereDegenerateNegative1);
		EXPECT_TRUE(this->hypersphereDegenerateZero2 != this->hypersphereDegenerateZero1);
		EXPECT_TRUE(this->hypersphereDegenerateZero2 != this->hypersphereDegenerateNegative2);
		EXPECT_FALSE(this->hypersphereDegenerateZero2 != this->hypersphereDegenerateZero2);
		EXPECT_TRUE(this->hypersphereDegenerateZero2 != this->hypersphereNegativeRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateZero2 != this->hypersphereZeroRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateZero2 != this->hyperspherePositiveRadius1);
		EXPECT_TRUE(this->hypersphereDegenerateZero2 != this->hypersphereNegativeRadius2);
		EXPECT_TRUE(this->hypersphereDegenerateZero2 != this->hypersphereZeroRadius2);
		EXPECT_TRUE(this->hypersphereDegenerateZero2 != this->hyperspherePositiveRadius2);

		EXPECT_TRUE(this->hypersphereNegativeRadius1 != this->hypersphereDegenerateNegative1);
		EXPECT_TRUE(this->hypersphereNegativeRadius1 != this->hypersphereDegenerateZero1);
		EXPECT_TRUE(this->hypersphereNegativeRadius1 != this->hypersphereDegenerateNegative2);
		EXPECT_TRUE(this->hypersphereNegativeRadius1 != this->hypersphereDegenerateZero2);
		EXPECT_FALSE(this->hypersphereNegativeRadius1 != this->hypersphereNegativeRadius1);
		EXPECT_TRUE(this->hypersphereNegativeRadius1 != this->hypersphereZeroRadius1);
		EXPECT_TRUE(this->hypersphereNegativeRadius1 != this->hyperspherePositiveRadius1);
		EXPECT_TRUE(this->hypersphereNegativeRadius1 != this->hypersphereNegativeRadius2);
		EXPECT_TRUE(this->hypersphereNegativeRadius1 != this->hypersphereZeroRadius2);
		EXPECT_TRUE(this->hypersphereNegativeRadius1 != this->hyperspherePositiveRadius2);

		EXPECT_TRUE(this->hypersphereZeroRadius1 != this->hypersphereDegenerateNegative1);
		EXPECT_TRUE(this->hypersphereZeroRadius1 != this->hypersphereDegenerateZero1);
		EXPECT_TRUE(this->hypersphereZeroRadius1 != this->hypersphereDegenerateNegative2);
		EXPECT_TRUE(this->hypersphereZeroRadius1 != this->hypersphereDegenerateZero2);
		EXPECT_TRUE(this->hypersphereZeroRadius1 != this->hypersphereNegativeRadius1);
		EXPECT_FALSE(this->hypersphereZeroRadius1 != this->hypersphereZeroRadius1);
		EXPECT_TRUE(this->hypersphereZeroRadius1 != this->hyperspherePositiveRadius1);
		EXPECT_TRUE(this->hypersphereZeroRadius1 != this->hypersphereNegativeRadius2);
		EXPECT_TRUE(this->hypersphereZeroRadius1 != this->hypersphereZeroRadius2);
		EXPECT_TRUE(this->hypersphereZeroRadius1 != this->hyperspherePositiveRadius2);

		EXPECT_TRUE(this->hyperspherePositiveRadius1 != this->hypersphereDegenerateNegative1);
		EXPECT_TRUE(this->hyperspherePositiveRadius1 != this->hypersphereDegenerateZero1);
		EXPECT_TRUE(this->hyperspherePositiveRadius1 != this->hypersphereDegenerateNegative2);
		EXPECT_TRUE(this->hyperspherePositiveRadius1 != this->hypersphereDegenerateZero2);
		EXPECT_TRUE(this->hyperspherePositiveRadius1 != this->hypersphereNegativeRadius1);
		EXPECT_TRUE(this->hyperspherePositiveRadius1 != this->hypersphereZeroRadius1);
		EXPECT_FALSE(this->hyperspherePositiveRadius1 != this->hyperspherePositiveRadius1);
		EXPECT_TRUE(this->hyperspherePositiveRadius1 != this->hypersphereNegativeRadius2);
		EXPECT_TRUE(this->hyperspherePositiveRadius1 != this->hypersphereZeroRadius2);
		EXPECT_TRUE(this->hyperspherePositiveRadius1 != this->hyperspherePositiveRadius2);

		EXPECT_TRUE(this->hypersphereNegativeRadius2 != this->hypersphereDegenerateNegative1);
		EXPECT_TRUE(this->hypersphereNegativeRadius2 != this->hypersphereDegenerateZero1);
		EXPECT_TRUE(this->hypersphereNegativeRadius2 != this->hypersphereDegenerateNegative2);
		EXPECT_TRUE(this->hypersphereNegativeRadius2 != this->hypersphereDegenerateZero2);
		EXPECT_TRUE(this->hypersphereNegativeRadius2 != this->hypersphereNegativeRadius1);
		EXPECT_TRUE(this->hypersphereNegativeRadius2 != this->hypersphereZeroRadius1);
		EXPECT_TRUE(this->hypersphereNegativeRadius2 != this->hyperspherePositiveRadius1);
		EXPECT_FALSE(this->hypersphereNegativeRadius2 != this->hypersphereNegativeRadius2);
		EXPECT_TRUE(this->hypersphereNegativeRadius2 != this->hypersphereZeroRadius2);
		EXPECT_TRUE(this->hypersphereNegativeRadius2 != this->hyperspherePositiveRadius2);

		EXPECT_TRUE(this->hypersphereZeroRadius2 != this->hypersphereDegenerateNegative1);
		EXPECT_TRUE(this->hypersphereZeroRadius2 != this->hypersphereDegenerateZero1);
		EXPECT_TRUE(this->hypersphereZeroRadius2 != this->hypersphereDegenerateNegative2);
		EXPECT_TRUE(this->hypersphereZeroRadius2 != this->hypersphereDegenerateZero2);
		EXPECT_TRUE(this->hypersphereZeroRadius2 != this->hypersphereNegativeRadius1);
		EXPECT_TRUE(this->hypersphereZeroRadius2 != this->hypersphereZeroRadius1);
		EXPECT_TRUE(this->hypersphereZeroRadius2 != this->hyperspherePositiveRadius1);
		EXPECT_TRUE(this->hypersphereZeroRadius2 != this->hypersphereNegativeRadius2);
		EXPECT_FALSE(this->hypersphereZeroRadius2 != this->hypersphereZeroRadius2);
		EXPECT_TRUE(this->hypersphereZeroRadius2 != this->hyperspherePositiveRadius2);

		EXPECT_TRUE(this->hyperspherePositiveRadius2 != this->hypersphereDegenerateNegative1);
		EXPECT_TRUE(this->hyperspherePositiveRadius2 != this->hypersphereDegenerateZero1);
		EXPECT_TRUE(this->hyperspherePositiveRadius2 != this->hypersphereDegenerateNegative2);
		EXPECT_TRUE(this->hyperspherePositiveRadius2 != this->hypersphereDegenerateZero2);
		EXPECT_TRUE(this->hyperspherePositiveRadius2 != this->hypersphereNegativeRadius1);
		EXPECT_TRUE(this->hyperspherePositiveRadius2 != this->hypersphereZeroRadius1);
		EXPECT_TRUE(this->hyperspherePositiveRadius2 != this->hyperspherePositiveRadius1);
		EXPECT_TRUE(this->hyperspherePositiveRadius2 != this->hypersphereNegativeRadius2);
		EXPECT_TRUE(this->hyperspherePositiveRadius2 != this->hypersphereZeroRadius2);
		EXPECT_FALSE(this->hyperspherePositiveRadius2 != this->hyperspherePositiveRadius2);
	}

	TYPED_TEST_P(GeometryHypersphereTest, SphericalDistance)
	{
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius1.spherical_distance(this->vertexPositiveX, this->vertexNegativeX), std::numbers::pi_v < typename TypeParam::value_type >));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius1.spherical_distance(this->vertexNegativeX, this->vertexPositiveX), std::numbers::pi_v < typename TypeParam::value_type >));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius1.spherical_distance(this->vertexPositiveX, this->vertexNegativeX), std::numbers::pi_v < typename TypeParam::value_type >));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius1.spherical_distance(this->vertexNegativeX, this->vertexPositiveX), std::numbers::pi_v < typename TypeParam::value_type >));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius1.spherical_distance(this->vertexPositiveX, this->vertexPositiveY), std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius1.spherical_distance(this->vertexNegativeX, this->vertexNegativeY), std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius1.spherical_distance(this->vertexPositiveX, this->vertexPositiveZ), std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius1.spherical_distance(this->vertexNegativeX, this->vertexNegativeZ), std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius1.spherical_distance(this->vertexPositiveX, this->vertexNegativeZ), std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius1.spherical_distance(this->vertexNegativeX, this->vertexPositiveZ), std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2}));

		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius2.spherical_distance(this->vertexPositiveDoubleX, this->vertexNegativeDoubleX), typename TypeParam::value_type{2} * std::numbers::pi_v < typename TypeParam::value_type >));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius2.spherical_distance(this->vertexNegativeDoubleX, this->vertexPositiveDoubleX), typename TypeParam::value_type{2} * std::numbers::pi_v < typename TypeParam::value_type >));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius2.spherical_distance(this->vertexPositiveDoubleX, this->vertexNegativeDoubleX), typename TypeParam::value_type{2} * std::numbers::pi_v < typename TypeParam::value_type >));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius2.spherical_distance(this->vertexNegativeDoubleX, this->vertexPositiveDoubleX), typename TypeParam::value_type{2} * std::numbers::pi_v < typename TypeParam::value_type >));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius2.spherical_distance(this->vertexPositiveDoubleX, this->vertexPositiveDoubleY), typename TypeParam::value_type{2} * std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius2.spherical_distance(this->vertexNegativeDoubleX, this->vertexNegativeDoubleY), typename TypeParam::value_type{2} * std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius2.spherical_distance(this->vertexPositiveDoubleX, this->vertexPositiveDoubleZ), typename TypeParam::value_type{2} * std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius2.spherical_distance(this->vertexNegativeDoubleX, this->vertexNegativeDoubleZ), typename TypeParam::value_type{2} * std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius2.spherical_distance(this->vertexPositiveDoubleX, this->vertexNegativeDoubleZ), typename TypeParam::value_type{2} * std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius2.spherical_distance(this->vertexNegativeDoubleX, this->vertexPositiveDoubleZ), typename TypeParam::value_type{2} * std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2}));
	}

	TYPED_TEST_P(GeometryHypersphereTest, Area)
	{
		const std::array < typename TypeParam::value_type, 10 > areas =
			{
				typename TypeParam::value_type{0},
				typename TypeParam::value_type{2},
				typename TypeParam::value_type{2} * std::numbers::pi_v < typename TypeParam::value_type >,
				typename TypeParam::value_type{4} * std::numbers::pi_v < typename TypeParam::value_type >,
				typename TypeParam::value_type{2} * std::pow(std::numbers::pi_v < typename TypeParam::value_type >, typename TypeParam::value_type{2}),
				(typename TypeParam::value_type{8} / typename TypeParam::value_type{3}) * std::pow(std::numbers::pi_v < typename TypeParam::value_type >, typename TypeParam::value_type{2}),
				std::pow(std::numbers::pi_v < typename TypeParam::value_type >, typename TypeParam::value_type{3}),
				(typename TypeParam::value_type{16} / typename TypeParam::value_type{15}) * std::pow(std::numbers::pi_v < typename TypeParam::value_type >, typename TypeParam::value_type{3}),
				(typename TypeParam::value_type{1} / typename TypeParam::value_type{3}) * std::pow(std::numbers::pi_v < typename TypeParam::value_type >, typename TypeParam::value_type{4}),
				(typename TypeParam::value_type{32} / typename TypeParam::value_type{105}) * std::pow(std::numbers::pi_v < typename TypeParam::value_type >, typename TypeParam::value_type{4})
			};

		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius1.template area < 0 > (), areas[0]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius1.template area < 1 > (), areas[1]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius1.template area < 2 > (), areas[2]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius1.template area < 3 > (), areas[3]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius1.template area < 4 > (), areas[4]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius1.template area < 5 > (), areas[5]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius1.template area < 6 > (), areas[6]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius1.template area < 7 > (), areas[7]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius1.template area < 8 > (), areas[8]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius1.template area < 9 > (), areas[9]));

		const std::array < typename TypeParam::value_type, 10 > radiusMultiplier =
			{
				typename TypeParam::value_type{1},
				typename TypeParam::value_type{1},
				this->hypersphereZeroRadius2.radius,
				std::pow(this->hypersphereZeroRadius2.radius, typename TypeParam::value_type{2}),
				std::pow(this->hypersphereZeroRadius2.radius, typename TypeParam::value_type{3}),
				std::pow(this->hypersphereZeroRadius2.radius, typename TypeParam::value_type{4}),
				std::pow(this->hypersphereZeroRadius2.radius, typename TypeParam::value_type{5}),
				std::pow(this->hypersphereZeroRadius2.radius, typename TypeParam::value_type{6}),
				std::pow(this->hypersphereZeroRadius2.radius, typename TypeParam::value_type{7}),
				std::pow(this->hypersphereZeroRadius2.radius, typename TypeParam::value_type{8}),
			};

		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius2.template area < 0 > (), areas[0] * radiusMultiplier[0]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius2.template area < 1 > (), areas[1] * radiusMultiplier[1]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius2.template area < 2 > (), areas[2] * radiusMultiplier[2]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius2.template area < 3 > (), areas[3] * radiusMultiplier[3]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius2.template area < 4 > (), areas[4] * radiusMultiplier[4]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius2.template area < 5 > (), areas[5] * radiusMultiplier[5]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius2.template area < 6 > (), areas[6] * radiusMultiplier[6]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius2.template area < 7 > (), areas[7] * radiusMultiplier[7]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius2.template area < 8 > (), areas[8] * radiusMultiplier[8]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius2.template area < 9 > (), areas[9] * radiusMultiplier[9]));
	}

	TYPED_TEST_P(GeometryHypersphereTest, Volume)
	{
		const std::array < typename TypeParam::value_type, 10 > volumes =
			{
				typename TypeParam::value_type{0},
				typename TypeParam::value_type{2},
				std::numbers::pi_v < typename TypeParam::value_type >,
				(typename TypeParam::value_type{4} / typename TypeParam::value_type{3}) * std::numbers::pi_v < typename TypeParam::value_type >,
				(typename TypeParam::value_type{1} / typename TypeParam::value_type{2}) * std::pow(std::numbers::pi_v < typename TypeParam::value_type >, typename TypeParam::value_type{2}),
				(typename TypeParam::value_type{8} / typename TypeParam::value_type{15}) * std::pow(std::numbers::pi_v < typename TypeParam::value_type >, typename TypeParam::value_type{2}),
				(typename TypeParam::value_type{1} / typename TypeParam::value_type{6}) * std::pow(std::numbers::pi_v < typename TypeParam::value_type >, typename TypeParam::value_type{3}),
				(typename TypeParam::value_type{16} / typename TypeParam::value_type{105}) * std::pow(std::numbers::pi_v < typename TypeParam::value_type >, typename TypeParam::value_type{3}),
				(typename TypeParam::value_type{1} / typename TypeParam::value_type{24}) * std::pow(std::numbers::pi_v < typename TypeParam::value_type >, typename TypeParam::value_type{4}),
				(typename TypeParam::value_type{32} / typename TypeParam::value_type{945}) * std::pow(std::numbers::pi_v < typename TypeParam::value_type >, typename TypeParam::value_type{4})
			};

		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius1.template volume < 0 > (), volumes[0]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius1.template volume < 1 > (), volumes[1]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius1.template volume < 2 > (), volumes[2]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius1.template volume < 3 > (), volumes[3]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius1.template volume < 4 > (), volumes[4]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius1.template volume < 5 > (), volumes[5]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius1.template volume < 6 > (), volumes[6]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius1.template volume < 7 > (), volumes[7]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius1.template volume < 8 > (), volumes[8]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius1.template volume < 9 > (), volumes[9]));

		const std::array < typename TypeParam::value_type, 10 > radiusMultiplier =
			{
				typename TypeParam::value_type{1},
				this->hypersphereZeroRadius2.radius,
				std::pow(this->hypersphereZeroRadius2.radius, typename TypeParam::value_type{2}),
				std::pow(this->hypersphereZeroRadius2.radius, typename TypeParam::value_type{3}),
				std::pow(this->hypersphereZeroRadius2.radius, typename TypeParam::value_type{4}),
				std::pow(this->hypersphereZeroRadius2.radius, typename TypeParam::value_type{5}),
				std::pow(this->hypersphereZeroRadius2.radius, typename TypeParam::value_type{6}),
				std::pow(this->hypersphereZeroRadius2.radius, typename TypeParam::value_type{7}),
				std::pow(this->hypersphereZeroRadius2.radius, typename TypeParam::value_type{8}),
				std::pow(this->hypersphereZeroRadius2.radius, typename TypeParam::value_type{9}),
			};

		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius2.template volume < 0 > (), volumes[0] * radiusMultiplier[0]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius2.template volume < 1 > (), volumes[1] * radiusMultiplier[1]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius2.template volume < 2 > (), volumes[2] * radiusMultiplier[2]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius2.template volume < 3 > (), volumes[3] * radiusMultiplier[3]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius2.template volume < 4 > (), volumes[4] * radiusMultiplier[4]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius2.template volume < 5 > (), volumes[5] * radiusMultiplier[5]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius2.template volume < 6 > (), volumes[6] * radiusMultiplier[6]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius2.template volume < 7 > (), volumes[7] * radiusMultiplier[7]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius2.template volume < 8 > (), volumes[8] * radiusMultiplier[8]));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->hypersphereZeroRadius2.template volume < 9 > (), volumes[9] * radiusMultiplier[9]));
	}

	REGISTER_TYPED_TEST_SUITE_P(
		GeometryHypersphereTest,
		ComparisonOperatorLess,
		ComparisonOperatorLessEqualTo,
		ComparisonOperatorEqualTo,
		ComparisonOperatorGreaterEqualTo,
		ComparisonOperatorGreater,
		ComparisonOperatorNotEqualTo,
		SphericalDistance,
		Area,
		Volume);

	using GeometryHypersphereTestDataTypes = std::tuple < float, double, long double >;

	using GeometryHypersphereTestGroup =
		xablau::testing::test_bundler < GeometryHypersphereTestDataTypes > ::template test_group < GeometryHypersphereTypeDefinitions >;

	using GeometryHypersphereTypes = xablau::testing::tuple_transformation < ::testing::Types, GeometryHypersphereTestGroup > ::types;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		Xablau,
		GeometryHypersphereTest,
		GeometryHypersphereTypes);
}
