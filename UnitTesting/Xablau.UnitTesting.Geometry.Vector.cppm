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
	struct GeometryVectorTypeDefinitions
	{
		using value_type = Type;
	};

	template < typename Types >
	class GeometryVectorTest : public ::testing::Test
	{
	public:
		static constexpr xablau::geometry::vector < typename Types::value_type, xablau::geometry::spatial_dimension < 0 > > vectorNull0;
		static constexpr xablau::geometry::vector < typename Types::value_type, xablau::geometry::spatial_dimension < 1 > > vectorNull1;
		static constexpr xablau::geometry::vector < typename Types::value_type, xablau::geometry::spatial_dimension < 2 > > vectorNull2;
		static constexpr xablau::geometry::vector < typename Types::value_type, xablau::geometry::spatial_dimension < 3 > > vectorNull3;
		static constexpr xablau::geometry::vector < typename Types::value_type, xablau::geometry::spatial_dimension < 4 > > vectorNull4;

		static constexpr xablau::geometry::vector < typename Types::value_type, xablau::geometry::spatial_dimension < 1 > > vectorNegative1 = std::to_array({ typename Types::value_type(-1) });
		static constexpr xablau::geometry::vector < typename Types::value_type, xablau::geometry::spatial_dimension < 2 > > vectorNegative2 = std::to_array({ typename Types::value_type(), typename Types::value_type(-1) });
		static constexpr xablau::geometry::vector < typename Types::value_type, xablau::geometry::spatial_dimension < 3 > > vectorNegative3 = std::to_array({ typename Types::value_type(), typename Types::value_type(), typename Types::value_type(-1) });
		static constexpr xablau::geometry::vector < typename Types::value_type, xablau::geometry::spatial_dimension < 4 > > vectorNegative4 = std::to_array({ typename Types::value_type(), typename Types::value_type(), typename Types::value_type(), typename Types::value_type(-1) });

		static constexpr xablau::geometry::vector < typename Types::value_type, xablau::geometry::spatial_dimension < 1 > > vectorPositive1 = std::to_array({ typename Types::value_type(1) });
		static constexpr xablau::geometry::vector < typename Types::value_type, xablau::geometry::spatial_dimension < 2 > > vectorPositive2 = std::to_array({ typename Types::value_type(), typename Types::value_type(1) });
		static constexpr xablau::geometry::vector < typename Types::value_type, xablau::geometry::spatial_dimension < 3 > > vectorPositive3 = std::to_array({ typename Types::value_type(), typename Types::value_type(), typename Types::value_type(1) });
		static constexpr xablau::geometry::vector < typename Types::value_type, xablau::geometry::spatial_dimension < 4 > > vectorPositive4 = std::to_array({ typename Types::value_type(), typename Types::value_type(), typename Types::value_type(), typename Types::value_type(1) });

		static constexpr xablau::geometry::vector < typename Types::value_type, xablau::geometry::spatial_dimension < 1 > > vectorFullNegative1 = std::to_array({ typename Types::value_type(-1) });
		static constexpr xablau::geometry::vector < typename Types::value_type, xablau::geometry::spatial_dimension < 2 > > vectorFullNegative2 = std::to_array({ typename Types::value_type(-1), typename Types::value_type(-1) });
		static constexpr xablau::geometry::vector < typename Types::value_type, xablau::geometry::spatial_dimension < 3 > > vectorFullNegative3 = std::to_array({ typename Types::value_type(-1), typename Types::value_type(-1), typename Types::value_type(-1) });
		static constexpr xablau::geometry::vector < typename Types::value_type, xablau::geometry::spatial_dimension < 4 > > vectorFullNegative4 = std::to_array({ typename Types::value_type(-1), typename Types::value_type(-1), typename Types::value_type(-1), typename Types::value_type(-1) });

		static constexpr xablau::geometry::vector < typename Types::value_type, xablau::geometry::spatial_dimension < 1 > > vectorFullPositive1 = std::to_array({ typename Types::value_type(1) });
		static constexpr xablau::geometry::vector < typename Types::value_type, xablau::geometry::spatial_dimension < 2 > > vectorFullPositive2 = std::to_array({ typename Types::value_type(1), typename Types::value_type(1) });
		static constexpr xablau::geometry::vector < typename Types::value_type, xablau::geometry::spatial_dimension < 3 > > vectorFullPositive3 = std::to_array({ typename Types::value_type(1), typename Types::value_type(1), typename Types::value_type(1) });
		static constexpr xablau::geometry::vector < typename Types::value_type, xablau::geometry::spatial_dimension < 4 > > vectorFullPositive4 = std::to_array({ typename Types::value_type(1), typename Types::value_type(1), typename Types::value_type(1), typename Types::value_type(1) });

		static void coordinatesAreAlmostEqual(const auto &coordinates1, const auto &coordinates2)
		{
			for (size_t i = 0; i < coordinates1.size(); i++)
			{
				EXPECT_LT(std::abs(coordinates1[i] - coordinates2[i]), typename Types::value_type{0.00001});
			}
		}
	};

	TYPED_TEST_SUITE_P(GeometryVectorTest);

	TYPED_TEST_P(GeometryVectorTest, ComparisonOperatorLesser)
	{
		EXPECT_TRUE(this->vectorNegative1 < this->vectorNull1);
		EXPECT_TRUE(this->vectorNegative1 < this->vectorNull2);
		EXPECT_TRUE(this->vectorNegative1 < this->vectorNull3);
		EXPECT_TRUE(this->vectorNegative1 < this->vectorNull4);
		EXPECT_TRUE(this->vectorNegative2 < this->vectorNull2);
		EXPECT_TRUE(this->vectorNegative2 < this->vectorNull3);
		EXPECT_TRUE(this->vectorNegative2 < this->vectorNull4);
		EXPECT_TRUE(this->vectorNegative3 < this->vectorNull3);
		EXPECT_TRUE(this->vectorNegative3 < this->vectorNull4);
		EXPECT_TRUE(this->vectorNegative4 < this->vectorNull4);
		EXPECT_FALSE(this->vectorNegative1 < this->vectorNegative1);
		EXPECT_FALSE(this->vectorNegative2 < this->vectorNegative2);
		EXPECT_FALSE(this->vectorNegative3 < this->vectorNegative3);
		EXPECT_FALSE(this->vectorNegative4 < this->vectorNegative4);

		EXPECT_FALSE(this->vectorPositive1 < this->vectorNull1);
		EXPECT_FALSE(this->vectorPositive1 < this->vectorNull2);
		EXPECT_FALSE(this->vectorPositive1 < this->vectorNull3);
		EXPECT_FALSE(this->vectorPositive1 < this->vectorNull4);
		EXPECT_FALSE(this->vectorPositive2 < this->vectorNull2);
		EXPECT_FALSE(this->vectorPositive2 < this->vectorNull3);
		EXPECT_FALSE(this->vectorPositive2 < this->vectorNull4);
		EXPECT_FALSE(this->vectorPositive3 < this->vectorNull3);
		EXPECT_FALSE(this->vectorPositive3 < this->vectorNull4);
		EXPECT_FALSE(this->vectorPositive4 < this->vectorNull4);
		EXPECT_FALSE(this->vectorPositive1 < this->vectorPositive1);
		EXPECT_FALSE(this->vectorPositive2 < this->vectorPositive2);
		EXPECT_FALSE(this->vectorPositive3 < this->vectorPositive3);
		EXPECT_FALSE(this->vectorPositive4 < this->vectorPositive4);
	}

	TYPED_TEST_P(GeometryVectorTest, ComparisonOperatorLesserEqualTo)
	{
		EXPECT_TRUE(this->vectorNegative1 <= this->vectorNull1);
		EXPECT_TRUE(this->vectorNegative1 <= this->vectorNull2);
		EXPECT_TRUE(this->vectorNegative1 <= this->vectorNull3);
		EXPECT_TRUE(this->vectorNegative1 <= this->vectorNull4);
		EXPECT_TRUE(this->vectorNegative2 <= this->vectorNull2);
		EXPECT_TRUE(this->vectorNegative2 <= this->vectorNull3);
		EXPECT_TRUE(this->vectorNegative2 <= this->vectorNull4);
		EXPECT_TRUE(this->vectorNegative3 <= this->vectorNull3);
		EXPECT_TRUE(this->vectorNegative3 <= this->vectorNull4);
		EXPECT_TRUE(this->vectorNegative4 <= this->vectorNull4);
		EXPECT_TRUE(this->vectorNegative1 <= this->vectorNegative1);
		EXPECT_TRUE(this->vectorNegative2 <= this->vectorNegative2);
		EXPECT_TRUE(this->vectorNegative3 <= this->vectorNegative3);
		EXPECT_TRUE(this->vectorNegative4 <= this->vectorNegative4);

		EXPECT_TRUE(this->vectorPositive1 <= this->vectorPositive1);
		EXPECT_TRUE(this->vectorPositive2 <= this->vectorPositive2);
		EXPECT_TRUE(this->vectorPositive3 <= this->vectorPositive3);
		EXPECT_TRUE(this->vectorPositive4 <= this->vectorPositive4);
	}

	TYPED_TEST_P(GeometryVectorTest, ComparisonOperatorEqualTo)
	{
		EXPECT_EQ(this->vectorNull0, this->vectorNull0);
		EXPECT_EQ(this->vectorNull0, this->vectorNull1);
		EXPECT_EQ(this->vectorNull0, this->vectorNull2);
		EXPECT_EQ(this->vectorNull0, this->vectorNull3);
		EXPECT_EQ(this->vectorNull0, this->vectorNull4);
		EXPECT_EQ(this->vectorNull1, this->vectorNull1);
		EXPECT_EQ(this->vectorNull1, this->vectorNull2);
		EXPECT_EQ(this->vectorNull1, this->vectorNull3);
		EXPECT_EQ(this->vectorNull1, this->vectorNull4);
		EXPECT_EQ(this->vectorNull2, this->vectorNull2);
		EXPECT_EQ(this->vectorNull2, this->vectorNull3);
		EXPECT_EQ(this->vectorNull2, this->vectorNull4);
		EXPECT_EQ(this->vectorNull3, this->vectorNull3);
		EXPECT_EQ(this->vectorNull3, this->vectorNull4);
		EXPECT_EQ(this->vectorNull4, this->vectorNull4);
	}

	TYPED_TEST_P(GeometryVectorTest, ComparisonOperatorGreaterEqualTo)
	{
		EXPECT_TRUE(this->vectorNegative1 >= this->vectorNegative1);
		EXPECT_TRUE(this->vectorNegative2 >= this->vectorNegative2);
		EXPECT_TRUE(this->vectorNegative3 >= this->vectorNegative3);
		EXPECT_TRUE(this->vectorNegative4 >= this->vectorNegative4);

		EXPECT_TRUE(this->vectorPositive1 >= this->vectorNull1);
		EXPECT_TRUE(this->vectorPositive1 >= this->vectorNull2);
		EXPECT_TRUE(this->vectorPositive1 >= this->vectorNull3);
		EXPECT_TRUE(this->vectorPositive1 >= this->vectorNull4);
		EXPECT_TRUE(this->vectorPositive2 >= this->vectorNull2);
		EXPECT_TRUE(this->vectorPositive2 >= this->vectorNull3);
		EXPECT_TRUE(this->vectorPositive2 >= this->vectorNull4);
		EXPECT_TRUE(this->vectorPositive3 >= this->vectorNull3);
		EXPECT_TRUE(this->vectorPositive3 >= this->vectorNull4);
		EXPECT_TRUE(this->vectorPositive4 >= this->vectorNull4);
		EXPECT_TRUE(this->vectorPositive1 >= this->vectorPositive1);
		EXPECT_TRUE(this->vectorPositive2 >= this->vectorPositive2);
		EXPECT_TRUE(this->vectorPositive3 >= this->vectorPositive3);
		EXPECT_TRUE(this->vectorPositive4 >= this->vectorPositive4);
	}

	TYPED_TEST_P(GeometryVectorTest, ComparisonOperatorGreater)
	{
		EXPECT_FALSE(this->vectorNegative1 > this->vectorNull1);
		EXPECT_FALSE(this->vectorNegative1 > this->vectorNull2);
		EXPECT_FALSE(this->vectorNegative1 > this->vectorNull3);
		EXPECT_FALSE(this->vectorNegative1 > this->vectorNull4);
		EXPECT_FALSE(this->vectorNegative2 > this->vectorNull2);
		EXPECT_FALSE(this->vectorNegative2 > this->vectorNull3);
		EXPECT_FALSE(this->vectorNegative2 > this->vectorNull4);
		EXPECT_FALSE(this->vectorNegative3 > this->vectorNull3);
		EXPECT_FALSE(this->vectorNegative3 > this->vectorNull4);
		EXPECT_FALSE(this->vectorNegative4 > this->vectorNull4);
		EXPECT_FALSE(this->vectorNegative1 > this->vectorNegative1);
		EXPECT_FALSE(this->vectorNegative2 > this->vectorNegative2);
		EXPECT_FALSE(this->vectorNegative3 > this->vectorNegative3);
		EXPECT_FALSE(this->vectorNegative4 > this->vectorNegative4);

		EXPECT_TRUE(this->vectorPositive1 > this->vectorNull1);
		EXPECT_TRUE(this->vectorPositive1 > this->vectorNull2);
		EXPECT_TRUE(this->vectorPositive1 > this->vectorNull3);
		EXPECT_TRUE(this->vectorPositive1 > this->vectorNull4);
		EXPECT_TRUE(this->vectorPositive2 > this->vectorNull2);
		EXPECT_TRUE(this->vectorPositive2 > this->vectorNull3);
		EXPECT_TRUE(this->vectorPositive2 > this->vectorNull4);
		EXPECT_TRUE(this->vectorPositive3 > this->vectorNull3);
		EXPECT_TRUE(this->vectorPositive3 > this->vectorNull4);
		EXPECT_TRUE(this->vectorPositive4 > this->vectorNull4);
		EXPECT_FALSE(this->vectorPositive1 > this->vectorPositive1);
		EXPECT_FALSE(this->vectorPositive2 > this->vectorPositive2);
		EXPECT_FALSE(this->vectorPositive3 > this->vectorPositive3);
		EXPECT_FALSE(this->vectorPositive4 > this->vectorPositive4);
	}

	TYPED_TEST_P(GeometryVectorTest, ComparisonOperatorNotEqualTo)
	{
		EXPECT_NE(this->vectorPositive1, this->vectorNegative1);
		EXPECT_NE(this->vectorPositive1, this->vectorNegative1);
		EXPECT_NE(this->vectorPositive1, this->vectorNegative1);
		EXPECT_NE(this->vectorPositive1, this->vectorNegative1);
		EXPECT_NE(this->vectorPositive2, this->vectorNegative2);
		EXPECT_NE(this->vectorPositive2, this->vectorNegative2);
		EXPECT_NE(this->vectorPositive2, this->vectorNegative2);
		EXPECT_NE(this->vectorPositive3, this->vectorNegative3);
		EXPECT_NE(this->vectorPositive3, this->vectorNegative3);
		EXPECT_NE(this->vectorPositive4, this->vectorNegative4);
	}

	TYPED_TEST_P(GeometryVectorTest, AccessAliases)
	{
		EXPECT_EQ(this->vectorNull0.x(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vectorNull0.y(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vectorNull0.z(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vectorNull0.w(), typename TypeParam::value_type{});

		EXPECT_EQ(this->vectorNegative1.x(), typename TypeParam::value_type{-1});
		EXPECT_EQ(this->vectorNegative1.y(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vectorNegative1.z(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vectorNegative1.w(), typename TypeParam::value_type{});

		EXPECT_EQ(this->vectorPositive1.x(), typename TypeParam::value_type{1});
		EXPECT_EQ(this->vectorPositive1.y(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vectorPositive1.z(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vectorPositive1.w(), typename TypeParam::value_type{});

		EXPECT_EQ(this->vectorNegative2.x(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vectorNegative2.y(), typename TypeParam::value_type{-1});
		EXPECT_EQ(this->vectorNegative2.z(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vectorNegative2.w(), typename TypeParam::value_type{});

		EXPECT_EQ(this->vectorPositive2.x(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vectorPositive2.y(), typename TypeParam::value_type{1});
		EXPECT_EQ(this->vectorPositive2.z(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vectorPositive2.w(), typename TypeParam::value_type{});

		EXPECT_EQ(this->vectorNegative3.x(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vectorNegative3.y(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vectorNegative3.z(), typename TypeParam::value_type{-1});
		EXPECT_EQ(this->vectorNegative3.w(), typename TypeParam::value_type{});

		EXPECT_EQ(this->vectorPositive3.x(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vectorPositive3.y(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vectorPositive3.z(), typename TypeParam::value_type{1});
		EXPECT_EQ(this->vectorPositive3.w(), typename TypeParam::value_type{});

		EXPECT_EQ(this->vectorNegative4.x(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vectorNegative4.y(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vectorNegative4.z(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vectorNegative4.w(), typename TypeParam::value_type{-1});

		EXPECT_EQ(this->vectorPositive4.x(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vectorPositive4.y(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vectorPositive4.z(), typename TypeParam::value_type{});
		EXPECT_EQ(this->vectorPositive4.w(), typename TypeParam::value_type{1});
	}

	TYPED_TEST_P(GeometryVectorTest, Scaling)
	{
		xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 4 > > vectorPower =
			std::to_array(
				{
					typename TypeParam::value_type(1),
					typename TypeParam::value_type(2),
					typename TypeParam::value_type(4),
					typename TypeParam::value_type(8)
				});

		EXPECT_EQ(
			(typename TypeParam::value_type{2} * vectorPower).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{2},
					typename TypeParam::value_type{4},
					typename TypeParam::value_type{8},
					typename TypeParam::value_type{16}
				}));

		EXPECT_EQ(
			(vectorPower * typename TypeParam::value_type{2}).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{2},
					typename TypeParam::value_type{4},
					typename TypeParam::value_type{8},
					typename TypeParam::value_type{16}
				}));

		EXPECT_EQ(
			vectorPower.scale_by(typename TypeParam::value_type{2}).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{2},
					typename TypeParam::value_type{4},
					typename TypeParam::value_type{8},
					typename TypeParam::value_type{16}
				}));

		EXPECT_EQ(
			(typename TypeParam::value_type{0.5} * vectorPower).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{2},
					typename TypeParam::value_type{4},
					typename TypeParam::value_type{8}
				}));

		EXPECT_EQ(
			(vectorPower * typename TypeParam::value_type{0.5}).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{2},
					typename TypeParam::value_type{4},
					typename TypeParam::value_type{8}
				}));

		EXPECT_EQ(
			(vectorPower / typename TypeParam::value_type{2}).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{2},
					typename TypeParam::value_type{4},
					typename TypeParam::value_type{8}
				}));

		EXPECT_EQ(
			vectorPower.scale_by(typename TypeParam::value_type{0.5}).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{2},
					typename TypeParam::value_type{4},
					typename TypeParam::value_type{8}
				}));

		EXPECT_EQ(
			vectorPower.scale_by(
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
			vectorPower.scale_by(
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
			vectorPower.scale_by(
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
			vectorPower.scale_by(
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

	TYPED_TEST_P(GeometryVectorTest, Norm)
	{
		auto localVectorFullPositive1 = this->vectorFullPositive1;
		auto localVectorFullPositive2 = this->vectorFullPositive2;
		auto localVectorFullPositive3 = this->vectorFullPositive3;
		auto localVectorFullPositive4 = this->vectorFullPositive4;

		EXPECT_EQ(this->vectorFullPositive1.norm(), std::sqrt(typename TypeParam::value_type{1}));
		EXPECT_EQ(this->vectorFullPositive2.norm(), std::sqrt(typename TypeParam::value_type{2}));
		EXPECT_EQ(this->vectorFullPositive3.norm(), std::sqrt(typename TypeParam::value_type{3}));
		EXPECT_EQ(this->vectorFullPositive4.norm(), std::sqrt(typename TypeParam::value_type{4}));

		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->vectorFullPositive1.normalize().norm(), typename TypeParam::value_type{1}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->vectorFullPositive2.normalize().norm(), typename TypeParam::value_type{1}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->vectorFullPositive3.normalize().norm(), typename TypeParam::value_type{1}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (this->vectorFullPositive4.normalize().norm(), typename TypeParam::value_type{1}));

		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (localVectorFullPositive1.normalize_in_place().norm(), typename TypeParam::value_type{1}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (localVectorFullPositive2.normalize_in_place().norm(), typename TypeParam::value_type{1}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (localVectorFullPositive3.normalize_in_place().norm(), typename TypeParam::value_type{1}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 1 > (localVectorFullPositive4.normalize_in_place().norm(), typename TypeParam::value_type{1}));
	}

	TYPED_TEST_P(GeometryVectorTest, ScalarProduct)
	{
		xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension <  4> > vectorProductLeft =
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{3},
					typename TypeParam::value_type{4},
					typename TypeParam::value_type{0}
				});

		xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 5 > > vectorProductRight =
			std::to_array(
				{
					typename TypeParam::value_type{2},
					typename TypeParam::value_type{7},
					typename TypeParam::value_type{-5},
					typename TypeParam::value_type{4},
					typename TypeParam::value_type{0}
				});

		EXPECT_EQ(
			vectorProductLeft.scalar_product(vectorProductRight),
			typename TypeParam::value_type{3});
	}

	TYPED_TEST_P(GeometryVectorTest, VectorProduct)
	{
		xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > vectorProductLeft =
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{3},
					typename TypeParam::value_type{4}
				});

		xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > vectorProductRight =
			std::to_array(
				{
					typename TypeParam::value_type{2},
					typename TypeParam::value_type{7},
					typename TypeParam::value_type{-5}
				});

		EXPECT_EQ(
			vectorProductLeft.
				template vector_product < xablau::geometry::vector_product_dimension::three > (vectorProductRight).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{-43},
					typename TypeParam::value_type{13},
					typename TypeParam::value_type{1}
				}));
	}

	TYPED_TEST_P(GeometryVectorTest, AngleWithoutSign)
	{
		const typename TypeParam::value_type value1 = typename TypeParam::value_type{0.5};
		const typename TypeParam::value_type value2 = std::sqrt(typename TypeParam::value_type{2}) / typename TypeParam::value_type{2};
		const typename TypeParam::value_type value3 = std::sqrt(typename TypeParam::value_type{3}) / typename TypeParam::value_type{2};

		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector0degree =
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{0}
				});

		const xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector30degree(
			std::to_array(
				{
					value3,
					value1
				}));

		const xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector45degree =
			std::to_array(
				{
					value2,
					value2
				});

		const xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector60degree =
			std::to_array(
				{
					value1,
					value3
				});

		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector90degree =
			std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				});

		const xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector120degree =
			std::to_array(
				{
					-value1,
					value3
				});

		const xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector135degree =
			std::to_array(
				{
					-value2,
					value2
				});

		const xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector150degree =
			std::to_array(
				{
					-value3,
					value1
				});

		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector180degree =
			std::to_array(
				{
					typename TypeParam::value_type{-1},
					typename TypeParam::value_type{0}
				});

		const xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector210degree =
			std::to_array(
				{
					-value3,
					-value1
				});

		const xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector225degree =
			std::to_array(
				{
					-value2,
					-value2
				});

		const xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector240degree =
			std::to_array(
				{
					-value1,
					-value3
				});

		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector270degree =
			std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{-1}
				});

		const xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector300degree =
			std::to_array(
				{
					value1,
					-value3
				});

		const xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector315degree =
			std::to_array(
				{
					value2,
					-value2
				});

		const xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector330degree =
			std::to_array(
				{
					value3,
					-value1
				});

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				vector0degree.angle_between(vector30degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{6}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				vector0degree.angle_between(vector45degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{4}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				vector0degree.angle_between(vector60degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{3}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				vector0degree.angle_between(vector90degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				vector0degree.angle_between(vector120degree),
				typename TypeParam::value_type{2} * std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{3}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				vector0degree.angle_between(vector135degree),
				typename TypeParam::value_type{3} * std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{4}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				vector0degree.angle_between(vector150degree),
				typename TypeParam::value_type{5} * std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{6}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				vector0degree.angle_between(vector180degree),
				std::numbers::pi_v < typename TypeParam::value_type >));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				vector0degree.angle_between(vector210degree),
				typename TypeParam::value_type{5} * std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{6}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				vector0degree.angle_between(vector225degree),
				typename TypeParam::value_type{3} * std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{4}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				vector0degree.angle_between(vector240degree),
				typename TypeParam::value_type{2} * std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{3}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				vector0degree.angle_between(vector270degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				vector0degree.angle_between(vector300degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{3}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				vector0degree.angle_between(vector315degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{4}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				vector0degree.angle_between(vector330degree),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{6}));
	}

	TYPED_TEST_P(GeometryVectorTest, AngleWithSign)
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

		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector0degree =
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{0}
				});

		const xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector30degree =
			std::to_array(
				{
					value3,
					value1
				});

		const xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector45degree =
			std::to_array(
				{
					value2,
					value2
				});

		const xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector60degree =
			std::to_array(
				{
					value1,
					value3
				});

		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector90degree =
			std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				});

		const xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector120degree =
			std::to_array(
				{
					-value1,
					value3
				});

		const xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector135degree =
			std::to_array(
				{
					-value2,
					value2
				});

		const xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector150degree =
			std::to_array(
				{
					-value3,
					value1
				});

		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector180degree =
			std::to_array(
				{
					typename TypeParam::value_type{-1},
					typename TypeParam::value_type{0}
				});

		const xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector210degree =
			std::to_array(
				{
					-value3,
					-value1
				});

		const xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector225degree =
			std::to_array(
				{
					-value2,
					-value2
				});

		const xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector240degree =
			std::to_array(
				{
					-value1,
					-value3
				});

		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector270degree =
			std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{-1}
				});

		const xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector300degree =
			std::to_array(
				{
					value1,
					-value3
				});

		const xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector315degree =
			std::to_array(
				{
					value2,
					-value2
				});

		const xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector330degree =
			std::to_array(
				{
					value3,
					-value1
				});

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				vector0degree.angle_between_with_sign(vector30degree, orientation),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{6}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				vector0degree.angle_between_with_sign(vector45degree, orientation),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{4}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				vector0degree.angle_between_with_sign(vector60degree, orientation),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{3}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				vector0degree.angle_between_with_sign(vector90degree, orientation),
				std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				vector0degree.angle_between_with_sign(vector120degree, orientation),
				typename TypeParam::value_type{2} * std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{3}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				vector0degree.angle_between_with_sign(vector135degree, orientation),
				typename TypeParam::value_type{3} * std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{4}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				vector0degree.angle_between_with_sign(vector150degree, orientation),
				typename TypeParam::value_type{5} * std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{6}));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				vector0degree.angle_between_with_sign(vector180degree, orientation),
				std::numbers::pi_v < typename TypeParam::value_type >));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				vector0degree.angle_between_with_sign(vector210degree, orientation),
				-(typename TypeParam::value_type{5} * std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{6})));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				vector0degree.angle_between_with_sign(vector225degree, orientation),
				-(typename TypeParam::value_type{3} * std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{4})));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				vector0degree.angle_between_with_sign(vector240degree, orientation),
				-(typename TypeParam::value_type{2} * std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{3})));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				vector0degree.angle_between_with_sign(vector270degree, orientation),
				-(std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2})));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				vector0degree.angle_between_with_sign(vector300degree, orientation),
				-(std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{3})));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				vector0degree.angle_between_with_sign(vector315degree, orientation),
				-(std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{4})));

		EXPECT_TRUE(
			xablau::algebra::functions::almost_equal < typename TypeParam::value_type > ::invoke < 10 > (
				vector0degree.angle_between_with_sign(vector330degree, orientation),
				-(std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{6})));
	}

	TYPED_TEST_P(GeometryVectorTest, Translation)
	{
		constexpr auto &vector2 = this->vectorPositive2;
		constexpr auto &vector3 = this->vectorPositive3;
		constexpr auto &vector4 = this->vectorPositive4;

		EXPECT_EQ(this->vectorPositive1 + vector2 + vector3 + vector4, this->vectorFullPositive4);
		EXPECT_EQ(this->vectorFullPositive4 - vector2 - vector3 - vector4, this->vectorPositive1);
	}

	TYPED_TEST_P(GeometryVectorTest, RotationAroundXAxis)
	{
		xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > vector(
			std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				}));

		constexpr auto ninetyDegrees =
			std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2};

		this->coordinatesAreAlmostEqual(
			vector.rotate_around_X_axis(ninetyDegrees).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{-1},
					typename TypeParam::value_type{1}
				}));

		this->coordinatesAreAlmostEqual(
			vector.rotate_around_X_axis(ninetyDegrees).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{-1},
					typename TypeParam::value_type{-1}
				}));

		this->coordinatesAreAlmostEqual(
			vector.rotate_around_X_axis(ninetyDegrees).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{-1}
				}));

		this->coordinatesAreAlmostEqual(
			vector.rotate_around_X_axis(ninetyDegrees).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				}));
	}

	TYPED_TEST_P(GeometryVectorTest, RotationAroundYAxis)
	{
		xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > vector(
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				}));

		constexpr auto ninetyDegrees =
			std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2};

		this->coordinatesAreAlmostEqual(
			vector.rotate_around_Y_axis(ninetyDegrees).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{-1}
				}));

		this->coordinatesAreAlmostEqual(
			vector.rotate_around_Y_axis(ninetyDegrees).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{-1},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{-1}
				}));

		this->coordinatesAreAlmostEqual(
			vector.rotate_around_Y_axis(ninetyDegrees).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{-1},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				}));

		this->coordinatesAreAlmostEqual(
			vector.rotate_around_Y_axis(ninetyDegrees).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{0},
					typename TypeParam::value_type{1}
				}));
	}

	TYPED_TEST_P(GeometryVectorTest, RotationAroundZAxis)
	{
		xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > vector(
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				}));

		constexpr auto ninetyDegrees =
			std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2};

		this->coordinatesAreAlmostEqual(
			vector.rotate_around_Z_axis(ninetyDegrees).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{-1},
					typename TypeParam::value_type{1}
				}));

		this->coordinatesAreAlmostEqual(
			vector.rotate_around_Z_axis(ninetyDegrees).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{-1},
					typename TypeParam::value_type{-1}
				}));

		this->coordinatesAreAlmostEqual(
			vector.rotate_around_Z_axis(ninetyDegrees).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{-1}
				}));

		this->coordinatesAreAlmostEqual(
			vector.rotate_around_Z_axis(ninetyDegrees).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				}));
	}

	TYPED_TEST_P(GeometryVectorTest, RotationAroundArbitraryAxis)
	{
		xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > vector(
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
			vector.rotate_around_arbitrary_axis(axis, ninetyDegrees).coordinates,
			std::to_array(
				{
					-((typename TypeParam::value_type{2} / std::sqrt(typename TypeParam::value_type{3})) -
						(typename TypeParam::value_type{1} / typename TypeParam::value_type{3})),
					typename TypeParam::value_type{-1} / typename TypeParam::value_type{3},
					-((typename TypeParam::value_type{-2} / std::sqrt(typename TypeParam::value_type{3})) -
						(typename TypeParam::value_type{1} / typename TypeParam::value_type{3}))
				}));

		this->coordinatesAreAlmostEqual(
			vector.rotate_around_arbitrary_axis(axis, ninetyDegrees).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{-1} / typename TypeParam::value_type{3},
					typename TypeParam::value_type{-5} / typename TypeParam::value_type{3},
					typename TypeParam::value_type{-1} / typename TypeParam::value_type{3}
				}));

		this->coordinatesAreAlmostEqual(
			vector.rotate_around_arbitrary_axis(axis, ninetyDegrees).coordinates,
			std::to_array(
				{
					-((typename TypeParam::value_type{-2} / std::sqrt(typename TypeParam::value_type{3})) -
						(typename TypeParam::value_type{1} / typename TypeParam::value_type{3})),
					typename TypeParam::value_type{-1} / typename TypeParam::value_type{3},
					-((typename TypeParam::value_type{2} / std::sqrt(typename TypeParam::value_type{3})) -
						(typename TypeParam::value_type{1} / typename TypeParam::value_type{3}))
				}));

		this->coordinatesAreAlmostEqual(
			vector.rotate_around_arbitrary_axis(axis, ninetyDegrees).coordinates,
			std::to_array(
				{
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1},
					typename TypeParam::value_type{1}
				}));
	}

	TYPED_TEST_P(GeometryVectorTest, Orthant)
	{
		EXPECT_TRUE(this->vectorNull4.same_orthant_as(this->vectorPositive4));
		EXPECT_TRUE(this->vectorNull4.same_orthant_as(this->vectorNegative4));
		EXPECT_TRUE(this->vectorNull4.same_orthant_as(this->vectorFullPositive4));
		EXPECT_TRUE(this->vectorNull4.same_orthant_as(this->vectorFullNegative4));

		EXPECT_FALSE(this->vectorPositive4.same_orthant_as(this->vectorNegative4));
		EXPECT_FALSE(this->vectorNegative4.same_orthant_as(this->vectorPositive4));
		EXPECT_FALSE(this->vectorFullPositive4.same_orthant_as(this->vectorFullNegative4));
		EXPECT_FALSE(this->vectorFullNegative4.same_orthant_as(this->vectorFullPositive4));
	}

	TYPED_TEST_P(GeometryVectorTest, Perpendicularity)
	{
		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 1 > > positiveX = std::to_array({ typename TypeParam::value_type(1) });
		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > positiveY = std::to_array({ typename TypeParam::value_type(), typename TypeParam::value_type(2) });
		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > positiveXY = std::to_array({ typename TypeParam::value_type(2), typename TypeParam::value_type(2) });
		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 1 > > negativeX = std::to_array({ typename TypeParam::value_type(-2) });
		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > negativeY = std::to_array({ typename TypeParam::value_type(), typename TypeParam::value_type(-2) });
		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > negativeXY = std::to_array({ typename TypeParam::value_type(-2), typename TypeParam::value_type(-2) });

		EXPECT_FALSE(positiveX.perpendicular_to(positiveX));
		EXPECT_TRUE(positiveX.perpendicular_to(positiveY));
		EXPECT_FALSE(positiveX.perpendicular_to(positiveXY));
		EXPECT_FALSE(positiveX.perpendicular_to(negativeX));
		EXPECT_TRUE(positiveX.perpendicular_to(negativeY));
		EXPECT_FALSE(positiveX.perpendicular_to(negativeXY));
		EXPECT_TRUE(positiveX.perpendicular_to(this->vectorNull4));
	}

	TYPED_TEST_P(GeometryVectorTest, Parallelism)
	{
		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 1 > > positiveX = std::to_array({ typename TypeParam::value_type(1) });
		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > positiveY = std::to_array({ typename TypeParam::value_type(), typename TypeParam::value_type(2) });
		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > positiveXY = std::to_array({ typename TypeParam::value_type(2), typename TypeParam::value_type(2) });
		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 1 > > negativeX = std::to_array({ typename TypeParam::value_type(-2) });
		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > negativeY = std::to_array({ typename TypeParam::value_type(), typename TypeParam::value_type(-2) });
		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > negativeXY = std::to_array({ typename TypeParam::value_type(-2), typename TypeParam::value_type(-2) });

		EXPECT_TRUE(positiveX.parallel_to(positiveX));
		EXPECT_FALSE(positiveX.parallel_to(positiveY));
		EXPECT_FALSE(positiveX.parallel_to(positiveXY));
		EXPECT_TRUE(positiveX.parallel_to(negativeX));
		EXPECT_FALSE(positiveX.parallel_to(negativeY));
		EXPECT_FALSE(positiveX.parallel_to(negativeXY));
		EXPECT_TRUE(positiveX.parallel_to(this->vectorNull4));
	}

	TYPED_TEST_P(GeometryVectorTest, Codirectional)
	{
		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 1 > > positiveX = std::to_array({ typename TypeParam::value_type(1) });
		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > positiveY = std::to_array({ typename TypeParam::value_type(), typename TypeParam::value_type(2) });
		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > positiveXY = std::to_array({ typename TypeParam::value_type(2), typename TypeParam::value_type(2) });
		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 1 > > negativeX = std::to_array({ typename TypeParam::value_type(-2) });
		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > negativeY = std::to_array({ typename TypeParam::value_type(), typename TypeParam::value_type(-2) });
		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > negativeXY = std::to_array({ typename TypeParam::value_type(-2), typename TypeParam::value_type(-2) });

		EXPECT_TRUE(positiveX.codirectional_to(positiveX));
		EXPECT_FALSE(positiveX.codirectional_to(positiveY));
		EXPECT_FALSE(positiveX.codirectional_to(positiveXY));
		EXPECT_FALSE(positiveX.codirectional_to(negativeX));
		EXPECT_FALSE(positiveX.codirectional_to(negativeY));
		EXPECT_FALSE(positiveX.codirectional_to(negativeXY));
		EXPECT_TRUE(positiveX.codirectional_to(this->vectorNull4));
	}

	TYPED_TEST_P(GeometryVectorTest, LinearIndependence)
	{
		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 1 > > positiveX = std::to_array({ typename TypeParam::value_type(1) });
		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > positiveY = std::to_array({ typename TypeParam::value_type(), typename TypeParam::value_type(2) });
		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > positiveXY = std::to_array({ typename TypeParam::value_type(2), typename TypeParam::value_type(2) });
		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 1 > > negativeX = std::to_array({ typename TypeParam::value_type(-2) });
		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > negativeY = std::to_array({ typename TypeParam::value_type(), typename TypeParam::value_type(-2) });
		constexpr xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > negativeXY = std::to_array({ typename TypeParam::value_type(-2), typename TypeParam::value_type(-2) });

		EXPECT_FALSE(positiveX.linearly_independent_to(positiveX));
		EXPECT_TRUE(positiveX.linearly_independent_to(positiveY));
		EXPECT_TRUE(positiveX.linearly_independent_to(positiveXY));
		EXPECT_FALSE(positiveX.linearly_independent_to(negativeX));
		EXPECT_TRUE(positiveX.linearly_independent_to(negativeY));
		EXPECT_TRUE(positiveX.linearly_independent_to(negativeXY));
		EXPECT_FALSE(positiveX.linearly_independent_to(this->vectorNull4));
	}

	TYPED_TEST_P(GeometryVectorTest, Projection)
	{
		EXPECT_EQ(this->vectorNull4.project_on(this->vectorPositive1), this->vectorNull0);
		EXPECT_EQ(this->vectorPositive2.project_on(this->vectorPositive1), this->vectorNull0);
		EXPECT_EQ(this->vectorFullPositive2.project_on(this->vectorPositive1), this->vectorPositive1);
		EXPECT_EQ(
			(typename TypeParam::value_type{2} * this->vectorFullPositive4).project_on(this->vectorPositive1),
			typename TypeParam::value_type{2} * this->vectorPositive1);
		EXPECT_EQ(
			(typename TypeParam::value_type{2} * this->vectorFullNegative4).project_on(this->vectorPositive1),
			typename TypeParam::value_type{2} * this->vectorNegative1);
		EXPECT_EQ(
			this->vectorPositive1.project_on(this->vectorFullPositive2),
			typename TypeParam::value_type{0.5} * this->vectorFullPositive2);
		EXPECT_EQ(
			(typename TypeParam::value_type{0.5} * this->vectorPositive1).project_on(this->vectorFullNegative2),
			(typename TypeParam::value_type{0.25} * this->vectorFullPositive2));
		EXPECT_EQ(this->vectorPositive1.project_on(this->vectorNull0), this->vectorNull0);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		GeometryVectorTest,
		ComparisonOperatorLesser,
		ComparisonOperatorLesserEqualTo,
		ComparisonOperatorEqualTo,
		ComparisonOperatorGreaterEqualTo,
		ComparisonOperatorGreater,
		ComparisonOperatorNotEqualTo,
		AccessAliases,
		Scaling,
		Norm,
		ScalarProduct,
		VectorProduct,
		AngleWithoutSign,
		AngleWithSign,
		Translation,
		RotationAroundXAxis,
		RotationAroundYAxis,
		RotationAroundZAxis,
		RotationAroundArbitraryAxis,
		Orthant,
		Perpendicularity,
		Parallelism,
		Codirectional,
		LinearIndependence,
		Projection);

	using GeometryVectorTestDataTypes = std::tuple < float, double, long double >;

	using GeometryVectorTestGroup =
		xablau::testing::test_bundler < GeometryVectorTestDataTypes > ::template test_group < GeometryVectorTypeDefinitions >;

	using GeometryVectorTypes = xablau::testing::tuple_transformation < ::testing::Types, GeometryVectorTestGroup > ::types;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		Xablau,
		GeometryVectorTest,
		GeometryVectorTypes);
}
