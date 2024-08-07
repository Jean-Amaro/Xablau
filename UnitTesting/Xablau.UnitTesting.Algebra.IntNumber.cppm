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

import xablau.algebra;
import xablau.testing;

namespace xablau::unit_testing
{
	class AlgebraIntNumberTest : public ::testing::Test
	{
	public:
		const xablau::algebra::int_number min = INTMAX_MIN;
		const xablau::algebra::int_number minus_sixteen = -16;
		const xablau::algebra::int_number minus_twelve = -12;
		const xablau::algebra::int_number minus_seven = -7;
		const xablau::algebra::int_number minus_six = -6;
		const xablau::algebra::int_number minus_five = -5;
		const xablau::algebra::int_number minus_four = -4;
		const xablau::algebra::int_number minus_three = -3;
		const xablau::algebra::int_number minus_two = -2;
		const xablau::algebra::int_number minus_one = -1;
		const xablau::algebra::int_number zero = 0;
		const xablau::algebra::int_number one = 1;
		const xablau::algebra::int_number two = 2;
		const xablau::algebra::int_number three = 3;
		const xablau::algebra::int_number four = 4;
		const xablau::algebra::int_number five = 5;
		const xablau::algebra::int_number six = 6;
		const xablau::algebra::int_number seven = 7;
		const xablau::algebra::int_number eleven = 11;
		const xablau::algebra::int_number twelve = 12;
		const xablau::algebra::int_number fifteen = 15;
		const xablau::algebra::int_number max = INTMAX_MAX;

		const xablau::algebra::int_number minus_two_2 = xablau::algebra::int_number(std::vector < bool > ({ false, true }));
		const xablau::algebra::int_number minus_one_1 = xablau::algebra::int_number(std::vector < bool > ({ true }));
		const xablau::algebra::int_number minus_one_2 = xablau::algebra::int_number(std::vector < bool > ({ true, true }));
		const xablau::algebra::int_number minus_one_3 = xablau::algebra::int_number(std::vector < bool > ({ true, true, true }));
		const xablau::algebra::int_number zero_0 = xablau::algebra::int_number(std::vector < bool > ({}));
		const xablau::algebra::int_number zero_1 = xablau::algebra::int_number(std::vector < bool > ({ false }));
		const xablau::algebra::int_number zero_2 = xablau::algebra::int_number(std::vector < bool > ({ false, false }));
		const xablau::algebra::int_number zero_3 = xablau::algebra::int_number(std::vector < bool > ({ false, false, false }));
		const xablau::algebra::int_number one_2 = xablau::algebra::int_number(std::vector < bool > ({ true, false }));
		const xablau::algebra::int_number one_3 = xablau::algebra::int_number(std::vector < bool > ({ true, false, false }));
	};

	TEST_F(AlgebraIntNumberTest, Signedness)
	{
		EXPECT_TRUE(this->min.bit_sign());
		EXPECT_TRUE(this->min.negative());
		EXPECT_FALSE(this->min.null());
		EXPECT_FALSE(this->min.positive());

		EXPECT_TRUE(this->minus_one_3.bit_sign());
		EXPECT_TRUE(this->minus_one_3.negative());
		EXPECT_FALSE(this->minus_one_3.null());
		EXPECT_FALSE(this->minus_one_3.positive());

		EXPECT_FALSE(this->zero_3.bit_sign());
		EXPECT_FALSE(this->zero_3.negative());
		EXPECT_TRUE(this->zero_3.null());
		EXPECT_FALSE(this->zero_3.positive());

		EXPECT_FALSE(this->one_3.bit_sign());
		EXPECT_FALSE(this->one_3.negative());
		EXPECT_FALSE(this->one_3.null());
		EXPECT_TRUE(this->one_3.positive());

		EXPECT_FALSE(this->max.bit_sign());
		EXPECT_FALSE(this->max.negative());
		EXPECT_FALSE(this->max.null());
		EXPECT_TRUE(this->max.positive());
	}

	TEST_F(AlgebraIntNumberTest, ElementAccess)
	{
		auto mutableMinusThree = this->minus_three;
		auto mutableZero = this->zero;
		auto mutableThree = this->three;

		EXPECT_EQ(mutableMinusThree[100], true);
		EXPECT_EQ(mutableMinusThree[3], true);
		EXPECT_EQ(mutableMinusThree[2], true);
		EXPECT_EQ(mutableMinusThree[1], false);
		EXPECT_EQ(mutableMinusThree[0], true);

		EXPECT_EQ(mutableZero[100], false);
		EXPECT_EQ(mutableZero[3], false);
		EXPECT_EQ(mutableZero[2], false);
		EXPECT_EQ(mutableZero[1], false);
		EXPECT_EQ(mutableZero[0], false);

		EXPECT_EQ(mutableThree[100], false);
		EXPECT_EQ(mutableThree[3], false);
		EXPECT_EQ(mutableThree[2], false);
		EXPECT_EQ(mutableThree[1], true);
		EXPECT_EQ(mutableThree[0], true);

		EXPECT_EQ(this->minus_three[100], true);
		EXPECT_EQ(this->minus_three[3], true);
		EXPECT_EQ(this->minus_three[2], true);
		EXPECT_EQ(this->minus_three[1], false);
		EXPECT_EQ(this->minus_three[0], true);

		EXPECT_EQ(this->zero[100], false);
		EXPECT_EQ(this->zero[3], false);
		EXPECT_EQ(this->zero[2], false);
		EXPECT_EQ(this->zero[1], false);
		EXPECT_EQ(this->zero[0], false);

		EXPECT_EQ(this->three[100], false);
		EXPECT_EQ(this->three[3], false);
		EXPECT_EQ(this->three[2], false);
		EXPECT_EQ(this->three[1], true);
		EXPECT_EQ(this->three[0], true);
	}

