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

import <concepts>;

import xablau.algebra;
import xablau.testing;

namespace xablau::unit_testing
{
	template <
		std::floating_point Type,
		xablau::algebra::concepts::complex_ordering Ordering >
	struct AlgebraComplexTypeDefinitions
	{
		using value_type = Type;
		using ordering = Ordering;
	};

	template < typename Types >
	class AlgebraComplexTest : public ::testing::Test
	{
	public:
		static constexpr xablau::algebra::complex < typename Types::value_type, typename Types::ordering > number1{ 0, 0 };
		static constexpr xablau::algebra::complex < typename Types::value_type, typename Types::ordering > number2{ 0, 1 };
		static constexpr xablau::algebra::complex < typename Types::value_type, typename Types::ordering > number3{ 1, 0 };
		static constexpr xablau::algebra::complex < typename Types::value_type, typename Types::ordering > number4{ 1, 1 };
		static constexpr xablau::algebra::complex < typename Types::value_type, typename Types::ordering > number5{ 2, 2 };

		static constexpr std::complex < typename Types::value_type > stdNumber1{ 0, 0 };
		static constexpr std::complex < typename Types::value_type > stdNumber2{ 0, 1 };
		static constexpr std::complex < typename Types::value_type > stdNumber3{ 1, 0 };
		static constexpr std::complex < typename Types::value_type > stdNumber4{ 1, 1 };
		static constexpr std::complex < typename Types::value_type > stdNumber5{ 2, 2 };
	};

	TYPED_TEST_SUITE_P(AlgebraComplexTest);

	TYPED_TEST_P(AlgebraComplexTest, ComparisonOperators)
	{
		EXPECT_EQ(this->number1, this->number1);
		EXPECT_NE(this->number1, this->number2);
		EXPECT_NE(this->number1, this->number3);
		EXPECT_NE(this->number1, this->number4);

		EXPECT_TRUE(this->number1 <= this->number1);
		EXPECT_TRUE(this->number1 >= this->number1);

		EXPECT_EQ(this->number1, this->stdNumber1);
		EXPECT_NE(this->number1, this->stdNumber2);
		EXPECT_NE(this->number1, this->stdNumber3);
		EXPECT_NE(this->number1, this->stdNumber4);

		if constexpr (
			TypeParam::ordering::ordering() == xablau::algebra::complex_ordering_value::real ||
			TypeParam::ordering::ordering() == xablau::algebra::complex_ordering_value::imaginary)
		{
			EXPECT_TRUE(this->number1 < this->number2);
			EXPECT_TRUE(this->number1 < this->number3);

			EXPECT_FALSE(this->number1 > this->number2);
			EXPECT_FALSE(this->number1 > this->number3);
		}

		else if constexpr (TypeParam::ordering::ordering() == xablau::algebra::complex_ordering_value::radius)
		{
			EXPECT_TRUE(this->number1 < this->number2);
			EXPECT_TRUE(this->number3 < this->number2);

			EXPECT_FALSE(this->number1 > this->number2);
			EXPECT_FALSE(this->number3 > this->number2);
		}

		else if constexpr (TypeParam::ordering::ordering() == xablau::algebra::complex_ordering_value::theta)
		{
			EXPECT_TRUE(this->number4 < this->number5);
			EXPECT_TRUE(this->number3 < this->number2);

			EXPECT_FALSE(this->number4 > this->number5);
			EXPECT_FALSE(this->number3 > this->number2);
		}

		else
		{
			throw std::runtime_error("Unkown ordering value_type.");
		}
	}

	TYPED_TEST_P(AlgebraComplexTest, MemberFunctions)
	{
		xablau::algebra::complex < typename TypeParam::value_type, typename TypeParam::ordering > number = this->number2;
		EXPECT_EQ(number, this->number2);

		number += this->number3;
		EXPECT_EQ(number, this->number4);

		number *= typename TypeParam::value_type{2};
		EXPECT_EQ(number, this->number5);

		number /= typename TypeParam::value_type{2};
		EXPECT_EQ(number, this->number4);

		number -= this->number3;
		EXPECT_EQ(number, this->number2);

		EXPECT_EQ(number.real(), this->number2.number.real());
		EXPECT_EQ(number.imag(), this->number2.number.imag());

		number.real(typename TypeParam::value_type{13});
		number.imag(typename TypeParam::value_type{17});

		EXPECT_EQ(number.real(), typename TypeParam::value_type{13});
		EXPECT_EQ(number.imag(), typename TypeParam::value_type{17});
	}

