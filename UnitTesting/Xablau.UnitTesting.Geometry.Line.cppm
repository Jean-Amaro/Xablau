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
	struct GeometryLineTypeDefinitions
	{
		using value_type = Type;
	};

	template < typename Types >
	class GeometryLineTest : public ::testing::Test
	{
	public:
		static constexpr xablau::geometry::line < typename Types::value_type, xablau::geometry::spatial_dimension < 1 > > lineDegenerateNegative{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename Types::value_type(-1)
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename Types::value_type()
				}))};

		static constexpr xablau::geometry::line < typename Types::value_type, xablau::geometry::spatial_dimension < 0 > > lineDegenerateZero;

		static constexpr xablau::geometry::line < typename Types::value_type, xablau::geometry::spatial_dimension < 1 > > lineDegeneratePositive{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename Types::value_type(1)
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename Types::value_type()
				}))};

		static constexpr xablau::geometry::line < typename Types::value_type, xablau::geometry::spatial_dimension < 1 > > lineDirectX{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename Types::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename Types::value_type(1)
				}))};

		static constexpr xablau::geometry::line < typename Types::value_type, xablau::geometry::spatial_dimension < 1 > > lineReverseX{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename Types::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename Types::value_type(-1)
				}))};

		static constexpr xablau::geometry::line < typename Types::value_type, xablau::geometry::spatial_dimension < 2 > > lineDirectY{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename Types::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename Types::value_type(),
					typename Types::value_type(1)
				}))};

		static constexpr xablau::geometry::line < typename Types::value_type, xablau::geometry::spatial_dimension < 2 > > lineReverseY{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename Types::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename Types::value_type(),
					typename Types::value_type(-1)
				}))};

		static constexpr xablau::geometry::line < typename Types::value_type, xablau::geometry::spatial_dimension < 3 > > lineDirectZ{
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

		static constexpr xablau::geometry::line < typename Types::value_type, xablau::geometry::spatial_dimension < 3 > > lineReverseZ{
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

	TYPED_TEST_SUITE_P(GeometryLineTest);

	TYPED_TEST_P(GeometryLineTest, ComparisonOperatorLesser)
	{
		EXPECT_FALSE(this->lineDegenerateNegative < this->lineDegenerateNegative);
		EXPECT_TRUE(this->lineDegenerateNegative < this->lineDegenerateZero);
		EXPECT_TRUE(this->lineDegenerateNegative < this->lineDegeneratePositive);
		EXPECT_TRUE(this->lineDegenerateNegative < this->lineDirectX);
		EXPECT_TRUE(this->lineDegenerateNegative < this->lineReverseX);
		EXPECT_TRUE(this->lineDegenerateNegative < this->lineDirectY);
		EXPECT_TRUE(this->lineDegenerateNegative < this->lineReverseY);
		EXPECT_TRUE(this->lineDegenerateNegative < this->lineDirectZ);
		EXPECT_TRUE(this->lineDegenerateNegative < this->lineReverseZ);

		EXPECT_FALSE(this->lineDegenerateZero < this->lineDegenerateNegative);
		EXPECT_FALSE(this->lineDegenerateZero < this->lineDegenerateZero);
		EXPECT_TRUE(this->lineDegenerateZero < this->lineDegeneratePositive);
		EXPECT_TRUE(this->lineDegenerateZero < this->lineDirectX);
		EXPECT_TRUE(this->lineDegenerateZero < this->lineReverseX);
		EXPECT_TRUE(this->lineDegenerateZero < this->lineDirectY);
		EXPECT_TRUE(this->lineDegenerateZero < this->lineReverseY);
		EXPECT_TRUE(this->lineDegenerateZero < this->lineDirectZ);
		EXPECT_TRUE(this->lineDegenerateZero < this->lineReverseZ);

		EXPECT_FALSE(this->lineDegeneratePositive < this->lineDegenerateNegative);
		EXPECT_FALSE(this->lineDegeneratePositive < this->lineDegenerateZero);
		EXPECT_FALSE(this->lineDegeneratePositive < this->lineDegeneratePositive);
		EXPECT_TRUE(this->lineDegeneratePositive < this->lineDirectX);
		EXPECT_TRUE(this->lineDegeneratePositive < this->lineReverseX);
		EXPECT_TRUE(this->lineDegeneratePositive < this->lineDirectY);
		EXPECT_TRUE(this->lineDegeneratePositive < this->lineReverseY);
		EXPECT_TRUE(this->lineDegeneratePositive < this->lineDirectZ);
		EXPECT_TRUE(this->lineDegeneratePositive < this->lineReverseZ);

		EXPECT_FALSE(this->lineDirectX < this->lineDegenerateNegative);
		EXPECT_FALSE(this->lineDirectX < this->lineDegenerateZero);
		EXPECT_FALSE(this->lineDirectX < this->lineDegeneratePositive);
		EXPECT_FALSE(this->lineDirectX < this->lineDirectX);
		EXPECT_FALSE(this->lineDirectX < this->lineReverseX);
		EXPECT_FALSE(this->lineDirectX < this->lineDirectY);
		EXPECT_FALSE(this->lineDirectX < this->lineReverseY);
		EXPECT_FALSE(this->lineDirectX < this->lineDirectZ);
		EXPECT_FALSE(this->lineDirectX < this->lineReverseZ);

		EXPECT_FALSE(this->lineReverseX < this->lineDegenerateNegative);
		EXPECT_FALSE(this->lineReverseX < this->lineDegenerateZero);
		EXPECT_FALSE(this->lineReverseX < this->lineDegeneratePositive);
		EXPECT_FALSE(this->lineReverseX < this->lineDirectX);
		EXPECT_FALSE(this->lineReverseX < this->lineReverseX);
		EXPECT_FALSE(this->lineReverseX < this->lineDirectY);
		EXPECT_FALSE(this->lineReverseX < this->lineReverseY);
		EXPECT_FALSE(this->lineReverseX < this->lineDirectZ);
		EXPECT_FALSE(this->lineReverseX < this->lineReverseZ);

		EXPECT_FALSE(this->lineDirectY < this->lineDegenerateNegative);
		EXPECT_FALSE(this->lineDirectY < this->lineDegenerateZero);
		EXPECT_FALSE(this->lineDirectY < this->lineDegeneratePositive);
		EXPECT_TRUE(this->lineDirectY < this->lineDirectX);
		EXPECT_TRUE(this->lineDirectY < this->lineReverseX);
		EXPECT_FALSE(this->lineDirectY < this->lineDirectY);
		EXPECT_FALSE(this->lineDirectY < this->lineReverseY);
		EXPECT_FALSE(this->lineDirectY < this->lineDirectZ);
		EXPECT_FALSE(this->lineDirectY < this->lineReverseZ);

		EXPECT_FALSE(this->lineReverseY < this->lineDegenerateNegative);
		EXPECT_FALSE(this->lineReverseY < this->lineDegenerateZero);
		EXPECT_FALSE(this->lineReverseY < this->lineDegeneratePositive);
		EXPECT_TRUE(this->lineReverseY < this->lineDirectX);
		EXPECT_TRUE(this->lineReverseY < this->lineReverseX);
		EXPECT_FALSE(this->lineReverseY < this->lineDirectY);
		EXPECT_FALSE(this->lineReverseY < this->lineReverseY);
		EXPECT_FALSE(this->lineReverseY < this->lineDirectZ);
		EXPECT_FALSE(this->lineReverseY < this->lineReverseZ);

		EXPECT_FALSE(this->lineDirectZ < this->lineDegenerateNegative);
		EXPECT_FALSE(this->lineDirectZ < this->lineDegenerateZero);
		EXPECT_FALSE(this->lineDirectZ < this->lineDegeneratePositive);
		EXPECT_TRUE(this->lineDirectZ < this->lineDirectX);
		EXPECT_TRUE(this->lineDirectZ < this->lineReverseX);
		EXPECT_TRUE(this->lineDirectZ < this->lineDirectY);
		EXPECT_TRUE(this->lineDirectZ < this->lineReverseY);
		EXPECT_FALSE(this->lineDirectZ < this->lineDirectZ);
		EXPECT_FALSE(this->lineDirectZ < this->lineReverseZ);

		EXPECT_FALSE(this->lineReverseZ < this->lineDegenerateNegative);
		EXPECT_FALSE(this->lineReverseZ < this->lineDegenerateZero);
		EXPECT_FALSE(this->lineReverseZ < this->lineDegeneratePositive);
		EXPECT_TRUE(this->lineReverseZ < this->lineDirectX);
		EXPECT_TRUE(this->lineReverseZ < this->lineReverseX);
		EXPECT_TRUE(this->lineReverseZ < this->lineDirectY);
		EXPECT_TRUE(this->lineReverseZ < this->lineReverseY);
		EXPECT_FALSE(this->lineReverseZ < this->lineDirectZ);
		EXPECT_FALSE(this->lineReverseZ < this->lineReverseZ);
	}

	TYPED_TEST_P(GeometryLineTest, ComparisonOperatorLesserEqualTo)
	{
		EXPECT_TRUE(this->lineDegenerateNegative <= this->lineDegenerateNegative);
		EXPECT_TRUE(this->lineDegenerateNegative <= this->lineDegenerateZero);
		EXPECT_TRUE(this->lineDegenerateNegative <= this->lineDegeneratePositive);
		EXPECT_TRUE(this->lineDegenerateNegative <= this->lineDirectX);
		EXPECT_TRUE(this->lineDegenerateNegative <= this->lineReverseX);
		EXPECT_TRUE(this->lineDegenerateNegative <= this->lineDirectY);
		EXPECT_TRUE(this->lineDegenerateNegative <= this->lineReverseY);
		EXPECT_TRUE(this->lineDegenerateNegative <= this->lineDirectZ);
		EXPECT_TRUE(this->lineDegenerateNegative <= this->lineReverseZ);

		EXPECT_FALSE(this->lineDegenerateZero <= this->lineDegenerateNegative);
		EXPECT_TRUE(this->lineDegenerateZero <= this->lineDegenerateZero);
		EXPECT_TRUE(this->lineDegenerateZero <= this->lineDegeneratePositive);
		EXPECT_TRUE(this->lineDegenerateZero <= this->lineDirectX);
		EXPECT_TRUE(this->lineDegenerateZero <= this->lineReverseX);
		EXPECT_TRUE(this->lineDegenerateZero <= this->lineDirectY);
		EXPECT_TRUE(this->lineDegenerateZero <= this->lineReverseY);
		EXPECT_TRUE(this->lineDegenerateZero <= this->lineDirectZ);
		EXPECT_TRUE(this->lineDegenerateZero <= this->lineReverseZ);

		EXPECT_FALSE(this->lineDegeneratePositive <= this->lineDegenerateNegative);
		EXPECT_FALSE(this->lineDegeneratePositive <= this->lineDegenerateZero);
		EXPECT_TRUE(this->lineDegeneratePositive <= this->lineDegeneratePositive);
		EXPECT_TRUE(this->lineDegeneratePositive <= this->lineDirectX);
		EXPECT_TRUE(this->lineDegeneratePositive <= this->lineReverseX);
		EXPECT_TRUE(this->lineDegeneratePositive <= this->lineDirectY);
		EXPECT_TRUE(this->lineDegeneratePositive <= this->lineReverseY);
		EXPECT_TRUE(this->lineDegeneratePositive <= this->lineDirectZ);
		EXPECT_TRUE(this->lineDegeneratePositive <= this->lineReverseZ);

		EXPECT_FALSE(this->lineDirectX <= this->lineDegenerateNegative);
		EXPECT_FALSE(this->lineDirectX <= this->lineDegenerateZero);
		EXPECT_FALSE(this->lineDirectX <= this->lineDegeneratePositive);
		EXPECT_TRUE(this->lineDirectX <= this->lineDirectX);
		EXPECT_TRUE(this->lineDirectX <= this->lineReverseX);
		EXPECT_FALSE(this->lineDirectX <= this->lineDirectY);
		EXPECT_FALSE(this->lineDirectX <= this->lineReverseY);
		EXPECT_FALSE(this->lineDirectX <= this->lineDirectZ);
		EXPECT_FALSE(this->lineDirectX <= this->lineReverseZ);

		EXPECT_FALSE(this->lineReverseX <= this->lineDegenerateNegative);
		EXPECT_FALSE(this->lineReverseX <= this->lineDegenerateZero);
		EXPECT_FALSE(this->lineReverseX <= this->lineDegeneratePositive);
		EXPECT_TRUE(this->lineReverseX <= this->lineDirectX);
		EXPECT_TRUE(this->lineReverseX <= this->lineReverseX);
		EXPECT_FALSE(this->lineReverseX <= this->lineDirectY);
		EXPECT_FALSE(this->lineReverseX <= this->lineReverseY);
		EXPECT_FALSE(this->lineReverseX <= this->lineDirectZ);
		EXPECT_FALSE(this->lineReverseX <= this->lineReverseZ);

		EXPECT_FALSE(this->lineDirectY <= this->lineDegenerateNegative);
		EXPECT_FALSE(this->lineDirectY <= this->lineDegenerateZero);
		EXPECT_FALSE(this->lineDirectY <= this->lineDegeneratePositive);
		EXPECT_TRUE(this->lineDirectY <= this->lineDirectX);
		EXPECT_TRUE(this->lineDirectY <= this->lineReverseX);
		EXPECT_TRUE(this->lineDirectY <= this->lineDirectY);
		EXPECT_TRUE(this->lineDirectY <= this->lineReverseY);
		EXPECT_FALSE(this->lineDirectY <= this->lineDirectZ);
		EXPECT_FALSE(this->lineDirectY <= this->lineReverseZ);

		EXPECT_FALSE(this->lineReverseY <= this->lineDegenerateNegative);
		EXPECT_FALSE(this->lineReverseY <= this->lineDegenerateZero);
		EXPECT_FALSE(this->lineReverseY <= this->lineDegeneratePositive);
		EXPECT_TRUE(this->lineReverseY <= this->lineDirectX);
		EXPECT_TRUE(this->lineReverseY <= this->lineReverseX);
		EXPECT_TRUE(this->lineReverseY <= this->lineDirectY);
		EXPECT_TRUE(this->lineReverseY <= this->lineReverseY);
		EXPECT_FALSE(this->lineReverseY <= this->lineDirectZ);
		EXPECT_FALSE(this->lineReverseY <= this->lineReverseZ);

		EXPECT_FALSE(this->lineDirectZ <= this->lineDegenerateNegative);
		EXPECT_FALSE(this->lineDirectZ <= this->lineDegenerateZero);
		EXPECT_FALSE(this->lineDirectZ <= this->lineDegeneratePositive);
		EXPECT_TRUE(this->lineDirectZ <= this->lineDirectX);
		EXPECT_TRUE(this->lineDirectZ <= this->lineReverseX);
		EXPECT_TRUE(this->lineDirectZ <= this->lineDirectY);
		EXPECT_TRUE(this->lineDirectZ <= this->lineReverseY);
		EXPECT_TRUE(this->lineDirectZ <= this->lineDirectZ);
		EXPECT_TRUE(this->lineDirectZ <= this->lineReverseZ);

		EXPECT_FALSE(this->lineReverseZ <= this->lineDegenerateNegative);
		EXPECT_FALSE(this->lineReverseZ <= this->lineDegenerateZero);
		EXPECT_FALSE(this->lineReverseZ <= this->lineDegeneratePositive);
		EXPECT_TRUE(this->lineReverseZ <= this->lineDirectX);
		EXPECT_TRUE(this->lineReverseZ <= this->lineReverseX);
		EXPECT_TRUE(this->lineReverseZ <= this->lineDirectY);
		EXPECT_TRUE(this->lineReverseZ <= this->lineReverseY);
		EXPECT_TRUE(this->lineReverseZ <= this->lineDirectZ);
		EXPECT_TRUE(this->lineReverseZ <= this->lineReverseZ);
	}

	TYPED_TEST_P(GeometryLineTest, ComparisonOperatorEqualTo)
	{
		EXPECT_TRUE(this->lineDegenerateNegative == this->lineDegenerateNegative);
		EXPECT_FALSE(this->lineDegenerateNegative == this->lineDegenerateZero);
		EXPECT_FALSE(this->lineDegenerateNegative == this->lineDegeneratePositive);
		EXPECT_FALSE(this->lineDegenerateNegative == this->lineDirectX);
		EXPECT_FALSE(this->lineDegenerateNegative == this->lineReverseX);
		EXPECT_FALSE(this->lineDegenerateNegative == this->lineDirectY);
		EXPECT_FALSE(this->lineDegenerateNegative == this->lineReverseY);
		EXPECT_FALSE(this->lineDegenerateNegative == this->lineDirectZ);
		EXPECT_FALSE(this->lineDegenerateNegative == this->lineReverseZ);

		EXPECT_FALSE(this->lineDegenerateZero == this->lineDegenerateNegative);
		EXPECT_TRUE(this->lineDegenerateZero == this->lineDegenerateZero);
		EXPECT_FALSE(this->lineDegenerateZero == this->lineDegeneratePositive);
		EXPECT_FALSE(this->lineDegenerateZero == this->lineDirectX);
		EXPECT_FALSE(this->lineDegenerateZero == this->lineReverseX);
		EXPECT_FALSE(this->lineDegenerateZero == this->lineDirectY);
		EXPECT_FALSE(this->lineDegenerateZero == this->lineReverseY);
		EXPECT_FALSE(this->lineDegenerateZero == this->lineDirectZ);
		EXPECT_FALSE(this->lineDegenerateZero == this->lineReverseZ);

		EXPECT_FALSE(this->lineDegeneratePositive == this->lineDegenerateNegative);
		EXPECT_FALSE(this->lineDegeneratePositive == this->lineDegenerateZero);
		EXPECT_TRUE(this->lineDegeneratePositive == this->lineDegeneratePositive);
		EXPECT_FALSE(this->lineDegeneratePositive == this->lineDirectX);
		EXPECT_FALSE(this->lineDegeneratePositive == this->lineReverseX);
		EXPECT_FALSE(this->lineDegeneratePositive == this->lineDirectY);
		EXPECT_FALSE(this->lineDegeneratePositive == this->lineReverseY);
		EXPECT_FALSE(this->lineDegeneratePositive == this->lineDirectZ);
		EXPECT_FALSE(this->lineDegeneratePositive == this->lineReverseZ);

		EXPECT_FALSE(this->lineDirectX == this->lineDegenerateNegative);
		EXPECT_FALSE(this->lineDirectX == this->lineDegenerateZero);
		EXPECT_FALSE(this->lineDirectX == this->lineDegeneratePositive);
		EXPECT_TRUE(this->lineDirectX == this->lineDirectX);
		EXPECT_TRUE(this->lineDirectX == this->lineReverseX);
		EXPECT_FALSE(this->lineDirectX == this->lineDirectY);
		EXPECT_FALSE(this->lineDirectX == this->lineReverseY);
		EXPECT_FALSE(this->lineDirectX == this->lineDirectZ);
		EXPECT_FALSE(this->lineDirectX == this->lineReverseZ);

		EXPECT_FALSE(this->lineReverseX == this->lineDegenerateNegative);
		EXPECT_FALSE(this->lineReverseX == this->lineDegenerateZero);
		EXPECT_FALSE(this->lineReverseX == this->lineDegeneratePositive);
		EXPECT_TRUE(this->lineReverseX == this->lineDirectX);
		EXPECT_TRUE(this->lineReverseX == this->lineReverseX);
		EXPECT_FALSE(this->lineReverseX == this->lineDirectY);
		EXPECT_FALSE(this->lineReverseX == this->lineReverseY);
		EXPECT_FALSE(this->lineReverseX == this->lineDirectZ);
		EXPECT_FALSE(this->lineReverseX == this->lineReverseZ);

		EXPECT_FALSE(this->lineDirectY == this->lineDegenerateNegative);
		EXPECT_FALSE(this->lineDirectY == this->lineDegenerateZero);
		EXPECT_FALSE(this->lineDirectY == this->lineDegeneratePositive);
		EXPECT_FALSE(this->lineDirectY == this->lineDirectX);
		EXPECT_FALSE(this->lineDirectY == this->lineReverseX);
		EXPECT_TRUE(this->lineDirectY == this->lineDirectY);
		EXPECT_TRUE(this->lineDirectY == this->lineReverseY);
		EXPECT_FALSE(this->lineDirectY == this->lineDirectZ);
		EXPECT_FALSE(this->lineDirectY == this->lineReverseZ);

		EXPECT_FALSE(this->lineReverseY == this->lineDegenerateNegative);
		EXPECT_FALSE(this->lineReverseY == this->lineDegenerateZero);
		EXPECT_FALSE(this->lineReverseY == this->lineDegeneratePositive);
		EXPECT_FALSE(this->lineReverseY == this->lineDirectX);
		EXPECT_FALSE(this->lineReverseY == this->lineReverseX);
		EXPECT_TRUE(this->lineReverseY == this->lineDirectY);
		EXPECT_TRUE(this->lineReverseY == this->lineReverseY);
		EXPECT_FALSE(this->lineReverseY == this->lineDirectZ);
		EXPECT_FALSE(this->lineReverseY == this->lineReverseZ);

		EXPECT_FALSE(this->lineDirectZ == this->lineDegenerateNegative);
		EXPECT_FALSE(this->lineDirectZ == this->lineDegenerateZero);
		EXPECT_FALSE(this->lineDirectZ == this->lineDegeneratePositive);
		EXPECT_FALSE(this->lineDirectZ == this->lineDirectX);
		EXPECT_FALSE(this->lineDirectZ == this->lineReverseX);
		EXPECT_FALSE(this->lineDirectZ == this->lineDirectY);
		EXPECT_FALSE(this->lineDirectZ == this->lineReverseY);
		EXPECT_TRUE(this->lineDirectZ == this->lineDirectZ);
		EXPECT_TRUE(this->lineDirectZ == this->lineReverseZ);

		EXPECT_FALSE(this->lineReverseZ == this->lineDegenerateNegative);
		EXPECT_FALSE(this->lineReverseZ == this->lineDegenerateZero);
		EXPECT_FALSE(this->lineReverseZ == this->lineDegeneratePositive);
		EXPECT_FALSE(this->lineReverseZ == this->lineDirectX);
		EXPECT_FALSE(this->lineReverseZ == this->lineReverseX);
		EXPECT_FALSE(this->lineReverseZ == this->lineDirectY);
		EXPECT_FALSE(this->lineReverseZ == this->lineReverseY);
		EXPECT_TRUE(this->lineReverseZ == this->lineDirectZ);
		EXPECT_TRUE(this->lineReverseZ == this->lineReverseZ);
	}

	TYPED_TEST_P(GeometryLineTest, ComparisonOperatorGreaterEqualTo)
	{
		EXPECT_TRUE(this->lineDegenerateNegative >= this->lineDegenerateNegative);
		EXPECT_FALSE(this->lineDegenerateNegative >= this->lineDegenerateZero);
		EXPECT_FALSE(this->lineDegenerateNegative >= this->lineDegeneratePositive);
		EXPECT_FALSE(this->lineDegenerateNegative >= this->lineDirectX);
		EXPECT_FALSE(this->lineDegenerateNegative >= this->lineReverseX);
		EXPECT_FALSE(this->lineDegenerateNegative >= this->lineDirectY);
		EXPECT_FALSE(this->lineDegenerateNegative >= this->lineReverseY);
		EXPECT_FALSE(this->lineDegenerateNegative >= this->lineDirectZ);
		EXPECT_FALSE(this->lineDegenerateNegative >= this->lineReverseZ);

		EXPECT_TRUE(this->lineDegenerateZero >= this->lineDegenerateNegative);
		EXPECT_TRUE(this->lineDegenerateZero >= this->lineDegenerateZero);
		EXPECT_FALSE(this->lineDegenerateZero >= this->lineDegeneratePositive);
		EXPECT_FALSE(this->lineDegenerateZero >= this->lineDirectX);
		EXPECT_FALSE(this->lineDegenerateZero >= this->lineReverseX);
		EXPECT_FALSE(this->lineDegenerateZero >= this->lineDirectY);
		EXPECT_FALSE(this->lineDegenerateZero >= this->lineReverseY);
		EXPECT_FALSE(this->lineDegenerateZero >= this->lineDirectZ);
		EXPECT_FALSE(this->lineDegenerateZero >= this->lineReverseZ);

		EXPECT_TRUE(this->lineDegeneratePositive >= this->lineDegenerateNegative);
		EXPECT_TRUE(this->lineDegeneratePositive >= this->lineDegenerateZero);
		EXPECT_TRUE(this->lineDegeneratePositive >= this->lineDegeneratePositive);
		EXPECT_FALSE(this->lineDegeneratePositive >= this->lineDirectX);
		EXPECT_FALSE(this->lineDegeneratePositive >= this->lineReverseX);
		EXPECT_FALSE(this->lineDegeneratePositive >= this->lineDirectY);
		EXPECT_FALSE(this->lineDegeneratePositive >= this->lineReverseY);
		EXPECT_FALSE(this->lineDegeneratePositive >= this->lineDirectZ);
		EXPECT_FALSE(this->lineDegeneratePositive >= this->lineReverseZ);

		EXPECT_TRUE(this->lineDirectX >= this->lineDegenerateNegative);
		EXPECT_TRUE(this->lineDirectX >= this->lineDegenerateZero);
		EXPECT_TRUE(this->lineDirectX >= this->lineDegeneratePositive);
		EXPECT_TRUE(this->lineDirectX >= this->lineDirectX);
		EXPECT_TRUE(this->lineDirectX >= this->lineReverseX);
		EXPECT_TRUE(this->lineDirectX >= this->lineDirectY);
		EXPECT_TRUE(this->lineDirectX >= this->lineReverseY);
		EXPECT_TRUE(this->lineDirectX >= this->lineDirectZ);
		EXPECT_TRUE(this->lineDirectX >= this->lineReverseZ);

		EXPECT_TRUE(this->lineReverseX >= this->lineDegenerateNegative);
		EXPECT_TRUE(this->lineReverseX >= this->lineDegenerateZero);
		EXPECT_TRUE(this->lineReverseX >= this->lineDegeneratePositive);
		EXPECT_TRUE(this->lineReverseX >= this->lineDirectX);
		EXPECT_TRUE(this->lineReverseX >= this->lineReverseX);
		EXPECT_TRUE(this->lineReverseX >= this->lineDirectY);
		EXPECT_TRUE(this->lineReverseX >= this->lineReverseY);
		EXPECT_TRUE(this->lineReverseX >= this->lineDirectZ);
		EXPECT_TRUE(this->lineReverseX >= this->lineReverseZ);

		EXPECT_TRUE(this->lineDirectY >= this->lineDegenerateNegative);
		EXPECT_TRUE(this->lineDirectY >= this->lineDegenerateZero);
		EXPECT_TRUE(this->lineDirectY >= this->lineDegeneratePositive);
		EXPECT_FALSE(this->lineDirectY >= this->lineDirectX);
		EXPECT_FALSE(this->lineDirectY >= this->lineReverseX);
		EXPECT_TRUE(this->lineDirectY >= this->lineDirectY);
		EXPECT_TRUE(this->lineDirectY >= this->lineReverseY);
		EXPECT_TRUE(this->lineDirectY >= this->lineDirectZ);
		EXPECT_TRUE(this->lineDirectY >= this->lineReverseZ);

		EXPECT_TRUE(this->lineReverseY >= this->lineDegenerateNegative);
		EXPECT_TRUE(this->lineReverseY >= this->lineDegenerateZero);
		EXPECT_TRUE(this->lineReverseY >= this->lineDegeneratePositive);
		EXPECT_FALSE(this->lineReverseY >= this->lineDirectX);
		EXPECT_FALSE(this->lineReverseY >= this->lineReverseX);
		EXPECT_TRUE(this->lineReverseY >= this->lineDirectY);
		EXPECT_TRUE(this->lineReverseY >= this->lineReverseY);
		EXPECT_TRUE(this->lineReverseY >= this->lineDirectZ);
		EXPECT_TRUE(this->lineReverseY >= this->lineReverseZ);

		EXPECT_TRUE(this->lineDirectZ >= this->lineDegenerateNegative);
		EXPECT_TRUE(this->lineDirectZ >= this->lineDegenerateZero);
		EXPECT_TRUE(this->lineDirectZ >= this->lineDegeneratePositive);
		EXPECT_FALSE(this->lineDirectZ >= this->lineDirectX);
		EXPECT_FALSE(this->lineDirectZ >= this->lineReverseX);
		EXPECT_FALSE(this->lineDirectZ >= this->lineDirectY);
		EXPECT_FALSE(this->lineDirectZ >= this->lineReverseY);
		EXPECT_TRUE(this->lineDirectZ >= this->lineDirectZ);
		EXPECT_TRUE(this->lineDirectZ >= this->lineReverseZ);

		EXPECT_TRUE(this->lineReverseZ >= this->lineDegenerateNegative);
		EXPECT_TRUE(this->lineReverseZ >= this->lineDegenerateZero);
		EXPECT_TRUE(this->lineReverseZ >= this->lineDegeneratePositive);
		EXPECT_FALSE(this->lineReverseZ >= this->lineDirectX);
		EXPECT_FALSE(this->lineReverseZ >= this->lineReverseX);
		EXPECT_FALSE(this->lineReverseZ >= this->lineDirectY);
		EXPECT_FALSE(this->lineReverseZ >= this->lineReverseY);
		EXPECT_TRUE(this->lineReverseZ >= this->lineDirectZ);
		EXPECT_TRUE(this->lineReverseZ >= this->lineReverseZ);
	}

	TYPED_TEST_P(GeometryLineTest, ComparisonOperatorGreater)
	{
		EXPECT_FALSE(this->lineDegenerateNegative > this->lineDegenerateNegative);
		EXPECT_FALSE(this->lineDegenerateNegative > this->lineDegenerateZero);
		EXPECT_FALSE(this->lineDegenerateNegative > this->lineDegeneratePositive);
		EXPECT_FALSE(this->lineDegenerateNegative > this->lineDirectX);
		EXPECT_FALSE(this->lineDegenerateNegative > this->lineReverseX);
		EXPECT_FALSE(this->lineDegenerateNegative > this->lineDirectY);
		EXPECT_FALSE(this->lineDegenerateNegative > this->lineReverseY);
		EXPECT_FALSE(this->lineDegenerateNegative > this->lineDirectZ);
		EXPECT_FALSE(this->lineDegenerateNegative > this->lineReverseZ);

		EXPECT_TRUE(this->lineDegenerateZero > this->lineDegenerateNegative);
		EXPECT_FALSE(this->lineDegenerateZero > this->lineDegenerateZero);
		EXPECT_FALSE(this->lineDegenerateZero > this->lineDegeneratePositive);
		EXPECT_FALSE(this->lineDegenerateZero > this->lineDirectX);
		EXPECT_FALSE(this->lineDegenerateZero > this->lineReverseX);
		EXPECT_FALSE(this->lineDegenerateZero > this->lineDirectY);
		EXPECT_FALSE(this->lineDegenerateZero > this->lineReverseY);
		EXPECT_FALSE(this->lineDegenerateZero > this->lineDirectZ);
		EXPECT_FALSE(this->lineDegenerateZero > this->lineReverseZ);

		EXPECT_TRUE(this->lineDegeneratePositive > this->lineDegenerateNegative);
		EXPECT_TRUE(this->lineDegeneratePositive > this->lineDegenerateZero);
		EXPECT_FALSE(this->lineDegeneratePositive > this->lineDegeneratePositive);
		EXPECT_FALSE(this->lineDegeneratePositive > this->lineDirectX);
		EXPECT_FALSE(this->lineDegeneratePositive > this->lineReverseX);
		EXPECT_FALSE(this->lineDegeneratePositive > this->lineDirectY);
		EXPECT_FALSE(this->lineDegeneratePositive > this->lineReverseY);
		EXPECT_FALSE(this->lineDegeneratePositive > this->lineDirectZ);
		EXPECT_FALSE(this->lineDegeneratePositive > this->lineReverseZ);

		EXPECT_TRUE(this->lineDirectX > this->lineDegenerateNegative);
		EXPECT_TRUE(this->lineDirectX > this->lineDegenerateZero);
		EXPECT_TRUE(this->lineDirectX > this->lineDegeneratePositive);
		EXPECT_FALSE(this->lineDirectX > this->lineDirectX);
		EXPECT_FALSE(this->lineDirectX > this->lineReverseX);
		EXPECT_TRUE(this->lineDirectX > this->lineDirectY);
		EXPECT_TRUE(this->lineDirectX > this->lineReverseY);
		EXPECT_TRUE(this->lineDirectX > this->lineDirectZ);
		EXPECT_TRUE(this->lineDirectX > this->lineReverseZ);

		EXPECT_TRUE(this->lineReverseX > this->lineDegenerateNegative);
		EXPECT_TRUE(this->lineReverseX > this->lineDegenerateZero);
		EXPECT_TRUE(this->lineReverseX > this->lineDegeneratePositive);
		EXPECT_FALSE(this->lineReverseX > this->lineDirectX);
		EXPECT_FALSE(this->lineReverseX > this->lineReverseX);
		EXPECT_TRUE(this->lineReverseX > this->lineDirectY);
		EXPECT_TRUE(this->lineReverseX > this->lineReverseY);
		EXPECT_TRUE(this->lineReverseX > this->lineDirectZ);
		EXPECT_TRUE(this->lineReverseX > this->lineReverseZ);

		EXPECT_TRUE(this->lineDirectY > this->lineDegenerateNegative);
		EXPECT_TRUE(this->lineDirectY > this->lineDegenerateZero);
		EXPECT_TRUE(this->lineDirectY > this->lineDegeneratePositive);
		EXPECT_FALSE(this->lineDirectY > this->lineDirectX);
		EXPECT_FALSE(this->lineDirectY > this->lineReverseX);
		EXPECT_FALSE(this->lineDirectY > this->lineDirectY);
		EXPECT_FALSE(this->lineDirectY > this->lineReverseY);
		EXPECT_TRUE(this->lineDirectY > this->lineDirectZ);
		EXPECT_TRUE(this->lineDirectY > this->lineReverseZ);

		EXPECT_TRUE(this->lineReverseY > this->lineDegenerateNegative);
		EXPECT_TRUE(this->lineReverseY > this->lineDegenerateZero);
		EXPECT_TRUE(this->lineReverseY > this->lineDegeneratePositive);
		EXPECT_FALSE(this->lineReverseY > this->lineDirectX);
		EXPECT_FALSE(this->lineReverseY > this->lineReverseX);
		EXPECT_FALSE(this->lineReverseY > this->lineDirectY);
		EXPECT_FALSE(this->lineReverseY > this->lineReverseY);
		EXPECT_TRUE(this->lineReverseY > this->lineDirectZ);
		EXPECT_TRUE(this->lineReverseY > this->lineReverseZ);

		EXPECT_TRUE(this->lineDirectZ > this->lineDegenerateNegative);
		EXPECT_TRUE(this->lineDirectZ > this->lineDegenerateZero);
		EXPECT_TRUE(this->lineDirectZ > this->lineDegeneratePositive);
		EXPECT_FALSE(this->lineDirectZ > this->lineDirectX);
		EXPECT_FALSE(this->lineDirectZ > this->lineReverseX);
		EXPECT_FALSE(this->lineDirectZ > this->lineDirectY);
		EXPECT_FALSE(this->lineDirectZ > this->lineReverseY);
		EXPECT_FALSE(this->lineDirectZ > this->lineDirectZ);
		EXPECT_FALSE(this->lineDirectZ > this->lineReverseZ);

		EXPECT_TRUE(this->lineReverseZ > this->lineDegenerateNegative);
		EXPECT_TRUE(this->lineReverseZ > this->lineDegenerateZero);
		EXPECT_TRUE(this->lineReverseZ > this->lineDegeneratePositive);
		EXPECT_FALSE(this->lineReverseZ > this->lineDirectX);
		EXPECT_FALSE(this->lineReverseZ > this->lineReverseX);
		EXPECT_FALSE(this->lineReverseZ > this->lineDirectY);
		EXPECT_FALSE(this->lineReverseZ > this->lineReverseY);
		EXPECT_FALSE(this->lineReverseZ > this->lineDirectZ);
		EXPECT_FALSE(this->lineReverseZ > this->lineReverseZ);
	}

	TYPED_TEST_P(GeometryLineTest, ComparisonOperatorNotEqualTo)
	{
		EXPECT_FALSE(this->lineDegenerateNegative != this->lineDegenerateNegative);
		EXPECT_TRUE(this->lineDegenerateNegative != this->lineDegenerateZero);
		EXPECT_TRUE(this->lineDegenerateNegative != this->lineDegeneratePositive);
		EXPECT_TRUE(this->lineDegenerateNegative != this->lineDirectX);
		EXPECT_TRUE(this->lineDegenerateNegative != this->lineReverseX);
		EXPECT_TRUE(this->lineDegenerateNegative != this->lineDirectY);
		EXPECT_TRUE(this->lineDegenerateNegative != this->lineReverseY);
		EXPECT_TRUE(this->lineDegenerateNegative != this->lineDirectZ);
		EXPECT_TRUE(this->lineDegenerateNegative != this->lineReverseZ);

		EXPECT_TRUE(this->lineDegenerateZero != this->lineDegenerateNegative);
		EXPECT_FALSE(this->lineDegenerateZero != this->lineDegenerateZero);
		EXPECT_TRUE(this->lineDegenerateZero != this->lineDegeneratePositive);
		EXPECT_TRUE(this->lineDegenerateZero != this->lineDirectX);
		EXPECT_TRUE(this->lineDegenerateZero != this->lineReverseX);
		EXPECT_TRUE(this->lineDegenerateZero != this->lineDirectY);
		EXPECT_TRUE(this->lineDegenerateZero != this->lineReverseY);
		EXPECT_TRUE(this->lineDegenerateZero != this->lineDirectZ);
		EXPECT_TRUE(this->lineDegenerateZero != this->lineReverseZ);

		EXPECT_TRUE(this->lineDegeneratePositive != this->lineDegenerateNegative);
		EXPECT_TRUE(this->lineDegeneratePositive != this->lineDegenerateZero);
		EXPECT_FALSE(this->lineDegeneratePositive != this->lineDegeneratePositive);
		EXPECT_TRUE(this->lineDegeneratePositive != this->lineDirectX);
		EXPECT_TRUE(this->lineDegeneratePositive != this->lineReverseX);
		EXPECT_TRUE(this->lineDegeneratePositive != this->lineDirectY);
		EXPECT_TRUE(this->lineDegeneratePositive != this->lineReverseY);
		EXPECT_TRUE(this->lineDegeneratePositive != this->lineDirectZ);
		EXPECT_TRUE(this->lineDegeneratePositive != this->lineReverseZ);

		EXPECT_TRUE(this->lineDirectX != this->lineDegenerateNegative);
		EXPECT_TRUE(this->lineDirectX != this->lineDegenerateZero);
		EXPECT_TRUE(this->lineDirectX != this->lineDegeneratePositive);
		EXPECT_FALSE(this->lineDirectX != this->lineDirectX);
		EXPECT_FALSE(this->lineDirectX != this->lineReverseX);
		EXPECT_TRUE(this->lineDirectX != this->lineDirectY);
		EXPECT_TRUE(this->lineDirectX != this->lineReverseY);
		EXPECT_TRUE(this->lineDirectX != this->lineDirectZ);
		EXPECT_TRUE(this->lineDirectX != this->lineReverseZ);

		EXPECT_TRUE(this->lineReverseX != this->lineDegenerateNegative);
		EXPECT_TRUE(this->lineReverseX != this->lineDegenerateZero);
		EXPECT_TRUE(this->lineReverseX != this->lineDegeneratePositive);
		EXPECT_FALSE(this->lineReverseX != this->lineDirectX);
		EXPECT_FALSE(this->lineReverseX != this->lineReverseX);
		EXPECT_TRUE(this->lineReverseX != this->lineDirectY);
		EXPECT_TRUE(this->lineReverseX != this->lineReverseY);
		EXPECT_TRUE(this->lineReverseX != this->lineDirectZ);
		EXPECT_TRUE(this->lineReverseX != this->lineReverseZ);

		EXPECT_TRUE(this->lineDirectY != this->lineDegenerateNegative);
		EXPECT_TRUE(this->lineDirectY != this->lineDegenerateZero);
		EXPECT_TRUE(this->lineDirectY != this->lineDegeneratePositive);
		EXPECT_TRUE(this->lineDirectY != this->lineDirectX);
		EXPECT_TRUE(this->lineDirectY != this->lineReverseX);
		EXPECT_FALSE(this->lineDirectY != this->lineDirectY);
		EXPECT_FALSE(this->lineDirectY != this->lineReverseY);
		EXPECT_TRUE(this->lineDirectY != this->lineDirectZ);
		EXPECT_TRUE(this->lineDirectY != this->lineReverseZ);

		EXPECT_TRUE(this->lineReverseY != this->lineDegenerateNegative);
		EXPECT_TRUE(this->lineReverseY != this->lineDegenerateZero);
		EXPECT_TRUE(this->lineReverseY != this->lineDegeneratePositive);
		EXPECT_TRUE(this->lineReverseY != this->lineDirectX);
		EXPECT_TRUE(this->lineReverseY != this->lineReverseX);
		EXPECT_FALSE(this->lineReverseY != this->lineDirectY);
		EXPECT_FALSE(this->lineReverseY != this->lineReverseY);
		EXPECT_TRUE(this->lineReverseY != this->lineDirectZ);
		EXPECT_TRUE(this->lineReverseY != this->lineReverseZ);

		EXPECT_TRUE(this->lineDirectZ != this->lineDegenerateNegative);
		EXPECT_TRUE(this->lineDirectZ != this->lineDegenerateZero);
		EXPECT_TRUE(this->lineDirectZ != this->lineDegeneratePositive);
		EXPECT_TRUE(this->lineDirectZ != this->lineDirectX);
		EXPECT_TRUE(this->lineDirectZ != this->lineReverseX);
		EXPECT_TRUE(this->lineDirectZ != this->lineDirectY);
		EXPECT_TRUE(this->lineDirectZ != this->lineReverseY);
		EXPECT_FALSE(this->lineDirectZ != this->lineDirectZ);
		EXPECT_FALSE(this->lineDirectZ != this->lineReverseZ);

		EXPECT_TRUE(this->lineReverseZ != this->lineDegenerateNegative);
		EXPECT_TRUE(this->lineReverseZ != this->lineDegenerateZero);
		EXPECT_TRUE(this->lineReverseZ != this->lineDegeneratePositive);
		EXPECT_TRUE(this->lineReverseZ != this->lineDirectX);
		EXPECT_TRUE(this->lineReverseZ != this->lineReverseX);
		EXPECT_TRUE(this->lineReverseZ != this->lineDirectY);
		EXPECT_TRUE(this->lineReverseZ != this->lineReverseY);
		EXPECT_FALSE(this->lineReverseZ != this->lineDirectZ);
		EXPECT_FALSE(this->lineReverseZ != this->lineReverseZ);
	}

	TYPED_TEST_P(GeometryLineTest, Angle)
	{
		const typename TypeParam::value_type value1 = typename TypeParam::value_type{0.5};
		const typename TypeParam::value_type value2 = std::sqrt(typename TypeParam::value_type{2}) / typename TypeParam::value_type{2};
		const typename TypeParam::value_type value3 = std::sqrt(typename TypeParam::value_type{3}) / typename TypeParam::value_type{2};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line0degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{0}
				}))};

		const xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line30degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					value3,
					value1
				}))};

		const xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line45degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					value2,
					value2
				}))};

		const xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line60degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					value1,
					value3
				}))};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line90degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				}))};

		const xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line120degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					-value1,
					value3
				}))};

		const xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line135degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					-value2,
					value2
				}))};

		const xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line150degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					-value3,
					value1
				}))};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line180degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{-1},
					typename TypeParam::value_type{0}
				}))};

		const xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line210degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					-value3,
					-value1
				}))};

		const xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line225degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					-value2,
					-value2
				}))};

		const xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line240degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					-value1,
					-value3
				}))};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line270degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{-1}
				}))};

		const xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line300degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					value1,
					-value3
				}))};

		const xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line315degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					value2,
					-value2
				}))};

		const xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > line330degree{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					value3,
					-value1
				}))};

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				line0degree.angle_between(line0degree),
				typename TypeParam::value_type{}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				line0degree.angle_between(line30degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{6}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				line0degree.angle_between(line45degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{4}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				line0degree.angle_between(line60degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{3}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				line0degree.angle_between(line90degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				line0degree.angle_between(line120degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{3}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				line0degree.angle_between(line135degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{4}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				line0degree.angle_between(line150degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{6}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				line0degree.angle_between(line180degree),
				typename TypeParam::value_type{0}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				line0degree.angle_between(line210degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{6}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				line0degree.angle_between(line225degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{4}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				line0degree.angle_between(line240degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{3}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				line0degree.angle_between(line270degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				line0degree.angle_between(line300degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{3}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				line0degree.angle_between(line315degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{4}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				line0degree.angle_between(line330degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{6}));
	}

	TYPED_TEST_P(GeometryLineTest, DistanceVertex)
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
			xablau::geometry::spatial_dimension < 1 > > vertexX{
				std::to_array(
				{
					typename TypeParam::value_type{1}
				})};

		constexpr xablau::geometry::vertex <
			typename TypeParam::value_type,
			xablau::geometry::spatial_dimension < 2 > > vertexY{
				std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				})};

		constexpr xablau::geometry::vertex <
			typename TypeParam::value_type,
			xablau::geometry::spatial_dimension < 3 > > vertexZ{
				std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				})};

		EXPECT_EQ(
			this->lineDirectX.distance_to < false > (vertexOrigin),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->lineReverseX.distance_to < false > (vertexOrigin),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->lineDirectY.distance_to < false > (vertexOrigin),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->lineReverseY.distance_to < false > (vertexOrigin),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->lineDirectZ.distance_to < false > (vertexOrigin),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->lineReverseZ.distance_to < false > (vertexOrigin),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->lineDirectX.distance_to < false > (vertexDiagonal),
			std::sqrt(typename TypeParam::value_type{2}));

		EXPECT_EQ(
			this->lineReverseX.distance_to < false > (vertexDiagonal),
			std::sqrt(typename TypeParam::value_type{2}));

		EXPECT_EQ(
			this->lineDirectY.distance_to < false > (vertexDiagonal),
			std::sqrt(typename TypeParam::value_type{2}));

		EXPECT_EQ(
			this->lineReverseY.distance_to < false > (vertexDiagonal),
			std::sqrt(typename TypeParam::value_type{2}));

		EXPECT_EQ(
			this->lineDirectZ.distance_to < false > (vertexDiagonal),
			std::sqrt(typename TypeParam::value_type{2}));

		EXPECT_EQ(
			this->lineReverseZ.distance_to < false > (vertexDiagonal),
			std::sqrt(typename TypeParam::value_type{2}));

		EXPECT_EQ(
			this->lineDirectX.distance_to < true > (vertexOrigin).second,
			vertexOrigin);

		EXPECT_EQ(
			this->lineReverseX.distance_to < true > (vertexOrigin).second,
			vertexOrigin);

		EXPECT_EQ(
			this->lineDirectY.distance_to < true > (vertexOrigin).second,
			vertexOrigin);

		EXPECT_EQ(
			this->lineReverseY.distance_to < true > (vertexOrigin).second,
			vertexOrigin);

		EXPECT_EQ(
			this->lineDirectZ.distance_to < true > (vertexOrigin).second,
			vertexOrigin);

		EXPECT_EQ(
			this->lineReverseZ.distance_to < true > (vertexOrigin).second,
			vertexOrigin);

		EXPECT_EQ(
			this->lineDirectX.distance_to < true > (vertexDiagonal).second,
			vertexX);

		EXPECT_EQ(
			this->lineReverseX.distance_to < true > (vertexDiagonal).second,
			vertexX);

		EXPECT_EQ(
			this->lineDirectY.distance_to < true > (vertexDiagonal).second,
			vertexY);

		EXPECT_EQ(
			this->lineReverseY.distance_to < true > (vertexDiagonal).second,
			vertexY);

		EXPECT_EQ(
			this->lineDirectZ.distance_to < true > (vertexDiagonal).second,
			vertexZ);

		EXPECT_EQ(
			this->lineReverseZ.distance_to < true > (vertexDiagonal).second,
			vertexZ);
	}

	TYPED_TEST_P(GeometryLineTest, DistanceLine)
	{
		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 0 > > vertexOrigin{};

		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 1 > > vertexOnLineX{
			std::to_array(
				{
					typename TypeParam::value_type{1}
				})};

		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertexOnLineY{
			std::to_array(
				{
					typename TypeParam::value_type{},
					typename TypeParam::value_type{1}
				})};

		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > vertexOnLineZ{
			std::to_array(
				{
					typename TypeParam::value_type{},
					typename TypeParam::value_type{},
					typename TypeParam::value_type{2}
				})};

		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > vertexOnDiagonalXY{
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})};

		constexpr auto vertexOnDiagonalZ = vertexOnLineZ;

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
					typename TypeParam::value_type{-1}
				}))};

		EXPECT_EQ(
			this->lineDirectX.distance_to < false > (lineOrigin),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->lineReverseX.distance_to < false > (lineOrigin),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->lineDirectY.distance_to < false > (lineOrigin),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->lineReverseY.distance_to < false > (lineOrigin),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->lineDirectZ.distance_to < false > (lineOrigin),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->lineReverseZ.distance_to < false > (lineOrigin),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->lineDirectX.distance_to < false > (lineDiagonal),
			std::sqrt(typename TypeParam::value_type{2}));

		EXPECT_EQ(
			this->lineReverseX.distance_to < false > (lineDiagonal),
			std::sqrt(typename TypeParam::value_type{2}));

		EXPECT_EQ(
			this->lineDirectY.distance_to < false > (lineDiagonal),
			std::sqrt(typename TypeParam::value_type{2}));

		EXPECT_EQ(
			this->lineReverseY.distance_to < false > (lineDiagonal),
			std::sqrt(typename TypeParam::value_type{2}));

		EXPECT_EQ(
			this->lineDirectZ.distance_to < false > (lineDiagonal),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->lineReverseZ.distance_to < false > (lineDiagonal),
			typename TypeParam::value_type{});

		auto resultX = this->lineDirectX.distance_to < true > (lineOrigin);

		EXPECT_EQ(std::get < 1 > (resultX), vertexOrigin);
		EXPECT_EQ(std::get < 2 > (resultX), vertexOrigin);

		resultX = this->lineReverseX.distance_to < true > (lineOrigin);

		EXPECT_EQ(std::get < 1 > (resultX), vertexOrigin);
		EXPECT_EQ(std::get < 2 > (resultX), vertexOrigin);

		auto resultY = this->lineDirectY.distance_to < true > (lineOrigin);

		EXPECT_EQ(std::get < 1 > (resultY), vertexOrigin);
		EXPECT_EQ(std::get < 2 > (resultY), vertexOrigin);

		resultY = this->lineReverseY.distance_to < true > (lineOrigin);

		EXPECT_EQ(std::get < 1 > (resultY), vertexOrigin);
		EXPECT_EQ(std::get < 2 > (resultY), vertexOrigin);

		auto resultZ = this->lineDirectZ.distance_to < true > (lineOrigin);

		EXPECT_EQ(std::get < 1 > (resultZ), vertexOrigin);
		EXPECT_EQ(std::get < 2 > (resultZ), vertexOrigin);

		resultZ = this->lineReverseZ.distance_to < true > (lineOrigin);

		EXPECT_EQ(std::get < 1 > (resultZ), vertexOrigin);
		EXPECT_EQ(std::get < 2 > (resultZ), vertexOrigin);

		resultX = this->lineDirectX.distance_to < true > (lineDiagonal);

		EXPECT_EQ(std::get < 1 > (resultX), vertexOnLineX);
		EXPECT_EQ(std::get < 2 > (resultX), vertexOnDiagonalXY);

		resultX = this->lineReverseX.distance_to < true > (lineDiagonal);

		EXPECT_EQ(std::get < 1 > (resultX), vertexOnLineX);
		EXPECT_EQ(std::get < 2 > (resultX), vertexOnDiagonalXY);

		resultY = this->lineDirectY.distance_to < true > (lineDiagonal);

		EXPECT_EQ(std::get < 1 > (resultY), vertexOnLineY);
		EXPECT_EQ(std::get < 2 > (resultY), vertexOnDiagonalXY);

		resultY = this->lineReverseY.distance_to < true > (lineDiagonal);

		EXPECT_EQ(std::get < 1 > (resultY), vertexOnLineY);
		EXPECT_EQ(std::get < 2 > (resultY), vertexOnDiagonalXY);

		resultZ = this->lineDirectZ.distance_to < true > (lineDiagonal);

		EXPECT_EQ(std::get < 1 > (resultZ), vertexOnLineZ);
		EXPECT_EQ(std::get < 2 > (resultZ), vertexOnDiagonalZ);

		resultZ = this->lineReverseZ.distance_to < true > (lineDiagonal);

		EXPECT_EQ(std::get < 1 > (resultZ), vertexOnLineZ);
		EXPECT_EQ(std::get < 2 > (resultZ), vertexOnDiagonalZ);
	}

	TYPED_TEST_P(GeometryLineTest, Containment)
	{
		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 0 > > vertexOrigin{};

		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > vertexOnDiagonal{
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})};

		EXPECT_TRUE(this->lineDirectX.contains(vertexOrigin));
		EXPECT_TRUE(this->lineReverseX.contains(vertexOrigin));
		EXPECT_TRUE(this->lineDirectY.contains(vertexOrigin));
		EXPECT_TRUE(this->lineReverseY.contains(vertexOrigin));
		EXPECT_TRUE(this->lineDirectZ.contains(vertexOrigin));
		EXPECT_TRUE(this->lineReverseZ.contains(vertexOrigin));

		EXPECT_FALSE(this->lineDirectX.contains(vertexOnDiagonal));
		EXPECT_FALSE(this->lineReverseX.contains(vertexOnDiagonal));
		EXPECT_FALSE(this->lineDirectY.contains(vertexOnDiagonal));
		EXPECT_FALSE(this->lineReverseY.contains(vertexOnDiagonal));
		EXPECT_FALSE(this->lineDirectZ.contains(vertexOnDiagonal));
		EXPECT_FALSE(this->lineReverseZ.contains(vertexOnDiagonal));
	}

	TYPED_TEST_P(GeometryLineTest, IntersectionLine)
	{
		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 0 > > vertexOrigin{};

		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > vertexOnLineZ{
			std::to_array(
				{
					typename TypeParam::value_type{},
					typename TypeParam::value_type{},
					typename TypeParam::value_type{2}
				})};

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
					typename TypeParam::value_type{-1}
				}))};

		EXPECT_TRUE(this->lineDirectX.intersects < false > (this->lineDirectX));
		EXPECT_TRUE(this->lineDirectX.intersects < false > (this->lineReverseX));
		EXPECT_TRUE(this->lineDirectX.intersects < false > (this->lineDirectY));
		EXPECT_TRUE(this->lineDirectX.intersects < false > (this->lineReverseY));
		EXPECT_TRUE(this->lineDirectX.intersects < false > (this->lineDirectZ));
		EXPECT_TRUE(this->lineDirectX.intersects < false > (this->lineReverseZ));

		EXPECT_FALSE(lineDiagonal.intersects < false > (this->lineDirectX));
		EXPECT_FALSE(lineDiagonal.intersects < false > (this->lineReverseX));
		EXPECT_FALSE(lineDiagonal.intersects < false > (this->lineDirectY));
		EXPECT_FALSE(lineDiagonal.intersects < false > (this->lineReverseY));
		EXPECT_TRUE(lineDiagonal.intersects < false > (this->lineDirectZ));
		EXPECT_TRUE(lineDiagonal.intersects < false > (this->lineReverseZ));

		EXPECT_EQ(this->lineDirectX.intersects < true > (this->lineDirectX).value(), vertexOrigin);
		EXPECT_EQ(this->lineDirectX.intersects < true > (this->lineReverseX).value(), vertexOrigin);
		EXPECT_EQ(this->lineDirectX.intersects < true > (this->lineDirectY).value(), vertexOrigin);
		EXPECT_EQ(this->lineDirectX.intersects < true > (this->lineReverseY).value(), vertexOrigin);
		EXPECT_EQ(this->lineDirectX.intersects < true > (this->lineDirectZ).value(), vertexOrigin);
		EXPECT_EQ(this->lineDirectX.intersects < true > (this->lineReverseZ).value(), vertexOrigin);

		EXPECT_EQ(lineDiagonal.intersects < true > (this->lineDirectX), std::nullopt);
		EXPECT_EQ(lineDiagonal.intersects < true > (this->lineReverseX), std::nullopt);
		EXPECT_EQ(lineDiagonal.intersects < true > (this->lineDirectY), std::nullopt);
		EXPECT_EQ(lineDiagonal.intersects < true > (this->lineReverseY), std::nullopt);
		EXPECT_EQ(lineDiagonal.intersects < true > (this->lineDirectZ).value(), vertexOnLineZ);
		EXPECT_EQ(lineDiagonal.intersects < true > (this->lineReverseZ).value(), vertexOnLineZ);
	}

	TYPED_TEST_P(GeometryLineTest, Coincidence)
	{
		EXPECT_TRUE(this->lineDirectX.coincides_with(this->lineDirectX));
		EXPECT_TRUE(this->lineDirectX.coincides_with(this->lineReverseX));
		EXPECT_FALSE(this->lineDirectX.coincides_with(this->lineDirectY));
		EXPECT_FALSE(this->lineDirectX.coincides_with(this->lineReverseY));
		EXPECT_FALSE(this->lineDirectX.coincides_with(this->lineDirectZ));
		EXPECT_FALSE(this->lineDirectX.coincides_with(this->lineReverseZ));

		EXPECT_FALSE(this->lineReverseZ.coincides_with(this->lineDirectX));
		EXPECT_FALSE(this->lineReverseZ.coincides_with(this->lineReverseX));
		EXPECT_FALSE(this->lineReverseZ.coincides_with(this->lineDirectY));
		EXPECT_FALSE(this->lineReverseZ.coincides_with(this->lineReverseY));
		EXPECT_TRUE(this->lineReverseZ.coincides_with(this->lineDirectZ));
		EXPECT_TRUE(this->lineReverseZ.coincides_with(this->lineReverseZ));
	}

	TYPED_TEST_P(GeometryLineTest, Concurrence)
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
					typename TypeParam::value_type{-1}
				}))};

		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > vertexOnLineZ{
			std::to_array(
				{
					typename TypeParam::value_type{},
					typename TypeParam::value_type{},
					typename TypeParam::value_type{2}
				})};

		EXPECT_FALSE(this->lineDirectX.concurres_with < false > (this->lineDirectX));
		EXPECT_FALSE(this->lineDirectX.concurres_with < false > (this->lineReverseX));
		EXPECT_TRUE(this->lineDirectX.concurres_with < false > (this->lineDirectY));
		EXPECT_TRUE(this->lineDirectX.concurres_with < false > (this->lineReverseY));
		EXPECT_TRUE(this->lineDirectX.concurres_with < false > (this->lineDirectZ));
		EXPECT_TRUE(this->lineDirectX.concurres_with < false >  (this->lineReverseZ));

		EXPECT_TRUE(this->lineReverseZ.concurres_with < false > (this->lineDirectX));
		EXPECT_TRUE(this->lineReverseZ.concurres_with < false > (this->lineReverseX));
		EXPECT_TRUE(this->lineReverseZ.concurres_with < false > (this->lineDirectY));
		EXPECT_TRUE(this->lineReverseZ.concurres_with < false > (this->lineReverseY));
		EXPECT_FALSE(this->lineReverseZ.concurres_with < false > (this->lineDirectZ));
		EXPECT_FALSE(this->lineReverseZ.concurres_with < false > (this->lineReverseZ));

		EXPECT_FALSE(lineDiagonal.concurres_with < false > (this->lineDirectX));
		EXPECT_FALSE(lineDiagonal.concurres_with < false > (this->lineReverseX));
		EXPECT_FALSE(lineDiagonal.concurres_with < false > (this->lineDirectY));
		EXPECT_FALSE(lineDiagonal.concurres_with < false > (this->lineReverseY));
		EXPECT_TRUE(lineDiagonal.concurres_with < false > (this->lineDirectZ));
		EXPECT_TRUE(lineDiagonal.concurres_with < false > (this->lineReverseZ));

		EXPECT_EQ(this->lineDirectX.concurres_with < true > (this->lineDirectX), std::nullopt);
		EXPECT_EQ(this->lineDirectX.concurres_with < true > (this->lineReverseX), std::nullopt);
		EXPECT_EQ(this->lineDirectX.concurres_with < true > (this->lineDirectY).value(), vertexOrigin);
		EXPECT_EQ(this->lineDirectX.concurres_with < true > (this->lineReverseY).value(), vertexOrigin);
		EXPECT_EQ(this->lineDirectX.concurres_with < true > (this->lineDirectZ).value(), vertexOrigin);
		EXPECT_EQ(this->lineDirectX.concurres_with < true > (this->lineReverseZ).value(), vertexOrigin);

		EXPECT_EQ(this->lineReverseZ.concurres_with < true > (this->lineDirectX).value(), vertexOrigin);
		EXPECT_EQ(this->lineReverseZ.concurres_with < true > (this->lineReverseX).value(), vertexOrigin);
		EXPECT_EQ(this->lineReverseZ.concurres_with < true > (this->lineDirectY).value(), vertexOrigin);
		EXPECT_EQ(this->lineReverseZ.concurres_with < true > (this->lineReverseY).value(), vertexOrigin);
		EXPECT_EQ(this->lineReverseZ.concurres_with < true > (this->lineDirectZ), std::nullopt);
		EXPECT_EQ(this->lineReverseZ.concurres_with < true > (this->lineReverseZ), std::nullopt);

		EXPECT_EQ(lineDiagonal.concurres_with < true > (this->lineDirectX), std::nullopt);
		EXPECT_EQ(lineDiagonal.concurres_with < true > (this->lineReverseX), std::nullopt);
		EXPECT_EQ(lineDiagonal.concurres_with < true > (this->lineDirectY), std::nullopt);
		EXPECT_EQ(lineDiagonal.concurres_with < true > (this->lineReverseY), std::nullopt);
		EXPECT_EQ(lineDiagonal.concurres_with < true > (this->lineDirectZ).value(), vertexOnLineZ);
		EXPECT_EQ(lineDiagonal.concurres_with < true > (this->lineReverseZ).value(), vertexOnLineZ);
	}

	TYPED_TEST_P(GeometryLineTest, Perpendicularity)
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
					typename TypeParam::value_type{-1}
				}))};

		EXPECT_FALSE(this->lineDirectX.perpendicular_to < false > (this->lineDirectX));
		EXPECT_FALSE(this->lineDirectX.perpendicular_to < false > (this->lineReverseX));
		EXPECT_TRUE(this->lineDirectX.perpendicular_to < false > (this->lineDirectY));
		EXPECT_TRUE(this->lineDirectX.perpendicular_to < false > (this->lineReverseY));
		EXPECT_TRUE(this->lineDirectX.perpendicular_to < false > (this->lineDirectZ));
		EXPECT_TRUE(this->lineDirectX.perpendicular_to < false > (this->lineReverseZ));

		EXPECT_FALSE(lineDiagonal.perpendicular_to < false > (this->lineDirectX));
		EXPECT_FALSE(lineDiagonal.perpendicular_to < false > (this->lineReverseX));
		EXPECT_FALSE(lineDiagonal.perpendicular_to < false > (this->lineDirectY));
		EXPECT_FALSE(lineDiagonal.perpendicular_to < false > (this->lineReverseY));
		EXPECT_FALSE(lineDiagonal.perpendicular_to < false > (this->lineDirectZ));
		EXPECT_FALSE(lineDiagonal.perpendicular_to < false > (this->lineReverseZ));

		EXPECT_EQ(this->lineDirectX.perpendicular_to < true > (this->lineDirectX), std::nullopt);
		EXPECT_EQ(this->lineDirectX.perpendicular_to < true > (this->lineReverseX), std::nullopt);
		EXPECT_EQ(this->lineDirectX.perpendicular_to < true > (this->lineDirectY).value(), vertexOrigin);
		EXPECT_EQ(this->lineDirectX.perpendicular_to < true > (this->lineReverseY).value(), vertexOrigin);
		EXPECT_EQ(this->lineDirectX.perpendicular_to < true > (this->lineDirectZ).value(), vertexOrigin);
		EXPECT_EQ(this->lineDirectX.perpendicular_to < true > (this->lineReverseZ).value(), vertexOrigin);

		EXPECT_EQ(lineDiagonal.perpendicular_to < true > (this->lineDirectX), std::nullopt);
		EXPECT_EQ(lineDiagonal.perpendicular_to < true > (this->lineReverseX), std::nullopt);
		EXPECT_EQ(lineDiagonal.perpendicular_to < true > (this->lineDirectY), std::nullopt);
		EXPECT_EQ(lineDiagonal.perpendicular_to < true > (this->lineReverseY), std::nullopt);
		EXPECT_EQ(lineDiagonal.perpendicular_to < true > (this->lineDirectZ), std::nullopt);
		EXPECT_EQ(lineDiagonal.perpendicular_to < true > (this->lineReverseZ), std::nullopt);
	}

	TYPED_TEST_P(GeometryLineTest, Parallelism)
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
					typename TypeParam::value_type{-1}
				}))};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > lineParallelToX{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{},
					typename TypeParam::value_type{}
				}))};

		EXPECT_FALSE(this->lineDirectX.parallel_to(this->lineDirectX));
		EXPECT_FALSE(this->lineDirectX.parallel_to(this->lineReverseX));
		EXPECT_FALSE(this->lineDirectX.parallel_to(this->lineDirectY));
		EXPECT_FALSE(this->lineDirectX.parallel_to(this->lineReverseY));
		EXPECT_FALSE(this->lineDirectX.parallel_to(this->lineDirectZ));
		EXPECT_FALSE(this->lineDirectX.parallel_to(this->lineReverseZ));

		EXPECT_FALSE(lineDiagonal.parallel_to(this->lineDirectX));
		EXPECT_FALSE(lineDiagonal.parallel_to(this->lineReverseX));
		EXPECT_FALSE(lineDiagonal.parallel_to(this->lineDirectY));
		EXPECT_FALSE(lineDiagonal.parallel_to(this->lineReverseY));
		EXPECT_FALSE(lineDiagonal.parallel_to(this->lineDirectZ));
		EXPECT_FALSE(lineDiagonal.parallel_to(this->lineReverseZ));

		EXPECT_TRUE(lineParallelToX.parallel_to(this->lineDirectX));
		EXPECT_TRUE(lineParallelToX.parallel_to(this->lineReverseX));
		EXPECT_FALSE(lineParallelToX.parallel_to(this->lineDirectY));
		EXPECT_FALSE(lineParallelToX.parallel_to(this->lineReverseY));
		EXPECT_FALSE(lineParallelToX.parallel_to(this->lineDirectZ));
		EXPECT_FALSE(lineParallelToX.parallel_to(this->lineReverseZ));
	}

	REGISTER_TYPED_TEST_SUITE_P(
		GeometryLineTest,
		ComparisonOperatorLesser,
		ComparisonOperatorLesserEqualTo,
		ComparisonOperatorEqualTo,
		ComparisonOperatorGreaterEqualTo,
		ComparisonOperatorGreater,
		ComparisonOperatorNotEqualTo,
		Angle,
		DistanceVertex,
		DistanceLine,
		Containment,
		IntersectionLine,
		Coincidence,
		Concurrence,
		Perpendicularity,
		Parallelism);

	using GeometryLineTestDataTypes = std::tuple < float, double, long double >;

	using GeometryLineTestGroup =
		xablau::testing::test_bundler < GeometryLineTestDataTypes > ::template test_group < GeometryLineTypeDefinitions >;

	using GeometryLineTypes = xablau::testing::tuple_transformation < ::testing::Types, GeometryLineTestGroup > ::types;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		Xablau,
		GeometryLineTest,
		GeometryLineTypes);
}
