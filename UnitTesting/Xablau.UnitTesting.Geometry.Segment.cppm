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
	struct GeometrySegmentTypeDefinitions
	{
		using value_type = Type;
	};

	template < typename Types >
	class GeometrySegmentTest : public ::testing::Test
	{
	public:
		static constexpr xablau::geometry::segment < typename Types::value_type, xablau::geometry::spatial_dimension < 1 > > segmentDegenerateNegative{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename Types::value_type(-1)
				})),
			xablau::geometry::make_vertex(std::to_array(
				{
					typename Types::value_type(-1)
				}))};

		static constexpr xablau::geometry::segment < typename Types::value_type, xablau::geometry::spatial_dimension < 0 > > segmentDegenerateZero;

		static constexpr xablau::geometry::segment < typename Types::value_type, xablau::geometry::spatial_dimension < 1 > > segmentDegeneratePositive{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename Types::value_type(1)
				})),
			xablau::geometry::make_vertex(std::to_array(
				{
					typename Types::value_type(1)
				}))};

		static constexpr xablau::geometry::segment < typename Types::value_type, xablau::geometry::spatial_dimension < 1 > > segmentDirectX{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename Types::value_type(-1)
				})),
			xablau::geometry::make_vertex(std::to_array(
				{
					typename Types::value_type(1)
				}))};

		static constexpr xablau::geometry::segment < typename Types::value_type, xablau::geometry::spatial_dimension < 1 > > segmentReverseX{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename Types::value_type(1)
				})),
			xablau::geometry::make_vertex(std::to_array(
				{
					typename Types::value_type(-1)
				}))};

		static constexpr xablau::geometry::segment < typename Types::value_type, xablau::geometry::spatial_dimension < 2 > > segmentDirectY{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename Types::value_type(0),
					typename Types::value_type(-1)
				})),
			xablau::geometry::make_vertex(std::to_array(
				{
					typename Types::value_type(0),
					typename Types::value_type(1)
				}))};

		static constexpr xablau::geometry::segment < typename Types::value_type, xablau::geometry::spatial_dimension < 2 > > segmentReverseY{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename Types::value_type(0),
					typename Types::value_type(1)
				})),
			xablau::geometry::make_vertex(std::to_array(
				{
					typename Types::value_type(0),
					typename Types::value_type(-1)
				}))};

		static constexpr xablau::geometry::segment < typename Types::value_type, xablau::geometry::spatial_dimension < 3 > > segmentDirectZ{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename Types::value_type(0),
					typename Types::value_type(0),
					typename Types::value_type(-1)
				})),
			xablau::geometry::make_vertex(std::to_array(
				{
					typename Types::value_type(0),
					typename Types::value_type(0),
					typename Types::value_type(1)
				}))};

		static constexpr xablau::geometry::segment < typename Types::value_type, xablau::geometry::spatial_dimension < 3 > > segmentReverseZ{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename Types::value_type(0),
					typename Types::value_type(0),
					typename Types::value_type(1)
				})),
			xablau::geometry::make_vertex(std::to_array(
				{
					typename Types::value_type(0),
					typename Types::value_type(0),
					typename Types::value_type(-1)
				}))};
	};

	TYPED_TEST_SUITE_P(GeometrySegmentTest);

	TYPED_TEST_P(GeometrySegmentTest, ComparisonOperatorLess)
	{
		EXPECT_FALSE(this->segmentDegenerateNegative < this->segmentDegenerateNegative);
		EXPECT_TRUE(this->segmentDegenerateNegative < this->segmentDegenerateZero);
		EXPECT_TRUE(this->segmentDegenerateNegative < this->segmentDegeneratePositive);
		EXPECT_TRUE(this->segmentDegenerateNegative < this->segmentDirectX);
		EXPECT_TRUE(this->segmentDegenerateNegative < this->segmentReverseX);
		EXPECT_TRUE(this->segmentDegenerateNegative < this->segmentDirectY);
		EXPECT_TRUE(this->segmentDegenerateNegative < this->segmentReverseY);
		EXPECT_TRUE(this->segmentDegenerateNegative < this->segmentDirectZ);
		EXPECT_TRUE(this->segmentDegenerateNegative < this->segmentReverseZ);

		EXPECT_FALSE(this->segmentDegenerateZero < this->segmentDegenerateNegative);
		EXPECT_FALSE(this->segmentDegenerateZero < this->segmentDegenerateZero);
		EXPECT_TRUE(this->segmentDegenerateZero < this->segmentDegeneratePositive);
		EXPECT_TRUE(this->segmentDegenerateZero < this->segmentDirectX);
		EXPECT_TRUE(this->segmentDegenerateZero < this->segmentReverseX);
		EXPECT_TRUE(this->segmentDegenerateZero < this->segmentDirectY);
		EXPECT_TRUE(this->segmentDegenerateZero < this->segmentReverseY);
		EXPECT_TRUE(this->segmentDegenerateZero < this->segmentDirectZ);
		EXPECT_TRUE(this->segmentDegenerateZero < this->segmentReverseZ);

		EXPECT_FALSE(this->segmentDegeneratePositive < this->segmentDegenerateNegative);
		EXPECT_FALSE(this->segmentDegeneratePositive < this->segmentDegenerateZero);
		EXPECT_FALSE(this->segmentDegeneratePositive < this->segmentDegeneratePositive);
		EXPECT_TRUE(this->segmentDegeneratePositive < this->segmentDirectX);
		EXPECT_TRUE(this->segmentDegeneratePositive < this->segmentReverseX);
		EXPECT_TRUE(this->segmentDegeneratePositive < this->segmentDirectY);
		EXPECT_TRUE(this->segmentDegeneratePositive < this->segmentReverseY);
		EXPECT_TRUE(this->segmentDegeneratePositive < this->segmentDirectZ);
		EXPECT_TRUE(this->segmentDegeneratePositive < this->segmentReverseZ);

		EXPECT_FALSE(this->segmentDirectX < this->segmentDegenerateNegative);
		EXPECT_FALSE(this->segmentDirectX < this->segmentDegenerateZero);
		EXPECT_FALSE(this->segmentDirectX < this->segmentDegeneratePositive);
		EXPECT_FALSE(this->segmentDirectX < this->segmentDirectX);
		EXPECT_FALSE(this->segmentDirectX < this->segmentReverseX);
		EXPECT_TRUE(this->segmentDirectX < this->segmentDirectY);
		EXPECT_TRUE(this->segmentDirectX < this->segmentReverseY);
		EXPECT_TRUE(this->segmentDirectX < this->segmentDirectZ);
		EXPECT_TRUE(this->segmentDirectX < this->segmentReverseZ);

		EXPECT_FALSE(this->segmentReverseX < this->segmentDegenerateNegative);
		EXPECT_FALSE(this->segmentReverseX < this->segmentDegenerateZero);
		EXPECT_FALSE(this->segmentReverseX < this->segmentDegeneratePositive);
		EXPECT_FALSE(this->segmentReverseX < this->segmentDirectX);
		EXPECT_FALSE(this->segmentReverseX < this->segmentReverseX);
		EXPECT_TRUE(this->segmentReverseX < this->segmentDirectY);
		EXPECT_TRUE(this->segmentReverseX < this->segmentReverseY);
		EXPECT_TRUE(this->segmentReverseX < this->segmentDirectZ);
		EXPECT_TRUE(this->segmentReverseX < this->segmentReverseZ);

		EXPECT_FALSE(this->segmentDirectY < this->segmentDegenerateNegative);
		EXPECT_FALSE(this->segmentDirectY < this->segmentDegenerateZero);
		EXPECT_FALSE(this->segmentDirectY < this->segmentDegeneratePositive);
		EXPECT_FALSE(this->segmentDirectY < this->segmentDirectX);
		EXPECT_FALSE(this->segmentDirectY < this->segmentReverseX);
		EXPECT_FALSE(this->segmentDirectY < this->segmentDirectY);
		EXPECT_FALSE(this->segmentDirectY < this->segmentReverseY);
		EXPECT_TRUE(this->segmentDirectY < this->segmentDirectZ);
		EXPECT_TRUE(this->segmentDirectY < this->segmentReverseZ);

		EXPECT_FALSE(this->segmentReverseY < this->segmentDegenerateNegative);
		EXPECT_FALSE(this->segmentReverseY < this->segmentDegenerateZero);
		EXPECT_FALSE(this->segmentReverseY < this->segmentDegeneratePositive);
		EXPECT_FALSE(this->segmentReverseY < this->segmentDirectX);
		EXPECT_FALSE(this->segmentReverseY < this->segmentReverseX);
		EXPECT_FALSE(this->segmentReverseY < this->segmentDirectY);
		EXPECT_FALSE(this->segmentReverseY < this->segmentReverseY);
		EXPECT_TRUE(this->segmentReverseY < this->segmentDirectZ);
		EXPECT_TRUE(this->segmentReverseY < this->segmentReverseZ);

		EXPECT_FALSE(this->segmentDirectZ < this->segmentDegenerateNegative);
		EXPECT_FALSE(this->segmentDirectZ < this->segmentDegenerateZero);
		EXPECT_FALSE(this->segmentDirectZ < this->segmentDegeneratePositive);
		EXPECT_FALSE(this->segmentDirectZ < this->segmentDirectX);
		EXPECT_FALSE(this->segmentDirectZ < this->segmentReverseX);
		EXPECT_FALSE(this->segmentDirectZ < this->segmentDirectY);
		EXPECT_FALSE(this->segmentDirectZ < this->segmentReverseY);
		EXPECT_FALSE(this->segmentDirectZ < this->segmentDirectZ);
		EXPECT_FALSE(this->segmentDirectZ < this->segmentReverseZ);

		EXPECT_FALSE(this->segmentReverseZ < this->segmentDegenerateNegative);
		EXPECT_FALSE(this->segmentReverseZ < this->segmentDegenerateZero);
		EXPECT_FALSE(this->segmentReverseZ < this->segmentDegeneratePositive);
		EXPECT_FALSE(this->segmentReverseZ < this->segmentDirectX);
		EXPECT_FALSE(this->segmentReverseZ < this->segmentReverseX);
		EXPECT_FALSE(this->segmentReverseZ < this->segmentDirectY);
		EXPECT_FALSE(this->segmentReverseZ < this->segmentReverseY);
		EXPECT_FALSE(this->segmentReverseZ < this->segmentDirectZ);
		EXPECT_FALSE(this->segmentReverseZ < this->segmentReverseZ);
	}

	TYPED_TEST_P(GeometrySegmentTest, ComparisonOperatorLessEqualTo)
	{
		EXPECT_TRUE(this->segmentDegenerateNegative <= this->segmentDegenerateNegative);
		EXPECT_TRUE(this->segmentDegenerateNegative <= this->segmentDegenerateZero);
		EXPECT_TRUE(this->segmentDegenerateNegative <= this->segmentDegeneratePositive);
		EXPECT_TRUE(this->segmentDegenerateNegative <= this->segmentDirectX);
		EXPECT_TRUE(this->segmentDegenerateNegative <= this->segmentReverseX);
		EXPECT_TRUE(this->segmentDegenerateNegative <= this->segmentDirectY);
		EXPECT_TRUE(this->segmentDegenerateNegative <= this->segmentReverseY);
		EXPECT_TRUE(this->segmentDegenerateNegative <= this->segmentDirectZ);
		EXPECT_TRUE(this->segmentDegenerateNegative <= this->segmentReverseZ);

		EXPECT_FALSE(this->segmentDegenerateZero <= this->segmentDegenerateNegative);
		EXPECT_TRUE(this->segmentDegenerateZero <= this->segmentDegenerateZero);
		EXPECT_TRUE(this->segmentDegenerateZero <= this->segmentDegeneratePositive);
		EXPECT_TRUE(this->segmentDegenerateZero <= this->segmentDirectX);
		EXPECT_TRUE(this->segmentDegenerateZero <= this->segmentReverseX);
		EXPECT_TRUE(this->segmentDegenerateZero <= this->segmentDirectY);
		EXPECT_TRUE(this->segmentDegenerateZero <= this->segmentReverseY);
		EXPECT_TRUE(this->segmentDegenerateZero <= this->segmentDirectZ);
		EXPECT_TRUE(this->segmentDegenerateZero <= this->segmentReverseZ);

		EXPECT_FALSE(this->segmentDegeneratePositive <= this->segmentDegenerateNegative);
		EXPECT_FALSE(this->segmentDegeneratePositive <= this->segmentDegenerateZero);
		EXPECT_TRUE(this->segmentDegeneratePositive <= this->segmentDegeneratePositive);
		EXPECT_TRUE(this->segmentDegeneratePositive <= this->segmentDirectX);
		EXPECT_TRUE(this->segmentDegeneratePositive <= this->segmentReverseX);
		EXPECT_TRUE(this->segmentDegeneratePositive <= this->segmentDirectY);
		EXPECT_TRUE(this->segmentDegeneratePositive <= this->segmentReverseY);
		EXPECT_TRUE(this->segmentDegeneratePositive <= this->segmentDirectZ);
		EXPECT_TRUE(this->segmentDegeneratePositive <= this->segmentReverseZ);

		EXPECT_FALSE(this->segmentDirectX <= this->segmentDegenerateNegative);
		EXPECT_FALSE(this->segmentDirectX <= this->segmentDegenerateZero);
		EXPECT_FALSE(this->segmentDirectX <= this->segmentDegeneratePositive);
		EXPECT_TRUE(this->segmentDirectX <= this->segmentDirectX);
		EXPECT_TRUE(this->segmentDirectX <= this->segmentReverseX);
		EXPECT_TRUE(this->segmentDirectX <= this->segmentDirectY);
		EXPECT_TRUE(this->segmentDirectX <= this->segmentReverseY);
		EXPECT_TRUE(this->segmentDirectX <= this->segmentDirectZ);
		EXPECT_TRUE(this->segmentDirectX <= this->segmentReverseZ);

		EXPECT_FALSE(this->segmentReverseX <= this->segmentDegenerateNegative);
		EXPECT_FALSE(this->segmentReverseX <= this->segmentDegenerateZero);
		EXPECT_FALSE(this->segmentReverseX <= this->segmentDegeneratePositive);
		EXPECT_TRUE(this->segmentReverseX <= this->segmentDirectX);
		EXPECT_TRUE(this->segmentReverseX <= this->segmentReverseX);
		EXPECT_TRUE(this->segmentReverseX <= this->segmentDirectY);
		EXPECT_TRUE(this->segmentReverseX <= this->segmentReverseY);
		EXPECT_TRUE(this->segmentReverseX <= this->segmentDirectZ);
		EXPECT_TRUE(this->segmentReverseX <= this->segmentReverseZ);

		EXPECT_FALSE(this->segmentDirectY <= this->segmentDegenerateNegative);
		EXPECT_FALSE(this->segmentDirectY <= this->segmentDegenerateZero);
		EXPECT_FALSE(this->segmentDirectY <= this->segmentDegeneratePositive);
		EXPECT_FALSE(this->segmentDirectY <= this->segmentDirectX);
		EXPECT_FALSE(this->segmentDirectY <= this->segmentReverseX);
		EXPECT_TRUE(this->segmentDirectY <= this->segmentDirectY);
		EXPECT_TRUE(this->segmentDirectY <= this->segmentReverseY);
		EXPECT_TRUE(this->segmentDirectY <= this->segmentDirectZ);
		EXPECT_TRUE(this->segmentDirectY <= this->segmentReverseZ);

		EXPECT_FALSE(this->segmentReverseY <= this->segmentDegenerateNegative);
		EXPECT_FALSE(this->segmentReverseY <= this->segmentDegenerateZero);
		EXPECT_FALSE(this->segmentReverseY <= this->segmentDegeneratePositive);
		EXPECT_FALSE(this->segmentReverseY <= this->segmentDirectX);
		EXPECT_FALSE(this->segmentReverseY <= this->segmentReverseX);
		EXPECT_TRUE(this->segmentReverseY <= this->segmentDirectY);
		EXPECT_TRUE(this->segmentReverseY <= this->segmentReverseY);
		EXPECT_TRUE(this->segmentReverseY <= this->segmentDirectZ);
		EXPECT_TRUE(this->segmentReverseY <= this->segmentReverseZ);

		EXPECT_FALSE(this->segmentDirectZ <= this->segmentDegenerateNegative);
		EXPECT_FALSE(this->segmentDirectZ <= this->segmentDegenerateZero);
		EXPECT_FALSE(this->segmentDirectZ <= this->segmentDegeneratePositive);
		EXPECT_FALSE(this->segmentDirectZ <= this->segmentDirectX);
		EXPECT_FALSE(this->segmentDirectZ <= this->segmentReverseX);
		EXPECT_FALSE(this->segmentDirectZ <= this->segmentDirectY);
		EXPECT_FALSE(this->segmentDirectZ <= this->segmentReverseY);
		EXPECT_TRUE(this->segmentDirectZ <= this->segmentDirectZ);
		EXPECT_TRUE(this->segmentDirectZ <= this->segmentReverseZ);

		EXPECT_FALSE(this->segmentReverseZ <= this->segmentDegenerateNegative);
		EXPECT_FALSE(this->segmentReverseZ <= this->segmentDegenerateZero);
		EXPECT_FALSE(this->segmentReverseZ <= this->segmentDegeneratePositive);
		EXPECT_FALSE(this->segmentReverseZ <= this->segmentDirectX);
		EXPECT_FALSE(this->segmentReverseZ <= this->segmentReverseX);
		EXPECT_FALSE(this->segmentReverseZ <= this->segmentDirectY);
		EXPECT_FALSE(this->segmentReverseZ <= this->segmentReverseY);
		EXPECT_TRUE(this->segmentReverseZ <= this->segmentDirectZ);
		EXPECT_TRUE(this->segmentReverseZ <= this->segmentReverseZ);
	}

	TYPED_TEST_P(GeometrySegmentTest, ComparisonOperatorEqualTo)
	{
		EXPECT_TRUE(this->segmentDegenerateNegative == this->segmentDegenerateNegative);
		EXPECT_FALSE(this->segmentDegenerateNegative == this->segmentDegenerateZero);
		EXPECT_FALSE(this->segmentDegenerateNegative == this->segmentDegeneratePositive);
		EXPECT_FALSE(this->segmentDegenerateNegative == this->segmentDirectX);
		EXPECT_FALSE(this->segmentDegenerateNegative == this->segmentReverseX);
		EXPECT_FALSE(this->segmentDegenerateNegative == this->segmentDirectY);
		EXPECT_FALSE(this->segmentDegenerateNegative == this->segmentReverseY);
		EXPECT_FALSE(this->segmentDegenerateNegative == this->segmentDirectZ);
		EXPECT_FALSE(this->segmentDegenerateNegative == this->segmentReverseZ);

		EXPECT_FALSE(this->segmentDegenerateZero == this->segmentDegenerateNegative);
		EXPECT_TRUE(this->segmentDegenerateZero == this->segmentDegenerateZero);
		EXPECT_FALSE(this->segmentDegenerateZero == this->segmentDegeneratePositive);
		EXPECT_FALSE(this->segmentDegenerateZero == this->segmentDirectX);
		EXPECT_FALSE(this->segmentDegenerateZero == this->segmentReverseX);
		EXPECT_FALSE(this->segmentDegenerateZero == this->segmentDirectY);
		EXPECT_FALSE(this->segmentDegenerateZero == this->segmentReverseY);
		EXPECT_FALSE(this->segmentDegenerateZero == this->segmentDirectZ);
		EXPECT_FALSE(this->segmentDegenerateZero == this->segmentReverseZ);

		EXPECT_FALSE(this->segmentDegeneratePositive == this->segmentDegenerateNegative);
		EXPECT_FALSE(this->segmentDegeneratePositive == this->segmentDegenerateZero);
		EXPECT_TRUE(this->segmentDegeneratePositive == this->segmentDegeneratePositive);
		EXPECT_FALSE(this->segmentDegeneratePositive == this->segmentDirectX);
		EXPECT_FALSE(this->segmentDegeneratePositive == this->segmentReverseX);
		EXPECT_FALSE(this->segmentDegeneratePositive == this->segmentDirectY);
		EXPECT_FALSE(this->segmentDegeneratePositive == this->segmentReverseY);
		EXPECT_FALSE(this->segmentDegeneratePositive == this->segmentDirectZ);
		EXPECT_FALSE(this->segmentDegeneratePositive == this->segmentReverseZ);

		EXPECT_FALSE(this->segmentDirectX == this->segmentDegenerateNegative);
		EXPECT_FALSE(this->segmentDirectX == this->segmentDegenerateZero);
		EXPECT_FALSE(this->segmentDirectX == this->segmentDegeneratePositive);
		EXPECT_TRUE(this->segmentDirectX == this->segmentDirectX);
		EXPECT_TRUE(this->segmentDirectX == this->segmentReverseX);
		EXPECT_FALSE(this->segmentDirectX == this->segmentDirectY);
		EXPECT_FALSE(this->segmentDirectX == this->segmentReverseY);
		EXPECT_FALSE(this->segmentDirectX == this->segmentDirectZ);
		EXPECT_FALSE(this->segmentDirectX == this->segmentReverseZ);

		EXPECT_FALSE(this->segmentReverseX == this->segmentDegenerateNegative);
		EXPECT_FALSE(this->segmentReverseX == this->segmentDegenerateZero);
		EXPECT_FALSE(this->segmentReverseX == this->segmentDegeneratePositive);
		EXPECT_TRUE(this->segmentReverseX == this->segmentDirectX);
		EXPECT_TRUE(this->segmentReverseX == this->segmentReverseX);
		EXPECT_FALSE(this->segmentReverseX == this->segmentDirectY);
		EXPECT_FALSE(this->segmentReverseX == this->segmentReverseY);
		EXPECT_FALSE(this->segmentReverseX == this->segmentDirectZ);
		EXPECT_FALSE(this->segmentReverseX == this->segmentReverseZ);

		EXPECT_FALSE(this->segmentDirectY == this->segmentDegenerateNegative);
		EXPECT_FALSE(this->segmentDirectY == this->segmentDegenerateZero);
		EXPECT_FALSE(this->segmentDirectY == this->segmentDegeneratePositive);
		EXPECT_FALSE(this->segmentDirectY == this->segmentDirectX);
		EXPECT_FALSE(this->segmentDirectY == this->segmentReverseX);
		EXPECT_TRUE(this->segmentDirectY == this->segmentDirectY);
		EXPECT_TRUE(this->segmentDirectY == this->segmentReverseY);
		EXPECT_FALSE(this->segmentDirectY == this->segmentDirectZ);
		EXPECT_FALSE(this->segmentDirectY == this->segmentReverseZ);

		EXPECT_FALSE(this->segmentReverseY == this->segmentDegenerateNegative);
		EXPECT_FALSE(this->segmentReverseY == this->segmentDegenerateZero);
		EXPECT_FALSE(this->segmentReverseY == this->segmentDegeneratePositive);
		EXPECT_FALSE(this->segmentReverseY == this->segmentDirectX);
		EXPECT_FALSE(this->segmentReverseY == this->segmentReverseX);
		EXPECT_TRUE(this->segmentReverseY == this->segmentDirectY);
		EXPECT_TRUE(this->segmentReverseY == this->segmentReverseY);
		EXPECT_FALSE(this->segmentReverseY == this->segmentDirectZ);
		EXPECT_FALSE(this->segmentReverseY == this->segmentReverseZ);

		EXPECT_FALSE(this->segmentDirectZ == this->segmentDegenerateNegative);
		EXPECT_FALSE(this->segmentDirectZ == this->segmentDegenerateZero);
		EXPECT_FALSE(this->segmentDirectZ == this->segmentDegeneratePositive);
		EXPECT_FALSE(this->segmentDirectZ == this->segmentDirectX);
		EXPECT_FALSE(this->segmentDirectZ == this->segmentReverseX);
		EXPECT_FALSE(this->segmentDirectZ == this->segmentDirectY);
		EXPECT_FALSE(this->segmentDirectZ == this->segmentReverseY);
		EXPECT_TRUE(this->segmentDirectZ == this->segmentDirectZ);
		EXPECT_TRUE(this->segmentDirectZ == this->segmentReverseZ);

		EXPECT_FALSE(this->segmentReverseZ == this->segmentDegenerateNegative);
		EXPECT_FALSE(this->segmentReverseZ == this->segmentDegenerateZero);
		EXPECT_FALSE(this->segmentReverseZ == this->segmentDegeneratePositive);
		EXPECT_FALSE(this->segmentReverseZ == this->segmentDirectX);
		EXPECT_FALSE(this->segmentReverseZ == this->segmentReverseX);
		EXPECT_FALSE(this->segmentReverseZ == this->segmentDirectY);
		EXPECT_FALSE(this->segmentReverseZ == this->segmentReverseY);
		EXPECT_TRUE(this->segmentReverseZ == this->segmentDirectZ);
		EXPECT_TRUE(this->segmentReverseZ == this->segmentReverseZ);
	}

	TYPED_TEST_P(GeometrySegmentTest, ComparisonOperatorGreaterEqualTo)
	{
		EXPECT_TRUE(this->segmentDegenerateNegative >= this->segmentDegenerateNegative);
		EXPECT_FALSE(this->segmentDegenerateNegative >= this->segmentDegenerateZero);
		EXPECT_FALSE(this->segmentDegenerateNegative >= this->segmentDegeneratePositive);
		EXPECT_FALSE(this->segmentDegenerateNegative >= this->segmentDirectX);
		EXPECT_FALSE(this->segmentDegenerateNegative >= this->segmentReverseX);
		EXPECT_FALSE(this->segmentDegenerateNegative >= this->segmentDirectY);
		EXPECT_FALSE(this->segmentDegenerateNegative >= this->segmentReverseY);
		EXPECT_FALSE(this->segmentDegenerateNegative >= this->segmentDirectZ);
		EXPECT_FALSE(this->segmentDegenerateNegative >= this->segmentReverseZ);

		EXPECT_TRUE(this->segmentDegenerateZero >= this->segmentDegenerateNegative);
		EXPECT_TRUE(this->segmentDegenerateZero >= this->segmentDegenerateZero);
		EXPECT_FALSE(this->segmentDegenerateZero >= this->segmentDegeneratePositive);
		EXPECT_FALSE(this->segmentDegenerateZero >= this->segmentDirectX);
		EXPECT_FALSE(this->segmentDegenerateZero >= this->segmentReverseX);
		EXPECT_FALSE(this->segmentDegenerateZero >= this->segmentDirectY);
		EXPECT_FALSE(this->segmentDegenerateZero >= this->segmentReverseY);
		EXPECT_FALSE(this->segmentDegenerateZero >= this->segmentDirectZ);
		EXPECT_FALSE(this->segmentDegenerateZero >= this->segmentReverseZ);

		EXPECT_TRUE(this->segmentDegeneratePositive >= this->segmentDegenerateNegative);
		EXPECT_TRUE(this->segmentDegeneratePositive >= this->segmentDegenerateZero);
		EXPECT_TRUE(this->segmentDegeneratePositive >= this->segmentDegeneratePositive);
		EXPECT_FALSE(this->segmentDegeneratePositive >= this->segmentDirectX);
		EXPECT_FALSE(this->segmentDegeneratePositive >= this->segmentReverseX);
		EXPECT_FALSE(this->segmentDegeneratePositive >= this->segmentDirectY);
		EXPECT_FALSE(this->segmentDegeneratePositive >= this->segmentReverseY);
		EXPECT_FALSE(this->segmentDegeneratePositive >= this->segmentDirectZ);
		EXPECT_FALSE(this->segmentDegeneratePositive >= this->segmentReverseZ);

		EXPECT_TRUE(this->segmentDirectX >= this->segmentDegenerateNegative);
		EXPECT_TRUE(this->segmentDirectX >= this->segmentDegenerateZero);
		EXPECT_TRUE(this->segmentDirectX >= this->segmentDegeneratePositive);
		EXPECT_TRUE(this->segmentDirectX >= this->segmentDirectX);
		EXPECT_TRUE(this->segmentDirectX >= this->segmentReverseX);
		EXPECT_FALSE(this->segmentDirectX >= this->segmentDirectY);
		EXPECT_FALSE(this->segmentDirectX >= this->segmentReverseY);
		EXPECT_FALSE(this->segmentDirectX >= this->segmentDirectZ);
		EXPECT_FALSE(this->segmentDirectX >= this->segmentReverseZ);

		EXPECT_TRUE(this->segmentReverseX >= this->segmentDegenerateNegative);
		EXPECT_TRUE(this->segmentReverseX >= this->segmentDegenerateZero);
		EXPECT_TRUE(this->segmentReverseX >= this->segmentDegeneratePositive);
		EXPECT_TRUE(this->segmentReverseX >= this->segmentDirectX);
		EXPECT_TRUE(this->segmentReverseX >= this->segmentReverseX);
		EXPECT_FALSE(this->segmentReverseX >= this->segmentDirectY);
		EXPECT_FALSE(this->segmentReverseX >= this->segmentReverseY);
		EXPECT_FALSE(this->segmentReverseX >= this->segmentDirectZ);
		EXPECT_FALSE(this->segmentReverseX >= this->segmentReverseZ);

		EXPECT_TRUE(this->segmentDirectY >= this->segmentDegenerateNegative);
		EXPECT_TRUE(this->segmentDirectY >= this->segmentDegenerateZero);
		EXPECT_TRUE(this->segmentDirectY >= this->segmentDegeneratePositive);
		EXPECT_TRUE(this->segmentDirectY >= this->segmentDirectX);
		EXPECT_TRUE(this->segmentDirectY >= this->segmentReverseX);
		EXPECT_TRUE(this->segmentDirectY >= this->segmentDirectY);
		EXPECT_TRUE(this->segmentDirectY >= this->segmentReverseY);
		EXPECT_FALSE(this->segmentDirectY >= this->segmentDirectZ);
		EXPECT_FALSE(this->segmentDirectY >= this->segmentReverseZ);

		EXPECT_TRUE(this->segmentReverseY >= this->segmentDegenerateNegative);
		EXPECT_TRUE(this->segmentReverseY >= this->segmentDegenerateZero);
		EXPECT_TRUE(this->segmentReverseY >= this->segmentDegeneratePositive);
		EXPECT_TRUE(this->segmentReverseY >= this->segmentDirectX);
		EXPECT_TRUE(this->segmentReverseY >= this->segmentReverseX);
		EXPECT_TRUE(this->segmentReverseY >= this->segmentDirectY);
		EXPECT_TRUE(this->segmentReverseY >= this->segmentReverseY);
		EXPECT_FALSE(this->segmentReverseY >= this->segmentDirectZ);
		EXPECT_FALSE(this->segmentReverseY >= this->segmentReverseZ);

		EXPECT_TRUE(this->segmentDirectZ >= this->segmentDegenerateNegative);
		EXPECT_TRUE(this->segmentDirectZ >= this->segmentDegenerateZero);
		EXPECT_TRUE(this->segmentDirectZ >= this->segmentDegeneratePositive);
		EXPECT_TRUE(this->segmentDirectZ >= this->segmentDirectX);
		EXPECT_TRUE(this->segmentDirectZ >= this->segmentReverseX);
		EXPECT_TRUE(this->segmentDirectZ >= this->segmentDirectY);
		EXPECT_TRUE(this->segmentDirectZ >= this->segmentReverseY);
		EXPECT_TRUE(this->segmentDirectZ >= this->segmentDirectZ);
		EXPECT_TRUE(this->segmentDirectZ >= this->segmentReverseZ);

		EXPECT_TRUE(this->segmentReverseZ >= this->segmentDegenerateNegative);
		EXPECT_TRUE(this->segmentReverseZ >= this->segmentDegenerateZero);
		EXPECT_TRUE(this->segmentReverseZ >= this->segmentDegeneratePositive);
		EXPECT_TRUE(this->segmentReverseZ >= this->segmentDirectX);
		EXPECT_TRUE(this->segmentReverseZ >= this->segmentReverseX);
		EXPECT_TRUE(this->segmentReverseZ >= this->segmentDirectY);
		EXPECT_TRUE(this->segmentReverseZ >= this->segmentReverseY);
		EXPECT_TRUE(this->segmentReverseZ >= this->segmentDirectZ);
		EXPECT_TRUE(this->segmentReverseZ >= this->segmentReverseZ);
	}

	TYPED_TEST_P(GeometrySegmentTest, ComparisonOperatorGreater)
	{
		EXPECT_FALSE(this->segmentDegenerateNegative > this->segmentDegenerateNegative);
		EXPECT_FALSE(this->segmentDegenerateNegative > this->segmentDegenerateZero);
		EXPECT_FALSE(this->segmentDegenerateNegative > this->segmentDegeneratePositive);
		EXPECT_FALSE(this->segmentDegenerateNegative > this->segmentDirectX);
		EXPECT_FALSE(this->segmentDegenerateNegative > this->segmentReverseX);
		EXPECT_FALSE(this->segmentDegenerateNegative > this->segmentDirectY);
		EXPECT_FALSE(this->segmentDegenerateNegative > this->segmentReverseY);
		EXPECT_FALSE(this->segmentDegenerateNegative > this->segmentDirectZ);
		EXPECT_FALSE(this->segmentDegenerateNegative > this->segmentReverseZ);

		EXPECT_TRUE(this->segmentDegenerateZero > this->segmentDegenerateNegative);
		EXPECT_FALSE(this->segmentDegenerateZero > this->segmentDegenerateZero);
		EXPECT_FALSE(this->segmentDegenerateZero > this->segmentDegeneratePositive);
		EXPECT_FALSE(this->segmentDegenerateZero > this->segmentDirectX);
		EXPECT_FALSE(this->segmentDegenerateZero > this->segmentReverseX);
		EXPECT_FALSE(this->segmentDegenerateZero > this->segmentDirectY);
		EXPECT_FALSE(this->segmentDegenerateZero > this->segmentReverseY);
		EXPECT_FALSE(this->segmentDegenerateZero > this->segmentDirectZ);
		EXPECT_FALSE(this->segmentDegenerateZero > this->segmentReverseZ);

		EXPECT_TRUE(this->segmentDegeneratePositive > this->segmentDegenerateNegative);
		EXPECT_TRUE(this->segmentDegeneratePositive > this->segmentDegenerateZero);
		EXPECT_FALSE(this->segmentDegeneratePositive > this->segmentDegeneratePositive);
		EXPECT_FALSE(this->segmentDegeneratePositive > this->segmentDirectX);
		EXPECT_FALSE(this->segmentDegeneratePositive > this->segmentReverseX);
		EXPECT_FALSE(this->segmentDegeneratePositive > this->segmentDirectY);
		EXPECT_FALSE(this->segmentDegeneratePositive > this->segmentReverseY);
		EXPECT_FALSE(this->segmentDegeneratePositive > this->segmentDirectZ);
		EXPECT_FALSE(this->segmentDegeneratePositive > this->segmentReverseZ);

		EXPECT_TRUE(this->segmentDirectX > this->segmentDegenerateNegative);
		EXPECT_TRUE(this->segmentDirectX > this->segmentDegenerateZero);
		EXPECT_TRUE(this->segmentDirectX > this->segmentDegeneratePositive);
		EXPECT_FALSE(this->segmentDirectX > this->segmentDirectX);
		EXPECT_FALSE(this->segmentDirectX > this->segmentReverseX);
		EXPECT_FALSE(this->segmentDirectX > this->segmentDirectY);
		EXPECT_FALSE(this->segmentDirectX > this->segmentReverseY);
		EXPECT_FALSE(this->segmentDirectX > this->segmentDirectZ);
		EXPECT_FALSE(this->segmentDirectX > this->segmentReverseZ);

		EXPECT_TRUE(this->segmentReverseX > this->segmentDegenerateNegative);
		EXPECT_TRUE(this->segmentReverseX > this->segmentDegenerateZero);
		EXPECT_TRUE(this->segmentReverseX > this->segmentDegeneratePositive);
		EXPECT_FALSE(this->segmentReverseX > this->segmentDirectX);
		EXPECT_FALSE(this->segmentReverseX > this->segmentReverseX);
		EXPECT_FALSE(this->segmentReverseX > this->segmentDirectY);
		EXPECT_FALSE(this->segmentReverseX > this->segmentReverseY);
		EXPECT_FALSE(this->segmentReverseX > this->segmentDirectZ);
		EXPECT_FALSE(this->segmentReverseX > this->segmentReverseZ);

		EXPECT_TRUE(this->segmentDirectY > this->segmentDegenerateNegative);
		EXPECT_TRUE(this->segmentDirectY > this->segmentDegenerateZero);
		EXPECT_TRUE(this->segmentDirectY > this->segmentDegeneratePositive);
		EXPECT_TRUE(this->segmentDirectY > this->segmentDirectX);
		EXPECT_TRUE(this->segmentDirectY > this->segmentReverseX);
		EXPECT_FALSE(this->segmentDirectY > this->segmentDirectY);
		EXPECT_FALSE(this->segmentDirectY > this->segmentReverseY);
		EXPECT_FALSE(this->segmentDirectY > this->segmentDirectZ);
		EXPECT_FALSE(this->segmentDirectY > this->segmentReverseZ);

		EXPECT_TRUE(this->segmentReverseY > this->segmentDegenerateNegative);
		EXPECT_TRUE(this->segmentReverseY > this->segmentDegenerateZero);
		EXPECT_TRUE(this->segmentReverseY > this->segmentDegeneratePositive);
		EXPECT_TRUE(this->segmentReverseY > this->segmentDirectX);
		EXPECT_TRUE(this->segmentReverseY > this->segmentReverseX);
		EXPECT_FALSE(this->segmentReverseY > this->segmentDirectY);
		EXPECT_FALSE(this->segmentReverseY > this->segmentReverseY);
		EXPECT_FALSE(this->segmentReverseY > this->segmentDirectZ);
		EXPECT_FALSE(this->segmentReverseY > this->segmentReverseZ);

		EXPECT_TRUE(this->segmentDirectZ > this->segmentDegenerateNegative);
		EXPECT_TRUE(this->segmentDirectZ > this->segmentDegenerateZero);
		EXPECT_TRUE(this->segmentDirectZ > this->segmentDegeneratePositive);
		EXPECT_TRUE(this->segmentDirectZ > this->segmentDirectX);
		EXPECT_TRUE(this->segmentDirectZ > this->segmentReverseX);
		EXPECT_TRUE(this->segmentDirectZ > this->segmentDirectY);
		EXPECT_TRUE(this->segmentDirectZ > this->segmentReverseY);
		EXPECT_FALSE(this->segmentDirectZ > this->segmentDirectZ);
		EXPECT_FALSE(this->segmentDirectZ > this->segmentReverseZ);

		EXPECT_TRUE(this->segmentReverseZ > this->segmentDegenerateNegative);
		EXPECT_TRUE(this->segmentReverseZ > this->segmentDegenerateZero);
		EXPECT_TRUE(this->segmentReverseZ > this->segmentDegeneratePositive);
		EXPECT_TRUE(this->segmentReverseZ > this->segmentDirectX);
		EXPECT_TRUE(this->segmentReverseZ > this->segmentReverseX);
		EXPECT_TRUE(this->segmentReverseZ > this->segmentDirectY);
		EXPECT_TRUE(this->segmentReverseZ > this->segmentReverseY);
		EXPECT_FALSE(this->segmentReverseZ > this->segmentDirectZ);
		EXPECT_FALSE(this->segmentReverseZ > this->segmentReverseZ);
	}

	TYPED_TEST_P(GeometrySegmentTest, ComparisonOperatorNotEqualTo)
	{
		EXPECT_FALSE(this->segmentDegenerateNegative != this->segmentDegenerateNegative);
		EXPECT_TRUE(this->segmentDegenerateNegative != this->segmentDegenerateZero);
		EXPECT_TRUE(this->segmentDegenerateNegative != this->segmentDegeneratePositive);
		EXPECT_TRUE(this->segmentDegenerateNegative != this->segmentDirectX);
		EXPECT_TRUE(this->segmentDegenerateNegative != this->segmentReverseX);
		EXPECT_TRUE(this->segmentDegenerateNegative != this->segmentDirectY);
		EXPECT_TRUE(this->segmentDegenerateNegative != this->segmentReverseY);
		EXPECT_TRUE(this->segmentDegenerateNegative != this->segmentDirectZ);
		EXPECT_TRUE(this->segmentDegenerateNegative != this->segmentReverseZ);

		EXPECT_TRUE(this->segmentDegenerateZero != this->segmentDegenerateNegative);
		EXPECT_FALSE(this->segmentDegenerateZero != this->segmentDegenerateZero);
		EXPECT_TRUE(this->segmentDegenerateZero != this->segmentDegeneratePositive);
		EXPECT_TRUE(this->segmentDegenerateZero != this->segmentDirectX);
		EXPECT_TRUE(this->segmentDegenerateZero != this->segmentReverseX);
		EXPECT_TRUE(this->segmentDegenerateZero != this->segmentDirectY);
		EXPECT_TRUE(this->segmentDegenerateZero != this->segmentReverseY);
		EXPECT_TRUE(this->segmentDegenerateZero != this->segmentDirectZ);
		EXPECT_TRUE(this->segmentDegenerateZero != this->segmentReverseZ);

		EXPECT_TRUE(this->segmentDegeneratePositive != this->segmentDegenerateNegative);
		EXPECT_TRUE(this->segmentDegeneratePositive != this->segmentDegenerateZero);
		EXPECT_FALSE(this->segmentDegeneratePositive != this->segmentDegeneratePositive);
		EXPECT_TRUE(this->segmentDegeneratePositive != this->segmentDirectX);
		EXPECT_TRUE(this->segmentDegeneratePositive != this->segmentReverseX);
		EXPECT_TRUE(this->segmentDegeneratePositive != this->segmentDirectY);
		EXPECT_TRUE(this->segmentDegeneratePositive != this->segmentReverseY);
		EXPECT_TRUE(this->segmentDegeneratePositive != this->segmentDirectZ);
		EXPECT_TRUE(this->segmentDegeneratePositive != this->segmentReverseZ);

		EXPECT_TRUE(this->segmentDirectX != this->segmentDegenerateNegative);
		EXPECT_TRUE(this->segmentDirectX != this->segmentDegenerateZero);
		EXPECT_TRUE(this->segmentDirectX != this->segmentDegeneratePositive);
		EXPECT_FALSE(this->segmentDirectX != this->segmentDirectX);
		EXPECT_FALSE(this->segmentDirectX != this->segmentReverseX);
		EXPECT_TRUE(this->segmentDirectX != this->segmentDirectY);
		EXPECT_TRUE(this->segmentDirectX != this->segmentReverseY);
		EXPECT_TRUE(this->segmentDirectX != this->segmentDirectZ);
		EXPECT_TRUE(this->segmentDirectX != this->segmentReverseZ);

		EXPECT_TRUE(this->segmentReverseX != this->segmentDegenerateNegative);
		EXPECT_TRUE(this->segmentReverseX != this->segmentDegenerateZero);
		EXPECT_TRUE(this->segmentReverseX != this->segmentDegeneratePositive);
		EXPECT_FALSE(this->segmentReverseX != this->segmentDirectX);
		EXPECT_FALSE(this->segmentReverseX != this->segmentReverseX);
		EXPECT_TRUE(this->segmentReverseX != this->segmentDirectY);
		EXPECT_TRUE(this->segmentReverseX != this->segmentReverseY);
		EXPECT_TRUE(this->segmentReverseX != this->segmentDirectZ);
		EXPECT_TRUE(this->segmentReverseX != this->segmentReverseZ);

		EXPECT_TRUE(this->segmentDirectY != this->segmentDegenerateNegative);
		EXPECT_TRUE(this->segmentDirectY != this->segmentDegenerateZero);
		EXPECT_TRUE(this->segmentDirectY != this->segmentDegeneratePositive);
		EXPECT_TRUE(this->segmentDirectY != this->segmentDirectX);
		EXPECT_TRUE(this->segmentDirectY != this->segmentReverseX);
		EXPECT_FALSE(this->segmentDirectY != this->segmentDirectY);
		EXPECT_FALSE(this->segmentDirectY != this->segmentReverseY);
		EXPECT_TRUE(this->segmentDirectY != this->segmentDirectZ);
		EXPECT_TRUE(this->segmentDirectY != this->segmentReverseZ);

		EXPECT_TRUE(this->segmentReverseY != this->segmentDegenerateNegative);
		EXPECT_TRUE(this->segmentReverseY != this->segmentDegenerateZero);
		EXPECT_TRUE(this->segmentReverseY != this->segmentDegeneratePositive);
		EXPECT_TRUE(this->segmentReverseY != this->segmentDirectX);
		EXPECT_TRUE(this->segmentReverseY != this->segmentReverseX);
		EXPECT_FALSE(this->segmentReverseY != this->segmentDirectY);
		EXPECT_FALSE(this->segmentReverseY != this->segmentReverseY);
		EXPECT_TRUE(this->segmentReverseY != this->segmentDirectZ);
		EXPECT_TRUE(this->segmentReverseY != this->segmentReverseZ);

		EXPECT_TRUE(this->segmentDirectZ != this->segmentDegenerateNegative);
		EXPECT_TRUE(this->segmentDirectZ != this->segmentDegenerateZero);
		EXPECT_TRUE(this->segmentDirectZ != this->segmentDegeneratePositive);
		EXPECT_TRUE(this->segmentDirectZ != this->segmentDirectX);
		EXPECT_TRUE(this->segmentDirectZ != this->segmentReverseX);
		EXPECT_TRUE(this->segmentDirectZ != this->segmentDirectY);
		EXPECT_TRUE(this->segmentDirectZ != this->segmentReverseY);
		EXPECT_FALSE(this->segmentDirectZ != this->segmentDirectZ);
		EXPECT_FALSE(this->segmentDirectZ != this->segmentReverseZ);

		EXPECT_TRUE(this->segmentReverseZ != this->segmentDegenerateNegative);
		EXPECT_TRUE(this->segmentReverseZ != this->segmentDegenerateZero);
		EXPECT_TRUE(this->segmentReverseZ != this->segmentDegeneratePositive);
		EXPECT_TRUE(this->segmentReverseZ != this->segmentDirectX);
		EXPECT_TRUE(this->segmentReverseZ != this->segmentReverseX);
		EXPECT_TRUE(this->segmentReverseZ != this->segmentDirectY);
		EXPECT_TRUE(this->segmentReverseZ != this->segmentReverseY);
		EXPECT_FALSE(this->segmentReverseZ != this->segmentDirectZ);
		EXPECT_FALSE(this->segmentReverseZ != this->segmentReverseZ);
	}

	TYPED_TEST_P(GeometrySegmentTest, Distance)
	{
		EXPECT_EQ(this->segmentDegenerateNegative.distance(), typename TypeParam::value_type{0});
		EXPECT_EQ(this->segmentDegenerateZero.distance(), typename TypeParam::value_type{0});
		EXPECT_EQ(this->segmentDegeneratePositive.distance(), typename TypeParam::value_type{0});
		EXPECT_EQ(this->segmentDirectX.distance(), typename TypeParam::value_type{2});
		EXPECT_EQ(this->segmentReverseX.distance(), typename TypeParam::value_type{2});
		EXPECT_EQ(this->segmentDirectY.distance(), typename TypeParam::value_type{2});
		EXPECT_EQ(this->segmentReverseY.distance(), typename TypeParam::value_type{2});
		EXPECT_EQ(this->segmentDirectZ.distance(), typename TypeParam::value_type{2});
		EXPECT_EQ(this->segmentReverseZ.distance(), typename TypeParam::value_type{2});
	}

	TYPED_TEST_P(GeometrySegmentTest, DistanceToVertex)
	{
		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > vertexFullNegative{
			std::to_array(
				{
					typename TypeParam::value_type(-1),
					typename TypeParam::value_type(-1),
					typename TypeParam::value_type(-1)
				})};

		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > vertexFullPositive{
			std::to_array(
				{
					typename TypeParam::value_type(1),
					typename TypeParam::value_type(1),
					typename TypeParam::value_type(1)
				})};

		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 0 > > vertexOrigin{};

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->segmentDirectX.template distance_to < false > (vertexFullNegative),
				static_cast < typename TypeParam::value_type > (std::sqrt(2))));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->segmentDirectX.template distance_to < false > (vertexFullPositive),
				static_cast < typename TypeParam::value_type > (std::sqrt(2))));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->segmentDirectX.template distance_to < false > (vertexOrigin),
				static_cast < typename TypeParam::value_type > (0)));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->segmentReverseZ.template distance_to < false > (vertexFullNegative),
				static_cast < typename TypeParam::value_type > (std::sqrt(2))));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->segmentReverseZ.template distance_to < false > (vertexFullPositive),
				static_cast < typename TypeParam::value_type > (std::sqrt(2))));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->segmentReverseZ.template distance_to < false > (vertexOrigin),
				static_cast < typename TypeParam::value_type > (0)));

		EXPECT_EQ(
			this->segmentDirectX.template distance_to < true > (vertexFullNegative).second,
			this->segmentDirectX.vertex1);

		EXPECT_EQ(
			this->segmentDirectX.template distance_to < true > (vertexFullPositive).second,
			this->segmentDirectX.vertex2);

		EXPECT_EQ(
			this->segmentDirectX.template distance_to < true > (vertexOrigin).second,
			vertexOrigin);

		EXPECT_EQ(
			this->segmentReverseZ.template distance_to < true > (vertexFullNegative).second,
			this->segmentReverseZ.vertex2);

		EXPECT_EQ(
			this->segmentReverseZ.template distance_to < true > (vertexFullPositive).second,
			this->segmentReverseZ.vertex1);

		EXPECT_EQ(
			this->segmentReverseZ.template distance_to < true > (vertexOrigin).second,
			vertexOrigin);
	}

	TYPED_TEST_P(GeometrySegmentTest, DistanceToSegment)
	{
		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 0 > > vertexOrigin{};

		constexpr xablau::geometry::segment < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > segmentCrossingOrigin{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type(-1),
					typename TypeParam::value_type(-1),
					typename TypeParam::value_type(-1)
				})),
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type(1),
					typename TypeParam::value_type(1),
					typename TypeParam::value_type(1)
				}))};

		constexpr xablau::geometry::segment < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > segmentOnCorner{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type(2),
					typename TypeParam::value_type(2),
					typename TypeParam::value_type(2)
				})),
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type(1),
					typename TypeParam::value_type(1),
					typename TypeParam::value_type(1)
				}))};

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->segmentDirectX.distance_to < false > (segmentCrossingOrigin),
				typename TypeParam::value_type{}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->segmentDirectX.distance_to < false > (segmentOnCorner),
				static_cast < typename TypeParam::value_type > (std::sqrt(2))));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->segmentReverseZ.distance_to < false > (segmentCrossingOrigin),
				typename TypeParam::value_type{}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->segmentReverseZ.distance_to < false > (segmentOnCorner),
				static_cast < typename TypeParam::value_type > (std::sqrt(2))));

		xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > vertex1{};
		xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > vertex2{};

		std::tie(std::ignore, vertex1, vertex2) =
			this->segmentDirectX.distance_to < true > (segmentCrossingOrigin);

		EXPECT_EQ(vertex1, vertexOrigin);
		EXPECT_EQ(vertex2, vertexOrigin);

		std::tie(std::ignore, vertex1, vertex2) =
			this->segmentDirectX.distance_to < true > (segmentOnCorner);

		EXPECT_EQ(vertex1, this->segmentDirectX.vertex2);
		EXPECT_EQ(vertex2, segmentOnCorner.vertex2);

		std::tie(std::ignore, vertex1, vertex2) =
			this->segmentReverseZ.distance_to < true > (segmentCrossingOrigin);

		EXPECT_EQ(vertex1, vertexOrigin);
		EXPECT_EQ(vertex2, vertexOrigin);

		std::tie(std::ignore, vertex1, vertex2) =
			this->segmentReverseZ.distance_to < true > (segmentOnCorner);

		EXPECT_EQ(vertex1, this->segmentReverseZ.vertex1);
		EXPECT_EQ(vertex2, segmentOnCorner.vertex2);
	}

	TYPED_TEST_P(GeometrySegmentTest, Containment)
	{
		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 0 > > vertexOrigin{};

		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > vertexOutOfRange{
			std::to_array(
				{
					typename TypeParam::value_type(0.5),
					typename TypeParam::value_type(0.5),
					typename TypeParam::value_type(0.5)
				})};

		EXPECT_TRUE(this->segmentDirectX.contains(this->segmentDirectX.vertex1));
		EXPECT_TRUE(this->segmentDirectX.contains(this->segmentDirectX.vertex2));
		EXPECT_TRUE(this->segmentDirectX.contains(vertexOrigin));

		EXPECT_TRUE(this->segmentReverseZ.contains(this->segmentReverseZ.vertex1));
		EXPECT_TRUE(this->segmentReverseZ.contains(this->segmentReverseZ.vertex2));
		EXPECT_TRUE(this->segmentReverseZ.contains(vertexOrigin));

		EXPECT_FALSE(this->segmentDirectX.contains(vertexOutOfRange));
		EXPECT_FALSE(this->segmentReverseX.contains(vertexOutOfRange));
		EXPECT_FALSE(this->segmentDirectY.contains(vertexOutOfRange));
		EXPECT_FALSE(this->segmentReverseY.contains(vertexOutOfRange));
		EXPECT_FALSE(this->segmentDirectZ.contains(vertexOutOfRange));
		EXPECT_FALSE(this->segmentReverseZ.contains(vertexOutOfRange));
	}

	TYPED_TEST_P(GeometrySegmentTest, Intersection)
	{
		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 0 > > vertexOrigin{};

		constexpr auto vertexDiagonal = xablau::geometry::make_vertex(std::to_array(
			{
				typename TypeParam::value_type(2),
				typename TypeParam::value_type(2)
			}));

		constexpr xablau::geometry::segment < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > segmentDiagonal1{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type(1),
					typename TypeParam::value_type(1)
				})),
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type(3),
					typename TypeParam::value_type(3)
				}))};

		constexpr xablau::geometry::segment < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > segmentDiagonal2{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type(1),
					typename TypeParam::value_type(1)
				})),
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type(2),
					typename TypeParam::value_type(2)
				}))};

		constexpr xablau::geometry::segment < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > segmentDiagonal3{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type(2),
					typename TypeParam::value_type(2)
				})),
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type(3),
					typename TypeParam::value_type(3)
				}))};

		EXPECT_TRUE(this->segmentDegenerateZero.intersects < false > (this->segmentDirectX));

		EXPECT_TRUE(this->segmentDirectX.intersects < false > (this->segmentDirectX));
		EXPECT_TRUE(this->segmentDirectX.intersects < false > (this->segmentDirectY));
		EXPECT_TRUE(this->segmentDirectX.intersects < false > (this->segmentDirectZ));

		EXPECT_TRUE(this->segmentReverseZ.intersects < false > (this->segmentReverseX));
		EXPECT_TRUE(this->segmentReverseZ.intersects < false > (this->segmentReverseY));
		EXPECT_TRUE(this->segmentReverseZ.intersects < false > (this->segmentReverseZ));

		EXPECT_FALSE(this->segmentReverseX.intersects < false > (segmentDiagonal1));
		EXPECT_FALSE(this->segmentReverseY.intersects < false > (segmentDiagonal1));
		EXPECT_FALSE(this->segmentReverseZ.intersects < false > (segmentDiagonal1));

		EXPECT_TRUE(segmentDiagonal1.intersects < false > (segmentDiagonal2));
		EXPECT_TRUE(segmentDiagonal1.intersects < false > (segmentDiagonal3));
		EXPECT_TRUE(segmentDiagonal2.intersects < false > (segmentDiagonal3));

		EXPECT_EQ(this->segmentDegenerateZero.intersects < true > (this->segmentDirectX), vertexOrigin);

		EXPECT_EQ(this->segmentDirectX.intersects < true > (this->segmentReverseY).value(), vertexOrigin);
		EXPECT_EQ(this->segmentDirectX.intersects < true > (this->segmentReverseZ).value(), vertexOrigin);

		EXPECT_EQ(this->segmentDirectY.intersects < true > (this->segmentReverseX).value(), vertexOrigin);
		EXPECT_EQ(this->segmentDirectY.intersects < true > (this->segmentReverseZ).value(), vertexOrigin);

		EXPECT_EQ(this->segmentDirectZ.intersects < true > (this->segmentReverseX).value(), vertexOrigin);
		EXPECT_EQ(this->segmentDirectZ.intersects < true > (this->segmentReverseY).value(), vertexOrigin);

		EXPECT_EQ(this->segmentReverseX.intersects < true > (segmentDiagonal1), std::nullopt);
		EXPECT_EQ(this->segmentReverseY.intersects < true > (segmentDiagonal1), std::nullopt);
		EXPECT_EQ(this->segmentReverseZ.intersects < true > (segmentDiagonal1), std::nullopt);

		EXPECT_EQ(segmentDiagonal2.intersects < true > (segmentDiagonal3).value(), vertexDiagonal);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		GeometrySegmentTest,
		ComparisonOperatorLess,
		ComparisonOperatorLessEqualTo,
		ComparisonOperatorEqualTo,
		ComparisonOperatorGreaterEqualTo,
		ComparisonOperatorGreater,
		ComparisonOperatorNotEqualTo,
		Distance,
		DistanceToVertex,
		DistanceToSegment,
		Containment,
		Intersection);

	using GeometrySegmentTestDataTypes = std::tuple < float, double, long double >;

	using GeometrySegmentTestGroup =
		xablau::testing::test_bundler < GeometrySegmentTestDataTypes > ::template test_group < GeometrySegmentTypeDefinitions >;

	using GeometrySegmentTypes = xablau::testing::tuple_transformation < ::testing::Types, GeometrySegmentTestGroup > ::types;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		Xablau,
		GeometrySegmentTest,
		GeometrySegmentTypes);
}