	TYPED_TEST_P(AlgebraComplexTest, NonMemberFunctions)
	{
		EXPECT_EQ(+this->number1, +this->stdNumber1);
		EXPECT_EQ(+this->number2, +this->stdNumber2);
		EXPECT_EQ(+this->number3, +this->stdNumber3);
		EXPECT_EQ(+this->number4, +this->stdNumber4);
		EXPECT_EQ(+this->number5, +this->stdNumber5);

		EXPECT_EQ(-this->number1, -this->stdNumber1);
		EXPECT_EQ(-this->number2, -this->stdNumber2);
		EXPECT_EQ(-this->number3, -this->stdNumber3);
		EXPECT_EQ(-this->number4, -this->stdNumber4);
		EXPECT_EQ(-this->number5, -this->stdNumber5);

		EXPECT_EQ(this->number1 + this->number2, this->stdNumber1 + this->stdNumber2);
		EXPECT_EQ(this->number2 + this->number3, this->stdNumber2 + this->stdNumber3);
		EXPECT_EQ(this->number3 + this->number4, this->stdNumber3 + this->stdNumber4);
		EXPECT_EQ(this->number4 + this->number5, this->stdNumber4 + this->stdNumber5);
		EXPECT_EQ(this->number5 + this->number1, this->stdNumber5 + this->stdNumber1);

		EXPECT_EQ(this->number1 + typename TypeParam::value_type{2}, this->stdNumber1 + typename TypeParam::value_type{2});
		EXPECT_EQ(this->number2 + typename TypeParam::value_type{3}, this->stdNumber2 + typename TypeParam::value_type{3});
		EXPECT_EQ(this->number3 + typename TypeParam::value_type{4}, this->stdNumber3 + typename TypeParam::value_type{4});
		EXPECT_EQ(this->number4 + typename TypeParam::value_type{5}, this->stdNumber4 + typename TypeParam::value_type{5});
		EXPECT_EQ(this->number5 + typename TypeParam::value_type{1}, this->stdNumber5 + typename TypeParam::value_type{1});

		EXPECT_EQ(this->number1 - this->number2, this->stdNumber1 - this->stdNumber2);
		EXPECT_EQ(this->number2 - this->number3, this->stdNumber2 - this->stdNumber3);
		EXPECT_EQ(this->number3 - this->number4, this->stdNumber3 - this->stdNumber4);
		EXPECT_EQ(this->number4 - this->number5, this->stdNumber4 - this->stdNumber5);
		EXPECT_EQ(this->number5 - this->number1, this->stdNumber5 - this->stdNumber1);

		EXPECT_EQ(this->number1 - typename TypeParam::value_type{2}, this->stdNumber1 - typename TypeParam::value_type{2});
		EXPECT_EQ(this->number2 - typename TypeParam::value_type{3}, this->stdNumber2 - typename TypeParam::value_type{3});
		EXPECT_EQ(this->number3 - typename TypeParam::value_type{4}, this->stdNumber3 - typename TypeParam::value_type{4});
		EXPECT_EQ(this->number4 - typename TypeParam::value_type{5}, this->stdNumber4 - typename TypeParam::value_type{5});
		EXPECT_EQ(this->number5 - typename TypeParam::value_type{1}, this->stdNumber5 - typename TypeParam::value_type{1});

		EXPECT_EQ(this->number1 * this->number2, this->stdNumber1 * this->stdNumber2);
		EXPECT_EQ(this->number2 * this->number3, this->stdNumber2 * this->stdNumber3);
		EXPECT_EQ(this->number3 * this->number4, this->stdNumber3 * this->stdNumber4);
		EXPECT_EQ(this->number4 * this->number5, this->stdNumber4 * this->stdNumber5);
		EXPECT_EQ(this->number5 * this->number1, this->stdNumber5 * this->stdNumber1);

		EXPECT_EQ(this->number1 * typename TypeParam::value_type{2}, this->stdNumber1 * typename TypeParam::value_type{2});
		EXPECT_EQ(this->number2 * typename TypeParam::value_type{3}, this->stdNumber2 * typename TypeParam::value_type{3});
		EXPECT_EQ(this->number3 * typename TypeParam::value_type{4}, this->stdNumber3 * typename TypeParam::value_type{4});
		EXPECT_EQ(this->number4 * typename TypeParam::value_type{5}, this->stdNumber4 * typename TypeParam::value_type{5});
		EXPECT_EQ(this->number5 * typename TypeParam::value_type{1}, this->stdNumber5 * typename TypeParam::value_type{1});

		EXPECT_EQ(this->number1 / this->number2, this->stdNumber1 / this->stdNumber2);
		EXPECT_EQ(this->number2 / this->number3, this->stdNumber2 / this->stdNumber3);
		EXPECT_EQ(this->number3 / this->number4, this->stdNumber3 / this->stdNumber4);
		EXPECT_EQ(this->number4 / this->number5, this->stdNumber4 / this->stdNumber5);

		EXPECT_EQ(this->number1 / typename TypeParam::value_type{2}, this->stdNumber1 / typename TypeParam::value_type{2});
		EXPECT_EQ(this->number2 / typename TypeParam::value_type{3}, this->stdNumber2 / typename TypeParam::value_type{3});
		EXPECT_EQ(this->number3 / typename TypeParam::value_type{4}, this->stdNumber3 / typename TypeParam::value_type{4});
		EXPECT_EQ(this->number4 / typename TypeParam::value_type{5}, this->stdNumber4 / typename TypeParam::value_type{5});

		EXPECT_EQ(xablau::algebra::real(this->number1), std::real(this->stdNumber1));
		EXPECT_EQ(xablau::algebra::real(this->number2), std::real(this->stdNumber2));
		EXPECT_EQ(xablau::algebra::real(this->number3), std::real(this->stdNumber3));
		EXPECT_EQ(xablau::algebra::real(this->number4), std::real(this->stdNumber4));
		EXPECT_EQ(xablau::algebra::real(this->number5), std::real(this->stdNumber5));

		EXPECT_EQ(xablau::algebra::imag(this->number1), std::imag(this->stdNumber1));
		EXPECT_EQ(xablau::algebra::imag(this->number2), std::imag(this->stdNumber2));
		EXPECT_EQ(xablau::algebra::imag(this->number3), std::imag(this->stdNumber3));
		EXPECT_EQ(xablau::algebra::imag(this->number4), std::imag(this->stdNumber4));
		EXPECT_EQ(xablau::algebra::imag(this->number5), std::imag(this->stdNumber5));

		EXPECT_EQ(xablau::algebra::abs(this->number1), std::abs(this->stdNumber1));
		EXPECT_EQ(xablau::algebra::abs(this->number2), std::abs(this->stdNumber2));
		EXPECT_EQ(xablau::algebra::abs(this->number3), std::abs(this->stdNumber3));
		EXPECT_EQ(xablau::algebra::abs(this->number4), std::abs(this->stdNumber4));
		EXPECT_EQ(xablau::algebra::abs(this->number5), std::abs(this->stdNumber5));

		EXPECT_EQ(xablau::algebra::arg(this->number1), std::arg(this->stdNumber1));
		EXPECT_EQ(xablau::algebra::arg(this->number2), std::arg(this->stdNumber2));
		EXPECT_EQ(xablau::algebra::arg(this->number3), std::arg(this->stdNumber3));
		EXPECT_EQ(xablau::algebra::arg(this->number4), std::arg(this->stdNumber4));
		EXPECT_EQ(xablau::algebra::arg(this->number5), std::arg(this->stdNumber5));

		EXPECT_EQ(xablau::algebra::norm(this->number1), std::norm(this->stdNumber1));
		EXPECT_EQ(xablau::algebra::norm(this->number2), std::norm(this->stdNumber2));
		EXPECT_EQ(xablau::algebra::norm(this->number3), std::norm(this->stdNumber3));
		EXPECT_EQ(xablau::algebra::norm(this->number4), std::norm(this->stdNumber4));
		EXPECT_EQ(xablau::algebra::norm(this->number5), std::norm(this->stdNumber5));

		EXPECT_EQ(xablau::algebra::conj(this->number1), std::conj(this->stdNumber1));
		EXPECT_EQ(xablau::algebra::conj(this->number2), std::conj(this->stdNumber2));
		EXPECT_EQ(xablau::algebra::conj(this->number3), std::conj(this->stdNumber3));
		EXPECT_EQ(xablau::algebra::conj(this->number4), std::conj(this->stdNumber4));
		EXPECT_EQ(xablau::algebra::conj(this->number5), std::conj(this->stdNumber5));

		EXPECT_EQ(xablau::algebra::proj(this->number1), std::proj(this->stdNumber1));
		EXPECT_EQ(xablau::algebra::proj(this->number2), std::proj(this->stdNumber2));
		EXPECT_EQ(xablau::algebra::proj(this->number3), std::proj(this->stdNumber3));
		EXPECT_EQ(xablau::algebra::proj(this->number4), std::proj(this->stdNumber4));
		EXPECT_EQ(xablau::algebra::proj(this->number5), std::proj(this->stdNumber5));

		EXPECT_EQ(
			xablau::algebra::polar < typename TypeParam::ordering > (typename TypeParam::value_type{1}, typename TypeParam::value_type{2}),
			std::polar(typename TypeParam::value_type{1}, typename TypeParam::value_type{2}));
		EXPECT_EQ(
			xablau::algebra::polar < typename TypeParam::ordering > (typename TypeParam::value_type{2}, typename TypeParam::value_type{3}),
			std::polar(typename TypeParam::value_type{2}, typename TypeParam::value_type{3}));
		EXPECT_EQ(
			xablau::algebra::polar < typename TypeParam::ordering > (typename TypeParam::value_type{3}, typename TypeParam::value_type{4}),
			std::polar(typename TypeParam::value_type{3}, typename TypeParam::value_type{4}));
		EXPECT_EQ(
			xablau::algebra::polar < typename TypeParam::ordering > (typename TypeParam::value_type{4}, typename TypeParam::value_type{5}),
			std::polar(typename TypeParam::value_type{4}, typename TypeParam::value_type{5}));
		EXPECT_EQ(
			xablau::algebra::polar < typename TypeParam::ordering > (typename TypeParam::value_type{5}, typename TypeParam::value_type{1}),
			std::polar(typename TypeParam::value_type{5}, typename TypeParam::value_type{1}));
	}