	TEST_F(AlgebraIntNumberTest, ComparisonOperatorLesserThan)
	{
		EXPECT_FALSE(this->minus_one < this->minus_one_1);
		EXPECT_FALSE(this->minus_one < this->minus_one_2);
		EXPECT_FALSE(this->minus_one < this->minus_one_3);
		EXPECT_FALSE(this->minus_one_1 < this->minus_one);
		EXPECT_FALSE(this->minus_one_2 < this->minus_one);
		EXPECT_FALSE(this->minus_one_3 < this->minus_one);

		EXPECT_FALSE(this->zero < this->zero_0);
		EXPECT_FALSE(this->zero < this->zero_1);
		EXPECT_FALSE(this->zero < this->zero_2);
		EXPECT_FALSE(this->zero < this->zero_3);
		EXPECT_FALSE(this->zero_0 < this->zero);
		EXPECT_FALSE(this->zero_1 < this->zero);
		EXPECT_FALSE(this->zero_2 < this->zero);
		EXPECT_FALSE(this->zero_3 < this->zero);

		EXPECT_FALSE(this->one < this->one_2);
		EXPECT_FALSE(this->one < this->one_3);
		EXPECT_FALSE(this->one_2 < this->one);
		EXPECT_FALSE(this->one_3 < this->one);

		EXPECT_TRUE(this->min < this->minus_sixteen);
		EXPECT_TRUE(this->minus_sixteen < this->minus_twelve);
		EXPECT_TRUE(this->minus_twelve < this->minus_six);
		EXPECT_TRUE(this->minus_six < this->minus_five);
		EXPECT_TRUE(this->minus_five < this->minus_four);
		EXPECT_TRUE(this->minus_four < this->minus_three);
		EXPECT_TRUE(this->minus_three < this->minus_two);
		EXPECT_TRUE(this->minus_two < this->minus_one);
		EXPECT_TRUE(this->minus_one < this->zero);
		EXPECT_TRUE(this->zero < this->one);
		EXPECT_TRUE(this->one < this->two);
		EXPECT_TRUE(this->two < this->three);
		EXPECT_TRUE(this->three < this->four);
		EXPECT_TRUE(this->four < this->five);
		EXPECT_TRUE(this->five < this->six);
		EXPECT_TRUE(this->six < this->twelve);
		EXPECT_TRUE(this->twelve < this->fifteen);
		EXPECT_TRUE(this->fifteen < this->max);

		EXPECT_FALSE(this->max < this->fifteen);
		EXPECT_FALSE(this->fifteen < this->twelve);
		EXPECT_FALSE(this->twelve < this->six);
		EXPECT_FALSE(this->six < this->five);
		EXPECT_FALSE(this->five < this->four);
		EXPECT_FALSE(this->four < this->three);
		EXPECT_FALSE(this->three < this->two);
		EXPECT_FALSE(this->two < this->one);
		EXPECT_FALSE(this->one < this->zero);
		EXPECT_FALSE(this->zero < this->minus_one);
		EXPECT_FALSE(this->minus_one < this->minus_two);
		EXPECT_FALSE(this->minus_two < this->minus_three);
		EXPECT_FALSE(this->minus_three < this->minus_four);
		EXPECT_FALSE(this->minus_four < this->minus_five);
		EXPECT_FALSE(this->minus_five < this->minus_six);
		EXPECT_FALSE(this->minus_six < this->minus_twelve);
		EXPECT_FALSE(this->minus_twelve < this->minus_sixteen);
		EXPECT_FALSE(this->minus_sixteen < this->min);

		EXPECT_TRUE(this->min < this->max);
		EXPECT_TRUE(this->minus_twelve < this->twelve);
		EXPECT_TRUE(this->minus_six < this->six);
		EXPECT_TRUE(this->minus_five < this->five);
		EXPECT_TRUE(this->minus_four < this->four);
		EXPECT_TRUE(this->minus_three < this->three);
		EXPECT_TRUE(this->minus_two < this->two);
		EXPECT_TRUE(this->minus_one < this->one);
		EXPECT_FALSE(this->zero < this->zero);
		EXPECT_FALSE(this->twelve < this->minus_twelve);
		EXPECT_FALSE(this->six < this->minus_six);
		EXPECT_FALSE(this->five < this->minus_five);
		EXPECT_FALSE(this->four < this->minus_four);
		EXPECT_FALSE(this->three < this->minus_three);
		EXPECT_FALSE(this->two < this->minus_two);
		EXPECT_FALSE(this->one < this->minus_one);
		EXPECT_FALSE(this->max < this->min);
	}

	TEST_F(AlgebraIntNumberTest, ComparisonOperatorLesserEqualTo)
	{
		EXPECT_TRUE(this->minus_one <= this->minus_one_1);
		EXPECT_TRUE(this->minus_one <= this->minus_one_2);
		EXPECT_TRUE(this->minus_one <= this->minus_one_3);
		EXPECT_TRUE(this->minus_one_1 <= this->minus_one);
		EXPECT_TRUE(this->minus_one_2 <= this->minus_one);
		EXPECT_TRUE(this->minus_one_3 <= this->minus_one);

		EXPECT_TRUE(this->zero <= this->zero_0);
		EXPECT_TRUE(this->zero <= this->zero_1);
		EXPECT_TRUE(this->zero <= this->zero_2);
		EXPECT_TRUE(this->zero <= this->zero_3);
		EXPECT_TRUE(this->zero_0 <= this->zero);
		EXPECT_TRUE(this->zero_1 <= this->zero);
		EXPECT_TRUE(this->zero_2 <= this->zero);
		EXPECT_TRUE(this->zero_3 <= this->zero);

		EXPECT_TRUE(this->one <= this->one_2);
		EXPECT_TRUE(this->one <= this->one_3);
		EXPECT_TRUE(this->one_2 <= this->one);
		EXPECT_TRUE(this->one_3 <= this->one);

		EXPECT_TRUE(this->min <= this->minus_sixteen);
		EXPECT_TRUE(this->minus_sixteen <= this->minus_twelve);
		EXPECT_TRUE(this->minus_twelve <= this->minus_six);
		EXPECT_TRUE(this->minus_six <= this->minus_five);
		EXPECT_TRUE(this->minus_five <= this->minus_four);
		EXPECT_TRUE(this->minus_four <= this->minus_three);
		EXPECT_TRUE(this->minus_three <= this->minus_two);
		EXPECT_TRUE(this->minus_two <= this->minus_one);
		EXPECT_TRUE(this->minus_one <= this->zero);
		EXPECT_TRUE(this->zero <= this->one);
		EXPECT_TRUE(this->one <= this->two);
		EXPECT_TRUE(this->two <= this->three);
		EXPECT_TRUE(this->three <= this->four);
		EXPECT_TRUE(this->four <= this->five);
		EXPECT_TRUE(this->five <= this->six);
		EXPECT_TRUE(this->six <= this->twelve);
		EXPECT_TRUE(this->twelve <= this->fifteen);
		EXPECT_TRUE(this->fifteen <= this->max);

		EXPECT_FALSE(this->max <= this->fifteen);
		EXPECT_FALSE(this->fifteen <= this->twelve);
		EXPECT_FALSE(this->twelve <= this->six);
		EXPECT_FALSE(this->six <= this->five);
		EXPECT_FALSE(this->five <= this->four);
		EXPECT_FALSE(this->four <= this->three);
		EXPECT_FALSE(this->three <= this->two);
		EXPECT_FALSE(this->two <= this->one);
		EXPECT_FALSE(this->one <= this->zero);
		EXPECT_FALSE(this->zero <= this->minus_one);
		EXPECT_FALSE(this->minus_one <= this->minus_two);
		EXPECT_FALSE(this->minus_two <= this->minus_three);
		EXPECT_FALSE(this->minus_three <= this->minus_four);
		EXPECT_FALSE(this->minus_four <= this->minus_five);
		EXPECT_FALSE(this->minus_five <= this->minus_six);
		EXPECT_FALSE(this->minus_six <= this->minus_twelve);
		EXPECT_FALSE(this->minus_twelve <= this->minus_sixteen);
		EXPECT_FALSE(this->minus_sixteen <= this->min);

		EXPECT_TRUE(this->min <= this->max);
		EXPECT_TRUE(this->minus_twelve <= this->twelve);
		EXPECT_TRUE(this->minus_six <= this->six);
		EXPECT_TRUE(this->minus_five <= this->five);
		EXPECT_TRUE(this->minus_four <= this->four);
		EXPECT_TRUE(this->minus_three <= this->three);
		EXPECT_TRUE(this->minus_two <= this->two);
		EXPECT_TRUE(this->minus_one <= this->one);
		EXPECT_TRUE(this->zero <= this->zero);
		EXPECT_FALSE(this->twelve <= this->minus_twelve);
		EXPECT_FALSE(this->six <= this->minus_six);
		EXPECT_FALSE(this->five <= this->minus_five);
		EXPECT_FALSE(this->four <= this->minus_four);
		EXPECT_FALSE(this->three <= this->minus_three);
		EXPECT_FALSE(this->two <= this->minus_two);
		EXPECT_FALSE(this->one <= this->minus_one);
		EXPECT_FALSE(this->max <= this->min);
	}

