// MIT License
//
// Copyright (c) 2024 Jean Amaro <jean.amaro@outlook.com.br>
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

export module xablau.graph:node_ref;

export import std;

export namespace xablau::graph
{
	template < typename Type >
	requires (std::is_object < Type > ::value)
	class node_ref final
	{
	private:
		std::reference_wrapper < Type > _ref{};

	public:
		using type = Type;

		[[nodiscard]] constexpr Type &get()
		{
			return this->_ref.get();
		}

		[[nodiscard]] constexpr const Type &get() const
		{
			return this->_ref.get();
		}

		[[nodiscard]] constexpr operator Type &()
		{
			return this->_ref.get();
		}

		[[nodiscard]] constexpr operator const Type &() const
		{
			return this->_ref.get();
		}

		[[nodiscard]] constexpr bool operator<(const node_ref &other) const
		{
			return this->_ref.get() < other._ref.get();
		}

		[[nodiscard]] constexpr bool operator<=(const node_ref &other) const
		{
			return this->_ref.get() <= other._ref.get();
		}

		[[nodiscard]] constexpr bool operator>(const node_ref &other) const
		{
			return this->_ref.get() > other._ref.get();
		}

		[[nodiscard]] constexpr bool operator>=(const node_ref &other) const
		{
			return this->_ref.get() >= other._ref.get();
		}

		[[nodiscard]] constexpr bool operator==(const node_ref &other) const
		{
			return this->_ref.get() == other._ref.get();
		}

		[[nodiscard]] constexpr bool operator!=(const node_ref &other) const
		{
			return this->_ref.get() != other._ref.get();
		}

		constexpr node_ref &operator=(const node_ref &) = default;

		constexpr node_ref() = delete;

		template < typename U >
		constexpr node_ref(U &&ref) :
			_ref(std::forward < U > (ref))
		{
		}

		constexpr node_ref(const node_ref &) = default;
	};
}

export namespace std
{
	template < typename Type >
	requires (std::is_object < Type > ::value)
	struct hash < xablau::graph::node_ref < Type > >
	{
		size_t operator()(const xablau::graph::node_ref < Type > &node) const
		{
			return std::hash < typename std::remove_cv < typename xablau::graph::node_ref < Type > ::type > ::type > {}(node.get());
		}
	};
}