	TYPED_TEST_P(AlgebraComplexTest, ExponentialFunctions)
	{
		EXPECT_EQ(xablau::algebra::exp(this->number1), std::exp(this->stdNumber1));
		EXPECT_EQ(xablau::algebra::exp(this->number2), std::exp(this->stdNumber2));
		EXPECT_EQ(xablau::algebra::exp(this->number3), std::exp(this->stdNumber3));
		EXPECT_EQ(xablau::algebra::exp(this->number4), std::exp(this->stdNumber4));
		EXPECT_EQ(xablau::algebra::exp(this->number5), std::exp(this->stdNumber5));

		EXPECT_EQ(xablau::algebra::log(this->number1), std::log(this->stdNumber1));
		EXPECT_EQ(xablau::algebra::log(this->number2), std::log(this->stdNumber2));
		EXPECT_EQ(xablau::algebra::log(this->number3), std::log(this->stdNumber3));
		EXPECT_EQ(xablau::algebra::log(this->number4), std::log(this->stdNumber4));
		EXPECT_EQ(xablau::algebra::log(this->number5), std::log(this->stdNumber5));

		EXPECT_EQ(xablau::algebra::log10(this->number1), std::log10(this->stdNumber1));
		EXPECT_EQ(xablau::algebra::log10(this->number2), std::log10(this->stdNumber2));
		EXPECT_EQ(xablau::algebra::log10(this->number3), std::log10(this->stdNumber3));
		EXPECT_EQ(xablau::algebra::log10(this->number4), std::log10(this->stdNumber4));
		EXPECT_EQ(xablau::algebra::log10(this->number5), std::log10(this->stdNumber5));
	}

