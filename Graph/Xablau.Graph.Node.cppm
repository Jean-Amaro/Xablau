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

export module xablau.graph:node;
export import :node_configurations;

export import <compare>;
export import <functional>;
export import <type_traits>;
export import <utility>;

export namespace xablau::graph
{
	template < typename Type >
	requires (std::is_object < Type > ::value)
	class node final
	{
	public:
		using value_type = Type;

		Type value{};

		[[nodiscard]] constexpr explicit operator Type &()
		{
			return this->value;
		}

		[[nodiscard]] constexpr explicit operator const Type &() const
		{
			return this->value;
		}

		[[nodiscard]] constexpr bool operator<(const node &other) const
		{
			return this->value < other.value;
		}

		[[nodiscard]] constexpr bool operator<=(const node &other) const
		{
			return this->value <= other.value;
		}

		[[nodiscard]] constexpr bool operator>(const node &other) const
		{
			return this->value > other.value;
		}

		[[nodiscard]] constexpr bool operator>=(const node &other) const
		{
			return this->value >= other.value;
		}

		[[nodiscard]] constexpr bool operator==(const node &other) const
		{
			return this->value == other.value;
		}

		[[nodiscard]] constexpr bool operator!=(const node &other) const
		{
			return this->value != other.value;
		}

		constexpr node &operator=(const node &)
			noexcept(std::is_nothrow_copy_assignable < Type > ::value) = default;

		constexpr node &operator=(node &&)
			noexcept(std::is_nothrow_move_assignable < Type > ::value) = default;

		constexpr node()
			noexcept(std::is_nothrow_default_constructible < Type > ::value) = default;

		constexpr node(const Type &value)
			noexcept(std::is_nothrow_copy_constructible < Type > ::value) :

			value(value)
		{
		}

		constexpr node(Type &&value)
			noexcept(std::is_nothrow_move_constructible < Type > ::value) :

			value(std::move(value))
		{
		}

		constexpr node(const node &)
			noexcept(std::is_nothrow_copy_constructible < Type > ::value) = default;

		constexpr node(node &&)
			noexcept(std::is_nothrow_move_constructible < Type > ::value) = default;
	};
}

export namespace std
{
	template < typename Type >
	requires (std::is_object < Type > ::value)
	struct hash < xablau::graph::node < Type > >
	{
		size_t operator()(const xablau::graph::node < Type > &node) const
		{
			return std::hash < Type > {}(node.value);
		}
	};
}