	TEST_F(AlgebraIntNumberTest, ComparisonOperatorGreaterThan)
	{
		EXPECT_FALSE(this->minus_one > this->minus_one_1);
		EXPECT_FALSE(this->minus_one > this->minus_one_2);
		EXPECT_FALSE(this->minus_one > this->minus_one_3);
		EXPECT_FALSE(this->minus_one_1 > this->minus_one);
		EXPECT_FALSE(this->minus_one_2 > this->minus_one);
		EXPECT_FALSE(this->minus_one_3 > this->minus_one);

		EXPECT_FALSE(this->zero > this->zero_0);
		EXPECT_FALSE(this->zero > this->zero_1);
		EXPECT_FALSE(this->zero > this->zero_2);
		EXPECT_FALSE(this->zero > this->zero_3);
		EXPECT_FALSE(this->zero_0 > this->zero);
		EXPECT_FALSE(this->zero_1 > this->zero);
		EXPECT_FALSE(this->zero_2 > this->zero);
		EXPECT_FALSE(this->zero_3 > this->zero);

		EXPECT_FALSE(this->one > this->one_2);
		EXPECT_FALSE(this->one > this->one_3);
		EXPECT_FALSE(this->one_2 > this->one);
		EXPECT_FALSE(this->one_3 > this->one);

		EXPECT_FALSE(this->min > this->minus_sixteen);
		EXPECT_FALSE(this->minus_sixteen > this->minus_twelve);
		EXPECT_FALSE(this->minus_twelve > this->minus_six);
		EXPECT_FALSE(this->minus_six > this->minus_five);
		EXPECT_FALSE(this->minus_five > this->minus_four);
		EXPECT_FALSE(this->minus_four > this->minus_three);
		EXPECT_FALSE(this->minus_three > this->minus_two);
		EXPECT_FALSE(this->minus_two > this->minus_one);
		EXPECT_FALSE(this->minus_one > this->zero);
		EXPECT_FALSE(this->zero > this->one);
		EXPECT_FALSE(this->one > this->two);
		EXPECT_FALSE(this->two > this->three);
		EXPECT_FALSE(this->three > this->four);
		EXPECT_FALSE(this->four > this->five);
		EXPECT_FALSE(this->five > this->six);
		EXPECT_FALSE(this->six > this->twelve);
		EXPECT_FALSE(this->twelve > this->fifteen);
		EXPECT_FALSE(this->fifteen > this->max);

		EXPECT_TRUE(this->max > this->fifteen);
		EXPECT_TRUE(this->fifteen > this->twelve);
		EXPECT_TRUE(this->twelve > this->six);
		EXPECT_TRUE(this->six > this->five);
		EXPECT_TRUE(this->five > this->four);
		EXPECT_TRUE(this->four > this->three);
		EXPECT_TRUE(this->three > this->two);
		EXPECT_TRUE(this->two > this->one);
		EXPECT_TRUE(this->one > this->zero);
		EXPECT_TRUE(this->zero > this->minus_one);
		EXPECT_TRUE(this->minus_one > this->minus_two);
		EXPECT_TRUE(this->minus_two > this->minus_three);
		EXPECT_TRUE(this->minus_three > this->minus_four);
		EXPECT_TRUE(this->minus_four > this->minus_five);
		EXPECT_TRUE(this->minus_five > this->minus_six);
		EXPECT_TRUE(this->minus_six > this->minus_twelve);
		EXPECT_TRUE(this->minus_twelve > this->minus_sixteen);
		EXPECT_TRUE(this->minus_sixteen > this->min);

		EXPECT_FALSE(this->min > this->max);
		EXPECT_FALSE(this->minus_twelve > this->twelve);
		EXPECT_FALSE(this->minus_six > this->six);
		EXPECT_FALSE(this->minus_five > this->five);
		EXPECT_FALSE(this->minus_four > this->four);
		EXPECT_FALSE(this->minus_three > this->three);
		EXPECT_FALSE(this->minus_two > this->two);
		EXPECT_FALSE(this->minus_one > this->one);
		EXPECT_FALSE(this->zero > this->zero);
		EXPECT_TRUE(this->twelve > this->minus_twelve);
		EXPECT_TRUE(this->six > this->minus_six);
		EXPECT_TRUE(this->five > this->minus_five);
		EXPECT_TRUE(this->four > this->minus_four);
		EXPECT_TRUE(this->three > this->minus_three);
		EXPECT_TRUE(this->two > this->minus_two);
		EXPECT_TRUE(this->one > this->minus_one);
		EXPECT_TRUE(this->max > this->min);
	}