	TYPED_TEST_P(AlgebraComplexTest, PowerFunctions)
	{
		EXPECT_EQ(xablau::algebra::pow(this->number1, this->number1), std::pow(this->stdNumber1, this->stdNumber1));
		EXPECT_EQ(xablau::algebra::pow(this->number2, this->number2), std::pow(this->stdNumber2, this->stdNumber2));
		EXPECT_EQ(xablau::algebra::pow(this->number3, this->number3), std::pow(this->stdNumber3, this->stdNumber3));
		EXPECT_EQ(xablau::algebra::pow(this->number4, this->number4), std::pow(this->stdNumber4, this->stdNumber4));
		EXPECT_EQ(xablau::algebra::pow(this->number5, this->number5), std::pow(this->stdNumber5, this->stdNumber5));

		EXPECT_EQ(
			xablau::algebra::pow(this->number1, typename TypeParam::value_type{1}),
			std::pow(this->stdNumber1, typename TypeParam::value_type{1}));
		EXPECT_EQ(
			xablau::algebra::pow(this->number2, typename TypeParam::value_type{2}),
			std::pow(this->stdNumber2, typename TypeParam::value_type{2}));
		EXPECT_EQ(
			xablau::algebra::pow(this->number3, typename TypeParam::value_type{3}),
			std::pow(this->stdNumber3, typename TypeParam::value_type{3}));
		EXPECT_EQ(
			xablau::algebra::pow(this->number4, typename TypeParam::value_type{4}),
			std::pow(this->stdNumber4, typename TypeParam::value_type{4}));
		EXPECT_EQ(
			xablau::algebra::pow(this->number5, typename TypeParam::value_type{5}),
			std::pow(this->stdNumber5, typename TypeParam::value_type{5}));

		EXPECT_EQ(
			std::pow(typename TypeParam::value_type{1}, this->stdNumber1),
			xablau::algebra::pow(typename TypeParam::value_type{1}, this->number1));
		EXPECT_EQ(
			std::pow(typename TypeParam::value_type{2}, this->stdNumber2),
			xablau::algebra::pow(typename TypeParam::value_type{2}, this->number2));
		EXPECT_EQ(
			std::pow(typename TypeParam::value_type{3}, this->stdNumber3),
			xablau::algebra::pow(typename TypeParam::value_type{3}, this->number3));
		EXPECT_EQ(
			std::pow(typename TypeParam::value_type{4}, this->stdNumber4),
			xablau::algebra::pow(typename TypeParam::value_type{4}, this->number4));
		EXPECT_EQ(
			std::pow(typename TypeParam::value_type{5}, this->stdNumber5),
			xablau::algebra::pow(typename TypeParam::value_type{5}, this->number5));

		EXPECT_EQ(xablau::algebra::sqrt(this->number1), std::sqrt(this->stdNumber1));
		EXPECT_EQ(xablau::algebra::sqrt(this->number2), std::sqrt(this->stdNumber2));
		EXPECT_EQ(xablau::algebra::sqrt(this->number3), std::sqrt(this->stdNumber3));
		EXPECT_EQ(xablau::algebra::sqrt(this->number4), std::sqrt(this->stdNumber4));
		EXPECT_EQ(xablau::algebra::sqrt(this->number5), std::sqrt(this->stdNumber5));
	}

