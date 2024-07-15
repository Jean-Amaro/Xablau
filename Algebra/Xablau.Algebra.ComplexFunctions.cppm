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

export module xablau.algebra:complex_functions;
export import :complex_concepts;
export import :complex_definitions;
export import :fundamental_concepts;
export import :fundamental_functions;

export import std;

export namespace xablau::algebra
{
	template < concepts::xablau_complex Type >
	[[nodiscard]] constexpr Type operator+(const Type &value)
	{
		return std::operator+(value.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] constexpr Type operator-(const Type &value)
	{
		return std::operator-(value.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] constexpr Type operator+(const Type &leftHandSide, const Type &rightHandSide)
	{
		return std::operator+(leftHandSide.number, rightHandSide.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] constexpr Type operator+(
		const Type &leftHandSide,
		const std::complex < typename Type::value_type > &rightHandSide)
	{
		return std::operator+(leftHandSide.number, rightHandSide);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] constexpr Type operator+(
		const std::complex < typename Type::value_type > &leftHandSide,
		const Type &rightHandSide)
	{
		return std::operator+(leftHandSide, rightHandSide.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] constexpr Type operator+(
		const Type &leftHandSide,
		const typename Type::value_type &rightHandSide)
	{
		return std::operator+(leftHandSide.number, rightHandSide);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] constexpr Type operator+(
		const typename Type::value_type &leftHandSide,
		const Type &rightHandSide)
	{
		return std::operator+(leftHandSide, rightHandSide.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] constexpr Type operator-(const Type &leftHandSide, const Type &rightHandSide)
	{
		return std::operator-(leftHandSide.number, rightHandSide.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] constexpr Type operator-(
		const Type &leftHandSide,
		const std::complex < typename Type::value_type > &rightHandSide)
	{
		return std::operator-(leftHandSide.number, rightHandSide);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] constexpr Type operator-(
		const std::complex < typename Type::value_type > &leftHandSide,
		const Type &rightHandSide)
	{
		return std::operator-(leftHandSide, rightHandSide.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] constexpr Type operator-(
		const Type &leftHandSide,
		const typename Type::value_type &rightHandSide)
	{
		return std::operator-(leftHandSide.number, rightHandSide);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] constexpr Type operator-(
		const typename Type::value_type &leftHandSide,
		const Type &rightHandSide)
	{
		return std::operator-(leftHandSide, rightHandSide.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] constexpr Type operator*(const Type &leftHandSide, const Type &rightHandSide)
	{
		return std::operator*(leftHandSide.number, rightHandSide.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] constexpr Type operator*(
		const Type &leftHandSide,
		const std::complex < typename Type::value_type > &rightHandSide)
	{
		return std::operator*(leftHandSide.number, rightHandSide);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] constexpr Type operator*(
		const std::complex < typename Type::value_type > &leftHandSide,
		const Type &rightHandSide)
	{
		return std::operator*(leftHandSide, rightHandSide.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] constexpr Type operator*(
		const Type &leftHandSide,
		const typename Type::value_type &rightHandSide)
	{
		return std::operator*(leftHandSide.number, rightHandSide);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] constexpr Type operator*(
		const typename Type::value_type &leftHandSide,
		const Type &rightHandSide)
	{
		return std::operator*(leftHandSide, rightHandSide.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] constexpr Type operator/(const Type &leftHandSide, const Type &rightHandSide)
	{
		return std::operator/(leftHandSide.number, rightHandSide.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] constexpr Type operator/(
		const Type &leftHandSide,
		const std::complex < typename Type::value_type > &rightHandSide)
	{
		return std::operator/(leftHandSide.number, rightHandSide);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] constexpr Type operator/(
		const std::complex < typename Type::value_type > &leftHandSide,
		const Type &rightHandSide)
	{
		return std::operator/(leftHandSide, rightHandSide.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] constexpr Type operator/(
		const Type &leftHandSide,
		const typename Type::value_type &rightHandSide)
	{
		return std::operator/(leftHandSide.number, rightHandSide);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] constexpr Type operator/(
		const typename Type::value_type &leftHandSide,
		const Type &rightHandSide)
	{
		return std::operator/(leftHandSide, rightHandSide.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] bool operator<(const Type &leftHandSide, const Type &rightHandSide)
	{
		return leftHandSide < rightHandSide;
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] bool operator<=(const Type &leftHandSide, const Type &rightHandSide)
	{
		return leftHandSide <= rightHandSide;
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] bool operator>(const Type &leftHandSide, const Type &rightHandSide)
	{
		return leftHandSide > rightHandSide;
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] bool operator>=(const Type &leftHandSide, const Type &rightHandSide)
	{
		return leftHandSide >= rightHandSide;
	}

	template < std::floating_point Type, concepts::complex_ordering Ordering1, concepts::complex_ordering Ordering2 >
	[[nodiscard]] constexpr bool operator==(
		const complex < Type, Ordering1 > &leftHandSide,
		const complex < Type, Ordering2 > &rightHandSide)
	{
		return std::operator==(leftHandSide.number, rightHandSide.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] constexpr bool operator==(
		const Type &leftHandSide,
		const std::complex < typename Type::value_type > &rightHandSide)
	{
		return std::operator==(leftHandSide.number, rightHandSide);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] constexpr bool operator==(
		const std::complex < typename Type::value_type > &leftHandSide,
		const Type &rightHandSide)
	{
		return std::operator==(leftHandSide, rightHandSide.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] constexpr bool operator==(
		const Type &leftHandSide,
		const typename Type::value_type &rightHandSide)
	{
		return std::operator==(leftHandSide.number, rightHandSide);
	}

	template < concepts::xablau_complex Type, typename CharacterDataType, typename Traits >
	std::basic_ostream < CharacterDataType, Traits > &operator<<(
		std::basic_ostream < CharacterDataType, Traits > &stream,
		const Type &value)
	{
		return std::operator<<(stream, value.number);
	}

	template < concepts::xablau_complex Type, typename CharacterDataType, typename Traits >
	std::basic_istream < CharacterDataType, Traits > &operator>>(
		std::basic_istream < CharacterDataType, Traits > &stream,
		Type &value)
	{
		return std::operator>>(stream, value.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] constexpr typename Type::value_type real(const Type &value)
	{
		return std::real(value.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] constexpr typename Type::value_type imag(const Type &value)
	{
		return std::imag(value.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] typename Type::value_type arg(const Type &value)
	{
		return std::arg(value.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] constexpr typename Type::value_type norm(const Type &value)
	{
		return std::norm(value.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] constexpr Type conj(const Type &value)
	{
		return std::conj(value.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] Type proj(const Type &value)
	{
		return std::proj(value.number);
	}

	template < concepts::complex_ordering Ordering, std::floating_point Type >
	[[nodiscard]] complex < Type, Ordering > polar(const Type &radius, const Type &theta = Type())
	{
		return std::polar(radius, theta);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] typename Type::value_type abs(const Type &value)
	{
		return std::abs(value.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] Type exp(const Type &value)
	{
		return std::exp(value.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] Type log(const Type &value)
	{
		return std::log(value.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] Type log10(const Type &value)
	{
		return std::log10(value.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] Type pow(const Type &base, const Type &exponent)
	{
		return std::pow(base.number, exponent.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] Type pow(
		const Type &base,
		const typename Type::value_type &exponent)
	{
		return std::pow(base.number, exponent);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] Type pow(
		const typename Type::value_type &base,
		const Type &exponent)
	{
		return std::pow(base, exponent.number);
	}

	template < std::floating_point BaseType, std::floating_point ExponentType, concepts::complex_ordering Ordering >
	requires (!std::same_as < BaseType, ExponentType >)
	[[nodiscard]] auto pow(
		const complex < BaseType, Ordering > &base,
		const complex < ExponentType, Ordering > &exponent)
	{
		using ResultDataType = typename decltype(std::pow(base.number, exponent.number))::value_type;

		return complex < ResultDataType, Ordering > (std::pow(base.number, exponent.number));
	}

	template < std::floating_point BaseType, std::floating_point ExponentType, concepts::complex_ordering Ordering >
	requires (!std::same_as < BaseType, ExponentType >)
	[[nodiscard]] auto pow(
		const complex < BaseType, Ordering > &base,
		const ExponentType &exponent)
	{
		using ResultDataType = typename decltype(std::pow(base.number, exponent))::value_type;

		return complex < ResultDataType, Ordering > (std::pow(base.number, exponent));
	}

	template < std::floating_point BaseType, std::floating_point ExponentType, concepts::complex_ordering Ordering >
	requires (!std::same_as < BaseType, ExponentType >)
	[[nodiscard]] auto pow(
		const BaseType &base,
		const complex < ExponentType, Ordering > &exponent)
	{
		using ResultDataType = typename decltype(std::pow(base, exponent.number))::value_type;

		return complex < ResultDataType, Ordering > (std::pow(base, exponent.number));
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] Type sqrt(const Type &value)
	{
		return std::sqrt(value.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] Type sin(const Type &value)
	{
		return std::sin(value.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] Type cos(const Type &value)
	{
		return std::cos(value.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] Type tan(const Type &value)
	{
		return std::tan(value.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] Type asin(const Type &value)
	{
		return std::asin(value.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] Type acos(const Type &value)
	{
		return std::acos(value.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] Type atan(const Type &value)
	{
		return std::atan(value.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] Type sinh(const Type &value)
	{
		return std::sinh(value.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] Type cosh(const Type &value)
	{
		return std::cosh(value.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] Type tanh(const Type &value)
	{
		return std::tanh(value.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] Type asinh(const Type &value)
	{
		return std::asinh(value.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] Type acosh(const Type &value)
	{
		return std::acosh(value.number);
	}

	template < concepts::xablau_complex Type >
	[[nodiscard]] Type atanh(const Type &value)
	{
		return std::atanh(value.number);
	}

	namespace functions
	{
		template < xablau::algebra::concepts::standard_complex Type >
		struct abs < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return std::abs(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return std::abs(value);
			}
		};

		template < xablau::algebra::concepts::xablau_complex Type >
		struct abs < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return xablau::algebra::abs(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return xablau::algebra::abs(value);
			}
		};

		template < xablau::algebra::concepts::standard_complex Type >
		struct exp < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return std::exp(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return std::exp(value);
			}
		};

		template < xablau::algebra::concepts::xablau_complex Type >
		struct exp < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return xablau::algebra::exp(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return xablau::algebra::exp(value);
			}
		};

		template < xablau::algebra::concepts::standard_complex Type >
		struct log < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return std::log(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return std::log(value);
			}
		};

		template < xablau::algebra::concepts::xablau_complex Type >
		struct log < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return xablau::algebra::log(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return xablau::algebra::log(value);
			}
		};

		template < xablau::algebra::concepts::standard_complex Type >
		struct log10 < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return std::log10(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return std::log10(value);
			}
		};

		template < xablau::algebra::concepts::xablau_complex Type >
		struct log10 < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return xablau::algebra::log10(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return xablau::algebra::log10(value);
			}
		};

		template <
			xablau::algebra::concepts::standard_complex Type1,
			xablau::algebra::concepts::standard_complex Type2 >
		struct pow < Type1, Type2 >
		{
			[[nodiscard]] auto operator()(const Type1 &value1, const Type2 &value2) const
			{
				return std::pow(value1, value2);
			}

			[[nodiscard]] static auto invoke(const Type1 &value1, const Type2 &value2)
			{
				return std::pow(value1, value2);
			}
		};

		template <
			xablau::algebra::concepts::xablau_complex Type1,
			xablau::algebra::concepts::xablau_complex Type2 >
		struct pow < Type1, Type2 >
		{
			[[nodiscard]] auto operator()(const Type1 &value1, const Type2 &value2) const
			{
				return xablau::algebra::pow(value1, value2);
			}

			[[nodiscard]] static auto invoke(const Type1 &value1, const Type2 &value2)
			{
				return xablau::algebra::pow(value1, value2);
			}
		};

		template < xablau::algebra::concepts::standard_complex Type >
		struct sqrt < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return std::sqrt(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return std::sqrt(value);
			}
		};

		template < xablau::algebra::concepts::xablau_complex Type >
		struct sqrt < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return xablau::algebra::sqrt(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return xablau::algebra::sqrt(value);
			}
		};

		template < xablau::algebra::concepts::standard_complex Type >
		struct sin < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return std::sin(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return std::sin(value);
			}
		};

		template < xablau::algebra::concepts::xablau_complex Type >
		struct sin < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return xablau::algebra::sin(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return xablau::algebra::sin(value);
			}
		};

		template < xablau::algebra::concepts::standard_complex Type >
		struct cos < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return std::cos(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return std::cos(value);
			}
		};

		template < xablau::algebra::concepts::xablau_complex Type >
		struct cos < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return xablau::algebra::cos(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return xablau::algebra::cos(value);
			}
		};

		template < xablau::algebra::concepts::standard_complex Type >
		struct tan < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return std::tan(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return std::tan(value);
			}
		};

		template < xablau::algebra::concepts::xablau_complex Type >
		struct tan < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return xablau::algebra::tan(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return xablau::algebra::tan(value);
			}
		};

		template < xablau::algebra::concepts::standard_complex Type >
		struct asin < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return std::asin(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return std::asin(value);
			}
		};

		template < xablau::algebra::concepts::xablau_complex Type >
		struct asin < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return xablau::algebra::asin(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return xablau::algebra::asin(value);
			}
		};

		template < xablau::algebra::concepts::standard_complex Type >
		struct acos < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return std::acos(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return std::acos(value);
			}
		};

		template < xablau::algebra::concepts::xablau_complex Type >
		struct acos < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return xablau::algebra::acos(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return xablau::algebra::acos(value);
			}
		};

		template < xablau::algebra::concepts::standard_complex Type >
		struct atan < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return std::atan(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return std::atan(value);
			}
		};

		template < xablau::algebra::concepts::xablau_complex Type >
		struct atan < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return xablau::algebra::atan(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return xablau::algebra::atan(value);
			}
		};

		template < xablau::algebra::concepts::standard_complex Type >
		struct sinh < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return std::sinh(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return std::sinh(value);
			}
		};

		template < xablau::algebra::concepts::xablau_complex Type >
		struct sinh < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return xablau::algebra::sinh(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return xablau::algebra::sinh(value);
			}
		};

		template < xablau::algebra::concepts::standard_complex Type >
		struct cosh < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return std::cosh(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return std::cosh(value);
			}
		};

		template < xablau::algebra::concepts::xablau_complex Type >
		struct cosh < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return xablau::algebra::cosh(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return xablau::algebra::cosh(value);
			}
		};

		template < xablau::algebra::concepts::standard_complex Type >
		struct tanh < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return std::tanh(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return std::tanh(value);
			}
		};

		template < xablau::algebra::concepts::xablau_complex Type >
		struct tanh < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return xablau::algebra::tanh(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return xablau::algebra::tanh(value);
			}
		};

		template < xablau::algebra::concepts::standard_complex Type >
		struct asinh < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return std::asinh(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return std::asinh(value);
			}
		};

		template < xablau::algebra::concepts::xablau_complex Type >
		struct asinh < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return xablau::algebra::asinh(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return xablau::algebra::asinh(value);
			}
		};

		template < xablau::algebra::concepts::standard_complex Type >
		struct acosh < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return std::acosh(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return std::acosh(value);
			}
		};

		template < xablau::algebra::concepts::xablau_complex Type >
		struct acosh < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return xablau::algebra::acosh(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return xablau::algebra::acosh(value);
			}
		};

		template < xablau::algebra::concepts::standard_complex Type >
		struct atanh < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return std::atanh(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return std::atanh(value);
			}
		};

		template < xablau::algebra::concepts::xablau_complex Type >
		struct atanh < Type >
		{
			[[nodiscard]] auto operator()(const Type &value) const
			{
				return xablau::algebra::atanh(value);
			}

			[[nodiscard]] static auto invoke(const Type &value)
			{
				return xablau::algebra::atanh(value);
			}
		};

		template < xablau::algebra::concepts::standard_complex Type >
		struct almost_equal < Type >
		{
			template < size_t UnitsInTheLastPlace >
			[[nodiscard]] constexpr bool operator()(const Type &value1, const Type &value2) const
			{
				return almost_equal::invoke < UnitsInTheLastPlace > (value1, value2);
			}

			[[nodiscard]] constexpr bool operator()(const Type &value1, const Type &value2, const size_t unitsInTheLastPlace) const
			{
				return almost_equal::invoke(value1, value2, unitsInTheLastPlace);
			}

			template < size_t UnitsInTheLastPlace >
			[[nodiscard]] static bool invoke(const Type &value1, const Type &value2)
			{
				using value_type = typename Type::value_type;

				return
					std::abs(value1 - value2) <=
						std::numeric_limits < value_type > ::epsilon() * std::abs(value1 + value2) * value_type{UnitsInTheLastPlace} ||

					std::abs(value1 - value2) < std::numeric_limits < value_type > ::min();
			}

			[[nodiscard]] static bool invoke(const Type &value1, const Type &value2, const size_t unitsInTheLastPlace)
			{
				using value_type = typename Type::value_type;

				return
					std::abs(value1 - value2) <=
						std::numeric_limits < value_type > ::epsilon() * std::abs(value1 + value2) * static_cast < value_type > (unitsInTheLastPlace) ||

					std::abs(value1 - value2) < std::numeric_limits < value_type > ::min();
			}
		};

		template < xablau::algebra::concepts::xablau_complex Type >
		struct almost_equal < Type >
		{
			template < size_t UnitsInTheLastPlace >
			[[nodiscard]] constexpr bool operator()(const Type &value1, const Type &value2) const
			{
				return almost_equal::invoke < UnitsInTheLastPlace > (value1, value2);
			}

			[[nodiscard]] constexpr bool operator()(const Type &value1, const Type &value2, const size_t unitsInTheLastPlace) const
			{
				return almost_equal::invoke(value1, value2, unitsInTheLastPlace);
			}

			template < size_t UnitsInTheLastPlace >
			[[nodiscard]] static bool invoke(const Type &value1, const Type &value2)
			{
				using value_type = typename Type::value_type;

				return
					xablau::algebra::abs(value1 - value2) <=
						std::numeric_limits < value_type > ::epsilon() * xablau::algebra::abs(value1 + value2) * value_type{UnitsInTheLastPlace} ||

					xablau::algebra::abs(value1 - value2) < std::numeric_limits < value_type > ::min();
			}

			[[nodiscard]] static bool invoke(const Type &value1, const Type &value2, const size_t unitsInTheLastPlace)
			{
				using value_type = typename Type::value_type;

				return
					xablau::algebra::abs(value1 - value2) <=
						std::numeric_limits < value_type > ::epsilon() * xablau::algebra::abs(value1 + value2) * static_cast < value_type > (unitsInTheLastPlace) ||

					xablau::algebra::abs(value1 - value2) < std::numeric_limits < value_type > ::min();
			}
		};

		template < concepts::complex Type >
		struct addition_identity < Type >
		{
			[[nodiscard]] constexpr Type operator()() const noexcept
			{
				return Type{};
			}

			[[nodiscard]] static consteval Type invoke() noexcept
			{
				return Type{};
			}
		};

		template < concepts::complex Type >
		struct multiplication_identity < Type >
		{
			[[nodiscard]] constexpr Type operator()() const noexcept
			{
				return Type(1, 0);
			}

			[[nodiscard]] static consteval Type invoke() noexcept
			{
				return Type(1, 0);
			}
		};

		template < concepts::complex Type >
		struct sign < Type >
		{
			[[nodiscard]] constexpr typename sign_type < Type > ::type operator()(const Type &value) const noexcept
			{
				return sign::invoke(value);
			}

			[[nodiscard]] static constexpr typename sign_type < Type > ::type invoke(const Type &value) noexcept
			{
				using ValueType = typename Type::value_type;

				return
					std::to_array(
						{
							(value.real() > ValueType{} ? sign_value::positive : (value.real() < ValueType{} ? sign_value::negative : sign_value::zero)),
							(value.imag() > ValueType{} ? sign_value::positive : (value.imag() < ValueType{} ? sign_value::negative : sign_value::zero))
						});
			}
		};

		template < concepts::complex Type >
		struct normal < Type >
		{
			[[nodiscard]] constexpr bool operator()(const Type &value) const noexcept
			{
				return
					normal < typename Type::value_type > {}(value.real()) &&
					normal < typename Type::value_type > {}(value.imag());
			}

			[[nodiscard]] static constexpr bool invoke(const Type &value) noexcept
			{
				return
					normal < typename Type::value_type > {}(value.real()) &&
					normal < typename Type::value_type > {}(value.imag());
			}
		};

		template < concepts::xablau_complex Type1, concepts::xablau_complex Type2 >
		struct min < Type1, Type2 >
		{
			[[nodiscard]] auto operator()(const Type1 &value1, const Type2 &value2) const
			{
				return std::min(value1, value2);
			}

			[[nodiscard]] static auto invoke(const Type1 &value1, const Type2 &value2)
			{
				return std::min(value1, value2);
			}
		};

		template < concepts::xablau_complex Type1, concepts::xablau_complex Type2 >
		struct max < Type1, Type2 >
		{
			[[nodiscard]] auto operator()(const Type1 &value1, const Type2 &value2) const
			{
				return std::max(value1, value2);
			}

			[[nodiscard]] static auto invoke(const Type1 &value1, const Type2 &value2)
			{
				return std::max(value1, value2);
			}
		};

		template < concepts::xablau_complex Type >
		struct min_ref_return < Type >
		{
			[[nodiscard]] const Type &operator()(const Type &value1, const Type &value2) const
			{
				return std::min(value1, value2);
			}

			[[nodiscard]] static const Type &invoke(const Type &value1, const Type &value2)
			{
				return std::min(value1, value2);
			}
		};

		template < concepts::xablau_complex Type >
		struct max_ref_return < Type >
		{
			[[nodiscard]] const Type &operator()(const Type &value1, const Type &value2) const
			{
				return std::max(value1, value2);
			}

			[[nodiscard]] static const Type &invoke(const Type &value1, const Type &value2)
			{
				return std::max(value1, value2);
			}
		};

		template < concepts::complex Type >
		struct conjugate < Type >
		{
			[[nodiscard]] constexpr typename Type operator()(const Type &value) const noexcept
			{
				return Type(value.real(), -value.imag());
			}

			[[nodiscard]] static constexpr typename Type invoke(const Type &value) noexcept
			{
				return Type(value.real(), -value.imag());
			}
		};
	}
}

export namespace std
{
	template < xablau::algebra::concepts::xablau_complex Type >
	struct hash < Type >
	{
		size_t operator()(const Type &complex) const
		{
			return
				std::hash < typename Type::value_type > {}(complex.real()) ^
					std::rotl(std::hash < typename Type::value_type > {}(complex.imag()), 1);
		}
	};
}