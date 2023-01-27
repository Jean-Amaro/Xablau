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

export module xablau.algebra:tensor_functions;
export import :tensor_concepts;

export import <cmath>;
export import <concepts>;

export namespace xablau::algebra::functions
{
	template < concepts::xablau_matrix_dynamic Type >
	struct addition_identity < Type >
	{
		[[nodiscard]] constexpr Type operator()(const std::array < size_t, Type::rank() > &dimensionalities) const
		{
			return Type(dimensionalities, Type{});
		}

		[[nodiscard]] static constexpr Type invoke(const std::array < size_t, Type::rank() > &dimensionalities)
		{
			return Type(dimensionalities, Type{});
		}
	};

	template < concepts::xablau_matrix_dynamic Type >
	struct multiplication_identity < Type >
	{
		[[nodiscard]] constexpr Type operator()(const size_t length) const
		{
			return multiplication_identity::invoke(length);
		}

		[[nodiscard]] static constexpr Type invoke(const size_t length)
		{
			Type matrix{};

			for (size_t i = 0; i < length; i++)
			{
				matrix(i, i) =
					xablau::algebra::functions::multiplication_identity < typename Type::value_type > ::invoke();
			}

			return matrix;
		}
	};

	template < concepts::xablau_matrix_fixed Type >
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

	template < concepts::xablau_matrix_fixed_square Type >
	struct multiplication_identity < Type >
	{
		[[nodiscard]] constexpr Type operator()() const noexcept
		{
			return multiplication_identity::invoke();
		}

		[[nodiscard]] static consteval Type invoke() noexcept
		{
			Type matrix{};
			constexpr auto length = Type::dimensionalities()[0];

			for (size_t i = 0; i < length; i++)
			{
				matrix(i, i) =
					xablau::algebra::functions::multiplication_identity < typename Type::value_type > ::invoke();
			}

			return matrix;
		}
	};
}