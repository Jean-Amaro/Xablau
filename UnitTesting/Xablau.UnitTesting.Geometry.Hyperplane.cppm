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

import xablau.geometry;
import xablau.testing;

namespace xablau::unit_testing
{
	template < xablau::geometry::concepts::geometric_assignable_arithmetic Type >
	struct GeometryHyperplaneTypeDefinitions
	{
		using value_type = Type;
	};

	template < typename Types >
	class GeometryHyperplaneTest : public ::testing::Test
	{
	public:
		static constexpr xablau::geometry::hyperplane < typename Types::value_type, xablau::geometry::spatial_dimension < 1 > > hyperplaneDegenerateNegative{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename Types::value_type(-1)
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename Types::value_type()
				}))};

		static constexpr xablau::geometry::hyperplane < typename Types::value_type, xablau::geometry::spatial_dimension < 0 > > hyperplaneDegenerateZero;

		static constexpr xablau::geometry::hyperplane < typename Types::value_type, xablau::geometry::spatial_dimension < 1 > > hyperplaneDegeneratePositive{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename Types::value_type(1)
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename Types::value_type()
				}))};

		static constexpr xablau::geometry::hyperplane < typename Types::value_type, xablau::geometry::spatial_dimension < 1 > > hyperplaneDirectX{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename Types::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename Types::value_type(1)
				}))};

		static constexpr xablau::geometry::hyperplane < typename Types::value_type, xablau::geometry::spatial_dimension < 1 > > hyperplaneReverseX{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename Types::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename Types::value_type(-1)
				}))};

		static constexpr xablau::geometry::hyperplane < typename Types::value_type, xablau::geometry::spatial_dimension < 2 > > hyperplaneDirectY{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename Types::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename Types::value_type(),
					typename Types::value_type(1)
				}))};

		static constexpr xablau::geometry::hyperplane < typename Types::value_type, xablau::geometry::spatial_dimension < 2 > > hyperplaneReverseY{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename Types::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename Types::value_type(),
					typename Types::value_type(-1)
				}))};

		static constexpr xablau::geometry::hyperplane < typename Types::value_type, xablau::geometry::spatial_dimension < 3 > > hyperplaneDirectZ{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename Types::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename Types::value_type(),
					typename Types::value_type(),
					typename Types::value_type(1)
				}))};

		static constexpr xablau::geometry::hyperplane < typename Types::value_type, xablau::geometry::spatial_dimension < 3 > > hyperplaneReverseZ{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename Types::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename Types::value_type(),
					typename Types::value_type(),
					typename Types::value_type(-1)
				}))};
	};

	TYPED_TEST_SUITE_P(GeometryHyperplaneTest);

	TYPED_TEST_P(GeometryHyperplaneTest, ComparisonOperatorLess)
	{
		EXPECT_FALSE(this->hyperplaneDegenerateNegative < this->hyperplaneDegenerateNegative);
		EXPECT_TRUE(this->hyperplaneDegenerateNegative < this->hyperplaneDegenerateZero);
		EXPECT_TRUE(this->hyperplaneDegenerateNegative < this->hyperplaneDegeneratePositive);
		EXPECT_TRUE(this->hyperplaneDegenerateNegative < this->hyperplaneDirectX);
		EXPECT_TRUE(this->hyperplaneDegenerateNegative < this->hyperplaneReverseX);
		EXPECT_TRUE(this->hyperplaneDegenerateNegative < this->hyperplaneDirectY);
		EXPECT_TRUE(this->hyperplaneDegenerateNegative < this->hyperplaneReverseY);
		EXPECT_TRUE(this->hyperplaneDegenerateNegative < this->hyperplaneDirectZ);
		EXPECT_TRUE(this->hyperplaneDegenerateNegative < this->hyperplaneReverseZ);

		EXPECT_FALSE(this->hyperplaneDegenerateZero < this->hyperplaneDegenerateNegative);
		EXPECT_FALSE(this->hyperplaneDegenerateZero < this->hyperplaneDegenerateZero);
		EXPECT_TRUE(this->hyperplaneDegenerateZero < this->hyperplaneDegeneratePositive);
		EXPECT_TRUE(this->hyperplaneDegenerateZero < this->hyperplaneDirectX);
		EXPECT_TRUE(this->hyperplaneDegenerateZero < this->hyperplaneReverseX);
		EXPECT_TRUE(this->hyperplaneDegenerateZero < this->hyperplaneDirectY);
		EXPECT_TRUE(this->hyperplaneDegenerateZero < this->hyperplaneReverseY);
		EXPECT_TRUE(this->hyperplaneDegenerateZero < this->hyperplaneDirectZ);
		EXPECT_TRUE(this->hyperplaneDegenerateZero < this->hyperplaneReverseZ);

		EXPECT_FALSE(this->hyperplaneDegeneratePositive < this->hyperplaneDegenerateNegative);
		EXPECT_FALSE(this->hyperplaneDegeneratePositive < this->hyperplaneDegenerateZero);
		EXPECT_FALSE(this->hyperplaneDegeneratePositive < this->hyperplaneDegeneratePositive);
		EXPECT_TRUE(this->hyperplaneDegeneratePositive < this->hyperplaneDirectX);
		EXPECT_TRUE(this->hyperplaneDegeneratePositive < this->hyperplaneReverseX);
		EXPECT_TRUE(this->hyperplaneDegeneratePositive < this->hyperplaneDirectY);
		EXPECT_TRUE(this->hyperplaneDegeneratePositive < this->hyperplaneReverseY);
		EXPECT_TRUE(this->hyperplaneDegeneratePositive < this->hyperplaneDirectZ);
		EXPECT_TRUE(this->hyperplaneDegeneratePositive < this->hyperplaneReverseZ);

		EXPECT_FALSE(this->hyperplaneDirectX < this->hyperplaneDegenerateNegative);
		EXPECT_FALSE(this->hyperplaneDirectX < this->hyperplaneDegenerateZero);
		EXPECT_FALSE(this->hyperplaneDirectX < this->hyperplaneDegeneratePositive);
		EXPECT_FALSE(this->hyperplaneDirectX < this->hyperplaneDirectX);
		EXPECT_FALSE(this->hyperplaneDirectX < this->hyperplaneReverseX);
		EXPECT_FALSE(this->hyperplaneDirectX < this->hyperplaneDirectY);
		EXPECT_FALSE(this->hyperplaneDirectX < this->hyperplaneReverseY);
		EXPECT_FALSE(this->hyperplaneDirectX < this->hyperplaneDirectZ);
		EXPECT_FALSE(this->hyperplaneDirectX < this->hyperplaneReverseZ);

		EXPECT_FALSE(this->hyperplaneReverseX < this->hyperplaneDegenerateNegative);
		EXPECT_FALSE(this->hyperplaneReverseX < this->hyperplaneDegenerateZero);
		EXPECT_FALSE(this->hyperplaneReverseX < this->hyperplaneDegeneratePositive);
		EXPECT_TRUE(this->hyperplaneReverseX < this->hyperplaneDirectX);
		EXPECT_FALSE(this->hyperplaneReverseX < this->hyperplaneReverseX);
		EXPECT_TRUE(this->hyperplaneReverseX < this->hyperplaneDirectY);
		EXPECT_TRUE(this->hyperplaneReverseX < this->hyperplaneReverseY);
		EXPECT_TRUE(this->hyperplaneReverseX < this->hyperplaneDirectZ);
		EXPECT_TRUE(this->hyperplaneReverseX < this->hyperplaneReverseZ);

		EXPECT_FALSE(this->hyperplaneDirectY < this->hyperplaneDegenerateNegative);
		EXPECT_FALSE(this->hyperplaneDirectY < this->hyperplaneDegenerateZero);
		EXPECT_FALSE(this->hyperplaneDirectY < this->hyperplaneDegeneratePositive);
		EXPECT_TRUE(this->hyperplaneDirectY < this->hyperplaneDirectX);
		EXPECT_FALSE(this->hyperplaneDirectY < this->hyperplaneReverseX);
		EXPECT_FALSE(this->hyperplaneDirectY < this->hyperplaneDirectY);
		EXPECT_FALSE(this->hyperplaneDirectY < this->hyperplaneReverseY);
		EXPECT_FALSE(this->hyperplaneDirectY < this->hyperplaneDirectZ);
		EXPECT_FALSE(this->hyperplaneDirectY < this->hyperplaneReverseZ);

		EXPECT_FALSE(this->hyperplaneReverseY < this->hyperplaneDegenerateNegative);
		EXPECT_FALSE(this->hyperplaneReverseY < this->hyperplaneDegenerateZero);
		EXPECT_FALSE(this->hyperplaneReverseY < this->hyperplaneDegeneratePositive);
		EXPECT_TRUE(this->hyperplaneReverseY < this->hyperplaneDirectX);
		EXPECT_FALSE(this->hyperplaneReverseY < this->hyperplaneReverseX);
		EXPECT_TRUE(this->hyperplaneReverseY < this->hyperplaneDirectY);
		EXPECT_FALSE(this->hyperplaneReverseY < this->hyperplaneReverseY);
		EXPECT_TRUE(this->hyperplaneReverseY < this->hyperplaneDirectZ);
		EXPECT_TRUE(this->hyperplaneReverseY < this->hyperplaneReverseZ);

		EXPECT_FALSE(this->hyperplaneDirectZ < this->hyperplaneDegenerateNegative);
		EXPECT_FALSE(this->hyperplaneDirectZ < this->hyperplaneDegenerateZero);
		EXPECT_FALSE(this->hyperplaneDirectZ < this->hyperplaneDegeneratePositive);
		EXPECT_TRUE(this->hyperplaneDirectZ < this->hyperplaneDirectX);
		EXPECT_FALSE(this->hyperplaneDirectZ < this->hyperplaneReverseX);
		EXPECT_TRUE(this->hyperplaneDirectZ < this->hyperplaneDirectY);
		EXPECT_FALSE(this->hyperplaneDirectZ < this->hyperplaneReverseY);
		EXPECT_FALSE(this->hyperplaneDirectZ < this->hyperplaneDirectZ);
		EXPECT_FALSE(this->hyperplaneDirectZ < this->hyperplaneReverseZ);

		EXPECT_FALSE(this->hyperplaneReverseZ < this->hyperplaneDegenerateNegative);
		EXPECT_FALSE(this->hyperplaneReverseZ < this->hyperplaneDegenerateZero);
		EXPECT_FALSE(this->hyperplaneReverseZ < this->hyperplaneDegeneratePositive);
		EXPECT_TRUE(this->hyperplaneReverseZ < this->hyperplaneDirectX);
		EXPECT_FALSE(this->hyperplaneReverseZ < this->hyperplaneReverseX);
		EXPECT_TRUE(this->hyperplaneReverseZ < this->hyperplaneDirectY);
		EXPECT_FALSE(this->hyperplaneReverseZ < this->hyperplaneReverseY);
		EXPECT_TRUE(this->hyperplaneReverseZ < this->hyperplaneDirectZ);
		EXPECT_FALSE(this->hyperplaneReverseZ < this->hyperplaneReverseZ);
	}

	TYPED_TEST_P(GeometryHyperplaneTest, ComparisonOperatorLessEqualTo)
	{
		EXPECT_TRUE(this->hyperplaneDegenerateNegative <= this->hyperplaneDegenerateNegative);
		EXPECT_TRUE(this->hyperplaneDegenerateNegative <= this->hyperplaneDegenerateZero);
		EXPECT_TRUE(this->hyperplaneDegenerateNegative <= this->hyperplaneDegeneratePositive);
		EXPECT_TRUE(this->hyperplaneDegenerateNegative <= this->hyperplaneDirectX);
		EXPECT_TRUE(this->hyperplaneDegenerateNegative <= this->hyperplaneReverseX);
		EXPECT_TRUE(this->hyperplaneDegenerateNegative <= this->hyperplaneDirectY);
		EXPECT_TRUE(this->hyperplaneDegenerateNegative <= this->hyperplaneReverseY);
		EXPECT_TRUE(this->hyperplaneDegenerateNegative <= this->hyperplaneDirectZ);
		EXPECT_TRUE(this->hyperplaneDegenerateNegative <= this->hyperplaneReverseZ);

		EXPECT_FALSE(this->hyperplaneDegenerateZero <= this->hyperplaneDegenerateNegative);
		EXPECT_TRUE(this->hyperplaneDegenerateZero <= this->hyperplaneDegenerateZero);
		EXPECT_TRUE(this->hyperplaneDegenerateZero <= this->hyperplaneDegeneratePositive);
		EXPECT_TRUE(this->hyperplaneDegenerateZero <= this->hyperplaneDirectX);
		EXPECT_TRUE(this->hyperplaneDegenerateZero <= this->hyperplaneReverseX);
		EXPECT_TRUE(this->hyperplaneDegenerateZero <= this->hyperplaneDirectY);
		EXPECT_TRUE(this->hyperplaneDegenerateZero <= this->hyperplaneReverseY);
		EXPECT_TRUE(this->hyperplaneDegenerateZero <= this->hyperplaneDirectZ);
		EXPECT_TRUE(this->hyperplaneDegenerateZero <= this->hyperplaneReverseZ);

		EXPECT_FALSE(this->hyperplaneDegeneratePositive <= this->hyperplaneDegenerateNegative);
		EXPECT_FALSE(this->hyperplaneDegeneratePositive <= this->hyperplaneDegenerateZero);
		EXPECT_TRUE(this->hyperplaneDegeneratePositive <= this->hyperplaneDegeneratePositive);
		EXPECT_TRUE(this->hyperplaneDegeneratePositive <= this->hyperplaneDirectX);
		EXPECT_TRUE(this->hyperplaneDegeneratePositive <= this->hyperplaneReverseX);
		EXPECT_TRUE(this->hyperplaneDegeneratePositive <= this->hyperplaneDirectY);
		EXPECT_TRUE(this->hyperplaneDegeneratePositive <= this->hyperplaneReverseY);
		EXPECT_TRUE(this->hyperplaneDegeneratePositive <= this->hyperplaneDirectZ);
		EXPECT_TRUE(this->hyperplaneDegeneratePositive <= this->hyperplaneReverseZ);

		EXPECT_FALSE(this->hyperplaneDirectX <= this->hyperplaneDegenerateNegative);
		EXPECT_FALSE(this->hyperplaneDirectX <= this->hyperplaneDegenerateZero);
		EXPECT_FALSE(this->hyperplaneDirectX <= this->hyperplaneDegeneratePositive);
		EXPECT_TRUE(this->hyperplaneDirectX <= this->hyperplaneDirectX);
		EXPECT_FALSE(this->hyperplaneDirectX <= this->hyperplaneReverseX);
		EXPECT_FALSE(this->hyperplaneDirectX <= this->hyperplaneDirectY);
		EXPECT_FALSE(this->hyperplaneDirectX <= this->hyperplaneReverseY);
		EXPECT_FALSE(this->hyperplaneDirectX <= this->hyperplaneDirectZ);
		EXPECT_FALSE(this->hyperplaneDirectX <= this->hyperplaneReverseZ);

		EXPECT_FALSE(this->hyperplaneReverseX <= this->hyperplaneDegenerateNegative);
		EXPECT_FALSE(this->hyperplaneReverseX <= this->hyperplaneDegenerateZero);
		EXPECT_FALSE(this->hyperplaneReverseX <= this->hyperplaneDegeneratePositive);
		EXPECT_TRUE(this->hyperplaneReverseX <= this->hyperplaneDirectX);
		EXPECT_TRUE(this->hyperplaneReverseX <= this->hyperplaneReverseX);
		EXPECT_TRUE(this->hyperplaneReverseX <= this->hyperplaneDirectY);
		EXPECT_TRUE(this->hyperplaneReverseX <= this->hyperplaneReverseY);
		EXPECT_TRUE(this->hyperplaneReverseX <= this->hyperplaneDirectZ);
		EXPECT_TRUE(this->hyperplaneReverseX <= this->hyperplaneReverseZ);

		EXPECT_FALSE(this->hyperplaneDirectY <= this->hyperplaneDegenerateNegative);
		EXPECT_FALSE(this->hyperplaneDirectY <= this->hyperplaneDegenerateZero);
		EXPECT_FALSE(this->hyperplaneDirectY <= this->hyperplaneDegeneratePositive);
		EXPECT_TRUE(this->hyperplaneDirectY <= this->hyperplaneDirectX);
		EXPECT_FALSE(this->hyperplaneDirectY <= this->hyperplaneReverseX);
		EXPECT_TRUE(this->hyperplaneDirectY <= this->hyperplaneDirectY);
		EXPECT_FALSE(this->hyperplaneDirectY <= this->hyperplaneReverseY);
		EXPECT_FALSE(this->hyperplaneDirectY <= this->hyperplaneDirectZ);
		EXPECT_FALSE(this->hyperplaneDirectY <= this->hyperplaneReverseZ);

		EXPECT_FALSE(this->hyperplaneReverseY <= this->hyperplaneDegenerateNegative);
		EXPECT_FALSE(this->hyperplaneReverseY <= this->hyperplaneDegenerateZero);
		EXPECT_FALSE(this->hyperplaneReverseY <= this->hyperplaneDegeneratePositive);
		EXPECT_TRUE(this->hyperplaneReverseY <= this->hyperplaneDirectX);
		EXPECT_FALSE(this->hyperplaneReverseY <= this->hyperplaneReverseX);
		EXPECT_TRUE(this->hyperplaneReverseY <= this->hyperplaneDirectY);
		EXPECT_TRUE(this->hyperplaneReverseY <= this->hyperplaneReverseY);
		EXPECT_TRUE(this->hyperplaneReverseY <= this->hyperplaneDirectZ);
		EXPECT_TRUE(this->hyperplaneReverseY <= this->hyperplaneReverseZ);

		EXPECT_FALSE(this->hyperplaneDirectZ <= this->hyperplaneDegenerateNegative);
		EXPECT_FALSE(this->hyperplaneDirectZ <= this->hyperplaneDegenerateZero);
		EXPECT_FALSE(this->hyperplaneDirectZ <= this->hyperplaneDegeneratePositive);
		EXPECT_TRUE(this->hyperplaneDirectZ <= this->hyperplaneDirectX);
		EXPECT_FALSE(this->hyperplaneDirectZ <= this->hyperplaneReverseX);
		EXPECT_TRUE(this->hyperplaneDirectZ <= this->hyperplaneDirectY);
		EXPECT_FALSE(this->hyperplaneDirectZ <= this->hyperplaneReverseY);
		EXPECT_TRUE(this->hyperplaneDirectZ <= this->hyperplaneDirectZ);
		EXPECT_FALSE(this->hyperplaneDirectZ <= this->hyperplaneReverseZ);

		EXPECT_FALSE(this->hyperplaneReverseZ <= this->hyperplaneDegenerateNegative);
		EXPECT_FALSE(this->hyperplaneReverseZ <= this->hyperplaneDegenerateZero);
		EXPECT_FALSE(this->hyperplaneReverseZ <= this->hyperplaneDegeneratePositive);
		EXPECT_TRUE(this->hyperplaneReverseZ <= this->hyperplaneDirectX);
		EXPECT_FALSE(this->hyperplaneReverseZ <= this->hyperplaneReverseX);
		EXPECT_TRUE(this->hyperplaneReverseZ <= this->hyperplaneDirectY);
		EXPECT_FALSE(this->hyperplaneReverseZ <= this->hyperplaneReverseY);
		EXPECT_TRUE(this->hyperplaneReverseZ <= this->hyperplaneDirectZ);
		EXPECT_TRUE(this->hyperplaneReverseZ <= this->hyperplaneReverseZ);
	}

	TYPED_TEST_P(GeometryHyperplaneTest, ComparisonOperatorEqualTo)
	{
		EXPECT_TRUE(this->hyperplaneDegenerateNegative == this->hyperplaneDegenerateNegative);
		EXPECT_FALSE(this->hyperplaneDegenerateNegative == this->hyperplaneDegenerateZero);
		EXPECT_FALSE(this->hyperplaneDegenerateNegative == this->hyperplaneDegeneratePositive);
		EXPECT_FALSE(this->hyperplaneDegenerateNegative == this->hyperplaneDirectX);
		EXPECT_FALSE(this->hyperplaneDegenerateNegative == this->hyperplaneReverseX);
		EXPECT_FALSE(this->hyperplaneDegenerateNegative == this->hyperplaneDirectY);
		EXPECT_FALSE(this->hyperplaneDegenerateNegative == this->hyperplaneReverseY);
		EXPECT_FALSE(this->hyperplaneDegenerateNegative == this->hyperplaneDirectZ);
		EXPECT_FALSE(this->hyperplaneDegenerateNegative == this->hyperplaneReverseZ);

		EXPECT_FALSE(this->hyperplaneDegenerateZero == this->hyperplaneDegenerateNegative);
		EXPECT_TRUE(this->hyperplaneDegenerateZero == this->hyperplaneDegenerateZero);
		EXPECT_FALSE(this->hyperplaneDegenerateZero == this->hyperplaneDegeneratePositive);
		EXPECT_FALSE(this->hyperplaneDegenerateZero == this->hyperplaneDirectX);
		EXPECT_FALSE(this->hyperplaneDegenerateZero == this->hyperplaneReverseX);
		EXPECT_FALSE(this->hyperplaneDegenerateZero == this->hyperplaneDirectY);
		EXPECT_FALSE(this->hyperplaneDegenerateZero == this->hyperplaneReverseY);
		EXPECT_FALSE(this->hyperplaneDegenerateZero == this->hyperplaneDirectZ);
		EXPECT_FALSE(this->hyperplaneDegenerateZero == this->hyperplaneReverseZ);

		EXPECT_FALSE(this->hyperplaneDegeneratePositive == this->hyperplaneDegenerateNegative);
		EXPECT_FALSE(this->hyperplaneDegeneratePositive == this->hyperplaneDegenerateZero);
		EXPECT_TRUE(this->hyperplaneDegeneratePositive == this->hyperplaneDegeneratePositive);
		EXPECT_FALSE(this->hyperplaneDegeneratePositive == this->hyperplaneDirectX);
		EXPECT_FALSE(this->hyperplaneDegeneratePositive == this->hyperplaneReverseX);
		EXPECT_FALSE(this->hyperplaneDegeneratePositive == this->hyperplaneDirectY);
		EXPECT_FALSE(this->hyperplaneDegeneratePositive == this->hyperplaneReverseY);
		EXPECT_FALSE(this->hyperplaneDegeneratePositive == this->hyperplaneDirectZ);
		EXPECT_FALSE(this->hyperplaneDegeneratePositive == this->hyperplaneReverseZ);

		EXPECT_FALSE(this->hyperplaneDirectX == this->hyperplaneDegenerateNegative);
		EXPECT_FALSE(this->hyperplaneDirectX == this->hyperplaneDegenerateZero);
		EXPECT_FALSE(this->hyperplaneDirectX == this->hyperplaneDegeneratePositive);
		EXPECT_TRUE(this->hyperplaneDirectX == this->hyperplaneDirectX);
		EXPECT_FALSE(this->hyperplaneDirectX == this->hyperplaneReverseX);
		EXPECT_FALSE(this->hyperplaneDirectX == this->hyperplaneDirectY);
		EXPECT_FALSE(this->hyperplaneDirectX == this->hyperplaneReverseY);
		EXPECT_FALSE(this->hyperplaneDirectX == this->hyperplaneDirectZ);
		EXPECT_FALSE(this->hyperplaneDirectX == this->hyperplaneReverseZ);

		EXPECT_FALSE(this->hyperplaneReverseX == this->hyperplaneDegenerateNegative);
		EXPECT_FALSE(this->hyperplaneReverseX == this->hyperplaneDegenerateZero);
		EXPECT_FALSE(this->hyperplaneReverseX == this->hyperplaneDegeneratePositive);
		EXPECT_FALSE(this->hyperplaneReverseX == this->hyperplaneDirectX);
		EXPECT_TRUE(this->hyperplaneReverseX == this->hyperplaneReverseX);
		EXPECT_FALSE(this->hyperplaneReverseX == this->hyperplaneDirectY);
		EXPECT_FALSE(this->hyperplaneReverseX == this->hyperplaneReverseY);
		EXPECT_FALSE(this->hyperplaneReverseX == this->hyperplaneDirectZ);
		EXPECT_FALSE(this->hyperplaneReverseX == this->hyperplaneReverseZ);

		EXPECT_FALSE(this->hyperplaneDirectY == this->hyperplaneDegenerateNegative);
		EXPECT_FALSE(this->hyperplaneDirectY == this->hyperplaneDegenerateZero);
		EXPECT_FALSE(this->hyperplaneDirectY == this->hyperplaneDegeneratePositive);
		EXPECT_FALSE(this->hyperplaneDirectY == this->hyperplaneDirectX);
		EXPECT_FALSE(this->hyperplaneDirectY == this->hyperplaneReverseX);
		EXPECT_TRUE(this->hyperplaneDirectY == this->hyperplaneDirectY);
		EXPECT_FALSE(this->hyperplaneDirectY == this->hyperplaneReverseY);
		EXPECT_FALSE(this->hyperplaneDirectY == this->hyperplaneDirectZ);
		EXPECT_FALSE(this->hyperplaneDirectY == this->hyperplaneReverseZ);

		EXPECT_FALSE(this->hyperplaneReverseY == this->hyperplaneDegenerateNegative);
		EXPECT_FALSE(this->hyperplaneReverseY == this->hyperplaneDegenerateZero);
		EXPECT_FALSE(this->hyperplaneReverseY == this->hyperplaneDegeneratePositive);
		EXPECT_FALSE(this->hyperplaneReverseY == this->hyperplaneDirectX);
		EXPECT_FALSE(this->hyperplaneReverseY == this->hyperplaneReverseX);
		EXPECT_FALSE(this->hyperplaneReverseY == this->hyperplaneDirectY);
		EXPECT_TRUE(this->hyperplaneReverseY == this->hyperplaneReverseY);
		EXPECT_FALSE(this->hyperplaneReverseY == this->hyperplaneDirectZ);
		EXPECT_FALSE(this->hyperplaneReverseY == this->hyperplaneReverseZ);

		EXPECT_FALSE(this->hyperplaneDirectZ == this->hyperplaneDegenerateNegative);
		EXPECT_FALSE(this->hyperplaneDirectZ == this->hyperplaneDegenerateZero);
		EXPECT_FALSE(this->hyperplaneDirectZ == this->hyperplaneDegeneratePositive);
		EXPECT_FALSE(this->hyperplaneDirectZ == this->hyperplaneDirectX);
		EXPECT_FALSE(this->hyperplaneDirectZ == this->hyperplaneReverseX);
		EXPECT_FALSE(this->hyperplaneDirectZ == this->hyperplaneDirectY);
		EXPECT_FALSE(this->hyperplaneDirectZ == this->hyperplaneReverseY);
		EXPECT_TRUE(this->hyperplaneDirectZ == this->hyperplaneDirectZ);
		EXPECT_FALSE(this->hyperplaneDirectZ == this->hyperplaneReverseZ);

		EXPECT_FALSE(this->hyperplaneReverseZ == this->hyperplaneDegenerateNegative);
		EXPECT_FALSE(this->hyperplaneReverseZ == this->hyperplaneDegenerateZero);
		EXPECT_FALSE(this->hyperplaneReverseZ == this->hyperplaneDegeneratePositive);
		EXPECT_FALSE(this->hyperplaneReverseZ == this->hyperplaneDirectX);
		EXPECT_FALSE(this->hyperplaneReverseZ == this->hyperplaneReverseX);
		EXPECT_FALSE(this->hyperplaneReverseZ == this->hyperplaneDirectY);
		EXPECT_FALSE(this->hyperplaneReverseZ == this->hyperplaneReverseY);
		EXPECT_FALSE(this->hyperplaneReverseZ == this->hyperplaneDirectZ);
		EXPECT_TRUE(this->hyperplaneReverseZ == this->hyperplaneReverseZ);
	}

	TYPED_TEST_P(GeometryHyperplaneTest, ComparisonOperatorGreaterEqualTo)
	{
		EXPECT_TRUE(this->hyperplaneDegenerateNegative >= this->hyperplaneDegenerateNegative);
		EXPECT_FALSE(this->hyperplaneDegenerateNegative >= this->hyperplaneDegenerateZero);
		EXPECT_FALSE(this->hyperplaneDegenerateNegative >= this->hyperplaneDegeneratePositive);
		EXPECT_FALSE(this->hyperplaneDegenerateNegative >= this->hyperplaneDirectX);
		EXPECT_FALSE(this->hyperplaneDegenerateNegative >= this->hyperplaneReverseX);
		EXPECT_FALSE(this->hyperplaneDegenerateNegative >= this->hyperplaneDirectY);
		EXPECT_FALSE(this->hyperplaneDegenerateNegative >= this->hyperplaneReverseY);
		EXPECT_FALSE(this->hyperplaneDegenerateNegative >= this->hyperplaneDirectZ);
		EXPECT_FALSE(this->hyperplaneDegenerateNegative >= this->hyperplaneReverseZ);

		EXPECT_TRUE(this->hyperplaneDegenerateZero >= this->hyperplaneDegenerateNegative);
		EXPECT_TRUE(this->hyperplaneDegenerateZero >= this->hyperplaneDegenerateZero);
		EXPECT_FALSE(this->hyperplaneDegenerateZero >= this->hyperplaneDegeneratePositive);
		EXPECT_FALSE(this->hyperplaneDegenerateZero >= this->hyperplaneDirectX);
		EXPECT_FALSE(this->hyperplaneDegenerateZero >= this->hyperplaneReverseX);
		EXPECT_FALSE(this->hyperplaneDegenerateZero >= this->hyperplaneDirectY);
		EXPECT_FALSE(this->hyperplaneDegenerateZero >= this->hyperplaneReverseY);
		EXPECT_FALSE(this->hyperplaneDegenerateZero >= this->hyperplaneDirectZ);
		EXPECT_FALSE(this->hyperplaneDegenerateZero >= this->hyperplaneReverseZ);

		EXPECT_TRUE(this->hyperplaneDegeneratePositive >= this->hyperplaneDegenerateNegative);
		EXPECT_TRUE(this->hyperplaneDegeneratePositive >= this->hyperplaneDegenerateZero);
		EXPECT_TRUE(this->hyperplaneDegeneratePositive >= this->hyperplaneDegeneratePositive);
		EXPECT_FALSE(this->hyperplaneDegeneratePositive >= this->hyperplaneDirectX);
		EXPECT_FALSE(this->hyperplaneDegeneratePositive >= this->hyperplaneReverseX);
		EXPECT_FALSE(this->hyperplaneDegeneratePositive >= this->hyperplaneDirectY);
		EXPECT_FALSE(this->hyperplaneDegeneratePositive >= this->hyperplaneReverseY);
		EXPECT_FALSE(this->hyperplaneDegeneratePositive >= this->hyperplaneDirectZ);
		EXPECT_FALSE(this->hyperplaneDegeneratePositive >= this->hyperplaneReverseZ);

		EXPECT_TRUE(this->hyperplaneDirectX >= this->hyperplaneDegenerateNegative);
		EXPECT_TRUE(this->hyperplaneDirectX >= this->hyperplaneDegenerateZero);
		EXPECT_TRUE(this->hyperplaneDirectX >= this->hyperplaneDegeneratePositive);
		EXPECT_TRUE(this->hyperplaneDirectX >= this->hyperplaneDirectX);
		EXPECT_TRUE(this->hyperplaneDirectX >= this->hyperplaneReverseX);
		EXPECT_TRUE(this->hyperplaneDirectX >= this->hyperplaneDirectY);
		EXPECT_TRUE(this->hyperplaneDirectX >= this->hyperplaneReverseY);
		EXPECT_TRUE(this->hyperplaneDirectX >= this->hyperplaneDirectZ);
		EXPECT_TRUE(this->hyperplaneDirectX >= this->hyperplaneReverseZ);

		EXPECT_TRUE(this->hyperplaneReverseX >= this->hyperplaneDegenerateNegative);
		EXPECT_TRUE(this->hyperplaneReverseX >= this->hyperplaneDegenerateZero);
		EXPECT_TRUE(this->hyperplaneReverseX >= this->hyperplaneDegeneratePositive);
		EXPECT_FALSE(this->hyperplaneReverseX >= this->hyperplaneDirectX);
		EXPECT_TRUE(this->hyperplaneReverseX >= this->hyperplaneReverseX);
		EXPECT_FALSE(this->hyperplaneReverseX >= this->hyperplaneDirectY);
		EXPECT_FALSE(this->hyperplaneReverseX >= this->hyperplaneReverseY);
		EXPECT_FALSE(this->hyperplaneReverseX >= this->hyperplaneDirectZ);
		EXPECT_FALSE(this->hyperplaneReverseX >= this->hyperplaneReverseZ);

		EXPECT_TRUE(this->hyperplaneDirectY >= this->hyperplaneDegenerateNegative);
		EXPECT_TRUE(this->hyperplaneDirectY >= this->hyperplaneDegenerateZero);
		EXPECT_TRUE(this->hyperplaneDirectY >= this->hyperplaneDegeneratePositive);
		EXPECT_FALSE(this->hyperplaneDirectY >= this->hyperplaneDirectX);
		EXPECT_TRUE(this->hyperplaneDirectY >= this->hyperplaneReverseX);
		EXPECT_TRUE(this->hyperplaneDirectY >= this->hyperplaneDirectY);
		EXPECT_TRUE(this->hyperplaneDirectY >= this->hyperplaneReverseY);
		EXPECT_TRUE(this->hyperplaneDirectY >= this->hyperplaneDirectZ);
		EXPECT_TRUE(this->hyperplaneDirectY >= this->hyperplaneReverseZ);

		EXPECT_TRUE(this->hyperplaneReverseY >= this->hyperplaneDegenerateNegative);
		EXPECT_TRUE(this->hyperplaneReverseY >= this->hyperplaneDegenerateZero);
		EXPECT_TRUE(this->hyperplaneReverseY >= this->hyperplaneDegeneratePositive);
		EXPECT_FALSE(this->hyperplaneReverseY >= this->hyperplaneDirectX);
		EXPECT_TRUE(this->hyperplaneReverseY >= this->hyperplaneReverseX);
		EXPECT_FALSE(this->hyperplaneReverseY >= this->hyperplaneDirectY);
		EXPECT_TRUE(this->hyperplaneReverseY >= this->hyperplaneReverseY);
		EXPECT_FALSE(this->hyperplaneReverseY >= this->hyperplaneDirectZ);
		EXPECT_FALSE(this->hyperplaneReverseY >= this->hyperplaneReverseZ);

		EXPECT_TRUE(this->hyperplaneDirectZ >= this->hyperplaneDegenerateNegative);
		EXPECT_TRUE(this->hyperplaneDirectZ >= this->hyperplaneDegenerateZero);
		EXPECT_TRUE(this->hyperplaneDirectZ >= this->hyperplaneDegeneratePositive);
		EXPECT_FALSE(this->hyperplaneDirectZ >= this->hyperplaneDirectX);
		EXPECT_TRUE(this->hyperplaneDirectZ >= this->hyperplaneReverseX);
		EXPECT_FALSE(this->hyperplaneDirectZ >= this->hyperplaneDirectY);
		EXPECT_TRUE(this->hyperplaneDirectZ >= this->hyperplaneReverseY);
		EXPECT_TRUE(this->hyperplaneDirectZ >= this->hyperplaneDirectZ);
		EXPECT_TRUE(this->hyperplaneDirectZ >= this->hyperplaneReverseZ);

		EXPECT_TRUE(this->hyperplaneReverseZ >= this->hyperplaneDegenerateNegative);
		EXPECT_TRUE(this->hyperplaneReverseZ >= this->hyperplaneDegenerateZero);
		EXPECT_TRUE(this->hyperplaneReverseZ >= this->hyperplaneDegeneratePositive);
		EXPECT_FALSE(this->hyperplaneReverseZ >= this->hyperplaneDirectX);
		EXPECT_TRUE(this->hyperplaneReverseZ >= this->hyperplaneReverseX);
		EXPECT_FALSE(this->hyperplaneReverseZ >= this->hyperplaneDirectY);
		EXPECT_TRUE(this->hyperplaneReverseZ >= this->hyperplaneReverseY);
		EXPECT_FALSE(this->hyperplaneReverseZ >= this->hyperplaneDirectZ);
		EXPECT_TRUE(this->hyperplaneReverseZ >= this->hyperplaneReverseZ);
	}

	TYPED_TEST_P(GeometryHyperplaneTest, ComparisonOperatorGreater)
	{
		EXPECT_FALSE(this->hyperplaneDegenerateNegative > this->hyperplaneDegenerateNegative);
		EXPECT_FALSE(this->hyperplaneDegenerateNegative > this->hyperplaneDegenerateZero);
		EXPECT_FALSE(this->hyperplaneDegenerateNegative > this->hyperplaneDegeneratePositive);
		EXPECT_FALSE(this->hyperplaneDegenerateNegative > this->hyperplaneDirectX);
		EXPECT_FALSE(this->hyperplaneDegenerateNegative > this->hyperplaneReverseX);
		EXPECT_FALSE(this->hyperplaneDegenerateNegative > this->hyperplaneDirectY);
		EXPECT_FALSE(this->hyperplaneDegenerateNegative > this->hyperplaneReverseY);
		EXPECT_FALSE(this->hyperplaneDegenerateNegative > this->hyperplaneDirectZ);
		EXPECT_FALSE(this->hyperplaneDegenerateNegative > this->hyperplaneReverseZ);

		EXPECT_TRUE(this->hyperplaneDegenerateZero > this->hyperplaneDegenerateNegative);
		EXPECT_FALSE(this->hyperplaneDegenerateZero > this->hyperplaneDegenerateZero);
		EXPECT_FALSE(this->hyperplaneDegenerateZero > this->hyperplaneDegeneratePositive);
		EXPECT_FALSE(this->hyperplaneDegenerateZero > this->hyperplaneDirectX);
		EXPECT_FALSE(this->hyperplaneDegenerateZero > this->hyperplaneReverseX);
		EXPECT_FALSE(this->hyperplaneDegenerateZero > this->hyperplaneDirectY);
		EXPECT_FALSE(this->hyperplaneDegenerateZero > this->hyperplaneReverseY);
		EXPECT_FALSE(this->hyperplaneDegenerateZero > this->hyperplaneDirectZ);
		EXPECT_FALSE(this->hyperplaneDegenerateZero > this->hyperplaneReverseZ);

		EXPECT_TRUE(this->hyperplaneDegeneratePositive > this->hyperplaneDegenerateNegative);
		EXPECT_TRUE(this->hyperplaneDegeneratePositive > this->hyperplaneDegenerateZero);
		EXPECT_FALSE(this->hyperplaneDegeneratePositive > this->hyperplaneDegeneratePositive);
		EXPECT_FALSE(this->hyperplaneDegeneratePositive > this->hyperplaneDirectX);
		EXPECT_FALSE(this->hyperplaneDegeneratePositive > this->hyperplaneReverseX);
		EXPECT_FALSE(this->hyperplaneDegeneratePositive > this->hyperplaneDirectY);
		EXPECT_FALSE(this->hyperplaneDegeneratePositive > this->hyperplaneReverseY);
		EXPECT_FALSE(this->hyperplaneDegeneratePositive > this->hyperplaneDirectZ);
		EXPECT_FALSE(this->hyperplaneDegeneratePositive > this->hyperplaneReverseZ);

		EXPECT_TRUE(this->hyperplaneDirectX > this->hyperplaneDegenerateNegative);
		EXPECT_TRUE(this->hyperplaneDirectX > this->hyperplaneDegenerateZero);
		EXPECT_TRUE(this->hyperplaneDirectX > this->hyperplaneDegeneratePositive);
		EXPECT_FALSE(this->hyperplaneDirectX > this->hyperplaneDirectX);
		EXPECT_TRUE(this->hyperplaneDirectX > this->hyperplaneReverseX);
		EXPECT_TRUE(this->hyperplaneDirectX > this->hyperplaneDirectY);
		EXPECT_TRUE(this->hyperplaneDirectX > this->hyperplaneReverseY);
		EXPECT_TRUE(this->hyperplaneDirectX > this->hyperplaneDirectZ);
		EXPECT_TRUE(this->hyperplaneDirectX > this->hyperplaneReverseZ);

		EXPECT_TRUE(this->hyperplaneReverseX > this->hyperplaneDegenerateNegative);
		EXPECT_TRUE(this->hyperplaneReverseX > this->hyperplaneDegenerateZero);
		EXPECT_TRUE(this->hyperplaneReverseX > this->hyperplaneDegeneratePositive);
		EXPECT_FALSE(this->hyperplaneReverseX > this->hyperplaneDirectX);
		EXPECT_FALSE(this->hyperplaneReverseX > this->hyperplaneReverseX);
		EXPECT_FALSE(this->hyperplaneReverseX > this->hyperplaneDirectY);
		EXPECT_FALSE(this->hyperplaneReverseX > this->hyperplaneReverseY);
		EXPECT_FALSE(this->hyperplaneReverseX > this->hyperplaneDirectZ);
		EXPECT_FALSE(this->hyperplaneReverseX > this->hyperplaneReverseZ);

		EXPECT_TRUE(this->hyperplaneDirectY > this->hyperplaneDegenerateNegative);
		EXPECT_TRUE(this->hyperplaneDirectY > this->hyperplaneDegenerateZero);
		EXPECT_TRUE(this->hyperplaneDirectY > this->hyperplaneDegeneratePositive);
		EXPECT_FALSE(this->hyperplaneDirectY > this->hyperplaneDirectX);
		EXPECT_TRUE(this->hyperplaneDirectY > this->hyperplaneReverseX);
		EXPECT_FALSE(this->hyperplaneDirectY > this->hyperplaneDirectY);
		EXPECT_TRUE(this->hyperplaneDirectY > this->hyperplaneReverseY);
		EXPECT_TRUE(this->hyperplaneDirectY > this->hyperplaneDirectZ);
		EXPECT_TRUE(this->hyperplaneDirectY > this->hyperplaneReverseZ);

		EXPECT_TRUE(this->hyperplaneReverseY > this->hyperplaneDegenerateNegative);
		EXPECT_TRUE(this->hyperplaneReverseY > this->hyperplaneDegenerateZero);
		EXPECT_TRUE(this->hyperplaneReverseY > this->hyperplaneDegeneratePositive);
		EXPECT_FALSE(this->hyperplaneReverseY > this->hyperplaneDirectX);
		EXPECT_TRUE(this->hyperplaneReverseY > this->hyperplaneReverseX);
		EXPECT_FALSE(this->hyperplaneReverseY > this->hyperplaneDirectY);
		EXPECT_FALSE(this->hyperplaneReverseY > this->hyperplaneReverseY);
		EXPECT_FALSE(this->hyperplaneReverseY > this->hyperplaneDirectZ);
		EXPECT_FALSE(this->hyperplaneReverseY > this->hyperplaneReverseZ);

		EXPECT_TRUE(this->hyperplaneDirectZ > this->hyperplaneDegenerateNegative);
		EXPECT_TRUE(this->hyperplaneDirectZ > this->hyperplaneDegenerateZero);
		EXPECT_TRUE(this->hyperplaneDirectZ > this->hyperplaneDegeneratePositive);
		EXPECT_FALSE(this->hyperplaneDirectZ > this->hyperplaneDirectX);
		EXPECT_TRUE(this->hyperplaneDirectZ > this->hyperplaneReverseX);
		EXPECT_FALSE(this->hyperplaneDirectZ > this->hyperplaneDirectY);
		EXPECT_TRUE(this->hyperplaneDirectZ > this->hyperplaneReverseY);
		EXPECT_FALSE(this->hyperplaneDirectZ > this->hyperplaneDirectZ);
		EXPECT_TRUE(this->hyperplaneDirectZ > this->hyperplaneReverseZ);

		EXPECT_TRUE(this->hyperplaneReverseZ > this->hyperplaneDegenerateNegative);
		EXPECT_TRUE(this->hyperplaneReverseZ > this->hyperplaneDegenerateZero);
		EXPECT_TRUE(this->hyperplaneReverseZ > this->hyperplaneDegeneratePositive);
		EXPECT_FALSE(this->hyperplaneReverseZ > this->hyperplaneDirectX);
		EXPECT_TRUE(this->hyperplaneReverseZ > this->hyperplaneReverseX);
		EXPECT_FALSE(this->hyperplaneReverseZ > this->hyperplaneDirectY);
		EXPECT_TRUE(this->hyperplaneReverseZ > this->hyperplaneReverseY);
		EXPECT_FALSE(this->hyperplaneReverseZ > this->hyperplaneDirectZ);
		EXPECT_FALSE(this->hyperplaneReverseZ > this->hyperplaneReverseZ);
	}

	TYPED_TEST_P(GeometryHyperplaneTest, ComparisonOperatorNotEqualTo)
	{
		EXPECT_FALSE(this->hyperplaneDegenerateNegative != this->hyperplaneDegenerateNegative);
		EXPECT_TRUE(this->hyperplaneDegenerateNegative != this->hyperplaneDegenerateZero);
		EXPECT_TRUE(this->hyperplaneDegenerateNegative != this->hyperplaneDegeneratePositive);
		EXPECT_TRUE(this->hyperplaneDegenerateNegative != this->hyperplaneDirectX);
		EXPECT_TRUE(this->hyperplaneDegenerateNegative != this->hyperplaneReverseX);
		EXPECT_TRUE(this->hyperplaneDegenerateNegative != this->hyperplaneDirectY);
		EXPECT_TRUE(this->hyperplaneDegenerateNegative != this->hyperplaneReverseY);
		EXPECT_TRUE(this->hyperplaneDegenerateNegative != this->hyperplaneDirectZ);
		EXPECT_TRUE(this->hyperplaneDegenerateNegative != this->hyperplaneReverseZ);

		EXPECT_TRUE(this->hyperplaneDegenerateZero != this->hyperplaneDegenerateNegative);
		EXPECT_FALSE(this->hyperplaneDegenerateZero != this->hyperplaneDegenerateZero);
		EXPECT_TRUE(this->hyperplaneDegenerateZero != this->hyperplaneDegeneratePositive);
		EXPECT_TRUE(this->hyperplaneDegenerateZero != this->hyperplaneDirectX);
		EXPECT_TRUE(this->hyperplaneDegenerateZero != this->hyperplaneReverseX);
		EXPECT_TRUE(this->hyperplaneDegenerateZero != this->hyperplaneDirectY);
		EXPECT_TRUE(this->hyperplaneDegenerateZero != this->hyperplaneReverseY);
		EXPECT_TRUE(this->hyperplaneDegenerateZero != this->hyperplaneDirectZ);
		EXPECT_TRUE(this->hyperplaneDegenerateZero != this->hyperplaneReverseZ);

		EXPECT_TRUE(this->hyperplaneDegeneratePositive != this->hyperplaneDegenerateNegative);
		EXPECT_TRUE(this->hyperplaneDegeneratePositive != this->hyperplaneDegenerateZero);
		EXPECT_FALSE(this->hyperplaneDegeneratePositive != this->hyperplaneDegeneratePositive);
		EXPECT_TRUE(this->hyperplaneDegeneratePositive != this->hyperplaneDirectX);
		EXPECT_TRUE(this->hyperplaneDegeneratePositive != this->hyperplaneReverseX);
		EXPECT_TRUE(this->hyperplaneDegeneratePositive != this->hyperplaneDirectY);
		EXPECT_TRUE(this->hyperplaneDegeneratePositive != this->hyperplaneReverseY);
		EXPECT_TRUE(this->hyperplaneDegeneratePositive != this->hyperplaneDirectZ);
		EXPECT_TRUE(this->hyperplaneDegeneratePositive != this->hyperplaneReverseZ);

		EXPECT_TRUE(this->hyperplaneDirectX != this->hyperplaneDegenerateNegative);
		EXPECT_TRUE(this->hyperplaneDirectX != this->hyperplaneDegenerateZero);
		EXPECT_TRUE(this->hyperplaneDirectX != this->hyperplaneDegeneratePositive);
		EXPECT_FALSE(this->hyperplaneDirectX != this->hyperplaneDirectX);
		EXPECT_TRUE(this->hyperplaneDirectX != this->hyperplaneReverseX);
		EXPECT_TRUE(this->hyperplaneDirectX != this->hyperplaneDirectY);
		EXPECT_TRUE(this->hyperplaneDirectX != this->hyperplaneReverseY);
		EXPECT_TRUE(this->hyperplaneDirectX != this->hyperplaneDirectZ);
		EXPECT_TRUE(this->hyperplaneDirectX != this->hyperplaneReverseZ);

		EXPECT_TRUE(this->hyperplaneReverseX != this->hyperplaneDegenerateNegative);
		EXPECT_TRUE(this->hyperplaneReverseX != this->hyperplaneDegenerateZero);
		EXPECT_TRUE(this->hyperplaneReverseX != this->hyperplaneDegeneratePositive);
		EXPECT_TRUE(this->hyperplaneReverseX != this->hyperplaneDirectX);
		EXPECT_FALSE(this->hyperplaneReverseX != this->hyperplaneReverseX);
		EXPECT_TRUE(this->hyperplaneReverseX != this->hyperplaneDirectY);
		EXPECT_TRUE(this->hyperplaneReverseX != this->hyperplaneReverseY);
		EXPECT_TRUE(this->hyperplaneReverseX != this->hyperplaneDirectZ);
		EXPECT_TRUE(this->hyperplaneReverseX != this->hyperplaneReverseZ);

		EXPECT_TRUE(this->hyperplaneDirectY != this->hyperplaneDegenerateNegative);
		EXPECT_TRUE(this->hyperplaneDirectY != this->hyperplaneDegenerateZero);
		EXPECT_TRUE(this->hyperplaneDirectY != this->hyperplaneDegeneratePositive);
		EXPECT_TRUE(this->hyperplaneDirectY != this->hyperplaneDirectX);
		EXPECT_TRUE(this->hyperplaneDirectY != this->hyperplaneReverseX);
		EXPECT_FALSE(this->hyperplaneDirectY != this->hyperplaneDirectY);
		EXPECT_TRUE(this->hyperplaneDirectY != this->hyperplaneReverseY);
		EXPECT_TRUE(this->hyperplaneDirectY != this->hyperplaneDirectZ);
		EXPECT_TRUE(this->hyperplaneDirectY != this->hyperplaneReverseZ);

		EXPECT_TRUE(this->hyperplaneReverseY != this->hyperplaneDegenerateNegative);
		EXPECT_TRUE(this->hyperplaneReverseY != this->hyperplaneDegenerateZero);
		EXPECT_TRUE(this->hyperplaneReverseY != this->hyperplaneDegeneratePositive);
		EXPECT_TRUE(this->hyperplaneReverseY != this->hyperplaneDirectX);
		EXPECT_TRUE(this->hyperplaneReverseY != this->hyperplaneReverseX);
		EXPECT_TRUE(this->hyperplaneReverseY != this->hyperplaneDirectY);
		EXPECT_FALSE(this->hyperplaneReverseY != this->hyperplaneReverseY);
		EXPECT_TRUE(this->hyperplaneReverseY != this->hyperplaneDirectZ);
		EXPECT_TRUE(this->hyperplaneReverseY != this->hyperplaneReverseZ);

		EXPECT_TRUE(this->hyperplaneDirectZ != this->hyperplaneDegenerateNegative);
		EXPECT_TRUE(this->hyperplaneDirectZ != this->hyperplaneDegenerateZero);
		EXPECT_TRUE(this->hyperplaneDirectZ != this->hyperplaneDegeneratePositive);
		EXPECT_TRUE(this->hyperplaneDirectZ != this->hyperplaneDirectX);
		EXPECT_TRUE(this->hyperplaneDirectZ != this->hyperplaneReverseX);
		EXPECT_TRUE(this->hyperplaneDirectZ != this->hyperplaneDirectY);
		EXPECT_TRUE(this->hyperplaneDirectZ != this->hyperplaneReverseY);
		EXPECT_FALSE(this->hyperplaneDirectZ != this->hyperplaneDirectZ);
		EXPECT_TRUE(this->hyperplaneDirectZ != this->hyperplaneReverseZ);

		EXPECT_TRUE(this->hyperplaneReverseZ != this->hyperplaneDegenerateNegative);
		EXPECT_TRUE(this->hyperplaneReverseZ != this->hyperplaneDegenerateZero);
		EXPECT_TRUE(this->hyperplaneReverseZ != this->hyperplaneDegeneratePositive);
		EXPECT_TRUE(this->hyperplaneReverseZ != this->hyperplaneDirectX);
		EXPECT_TRUE(this->hyperplaneReverseZ != this->hyperplaneReverseX);
		EXPECT_TRUE(this->hyperplaneReverseZ != this->hyperplaneDirectY);
		EXPECT_TRUE(this->hyperplaneReverseZ != this->hyperplaneReverseY);
		EXPECT_TRUE(this->hyperplaneReverseZ != this->hyperplaneDirectZ);
		EXPECT_FALSE(this->hyperplaneReverseZ != this->hyperplaneReverseZ);
	}

	TYPED_TEST_P(GeometryHyperplaneTest, SideOfHyperplane)
	{
		constexpr auto vertexOrigin =
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				}));

		constexpr auto vertexPlaneXY =
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				}));

		EXPECT_EQ(
			this->hyperplaneDirectX.side_of_hyperplane(vertexOrigin),
			xablau::geometry::hyperplane_side::contained_in_the_hyperplane);

		EXPECT_EQ(
			this->hyperplaneReverseX.side_of_hyperplane(vertexOrigin),
			xablau::geometry::hyperplane_side::contained_in_the_hyperplane);

		EXPECT_EQ(
			this->hyperplaneDirectZ.side_of_hyperplane(vertexOrigin),
			xablau::geometry::hyperplane_side::contained_in_the_hyperplane);

		EXPECT_EQ(
			this->hyperplaneReverseZ.side_of_hyperplane(vertexOrigin),
			xablau::geometry::hyperplane_side::contained_in_the_hyperplane);

		EXPECT_EQ(
			this->hyperplaneDirectX.side_of_hyperplane(vertexPlaneXY),
			xablau::geometry::hyperplane_side::same_sense_as_normal_vector);

		EXPECT_EQ(
			this->hyperplaneReverseX.side_of_hyperplane(vertexPlaneXY),
			xablau::geometry::hyperplane_side::opposite_sense_as_normal_vector);

		EXPECT_EQ(
			this->hyperplaneDirectY.side_of_hyperplane(vertexPlaneXY),
			xablau::geometry::hyperplane_side::same_sense_as_normal_vector);

		EXPECT_EQ(
			this->hyperplaneReverseY.side_of_hyperplane(vertexPlaneXY),
			xablau::geometry::hyperplane_side::opposite_sense_as_normal_vector);

		EXPECT_EQ(
			this->hyperplaneDirectZ.side_of_hyperplane(vertexPlaneXY),
			xablau::geometry::hyperplane_side::contained_in_the_hyperplane);
	}

	TYPED_TEST_P(GeometryHyperplaneTest, AngleLine)
	{
		const typename TypeParam::value_type value1 = typename TypeParam::value_type{0.5};
		const typename TypeParam::value_type value2 = std::sqrt(typename TypeParam::value_type{2}) / typename TypeParam::value_type{2};
		const typename TypeParam::value_type value3 = std::sqrt(typename TypeParam::value_type{3}) / typename TypeParam::value_type{2};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line0degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{0}
				}))};

		const xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line30degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					value3,
					value1
				}))};

		const xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line45degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					value2,
					value2
				}))};

		const xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line60degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					value1,
					value3
				}))};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line90degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				}))};

		const xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line120degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					-value1,
					value3
				}))};

		const xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line135degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					-value2,
					value2
				}))};

		const xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line150degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					-value3,
					value1
				}))};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line180degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{-1},
					typename TypeParam::value_type{0}
				}))};

		const xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line210degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					-value3,
					-value1
				}))};

		const xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line225degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					-value2,
					-value2
				}))};

		const xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line240degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					-value1,
					-value3
				}))};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line270degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{-1}
				}))};

		const xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line300degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					value1,
					-value3
				}))};

		const xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line315degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					value2,
					-value2
				}))};

		const xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line330degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					value3,
					-value1
				}))};

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectY.angle_between(line0degree),
				typename TypeParam::value_type{}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectY.angle_between(line30degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{6}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectY.angle_between(line45degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{4}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectY.angle_between(line60degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{3}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectY.angle_between(line90degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectY.angle_between(line120degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{3}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectY.angle_between(line135degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{4}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectY.angle_between(line150degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{6}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectY.angle_between(line180degree),
				typename TypeParam::value_type{0}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectY.angle_between(line210degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{6}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectY.angle_between(line225degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{4}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectY.angle_between(line240degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{3}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectY.angle_between(line270degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectY.angle_between(line300degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{3}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectY.angle_between(line315degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{4}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectY.angle_between(line330degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{6}));
	}

	TYPED_TEST_P(GeometryHyperplaneTest, AngleHyperplane)
	{
		const typename TypeParam::value_type value1 = typename TypeParam::value_type{0.5};
		const typename TypeParam::value_type value2 = std::sqrt(typename TypeParam::value_type{2}) / typename TypeParam::value_type{2};
		const typename TypeParam::value_type value3 = std::sqrt(typename TypeParam::value_type{3}) / typename TypeParam::value_type{2};

		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > hyperplane0degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{0}
				}))};

		const xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > hyperplane30degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					value3,
					value1
				}))};

		const xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > hyperplane45degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					value2,
					value2
				}))};

		const xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > hyperplane60degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					value1,
					value3
				}))};

		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > hyperplane90degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				}))};

		const xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > hyperplane120degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					-value1,
					value3
				}))};

		const xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > hyperplane135degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					-value2,
					value2
				}))};

		const xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > hyperplane150degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					-value3,
					value1
				}))};

		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > hyperplane180degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{-1},
					typename TypeParam::value_type{0}
				}))};

		const xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > hyperplane210degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					-value3,
					-value1
				}))};

		const xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > hyperplane225degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					-value2,
					-value2
				}))};

		const xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > hyperplane240degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					-value1,
					-value3
				}))};

		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > hyperplane270degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{-1}
				}))};

		const xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > hyperplane300degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					value1,
					-value3
				}))};

		const xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > hyperplane315degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					value2,
					-value2
				}))};

		const xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > hyperplane330degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					value3,
					-value1
				}))};

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectX.angle_between(hyperplane0degree),
				typename TypeParam::value_type{}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectX.angle_between(hyperplane30degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{6}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectX.angle_between(hyperplane45degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{4}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectX.angle_between(hyperplane60degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{3}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectX.angle_between(hyperplane90degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectX.angle_between(hyperplane120degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{3}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectX.angle_between(hyperplane135degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{4}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectX.angle_between(hyperplane150degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{6}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectX.angle_between(hyperplane180degree),
				typename TypeParam::value_type{0}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectX.angle_between(hyperplane210degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{6}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectX.angle_between(hyperplane225degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{4}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectX.angle_between(hyperplane240degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{3}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectX.angle_between(hyperplane270degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectX.angle_between(hyperplane300degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{3}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectX.angle_between(hyperplane315degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{4}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->hyperplaneDirectX.angle_between(hyperplane330degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{6}));
	}

	TYPED_TEST_P(GeometryHyperplaneTest, DistanceVertex)
	{
		constexpr xablau::geometry::vertex <
			typename TypeParam::value_type,
			xablau::geometry::spatial_dimension < 0 > > vertexOrigin{};

		constexpr xablau::geometry::vertex <
			typename TypeParam::value_type,
			xablau::geometry::spatial_dimension < 3 > > vertexDiagonal{
				std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})};

		constexpr xablau::geometry::vertex <
			typename TypeParam::value_type,
			xablau::geometry::spatial_dimension < 3 > > vertexX{
				std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})};

		constexpr xablau::geometry::vertex <
			typename TypeParam::value_type,
			xablau::geometry::spatial_dimension < 3 > > vertexY{
				std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				})};

		constexpr xablau::geometry::vertex <
			typename TypeParam::value_type,
			xablau::geometry::spatial_dimension < 2 > > vertexZ{
				std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})};

		EXPECT_EQ(
			this->hyperplaneDirectX.distance_to < false > (vertexOrigin),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->hyperplaneReverseX.distance_to < false > (vertexOrigin),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->hyperplaneDirectY.distance_to < false > (vertexOrigin),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->hyperplaneReverseY.distance_to < false > (vertexOrigin),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->hyperplaneDirectZ.distance_to < false > (vertexOrigin),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->hyperplaneReverseZ.distance_to < false > (vertexOrigin),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->hyperplaneDirectX.distance_to < false > (vertexDiagonal),
			typename TypeParam::value_type{1});

		EXPECT_EQ(
			this->hyperplaneReverseX.distance_to < false > (vertexDiagonal),
			typename TypeParam::value_type{1});

		EXPECT_EQ(
			this->hyperplaneDirectY.distance_to < false > (vertexDiagonal),
			typename TypeParam::value_type{1});

		EXPECT_EQ(
			this->hyperplaneReverseY.distance_to < false > (vertexDiagonal),
			typename TypeParam::value_type{1});

		EXPECT_EQ(
			this->hyperplaneDirectZ.distance_to < false > (vertexDiagonal),
			typename TypeParam::value_type{1});

		EXPECT_EQ(
			this->hyperplaneReverseZ.distance_to < false > (vertexDiagonal),
			typename TypeParam::value_type{1});

		EXPECT_EQ(
			this->hyperplaneDirectX.distance_to < true > (vertexOrigin).second,
			vertexOrigin);

		EXPECT_EQ(
			this->hyperplaneReverseX.distance_to < true > (vertexOrigin).second,
			vertexOrigin);

		EXPECT_EQ(
			this->hyperplaneDirectY.distance_to < true > (vertexOrigin).second,
			vertexOrigin);

		EXPECT_EQ(
			this->hyperplaneReverseY.distance_to < true > (vertexOrigin).second,
			vertexOrigin);

		EXPECT_EQ(
			this->hyperplaneDirectZ.distance_to < true > (vertexOrigin).second,
			vertexOrigin);

		EXPECT_EQ(
			this->hyperplaneReverseZ.distance_to < true > (vertexOrigin).second,
			vertexOrigin);

		EXPECT_EQ(
			this->hyperplaneDirectX.distance_to < true > (vertexDiagonal).second,
			vertexX);

		EXPECT_EQ(
			this->hyperplaneReverseX.distance_to < true > (vertexDiagonal).second,
			vertexX);

		EXPECT_EQ(
			this->hyperplaneDirectY.distance_to < true > (vertexDiagonal).second,
			vertexY);

		EXPECT_EQ(
			this->hyperplaneReverseY.distance_to < true > (vertexDiagonal).second,
			vertexY);

		EXPECT_EQ(
			this->hyperplaneDirectZ.distance_to < true > (vertexDiagonal).second,
			vertexZ);

		EXPECT_EQ(
			this->hyperplaneReverseZ.distance_to < true > (vertexDiagonal).second,
			vertexZ);
	}

	TYPED_TEST_P(GeometryHyperplaneTest, DistanceLine)
	{
		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 0 > > vertexOrigin{};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > lineOrigin{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				}))};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > lineParallelToYZ{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				}))};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > lineParallelToXZ{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				}))};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > lineParallelToXY{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				}))};

		EXPECT_EQ(
			this->hyperplaneDirectX.distance_to < false > (lineOrigin),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->hyperplaneReverseX.distance_to < false > (lineOrigin),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->hyperplaneDirectY.distance_to < false > (lineOrigin),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->hyperplaneReverseY.distance_to < false > (lineOrigin),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->hyperplaneDirectZ.distance_to < false > (lineOrigin),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->hyperplaneReverseZ.distance_to < false > (lineOrigin),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->hyperplaneDirectX.distance_to < false > (lineParallelToYZ),
			typename TypeParam::value_type{1});

		EXPECT_EQ(
			this->hyperplaneReverseX.distance_to < false > (lineParallelToYZ),
			typename TypeParam::value_type{1});

		EXPECT_EQ(
			this->hyperplaneDirectY.distance_to < false > (lineParallelToXZ),
			typename TypeParam::value_type{1});

		EXPECT_EQ(
			this->hyperplaneReverseY.distance_to < false > (lineParallelToXZ),
			typename TypeParam::value_type{1});

		EXPECT_EQ(
			this->hyperplaneDirectZ.distance_to < false > (lineParallelToXY),
			typename TypeParam::value_type{1});

		EXPECT_EQ(
			this->hyperplaneReverseZ.distance_to < false > (lineParallelToXY),
			typename TypeParam::value_type{1});

		EXPECT_EQ(
			this->hyperplaneDirectX.distance_to < true > (lineOrigin).second,
			vertexOrigin);

		EXPECT_EQ(
			this->hyperplaneReverseX.distance_to < true > (lineOrigin).second,
			vertexOrigin);

		EXPECT_EQ(
			this->hyperplaneDirectY.distance_to < true > (lineOrigin).second,
			vertexOrigin);

		EXPECT_EQ(
			this->hyperplaneReverseY.distance_to < true > (lineOrigin).second,
			vertexOrigin);

		EXPECT_EQ(
			this->hyperplaneDirectZ.distance_to < true > (lineOrigin).second,
			vertexOrigin);

		EXPECT_EQ(
			this->hyperplaneReverseZ.distance_to < true > (lineOrigin).second,
			vertexOrigin);

		EXPECT_EQ(
			this->hyperplaneDirectX.distance_to < true > (lineParallelToYZ).second,
			vertexOrigin);

		EXPECT_EQ(
			this->hyperplaneReverseX.distance_to < true > (lineParallelToYZ).second,
			vertexOrigin);

		EXPECT_EQ(
			this->hyperplaneDirectY.distance_to < true > (lineParallelToXZ).second,
			vertexOrigin);

		EXPECT_EQ(
			this->hyperplaneReverseY.distance_to < true > (lineParallelToXZ).second,
			vertexOrigin);

		EXPECT_EQ(
			this->hyperplaneDirectZ.distance_to < true > (lineParallelToXY).second,
			vertexOrigin);

		EXPECT_EQ(
			this->hyperplaneReverseZ.distance_to < true > (lineParallelToXY).second,
			vertexOrigin);
	}

	TYPED_TEST_P(GeometryHyperplaneTest, DistanceHyperplane)
	{
		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > hyperplaneOrigin{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				}))};

		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 1 > > hyperplaneParallelToYZ{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{1}
				}))};

		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > hyperplaneParallelToXZ{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				}))};

		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > hyperplaneParallelToXY{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				}))};

		EXPECT_EQ(
			this->hyperplaneDirectX.distance_to(hyperplaneOrigin),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->hyperplaneReverseX.distance_to(hyperplaneOrigin),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->hyperplaneDirectY.distance_to(hyperplaneOrigin),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->hyperplaneReverseY.distance_to(hyperplaneOrigin),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->hyperplaneDirectZ.distance_to(hyperplaneOrigin),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->hyperplaneReverseZ.distance_to(hyperplaneOrigin),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->hyperplaneDirectX.distance_to(hyperplaneParallelToYZ),
			typename TypeParam::value_type{1});

		EXPECT_EQ(
			this->hyperplaneReverseX.distance_to(hyperplaneParallelToYZ),
			typename TypeParam::value_type{1});

		EXPECT_EQ(
			this->hyperplaneDirectY.distance_to(hyperplaneParallelToXZ),
			typename TypeParam::value_type{1});

		EXPECT_EQ(
			this->hyperplaneReverseY.distance_to(hyperplaneParallelToXZ),
			typename TypeParam::value_type{1});

		EXPECT_EQ(
			this->hyperplaneDirectZ.distance_to(hyperplaneParallelToXY),
			typename TypeParam::value_type{1});

		EXPECT_EQ(
			this->hyperplaneReverseZ.distance_to(hyperplaneParallelToXY),
			typename TypeParam::value_type{1});
	}

	TYPED_TEST_P(GeometryHyperplaneTest, ContainmentVertex)
	{
		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 0 > > vertexOrigin{};

		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 1 > > vertexOnXAxis{
			std::to_array(
				{
					typename TypeParam::value_type{1}
				})};

		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertexOnYAxis{
			std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				})};

		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > vertexOnZAxis{
			std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				})};

		EXPECT_TRUE(this->hyperplaneDirectX.contains(vertexOrigin));
		EXPECT_TRUE(this->hyperplaneReverseX.contains(vertexOrigin));
		EXPECT_TRUE(this->hyperplaneDirectY.contains(vertexOrigin));
		EXPECT_TRUE(this->hyperplaneReverseY.contains(vertexOrigin));
		EXPECT_TRUE(this->hyperplaneDirectZ.contains(vertexOrigin));
		EXPECT_TRUE(this->hyperplaneReverseZ.contains(vertexOrigin));

		EXPECT_FALSE(this->hyperplaneDirectX.contains(vertexOnXAxis));
		EXPECT_FALSE(this->hyperplaneReverseX.contains(vertexOnXAxis));
		EXPECT_FALSE(this->hyperplaneDirectY.contains(vertexOnYAxis));
		EXPECT_FALSE(this->hyperplaneReverseY.contains(vertexOnYAxis));
		EXPECT_FALSE(this->hyperplaneDirectZ.contains(vertexOnZAxis));
		EXPECT_FALSE(this->hyperplaneReverseZ.contains(vertexOnZAxis));
	}

	TYPED_TEST_P(GeometryHyperplaneTest, ContainmentLine)
	{
		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertexOnXYAxis{
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})};

		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > vertexOnXZAxis{
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				})};

		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > vertexOnYZAxis{
			std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})};

		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > vertexOnDiagonal{
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})};

		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vectorOnXYPlane{
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})};

		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > vectorOnXZPlane{
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				})};

		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > vectorOnYZPlane{
			std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})};

		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > vectorOnDiagonal{
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > lineOnXYPlane{
				vertexOnXYAxis,
				vectorOnXYPlane
			};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > lineOnXZPlane{
				vertexOnXZAxis,
				vectorOnXZPlane
			};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > lineOnYZPlane{
				vertexOnYZAxis,
				vectorOnYZPlane
			};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > lineOnDiagonal{
				vertexOnDiagonal,
				vectorOnDiagonal
			};

		EXPECT_TRUE(this->hyperplaneDirectX.contains(lineOnYZPlane));
		EXPECT_TRUE(this->hyperplaneReverseX.contains(lineOnYZPlane));
		EXPECT_TRUE(this->hyperplaneDirectY.contains(lineOnXZPlane));
		EXPECT_TRUE(this->hyperplaneReverseY.contains(lineOnXZPlane));
		EXPECT_TRUE(this->hyperplaneDirectZ.contains(lineOnXYPlane));
		EXPECT_TRUE(this->hyperplaneReverseZ.contains(lineOnXYPlane));

		EXPECT_FALSE(this->hyperplaneDirectX.contains(lineOnDiagonal));
		EXPECT_FALSE(this->hyperplaneReverseZ.contains(lineOnDiagonal));

		EXPECT_FALSE(this->hyperplaneDirectX.contains(lineOnDiagonal));
		EXPECT_FALSE(this->hyperplaneReverseX.contains(lineOnDiagonal));
		EXPECT_FALSE(this->hyperplaneDirectY.contains(lineOnDiagonal));
		EXPECT_FALSE(this->hyperplaneReverseY.contains(lineOnDiagonal));
		EXPECT_FALSE(this->hyperplaneDirectZ.contains(lineOnDiagonal));
		EXPECT_FALSE(this->hyperplaneReverseZ.contains(lineOnDiagonal));
	}

	TYPED_TEST_P(GeometryHyperplaneTest, IntersectionLine)
	{
		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 0 > > vertexOrigin{};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > lineDiagonal{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				}))};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > lineOnPlaneXY{
			vertexOrigin,
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				}))};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > lineOnPlaneXYWithOffset{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				}))};

		constexpr auto offsetVector =
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				}));

		EXPECT_TRUE(this->hyperplaneDirectX.intersects < false > (lineOnPlaneXY));
		EXPECT_TRUE(this->hyperplaneReverseX.intersects < false > (lineOnPlaneXY));
		EXPECT_TRUE(this->hyperplaneDirectY.intersects < false > (lineOnPlaneXY));
		EXPECT_TRUE(this->hyperplaneReverseY.intersects < false > (lineOnPlaneXY));
		EXPECT_TRUE(this->hyperplaneDirectZ.intersects < false > (lineOnPlaneXY));
		EXPECT_TRUE(this->hyperplaneReverseZ.intersects < false > (lineOnPlaneXY));

		EXPECT_TRUE(this->hyperplaneDirectX.intersects < false > (lineOnPlaneXYWithOffset));
		EXPECT_TRUE(this->hyperplaneReverseX.intersects < false > (lineOnPlaneXYWithOffset));
		EXPECT_TRUE(this->hyperplaneDirectY.intersects < false > (lineOnPlaneXYWithOffset));
		EXPECT_TRUE(this->hyperplaneReverseY.intersects < false > (lineOnPlaneXYWithOffset));
		EXPECT_FALSE(this->hyperplaneDirectZ.intersects < false > (lineOnPlaneXYWithOffset));
		EXPECT_FALSE(this->hyperplaneReverseZ.intersects < false > (lineOnPlaneXYWithOffset));

		EXPECT_TRUE(this->hyperplaneDirectX.intersects < false > (lineDiagonal));
		EXPECT_TRUE(this->hyperplaneReverseX.intersects < false > (lineDiagonal));
		EXPECT_TRUE(this->hyperplaneDirectY.intersects < false > (lineDiagonal));
		EXPECT_TRUE(this->hyperplaneReverseY.intersects < false > (lineDiagonal));
		EXPECT_TRUE(this->hyperplaneDirectZ.intersects < false > (lineDiagonal));
		EXPECT_TRUE(this->hyperplaneReverseZ.intersects < false > (lineDiagonal));

		EXPECT_EQ(this->hyperplaneDirectX.intersects < true > (lineOnPlaneXY).value(), lineOnPlaneXY.vertex);
		EXPECT_EQ(this->hyperplaneReverseX.intersects < true > (lineOnPlaneXY).value(), lineOnPlaneXY.vertex);
		EXPECT_EQ(this->hyperplaneDirectY.intersects < true > (lineOnPlaneXY).value(), lineOnPlaneXY.vertex);
		EXPECT_EQ(this->hyperplaneReverseY.intersects < true > (lineOnPlaneXY).value(), lineOnPlaneXY.vertex);
		EXPECT_EQ(this->hyperplaneDirectZ.intersects < true > (lineOnPlaneXY).value(), lineOnPlaneXY.vertex);
		EXPECT_EQ(this->hyperplaneReverseZ.intersects < true > (lineOnPlaneXY).value(), lineOnPlaneXY.vertex);

		EXPECT_EQ(this->hyperplaneDirectX.intersects < true > (lineOnPlaneXYWithOffset), lineOnPlaneXY.vertex + offsetVector);
		EXPECT_EQ(this->hyperplaneReverseX.intersects < true > (lineOnPlaneXYWithOffset), lineOnPlaneXY.vertex + offsetVector);
		EXPECT_EQ(this->hyperplaneDirectY.intersects < true > (lineOnPlaneXYWithOffset), lineOnPlaneXY.vertex + offsetVector);
		EXPECT_EQ(this->hyperplaneReverseY.intersects < true > (lineOnPlaneXYWithOffset), lineOnPlaneXY.vertex + offsetVector);
		EXPECT_EQ(this->hyperplaneDirectZ.intersects < true > (lineOnPlaneXYWithOffset), std::nullopt);
		EXPECT_EQ(this->hyperplaneReverseZ.intersects < true > (lineOnPlaneXYWithOffset), std::nullopt);

		EXPECT_EQ(this->hyperplaneDirectX.intersects < true > (lineDiagonal), vertexOrigin);
		EXPECT_EQ(this->hyperplaneReverseX.intersects < true > (lineDiagonal), vertexOrigin);
		EXPECT_EQ(this->hyperplaneDirectY.intersects < true > (lineDiagonal), vertexOrigin);
		EXPECT_EQ(this->hyperplaneReverseY.intersects < true > (lineDiagonal), vertexOrigin);
		EXPECT_EQ(this->hyperplaneDirectZ.intersects < true > (lineDiagonal), vertexOrigin);
		EXPECT_EQ(this->hyperplaneReverseZ.intersects < true > (lineDiagonal), vertexOrigin);
	}

	TYPED_TEST_P(GeometryHyperplaneTest, IntersectionHyperplane)
	{
		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 0 > > vertexOrigin{};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 1 > > lineOnXAxis{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{1}
				}))};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > lineOnYAxis{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				}))};

		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > hyperplaneDiagonal{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				}))};

		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > hyperplaneOnPlaneXY{
			vertexOrigin,
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				}))};

		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > hyperplaneOnPlaneXYWithOffset{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				}))};

		constexpr auto offsetVector =
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				}));

		EXPECT_TRUE(this->hyperplaneDirectX.intersects < false > (hyperplaneOnPlaneXY));
		EXPECT_TRUE(this->hyperplaneReverseX.intersects < false > (hyperplaneOnPlaneXY));
		EXPECT_TRUE(this->hyperplaneDirectY.intersects < false > (hyperplaneOnPlaneXY));
		EXPECT_TRUE(this->hyperplaneReverseY.intersects < false > (hyperplaneOnPlaneXY));
		EXPECT_TRUE(this->hyperplaneDirectZ.intersects < false > (hyperplaneOnPlaneXY));
		EXPECT_TRUE(this->hyperplaneReverseZ.intersects < false > (hyperplaneOnPlaneXY));

		EXPECT_TRUE(this->hyperplaneDirectX.intersects < false > (hyperplaneOnPlaneXYWithOffset));
		EXPECT_TRUE(this->hyperplaneReverseX.intersects < false > (hyperplaneOnPlaneXYWithOffset));
		EXPECT_TRUE(this->hyperplaneDirectY.intersects < false > (hyperplaneOnPlaneXYWithOffset));
		EXPECT_TRUE(this->hyperplaneReverseY.intersects < false > (hyperplaneOnPlaneXYWithOffset));
		EXPECT_FALSE(this->hyperplaneDirectZ.intersects < false > (hyperplaneOnPlaneXYWithOffset));
		EXPECT_FALSE(this->hyperplaneReverseZ.intersects < false > (hyperplaneOnPlaneXYWithOffset));

		EXPECT_TRUE(this->hyperplaneDirectX.intersects < false > (hyperplaneDiagonal));
		EXPECT_TRUE(this->hyperplaneReverseX.intersects < false > (hyperplaneDiagonal));
		EXPECT_TRUE(this->hyperplaneDirectY.intersects < false > (hyperplaneDiagonal));
		EXPECT_TRUE(this->hyperplaneReverseY.intersects < false > (hyperplaneDiagonal));
		EXPECT_TRUE(this->hyperplaneDirectZ.intersects < false > (hyperplaneDiagonal));
		EXPECT_TRUE(this->hyperplaneReverseZ.intersects < false > (hyperplaneDiagonal));

		EXPECT_EQ(std::get < 0 > (this->hyperplaneDirectX.intersects < true > (hyperplaneOnPlaneXY).value()), lineOnYAxis);
		EXPECT_EQ(std::get < 0 > (this->hyperplaneReverseX.intersects < true > (hyperplaneOnPlaneXY).value()), lineOnYAxis);
		EXPECT_EQ(std::get < 0 > (this->hyperplaneDirectY.intersects < true > (hyperplaneOnPlaneXY).value()), lineOnXAxis);
		EXPECT_EQ(std::get < 0 > (this->hyperplaneReverseY.intersects < true > (hyperplaneOnPlaneXY).value()), lineOnXAxis);
		EXPECT_EQ(std::get < 1 > (this->hyperplaneDirectZ.intersects < true > (hyperplaneOnPlaneXY).value()).get(), this->hyperplaneDirectZ);
		EXPECT_EQ(std::get < 1 > (this->hyperplaneReverseZ.intersects < true > (hyperplaneOnPlaneXY).value()).get(), this->hyperplaneReverseZ);

		EXPECT_EQ(std::get < 0 > (this->hyperplaneDirectX.intersects < true > (hyperplaneOnPlaneXYWithOffset).value()), lineOnYAxis + offsetVector);
		EXPECT_EQ(std::get < 0 > (this->hyperplaneReverseX.intersects < true > (hyperplaneOnPlaneXYWithOffset).value()), lineOnYAxis + offsetVector);
		EXPECT_EQ(std::get < 0 > (this->hyperplaneDirectY.intersects < true > (hyperplaneOnPlaneXYWithOffset).value()), lineOnXAxis + offsetVector);
		EXPECT_EQ(std::get < 0 > (this->hyperplaneReverseY.intersects < true > (hyperplaneOnPlaneXYWithOffset).value()), lineOnXAxis + offsetVector);
		EXPECT_EQ(this->hyperplaneDirectZ.intersects < true > (hyperplaneOnPlaneXYWithOffset), std::nullopt);
		EXPECT_EQ(this->hyperplaneReverseZ.intersects < true > (hyperplaneOnPlaneXYWithOffset), std::nullopt);
	}

	TYPED_TEST_P(GeometryHyperplaneTest, CoincidenceWithoutCodirectionalNormalVector)
	{
		EXPECT_TRUE(this->hyperplaneDirectX.coincides_with(this->hyperplaneDirectX));
		EXPECT_TRUE(this->hyperplaneDirectX.coincides_with(this->hyperplaneReverseX));
		EXPECT_FALSE(this->hyperplaneDirectX.coincides_with(this->hyperplaneDirectY));
		EXPECT_FALSE(this->hyperplaneDirectX.coincides_with(this->hyperplaneReverseY));
		EXPECT_FALSE(this->hyperplaneDirectX.coincides_with(this->hyperplaneDirectZ));
		EXPECT_FALSE(this->hyperplaneDirectX.coincides_with(this->hyperplaneReverseZ));

		EXPECT_FALSE(this->hyperplaneReverseZ.coincides_with(this->hyperplaneDirectX));
		EXPECT_FALSE(this->hyperplaneReverseZ.coincides_with(this->hyperplaneReverseX));
		EXPECT_FALSE(this->hyperplaneReverseZ.coincides_with(this->hyperplaneDirectY));
		EXPECT_FALSE(this->hyperplaneReverseZ.coincides_with(this->hyperplaneReverseY));
		EXPECT_TRUE(this->hyperplaneReverseZ.coincides_with(this->hyperplaneDirectZ));
		EXPECT_TRUE(this->hyperplaneReverseZ.coincides_with(this->hyperplaneReverseZ));
	}

	TYPED_TEST_P(GeometryHyperplaneTest, CoincidenceWithCodirectionalNormalVector)
	{
		EXPECT_TRUE(this->hyperplaneDirectX.coincides_with_codirectional_normal_vector(this->hyperplaneDirectX));
		EXPECT_FALSE(this->hyperplaneDirectX.coincides_with_codirectional_normal_vector(this->hyperplaneReverseX));
		EXPECT_FALSE(this->hyperplaneDirectX.coincides_with_codirectional_normal_vector(this->hyperplaneDirectY));
		EXPECT_FALSE(this->hyperplaneDirectX.coincides_with_codirectional_normal_vector(this->hyperplaneReverseY));
		EXPECT_FALSE(this->hyperplaneDirectX.coincides_with_codirectional_normal_vector(this->hyperplaneDirectZ));
		EXPECT_FALSE(this->hyperplaneDirectX.coincides_with_codirectional_normal_vector(this->hyperplaneReverseZ));

		EXPECT_FALSE(this->hyperplaneReverseZ.coincides_with_codirectional_normal_vector(this->hyperplaneDirectX));
		EXPECT_FALSE(this->hyperplaneReverseZ.coincides_with_codirectional_normal_vector(this->hyperplaneReverseX));
		EXPECT_FALSE(this->hyperplaneReverseZ.coincides_with_codirectional_normal_vector(this->hyperplaneDirectY));
		EXPECT_FALSE(this->hyperplaneReverseZ.coincides_with_codirectional_normal_vector(this->hyperplaneReverseY));
		EXPECT_FALSE(this->hyperplaneReverseZ.coincides_with_codirectional_normal_vector(this->hyperplaneDirectZ));
		EXPECT_TRUE(this->hyperplaneReverseZ.coincides_with_codirectional_normal_vector(this->hyperplaneReverseZ));
	}

	TYPED_TEST_P(GeometryHyperplaneTest, PerpendicularityHyperplane)
	{
		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > hyperplaneDiagonal{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				}))};

		EXPECT_FALSE(this->hyperplaneDirectX.perpendicular_to(this->hyperplaneDirectX));
		EXPECT_FALSE(this->hyperplaneDirectX.perpendicular_to(this->hyperplaneReverseX));
		EXPECT_TRUE(this->hyperplaneDirectX.perpendicular_to(this->hyperplaneDirectY));
		EXPECT_TRUE(this->hyperplaneDirectX.perpendicular_to(this->hyperplaneReverseY));
		EXPECT_TRUE(this->hyperplaneDirectX.perpendicular_to(this->hyperplaneDirectZ));
		EXPECT_TRUE(this->hyperplaneDirectX.perpendicular_to(this->hyperplaneReverseZ));

		EXPECT_TRUE(this->hyperplaneReverseZ.perpendicular_to(this->hyperplaneDirectX));
		EXPECT_TRUE(this->hyperplaneReverseZ.perpendicular_to(this->hyperplaneReverseX));
		EXPECT_TRUE(this->hyperplaneReverseZ.perpendicular_to(this->hyperplaneDirectY));
		EXPECT_TRUE(this->hyperplaneReverseZ.perpendicular_to(this->hyperplaneReverseY));
		EXPECT_FALSE(this->hyperplaneReverseZ.perpendicular_to(this->hyperplaneDirectZ));
		EXPECT_FALSE(this->hyperplaneReverseZ.perpendicular_to(this->hyperplaneReverseZ));

		EXPECT_FALSE(hyperplaneDiagonal.perpendicular_to(this->hyperplaneDirectX));
		EXPECT_FALSE(hyperplaneDiagonal.perpendicular_to(this->hyperplaneReverseX));
		EXPECT_FALSE(hyperplaneDiagonal.perpendicular_to(this->hyperplaneDirectY));
		EXPECT_FALSE(hyperplaneDiagonal.perpendicular_to(this->hyperplaneReverseY));
		EXPECT_FALSE(hyperplaneDiagonal.perpendicular_to(this->hyperplaneDirectZ));
		EXPECT_FALSE(hyperplaneDiagonal.perpendicular_to(this->hyperplaneReverseZ));
	}

	TYPED_TEST_P(GeometryHyperplaneTest, ParallelismHyperplane)
	{
		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 0 > > vertexOrigin{};

		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > hyperplaneDiagonal{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				}))};

		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > hyperplaneParallelToPlaneXY{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				}))};

		EXPECT_FALSE(this->hyperplaneDirectX.parallel_to(this->hyperplaneDirectX));
		EXPECT_FALSE(this->hyperplaneDirectX.parallel_to(this->hyperplaneReverseX));
		EXPECT_FALSE(this->hyperplaneDirectX.parallel_to(this->hyperplaneDirectY));
		EXPECT_FALSE(this->hyperplaneDirectX.parallel_to(this->hyperplaneReverseY));
		EXPECT_FALSE(this->hyperplaneDirectX.parallel_to(this->hyperplaneDirectZ));
		EXPECT_FALSE(this->hyperplaneDirectX.parallel_to(this->hyperplaneReverseZ));

		EXPECT_FALSE(this->hyperplaneReverseZ.parallel_to(this->hyperplaneDirectX));
		EXPECT_FALSE(this->hyperplaneReverseZ.parallel_to(this->hyperplaneReverseX));
		EXPECT_FALSE(this->hyperplaneReverseZ.parallel_to(this->hyperplaneDirectY));
		EXPECT_FALSE(this->hyperplaneReverseZ.parallel_to(this->hyperplaneReverseY));
		EXPECT_FALSE(this->hyperplaneReverseZ.parallel_to(this->hyperplaneDirectZ));
		EXPECT_FALSE(this->hyperplaneReverseZ.parallel_to(this->hyperplaneReverseZ));

		EXPECT_FALSE(hyperplaneDiagonal.parallel_to(this->hyperplaneDirectX));
		EXPECT_FALSE(hyperplaneDiagonal.parallel_to(this->hyperplaneReverseX));
		EXPECT_FALSE(hyperplaneDiagonal.parallel_to(this->hyperplaneDirectY));
		EXPECT_FALSE(hyperplaneDiagonal.parallel_to(this->hyperplaneReverseY));
		EXPECT_FALSE(hyperplaneDiagonal.parallel_to(this->hyperplaneDirectZ));
		EXPECT_FALSE(hyperplaneDiagonal.parallel_to(this->hyperplaneReverseZ));

		EXPECT_FALSE(hyperplaneParallelToPlaneXY.parallel_to(this->hyperplaneDirectX));
		EXPECT_FALSE(hyperplaneParallelToPlaneXY.parallel_to(this->hyperplaneReverseX));
		EXPECT_FALSE(hyperplaneParallelToPlaneXY.parallel_to(this->hyperplaneDirectY));
		EXPECT_FALSE(hyperplaneParallelToPlaneXY.parallel_to(this->hyperplaneReverseY));
		EXPECT_TRUE(hyperplaneParallelToPlaneXY.parallel_to(this->hyperplaneDirectZ));
		EXPECT_TRUE(hyperplaneParallelToPlaneXY.parallel_to(this->hyperplaneReverseZ));
	}

	REGISTER_TYPED_TEST_SUITE_P(
		GeometryHyperplaneTest,
		ComparisonOperatorLess,
		ComparisonOperatorLessEqualTo,
		ComparisonOperatorEqualTo,
		ComparisonOperatorGreaterEqualTo,
		ComparisonOperatorGreater,
		ComparisonOperatorNotEqualTo,
		SideOfHyperplane,
		AngleLine,
		AngleHyperplane,
		DistanceVertex,
		DistanceLine,
		DistanceHyperplane,
		ContainmentVertex,
		ContainmentLine,
		IntersectionLine,
		IntersectionHyperplane,
		CoincidenceWithoutCodirectionalNormalVector,
		CoincidenceWithCodirectionalNormalVector,
		PerpendicularityHyperplane,
		ParallelismHyperplane);

	using GeometryHyperplaneTestDataTypes = std::tuple < float, double, long double >;

	using GeometryHyperplaneTestGroup =
		xablau::testing::test_bundler < GeometryHyperplaneTestDataTypes > ::template test_group < GeometryHyperplaneTypeDefinitions >;

	using GeometryHyperplaneTypes = xablau::testing::tuple_transformation < ::testing::Types, GeometryHyperplaneTestGroup > ::types;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		Xablau,
		GeometryHyperplaneTest,
		GeometryHyperplaneTypes);
}
