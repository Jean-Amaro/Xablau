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

export module xablau.graph:binary_tree_node;
export import :node_configurations;

export import <functional>;
export import <memory>;
export import <optional>;
export import <type_traits>;
export import <utility>;

export namespace xablau::graph
{
	template < typename Type >
	requires (std::is_object < Type > ::value)
	class binary_tree_node final
	{
	public:
		using value_type = Type;

		using size_type = size_t;

		Type value{};

	private:
		constexpr void update_children() noexcept
		{
			if (this->_left_child)
			{
				this->_left_child->_parent = *this;
			}

			if (this->_right_child)
			{
				this->_right_child->_parent = *this;
			}
		}

		std::optional < std::reference_wrapper < binary_tree_node > > _parent = std::nullopt;

		std::unique_ptr < binary_tree_node > _left_child = nullptr;

		std::unique_ptr < binary_tree_node > _right_child = nullptr;

	public:
		[[nodiscard]] constexpr bool operator<(const binary_tree_node &node) const
		{
			return this->value < node.value;
		}

		[[nodiscard]] constexpr bool operator<=(const binary_tree_node &node) const
		{
			return this->value <= node.value;
		}

		[[nodiscard]] constexpr bool operator==(const binary_tree_node &node) const
		{
			return this->value == node.value;
		}

		[[nodiscard]] constexpr bool operator>=(const binary_tree_node &node) const
		{
			return this->value >= node.value;
		}

		[[nodiscard]] constexpr bool operator>(const binary_tree_node &node) const
		{
			return this->value > node.value;
		}

		[[nodiscard]] constexpr bool operator!=(const binary_tree_node &node) const
		{
			return this->value != node.value;
		}

		constexpr binary_tree_node &insert_left_child(const Type &valueChild)
			noexcept(std::is_nothrow_copy_constructible < Type > ::value)
		{
			this->_left_child = std::make_unique < binary_tree_node > (valueChild);
			this->_left_child->_parent = *this;

			return *(this->_left_child);
		}

		constexpr binary_tree_node &insert_right_child(const Type &valueChild)
			noexcept(std::is_nothrow_copy_constructible < Type > ::value)
		{
			this->_right_child = std::make_unique < binary_tree_node > (valueChild);
			this->_right_child->_parent = *this;

			return *(this->_right_child);
		}

		constexpr binary_tree_node &insert_left_child(Type &&valueChild)
			noexcept(std::is_nothrow_move_constructible < Type > ::value)
		{
			this->_left_child = std::make_unique < binary_tree_node > (std::move(valueChild));
			this->_left_child->_parent = *this;

			return *(this->_left_child);
		}

		constexpr binary_tree_node &insert_right_child(Type &&valueChild)
			noexcept(std::is_nothrow_move_constructible < Type > ::value)
		{
			this->_right_child = std::make_unique < binary_tree_node > (std::move(valueChild));
			this->_right_child->_parent = *this;

			return *(this->_right_child);
		}

		constexpr binary_tree_node &insert_left_child(const binary_tree_node &child)
			noexcept(std::is_nothrow_copy_constructible < binary_tree_node >::value)
		{
			this->_left_child = std::make_unique < binary_tree_node > (child);
			this->_left_child->_parent = *this;

			return *(this->_left_child);
		}

		constexpr binary_tree_node &insert_right_child(const binary_tree_node &child)
			noexcept(std::is_nothrow_copy_constructible < binary_tree_node >::value)
		{
			this->_right_child = std::make_unique < binary_tree_node > (child);
			this->_right_child->_parent = *this;

			return *(this->_right_child);
		}

		constexpr binary_tree_node &insert_left_child(binary_tree_node &&child)
			noexcept(std::is_nothrow_move_constructible < Type >::value)
		{
			this->_left_child = std::make_unique < binary_tree_node > (std::move(child));
			this->_left_child->_parent = *this;

			return *(this->_left_child);
		}

