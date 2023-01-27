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
	struct GeometryVertexTypeDefinitions
	{
		using value_type = Type;
	};

	template < typename Types >
	class GeometryVertexTest : public ::testing::Test
	{
	public:
		static constexpr xablau::geometry::vertex < typename Types::value_type, xablau::geometry::spatial_dimension < 0 > > vertexNull0{};
		static constexpr xablau::geometry::vertex < typename Types::value_type, xablau::geometry::spatial_dimension < 1 > > vertexNull1{};
		static constexpr xablau::geometry::vertex < typename Types::value_type, xablau::geometry::spatial_dimension < 2 > > vertexNull2{};
		static constexpr xablau::geometry::vertex < typename Types::value_type, xablau::geometry::spatial_dimension < 3 > > vertexNull3{};
		static constexpr xablau::geometry::vertex < typename Types::value_type, xablau::geometry::spatial_dimension < 4 > > vertexNull4{};

		static constexpr auto vertexNegative1 = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(-1) }));
		static constexpr auto vertexNegative2 = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(), typename Types::value_type(-1) }));
		static constexpr auto vertexNegative3 = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(), typename Types::value_type(), typename Types::value_type(-1) }));
		static constexpr auto vertexNegative4 = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(), typename Types::value_type(), typename Types::value_type(), typename Types::value_type(-1) }));

		static constexpr auto vertexPositive1 = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(1) }));
		static constexpr auto vertexPositive2 = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(), typename Types::value_type(1) }));
		static constexpr auto vertexPositive3 = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(), typename Types::value_type(), typename Types::value_type(1) }));
		static constexpr auto vertexPositive4 = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(), typename Types::value_type(), typename Types::value_type(), typename Types::value_type(1) }));

		static constexpr auto vertexFullNegative1 = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(-1) }));
		static constexpr auto vertexFullNegative2 = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(-1), typename Types::value_type(-1) }));
		static constexpr auto vertexFullNegative3 = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(-1), typename Types::value_type(-1), typename Types::value_type(-1) }));
		static constexpr auto vertexFullNegative4 = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(-1), typename Types::value_type(-1), typename Types::value_type(-1), typename Types::value_type(-1) }));

		static constexpr auto vertexFullPositive1 = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(1) }));
		static constexpr auto vertexFullPositive2 = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(1), typename Types::value_type(1) }));
		static constexpr auto vertexFullPositive3 = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(1), typename Types::value_type(1), typename Types::value_type(1) }));
		static constexpr auto vertexFullPositive4 = xablau::geometry::make_vertex(std::to_array({ typename Types::value_type(1), typename Types::value_type(1), typename Types::value_type(1), typename Types::value_type(1) }));

		static void coordinatesAreAlmostEqual(const auto &coordinates1, const auto &coordinates2)
		{
			for (size_t i = 0; i < coordinates1.size(); i++)
			{
				EXPECT_LT(std::abs(coordinates1[i] - coordinates2[i]), typename Types::value_type{0.00001});
			}
		}
	};

	TYPED_TEST_SUITE_P(GeometryVertexTest);

	TYPED_TEST_P(GeometryVertexTest, ComparisonOperatorLess)
	{
		EXPECT_TRUE(this->vertexNegative1 < this->vertexNull1);
		EXPECT_TRUE(this->vertexNegative1 < this->vertexNull2);
		EXPECT_TRUE(this->vertexNegative1 < this->vertexNull3);
		EXPECT_TRUE(this->vertexNegative1 < this->vertexNull4);
		EXPECT_TRUE(this->vertexNegative2 < this->vertexNull2);
		EXPECT_TRUE(this->vertexNegative2 < this->vertexNull3);
		EXPECT_TRUE(this->vertexNegative2 < this->vertexNull4);
		EXPECT_TRUE(this->vertexNegative3 < this->vertexNull3);
		EXPECT_TRUE(this->vertexNegative3 < this->vertexNull4);
		EXPECT_TRUE(this->vertexNegative4 < this->vertexNull4);
		EXPECT_FALSE(this->vertexNegative1 < this->vertexNegative1);
		EXPECT_FALSE(this->vertexNegative2 < this->vertexNegative2);
		EXPECT_FALSE(this->vertexNegative3 < this->vertexNegative3);
		EXPECT_FALSE(this->vertexNegative4 < this->vertexNegative4);

		EXPECT_FALSE(this->vertexPositive1 < this->vertexNull1);
		EXPECT_FALSE(this->vertexPositive1 < this->vertexNull2);
		EXPECT_FALSE(this->vertexPositive1 < this->vertexNull3);
		EXPECT_FALSE(this->vertexPositive1 < this->vertexNull4);
		EXPECT_FALSE(this->vertexPositive2 < this->vertexNull2);
		EXPECT_FALSE(this->vertexPositive2 < this->vertexNull3);
		EXPECT_FALSE(this->vertexPositive2 < this->vertexNull4);
		EXPECT_FALSE(this->vertexPositive3 < this->vertexNull3);
		EXPECT_FALSE(this->vertexPositive3 < this->vertexNull4);
		EXPECT_FALSE(this->vertexPositive4 < this->vertexNull4);
		EXPECT_FALSE(this->vertexPositive1 < this->vertexPositive1);
		EXPECT_FALSE(this->vertexPositive2 < this->vertexPositive2);
		EXPECT_FALSE(this->vertexPositive3 < this->vertexPositive3);
		EXPECT_FALSE(this->vertexPositive4 < this->vertexPositive4);
	}

	TYPED_TEST_P(GeometryVertexTest, ComparisonOperatorLessEqualTo)
	{
		EXPECT_TRUE(this->vertexNegative1 <= this->vertexNull1);
		EXPECT_TRUE(this->vertexNegative1 <= this->vertexNull2);
		EXPECT_TRUE(this->vertexNegative1 <= this->vertexNull3);
		EXPECT_TRUE(this->vertexNegative1 <= this->vertexNull4);
		EXPECT_TRUE(this->vertexNegative2 <= this->vertexNull2);
		EXPECT_TRUE(this->vertexNegative2 <= this->vertexNull3);
		EXPECT_TRUE(this->vertexNegative2 <= this->vertexNull4);
		EXPECT_TRUE(this->vertexNegative3 <= this->vertexNull3);
		EXPECT_TRUE(this->vertexNegative3 <= this->vertexNull4);
		EXPECT_TRUE(this->vertexNegative4 <= this->vertexNull4);
		EXPECT_TRUE(this->vertexNegative1 <= this->vertexNegative1);
		EXPECT_TRUE(this->vertexNegative2 <= this->vertexNegative2);
		EXPECT_TRUE(this->vertexNegative3 <= this->vertexNegative3);
		EXPECT_TRUE(this->vertexNegative4 <= this->vertexNegative4);

		EXPECT_TRUE(this->vertexPositive1 <= this->vertexPositive1);
		EXPECT_TRUE(this->vertexPositive2 <= this->vertexPositive2);
		EXPECT_TRUE(this->vertexPositive3 <= this->vertexPositive3);
		EXPECT_TRUE(this->vertexPositive4 <= this->vertexPositive4);
	}

	TYPED_TEST_P(GeometryVertexTest, ComparisonOperatorEqualTo)
	{
		EXPECT_EQ(this->vertexNull0, this->vertexNull0);
		EXPECT_EQ(this->vertexNull0, this->vertexNull1);
		EXPECT_EQ(this->vertexNull0, this->vertexNull2);
		EXPECT_EQ(this->vertexNull0, this->vertexNull3);
		EXPECT_EQ(this->vertexNull0, this->vertexNull4);
		EXPECT_EQ(this->vertexNull1, this->vertexNull1);
		EXPECT_EQ(this->vertexNull1, this->vertexNull2);
		EXPECT_EQ(this->vertexNull1, this->vertexNull3);
		EXPECT_EQ(this->vertexNull1, this->vertexNull4);
		EXPECT_EQ(this->vertexNull2, this->vertexNull2);
		EXPECT_EQ(this->vertexNull2, this->vertexNull3);
		EXPECT_EQ(this->vertexNull2, this->vertexNull4);
		EXPECT_EQ(this->vertexNull3, this->vertexNull3);
		EXPECT_EQ(this->vertexNull3, this->vertexNull4);
		EXPECT_EQ(this->vertexNull4, this->vertexNull4);
	}

	TYPED_TEST_P(GeometryVertexTest, ComparisonOperatorGreaterEqualTo)
	{
		EXPECT_TRUE(this->vertexNegative1 >= this->vertexNegative1);
		EXPECT_TRUE(this->vertexNegative2 >= this->vertexNegative2);
		EXPECT_TRUE(this->vertexNegative3 >= this->vertexNegative3);
		EXPECT_TRUE(this->vertexNegative4 >= this->vertexNegative4);

		EXPECT_TRUE(this->vertexPositive1 >= this->vertexNull1);
		EXPECT_TRUE(this->vertexPositive1 >= this->vertexNull2);
		EXPECT_TRUE(this->vertexPositive1 >= this->vertexNull3);
		EXPECT_TRUE(this->vertexPositive1 >= this->vertexNull4);
		EXPECT_TRUE(this->vertexPositive2 >= this->vertexNull2);
		EXPECT_TRUE(this->vertexPositive2 >= this->vertexNull3);
		EXPECT_TRUE(this->vertexPositive2 >= this->vertexNull4);
		EXPECT_TRUE(this->vertexPositive3 >= this->vertexNull3);
		EXPECT_TRUE(this->vertexPositive3 >= this->vertexNull4);
		EXPECT_TRUE(this->vertexPositive4 >= this->vertexNull4);
		EXPECT_TRUE(this->vertexPositive1 >= this->vertexPositive1);
		EXPECT_TRUE(this->vertexPositive2 >= this->vertexPositive2);
		EXPECT_TRUE(this->vertexPositive3 >= this->vertexPositive3);
		EXPECT_TRUE(this->vertexPositive4 >= this->vertexPositive4);
	}

	TYPED_TEST_P(GeometryVertexTest, ComparisonOperatorGreater)
	{
		EXPECT_FALSE(this->vertexNegative1 > this->vertexNull1);
		EXPECT_FALSE(this->vertexNegative1 > this->vertexNull2);
		EXPECT_FALSE(this->vertexNegative1 > this->vertexNull3);
		EXPECT_FALSE(this->vertexNegative1 > this->vertexNull4);
		EXPECT_FALSE(this->vertexNegative2 > this->vertexNull2);
		EXPECT_FALSE(this->vertexNegative2 > this->vertexNull3);
		EXPECT_FALSE(this->vertexNegative2 > this->vertexNull4);
		EXPECT_FALSE(this->vertexNegative3 > this->vertexNull3);
		EXPECT_FALSE(this->vertexNegative3 > this->vertexNull4);
		EXPECT_FALSE(this->vertexNegative4 > this->vertexNull4);
		EXPECT_FALSE(this->vertexNegative1 > this->vertexNegative1);
		EXPECT_FALSE(this->vertexNegative2 > this->vertexNegative2);
		EXPECT_FALSE(this->vertexNegative3 > this->vertexNegative3);
		EXPECT_FALSE(this->vertexNegative4 > this->vertexNegative4);

		EXPECT_TRUE(this->vertexPositive1 > this->vertexNull1);
		EXPECT_TRUE(this->vertexPositive1 > this->vertexNull2);
		EXPECT_TRUE(this->vertexPositive1 > this->vertexNull3);
		EXPECT_TRUE(this->vertexPositive1 > this->vertexNull4);
		EXPECT_TRUE(this->vertexPositive2 > this->vertexNull2);
		EXPECT_TRUE(this->vertexPositive2 > this->vertexNull3);
		EXPECT_TRUE(this->vertexPositive2 > this->vertexNull4);
		EXPECT_TRUE(this->vertexPositive3 > this->vertexNull3);
		EXPECT_TRUE(this->vertexPositive3 > this->vertexNull4);
		EXPECT_TRUE(this->vertexPositive4 > this->vertexNull4);
		EXPECT_FALSE(this->vertexPositive1 > this->vertexPositive1);
		EXPECT_FALSE(this->vertexPositive2 > this->vertexPositive2);
		EXPECT_FALSE(this->vertexPositive3 > this->vertexPositive3);
		EXPECT_FALSE(this->vertexPositive4 > this->vertexPositive4);
	}

	TYPED_TEST_P(GeometryVertexTest, ComparisonOperatorNotEqualTo)
	{
		EXPECT_NE(this->vertexPositive1, this->vertexNegative1);
		EXPECT_NE(this->vertexPositive1, this->vertexNegative1);
		EXPECT_NE(this->vertexPositive1, this->vertexNegative1);
		EXPECT_NE(this->vertexPositive1, this->vertexNegative1);
		EXPECT_NE(this->vertexPositive2, this->vertexNegative2);
		EXPECT_NE(this->vertexPositive2, this->vertexNegative2);
		EXPECT_NE(this->vertexPositive2, this->vertexNegative2);
		EXPECT_NE(this->vertexPositive3, this->vertexNegative3);
		EXPECT_NE(this->vertexPositive3, this->vertexNegative3);
		EXPECT_NE(this->vertexPositive4, this->vertexNegative4);
	}

	TYPED_TEST_P(GeometryVertexTest, AccessAliases)
	{
		EXPECT_EQ(this->vertexNull0.x(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vertexNull0.y(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vertexNull0.z(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vertexNull0.w(), typename TypeParam::value_type{});

		EXPECT_EQ(this->vertexNegative1.x(), typename TypeParam::value_type{-1});
		EXPECT_EQ(this->vertexNegative1.y(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vertexNegative1.z(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vertexNegative1.w(), typename TypeParam::value_type{});

		EXPECT_EQ(this->vertexPositive1.x(), typename TypeParam::value_type{1});
		EXPECT_EQ(this->vertexPositive1.y(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vertexPositive1.z(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vertexPositive1.w(), typename TypeParam::value_type{});

		EXPECT_EQ(this->vertexNegative2.x(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vertexNegative2.y(), typename TypeParam::value_type{-1});
		EXPECT_EQ(this->vertexNegative2.z(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vertexNegative2.w(), typename TypeParam::value_type{});

		EXPECT_EQ(this->vertexPositive2.x(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vertexPositive2.y(), typename TypeParam::value_type{1});
		EXPECT_EQ(this->vertexPositive2.z(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vertexPositive2.w(), typename TypeParam::value_type{});

		EXPECT_EQ(this->vertexNegative3.x(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vertexNegative3.y(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vertexNegative3.z(), typename TypeParam::value_type{-1});
		EXPECT_EQ(this->vertexNegative3.w(), typename TypeParam::value_type{});

		EXPECT_EQ(this->vertexPositive3.x(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vertexPositive3.y(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vertexPositive3.z(), typename TypeParam::value_type{1});
		EXPECT_EQ(this->vertexPositive3.w(), typename TypeParam::value_type{});

		EXPECT_EQ(this->vertexNegative4.x(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vertexNegative4.y(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vertexNegative4.z(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vertexNegative4.w(), typename TypeParam::value_type{-1});

		EXPECT_EQ(this->vertexPositive4.x(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vertexPositive4.y(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vertexPositive4.z(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vertexPositive4.w(), typename TypeParam::value_type{1});
	}

	TYPED_TEST_P(GeometryVertexTest, ContainmentSegment)
	{
		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > vertexOutOfRange{
			std::to_array(
				{
					typename TypeParam::value_type(0.5),
					typename TypeParam::value_type(0.5),
					typename TypeParam::value_type(0.5)
				})};

		constexpr xablau::geometry::segment < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 1 > > segmentDirectX{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type(-1)
				})),
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type(1)
				}))};

		constexpr xablau::geometry::segment < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 1 > > segmentReverseX{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type(1)
				})),
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type(-1)
				}))};

		constexpr xablau::geometry::segment < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > segmentDirectY{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type(0),
					typename TypeParam::value_type(-1)
				})),
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type(0),
					typename TypeParam::value_type(1)
				}))};

		constexpr xablau::geometry::segment < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > segmentReverseY{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type(0),
					typename TypeParam::value_type(1)
				})),
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type(0),
					typename TypeParam::value_type(-1)
				}))};

		constexpr xablau::geometry::segment < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > segmentDirectZ{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type(0),
					typename TypeParam::value_type(0),
					typename TypeParam::value_type(-1)
				})),
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type(0),
					typename TypeParam::value_type(0),
					typename TypeParam::value_type(1)
				}))};

		constexpr xablau::geometry::segment < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > segmentReverseZ{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type(0),
					typename TypeParam::value_type(0),
					typename TypeParam::value_type(1)
				})),
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type(0),
					typename TypeParam::value_type(0),
					typename TypeParam::value_type(-1)
				}))};

		EXPECT_TRUE(segmentDirectX.vertex1.contained_in(segmentDirectX));
		EXPECT_TRUE(segmentDirectX.vertex2.contained_in(segmentDirectX));
		EXPECT_TRUE(this->vertexNull4.contained_in(segmentDirectX));

		EXPECT_TRUE(segmentReverseZ.vertex1.contained_in(segmentReverseZ));
		EXPECT_TRUE(segmentReverseZ.vertex2.contained_in(segmentReverseZ));
		EXPECT_TRUE(this->vertexNull4.contained_in(segmentReverseZ));

		EXPECT_FALSE(vertexOutOfRange.contained_in(segmentDirectX));
		EXPECT_FALSE(vertexOutOfRange.contained_in(segmentReverseX));
		EXPECT_FALSE(vertexOutOfRange.contained_in(segmentDirectY));
		EXPECT_FALSE(vertexOutOfRange.contained_in(segmentReverseY));
		EXPECT_FALSE(vertexOutOfRange.contained_in(segmentDirectZ));
		EXPECT_FALSE(vertexOutOfRange.contained_in(segmentReverseZ));
	}

	TYPED_TEST_P(GeometryVertexTest, ContainmentLine)
	{
		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 1 > > lineDirectX{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(1)
				}))};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 1 > > lineReverseX{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(-1)
				}))};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > lineDirectY{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(),
					typename TypeParam::value_type(1)
				}))};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > lineReverseY{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(),
					typename TypeParam::value_type(-1)
				}))};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > lineDirectZ{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(),
					typename TypeParam::value_type(),
					typename TypeParam::value_type(1)
				}))};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > lineReverseZ{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(),
					typename TypeParam::value_type(),
					typename TypeParam::value_type(-1)
				}))};

		EXPECT_TRUE(lineDirectX.contains(this->vertexNull4));
		EXPECT_TRUE(lineReverseX.contains(this->vertexNull4));
		EXPECT_TRUE(lineDirectY.contains(this->vertexNull4));
		EXPECT_TRUE(lineReverseY.contains(this->vertexNull4));
		EXPECT_TRUE(lineDirectZ.contains(this->vertexNull4));
		EXPECT_TRUE(lineReverseZ.contains(this->vertexNull4));

		EXPECT_FALSE(lineDirectX.contains(this->vertexFullPositive3));
		EXPECT_FALSE(lineReverseX.contains(this->vertexFullPositive3));
		EXPECT_FALSE(lineDirectY.contains(this->vertexFullPositive3));
		EXPECT_FALSE(lineReverseY.contains(this->vertexFullPositive3));
		EXPECT_FALSE(lineDirectZ.contains(this->vertexFullPositive3));
		EXPECT_FALSE(lineReverseZ.contains(this->vertexFullPositive3));
	}

	TYPED_TEST_P(GeometryVertexTest, ContainmentHyperplane)
	{
		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 1 > > hyperplaneDirectX{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(1)
				}))};

		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 1 > > hyperplaneReverseX{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(-1)
				}))};

		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > hyperplaneDirectY{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(),
					typename TypeParam::value_type(1)
				}))};

		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > hyperplaneReverseY{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(),
					typename TypeParam::value_type(-1)
				}))};

		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > hyperplaneDirectZ{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(),
					typename TypeParam::value_type(),
					typename TypeParam::value_type(1)
				}))};

		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > hyperplaneReverseZ{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(),
					typename TypeParam::value_type(),
					typename TypeParam::value_type(-1)
				}))};

		EXPECT_TRUE(this->vertexNull3.contained_in(hyperplaneDirectX));
		EXPECT_TRUE(this->vertexNull3.contained_in(hyperplaneReverseX));
		EXPECT_TRUE(this->vertexNull3.contained_in(hyperplaneDirectY));
		EXPECT_TRUE(this->vertexNull3.contained_in(hyperplaneReverseY));
		EXPECT_TRUE(this->vertexNull3.contained_in(hyperplaneDirectZ));
		EXPECT_TRUE(this->vertexNull3.contained_in(hyperplaneReverseZ));

		EXPECT_FALSE(hyperplaneDirectX.contains(this->vertexPositive1));
		EXPECT_FALSE(hyperplaneReverseX.contains(this->vertexPositive1));
		EXPECT_FALSE(hyperplaneDirectY.contains(this->vertexPositive2));
		EXPECT_FALSE(hyperplaneReverseY.contains(this->vertexPositive2));
		EXPECT_FALSE(hyperplaneDirectZ.contains(this->vertexPositive3));
		EXPECT_FALSE(hyperplaneReverseZ.contains(this->vertexPositive3));
	}

	TYPED_TEST_P(GeometryVertexTest, ContainmentHypersphere)
	{
		constexpr xablau::geometry::hypersphere < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > hypersphere{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type(0),
					typename TypeParam::value_type(0)
				})),
			typename TypeParam::value_type(1)};

		EXPECT_EQ(this->vertexNull2.contained_in < false > (hypersphere), xablau::geometry::hypersphere_side::inside);

		EXPECT_EQ(this->vertexPositive1.contained_in < false > (hypersphere), xablau::geometry::hypersphere_side::on_surface);
		EXPECT_EQ(this->vertexPositive2.contained_in < false > (hypersphere), xablau::geometry::hypersphere_side::on_surface);
		EXPECT_EQ(this->vertexNegative1.contained_in < false > (hypersphere), xablau::geometry::hypersphere_side::on_surface);
		EXPECT_EQ(this->vertexNegative2.contained_in < false > (hypersphere), xablau::geometry::hypersphere_side::on_surface);

		EXPECT_EQ(this->vertexFullPositive2.contained_in < false > (hypersphere), xablau::geometry::hypersphere_side::outside);
		EXPECT_EQ(this->vertexFullPositive3.contained_in < false > (hypersphere), xablau::geometry::hypersphere_side::outside);
		EXPECT_EQ(this->vertexFullNegative2.contained_in < false > (hypersphere), xablau::geometry::hypersphere_side::outside);
		EXPECT_EQ(this->vertexFullNegative3.contained_in < false > (hypersphere), xablau::geometry::hypersphere_side::outside);

		EXPECT_EQ(this->vertexNull2.contained_in < true > (hypersphere).second, typename TypeParam::value_type(0));

		EXPECT_EQ(this->vertexPositive1.contained_in < true > (hypersphere).second, typename TypeParam::value_type(1));
		EXPECT_EQ(this->vertexPositive2.contained_in < true > (hypersphere).second, typename TypeParam::value_type(1));
		EXPECT_EQ(this->vertexNegative1.contained_in < true > (hypersphere).second, typename TypeParam::value_type(1));
		EXPECT_EQ(this->vertexNegative2.contained_in < true > (hypersphere).second, typename TypeParam::value_type(1));

		EXPECT_EQ(this->vertexFullPositive2.contained_in < true > (hypersphere).second, std::sqrt(typename TypeParam::value_type(2)));
		EXPECT_EQ(this->vertexFullPositive3.contained_in < true > (hypersphere).second, std::sqrt(typename TypeParam::value_type(3)));
		EXPECT_EQ(this->vertexFullNegative2.contained_in < true > (hypersphere).second, std::sqrt(typename TypeParam::value_type(2)));
		EXPECT_EQ(this->vertexFullNegative3.contained_in < true > (hypersphere).second, std::sqrt(typename TypeParam::value_type(3)));
	}

	TYPED_TEST_P(GeometryVertexTest, DistanceVertice)
	{
		EXPECT_EQ(this->vertexNull0.distance_to_origin(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vertexPositive1.distance_to_origin(), typename TypeParam::value_type{1});
		EXPECT_EQ(this->vertexPositive2.distance_to_origin(), typename TypeParam::value_type{1});
		EXPECT_EQ(this->vertexPositive3.distance_to_origin(), typename TypeParam::value_type{1});
		EXPECT_EQ(this->vertexPositive4.distance_to_origin(), typename TypeParam::value_type{1});
		EXPECT_EQ(this->vertexNegative1.distance_to_origin(), typename TypeParam::value_type{1});
		EXPECT_EQ(this->vertexNegative2.distance_to_origin(), typename TypeParam::value_type{1});
		EXPECT_EQ(this->vertexNegative3.distance_to_origin(), typename TypeParam::value_type{1});
		EXPECT_EQ(this->vertexNegative4.distance_to_origin(), typename TypeParam::value_type{1});

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexFullPositive1.distance_to(this->vertexFullNegative1),
				static_cast < typename TypeParam::value_type > (2)));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexFullPositive2.distance_to(this->vertexFullNegative2),
				static_cast < typename TypeParam::value_type > (2 * std::sqrt(2))));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexFullPositive3.distance_to(this->vertexFullNegative3),
				static_cast < typename TypeParam::value_type > (2 * std::sqrt(3))));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexFullPositive4.distance_to(this->vertexFullNegative4),
				static_cast < typename TypeParam::value_type > (2 * std::sqrt(4))));

		constexpr auto orders =
			std::to_array(
				{
					typename TypeParam::value_type{-100},
					typename TypeParam::value_type{-50},
					typename TypeParam::value_type{-10},
					typename TypeParam::value_type{-5},
					typename TypeParam::value_type{-3},
					typename TypeParam::value_type{-2},
					typename TypeParam::value_type{-1},
					typename TypeParam::value_type{-0.5},
					typename TypeParam::value_type{-0.25},
					typename TypeParam::value_type{},
					typename TypeParam::value_type{0.25},
					typename TypeParam::value_type{0.5},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1.5},
					typename TypeParam::value_type{2},
					typename TypeParam::value_type{3},
					typename TypeParam::value_type{4},
					typename TypeParam::value_type{5},
					typename TypeParam::value_type{10},
					typename TypeParam::value_type{50},
					typename TypeParam::value_type{100}
				});

		constexpr auto result =
			std::to_array(
				{
					typename TypeParam::value_type{1.9724654089867184},
					typename TypeParam::value_type{1.9453098948245711},
					typename TypeParam::value_type{1.7411011265922482},
					typename TypeParam::value_type{1.5157165665103982},
					typename TypeParam::value_type{1.2599210498948732},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{0.5},
					typename TypeParam::value_type{0.1250},
					typename TypeParam::value_type{0.0078125},
					typename TypeParam::value_type{std::numeric_limits < typename TypeParam::value_type > ::infinity()},
					typename TypeParam::value_type{512},
					typename TypeParam::value_type{32},
					typename TypeParam::value_type{8},
					typename TypeParam::value_type{5.0396841995794928},
					typename TypeParam::value_type{4},
					typename TypeParam::value_type{3.1748021039363987},
					typename TypeParam::value_type{2.8284271247461903},
					typename TypeParam::value_type{2.6390158215457888},
					typename TypeParam::value_type{2.2973967099940702},
					typename TypeParam::value_type{2.0562276533121331},
					typename TypeParam::value_type{2.0279189595800582}
				});

		for (size_t i = 0; i < orders.size(); i++)
		{
			if (orders[i] == 0)
			{
				EXPECT_EQ(
					this->vertexFullPositive4.Minkowski_distance_to(
						this->vertexFullNegative4,
						orders[i]),
					result[i]);
			}

			else
			{
				EXPECT_TRUE(
					xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (
						this->vertexFullPositive4.Minkowski_distance_to(
							this->vertexFullNegative4,
							orders[i]),
						result[i]));
			}
		}
	}

	TYPED_TEST_P(GeometryVertexTest, DistanceLine)
	{
		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 1 > > lineDirectX{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(1)
				}))};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 1 > > lineReverseX{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(-1)
				}))};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > lineDirectY{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(),
					typename TypeParam::value_type(1)
				}))};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > lineReverseY{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(),
					typename TypeParam::value_type(-1)
				}))};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > lineDirectZ{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(),
					typename TypeParam::value_type(),
					typename TypeParam::value_type(1)
				}))};

		constexpr xablau::geometry::line < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > lineReverseZ{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(),
					typename TypeParam::value_type(),
					typename TypeParam::value_type(-1)
				}))};

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
			this->vertexNull3.distance_to < false > (lineDirectX),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->vertexNull3.distance_to < false > (lineReverseX),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->vertexNull3.distance_to < false > (lineDirectY),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->vertexNull3.distance_to < false > (lineReverseY),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->vertexNull3.distance_to < false > (lineDirectZ),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->vertexNull3.distance_to < false > (lineReverseZ),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->vertexFullPositive3.distance_to < false > (lineDirectX),
			std::sqrt(typename TypeParam::value_type{2}));

		EXPECT_EQ(
			this->vertexFullPositive3.distance_to < false > (lineReverseX),
			std::sqrt(typename TypeParam::value_type{2}));

		EXPECT_EQ(
			this->vertexFullPositive3.distance_to < false > (lineDirectY),
			std::sqrt(typename TypeParam::value_type{2}));

		EXPECT_EQ(
			this->vertexFullPositive3.distance_to < false > (lineReverseY),
			std::sqrt(typename TypeParam::value_type{2}));

		EXPECT_EQ(
			this->vertexFullPositive3.distance_to < false > (lineDirectZ),
			std::sqrt(typename TypeParam::value_type{2}));

		EXPECT_EQ(
			this->vertexFullPositive3.distance_to < false > (lineReverseZ),
			std::sqrt(typename TypeParam::value_type{2}));

		EXPECT_EQ(
			this->vertexNull3.distance_to < true > (lineDirectX).second,
			this->vertexNull3);

		EXPECT_EQ(
			this->vertexNull3.distance_to < true > (lineReverseX).second,
			this->vertexNull3);

		EXPECT_EQ(
			this->vertexNull3.distance_to < true > (lineDirectY).second,
			this->vertexNull3);

		EXPECT_EQ(
			this->vertexNull3.distance_to < true > (lineReverseY).second,
			this->vertexNull3);

		EXPECT_EQ(
			this->vertexNull3.distance_to < true > (lineDirectZ).second,
			this->vertexNull3);

		EXPECT_EQ(
			this->vertexNull3.distance_to < true > (lineReverseZ).second,
			this->vertexNull3);

		EXPECT_EQ(
			this->vertexFullPositive3.distance_to < true > (lineDirectX).second,
			this->vertexPositive1);

		EXPECT_EQ(
			this->vertexFullPositive3.distance_to < true > (lineReverseX).second,
			this->vertexPositive1);

		EXPECT_EQ(
			this->vertexFullPositive3.distance_to < true > (lineDirectY).second,
			this->vertexPositive2);

		EXPECT_EQ(
			this->vertexFullPositive3.distance_to < true > (lineReverseY).second,
			this->vertexPositive2);

		EXPECT_EQ(
			this->vertexFullPositive3.distance_to < true > (lineDirectZ).second,
			this->vertexPositive3);

		EXPECT_EQ(
			this->vertexFullPositive3.distance_to < true > (lineReverseZ).second,
			this->vertexPositive3);
	}

	TYPED_TEST_P(GeometryVertexTest, DistanceHyperplane)
	{
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

		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 1 > > hyperplaneDirectX{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(1)
				}))};

		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 1 > > hyperplaneReverseX{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(-1)
				}))};

		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > hyperplaneDirectY{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(),
					typename TypeParam::value_type(1)
				}))};

		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > hyperplaneReverseY{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(),
					typename TypeParam::value_type(-1)
				}))};

		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > hyperplaneDirectZ{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(),
					typename TypeParam::value_type(),
					typename TypeParam::value_type(1)
				}))};

		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > hyperplaneReverseZ{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(),
					typename TypeParam::value_type(),
					typename TypeParam::value_type(-1)
				}))};

		EXPECT_EQ(
			this->vertexNull3.distance_to < false > (hyperplaneDirectX),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->vertexNull3.distance_to < false > (hyperplaneReverseX),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->vertexNull3.distance_to < false > (hyperplaneDirectY),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->vertexNull3.distance_to < false > (hyperplaneReverseY),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->vertexNull3.distance_to < false > (hyperplaneDirectZ),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->vertexNull3.distance_to < false > (hyperplaneReverseZ),
			typename TypeParam::value_type{});

		EXPECT_EQ(
			this->vertexFullPositive3.distance_to < false > (hyperplaneDirectX),
			typename TypeParam::value_type{1});

		EXPECT_EQ(
			this->vertexFullPositive3.distance_to < false > (hyperplaneReverseX),
			typename TypeParam::value_type{1});

		EXPECT_EQ(
			this->vertexFullPositive3.distance_to < false > (hyperplaneDirectY),
			typename TypeParam::value_type{1});

		EXPECT_EQ(
			this->vertexFullPositive3.distance_to < false > (hyperplaneReverseY),
			typename TypeParam::value_type{1});

		EXPECT_EQ(
			this->vertexFullPositive3.distance_to < false > (hyperplaneDirectZ),
			typename TypeParam::value_type{1});

		EXPECT_EQ(
			this->vertexFullPositive3.distance_to < false > (hyperplaneReverseZ),
			typename TypeParam::value_type{1});

		EXPECT_EQ(
			this->vertexNull3.distance_to < true > (hyperplaneDirectX).second,
			this->vertexNull3);

		EXPECT_EQ(
			this->vertexNull3.distance_to < true > (hyperplaneReverseX).second,
			this->vertexNull3);

		EXPECT_EQ(
			this->vertexNull3.distance_to < true > (hyperplaneDirectY).second,
			this->vertexNull3);

		EXPECT_EQ(
			this->vertexNull3.distance_to < true > (hyperplaneReverseY).second,
			this->vertexNull3);

		EXPECT_EQ(
			this->vertexNull3.distance_to < true > (hyperplaneDirectZ).second,
			this->vertexNull3);

		EXPECT_EQ(
			this->vertexNull3.distance_to < true > (hyperplaneReverseZ).second,
			this->vertexNull3);

		EXPECT_EQ(
			this->vertexFullPositive3.distance_to < true > (hyperplaneDirectX).second,
			vertexX);

		EXPECT_EQ(
			this->vertexFullPositive3.distance_to < true > (hyperplaneReverseX).second,
			vertexX);

		EXPECT_EQ(
			this->vertexFullPositive3.distance_to < true > (hyperplaneDirectY).second,
			vertexY);

		EXPECT_EQ(
			this->vertexFullPositive3.distance_to < true > (hyperplaneReverseY).second,
			vertexY);

		EXPECT_EQ(
			this->vertexFullPositive3.distance_to < true > (hyperplaneDirectZ).second,
			vertexZ);

		EXPECT_EQ(
			this->vertexFullPositive3.distance_to < true > (hyperplaneReverseZ).second,
			vertexZ);
	}

	TYPED_TEST_P(GeometryVertexTest, Scaling)
	{
		xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 4 > > vertexPower =
			std::to_array(
				{
					typename TypeParam::value_type(1),
					typename TypeParam::value_type(2),
					typename TypeParam::value_type(4),
					typename TypeParam::value_type(8)
				});

		EXPECT_EQ(
			vertexPower.scale_by(typename TypeParam::value_type{2}).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{2},
					typename TypeParam::value_type{4},
					typename TypeParam::value_type{8},
					typename TypeParam::value_type{16}
				}));

		EXPECT_EQ(
			vertexPower.scale_by(typename TypeParam::value_type{0.5}).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{2},
					typename TypeParam::value_type{4},
					typename TypeParam::value_type{8}
				}));

		EXPECT_EQ(
			vertexPower.scale_by(
				std::to_array(
					{
						typename TypeParam::value_type{2},
						typename TypeParam::value_type{2},
						typename TypeParam::value_type{2},
						typename TypeParam::value_type{2}
					})).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{2},
					typename TypeParam::value_type{4},
					typename TypeParam::value_type{8},
					typename TypeParam::value_type{16}
				}));

		EXPECT_EQ(
			vertexPower.scale_by(
				std::to_array(
					{
						static_cast < typename TypeParam::value_type > (0.5),
						static_cast < typename TypeParam::value_type > (0.25),
						static_cast < typename TypeParam::value_type > (0.125),
						static_cast < typename TypeParam::value_type > (0.0625)
					})).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				}));

		EXPECT_EQ(
			vertexPower.scale_by(
				std::to_array(
					{
						static_cast < typename TypeParam::value_type > (1),
						static_cast < typename TypeParam::value_type > (1)
					})).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{0}
				}));

		EXPECT_EQ(
			vertexPower.scale_by(
				std::to_array(
					{
						static_cast < typename TypeParam::value_type > (1),
						static_cast < typename TypeParam::value_type > (0),
						static_cast < typename TypeParam::value_type > (1),
						static_cast < typename TypeParam::value_type > (1),
						static_cast < typename TypeParam::value_type > (1)
					})).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{0}
				}));
	}

	TYPED_TEST_P(GeometryVertexTest, VectorCreation)
	{
		EXPECT_EQ(
			this->vertexFullPositive4.coordinates,
			this->vertexFullPositive4.vector_from_origin().coordinates);

		EXPECT_EQ(
			this->vertexFullNegative4.coordinates,
			this->vertexFullNegative4.vector_from_origin().coordinates);

		EXPECT_EQ(
			(this->vertexFullPositive4 - this->vertexFullNegative4).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{2},
					typename TypeParam::value_type{2},
					typename TypeParam::value_type{2},
					typename TypeParam::value_type{2}
				}));

		EXPECT_EQ(
			(this->vertexFullNegative4 - this->vertexFullPositive4).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{-2},
					typename TypeParam::value_type{-2},
					typename TypeParam::value_type{-2},
					typename TypeParam::value_type{-2}
				}));
	}

	TYPED_TEST_P(GeometryVertexTest, VectorProduct)
	{
		xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > vectorProductLeft =
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{3},
					typename TypeParam::value_type{4}
				});

		xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > vectorProductRight =
			std::to_array(
				{
					typename TypeParam::value_type{2},
					typename TypeParam::value_type{7},
					typename TypeParam::value_type{-5}
				});

		EXPECT_EQ(
			this->vertexNull0.template vector_product < xablau::geometry::vector_product_dimension::three > (
				vectorProductLeft,
				vectorProductRight).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{-43},
					typename TypeParam::value_type{13},
					typename TypeParam::value_type{1}
				}));
	}

	TYPED_TEST_P(GeometryVertexTest, AngleWithoutSign)
	{
		const typename TypeParam::value_type value1 = typename TypeParam::value_type{0.5};
		const typename TypeParam::value_type value2 = std::sqrt(typename TypeParam::value_type{2}) / typename TypeParam::value_type{2};
		const typename TypeParam::value_type value3 = std::sqrt(typename TypeParam::value_type{3}) / typename TypeParam::value_type{2};

		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex0degree =
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{0}
				});

		const xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex30degree(
			std::to_array(
				{
					value3,
					value1
				}));

		const xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex45degree =
			std::to_array(
				{
					value2,
					value2
				});

		const xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex60degree =
			std::to_array(
				{
					value1,
					value3
				});

		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex90degree =
			std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				});

		const xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex120degree =
			std::to_array(
				{
					-value1,
					value3
				});

		const xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex135degree =
			std::to_array(
				{
					-value2,
					value2
				});

		const xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex150degree =
			std::to_array(
				{
					-value3,
					value1
				});

		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex180degree =
			std::to_array(
				{
					typename TypeParam::value_type{-1},
					typename TypeParam::value_type{0}
				});

		const xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex210degree =
			std::to_array(
				{
					-value3,
					-value1
				});

		const xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex225degree =
			std::to_array(
				{
					-value2,
					-value2
				});

		const xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex240degree =
			std::to_array(
				{
					-value1,
					-value3
				});

		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex270degree =
			std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{-1}
				});

		const xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex300degree =
			std::to_array(
				{
					value1,
					-value3
				});

		const xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex315degree =
			std::to_array(
				{
					value2,
					-value2
				});

		const xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex330degree =
			std::to_array(
				{
					value3,
					-value1
				});

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexNull0.angle(vertex0degree, vertex30degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{6}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexNull0.angle(vertex0degree, vertex45degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{4}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexNull0.angle(vertex0degree, vertex60degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{3}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexNull0.angle(vertex0degree, vertex90degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexNull0.angle(vertex0degree, vertex120degree),
				typename TypeParam::value_type{2} * std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{3}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexNull0.angle(vertex0degree, vertex135degree),
				typename TypeParam::value_type{3} * std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{4}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexNull0.angle(vertex0degree, vertex150degree),
				typename TypeParam::value_type{5} * std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{6}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexNull0.angle(vertex0degree, vertex180degree),
				std::numbers::pi_v < typename TypeParam::value_type >));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexNull0.angle(vertex0degree, vertex210degree),
				typename TypeParam::value_type{5} * std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{6}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexNull0.angle(vertex0degree, vertex225degree),
				typename TypeParam::value_type{3} * std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{4}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexNull0.angle(vertex0degree, vertex240degree),
				typename TypeParam::value_type{2} * std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{3}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexNull0.angle(vertex0degree, vertex270degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexNull0.angle(vertex0degree, vertex300degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{3}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexNull0.angle(vertex0degree, vertex315degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{4}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexNull0.angle(vertex0degree, vertex330degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{6}));
	}

	TYPED_TEST_P(GeometryVertexTest, AngleWithSign)
	{
		const typename TypeParam::value_type value1 = typename TypeParam::value_type{0.5};
		const typename TypeParam::value_type value2 = std::sqrt(typename TypeParam::value_type{2}) / typename TypeParam::value_type{2};
		const typename TypeParam::value_type value3 = std::sqrt(typename TypeParam::value_type{3}) / typename TypeParam::value_type{2};

		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > orientation =
			std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				});

		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex0degree =
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{0}
				});

		const xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex30degree =
			std::to_array(
				{
					value3,
					value1
				});

		const xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex45degree =
			std::to_array(
				{
					value2,
					value2
				});

		const xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex60degree =
			std::to_array(
				{
					value1,
					value3
				});

		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex90degree =
			std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				});

		const xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex120degree =
			std::to_array(
				{
					-value1,
					value3
				});

		const xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex135degree =
			std::to_array(
				{
					-value2,
					value2
				});

		const xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex150degree =
			std::to_array(
				{
					-value3,
					value1
				});

		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex180degree =
			std::to_array(
				{
					typename TypeParam::value_type{-1},
					typename TypeParam::value_type{0}
				});

		const xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex210degree =
			std::to_array(
				{
					-value3,
					-value1
				});

		const xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex225degree =
			std::to_array(
				{
					-value2,
					-value2
				});

		const xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex240degree =
			std::to_array(
				{
					-value1,
					-value3
				});

		constexpr xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex270degree =
			std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{-1}
				});

		const xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex300degree =
			std::to_array(
				{
					value1,
					-value3
				});

		const xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex315degree =
			std::to_array(
				{
					value2,
					-value2
				});

		const xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex330degree =
			std::to_array(
				{
					value3,
					-value1
				});

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexNull0.angle_with_sign(vertex0degree, vertex30degree, orientation),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{6}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexNull0.angle_with_sign(vertex0degree, vertex45degree, orientation),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{4}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexNull0.angle_with_sign(vertex0degree, vertex60degree, orientation),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{3}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexNull0.angle_with_sign(vertex0degree, vertex90degree, orientation),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexNull0.angle_with_sign(vertex0degree, vertex120degree, orientation),
				typename TypeParam::value_type{2} * std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{3}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexNull0.angle_with_sign(vertex0degree, vertex135degree, orientation),
				typename TypeParam::value_type{3} * std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{4}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexNull0.angle_with_sign(vertex0degree, vertex150degree, orientation),
				typename TypeParam::value_type{5} * std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{6}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexNull0.angle_with_sign(vertex0degree, vertex180degree, orientation),
				std::numbers::pi_v < typename TypeParam::value_type >));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexNull0.angle_with_sign(vertex0degree, vertex210degree, orientation),
				-(typename TypeParam::value_type{5} * std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{6})));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexNull0.angle_with_sign(vertex0degree, vertex225degree, orientation),
				-(typename TypeParam::value_type{3} * std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{4})));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexNull0.angle_with_sign(vertex0degree, vertex240degree, orientation),
				-(typename TypeParam::value_type{2} * std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{3})));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexNull0.angle_with_sign(vertex0degree, vertex270degree, orientation),
				-(std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2})));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexNull0.angle_with_sign(vertex0degree, vertex300degree, orientation),
				-(std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{3})));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexNull0.angle_with_sign(vertex0degree, vertex315degree, orientation),
				-(std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{4})));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				this->vertexNull0.angle_with_sign(vertex0degree, vertex330degree, orientation),
				-(std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{6})));
	}

	TYPED_TEST_P(GeometryVertexTest, Translation)
	{
		constexpr auto vector2 = this->vertexPositive2 - this->vertexNull0;
		constexpr auto vector3 = this->vertexPositive3 - this->vertexNull0;
		constexpr auto vector4 = this->vertexPositive4 - this->vertexNull0;

		EXPECT_EQ(this->vertexPositive1 + vector2 + vector3 + vector4, this->vertexFullPositive4);
		EXPECT_EQ(this->vertexFullPositive4 - vector2 - vector3 - vector4, this->vertexPositive1);
	}

	TYPED_TEST_P(GeometryVertexTest, Projection)
	{
		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 4 > > vectorNull{};

		constexpr auto vectorDiagonal =
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(1),
					typename TypeParam::value_type(1),
					typename TypeParam::value_type(1),
					typename TypeParam::value_type(1)
				}));

		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 1 > > hyperplaneDirectX{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(1)
				}))};

		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 1 > > hyperplaneReverseX{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(-1)
				}))};

		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > hyperplaneDirectY{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(),
					typename TypeParam::value_type(1)
				}))};

		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > hyperplaneReverseY{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(),
					typename TypeParam::value_type(-1)
				}))};

		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > hyperplaneDirectZ{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(),
					typename TypeParam::value_type(),
					typename TypeParam::value_type(1)
				}))};

		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > hyperplaneReverseZ{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type()
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(),
					typename TypeParam::value_type(),
					typename TypeParam::value_type(-1)
				}))};

		constexpr xablau::geometry::hyperplane < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 1 > > hyperplaneDirectXWithOffset{
			xablau::geometry::make_vertex(std::to_array(
				{
					typename TypeParam::value_type(-1)
				})),
			xablau::geometry::make_vector(std::to_array(
				{
					typename TypeParam::value_type(1)
				}))};

		EXPECT_EQ(
			this->vertexNull4.project_on(hyperplaneDirectX, vectorDiagonal),
			this->vertexNull4);

		EXPECT_EQ(
			this->vertexNull4.project_on(hyperplaneDirectX, vectorNull),
			this->vertexNull4);

		EXPECT_EQ(
			this->vertexFullPositive4.project_on(hyperplaneDirectX, vectorNull),
			std::nullopt);

		EXPECT_EQ(
			this->vertexFullPositive4.project_on(hyperplaneDirectX, vectorDiagonal),
			std::nullopt);

		EXPECT_EQ(
			this->vertexFullPositive4.project_on(hyperplaneReverseX, vectorDiagonal),
			std::nullopt);

		EXPECT_EQ(
			this->vertexFullPositive4.project_on(hyperplaneDirectX, -vectorDiagonal).value(),
			this->vertexNull4);

		EXPECT_EQ(
			this->vertexFullPositive4.project_on(hyperplaneReverseX, -vectorDiagonal).value(),
			this->vertexNull4);

		EXPECT_EQ(
			this->vertexFullPositive4.project_on(hyperplaneDirectY, vectorDiagonal),
			std::nullopt);

		EXPECT_EQ(
			this->vertexFullPositive4.project_on(hyperplaneReverseY, vectorDiagonal),
			std::nullopt);

		EXPECT_EQ(
			this->vertexFullPositive4.project_on(hyperplaneDirectY, -vectorDiagonal).value(),
			this->vertexNull4);

		EXPECT_EQ(
			this->vertexFullPositive4.project_on(hyperplaneReverseY, -vectorDiagonal).value(),
			this->vertexNull4);

		EXPECT_EQ(
			this->vertexFullPositive4.project_on(
				hyperplaneDirectX,
				xablau::geometry::make_vector(std::to_array(
					{
						typename TypeParam::value_type{-1}
					}))).value(),
			this->vertexNull4 + (this->vertexFullPositive4 - this->vertexPositive1));

		EXPECT_EQ(
			this->vertexFullPositive4.project_on(
				hyperplaneReverseX,
				xablau::geometry::make_vector(std::to_array(
					{
						typename TypeParam::value_type{-1}
					}))).value(),
			this->vertexNull4 + (this->vertexFullPositive4 - this->vertexPositive1));

		EXPECT_EQ(
			this->vertexFullPositive4.project_on(
				hyperplaneDirectZ,
				xablau::geometry::make_vector(std::to_array(
					{
						typename TypeParam::value_type{0},
						typename TypeParam::value_type{0},
						typename TypeParam::value_type{-1}
					}))).value(),
			this->vertexNull4 + (this->vertexFullPositive4 - this->vertexPositive3));

		EXPECT_EQ(
			this->vertexFullPositive4.project_on(
				hyperplaneReverseZ,
				xablau::geometry::make_vector(std::to_array(
					{
						typename TypeParam::value_type{0},
						typename TypeParam::value_type{0},
						typename TypeParam::value_type{-1}
					}))).value(),
			this->vertexNull4 + (this->vertexFullPositive4 - this->vertexPositive3));

		EXPECT_EQ(
			this->vertexFullPositive4.project_on(hyperplaneDirectXWithOffset, -vectorDiagonal).value(),
			this->vertexNull4 - (this->vertexFullPositive4 - this->vertexNull4));
	}

	TYPED_TEST_P(GeometryVertexTest, RotationAroundXAxis)
	{
		xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > vertex(
			std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				}));

		constexpr auto ninetyDegrees =
			std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2};

		this->coordinatesAreAlmostEqual(
			vertex.rotate_around_X_axis(ninetyDegrees).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{-1},
					typename TypeParam::value_type{1}
				}));

		this->coordinatesAreAlmostEqual(
			vertex.rotate_around_X_axis(ninetyDegrees).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{-1},
					typename TypeParam::value_type{-1}
				}));

		this->coordinatesAreAlmostEqual(
			vertex.rotate_around_X_axis(ninetyDegrees).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{-1}
				}));

		this->coordinatesAreAlmostEqual(
			vertex.rotate_around_X_axis(ninetyDegrees).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				}));
	}

	TYPED_TEST_P(GeometryVertexTest, RotationAroundYAxis)
	{
		xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > vertex(
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				}));

		constexpr auto ninetyDegrees =
			std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2};

		this->coordinatesAreAlmostEqual(
			vertex.rotate_around_Y_axis(ninetyDegrees).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{-1}
				}));

		this->coordinatesAreAlmostEqual(
			vertex.rotate_around_Y_axis(ninetyDegrees).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{-1},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{-1}
				}));

		this->coordinatesAreAlmostEqual(
			vertex.rotate_around_Y_axis(ninetyDegrees).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{-1},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				}));

		this->coordinatesAreAlmostEqual(
			vertex.rotate_around_Y_axis(ninetyDegrees).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				}));
	}

	TYPED_TEST_P(GeometryVertexTest, RotationAroundZAxis)
	{
		xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vertex(
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				}));

		constexpr auto ninetyDegrees =
			std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2};

		this->coordinatesAreAlmostEqual(
			vertex.rotate_around_Z_axis(ninetyDegrees).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{-1},
					typename TypeParam::value_type{1}
				}));

		this->coordinatesAreAlmostEqual(
			vertex.rotate_around_Z_axis(ninetyDegrees).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{-1},
					typename TypeParam::value_type{-1}
				}));

		this->coordinatesAreAlmostEqual(
			vertex.rotate_around_Z_axis(ninetyDegrees).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{-1}
				}));

		this->coordinatesAreAlmostEqual(
			vertex.rotate_around_Z_axis(ninetyDegrees).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				}));
	}

	TYPED_TEST_P(GeometryVertexTest, RotationAroundArbitraryAxis)
	{
		xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > vertex(
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				}));

		xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > axis(
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{-1},
					typename TypeParam::value_type{1}
				}));

		constexpr auto ninetyDegrees =
			std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2};

		this->coordinatesAreAlmostEqual(
			vertex.rotate_around_arbitrary_axis(axis, ninetyDegrees).coordinates,
			std::to_array(
				{
					-((typename TypeParam::value_type{2} / std::sqrt(typename TypeParam::value_type{3})) -
						(typename TypeParam::value_type{1} / typename TypeParam::value_type{3})),
					typename TypeParam::value_type{-1} / typename TypeParam::value_type{3},
					-((typename TypeParam::value_type{-2} / std::sqrt(typename TypeParam::value_type{3})) -
						(typename TypeParam::value_type{1} / typename TypeParam::value_type{3}))
				}));

		this->coordinatesAreAlmostEqual(
			vertex.rotate_around_arbitrary_axis(axis, ninetyDegrees).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{-1} / typename TypeParam::value_type{3},
					typename TypeParam::value_type{-5} / typename TypeParam::value_type{3},
					typename TypeParam::value_type{-1} / typename TypeParam::value_type{3}
				}));

		this->coordinatesAreAlmostEqual(
			vertex.rotate_around_arbitrary_axis(axis, ninetyDegrees).coordinates,
			std::to_array(
				{
					-((typename TypeParam::value_type{-2} / std::sqrt(typename TypeParam::value_type{3})) -
						(typename TypeParam::value_type{1} / typename TypeParam::value_type{3})),
					typename TypeParam::value_type{-1} / typename TypeParam::value_type{3},
					-((typename TypeParam::value_type{2} / std::sqrt(typename TypeParam::value_type{3})) -
						(typename TypeParam::value_type{1} / typename TypeParam::value_type{3}))
				}));

		this->coordinatesAreAlmostEqual(
			vertex.rotate_around_arbitrary_axis(axis, ninetyDegrees).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				}));
	}

	REGISTER_TYPED_TEST_SUITE_P(
		GeometryVertexTest,
		ComparisonOperatorLess,
		ComparisonOperatorLessEqualTo,
		ComparisonOperatorEqualTo,
		ComparisonOperatorGreaterEqualTo,
		ComparisonOperatorGreater,
		ComparisonOperatorNotEqualTo,
		AccessAliases,
		ContainmentSegment,
		ContainmentLine,
		ContainmentHyperplane,
		ContainmentHypersphere,
		DistanceVertice,
		DistanceLine,
		DistanceHyperplane,
		Scaling,
		VectorCreation,
		VectorProduct,
		AngleWithoutSign,
		AngleWithSign,
		Translation,
		Projection,
		RotationAroundXAxis,
		RotationAroundYAxis,
		RotationAroundZAxis,
		RotationAroundArbitraryAxis);

	using GeometryVertexTestDataTypes = std::tuple < float, double, long double >;

	using GeometryVertexTestGroup =
		xablau::testing::test_bundler < GeometryVertexTestDataTypes > ::template test_group < GeometryVertexTypeDefinitions >;

	using GeometryVertexTypes = xablau::testing::tuple_transformation < ::testing::Types, GeometryVertexTestGroup > ::types;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		Xablau,
		GeometryVertexTest,
		GeometryVertexTypes);
}
