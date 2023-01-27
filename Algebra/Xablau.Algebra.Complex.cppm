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

export module xablau.algebra:complex;
export import :complex_configurations;
export import :fundamental_concepts;
export import :fundamental_functions;

export import <complex>;
export import <concepts>;

export namespace xablau::algebra
{
	template < std::floating_point Type, concepts::complex_ordering Ordering >
	class complex final
	{
	public:
		using value_type = typename std::complex < Type > ::value_type;

		static constexpr complex_ordering_value ordering = Ordering::ordering();

		std::complex < Type > number{};

		[[nodiscard]] explicit operator std::complex < Type > &()
		{
			return this->number;
		}

		[[nodiscard]] explicit operator const std::complex < Type > &() const
		{
			return this->number;
		}

		template < typename NewType >
		[[nodiscard]] constexpr complex < NewType, Ordering > convert_type() const
		{
			return complex < NewType, Ordering > (
				static_cast < NewType > (this->number.real()),
				static_cast < NewType > (this->number.imag()));
		}

		[[nodiscard]] constexpr Type &real()
		{
			return reinterpret_cast < Type(&)[2] > (this->number)[0];
		}

		constexpr void real(Type value)
		{
			this->number.real(value);
		}

		[[nodiscard]] constexpr const Type &real() const
		{
			return reinterpret_cast < const Type(&)[2] > (this->number)[0];
		}

		[[nodiscard]] constexpr Type &imag()
		{
			return reinterpret_cast < Type(&)[2] > (this->number)[1];
		}

		constexpr void imag(Type value)
		{
			this->number.imag(value);
		}

		[[nodiscard]] constexpr const Type &imag() const
		{
			return reinterpret_cast < const Type(&)[2] > (this->number)[1];
		}

		constexpr complex &operator=(const complex &) = default;

		constexpr complex &operator=(complex &&) noexcept = default;

		constexpr complex &operator=(const std::complex < Type > &number)
		{
			this->number = number;
			
			return *this;
		}

		constexpr complex &operator=(const Type &value)
		{
			this->number = value;

			return *this;
		}

		constexpr complex &operator+=(const Type &value)
		{
			this->number += value;

			return *this;
		}

		constexpr complex &operator-=(const Type &value)
		{
			this->number -= value;

			return *this;
		}

		constexpr complex &operator*=(const Type &value)
		{
			this->number *= value;

			return *this;
		}

		constexpr complex &operator/=(const Type &value)
		{
			this->number /= value;

			return *this;
		}

		constexpr complex &operator+=(const complex &complex)
		{
			this->number += complex.number;

			return *this;
		}

		constexpr complex &operator-=(const complex &complex)
		{
			this->number -= complex.number;

			return *this;
		}

		constexpr complex &operator*=(const complex &complex)
		{
			this->number *= complex.number;

			return *this;
		}

		constexpr complex &operator/=(const complex &complex)
		{
			this->number /= complex.number;

			return *this;
		}

		[[nodiscard]] bool operator<(const complex &complex) const
		{
			if constexpr (complex::ordering == complex_ordering_value::real)
			{
				return
					this->number.real() < complex.number.real() ||
					this->number.real() == complex.number.real() && this->number.imag() < complex.number.imag();
			}

			else if constexpr (complex::ordering == complex_ordering_value::imaginary)
			{
				return
					this->number.imag() < complex.number.imag() ||
					this->number.imag() == complex.number.imag() && this->number.real() < complex.number.real();
			}

			else if constexpr (complex::ordering == complex_ordering_value::radius)
			{
				const auto radius1 = std::abs(this->number);
				const auto radius2 = std::abs(complex.number);

				return
					radius1 < radius2 ||
					radius1 == radius2 && std::arg(this->number) < std::arg(complex.number);
			}

			else
			{
				const auto theta1 = std::arg(this->number);
				const auto theta2 = std::arg(complex.number);

				return
					theta1 < theta2 ||
					theta1 == theta2 && std::abs(this->number) < std::abs(complex.number);
			}
		}