	TEST_F(AlgebraIntNumberTest, ComparisonOperatorGreaterEqualTo)
	{
		EXPECT_TRUE(this->minus_one >= this->minus_one_1);
		EXPECT_TRUE(this->minus_one >= this->minus_one_2);
		EXPECT_TRUE(this->minus_one >= this->minus_one_3);
		EXPECT_TRUE(this->minus_one_1 >= this->minus_one);
		EXPECT_TRUE(this->minus_one_2 >= this->minus_one);
		EXPECT_TRUE(this->minus_one_3 >= this->minus_one);

		EXPECT_TRUE(this->zero >= this->zero_0);
		EXPECT_TRUE(this->zero >= this->zero_1);
		EXPECT_TRUE(this->zero >= this->zero_2);
		EXPECT_TRUE(this->zero >= this->zero_3);
		EXPECT_TRUE(this->zero_0 >= this->zero);
		EXPECT_TRUE(this->zero_1 >= this->zero);
		EXPECT_TRUE(this->zero_2 >= this->zero);
		EXPECT_TRUE(this->zero_3 >= this->zero);

		EXPECT_TRUE(this->one >= this->one_2);
		EXPECT_TRUE(this->one >= this->one_3);
		EXPECT_TRUE(this->one_2 >= this->one);
		EXPECT_TRUE(this->one_3 >= this->one);

		EXPECT_FALSE(this->min >= this->minus_sixteen);
		EXPECT_FALSE(this->minus_sixteen >= this->minus_twelve);
		EXPECT_FALSE(this->minus_twelve >= this->minus_six);
		EXPECT_FALSE(this->minus_six >= this->minus_five);
		EXPECT_FALSE(this->minus_five >= this->minus_four);
		EXPECT_FALSE(this->minus_four >= this->minus_three);
		EXPECT_FALSE(this->minus_three >= this->minus_two);
		EXPECT_FALSE(this->minus_two >= this->minus_one);
		EXPECT_FALSE(this->minus_one >= this->zero);
		EXPECT_FALSE(this->zero >= this->one);
		EXPECT_FALSE(this->one >= this->two);
		EXPECT_FALSE(this->two >= this->three);
		EXPECT_FALSE(this->three >= this->four);
		EXPECT_FALSE(this->four >= this->five);
		EXPECT_FALSE(this->five >= this->six);
		EXPECT_FALSE(this->six >= this->twelve);
		EXPECT_FALSE(this->twelve >= this->fifteen);
		EXPECT_FALSE(this->fifteen >= this->max);

		EXPECT_TRUE(this->max >= this->fifteen);
		EXPECT_TRUE(this->fifteen >= this->twelve);
		EXPECT_TRUE(this->twelve >= this->six);
		EXPECT_TRUE(this->six >= this->five);
		EXPECT_TRUE(this->five >= this->four);
		EXPECT_TRUE(this->four >= this->three);
		EXPECT_TRUE(this->three >= this->two);
		EXPECT_TRUE(this->two >= this->one);
		EXPECT_TRUE(this->one >= this->zero);
		EXPECT_TRUE(this->zero >= this->minus_one);
		EXPECT_TRUE(this->minus_one >= this->minus_two);
		EXPECT_TRUE(this->minus_two >= this->minus_three);
		EXPECT_TRUE(this->minus_three >= this->minus_four);
		EXPECT_TRUE(this->minus_four >= this->minus_five);
		EXPECT_TRUE(this->minus_five >= this->minus_six);
		EXPECT_TRUE(this->minus_six >= this->minus_twelve);
		EXPECT_TRUE(this->minus_twelve >= this->minus_sixteen);
		EXPECT_TRUE(this->minus_sixteen >= this->min);

		EXPECT_FALSE(this->min >= this->max);
		EXPECT_FALSE(this->minus_twelve >= this->twelve);
		EXPECT_FALSE(this->minus_six >= this->six);
		EXPECT_FALSE(this->minus_five >= this->five);
		EXPECT_FALSE(this->minus_four >= this->four);
		EXPECT_FALSE(this->minus_three >= this->three);
		EXPECT_FALSE(this->minus_two >= this->two);
		EXPECT_FALSE(this->minus_one >= this->one);
		EXPECT_TRUE(this->zero >= this->zero);
		EXPECT_TRUE(this->twelve >= this->minus_twelve);
		EXPECT_TRUE(this->six >= this->minus_six);
		EXPECT_TRUE(this->five >= this->minus_five);
		EXPECT_TRUE(this->four >= this->minus_four);
		EXPECT_TRUE(this->three >= this->minus_three);
		EXPECT_TRUE(this->two >= this->minus_two);
		EXPECT_TRUE(this->one >= this->minus_one);
		EXPECT_TRUE(this->max >= this->min);
	}

	TEST_F(AlgebraIntNumberTest, ComparisonOperatorEqualTo)
	{
		EXPECT_EQ(this->minus_one, this->minus_one_1);
		EXPECT_EQ(this->minus_one, this->minus_one_2);
		EXPECT_EQ(this->minus_one, this->minus_one_3);
		EXPECT_EQ(this->minus_one_1, this->minus_one);
		EXPECT_EQ(this->minus_one_2, this->minus_one);
		EXPECT_EQ(this->minus_one_3, this->minus_one);

		EXPECT_EQ(this->zero, this->zero_0);
		EXPECT_EQ(this->zero, this->zero_1);
		EXPECT_EQ(this->zero, this->zero_2);
		EXPECT_EQ(this->zero, this->zero_3);
		EXPECT_EQ(this->zero_0, this->zero);
		EXPECT_EQ(this->zero_1, this->zero);
		EXPECT_EQ(this->zero_2, this->zero);
		EXPECT_EQ(this->zero_3, this->zero);

		EXPECT_EQ(this->one, this->one_2);
		EXPECT_EQ(this->one, this->one_3);
		EXPECT_EQ(this->one_2, this->one);
		EXPECT_EQ(this->one_3, this->one);
	}