	TYPED_TEST_P(AlgebraComplexTest, TrigonometricFunctions)
	{
		EXPECT_EQ(xablau::algebra::sin(this->number1), std::sin(this->stdNumber1));
		EXPECT_EQ(xablau::algebra::sin(this->number2), std::sin(this->stdNumber2));
		EXPECT_EQ(xablau::algebra::sin(this->number3), std::sin(this->stdNumber3));
		EXPECT_EQ(xablau::algebra::sin(this->number4), std::sin(this->stdNumber4));
		EXPECT_EQ(xablau::algebra::sin(this->number5), std::sin(this->stdNumber5));

		EXPECT_EQ(xablau::algebra::cos(this->number1), std::cos(this->stdNumber1));
		EXPECT_EQ(xablau::algebra::cos(this->number2), std::cos(this->stdNumber2));
		EXPECT_EQ(xablau::algebra::cos(this->number3), std::cos(this->stdNumber3));
		EXPECT_EQ(xablau::algebra::cos(this->number4), std::cos(this->stdNumber4));
		EXPECT_EQ(xablau::algebra::cos(this->number5), std::cos(this->stdNumber5));

		EXPECT_EQ(xablau::algebra::tan(this->number1), std::tan(this->stdNumber1));
		EXPECT_EQ(xablau::algebra::tan(this->number2), std::tan(this->stdNumber2));
		EXPECT_EQ(xablau::algebra::tan(this->number3), std::tan(this->stdNumber3));
		EXPECT_EQ(xablau::algebra::tan(this->number4), std::tan(this->stdNumber4));
		EXPECT_EQ(xablau::algebra::tan(this->number5), std::tan(this->stdNumber5));

		EXPECT_EQ(xablau::algebra::asin(this->number1), std::asin(this->stdNumber1));
		EXPECT_EQ(xablau::algebra::asin(this->number2), std::asin(this->stdNumber2));
		EXPECT_EQ(xablau::algebra::asin(this->number3), std::asin(this->stdNumber3));
		EXPECT_EQ(xablau::algebra::asin(this->number4), std::asin(this->stdNumber4));
		EXPECT_EQ(xablau::algebra::asin(this->number5), std::asin(this->stdNumber5));

		EXPECT_EQ(xablau::algebra::acos(this->number1), std::acos(this->stdNumber1));
		EXPECT_EQ(xablau::algebra::acos(this->number2), std::acos(this->stdNumber2));
		EXPECT_EQ(xablau::algebra::acos(this->number3), std::acos(this->stdNumber3));
		EXPECT_EQ(xablau::algebra::acos(this->number4), std::acos(this->stdNumber4));
		EXPECT_EQ(xablau::algebra::acos(this->number5), std::acos(this->stdNumber5));

		EXPECT_EQ(xablau::algebra::atan(this->number1), std::atan(this->stdNumber1));
		EXPECT_EQ(xablau::algebra::atan(this->number2), std::atan(this->stdNumber2));
		EXPECT_EQ(xablau::algebra::atan(this->number3), std::atan(this->stdNumber3));
		EXPECT_EQ(xablau::algebra::atan(this->number4), std::atan(this->stdNumber4));
		EXPECT_EQ(xablau::algebra::atan(this->number5), std::atan(this->stdNumber5));
	}