		constexpr binary_tree_node &insert_right_child(binary_tree_node &&child)
			noexcept(std::is_nothrow_move_constructible < Type >::value)
		{
			this->_right_child = std::make_unique < binary_tree_node > (std::move(child));
			this->_right_child->_parent = *this;

			return *(this->_right_child);
		}

		constexpr void erase_left_child() noexcept
		{
			this->_left_child.reset();
		}

		constexpr void erase_right_child() noexcept
		{
			this->_right_child.reset();
		}

		[[nodiscard]] constexpr bool empty() const noexcept
		{
			return this->_left_child == nullptr && this->_right_child == nullptr;
		}

		[[nodiscard]] constexpr binary_tree_node::size_type valid_size() const noexcept
		{
			return
				static_cast < size_t > (this->_left_child != nullptr) +
				static_cast < size_t > (this->_right_child != nullptr);
		}

		[[nodiscard]] constexpr binary_tree_node::size_type size() const noexcept
		{
			return 2;
		}

		constexpr void clear() noexcept
		{
			this->erase_left_child();
			this->erase_right_child();
		}

		[[nodiscard]] constexpr bool has_left_child() const noexcept
		{
			return this->_left_child != nullptr;
		}

		[[nodiscard]] constexpr bool has_right_child() const noexcept
		{
			return this->_right_child != nullptr;
		}

		[[nodiscard]] constexpr bool has_parent() const noexcept
		{
			return this->_parent.has_value();
		}

		[[nodiscard]] constexpr std::optional < std::reference_wrapper < const binary_tree_node > > left_child() const noexcept
		{
			if (this->_left_child)
			{
				return *(this->_left_child.get());
			}

			return std::nullopt;
		}

		[[nodiscard]] constexpr std::optional < std::reference_wrapper < const binary_tree_node > > right_child() const noexcept
		{
			if (this->_right_child)
			{
				return *(this->_right_child.get());
			}

			return std::nullopt;
		}

		[[nodiscard]] constexpr std::optional < std::reference_wrapper < const binary_tree_node > > parent() const noexcept
		{
			if (this->_parent.has_value())
			{
				return this->_parent.value().get();
			}

			return std::nullopt;
		}

		[[nodiscard]] constexpr
			std::optional <
				std::pair <
					std::reference_wrapper < const binary_tree_node >,
					xablau::graph::binary_node_side > > parent_and_my_position() const noexcept
		{
			if (!this->_parent.has_value())
			{
				return std::nullopt;
			}

			auto &parent = this->_parent.value().get();

			std::pair < std::reference_wrapper < const binary_tree_node >, xablau::graph::binary_node_side > pair(
				parent,
				xablau::graph::binary_node_side::left);

			if (parent._right_child.get() == this)
			{
				pair.second = xablau::graph::binary_node_side::right;
			}

			return pair;
		}

		[[nodiscard]] constexpr std::optional < std::reference_wrapper < binary_tree_node > > left_child() noexcept
		{
			if (this->_left_child)
			{
				return *(this->_left_child.get());
			}

			return std::nullopt;
		}

		[[nodiscard]] constexpr std::optional < std::reference_wrapper < binary_tree_node > > right_child() noexcept
		{
			if (this->_right_child)
			{
				return *(this->_right_child.get());
			}

			return std::nullopt;
		}

		[[nodiscard]] constexpr std::optional < std::reference_wrapper < binary_tree_node > > parent() noexcept
		{
			if (this->_parent.has_value())
			{
				return this->_parent.value().get();
			}

			return std::nullopt;
		}

		[[nodiscard]] constexpr
			std::optional <
				std::pair <
					std::reference_wrapper < binary_tree_node >,
					xablau::graph::binary_node_side > > parent_and_my_position() noexcept
		{
			if (!this->_parent.has_value())
			{
				return std::nullopt;
			}

			auto &parent = this->_parent.value().get();

			std::pair < std::reference_wrapper < binary_tree_node >, xablau::graph::binary_node_side > pair(
				parent,
				xablau::graph::binary_node_side::left);

			if (parent._right_child.get() == this)
			{
				pair.second = xablau::graph::binary_node_side::right;
			}

			return pair;
		}