	TEST_F(AlgebraIntNumberTest, ComparisonOperatorNotEqualTo)
	{
		EXPECT_NE(this->minus_one, this->zero_3);
		EXPECT_NE(this->minus_one, this->one_3);
		EXPECT_NE(this->minus_one, this->two);
		EXPECT_NE(this->minus_one, this->three);
		EXPECT_NE(this->zero_3, this->minus_one);
		EXPECT_NE(this->one_3, this->minus_one);
		EXPECT_NE(this->two, this->minus_one);
		EXPECT_NE(this->three, this->minus_one);

		EXPECT_NE(this->zero, this->one_3);
		EXPECT_NE(this->zero, this->two);
		EXPECT_NE(this->zero, this->three);
		EXPECT_NE(this->one_3, this->zero);
		EXPECT_NE(this->two, this->zero);
		EXPECT_NE(this->three, this->zero);

		EXPECT_NE(this->one, this->two);
		EXPECT_NE(this->one, this->three);
		EXPECT_NE(this->two, this->one);
		EXPECT_NE(this->three, this->one);

		EXPECT_NE(this->two, this->three);
		EXPECT_NE(this->three, this->two);
	}

	TEST_F(AlgebraIntNumberTest, Conversion)
	{
		EXPECT_EQ(this->min.convert_to_int(), std::intmax_t{INTMAX_MIN});
		EXPECT_EQ(this->minus_three.convert_to_int(), std::intmax_t{-3});
		EXPECT_EQ(this->minus_two.convert_to_int(), std::intmax_t{-2});
		EXPECT_EQ(this->minus_one.convert_to_int(), std::intmax_t{-1});
		EXPECT_EQ(this->zero.convert_to_int(), std::intmax_t{0});
		EXPECT_EQ(this->one.convert_to_int(), std::intmax_t{1});
		EXPECT_EQ(this->two.convert_to_int(), std::intmax_t{2});
		EXPECT_EQ(this->three.convert_to_int(), std::intmax_t{3});
		EXPECT_EQ(this->max.convert_to_int(), std::intmax_t{INTMAX_MAX});

		EXPECT_EQ(this->minus_one_1.convert_to_int(), std::intmax_t{-1});
		EXPECT_EQ(this->minus_one_2.convert_to_int(), std::intmax_t{-1});
		EXPECT_EQ(this->minus_one_3.convert_to_int(), std::intmax_t{-1});
		EXPECT_EQ(this->zero_0.convert_to_int(), std::intmax_t{0});
		EXPECT_EQ(this->zero_1.convert_to_int(), std::intmax_t{0});
		EXPECT_EQ(this->zero_2.convert_to_int(), std::intmax_t{0});
		EXPECT_EQ(this->zero_3.convert_to_int(), std::intmax_t{0});
		EXPECT_EQ(this->one_2.convert_to_int(), std::intmax_t{1});
		EXPECT_EQ(this->one_3.convert_to_int(), std::intmax_t{1});
	}

	TEST_F(AlgebraIntNumberTest, Negation)
	{
		auto mutableThree = this->minus_three;
		auto mutableZero = this->zero;
		auto mutableMinusThree = this->three;

		mutableMinusThree.negate();
		mutableZero.negate();
		mutableThree.negate();

		EXPECT_EQ(mutableMinusThree, this->minus_three);
		EXPECT_EQ(mutableZero, this->zero_3);
		EXPECT_EQ(mutableThree, this->three);

		EXPECT_EQ(mutableMinusThree, -this->three);
		EXPECT_EQ(mutableZero, -this->zero_3);
		EXPECT_EQ(mutableThree, -this->minus_three);
	}

	TEST_F(AlgebraIntNumberTest, Addition)
	{
		EXPECT_EQ(this->minus_two + this->minus_one, this->minus_three);
		EXPECT_EQ(this->minus_one + this->minus_two, this->minus_three);

		EXPECT_EQ(this->minus_one + this->zero_0, this->minus_one_3);
		EXPECT_EQ(this->zero_0 + this->minus_one, this->minus_one_3);

		EXPECT_EQ(this->minus_one_3 + this->one, this->zero_0);
		EXPECT_EQ(this->one + this->minus_one_3, this->zero_0);

		EXPECT_EQ(this->one_3 + this->zero, this->one_3);
		EXPECT_EQ(this->zero + this->one_3, this->one_3);

		EXPECT_EQ(this->one_2 + this->one, this->two);
		EXPECT_EQ(this->one + this->one_2, this->two);
	}

	TEST_F(AlgebraIntNumberTest, Subtraction)
	{
		EXPECT_EQ(this->minus_two - this->minus_one, this->minus_one_3);
		EXPECT_EQ(this->minus_one - this->minus_two, this->one_3);

		EXPECT_EQ(this->minus_one - this->zero_0, this->minus_one_3);
		EXPECT_EQ(this->zero_0 - this->minus_one, this->one_3);

		EXPECT_EQ(this->minus_one_3 - this->one, this->minus_two);
		EXPECT_EQ(this->one - this->minus_one_3, this->two);

		EXPECT_EQ(this->one_3 - this->zero, this->one_2);
		EXPECT_EQ(this->zero - this->one_3, this->minus_one_1);

		EXPECT_EQ(this->one_2 - this->one, this->zero);
		EXPECT_EQ(this->one - this->one_2, this->zero_3);
	}

	TEST_F(AlgebraIntNumberTest, Multiplication)
	{
		EXPECT_EQ(this->zero * this->minus_one, this->zero);
		EXPECT_EQ(this->minus_one * this->zero, this->zero);

		EXPECT_EQ(this->zero_0 * this->zero_2, this->zero_0);
		EXPECT_EQ(this->zero_2 * this->zero_0, this->zero_0);

		EXPECT_EQ(this->zero_0 * this->one_3, this->zero_1);
		EXPECT_EQ(this->one_3 * this->zero_0, this->zero_1);

		EXPECT_EQ(this->minus_six * this->minus_two, this->twelve);
		EXPECT_EQ(this->minus_two * this->minus_six, this->twelve);

		EXPECT_EQ(this->minus_six * this->two, this->minus_twelve);
		EXPECT_EQ(this->two * this->minus_six, this->minus_twelve);

		EXPECT_EQ(this->six * this->minus_two, this->minus_twelve);
		EXPECT_EQ(this->minus_two * this->six, this->minus_twelve);

		EXPECT_EQ(this->six * this->two, this->twelve);
		EXPECT_EQ(this->two * this->six, this->twelve);
	}

