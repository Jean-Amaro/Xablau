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

export module xablau.algebra:standard_functions;
export import :fundamental_concepts;
export import :fundamental_functions;
export import :standard_definitions;

export import <cmath>;
export import <concepts>;

export namespace xablau::algebra::functions
{
	template < concepts::standard_arithmetic Type >
	struct abs < Type >
	{
		[[nodiscard]] auto operator()(const Type value) const
		{
			return std::abs(value);
		}

		[[nodiscard]] static auto invoke(const Type value)
		{
			return std::abs(value);
		}
	};

	template < concepts::standard_arithmetic Type >
	struct exp < Type >
	{
		[[nodiscard]] auto operator()(const Type value) const
		{
			return std::exp(value);
		}

		[[nodiscard]] static auto invoke(const Type value)
		{
			return std::exp(value);
		}
	};

	template < concepts::standard_arithmetic Type >
	struct log < Type >
	{
		[[nodiscard]] auto operator()(const Type value) const
		{
			return std::log(value);
		}

		[[nodiscard]] static auto invoke(const Type value)
		{
			return std::log(value);
		}
	};

	template < concepts::standard_arithmetic Type >
	struct log2 < Type >
	{
		[[nodiscard]] auto operator()(const Type value) const
		{
			return std::log2(value);
		}

		[[nodiscard]] static auto invoke(const Type value)
		{
			return std::log2(value);
		}
	};

	template < concepts::standard_arithmetic Type >
	struct log10 < Type >
	{
		[[nodiscard]] auto operator()(const Type value) const
		{
			return std::log10(value);
		}

		[[nodiscard]] static auto invoke(const Type value)
		{
			return std::log10(value);
		}
	};

	template < concepts::standard_arithmetic Type1, concepts::standard_arithmetic Type2 >
	struct pow < Type1, Type2 >
	{
		[[nodiscard]] auto operator()(const Type1 value1, const Type2 value2) const
		{
			return std::pow(value1, value2);
		}

		[[nodiscard]] static auto invoke(const Type1 value1, const Type2 value2)
		{
			return std::pow(value1, value2);
		}
	};

	template < concepts::standard_arithmetic Type >
	struct sqrt < Type >
	{
		[[nodiscard]] auto operator()(const Type value) const
		{
			return std::sqrt(value);
		}

		[[nodiscard]] static auto invoke(const Type value)
		{
			return std::sqrt(value);
		}
	};

	template < concepts::standard_arithmetic Type >
	struct sin < Type >
	{
		[[nodiscard]] auto operator()(const Type value) const
		{
			return std::sin(value);
		}

		[[nodiscard]] static auto invoke(const Type value)
		{
			return std::sin(value);
		}
	};

	template < concepts::standard_arithmetic Type >
	struct cos < Type >
	{
		[[nodiscard]] auto operator()(const Type value) const
		{
			return std::cos(value);
		}

		[[nodiscard]] static auto invoke(const Type value)
		{
			return std::cos(value);
		}
	};

	template < concepts::standard_arithmetic Type >
	struct tan < Type >
	{
		[[nodiscard]] auto operator()(const Type value) const
		{
			return std::tan(value);
		}

		[[nodiscard]] static auto invoke(const Type value)
		{
			return std::tan(value);
		}
	};

	template < concepts::standard_arithmetic Type >
	struct asin < Type >
	{
		[[nodiscard]] auto operator()(const Type value) const
		{
			return std::asin(value);
		}

		[[nodiscard]] static auto invoke(const Type value)
		{
			return std::asin(value);
		}
	};

	template < concepts::standard_arithmetic Type >
	struct acos < Type >
	{
		[[nodiscard]] auto operator()(const Type value) const
		{
			return std::acos(value);
		}

		[[nodiscard]] static auto invoke(const Type value)
		{
			return std::acos(value);
		}
	};

	template < concepts::standard_arithmetic Type >
	struct atan < Type >
	{
		[[nodiscard]] auto operator()(const Type value) const
		{
			return std::atan(value);
		}

		[[nodiscard]] static auto invoke(const Type value)
		{
			return std::atan(value);
		}
	};

	template < concepts::standard_arithmetic Type >
	struct sinh < Type >
	{
		[[nodiscard]] auto operator()(const Type value) const
		{
			return std::sinh(value);
		}

		[[nodiscard]] static auto invoke(const Type value)
		{
			return std::sinh(value);
		}
	};

	template < concepts::standard_arithmetic Type >
	struct cosh < Type >
	{
		[[nodiscard]] auto operator()(const Type value) const
		{
			return std::cosh(value);
		}

		[[nodiscard]] static auto invoke(const Type value)
		{
			return std::cosh(value);
		}
	};

	template < concepts::standard_arithmetic Type >
	struct tanh < Type >
	{
		[[nodiscard]] auto operator()(const Type value) const
		{
			return std::tanh(value);
		}

		[[nodiscard]] static auto invoke(const Type value)
		{
			return std::tanh(value);
		}
	};

	template < concepts::standard_arithmetic Type >
	struct asinh < Type >
	{
		[[nodiscard]] auto operator()(const Type value) const
		{
			return std::asinh(value);
		}

		[[nodiscard]] static auto invoke(const Type value)
		{
			return std::asinh(value);
		}
	};

	template < concepts::standard_arithmetic Type >
	struct acosh < Type >
	{
		[[nodiscard]] auto operator()(const Type value) const
		{
			return std::acosh(value);
		}

		[[nodiscard]] static auto invoke(const Type value)
		{
			return std::acosh(value);
		}
	};

	template < concepts::standard_arithmetic Type >
	struct atanh < Type >
	{
		[[nodiscard]] auto operator()(const Type value) const
		{
			return std::atanh(value);
		}