		[[nodiscard]] bool operator<=(const complex &complex) const
		{
			if constexpr (complex::ordering == complex_ordering_value::real)
			{
				return
					this->number.real() < complex.number.real() ||
					this->number.real() == complex.number.real() && this->number.imag() <= complex.number.imag();
			}

			else if constexpr (complex::ordering == complex_ordering_value::imaginary)
			{
				return
					this->number.imag() < complex.number.imag() ||
					this->number.imag() == complex.number.imag() && this->number.real() <= complex.number.real();
			}

			else if constexpr (complex::ordering == complex_ordering_value::radius)
			{
				const auto radius1 = std::abs(this->number);
				const auto radius2 = std::abs(complex.number);

				return
					radius1 < radius2 ||
					radius1 == radius2 && std::arg(this->number) <= std::arg(complex.number);
			}

			else
			{
				const auto theta1 = std::arg(this->number);
				const auto theta2 = std::arg(complex.number);

				return
					theta1 < theta2 ||
					theta1 == theta2 && std::abs(this->number) <= std::abs(complex.number);
			}
		}

		[[nodiscard]] bool operator>(const complex &complex) const
		{
			if constexpr (complex::ordering == complex_ordering_value::real)
			{
				return
					this->number.real() > complex.number.real() ||
					this->number.real() == complex.number.real() && this->number.imag() > complex.number.imag();
			}

			else if constexpr (complex::ordering == complex_ordering_value::imaginary)
			{
				return
					this->number.imag() > complex.number.imag() ||
					this->number.imag() == complex.number.imag() && this->number.real() > complex.number.real();
			}

			else if constexpr (complex::ordering == complex_ordering_value::radius)
			{
				const auto radius1 = std::abs(this->number);
				const auto radius2 = std::abs(complex.number);

				return
					radius1 > radius2 ||
					radius1 == radius2 && std::arg(this->number) > std::arg(complex.number);
			}

			else
			{
				const auto theta1 = std::arg(this->number);
				const auto theta2 = std::arg(complex.number);

				return
					theta1 > theta2 ||
					theta1 == theta2 && std::abs(this->number) > std::abs(complex.number);
			}
		}

		[[nodiscard]] bool operator>=(const complex &complex) const
		{
			if constexpr (complex::ordering == complex_ordering_value::real)
			{
				return
					this->number.real() > complex.number.real() ||
					this->number.real() == complex.number.real() && this->number.imag() >= complex.number.imag();
			}

			else if constexpr (complex::ordering == complex_ordering_value::imaginary)
			{
				return
					this->number.imag() > complex.number.imag() ||
					this->number.imag() == complex.number.imag() && this->number.real() >= complex.number.real();
			}

			else if constexpr (complex::ordering == complex_ordering_value::radius)
			{
				const auto radius1 = std::abs(this->number);
				const auto radius2 = std::abs(complex.number);

				return
					radius1 > radius2 ||
					radius1 == radius2 && std::arg(this->number) >= std::arg(complex.number);
			}

			else
			{
				const auto theta1 = std::arg(this->number);
				const auto theta2 = std::arg(complex.number);

				return
					theta1 > theta2 ||
					theta1 == theta2 && std::abs(this->number) >= std::abs(complex.number);
			}
		}

		[[nodiscard]] constexpr bool operator==(const complex &) const = default;

		template < concepts::complex_ordering OtherOrdering >
		[[nodiscard]] constexpr bool operator==(const complex < Type, OtherOrdering > &complex) const
		{
			return this->number == complex.number;
		}

		constexpr complex(const Type &real = Type{}, const Type &imag = Type{}) :
			number(real, imag)
		{
		}

		constexpr complex(const std::complex < Type > &number) :
			number(number)
		{
		}

		constexpr complex(const complex &) = default;

		constexpr complex(complex &&) noexcept = default;
	};
}