	TEST_F(AlgebraIntNumberTest, Division)
	{
		EXPECT_EQ(this->zero / this->minus_two, this->zero);
		EXPECT_EQ(this->zero / this->minus_one, this->zero);
		EXPECT_THROW(this->minus_one / this->zero, std::domain_error);
		EXPECT_EQ(this->zero / this->one, this->zero);
		EXPECT_EQ(this->zero / this->two, this->zero);

		EXPECT_EQ(this->zero_0 / this->one_3, this->zero_1);

		EXPECT_EQ(this->minus_twelve / this->minus_three, this->four);
		EXPECT_EQ(this->minus_twelve / this->minus_two, this->six);
		EXPECT_EQ(this->minus_seven / this->minus_two, this->three);
		EXPECT_EQ(this->minus_six / this->minus_two, this->three);
		EXPECT_EQ(this->minus_two / this->minus_six, this->zero);
		EXPECT_EQ(this->minus_two / this->minus_seven, this->zero);
		EXPECT_EQ(this->minus_two / this->minus_twelve, this->zero);
		EXPECT_EQ(this->minus_three / this->minus_twelve, this->zero);

		EXPECT_EQ(this->minus_twelve / this->three, this->minus_four);
		EXPECT_EQ(this->minus_twelve / this->two, this->minus_six);
		EXPECT_EQ(this->minus_seven / this->two, this->minus_three);
		EXPECT_EQ(this->minus_six / this->two, this->minus_three);
		EXPECT_EQ(this->two / this->minus_six, this->zero);
		EXPECT_EQ(this->two / this->minus_seven, this->zero);
		EXPECT_EQ(this->two / this->minus_twelve, this->zero);
		EXPECT_EQ(this->three / this->minus_twelve, this->zero);

		EXPECT_EQ(this->twelve / this->minus_three, this->minus_four);
		EXPECT_EQ(this->twelve / this->minus_two, this->minus_six);
		EXPECT_EQ(this->seven / this->minus_two, this->minus_three);
		EXPECT_EQ(this->six / this->minus_two, this->minus_three);
		EXPECT_EQ(this->minus_two / this->six, this->zero);
		EXPECT_EQ(this->minus_two / this->seven, this->zero);
		EXPECT_EQ(this->minus_two / this->twelve, this->zero);
		EXPECT_EQ(this->minus_three / this->twelve, this->zero);

		EXPECT_EQ(this->twelve / this->three, this->four);
		EXPECT_EQ(this->twelve / this->two, this->six);
		EXPECT_EQ(this->seven / this->two, this->three);
		EXPECT_EQ(this->six / this->two, this->three);
		EXPECT_EQ(this->two / this->six, this->zero);
		EXPECT_EQ(this->two / this->seven, this->zero);
		EXPECT_EQ(this->two / this->twelve, this->zero);
		EXPECT_EQ(this->three / this->twelve, this->zero);
	}

	TEST_F(AlgebraIntNumberTest, Shiftiness)
	{
		EXPECT_EQ(this->minus_twelve << 0, std::intmax_t{-12});
		EXPECT_EQ(this->minus_twelve << 1, std::intmax_t{-24});
		EXPECT_EQ(this->minus_twelve << 2, std::intmax_t{-48});
		EXPECT_EQ(this->minus_twelve << 3, std::intmax_t{-96});

		EXPECT_EQ(this->minus_twelve >> 0, std::intmax_t{-12});
		EXPECT_EQ(this->minus_twelve >> 1, std::intmax_t{-6});
		EXPECT_EQ(this->minus_twelve >> 2, std::intmax_t{-3});
		EXPECT_EQ(this->minus_twelve >> 3, std::intmax_t{-2});
		EXPECT_EQ(this->minus_twelve >> 4, std::intmax_t{-1});
		EXPECT_EQ(this->minus_twelve >> 5, std::intmax_t{-1});

		EXPECT_EQ(this->minus_one_1 << 0, std::intmax_t{-1});
		EXPECT_EQ(this->minus_one_1 << 1, std::intmax_t{-2});
		EXPECT_EQ(this->minus_one_1 << 2, std::intmax_t{-4});
		EXPECT_EQ(this->minus_one_1 << 3, std::intmax_t{-8});

		EXPECT_EQ(this->minus_one_1 >> 0, std::intmax_t{-1});
		EXPECT_EQ(this->minus_one_1 >> 1, std::intmax_t{-1});
		EXPECT_EQ(this->minus_one_1 >> 2, std::intmax_t{-1});
		EXPECT_EQ(this->minus_one_1 >> 3, std::intmax_t{-1});
		EXPECT_EQ(this->minus_one_1 >> 4, std::intmax_t{-1});
		EXPECT_EQ(this->minus_one_1 >> 5, std::intmax_t{-1});

		EXPECT_EQ(this->zero_0 << 0, std::intmax_t{});
		EXPECT_EQ(this->zero_0 << 1, std::intmax_t{});
		EXPECT_EQ(this->zero_0 << 2, std::intmax_t{});
		EXPECT_EQ(this->zero_0 << 3, std::intmax_t{});

		EXPECT_EQ(this->zero_0 >> 0, std::intmax_t{});
		EXPECT_EQ(this->zero_0 >> 1, std::intmax_t{});
		EXPECT_EQ(this->zero_0 >> 2, std::intmax_t{});
		EXPECT_EQ(this->zero_0 >> 3, std::intmax_t{});

		EXPECT_EQ(this->twelve << 0, std::intmax_t{12});
		EXPECT_EQ(this->twelve << 1, std::intmax_t{24});
		EXPECT_EQ(this->twelve << 2, std::intmax_t{48});
		EXPECT_EQ(this->twelve << 3, std::intmax_t{96});

		EXPECT_EQ(this->twelve >> 0, std::intmax_t{12});
		EXPECT_EQ(this->twelve >> 1, std::intmax_t{6});
		EXPECT_EQ(this->twelve >> 2, std::intmax_t{3});
		EXPECT_EQ(this->twelve >> 3, std::intmax_t{1});
		EXPECT_EQ(this->twelve >> 4, std::intmax_t{});
		EXPECT_EQ(this->twelve >> 5, std::intmax_t{});

		EXPECT_EQ(this->one_2 << 0, std::intmax_t{1});
		EXPECT_EQ(this->one_2 << 1, std::intmax_t{2});
		EXPECT_EQ(this->one_2 << 2, std::intmax_t{4});
		EXPECT_EQ(this->one_2 << 3, std::intmax_t{8});

		EXPECT_EQ(this->one_2 >> 0, std::intmax_t{1});
		EXPECT_EQ(this->one_2 >> 1, std::intmax_t{});
		EXPECT_EQ(this->one_2 >> 2, std::intmax_t{});
		EXPECT_EQ(this->one_2 >> 3, std::intmax_t{});
		EXPECT_EQ(this->one_2 >> 4, std::intmax_t{});
		EXPECT_EQ(this->one_2 >> 5, std::intmax_t{});

		EXPECT_EQ(this->minus_twelve << -1, std::intmax_t{-6});
		EXPECT_EQ(this->minus_twelve << -2, std::intmax_t{-3});
		EXPECT_EQ(this->minus_twelve << -3, std::intmax_t{-2});
		EXPECT_EQ(this->minus_twelve << -4, std::intmax_t{-1});
		EXPECT_EQ(this->minus_twelve << -5, std::intmax_t{-1});

		EXPECT_EQ(this->minus_twelve >> -1, std::intmax_t{-24});
		EXPECT_EQ(this->minus_twelve >> -2, std::intmax_t{-48});
		EXPECT_EQ(this->minus_twelve >> -3, std::intmax_t{-96});
		EXPECT_EQ(this->minus_twelve >> -4, std::intmax_t{-192});

		EXPECT_EQ(this->minus_one_1 << -1, std::intmax_t{-1});
		EXPECT_EQ(this->minus_one_1 << -2, std::intmax_t{-1});
		EXPECT_EQ(this->minus_one_1 << -3, std::intmax_t{-1});

		EXPECT_EQ(this->minus_one_1 >> -1, std::intmax_t{-2});
		EXPECT_EQ(this->minus_one_1 >> -2, std::intmax_t{-4});
		EXPECT_EQ(this->minus_one_1 >> -3, std::intmax_t{-8});
		EXPECT_EQ(this->minus_one_1 >> -4, std::intmax_t{-16});
		EXPECT_EQ(this->minus_one_1 >> -5, std::intmax_t{-32});

		EXPECT_EQ(this->zero_0 << -1, std::intmax_t{});
		EXPECT_EQ(this->zero_0 << -2, std::intmax_t{});
		EXPECT_EQ(this->zero_0 << -3, std::intmax_t{});

		EXPECT_EQ(this->zero_0 >> -1, std::intmax_t{});
		EXPECT_EQ(this->zero_0 >> -2, std::intmax_t{});
		EXPECT_EQ(this->zero_0 >> -3, std::intmax_t{});

		EXPECT_EQ(this->twelve << -1, std::intmax_t{6});
		EXPECT_EQ(this->twelve << -2, std::intmax_t{3});
		EXPECT_EQ(this->twelve << -3, std::intmax_t{1});
		EXPECT_EQ(this->twelve << -4, std::intmax_t{});
		EXPECT_EQ(this->twelve << -5, std::intmax_t{});

		EXPECT_EQ(this->twelve >> -1, std::intmax_t{24});
		EXPECT_EQ(this->twelve >> -2, std::intmax_t{48});
		EXPECT_EQ(this->twelve >> -3, std::intmax_t{96});
		EXPECT_EQ(this->twelve >> -4, std::intmax_t{192});

		EXPECT_EQ(this->one_2 << -1, std::intmax_t{});
		EXPECT_EQ(this->one_2 << -2, std::intmax_t{});
		EXPECT_EQ(this->one_2 << -3, std::intmax_t{});

		EXPECT_EQ(this->one_2 >> -1, std::intmax_t{2});
		EXPECT_EQ(this->one_2 >> -2, std::intmax_t{4});
		EXPECT_EQ(this->one_2 >> -3, std::intmax_t{8});
		EXPECT_EQ(this->one_2 >> -4, std::intmax_t{16});
		EXPECT_EQ(this->one_2 >> -5, std::intmax_t{32});
	}