	TYPED_TEST_P(AlgebraComplexTest, HyperbolicFunctions)
	{
		EXPECT_EQ(xablau::algebra::sinh(this->number1), std::sinh(this->stdNumber1));
		EXPECT_EQ(xablau::algebra::sinh(this->number2), std::sinh(this->stdNumber2));
		EXPECT_EQ(xablau::algebra::sinh(this->number3), std::sinh(this->stdNumber3));
		EXPECT_EQ(xablau::algebra::sinh(this->number4), std::sinh(this->stdNumber4));
		EXPECT_EQ(xablau::algebra::sinh(this->number5), std::sinh(this->stdNumber5));

		EXPECT_EQ(xablau::algebra::cosh(this->number1), std::cosh(this->stdNumber1));
		EXPECT_EQ(xablau::algebra::cosh(this->number2), std::cosh(this->stdNumber2));
		EXPECT_EQ(xablau::algebra::cosh(this->number3), std::cosh(this->stdNumber3));
		EXPECT_EQ(xablau::algebra::cosh(this->number4), std::cosh(this->stdNumber4));
		EXPECT_EQ(xablau::algebra::cosh(this->number5), std::cosh(this->stdNumber5));

		EXPECT_EQ(xablau::algebra::tanh(this->number1), std::tanh(this->stdNumber1));
		EXPECT_EQ(xablau::algebra::tanh(this->number2), std::tanh(this->stdNumber2));
		EXPECT_EQ(xablau::algebra::tanh(this->number3), std::tanh(this->stdNumber3));
		EXPECT_EQ(xablau::algebra::tanh(this->number4), std::tanh(this->stdNumber4));
		EXPECT_EQ(xablau::algebra::tanh(this->number5), std::tanh(this->stdNumber5));

		EXPECT_EQ(xablau::algebra::asinh(this->number1), std::asinh(this->stdNumber1));
		EXPECT_EQ(xablau::algebra::asinh(this->number2), std::asinh(this->stdNumber2));
		EXPECT_EQ(xablau::algebra::asinh(this->number3), std::asinh(this->stdNumber3));
		EXPECT_EQ(xablau::algebra::asinh(this->number4), std::asinh(this->stdNumber4));
		EXPECT_EQ(xablau::algebra::asinh(this->number5), std::asinh(this->stdNumber5));

		EXPECT_EQ(xablau::algebra::acosh(this->number1), std::acosh(this->stdNumber1));
		EXPECT_EQ(xablau::algebra::acosh(this->number2), std::acosh(this->stdNumber2));
		EXPECT_EQ(xablau::algebra::acosh(this->number3), std::acosh(this->stdNumber3));
		EXPECT_EQ(xablau::algebra::acosh(this->number4), std::acosh(this->stdNumber4));
		EXPECT_EQ(xablau::algebra::acosh(this->number5), std::acosh(this->stdNumber5));

		EXPECT_EQ(xablau::algebra::atanh(this->number1), std::atanh(this->stdNumber1));
		EXPECT_EQ(xablau::algebra::atanh(this->number2), std::atanh(this->stdNumber2));
		EXPECT_EQ(xablau::algebra::atanh(this->number3), std::atanh(this->stdNumber3));
		EXPECT_EQ(xablau::algebra::atanh(this->number4), std::atanh(this->stdNumber4));
		EXPECT_EQ(xablau::algebra::atanh(this->number5), std::atanh(this->stdNumber5));
	}