		[[nodiscard]] static auto invoke(const Type value)
		{
			return std::atanh(value);
		}
	};

	template < std::integral Type >
	struct almost_equal < Type >
	{
		template < size_t UnitsInTheLastPlace = 0 >
		[[nodiscard]] bool operator()(const Type value1, const Type value2) const
		{
			return value1 == value2;
		}

		[[nodiscard]] bool operator()(const Type value1, const Type value2, const size_t unitsInTheLastPlace = 0) const
		{
			static_cast < void > (unitsInTheLastPlace);

			return value1 == value2;
		}

		template < size_t UnitsInTheLastPlace = 0 >
		[[nodiscard]] static bool invoke(const Type value1, const Type value2)
		{
			return value1 == value2;
		}

		[[nodiscard]] static bool invoke(const Type value1, const Type value2, const size_t unitsInTheLastPlace = 0)
		{
			static_cast < void > (unitsInTheLastPlace);

			return value1 == value2;
		}
	};

	// https://en.cppreference.com/w/cpp/types/numeric_limits/epsilon
	template < std::floating_point Type >
	struct almost_equal < Type >
	{
		template < size_t UnitsInTheLastPlace >
		[[nodiscard]] bool operator()(const Type value1, const Type value2) const
		{
			return almost_equal::invoke < UnitsInTheLastPlace > (value1, value2);
		}

		[[nodiscard]] bool operator()(const Type value1, const Type value2, const size_t unitsInTheLastPlace) const
		{
			return almost_equal::invoke(value1, value2, unitsInTheLastPlace);
		}

		template < size_t UnitsInTheLastPlace >
		[[nodiscard]] static bool invoke(const Type value1, const Type value2)
		{
			return
				std::abs(value1 - value2) <=
					std::numeric_limits < Type > ::epsilon() * std::abs(value1 + value2) * Type{UnitsInTheLastPlace} ||

				std::abs(value1 - value2) < std::numeric_limits < Type > ::min();
		}

		[[nodiscard]] static bool invoke(const Type value1, const Type value2, const size_t unitsInTheLastPlace)
		{
			return
				std::abs(value1 - value2) <=
					std::numeric_limits < Type > ::epsilon() * std::abs(value1 + value2) * static_cast < Type > (unitsInTheLastPlace) ||

				std::abs(value1 - value2) < std::numeric_limits < Type > ::min();
		}
	};

	template < concepts::standard_arithmetic Type >
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

	template < concepts::standard_arithmetic Type >
	struct multiplication_identity < Type >
	{
		[[nodiscard]] constexpr Type operator()() const noexcept
		{
			return Type{1};
		}

		[[nodiscard]] static consteval Type invoke() noexcept
		{
			return Type{1};
		}
	};

	template < concepts::standard_arithmetic Type >
	struct sign < Type >
	{
		[[nodiscard]] constexpr typename sign_type < Type > ::type operator()(const Type value) const noexcept
		{
			return sign::invoke(value);
		}

		[[nodiscard]] static constexpr typename sign_type < Type > ::type invoke(const Type value) noexcept
		{
			if (value > Type{})
			{
				return std::to_array({sign_value::positive});
			}

			if (value < Type{})
			{
				return std::to_array({sign_value::negative});
			}

			return std::to_array({sign_value::zero});
		}
	};

	template < concepts::standard_arithmetic Type >
	struct normal < Type >
	{
		[[nodiscard]] constexpr bool operator()(const Type value) const noexcept
		{
			return std::isnormal(value);
		}

		[[nodiscard]] static constexpr bool invoke(const Type value) noexcept
		{
			return std::isnormal(value);
		}
	};

	template < concepts::standard_arithmetic Type1, concepts::standard_arithmetic Type2 >
	struct min < Type1, Type2 >
	{
		[[nodiscard]] constexpr auto operator()(const Type1 value1, const Type2 value2) const
		{
			return std::min(value1, value2);
		}

		[[nodiscard]] static constexpr auto invoke(const Type1 value1, const Type2 value2)
		{
			return std::min(value1, value2);
		}
	};

	template < concepts::standard_arithmetic Type1, concepts::standard_arithmetic Type2 >
	struct max < Type1, Type2 >
	{
		[[nodiscard]] constexpr auto operator()(const Type1 value1, const Type2 value2) const
		{
			return std::max(value1, value2);
		}

		[[nodiscard]] static constexpr auto invoke(const Type1 value1, const Type2 value2)
		{
			return std::max(value1, value2);
		}
	};

	template < concepts::standard_arithmetic Type >
	struct min_ref_return < Type >
	{
		[[nodiscard]] constexpr const Type &operator()(const Type &value1, const Type &value2) const
		{
			return std::min(value1, value2);
		}

		[[nodiscard]] static constexpr const Type &invoke(const Type &value1, const Type &value2)
		{
			return std::min(value1, value2);
		}
	};

	template < concepts::standard_arithmetic Type >
	struct max_ref_return < Type >
	{
		[[nodiscard]] constexpr const Type &operator()(const Type &value1, const Type &value2) const
		{
			return std::max(value1, value2);
		}

		[[nodiscard]] static constexpr const Type &invoke(const Type &value1, const Type &value2)
		{
			return std::max(value1, value2);
		}
	};

	template < concepts::standard_arithmetic Type >
	struct conjugate < Type >
	{
		[[nodiscard]] constexpr typename Type operator()(const Type value) const noexcept
		{
			return value;
		}

		[[nodiscard]] static constexpr typename Type invoke(const Type value) noexcept
		{
			return value;
		}
	};
}