	TEST_F(AlgebraIntNumberTest, BitwiseOperatorAnd)
	{
		EXPECT_EQ(this->minus_twelve & this->zero, this->zero);
		EXPECT_EQ(this->minus_three & this->zero, this->zero);
		EXPECT_EQ(this->zero & this->zero, this->zero);
		EXPECT_EQ(this->three & this->zero, this->zero);
		EXPECT_EQ(this->twelve & this->zero, this->zero);

		EXPECT_EQ(this->zero & this->minus_twelve, this->zero);
		EXPECT_EQ(this->zero & this->minus_three, this->zero);
		EXPECT_EQ(this->zero & this->three, this->zero);
		EXPECT_EQ(this->zero & this->twelve, this->zero);

		EXPECT_EQ(this->minus_twelve & this->minus_six, this->minus_sixteen);
		EXPECT_EQ(this->minus_twelve & this->minus_one_1, this->minus_twelve);
		EXPECT_EQ(this->minus_twelve & this->twelve, this->four);
		EXPECT_EQ(this->minus_six & this->three, this->two);
		EXPECT_EQ(this->twelve & this->three, this->zero);
		EXPECT_EQ(this->three & this->one, this->one);

		EXPECT_EQ(this->minus_six & this->minus_twelve, this->minus_sixteen);
		EXPECT_EQ(this->minus_one_1 & this->minus_twelve, this->minus_twelve);
		EXPECT_EQ(this->twelve & this->minus_twelve, this->four);
		EXPECT_EQ(this->three & this->minus_six, this->two);
		EXPECT_EQ(this->three & this->twelve, this->zero);
		EXPECT_EQ(this->one & this->three, this->one);
	}

	TEST_F(AlgebraIntNumberTest, BitwiseOperatorOr)
	{
		EXPECT_EQ(this->minus_twelve | this->zero, this->minus_twelve);
		EXPECT_EQ(this->minus_three | this->zero, this->minus_three);
		EXPECT_EQ(this->zero | this->zero, this->zero);
		EXPECT_EQ(this->three | this->zero, this->three);
		EXPECT_EQ(this->twelve | this->zero, this->twelve);

		EXPECT_EQ(this->zero | this->minus_twelve, this->minus_twelve);
		EXPECT_EQ(this->zero | this->minus_three, this->minus_three);
		EXPECT_EQ(this->zero | this->three, this->three);
		EXPECT_EQ(this->zero | this->twelve, this->twelve);

		EXPECT_EQ(this->minus_twelve | this->minus_six, this->minus_two);
		EXPECT_EQ(this->minus_twelve | this->minus_one_1, this->minus_one);
		EXPECT_EQ(this->minus_twelve | this->twelve, this->minus_four);
		EXPECT_EQ(this->minus_six | this->three, this->minus_five);
		EXPECT_EQ(this->twelve | this->three, this->fifteen);
		EXPECT_EQ(this->three | this->one, this->three);

		EXPECT_EQ(this->minus_six | this->minus_twelve, this->minus_two);
		EXPECT_EQ(this->minus_one_1 | this->minus_twelve, this->minus_one);
		EXPECT_EQ(this->twelve | this->minus_twelve, this->minus_four);
		EXPECT_EQ(this->three | this->minus_six, this->minus_five);
		EXPECT_EQ(this->three | this->twelve, this->fifteen);
		EXPECT_EQ(this->one | this->three, this->three);
	}

