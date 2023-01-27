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

export module xablau.geometry:fundamental_functions;

export import <type_traits>;

export import xablau.algebra;

export namespace xablau::geometry::functions
{
	template < typename >
	struct scalar_product_reduction;

	template < xablau::algebra::concepts::standard_arithmetic Type >
	struct scalar_product_reduction < Type >
	{
		[[nodiscard]] Type operator()(Type value1, Type value2) const noexcept
		{
			return value1 + value2;
		}

		[[nodiscard]] static Type invoke(Type value1, Type value2) noexcept
		{
			return value1 + value2;
		}
	};

	template < xablau::algebra::concepts::complex Type >
	struct scalar_product_reduction < Type >
	{
		[[nodiscard]] typename Type::value_type operator()(typename Type::value_type value1, typename Type::value_type value2) const noexcept
		{
			return value1 + value2;
		}

		[[nodiscard]] static typename Type::value_type invoke(typename Type::value_type value1, typename Type::value_type value2) noexcept
		{
			return value1 + value2;
		}
	};

	template < typename >
	struct scalar_product_transformation;

	template < xablau::algebra::concepts::standard_arithmetic Type >
	struct scalar_product_transformation < Type >
	{
		[[nodiscard]] Type operator()(Type value1, Type value2) const noexcept
		{
			return value1 * value2;
		}

		[[nodiscard]] static Type invoke(Type value1, Type value2) noexcept
		{
			return value1 * value2;
		}
	};

	template < xablau::algebra::concepts::standard_complex Type >
	struct scalar_product_transformation < Type >
	{
		[[nodiscard]] typename Type::value_type operator()(const Type &value1, const Type &value2) const noexcept
		{
			return (value1 * std::conj(value2)).real();
		}

		[[nodiscard]] static typename Type::value_type invoke(const Type &value1, const Type &value2) noexcept
		{
			return (value1 * std::conj(value2)).real();
		}
	};

	template < xablau::algebra::concepts::complex Type >
	struct scalar_product_transformation < Type >
	{
		[[nodiscard]] typename Type::value_type operator()(const Type &value1, const Type &value2) const noexcept
		{
			return (value1 * xablau::algebra::conj(value2)).real();
		}

		[[nodiscard]] static typename Type::value_type invoke(const Type &value1, const Type &value2) noexcept
		{
			return (value1 * xablau::algebra::conj(value2)).real();
		}
	};
}