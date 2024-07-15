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
	struct GeometryEuclideanTriangleTypeDefinitions
	{
		using value_type = Type;
	};

	template < typename Types >
	class GeometryEuclideanTriangleTest : public ::testing::Test
	{
	public:
		static constexpr xablau::geometry::vertex < typename Types::value_type, xablau::geometry::spatial_dimension < 0 > > vertexOrigin{};
		static constexpr auto vertexPositiveX = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(1) }));
		static constexpr auto vertexNegativeX = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(-1) }));
		static constexpr auto vertexPositiveY = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(), typename Types::value_type(1) }));
		static constexpr auto vertexPositiveZ = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(), typename Types::value_type(), typename Types::value_type(1) }));
		static constexpr auto vertexHalfDiagonal = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(0.5), typename Types::value_type(0.5) }));
		static constexpr auto vertexDoubleDiagonal = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(2), typename Types::value_type(2) }));

		static constexpr auto triangleDegenerateNegative = xablau::geometry::make_euclidean_triangle(vertexOrigin, vertexNegativeX, vertexOrigin);
		static constexpr auto triangleDegenerateZero = xablau::geometry::make_euclidean_triangle(vertexOrigin, vertexOrigin, vertexOrigin);
		static constexpr auto triangleDegeneratePositive = xablau::geometry::make_euclidean_triangle(vertexPositiveX, vertexOrigin, vertexOrigin);

		static constexpr auto triangleEquilateral = xablau::geometry::make_euclidean_triangle(vertexPositiveZ, vertexPositiveY, vertexPositiveX);
		static constexpr auto triangleIsosceles = xablau::geometry::make_euclidean_triangle(vertexOrigin, vertexPositiveX, vertexHalfDiagonal);
		static constexpr auto triangleScalene = xablau::geometry::make_euclidean_triangle(vertexOrigin, vertexPositiveX, vertexDoubleDiagonal);
		static constexpr auto triangleScaleneRotated1 = xablau::geometry::make_euclidean_triangle(vertexPositiveX, vertexDoubleDiagonal, vertexOrigin);
		static constexpr auto triangleScaleneRotated2 = xablau::geometry::make_euclidean_triangle(vertexDoubleDiagonal, vertexOrigin, vertexPositiveX);

		static void coordinatesAreAlmostEqual(const auto &coordinates1, const auto &coordinates2)
		{
			size_t i = 0;
			const auto min = std::min(coordinates1.size(), coordinates2.size());

			for (i = 0; i < min; i++)
			{
				EXPECT_LT(std::abs(coordinates1[i] - coordinates2[i]), typename Types::value_type{0.00001});
			}

			for (; i < coordinates1.size(); i++)
			{
				EXPECT_LT(std::abs(coordinates1[i]), typename Types::value_type{0.00001});
			}

			for (; i < coordinates2.size(); i++)
			{
				EXPECT_LT(std::abs(coordinates2[i]), typename Types::value_type{0.00001});
			}
		}
	};

	TYPED_TEST_SUITE_P(GeometryEuclideanTriangleTest);

	TYPED_TEST_P(GeometryEuclideanTriangleTest, ComparisonOperatorLess)
	{
		EXPECT_FALSE(this->triangleDegenerateNegative < this->triangleDegenerateNegative);
		EXPECT_TRUE(this->triangleDegenerateNegative < this->triangleDegenerateZero);
		EXPECT_TRUE(this->triangleDegenerateNegative < this->triangleDegeneratePositive);
		EXPECT_TRUE(this->triangleDegenerateNegative < this->triangleIsosceles);
		EXPECT_TRUE(this->triangleDegenerateNegative < this->triangleScalene);
		EXPECT_TRUE(this->triangleDegenerateNegative < this->triangleScaleneRotated1);
		EXPECT_TRUE(this->triangleDegenerateNegative < this->triangleScaleneRotated2);
		EXPECT_TRUE(this->triangleDegenerateNegative < this->triangleEquilateral);

		EXPECT_FALSE(this->triangleDegenerateZero < this->triangleDegenerateNegative);
		EXPECT_FALSE(this->triangleDegenerateZero < this->triangleDegenerateZero);
		EXPECT_TRUE(this->triangleDegenerateZero < this->triangleDegeneratePositive);
		EXPECT_TRUE(this->triangleDegenerateZero < this->triangleIsosceles);
		EXPECT_TRUE(this->triangleDegenerateZero < this->triangleScalene);
		EXPECT_TRUE(this->triangleDegenerateZero < this->triangleScaleneRotated1);
		EXPECT_TRUE(this->triangleDegenerateZero < this->triangleScaleneRotated2);
		EXPECT_TRUE(this->triangleDegenerateZero < this->triangleEquilateral);

		EXPECT_FALSE(this->triangleDegeneratePositive < this->triangleDegenerateNegative);
		EXPECT_FALSE(this->triangleDegeneratePositive < this->triangleDegenerateZero);
		EXPECT_FALSE(this->triangleDegeneratePositive < this->triangleDegeneratePositive);
		EXPECT_TRUE(this->triangleDegeneratePositive < this->triangleIsosceles);
		EXPECT_TRUE(this->triangleDegeneratePositive < this->triangleScalene);
		EXPECT_TRUE(this->triangleDegeneratePositive < this->triangleScaleneRotated1);
		EXPECT_TRUE(this->triangleDegeneratePositive < this->triangleScaleneRotated2);
		EXPECT_TRUE(this->triangleDegeneratePositive < this->triangleEquilateral);

		EXPECT_FALSE(this->triangleIsosceles < this->triangleDegenerateNegative);
		EXPECT_FALSE(this->triangleIsosceles < this->triangleDegenerateZero);
		EXPECT_FALSE(this->triangleIsosceles < this->triangleDegeneratePositive);
		EXPECT_FALSE(this->triangleIsosceles < this->triangleIsosceles);
		EXPECT_TRUE(this->triangleIsosceles < this->triangleScalene);
		EXPECT_TRUE(this->triangleIsosceles < this->triangleScaleneRotated1);
		EXPECT_TRUE(this->triangleIsosceles < this->triangleScaleneRotated2);
		EXPECT_TRUE(this->triangleIsosceles < this->triangleEquilateral);

		EXPECT_FALSE(this->triangleScalene < this->triangleDegenerateNegative);
		EXPECT_FALSE(this->triangleScalene < this->triangleDegenerateZero);
		EXPECT_FALSE(this->triangleScalene < this->triangleDegeneratePositive);
		EXPECT_FALSE(this->triangleScalene < this->triangleIsosceles);
		EXPECT_FALSE(this->triangleScalene < this->triangleScalene);
		EXPECT_FALSE(this->triangleScalene < this->triangleScaleneRotated1);
		EXPECT_FALSE(this->triangleScalene < this->triangleScaleneRotated2);
		EXPECT_TRUE(this->triangleScalene < this->triangleEquilateral);

		EXPECT_FALSE(this->triangleScaleneRotated1 < this->triangleDegenerateNegative);
		EXPECT_FALSE(this->triangleScaleneRotated1 < this->triangleDegenerateZero);
		EXPECT_FALSE(this->triangleScaleneRotated1 < this->triangleDegeneratePositive);
		EXPECT_FALSE(this->triangleScaleneRotated1 < this->triangleIsosceles);
		EXPECT_FALSE(this->triangleScaleneRotated1 < this->triangleScalene);
		EXPECT_FALSE(this->triangleScaleneRotated1 < this->triangleScaleneRotated1);
		EXPECT_FALSE(this->triangleScaleneRotated1 < this->triangleScaleneRotated2);
		EXPECT_TRUE(this->triangleScaleneRotated1 < this->triangleEquilateral);

		EXPECT_FALSE(this->triangleScaleneRotated2 < this->triangleDegenerateNegative);
		EXPECT_FALSE(this->triangleScaleneRotated2 < this->triangleDegenerateZero);
		EXPECT_FALSE(this->triangleScaleneRotated2 < this->triangleDegeneratePositive);
		EXPECT_FALSE(this->triangleScaleneRotated2 < this->triangleIsosceles);
		EXPECT_FALSE(this->triangleScaleneRotated2 < this->triangleScalene);
		EXPECT_FALSE(this->triangleScaleneRotated2 < this->triangleScaleneRotated1);
		EXPECT_FALSE(this->triangleScaleneRotated2 < this->triangleScaleneRotated2);
		EXPECT_TRUE(this->triangleScaleneRotated2 < this->triangleEquilateral);

		EXPECT_FALSE(this->triangleEquilateral < this->triangleDegenerateNegative);
		EXPECT_FALSE(this->triangleEquilateral < this->triangleDegenerateZero);
		EXPECT_FALSE(this->triangleEquilateral < this->triangleDegeneratePositive);
		EXPECT_FALSE(this->triangleEquilateral < this->triangleIsosceles);
		EXPECT_FALSE(this->triangleEquilateral < this->triangleScalene);
		EXPECT_FALSE(this->triangleEquilateral < this->triangleScaleneRotated1);
		EXPECT_FALSE(this->triangleEquilateral < this->triangleScaleneRotated2);
		EXPECT_FALSE(this->triangleEquilateral < this->triangleEquilateral);
	}

	TYPED_TEST_P(GeometryEuclideanTriangleTest, ComparisonOperatorLessEqualTo)
	{
		EXPECT_TRUE(this->triangleDegenerateNegative <= this->triangleDegenerateNegative);
		EXPECT_TRUE(this->triangleDegenerateNegative <= this->triangleDegenerateZero);
		EXPECT_TRUE(this->triangleDegenerateNegative <= this->triangleDegeneratePositive);
		EXPECT_TRUE(this->triangleDegenerateNegative <= this->triangleIsosceles);
		EXPECT_TRUE(this->triangleDegenerateNegative <= this->triangleScalene);
		EXPECT_TRUE(this->triangleDegenerateNegative <= this->triangleScaleneRotated1);
		EXPECT_TRUE(this->triangleDegenerateNegative <= this->triangleScaleneRotated2);
		EXPECT_TRUE(this->triangleDegenerateNegative <= this->triangleEquilateral);

		EXPECT_FALSE(this->triangleDegenerateZero <= this->triangleDegenerateNegative);
		EXPECT_TRUE(this->triangleDegenerateZero <= this->triangleDegenerateZero);
		EXPECT_TRUE(this->triangleDegenerateZero <= this->triangleDegeneratePositive);
		EXPECT_TRUE(this->triangleDegenerateZero <= this->triangleIsosceles);
		EXPECT_TRUE(this->triangleDegenerateZero <= this->triangleScalene);
		EXPECT_TRUE(this->triangleDegenerateZero <= this->triangleScaleneRotated1);
		EXPECT_TRUE(this->triangleDegenerateZero <= this->triangleScaleneRotated2);
		EXPECT_TRUE(this->triangleDegenerateZero <= this->triangleEquilateral);

		EXPECT_FALSE(this->triangleDegeneratePositive <= this->triangleDegenerateNegative);
		EXPECT_FALSE(this->triangleDegeneratePositive <= this->triangleDegenerateZero);
		EXPECT_TRUE(this->triangleDegeneratePositive <= this->triangleDegeneratePositive);
		EXPECT_TRUE(this->triangleDegeneratePositive <= this->triangleIsosceles);
		EXPECT_TRUE(this->triangleDegeneratePositive <= this->triangleScalene);
		EXPECT_TRUE(this->triangleDegeneratePositive <= this->triangleScaleneRotated1);
		EXPECT_TRUE(this->triangleDegeneratePositive <= this->triangleScaleneRotated2);
		EXPECT_TRUE(this->triangleDegeneratePositive <= this->triangleEquilateral);

		EXPECT_FALSE(this->triangleIsosceles <= this->triangleDegenerateNegative);
		EXPECT_FALSE(this->triangleIsosceles <= this->triangleDegenerateZero);
		EXPECT_FALSE(this->triangleIsosceles <= this->triangleDegeneratePositive);
		EXPECT_TRUE(this->triangleIsosceles <= this->triangleIsosceles);
		EXPECT_TRUE(this->triangleIsosceles <= this->triangleScalene);
		EXPECT_TRUE(this->triangleIsosceles <= this->triangleScaleneRotated1);
		EXPECT_TRUE(this->triangleIsosceles <= this->triangleScaleneRotated2);
		EXPECT_TRUE(this->triangleIsosceles <= this->triangleEquilateral);

		EXPECT_FALSE(this->triangleScalene <= this->triangleDegenerateNegative);
		EXPECT_FALSE(this->triangleScalene <= this->triangleDegenerateZero);
		EXPECT_FALSE(this->triangleScalene <= this->triangleDegeneratePositive);
		EXPECT_FALSE(this->triangleScalene <= this->triangleIsosceles);
		EXPECT_TRUE(this->triangleScalene <= this->triangleScalene);
		EXPECT_TRUE(this->triangleScalene <= this->triangleScaleneRotated1);
		EXPECT_TRUE(this->triangleScalene <= this->triangleScaleneRotated2);
		EXPECT_TRUE(this->triangleScalene <= this->triangleEquilateral);

		EXPECT_FALSE(this->triangleScaleneRotated1 <= this->triangleDegenerateNegative);
		EXPECT_FALSE(this->triangleScaleneRotated1 <= this->triangleDegenerateZero);
		EXPECT_FALSE(this->triangleScaleneRotated1 <= this->triangleDegeneratePositive);
		EXPECT_FALSE(this->triangleScaleneRotated1 <= this->triangleIsosceles);
		EXPECT_TRUE(this->triangleScaleneRotated1 <= this->triangleScalene);
		EXPECT_TRUE(this->triangleScaleneRotated1 <= this->triangleScaleneRotated1);
		EXPECT_TRUE(this->triangleScaleneRotated1 <= this->triangleScaleneRotated2);
		EXPECT_TRUE(this->triangleScaleneRotated1 <= this->triangleEquilateral);

		EXPECT_FALSE(this->triangleScaleneRotated2 <= this->triangleDegenerateNegative);
		EXPECT_FALSE(this->triangleScaleneRotated2 <= this->triangleDegenerateZero);
		EXPECT_FALSE(this->triangleScaleneRotated2 <= this->triangleDegeneratePositive);
		EXPECT_FALSE(this->triangleScaleneRotated2 <= this->triangleIsosceles);
		EXPECT_TRUE(this->triangleScaleneRotated2 <= this->triangleScalene);
		EXPECT_TRUE(this->triangleScaleneRotated2 <= this->triangleScaleneRotated1);
		EXPECT_TRUE(this->triangleScaleneRotated2 <= this->triangleScaleneRotated2);
		EXPECT_TRUE(this->triangleScaleneRotated2 <= this->triangleEquilateral);

		EXPECT_FALSE(this->triangleEquilateral <= this->triangleDegenerateNegative);
		EXPECT_FALSE(this->triangleEquilateral <= this->triangleDegenerateZero);
		EXPECT_FALSE(this->triangleEquilateral <= this->triangleDegeneratePositive);
		EXPECT_FALSE(this->triangleEquilateral <= this->triangleIsosceles);
		EXPECT_FALSE(this->triangleEquilateral <= this->triangleScalene);
		EXPECT_FALSE(this->triangleEquilateral <= this->triangleScaleneRotated1);
		EXPECT_FALSE(this->triangleEquilateral <= this->triangleScaleneRotated2);
		EXPECT_TRUE(this->triangleEquilateral <= this->triangleEquilateral);
	}

	TYPED_TEST_P(GeometryEuclideanTriangleTest, ComparisonOperatorEqualTo)
	{
		EXPECT_TRUE(this->triangleDegenerateNegative == this->triangleDegenerateNegative);
		EXPECT_FALSE(this->triangleDegenerateNegative == this->triangleDegenerateZero);
		EXPECT_FALSE(this->triangleDegenerateNegative == this->triangleDegeneratePositive);
		EXPECT_FALSE(this->triangleDegenerateNegative == this->triangleIsosceles);
		EXPECT_FALSE(this->triangleDegenerateNegative == this->triangleScalene);
		EXPECT_FALSE(this->triangleDegenerateNegative == this->triangleScaleneRotated1);
		EXPECT_FALSE(this->triangleDegenerateNegative == this->triangleScaleneRotated2);
		EXPECT_FALSE(this->triangleDegenerateNegative == this->triangleEquilateral);

		EXPECT_FALSE(this->triangleDegenerateZero == this->triangleDegenerateNegative);
		EXPECT_TRUE(this->triangleDegenerateZero == this->triangleDegenerateZero);
		EXPECT_FALSE(this->triangleDegenerateZero == this->triangleDegeneratePositive);
		EXPECT_FALSE(this->triangleDegenerateZero == this->triangleIsosceles);
		EXPECT_FALSE(this->triangleDegenerateZero == this->triangleScalene);
		EXPECT_FALSE(this->triangleDegenerateZero == this->triangleScaleneRotated1);
		EXPECT_FALSE(this->triangleDegenerateZero == this->triangleScaleneRotated2);
		EXPECT_FALSE(this->triangleDegenerateZero == this->triangleEquilateral);

		EXPECT_FALSE(this->triangleDegeneratePositive == this->triangleDegenerateNegative);
		EXPECT_FALSE(this->triangleDegeneratePositive == this->triangleDegenerateZero);
		EXPECT_TRUE(this->triangleDegeneratePositive == this->triangleDegeneratePositive);
		EXPECT_FALSE(this->triangleDegeneratePositive == this->triangleIsosceles);
		EXPECT_FALSE(this->triangleDegeneratePositive == this->triangleScalene);
		EXPECT_FALSE(this->triangleDegeneratePositive == this->triangleScaleneRotated1);
		EXPECT_FALSE(this->triangleDegeneratePositive == this->triangleScaleneRotated2);
		EXPECT_FALSE(this->triangleDegeneratePositive == this->triangleEquilateral);

		EXPECT_FALSE(this->triangleIsosceles == this->triangleDegenerateNegative);
		EXPECT_FALSE(this->triangleIsosceles == this->triangleDegenerateZero);
		EXPECT_FALSE(this->triangleIsosceles == this->triangleDegeneratePositive);
		EXPECT_TRUE(this->triangleIsosceles == this->triangleIsosceles);
		EXPECT_FALSE(this->triangleIsosceles == this->triangleScalene);
		EXPECT_FALSE(this->triangleIsosceles == this->triangleScaleneRotated1);
		EXPECT_FALSE(this->triangleIsosceles == this->triangleScaleneRotated2);
		EXPECT_FALSE(this->triangleIsosceles == this->triangleEquilateral);

		EXPECT_FALSE(this->triangleScalene == this->triangleDegenerateNegative);
		EXPECT_FALSE(this->triangleScalene == this->triangleDegenerateZero);
		EXPECT_FALSE(this->triangleScalene == this->triangleDegeneratePositive);
		EXPECT_FALSE(this->triangleScalene == this->triangleIsosceles);
		EXPECT_TRUE(this->triangleScalene == this->triangleScalene);
		EXPECT_TRUE(this->triangleScalene == this->triangleScaleneRotated1);
		EXPECT_TRUE(this->triangleScalene == this->triangleScaleneRotated2);
		EXPECT_FALSE(this->triangleScalene == this->triangleEquilateral);

		EXPECT_FALSE(this->triangleScaleneRotated1 == this->triangleDegenerateNegative);
		EXPECT_FALSE(this->triangleScaleneRotated1 == this->triangleDegenerateZero);
		EXPECT_FALSE(this->triangleScaleneRotated1 == this->triangleDegeneratePositive);
		EXPECT_FALSE(this->triangleScaleneRotated1 == this->triangleIsosceles);
		EXPECT_TRUE(this->triangleScaleneRotated1 == this->triangleScalene);
		EXPECT_TRUE(this->triangleScaleneRotated1 == this->triangleScaleneRotated1);
		EXPECT_TRUE(this->triangleScaleneRotated1 == this->triangleScaleneRotated2);
		EXPECT_FALSE(this->triangleScaleneRotated1 == this->triangleEquilateral);

		EXPECT_FALSE(this->triangleScaleneRotated2 == this->triangleDegenerateNegative);
		EXPECT_FALSE(this->triangleScaleneRotated2 == this->triangleDegenerateZero);
		EXPECT_FALSE(this->triangleScaleneRotated2 == this->triangleDegeneratePositive);
		EXPECT_FALSE(this->triangleScaleneRotated2 == this->triangleIsosceles);
		EXPECT_TRUE(this->triangleScaleneRotated2 == this->triangleScalene);
		EXPECT_TRUE(this->triangleScaleneRotated2 == this->triangleScaleneRotated1);
		EXPECT_TRUE(this->triangleScaleneRotated2 == this->triangleScaleneRotated2);
		EXPECT_FALSE(this->triangleScaleneRotated2 == this->triangleEquilateral);

		EXPECT_FALSE(this->triangleEquilateral == this->triangleDegenerateNegative);
		EXPECT_FALSE(this->triangleEquilateral == this->triangleDegenerateZero);
		EXPECT_FALSE(this->triangleEquilateral == this->triangleDegeneratePositive);
		EXPECT_FALSE(this->triangleEquilateral == this->triangleIsosceles);
		EXPECT_FALSE(this->triangleEquilateral == this->triangleScalene);
		EXPECT_FALSE(this->triangleEquilateral == this->triangleScaleneRotated1);
		EXPECT_FALSE(this->triangleEquilateral == this->triangleScaleneRotated2);
		EXPECT_TRUE(this->triangleEquilateral == this->triangleEquilateral);
	}

	TYPED_TEST_P(GeometryEuclideanTriangleTest, ComparisonOperatorGreaterEqualTo)
	{
		EXPECT_TRUE(this->triangleDegenerateNegative >= this->triangleDegenerateNegative);
		EXPECT_FALSE(this->triangleDegenerateNegative >= this->triangleDegenerateZero);
		EXPECT_FALSE(this->triangleDegenerateNegative >= this->triangleDegeneratePositive);
		EXPECT_FALSE(this->triangleDegenerateNegative >= this->triangleIsosceles);
		EXPECT_FALSE(this->triangleDegenerateNegative >= this->triangleScalene);
		EXPECT_FALSE(this->triangleDegenerateNegative >= this->triangleScaleneRotated1);
		EXPECT_FALSE(this->triangleDegenerateNegative >= this->triangleScaleneRotated2);
		EXPECT_FALSE(this->triangleDegenerateNegative >= this->triangleEquilateral);

		EXPECT_TRUE(this->triangleDegenerateZero >= this->triangleDegenerateNegative);
		EXPECT_TRUE(this->triangleDegenerateZero >= this->triangleDegenerateZero);
		EXPECT_FALSE(this->triangleDegenerateZero >= this->triangleDegeneratePositive);
		EXPECT_FALSE(this->triangleDegenerateZero >= this->triangleIsosceles);
		EXPECT_FALSE(this->triangleDegenerateZero >= this->triangleScalene);
		EXPECT_FALSE(this->triangleDegenerateZero >= this->triangleScaleneRotated1);
		EXPECT_FALSE(this->triangleDegenerateZero >= this->triangleScaleneRotated2);
		EXPECT_FALSE(this->triangleDegenerateZero >= this->triangleEquilateral);

		EXPECT_TRUE(this->triangleDegeneratePositive >= this->triangleDegenerateNegative);
		EXPECT_TRUE(this->triangleDegeneratePositive >= this->triangleDegenerateZero);
		EXPECT_TRUE(this->triangleDegeneratePositive >= this->triangleDegeneratePositive);
		EXPECT_FALSE(this->triangleDegeneratePositive >= this->triangleIsosceles);
		EXPECT_FALSE(this->triangleDegeneratePositive >= this->triangleScalene);
		EXPECT_FALSE(this->triangleDegeneratePositive >= this->triangleScaleneRotated1);
		EXPECT_FALSE(this->triangleDegeneratePositive >= this->triangleScaleneRotated2);
		EXPECT_FALSE(this->triangleDegeneratePositive >= this->triangleEquilateral);

		EXPECT_TRUE(this->triangleIsosceles >= this->triangleDegenerateNegative);
		EXPECT_TRUE(this->triangleIsosceles >= this->triangleDegenerateZero);
		EXPECT_TRUE(this->triangleIsosceles >= this->triangleDegeneratePositive);
		EXPECT_TRUE(this->triangleIsosceles >= this->triangleIsosceles);
		EXPECT_FALSE(this->triangleIsosceles >= this->triangleScalene);
		EXPECT_FALSE(this->triangleIsosceles >= this->triangleScaleneRotated1);
		EXPECT_FALSE(this->triangleIsosceles >= this->triangleScaleneRotated2);
		EXPECT_FALSE(this->triangleIsosceles >= this->triangleEquilateral);

		EXPECT_TRUE(this->triangleScalene >= this->triangleDegenerateNegative);
		EXPECT_TRUE(this->triangleScalene >= this->triangleDegenerateZero);
		EXPECT_TRUE(this->triangleScalene >= this->triangleDegeneratePositive);
		EXPECT_TRUE(this->triangleScalene >= this->triangleIsosceles);
		EXPECT_TRUE(this->triangleScalene >= this->triangleScalene);
		EXPECT_TRUE(this->triangleScalene >= this->triangleScaleneRotated1);
		EXPECT_TRUE(this->triangleScalene >= this->triangleScaleneRotated2);
		EXPECT_FALSE(this->triangleScalene >= this->triangleEquilateral);

		EXPECT_TRUE(this->triangleScaleneRotated1 >= this->triangleDegenerateNegative);
		EXPECT_TRUE(this->triangleScaleneRotated1 >= this->triangleDegenerateZero);
		EXPECT_TRUE(this->triangleScaleneRotated1 >= this->triangleDegeneratePositive);
		EXPECT_TRUE(this->triangleScaleneRotated1 >= this->triangleIsosceles);
		EXPECT_TRUE(this->triangleScaleneRotated1 >= this->triangleScalene);
		EXPECT_TRUE(this->triangleScaleneRotated1 >= this->triangleScaleneRotated1);
		EXPECT_TRUE(this->triangleScaleneRotated1 >= this->triangleScaleneRotated2);
		EXPECT_FALSE(this->triangleScaleneRotated1 >= this->triangleEquilateral);

		EXPECT_TRUE(this->triangleScaleneRotated2 >= this->triangleDegenerateNegative);
		EXPECT_TRUE(this->triangleScaleneRotated2 >= this->triangleDegenerateZero);
		EXPECT_TRUE(this->triangleScaleneRotated2 >= this->triangleDegeneratePositive);
		EXPECT_TRUE(this->triangleScaleneRotated2 >= this->triangleIsosceles);
		EXPECT_TRUE(this->triangleScaleneRotated2 >= this->triangleScalene);
		EXPECT_TRUE(this->triangleScaleneRotated2 >= this->triangleScaleneRotated1);
		EXPECT_TRUE(this->triangleScaleneRotated2 >= this->triangleScaleneRotated2);
		EXPECT_FALSE(this->triangleScaleneRotated2 >= this->triangleEquilateral);

		EXPECT_TRUE(this->triangleEquilateral >= this->triangleDegenerateNegative);
		EXPECT_TRUE(this->triangleEquilateral >= this->triangleDegenerateZero);
		EXPECT_TRUE(this->triangleEquilateral >= this->triangleDegeneratePositive);
		EXPECT_TRUE(this->triangleEquilateral >= this->triangleIsosceles);
		EXPECT_TRUE(this->triangleEquilateral >= this->triangleScalene);
		EXPECT_TRUE(this->triangleEquilateral >= this->triangleScaleneRotated1);
		EXPECT_TRUE(this->triangleEquilateral >= this->triangleScaleneRotated2);
		EXPECT_TRUE(this->triangleEquilateral >= this->triangleEquilateral);
	}

	TYPED_TEST_P(GeometryEuclideanTriangleTest, ComparisonOperatorGreater)
	{
		EXPECT_FALSE(this->triangleDegenerateNegative > this->triangleDegenerateNegative);
		EXPECT_FALSE(this->triangleDegenerateNegative > this->triangleDegenerateZero);
		EXPECT_FALSE(this->triangleDegenerateNegative > this->triangleDegeneratePositive);
		EXPECT_FALSE(this->triangleDegenerateNegative > this->triangleIsosceles);
		EXPECT_FALSE(this->triangleDegenerateNegative > this->triangleScalene);
		EXPECT_FALSE(this->triangleDegenerateNegative > this->triangleScaleneRotated1);
		EXPECT_FALSE(this->triangleDegenerateNegative > this->triangleScaleneRotated2);
		EXPECT_FALSE(this->triangleDegenerateNegative > this->triangleEquilateral);

		EXPECT_TRUE(this->triangleDegenerateZero > this->triangleDegenerateNegative);
		EXPECT_FALSE(this->triangleDegenerateZero > this->triangleDegenerateZero);
		EXPECT_FALSE(this->triangleDegenerateZero > this->triangleDegeneratePositive);
		EXPECT_FALSE(this->triangleDegenerateZero > this->triangleIsosceles);
		EXPECT_FALSE(this->triangleDegenerateZero > this->triangleScalene);
		EXPECT_FALSE(this->triangleDegenerateZero > this->triangleScaleneRotated1);
		EXPECT_FALSE(this->triangleDegenerateZero > this->triangleScaleneRotated2);
		EXPECT_FALSE(this->triangleDegenerateZero > this->triangleEquilateral);

		EXPECT_TRUE(this->triangleDegeneratePositive > this->triangleDegenerateNegative);
		EXPECT_TRUE(this->triangleDegeneratePositive > this->triangleDegenerateZero);
		EXPECT_FALSE(this->triangleDegeneratePositive > this->triangleDegeneratePositive);
		EXPECT_FALSE(this->triangleDegeneratePositive > this->triangleIsosceles);
		EXPECT_FALSE(this->triangleDegeneratePositive > this->triangleScalene);
		EXPECT_FALSE(this->triangleDegeneratePositive > this->triangleScaleneRotated1);
		EXPECT_FALSE(this->triangleDegeneratePositive > this->triangleScaleneRotated2);
		EXPECT_FALSE(this->triangleDegeneratePositive > this->triangleEquilateral);

		EXPECT_TRUE(this->triangleIsosceles > this->triangleDegenerateNegative);
		EXPECT_TRUE(this->triangleIsosceles > this->triangleDegenerateZero);
		EXPECT_TRUE(this->triangleIsosceles > this->triangleDegeneratePositive);
		EXPECT_FALSE(this->triangleIsosceles > this->triangleIsosceles);
		EXPECT_FALSE(this->triangleIsosceles > this->triangleScalene);
		EXPECT_FALSE(this->triangleIsosceles > this->triangleScaleneRotated1);
		EXPECT_FALSE(this->triangleIsosceles > this->triangleScaleneRotated2);
		EXPECT_FALSE(this->triangleIsosceles > this->triangleEquilateral);

		EXPECT_TRUE(this->triangleScalene > this->triangleDegenerateNegative);
		EXPECT_TRUE(this->triangleScalene > this->triangleDegenerateZero);
		EXPECT_TRUE(this->triangleScalene > this->triangleDegeneratePositive);
		EXPECT_TRUE(this->triangleScalene > this->triangleIsosceles);
		EXPECT_FALSE(this->triangleScalene > this->triangleScalene);
		EXPECT_FALSE(this->triangleScalene > this->triangleScaleneRotated1);
		EXPECT_FALSE(this->triangleScalene > this->triangleScaleneRotated2);
		EXPECT_FALSE(this->triangleScalene > this->triangleEquilateral);

		EXPECT_TRUE(this->triangleScaleneRotated1 > this->triangleDegenerateNegative);
		EXPECT_TRUE(this->triangleScaleneRotated1 > this->triangleDegenerateZero);
		EXPECT_TRUE(this->triangleScaleneRotated1 > this->triangleDegeneratePositive);
		EXPECT_TRUE(this->triangleScaleneRotated1 > this->triangleIsosceles);
		EXPECT_FALSE(this->triangleScaleneRotated1 > this->triangleScalene);
		EXPECT_FALSE(this->triangleScaleneRotated1 > this->triangleScaleneRotated1);
		EXPECT_FALSE(this->triangleScaleneRotated1 > this->triangleScaleneRotated2);
		EXPECT_FALSE(this->triangleScaleneRotated1 > this->triangleEquilateral);

		EXPECT_TRUE(this->triangleScaleneRotated2 > this->triangleDegenerateNegative);
		EXPECT_TRUE(this->triangleScaleneRotated2 > this->triangleDegenerateZero);
		EXPECT_TRUE(this->triangleScaleneRotated2 > this->triangleDegeneratePositive);
		EXPECT_TRUE(this->triangleScaleneRotated2 > this->triangleIsosceles);
		EXPECT_FALSE(this->triangleScaleneRotated2 > this->triangleScalene);
		EXPECT_FALSE(this->triangleScaleneRotated2 > this->triangleScaleneRotated1);
		EXPECT_FALSE(this->triangleScaleneRotated2 > this->triangleScaleneRotated2);
		EXPECT_FALSE(this->triangleScaleneRotated2 > this->triangleEquilateral);

		EXPECT_TRUE(this->triangleEquilateral > this->triangleDegenerateNegative);
		EXPECT_TRUE(this->triangleEquilateral > this->triangleDegenerateZero);
		EXPECT_TRUE(this->triangleEquilateral > this->triangleDegeneratePositive);
		EXPECT_TRUE(this->triangleEquilateral > this->triangleIsosceles);
		EXPECT_TRUE(this->triangleEquilateral > this->triangleScalene);
		EXPECT_TRUE(this->triangleEquilateral > this->triangleScaleneRotated1);
		EXPECT_TRUE(this->triangleEquilateral > this->triangleScaleneRotated2);
		EXPECT_FALSE(this->triangleEquilateral > this->triangleEquilateral);
	}

	TYPED_TEST_P(GeometryEuclideanTriangleTest, ComparisonOperatorNotEqualTo)
	{
		EXPECT_FALSE(this->triangleDegenerateNegative != this->triangleDegenerateNegative);
		EXPECT_TRUE(this->triangleDegenerateNegative != this->triangleDegenerateZero);
		EXPECT_TRUE(this->triangleDegenerateNegative != this->triangleDegeneratePositive);
		EXPECT_TRUE(this->triangleDegenerateNegative != this->triangleIsosceles);
		EXPECT_TRUE(this->triangleDegenerateNegative != this->triangleScalene);
		EXPECT_TRUE(this->triangleDegenerateNegative != this->triangleScaleneRotated1);
		EXPECT_TRUE(this->triangleDegenerateNegative != this->triangleScaleneRotated2);
		EXPECT_TRUE(this->triangleDegenerateNegative != this->triangleEquilateral);

		EXPECT_TRUE(this->triangleDegenerateZero != this->triangleDegenerateNegative);
		EXPECT_FALSE(this->triangleDegenerateZero != this->triangleDegenerateZero);
		EXPECT_TRUE(this->triangleDegenerateZero != this->triangleDegeneratePositive);
		EXPECT_TRUE(this->triangleDegenerateZero != this->triangleIsosceles);
		EXPECT_TRUE(this->triangleDegenerateZero != this->triangleScalene);
		EXPECT_TRUE(this->triangleDegenerateZero != this->triangleScaleneRotated1);
		EXPECT_TRUE(this->triangleDegenerateZero != this->triangleScaleneRotated2);
		EXPECT_TRUE(this->triangleDegenerateZero != this->triangleEquilateral);

		EXPECT_TRUE(this->triangleDegeneratePositive != this->triangleDegenerateNegative);
		EXPECT_TRUE(this->triangleDegeneratePositive != this->triangleDegenerateZero);
		EXPECT_FALSE(this->triangleDegeneratePositive != this->triangleDegeneratePositive);
		EXPECT_TRUE(this->triangleDegeneratePositive != this->triangleIsosceles);
		EXPECT_TRUE(this->triangleDegeneratePositive != this->triangleScalene);
		EXPECT_TRUE(this->triangleDegeneratePositive != this->triangleScaleneRotated1);
		EXPECT_TRUE(this->triangleDegeneratePositive != this->triangleScaleneRotated2);
		EXPECT_TRUE(this->triangleDegeneratePositive != this->triangleEquilateral);

		EXPECT_TRUE(this->triangleIsosceles != this->triangleDegenerateNegative);
		EXPECT_TRUE(this->triangleIsosceles != this->triangleDegenerateZero);
		EXPECT_TRUE(this->triangleIsosceles != this->triangleDegeneratePositive);
		EXPECT_FALSE(this->triangleIsosceles != this->triangleIsosceles);
		EXPECT_TRUE(this->triangleIsosceles != this->triangleScalene);
		EXPECT_TRUE(this->triangleIsosceles != this->triangleScaleneRotated1);
		EXPECT_TRUE(this->triangleIsosceles != this->triangleScaleneRotated2);
		EXPECT_TRUE(this->triangleIsosceles != this->triangleEquilateral);

		EXPECT_TRUE(this->triangleScalene != this->triangleDegenerateNegative);
		EXPECT_TRUE(this->triangleScalene != this->triangleDegenerateZero);
		EXPECT_TRUE(this->triangleScalene != this->triangleDegeneratePositive);
		EXPECT_TRUE(this->triangleScalene != this->triangleIsosceles);
		EXPECT_FALSE(this->triangleScalene != this->triangleScalene);
		EXPECT_FALSE(this->triangleScalene != this->triangleScaleneRotated1);
		EXPECT_FALSE(this->triangleScalene != this->triangleScaleneRotated2);
		EXPECT_TRUE(this->triangleScalene != this->triangleEquilateral);

		EXPECT_TRUE(this->triangleScaleneRotated1 != this->triangleDegenerateNegative);
		EXPECT_TRUE(this->triangleScaleneRotated1 != this->triangleDegenerateZero);
		EXPECT_TRUE(this->triangleScaleneRotated1 != this->triangleDegeneratePositive);
		EXPECT_TRUE(this->triangleScaleneRotated1 != this->triangleIsosceles);
		EXPECT_FALSE(this->triangleScaleneRotated1 != this->triangleScalene);
		EXPECT_FALSE(this->triangleScaleneRotated1 != this->triangleScaleneRotated1);
		EXPECT_FALSE(this->triangleScaleneRotated1 != this->triangleScaleneRotated2);
		EXPECT_TRUE(this->triangleScaleneRotated1 != this->triangleEquilateral);

		EXPECT_TRUE(this->triangleScaleneRotated2 != this->triangleDegenerateNegative);
		EXPECT_TRUE(this->triangleScaleneRotated2 != this->triangleDegenerateZero);
		EXPECT_TRUE(this->triangleScaleneRotated2 != this->triangleDegeneratePositive);
		EXPECT_TRUE(this->triangleScaleneRotated2 != this->triangleIsosceles);
		EXPECT_FALSE(this->triangleScaleneRotated2 != this->triangleScalene);
		EXPECT_FALSE(this->triangleScaleneRotated2 != this->triangleScaleneRotated1);
		EXPECT_FALSE(this->triangleScaleneRotated2 != this->triangleScaleneRotated2);
		EXPECT_TRUE(this->triangleScaleneRotated2 != this->triangleEquilateral);

		EXPECT_TRUE(this->triangleEquilateral != this->triangleDegenerateNegative);
		EXPECT_TRUE(this->triangleEquilateral != this->triangleDegenerateZero);
		EXPECT_TRUE(this->triangleEquilateral != this->triangleDegeneratePositive);
		EXPECT_TRUE(this->triangleEquilateral != this->triangleIsosceles);
		EXPECT_TRUE(this->triangleEquilateral != this->triangleScalene);
		EXPECT_TRUE(this->triangleEquilateral != this->triangleScaleneRotated1);
		EXPECT_TRUE(this->triangleEquilateral != this->triangleScaleneRotated2);
		EXPECT_FALSE(this->triangleEquilateral != this->triangleEquilateral);
	}

	TYPED_TEST_P(GeometryEuclideanTriangleTest, AccessAliases)
	{
		EXPECT_EQ(this->triangleEquilateral[0], this->triangleEquilateral.vertex1);
		EXPECT_EQ(this->triangleEquilateral[1], this->triangleEquilateral.vertex2);
		EXPECT_EQ(this->triangleEquilateral[2], this->triangleEquilateral.vertex3);

		EXPECT_EQ(this->triangleIsosceles[0], this->triangleIsosceles.vertex1);
		EXPECT_EQ(this->triangleIsosceles[1], this->triangleIsosceles.vertex2);
		EXPECT_EQ(this->triangleIsosceles[2], this->triangleIsosceles.vertex3);

		EXPECT_EQ(this->triangleScalene[0], this->triangleScalene.vertex1);
		EXPECT_EQ(this->triangleScalene[1], this->triangleScalene.vertex2);
		EXPECT_EQ(this->triangleScalene[2], this->triangleScalene.vertex3);
	}

	TYPED_TEST_P(GeometryEuclideanTriangleTest, Distance)
	{
		EXPECT_EQ(
			this->triangleEquilateral.distance_edge1(),
			this->triangleEquilateral[0].distance_to(this->triangleEquilateral[1]));

		EXPECT_EQ(
			this->triangleEquilateral.distance_edge2(),
			this->triangleEquilateral[1].distance_to(this->triangleEquilateral[2]));

		EXPECT_EQ(
			this->triangleEquilateral.distance_edge3(),
			this->triangleEquilateral[2].distance_to(this->triangleEquilateral[0]));

		EXPECT_EQ(
			this->triangleScalene.distance_edge1(),
			this->triangleScalene[0].distance_to(this->triangleScalene[1]));

		EXPECT_EQ(
			this->triangleScalene.distance_edge2(),
			this->triangleScalene[1].distance_to(this->triangleScalene[2]));

		EXPECT_EQ(
			this->triangleScalene.distance_edge3(),
			this->triangleScalene[2].distance_to(this->triangleScalene[0]));
	}

	TYPED_TEST_P(GeometryEuclideanTriangleTest, Angle)
	{
		EXPECT_EQ(
			this->triangleEquilateral.angle1(),
			this->triangleEquilateral[0].angle(this->triangleEquilateral[1], this->triangleEquilateral[2]));

		EXPECT_EQ(
			this->triangleEquilateral.angle2(),
			this->triangleEquilateral[1].angle(this->triangleEquilateral[0], this->triangleEquilateral[2]));

		EXPECT_EQ(
			this->triangleEquilateral.angle3(),
			this->triangleEquilateral[2].angle(this->triangleEquilateral[0], this->triangleEquilateral[1]));

		EXPECT_EQ(
			this->triangleScalene.angle1(),
			this->triangleScalene[0].angle(this->triangleScalene[1], this->triangleScalene[2]));

		EXPECT_EQ(
			this->triangleScalene.angle2(),
			this->triangleScalene[1].angle(this->triangleScalene[0], this->triangleScalene[2]));

		EXPECT_EQ(
			this->triangleScalene.angle3(),
			this->triangleScalene[2].angle(this->triangleScalene[0], this->triangleScalene[1]));
	}

	TYPED_TEST_P(GeometryEuclideanTriangleTest, Area)
	{
		EXPECT_LT(
			std::abs(this->triangleEquilateral.area() - std::sqrt(typename TypeParam::value_type(3)) /
				typename TypeParam::value_type(2)),
			typename TypeParam::value_type(0.00001));

		EXPECT_LT(
			std::abs(this->triangleIsosceles.area() - typename TypeParam::value_type(0.25)),
			typename TypeParam::value_type(0.00001));

		EXPECT_LT(
			std::abs(this->triangleScalene.area() - typename TypeParam::value_type(1)),
			typename TypeParam::value_type(0.00001));
	}

	TYPED_TEST_P(GeometryEuclideanTriangleTest, Perimeter)
	{
		EXPECT_LT(
			std::abs(this->triangleEquilateral.perimeter() - typename TypeParam::value_type(3) * std::sqrt(typename TypeParam::value_type(2))),
			typename TypeParam::value_type(0.00001));

		EXPECT_LT(
			std::abs(this->triangleIsosceles.perimeter() - (typename TypeParam::value_type(1) + std::sqrt(typename TypeParam::value_type(2)))),
			typename TypeParam::value_type(0.00001));

		EXPECT_LT(
			std::abs(this->triangleScalene.perimeter() - (typename TypeParam::value_type(1) + std::sqrt(typename TypeParam::value_type(8)) + std::sqrt(typename TypeParam::value_type(5)))),
			typename TypeParam::value_type(0.00001));
	}

	TYPED_TEST_P(GeometryEuclideanTriangleTest, Incenter)
	{
		constexpr auto oneThird = typename TypeParam::value_type(1) / typename TypeParam::value_type(3);

		this->coordinatesAreAlmostEqual(
			this->triangleEquilateral.incenter().coordinates,
			std::to_array({ oneThird, oneThird, oneThird }));

		this->coordinatesAreAlmostEqual(
			this->triangleIsosceles.incenter().coordinates,
			std::to_array({ typename TypeParam::value_type(0.5), typename TypeParam::value_type(0.207107) }));

		this->coordinatesAreAlmostEqual(
			this->triangleScalene.incenter().coordinates,
			std::to_array({ typename TypeParam::value_type(0.79618), typename TypeParam::value_type(0.329788) }));
	}

	TYPED_TEST_P(GeometryEuclideanTriangleTest, Orthocenter)
	{
		constexpr auto oneThird = typename TypeParam::value_type(1) / typename TypeParam::value_type(3);

		this->coordinatesAreAlmostEqual(
			this->triangleEquilateral.orthocenter().coordinates,
			std::to_array({ oneThird, oneThird, oneThird }));

		this->coordinatesAreAlmostEqual(
			this->triangleIsosceles.orthocenter().coordinates,
			std::to_array({ typename TypeParam::value_type(0.5), typename TypeParam::value_type(0.5) }));

		this->coordinatesAreAlmostEqual(
			this->triangleScalene.orthocenter().coordinates,
			std::to_array({ typename TypeParam::value_type(2), typename TypeParam::value_type(-1) }));
	}

	TYPED_TEST_P(GeometryEuclideanTriangleTest, Barycenter)
	{
		constexpr auto oneThird = typename TypeParam::value_type(1) / typename TypeParam::value_type(3);

		this->coordinatesAreAlmostEqual(
			this->triangleEquilateral.barycenter().coordinates,
			std::to_array({ oneThird, oneThird, oneThird }));

		this->coordinatesAreAlmostEqual(
			this->triangleIsosceles.barycenter().coordinates,
			std::to_array({ typename TypeParam::value_type(0.5), typename TypeParam::value_type(1) / typename TypeParam::value_type(6) }));

		this->coordinatesAreAlmostEqual(
			this->triangleScalene.barycenter().coordinates,
			std::to_array({ typename TypeParam::value_type(1), typename TypeParam::value_type(2) / typename TypeParam::value_type(3) }));
	}

	TYPED_TEST_P(GeometryEuclideanTriangleTest, Circumcenter)
	{
		constexpr auto oneThird = typename TypeParam::value_type(1) / typename TypeParam::value_type(3);

		this->coordinatesAreAlmostEqual(
			this->triangleEquilateral.circumcenter().coordinates,
			std::to_array({ oneThird, oneThird, oneThird }));

		this->coordinatesAreAlmostEqual(
			this->triangleIsosceles.circumcenter().coordinates,
			std::to_array({ typename TypeParam::value_type(0.5), typename TypeParam::value_type(0) }));

		this->coordinatesAreAlmostEqual(
			this->triangleScalene.circumcenter().coordinates,
			std::to_array({ typename TypeParam::value_type(0.5), typename TypeParam::value_type(1.5) }));
	}

	TYPED_TEST_P(GeometryEuclideanTriangleTest, Containment)
	{
		EXPECT_TRUE(this->triangleEquilateral.contains(this->triangleEquilateral.incenter()));
		EXPECT_TRUE(this->triangleEquilateral.contains(this->triangleEquilateral.orthocenter()));
		EXPECT_TRUE(this->triangleEquilateral.contains(this->triangleEquilateral.barycenter()));
		EXPECT_TRUE(this->triangleEquilateral.contains(this->triangleEquilateral.circumcenter()));

		EXPECT_TRUE(this->triangleIsosceles.contains(this->triangleIsosceles.incenter()));
		EXPECT_TRUE(this->triangleIsosceles.contains(this->triangleIsosceles.orthocenter(), 2));
		EXPECT_TRUE(this->triangleIsosceles.contains(this->triangleIsosceles.barycenter()));
		EXPECT_TRUE(this->triangleIsosceles.contains(this->triangleIsosceles.circumcenter()));

		EXPECT_TRUE(this->triangleScalene.contains(this->triangleScalene.incenter(), 2));
		EXPECT_FALSE(this->triangleScalene.contains(this->triangleScalene.orthocenter()));
		EXPECT_TRUE(this->triangleScalene.contains(this->triangleScalene.barycenter()));
		EXPECT_FALSE(this->triangleScalene.contains(this->triangleScalene.circumcenter()));

		EXPECT_FALSE(this->triangleEquilateral.contains(this->vertexOrigin));
		EXPECT_TRUE(this->triangleIsosceles.contains(this->vertexOrigin));
		EXPECT_TRUE(this->triangleScalene.contains(this->vertexOrigin));
	}

	TYPED_TEST_P(GeometryEuclideanTriangleTest, Intersection)
	{
		constexpr auto intersection1 =
			xablau::geometry::make_vertex(std::to_array({
				typename TypeParam::value_type(0.5),
				typename TypeParam::value_type(0.5) }));

		constexpr auto intersection2 =
			xablau::geometry::make_vertex(std::to_array({
				typename TypeParam::value_type(0.49),
				typename TypeParam::value_type(0.5),
				typename TypeParam::value_type(0.01) }));

		constexpr auto intersection3 =
			xablau::geometry::make_vertex(std::to_array({
				typename TypeParam::value_type(1) }));

		constexpr auto segment1 =
			xablau::geometry::make_segment(
				xablau::geometry::make_vertex(std::to_array({
					typename TypeParam::value_type(0.5),
					typename TypeParam::value_type(0.5),
					typename TypeParam::value_type(-1) })),
				xablau::geometry::make_vertex(std::to_array({
					typename TypeParam::value_type(0.5),
					typename TypeParam::value_type(0.5),
					typename TypeParam::value_type(1) })));

		constexpr auto segment2 =
			xablau::geometry::make_segment(
				xablau::geometry::make_vertex(std::to_array({
					typename TypeParam::value_type(0.49),
					typename TypeParam::value_type(0.5),
					typename TypeParam::value_type(-1) })),
				xablau::geometry::make_vertex(std::to_array({
					typename TypeParam::value_type(0.49),
					typename TypeParam::value_type(0.5),
					typename TypeParam::value_type(1) })));

		constexpr auto segment3 =
			xablau::geometry::make_segment(
				xablau::geometry::make_vertex(std::to_array({
					typename TypeParam::value_type(0) })),
				xablau::geometry::make_vertex(std::to_array({
					typename TypeParam::value_type(1) })));

		constexpr auto segment4 =
			xablau::geometry::make_segment(
				xablau::geometry::make_vertex(std::to_array({
					typename TypeParam::value_type(-0.1) })),
				xablau::geometry::make_vertex(std::to_array({
					typename TypeParam::value_type(0.9) })));

		constexpr auto segment5 =
			xablau::geometry::make_segment(
				xablau::geometry::make_vertex(std::to_array({
					typename TypeParam::value_type(-0.9) })),
				xablau::geometry::make_vertex(std::to_array({
					typename TypeParam::value_type(0.1) })));

		EXPECT_EQ(this->triangleEquilateral.intersects < false > (segment1), xablau::geometry::euclidean_triangle_intersection_result::intersects);
		EXPECT_EQ(this->triangleIsosceles.intersects < false > (segment1), xablau::geometry::euclidean_triangle_intersection_result::intersects);
		EXPECT_EQ(this->triangleScalene.intersects < false > (segment1), xablau::geometry::euclidean_triangle_intersection_result::intersects);

		EXPECT_EQ(this->triangleEquilateral.intersects < false > (segment2), xablau::geometry::euclidean_triangle_intersection_result::intersects);
		EXPECT_EQ(this->triangleIsosceles.intersects < false > (segment2), xablau::geometry::euclidean_triangle_intersection_result::does_not_intersect);
		EXPECT_EQ(this->triangleScalene.intersects < false > (segment2), xablau::geometry::euclidean_triangle_intersection_result::does_not_intersect);

		EXPECT_EQ(this->triangleEquilateral.intersects < false > (segment3), xablau::geometry::euclidean_triangle_intersection_result::intersects);
		EXPECT_EQ(this->triangleIsosceles.intersects < false > (segment3), xablau::geometry::euclidean_triangle_intersection_result::coplanar);
		EXPECT_EQ(this->triangleScalene.intersects < false > (segment3), xablau::geometry::euclidean_triangle_intersection_result::coplanar);

		EXPECT_EQ(this->triangleEquilateral.intersects < false > (segment4), xablau::geometry::euclidean_triangle_intersection_result::does_not_intersect);
		EXPECT_EQ(this->triangleIsosceles.intersects < false > (segment4), xablau::geometry::euclidean_triangle_intersection_result::coplanar);
		EXPECT_EQ(this->triangleScalene.intersects < false > (segment4), xablau::geometry::euclidean_triangle_intersection_result::coplanar);

		EXPECT_EQ(this->triangleEquilateral.intersects < false > (segment5), xablau::geometry::euclidean_triangle_intersection_result::does_not_intersect);
		EXPECT_EQ(this->triangleIsosceles.intersects < false > (segment5), xablau::geometry::euclidean_triangle_intersection_result::coplanar);
		EXPECT_EQ(this->triangleScalene.intersects < false > (segment5), xablau::geometry::euclidean_triangle_intersection_result::coplanar);

		EXPECT_EQ(this->triangleEquilateral.intersects < true > (segment1).second.value(), intersection1);
		EXPECT_EQ(this->triangleIsosceles.intersects < true > (segment1).second.value(), intersection1);
		EXPECT_EQ(this->triangleScalene.intersects < true > (segment1).second.value(), intersection1);

		this->coordinatesAreAlmostEqual(
			this->triangleEquilateral.intersects < true > (segment2).second.value().coordinates,
			intersection2.coordinates);
		EXPECT_EQ(this->triangleIsosceles.intersects < true > (segment2).second, std::nullopt);
		EXPECT_EQ(this->triangleScalene.intersects < true > (segment2).second, std::nullopt);

		EXPECT_EQ(this->triangleEquilateral.intersects < true > (segment3).second.value(), intersection3);
		EXPECT_EQ(this->triangleIsosceles.intersects < true > (segment3).second, std::nullopt);
		EXPECT_EQ(this->triangleScalene.intersects < true > (segment3).second, std::nullopt);

		EXPECT_EQ(this->triangleEquilateral.intersects < true > (segment4).second, std::nullopt);
		EXPECT_EQ(this->triangleIsosceles.intersects < true > (segment4).second, std::nullopt);
		EXPECT_EQ(this->triangleScalene.intersects < true > (segment4).second, std::nullopt);

		EXPECT_EQ(this->triangleEquilateral.intersects < true > (segment5).second, std::nullopt);
		EXPECT_EQ(this->triangleIsosceles.intersects < true > (segment5).second, std::nullopt);
		EXPECT_EQ(this->triangleScalene.intersects < true > (segment5).second, std::nullopt);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		GeometryEuclideanTriangleTest,
		ComparisonOperatorLess,
		ComparisonOperatorLessEqualTo,
		ComparisonOperatorEqualTo,
		ComparisonOperatorGreaterEqualTo,
		ComparisonOperatorGreater,
		ComparisonOperatorNotEqualTo,
		AccessAliases,
		Distance,
		Angle,
		Area,
		Perimeter,
		Incenter,
		Orthocenter,
		Barycenter,
		Circumcenter,
		Containment,
		Intersection);

	using GeometryEuclideanTriangleTestDataTypes = std::tuple < float, double, long double >;

	using GeometryEuclideanTriangleTestGroup =
		xablau::testing::test_bundler < GeometryEuclideanTriangleTestDataTypes > ::template test_group < GeometryEuclideanTriangleTypeDefinitions >;

	using GeometryEuclideanTriangleTypes = xablau::testing::tuple_transformation < ::testing::Types, GeometryEuclideanTriangleTestGroup > ::types;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		Xablau,
		GeometryEuclideanTriangleTest,
		GeometryEuclideanTriangleTypes);
}