		constexpr
			std::optional <
				std::pair <
					std::unique_ptr < binary_tree_node >,
					xablau::graph::binary_node_side > > extract_myself() noexcept
		{
			if (!this->_parent.has_value())
			{
				return std::nullopt;
			}

			std::unique_ptr < binary_tree_node > myself{};
			auto &parent = this->_parent.value().get();

			if (parent._left_child.get() == this)
			{
				parent._left_child.swap(myself);

				this->_parent.reset();

				return std::make_pair(std::move(myself), xablau::graph::binary_node_side::left);
			}

			parent._right_child.swap(myself);

			this->_parent.reset();

			return std::make_pair(std::move(myself), xablau::graph::binary_node_side::right);
		}

		constexpr std::unique_ptr < binary_tree_node > extract_left_child() noexcept
		{
			if (this->_left_child)
			{
				this->_left_child->_parent.reset();

				return std::move(this->_left_child);
			}

			return nullptr;
		}

		constexpr std::unique_ptr < binary_tree_node > extract_right_child() noexcept
		{
			if (this->_right_child)
			{
				this->_right_child->_parent.reset();

				return std::move(this->_right_child);
			}

			return nullptr;
		}

		constexpr void swap(binary_tree_node &node)
			noexcept(
				std::is_nothrow_move_constructible < Type >::value &&
				std::is_nothrow_move_assignable < Type > ::value)
		{
			std::swap(this->value, node.value);
			std::swap(this->_parent, node._parent);
			std::swap(this->_left_child, node._left_child);
			std::swap(this->_right_child, node._right_child);
		}

		constexpr binary_tree_node &operator=(const binary_tree_node &node)
			noexcept(
				std::is_nothrow_copy_assignable < Type > ::value &&
				std::is_nothrow_copy_constructible < Type > ::value)
		{
			if (this == &node)
			{
				return *this;
			}

			this->value = node.value;
			this->_left_child.reset(node._left_child ? std::make_unique < binary_tree_node > (*node._left_child) : nullptr);
			this->_right_child.reset(node._right_child ? std::make_unique < binary_tree_node > (*node._right_child) : nullptr);

			this->update_children();

			return *this;
		}

		constexpr binary_tree_node &operator=(binary_tree_node &&node)
			noexcept(std::is_nothrow_move_assignable < Type >::value)
		{
			this->value = std::move(node.value);
			this->_left_child = std::move(node._left_child);
			this->_right_child = std::move(node._right_child);

			this->update_children();

			node.extract_myself();

			return *this;
		}

		constexpr binary_tree_node()
			noexcept(std::is_nothrow_default_constructible < Type > ::value) = default;

		constexpr binary_tree_node(const Type &data)
			noexcept(std::is_nothrow_copy_constructible < Type > ::value) :

			value(data)
		{
		}

		constexpr binary_tree_node(Type &&data)
			noexcept(std::is_nothrow_move_constructible < Type > ::value) :

			value(std::move(data))
		{
		}

		constexpr binary_tree_node(const binary_tree_node &node)
			noexcept(std::is_nothrow_copy_constructible < Type > ::value) :

			value(node.value),
			_left_child(node._left_child ? std::make_unique < binary_tree_node > (*(node._left_child.get())) : nullptr),
			_right_child(node._right_child ? std::make_unique < binary_tree_node > (*(node._right_child.get())) : nullptr)
		{
			this->update_children();
		}

		constexpr binary_tree_node(binary_tree_node &&node)
			noexcept(std::is_nothrow_move_constructible < Type >::value) :

			value(std::move(node.value)),
			_left_child(std::move(node._left_child)),
			_right_child(std::move(node._right_child))
		{
			this->update_children();

			node.extract_myself();
		}
	};
}

export namespace std
{
	template < typename Type >
	requires (std::is_object < Type > ::value)
	struct hash < xablau::graph::binary_tree_node < Type > >
	{
		size_t operator()(const xablau::graph::binary_tree_node < Type > &node) const
		{
			return std::hash < Type > {}(node.value);
		}
	};
}