	TYPED_TEST_P(AlgebraComplexTest, Concepts)
	{
		using XablauComplex = xablau::algebra::complex < typename TypeParam::value_type, typename TypeParam::ordering >;
		using StandardComplex = std::complex < typename TypeParam::value_type >;

		EXPECT_TRUE(xablau::algebra::concepts::xablau_complex < XablauComplex >);
		EXPECT_TRUE(xablau::algebra::concepts::standard_complex < StandardComplex >);

		EXPECT_FALSE(xablau::algebra::concepts::xablau_complex < StandardComplex >);
		EXPECT_FALSE(xablau::algebra::concepts::standard_complex < XablauComplex >);

		EXPECT_TRUE(xablau::algebra::concepts::complex < XablauComplex >);
		EXPECT_TRUE(xablau::algebra::concepts::complex < StandardComplex >);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		AlgebraComplexTest,
		ComparisonOperators,
		MemberFunctions,
		NonMemberFunctions,
		ExponentialFunctions,
		PowerFunctions,
		TrigonometricFunctions,
		HyperbolicFunctions,
		Concepts);

	using AlgebraComplexTestDataTypes = std::tuple < float, double, long double >;

	using AlgebraComplexTestOrderings =
		std::tuple <
			xablau::algebra::complex_ordering < xablau::algebra::complex_ordering_value::real >,
			xablau::algebra::complex_ordering < xablau::algebra::complex_ordering_value::imaginary >,
			xablau::algebra::complex_ordering < xablau::algebra::complex_ordering_value::radius >,
			xablau::algebra::complex_ordering < xablau::algebra::complex_ordering_value::theta > >;

	using AlgebraComplexTestGroup =
		xablau::testing::test_bundler <
			AlgebraComplexTestDataTypes,
			AlgebraComplexTestOrderings > ::template test_group < AlgebraComplexTypeDefinitions >;

	using AlgebraComplexTypes = xablau::testing::tuple_transformation < ::testing::Types, AlgebraComplexTestGroup > ::types;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		Xablau,
		AlgebraComplexTest,
		AlgebraComplexTypes);
}
