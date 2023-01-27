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

export module xablau.algebra:standard_definitions;
export import :fundamental_concepts;
export import :fundamental_configurations;
export import :fundamental_definitions;

export import <array>;

export namespace xablau::algebra
{
	template < concepts::standard_arithmetic Type >
	struct abs_type < Type >
	{
		using type = Type;
	};

	template < concepts::standard_arithmetic Type >
	struct sign_type < Type >
	{
		using type = std::array < sign_value, 1 >;
	};

	template < xablau::algebra::concepts::standard_arithmetic Type >
	struct associative_addition < Type > : std::true_type{};

	template < xablau::algebra::concepts::standard_arithmetic Type >
	struct commutative_addition < Type > : std::true_type{};

	template < xablau::algebra::concepts::standard_arithmetic Type >
	struct associative_and_commutative_addition < Type > : std::true_type{};

	template < xablau::algebra::concepts::standard_arithmetic Type >
	struct associative_multiplication < Type > : std::true_type{};

	template < xablau::algebra::concepts::standard_arithmetic Type >
	struct commutative_multiplication < Type > : std::true_type{};

	template < xablau::algebra::concepts::standard_arithmetic Type >
	struct associative_and_commutative_multiplication < Type > : std::true_type{};
}