	TEST_F(AlgebraIntNumberTest, BitwiseOperatorNot)
	{
		EXPECT_EQ(~this->min, this->max);
		EXPECT_EQ(~this->minus_sixteen, this->fifteen);
		EXPECT_EQ(~this->minus_twelve, this->eleven);
		EXPECT_EQ(~this->minus_six, this->five);
		EXPECT_EQ(~this->minus_five, this->four);
		EXPECT_EQ(~this->minus_four, this->three);
		EXPECT_EQ(~this->minus_three, this->two);
		EXPECT_EQ(~this->minus_two, this->one);
		EXPECT_EQ(~this->minus_one, this->zero);
		EXPECT_EQ(~this->zero, this->minus_one);
		EXPECT_EQ(~this->one, this->minus_two);
		EXPECT_EQ(~this->two, this->minus_three);
		EXPECT_EQ(~this->three, this->minus_four);
		EXPECT_EQ(~this->four, this->minus_five);
		EXPECT_EQ(~this->five, this->minus_six);
		EXPECT_EQ(~this->fifteen, this->minus_sixteen);
		EXPECT_EQ(~this->max, this->min);
	}

	TEST_F(AlgebraIntNumberTest, Lean)
	{
		auto mutableMin = this->min;
		auto mutableMinusTwo = this->minus_two;
		auto mutableMinusOne = this->minus_one;
		auto mutableZero = this->zero;
		auto mutableOne = this->one;
		auto mutableMax = this->max;

		mutableMin.lean();
		mutableMinusTwo.lean();
		mutableMinusOne.lean();
		mutableZero.lean();
		mutableOne.lean();
		mutableMax.lean();

		EXPECT_EQ(mutableMin.container(), this->min.container());
		EXPECT_EQ(mutableMinusTwo.container(), this->minus_two_2.container());
		EXPECT_EQ(mutableMinusOne.container(), this->minus_one_1.container());
		EXPECT_EQ(mutableZero.container(), this->zero_1.container());
		EXPECT_EQ(mutableOne.container(), this->one_2.container());
		EXPECT_EQ(mutableMax.container(), this->max.container());
	}

	TEST_F(AlgebraIntNumberTest, Formatting)
	{
		EXPECT_EQ(this->min.format(), sizeof(std::intmax_t) == 8 ? "0X8000000000000000" : "0X80000000");
		EXPECT_EQ(this->minus_sixteen.format(), sizeof(int) == 8 ? "0XFFFFFFFFFFFFFFF0" : "0XFFFFFFF0");
		EXPECT_EQ(this->minus_twelve.format(), sizeof(int) == 8 ? "0XFFFFFFFFFFFFFFF4" : "0XFFFFFFF4");
		EXPECT_EQ(this->minus_seven.format(), sizeof(int) == 8 ? "0XFFFFFFFFFFFFFFF9" : "0XFFFFFFF9");
		EXPECT_EQ(this->minus_six.format(), sizeof(int) == 8 ? "0XFFFFFFFFFFFFFFFA" : "0XFFFFFFFA");
		EXPECT_EQ(this->minus_five.format(), sizeof(int) == 8 ? "0XFFFFFFFFFFFFFFFB" : "0XFFFFFFFB");
		EXPECT_EQ(this->minus_four.format(), sizeof(int) == 8 ? "0XFFFFFFFFFFFFFFFC" : "0XFFFFFFFC");
		EXPECT_EQ(this->minus_three.format(), sizeof(int) == 8 ? "0XFFFFFFFFFFFFFFFD" : "0XFFFFFFFD");
		EXPECT_EQ(this->minus_two.format(), sizeof(int) == 8 ? "0XFFFFFFFFFFFFFFFE" : "0XFFFFFFFE");
		EXPECT_EQ(this->minus_one.format(), sizeof(int) == 8 ? "0XFFFFFFFFFFFFFFFF" : "0XFFFFFFFF");
		EXPECT_EQ(this->zero.format(), sizeof(int) == 8 ? "0X0000000000000000" : "0X00000000");
		EXPECT_EQ(this->one.format(), sizeof(int) == 8 ? "0X0000000000000001" : "0X00000001");
		EXPECT_EQ(this->two.format(), sizeof(int) == 8 ? "0X0000000000000002" : "0X00000002");
		EXPECT_EQ(this->three.format(), sizeof(int) == 8 ? "0X0000000000000003" : "0X00000003");
		EXPECT_EQ(this->four.format(), sizeof(int) == 8 ? "0X0000000000000004" : "0X00000004");
		EXPECT_EQ(this->five.format(), sizeof(int) == 8 ? "0X0000000000000005" : "0X00000005");
		EXPECT_EQ(this->six.format(), sizeof(int) == 8 ? "0X0000000000000006" : "0X00000006");
		EXPECT_EQ(this->seven.format(), sizeof(int) == 8 ? "0X0000000000000007" : "0X00000007");
		EXPECT_EQ(this->eleven.format(), sizeof(int) == 8 ? "0X000000000000000B" : "0X0000000B");
		EXPECT_EQ(this->twelve.format(), sizeof(int) == 8 ? "0X000000000000000C" : "0X0000000C");
		EXPECT_EQ(this->fifteen.format(), sizeof(int) == 8 ? "0X000000000000000F" : "0X0000000F");
		EXPECT_EQ(this->max.format(), sizeof(std::intmax_t) == 8 ? "0X7FFFFFFFFFFFFFFF" : "0X7FFFFFFF");
		EXPECT_EQ(this->minus_two_2.format(), "0XFE");
		EXPECT_EQ(this->minus_one_1.format(), "0XFF");
		EXPECT_EQ(this->minus_one_2.format(), "0XFF");
		EXPECT_EQ(this->minus_one_3.format(), "0XFF");
		EXPECT_EQ(this->zero_0.format(), "0X00");
		EXPECT_EQ(this->zero_1.format(), "0X00");
		EXPECT_EQ(this->zero_2.format(), "0X00");
		EXPECT_EQ(this->zero_3.format(), "0X00");
		EXPECT_EQ(this->one_2.format(), "0X01");
		EXPECT_EQ(this->one_3.format(), "0X01");
	}
}
