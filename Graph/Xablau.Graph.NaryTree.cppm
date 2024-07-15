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

export module xablau.graph:nary_tree;
export import :fundamental_concepts;

export import std;

export namespace xablau::graph
{
	template < concepts::nary_node_with_parent NodeType >
	class nary_tree final
	{
	public:
		class const_pre_order_iterator;
		class pre_order_iterator;
		class const_post_order_iterator;
		class post_order_iterator;

		using const_reverse_pre_order_iterator = std::reverse_iterator < const_pre_order_iterator >;
		using reverse_pre_order_iterator = std::reverse_iterator < pre_order_iterator >;
		using const_reverse_post_order_iterator = std::reverse_iterator < const_post_order_iterator >;
		using reverse_post_order_iterator = std::reverse_iterator < post_order_iterator >;

		using const_iterator = const_pre_order_iterator;
		using iterator = pre_order_iterator;

		using const_reverse_iterator = const_reverse_pre_order_iterator;
		using reverse_iterator = reverse_pre_order_iterator;

		using value_type = typename NodeType::value_type;
		using node_type = NodeType;
		using size_type = typename NodeType::size_type;

	private:
		struct FirstElement
		{
			pre_order_iterator _preOrder;
			post_order_iterator _postOrder;
		};

		struct LastElement
		{
			pre_order_iterator _preOrder;
			post_order_iterator _postOrder;
		};

	public:
		class pre_order_iterator final :
			public std::bidirectional_iterator_tag
		{
		private:
			friend nary_tree < NodeType >;

			std::optional < std::reference_wrapper < LastElement > > _last_element = std::nullopt;

			std::optional < std::reference_wrapper < NodeType > > _node = std::nullopt;

			[[nodiscard]] constexpr pre_order_iterator(std::optional < std::reference_wrapper < NodeType > > node, LastElement &last_element) noexcept :
				_last_element(last_element),
				_node(node)
			{
			}

		public:
			using difference_type = std::ptrdiff_t;
			using value_type = nary_tree::value_type;
			using pointer = nary_tree::value_type *;
			using reference = nary_tree::value_type &;
			using iterator_category = std::bidirectional_iterator_tag;

			[[nodiscard]] constexpr operator bool() const
			{
				return this->_node.has_value();
			}

			[[nodiscard]] constexpr bool operator==(const const_pre_order_iterator &iterator) const noexcept
			{
				if (this->_node.has_value() && iterator._node.has_value())
				{
					return std::addressof(this->_node.value().get()) == std::addressof(iterator._node.value().get());
				}

				if (!this->_node.has_value() && !iterator._node.has_value())
				{
					return true;
				}

				return false;
			}

			[[nodiscard]] constexpr bool operator==(const pre_order_iterator &iterator) const noexcept
			{
				if (this->_node.has_value() && iterator._node.has_value())
				{
					return std::addressof(this->_node.value().get()) == std::addressof(iterator._node.value().get());
				}

				if (!this->_node.has_value() && !iterator._node.has_value())
				{
					return true;
				}

				return false;
			}

			constexpr pre_order_iterator &operator=(const pre_order_iterator &) noexcept = default;

			constexpr pre_order_iterator &operator=(pre_order_iterator &&) noexcept = default;

			constexpr pre_order_iterator &operator++()
			{
				if (this->_node.has_value())
				{
					auto &node1 = this->_node.value().get();

					if (!node1.empty())
					{
						this->_node = node1.front_child().value().get();

						return *this;
					}

					auto node2 = this->_node;

					while (node2.has_value())
					{
						auto &currentNode = node2.value().get();
						auto rightSibling = currentNode.right_valid_sibling();

						if (rightSibling.has_value())
						{
							this->_node = rightSibling.value().get();

							return *this;
						}

						node2 = currentNode.parent();
					}

					this->_node.reset();

					return *this;
				}

				throw std::out_of_range("Increment of an invalid iterator.");
			}

			constexpr pre_order_iterator operator++(int)
			{
				pre_order_iterator _iterator(*this);

				operator++();

				return _iterator;
			}

			constexpr pre_order_iterator &operator--()
			{
				if (this->_node.has_value())
				{
					auto leftSibling = this->_node.value().get().left_valid_sibling();

					if (leftSibling.has_value())
					{
						auto rightmostChild = leftSibling;

						while (rightmostChild.value().get().valid_back_child() != std::nullopt)
						{
							rightmostChild = rightmostChild.value().get().valid_back_child();
						}

						this->_node = rightmostChild.value().get();

						return *this;
					}

					auto parent = this->_node.value().get().parent();

					if (parent.has_value())
					{
						this->_node = parent.value().get();

						return *this;
					}

					throw std::out_of_range("Decrement of an invalid iterator.");
				}

				if (this->_last_element.has_value() && this->_last_element.value().get()._preOrder._node.has_value())
				{
					this->_node = this->_last_element.value().get()._preOrder._node.value().get();

					return *this;
				}

				throw std::out_of_range("Decrement of an invalid iterator.");
			}

			constexpr pre_order_iterator operator--(int)
			{
				pre_order_iterator _iterator(*this);

				operator--();

				return _iterator;
			}

			[[nodiscard]] constexpr reference operator*() const
			{
				if (this->_node.has_value())
				{
					return this->_node.value().get().value();
				}

				throw std::logic_error("Attempt of getting the reference of an invalid iterator.");
			}

			[[nodiscard]] constexpr pointer operator->() const
			{
				if (this->_node.has_value())
				{
					return &(this->_node.value().get().value());
				}

				throw std::logic_error("Attempt of getting the pointer of an invalid iterator.");
			}

			[[nodiscard]] constexpr std::optional < std::reference_wrapper < const NodeType > > node() const noexcept
			{
				if (this->_node.has_value())
				{
					return this->_node.value().get();
				}

				return std::nullopt;
			}

			[[nodiscard]] constexpr post_order_iterator convert_to_post_order() const noexcept
			{
				if (this->_last_element.has_value())
				{
					return post_order_iterator(this->_node, this->_last_element.value().get());
				}

				return post_order_iterator{};
			}

			constexpr void swap(pre_order_iterator &iterator) noexcept
			{
				std::swap(this->_node, iterator._node);
				std::swap(this->_last_element, iterator._last_element);
			}

			constexpr pre_order_iterator() noexcept = default;

			constexpr pre_order_iterator(const pre_order_iterator &) noexcept = default;

			constexpr pre_order_iterator(pre_order_iterator &&) noexcept = default;
		};

		class const_pre_order_iterator final :
			public std::bidirectional_iterator_tag
		{
		private:
			friend nary_tree < NodeType >;

			std::optional < std::reference_wrapper < LastElement > > _last_element = std::nullopt;

			std::optional < std::reference_wrapper < NodeType > > _node = std::nullopt;

			[[nodiscard]] constexpr const_pre_order_iterator(std::optional < std::reference_wrapper < NodeType > > node, LastElement &last_element) noexcept :
				_last_element(last_element),
				_node(node)
			{
			}

		public:
			using difference_type = std::ptrdiff_t;
			using value_type = nary_tree::value_type;
			using pointer = const nary_tree::value_type *;
			using reference = const nary_tree::value_type &;
			using iterator_category = std::bidirectional_iterator_tag;

			[[nodiscard]] constexpr operator bool() const
			{
				return this->_node.has_value();
			}

			[[nodiscard]] constexpr bool operator==(const const_pre_order_iterator &iterator) const noexcept
			{
				if (this->_node.has_value() && iterator._node.has_value())
				{
					return std::addressof(this->_node.value().get()) == std::addressof(iterator._node.value().get());
				}

				if (!this->_node.has_value() && !iterator._node.has_value())
				{
					return true;
				}

				return false;
			}

			[[nodiscard]] constexpr bool operator==(const pre_order_iterator &iterator) const noexcept
			{
				if (this->_node.has_value() && iterator._node.has_value())
				{
					return std::addressof(this->_node.value().get()) == std::addressof(iterator._node.value().get());
				}

				if (!this->_node.has_value() && !iterator._node.has_value())
				{
					return true;
				}

				return false;
			}

			constexpr const_pre_order_iterator &operator=(const const_pre_order_iterator &) noexcept = default;

			constexpr const_pre_order_iterator &operator=(const_pre_order_iterator &&) noexcept = default;

			constexpr const_pre_order_iterator &operator=(const pre_order_iterator &iterator) noexcept
			{
				this->_node = iterator._node;
				this->_last_element = iterator._last_element;

				return *this;
			}

			constexpr const_pre_order_iterator &operator++()
			{
				if (this->_node.has_value())
				{
					auto &node1 = this->_node.value().get();

					if (!node1.empty())
					{
						this->_node = node1.front_child().value().get();

						return *this;
					}

					std::optional < std::reference_wrapper < NodeType > > node2 = this->_node;

					do
					{
						auto rightSibling = node2.value().get().right_valid_sibling();

						if (rightSibling.has_value())
						{
							this->_node = rightSibling.value().get();

							return *this;
						}

						node2 = node2.value().get().parent();
					} while (node2.has_value());

					this->_node.reset();

					return *this;
				}

				throw std::out_of_range("Increment of an invalid iterator.");
			}

			constexpr const_pre_order_iterator operator++(int)
			{
				const_pre_order_iterator _iterator(*this);

				operator++();

				return _iterator;
			}

			constexpr const_pre_order_iterator &operator--()
			{
				if (this->_node.has_value())
				{
					auto leftSibling = this->_node.value().get().left_valid_sibling();

					if (leftSibling.has_value())
					{
						auto rightmostChild = leftSibling;

						while (rightmostChild.value().get().valid_back_child() != std::nullopt)
						{
							rightmostChild = rightmostChild.value().get().valid_back_child();
						}

						this->_node = rightmostChild.value().get();

						return *this;
					}

					auto parent = this->_node.value().get().parent();

					if (parent.has_value())
					{
						this->_node = parent.value().get();

						return *this;
					}

					throw std::out_of_range("Decrement of an invalid iterator.");
				}

				if (this->_last_element.has_value() && this->_last_element.value().get()._preOrder._node.has_value())
				{
					this->_node = this->_last_element.value().get()._preOrder._node.value().get();

					return *this;
				}

				throw std::out_of_range("Decrement of an invalid iterator.");
			}

			constexpr const_pre_order_iterator operator--(int)
			{
				const_pre_order_iterator _iterator(*this);

				operator--();

				return _iterator;
			}

			[[nodiscard]] constexpr reference operator*() const
			{
				if (this->_node.has_value())
				{
					return this->_node.value().get().value();
				}

				throw std::logic_error("Attempt of getting the reference of an invalid iterator.");
			}

			[[nodiscard]] constexpr pointer operator->() const
			{
				if (this->_node.has_value())
				{
					return &(this->_node.value().get().value());
				}

				throw std::logic_error("Attempt of getting the pointer of an invalid iterator.");
			}

			[[nodiscard]] constexpr std::optional < std::reference_wrapper < const NodeType > > node() const noexcept
			{
				if (this->_node.has_value())
				{
					return this->_node.value().get();
				}

				return std::nullopt;
			}

			[[nodiscard]] constexpr const_post_order_iterator convert_to_post_order() const noexcept
			{
				if (this->_last_element.has_value())
				{
					return const_post_order_iterator(this->_node, this->_last_element.value().get());
				}

				return const_post_order_iterator{};
			}

			constexpr void swap(const_pre_order_iterator &iterator) noexcept
			{
				std::swap(this->_node, iterator._node);
				std::swap(this->_last_element, iterator._last_element);
			}

			constexpr const_pre_order_iterator() noexcept = default;

			constexpr const_pre_order_iterator(const pre_order_iterator &iterator) noexcept :
				_last_element(iterator._last_element),
				_node(iterator._node)
			{
			}

			constexpr const_pre_order_iterator(const const_pre_order_iterator &) noexcept = default;

			constexpr const_pre_order_iterator(const_pre_order_iterator &&) noexcept = default;
		};

		class post_order_iterator final :
			public std::bidirectional_iterator_tag
		{
		private:
			friend nary_tree < NodeType >;

			std::optional < std::reference_wrapper < LastElement > > _last_element = std::nullopt;

			std::optional < std::reference_wrapper < NodeType > > _node = std::nullopt;

			[[nodiscard]] constexpr post_order_iterator(std::optional < std::reference_wrapper < NodeType > > node, LastElement &last_element) noexcept :
				_last_element(last_element),
				_node(node)
			{
			}

		public:
			using difference_type = std::ptrdiff_t;
			using value_type = nary_tree::value_type;
			using pointer = nary_tree::value_type *;
			using reference = nary_tree::value_type &;
			using iterator_category = std::bidirectional_iterator_tag;

			[[nodiscard]] constexpr operator bool() const
			{
				return this->_node.has_value();
			}

			[[nodiscard]] constexpr bool operator==(const const_pre_order_iterator &iterator) const noexcept
			{
				if (this->_node.has_value() && iterator._node.has_value())
				{
					return std::addressof(this->_node.value().get()) == std::addressof(iterator._node.value().get());
				}

				if (!this->_node.has_value() && !iterator._node.has_value())
				{
					return true;
				}

				return false;
			}

			[[nodiscard]] constexpr bool operator==(const post_order_iterator &iterator) const noexcept
			{
				if (this->_node.has_value() && iterator._node.has_value())
				{
					return std::addressof(this->_node.value().get()) == std::addressof(iterator._node.value().get());
				}

				if (!this->_node.has_value() && !iterator._node.has_value())
				{
					return true;
				}

				return false;
			}

			constexpr post_order_iterator &operator=(const post_order_iterator &) noexcept = default;

			constexpr post_order_iterator &operator=(post_order_iterator &&) noexcept = default;

			constexpr post_order_iterator &operator++()
			{
				if (this->_node.has_value())
				{
					auto &node = this->_node.value().get();

					if (node.has_parent())
					{
						const auto rightSibling = node.right_valid_sibling();

						if (rightSibling.has_value())
						{
							auto leftmostChild = rightSibling;

							while (leftmostChild.value().get().valid_front_child() != std::nullopt)
							{
								leftmostChild = leftmostChild.value().get().valid_front_child();
							}

							this->_node = leftmostChild.value().get();
						}

						else
						{
							this->_node = node.parent().value().get();
						}

						return *this;
					}

					this->_node.reset();

					return *this;
				}

				throw std::out_of_range("Increment of an invalid iterator.");
			}

			constexpr post_order_iterator operator++(int)
			{
				post_order_iterator _iterator(*this);

				operator++();

				return _iterator;
			}

			constexpr post_order_iterator &operator--()
			{
				if (this->_node.has_value())
				{
					const auto backChild = this->_node.value().get().valid_back_child();

					if (backChild.has_value())
					{
						this->_node = backChild.value().get();

						return *this;
					}

					auto node = this->_node;

					while (node.has_value())
					{
						auto &currentNode = node.value().get();
						auto leftSibling = currentNode.left_valid_sibling();

						if (leftSibling.has_value())
						{
							this->_node = currentNode.left_valid_sibling().value().get();

							return *this;
						}

						node = node.value().get().parent();
					}

					throw std::out_of_range("Decrement of an invalid iterator.");
				}

				if (this->_last_element.has_value() && this->_last_element.value().get()._postOrder._node.has_value())
				{
					this->_node = this->_last_element.value().get()._postOrder._node.value().get();

					return *this;
				}

				throw std::out_of_range("Decrement of an invalid iterator.");
			}

			constexpr post_order_iterator operator--(int)
			{
				post_order_iterator _iterator(*this);

				operator--();

				return _iterator;
			}

			[[nodiscard]] constexpr reference operator*() const
			{
				if (this->_node.has_value())
				{
					return this->_node.value().get().value();
				}

				throw std::logic_error("Attempt of getting the reference of an invalid iterator.");
			}

			[[nodiscard]] constexpr pointer operator->() const
			{
				if (this->_node.has_value())
				{
					return &(this->_node.value().get().value());
				}

				throw std::logic_error("Attempt of getting the pointer of an invalid iterator.");
			}

			[[nodiscard]] constexpr std::optional < std::reference_wrapper < const NodeType > > node() const noexcept
			{
				if (this->_node.has_value())
				{
					return this->_node.value().get();
				}

				return std::nullopt;
			}

			[[nodiscard]] constexpr pre_order_iterator convert_to_pre_order() const noexcept
			{
				if (this->_last_element.has_value())
				{
					return pre_order_iterator(this->_node, this->_last_element.value().get());
				}

				return pre_order_iterator{};
			}

			constexpr void swap(post_order_iterator &iterator) noexcept
			{
				std::swap(this->_node, iterator._node);
				std::swap(this->_last_element, iterator._last_element);
			}

			constexpr post_order_iterator() noexcept = default;

			constexpr post_order_iterator(const post_order_iterator &) noexcept = default;

			constexpr post_order_iterator(post_order_iterator &&) noexcept = default;
		};

		class const_post_order_iterator final :
			public std::bidirectional_iterator_tag
		{
		private:
			friend nary_tree < NodeType >;

			std::optional < std::reference_wrapper < LastElement > > _last_element = std::nullopt;

			std::optional < std::reference_wrapper < NodeType > > _node = std::nullopt;

			[[nodiscard]] constexpr const_post_order_iterator(std::optional < std::reference_wrapper < NodeType > > node, LastElement &last_element) noexcept :
				_last_element(last_element),
				_node(node)
			{
			}

		public:
			using difference_type = std::ptrdiff_t;
			using value_type = nary_tree::value_type;
			using pointer = const nary_tree::value_type *;
			using reference = const nary_tree::value_type &;
			using iterator_category = std::bidirectional_iterator_tag;

			[[nodiscard]] constexpr operator bool() const
			{
				return this->_node.has_value();
			}

			[[nodiscard]] constexpr bool operator==(const const_post_order_iterator &iterator) const noexcept
			{
				if (this->_node.has_value() && iterator._node.has_value())
				{
					return std::addressof(this->_node.value().get()) == std::addressof(iterator._node.value().get());
				}

				if (!this->_node.has_value() && !iterator._node.has_value())
				{
					return true;
				}

				return false;
			}

			[[nodiscard]] constexpr bool operator==(const post_order_iterator &iterator) const noexcept
			{
				if (this->_node.has_value() && iterator._node.has_value())
				{
					return std::addressof(this->_node.value().get()) == std::addressof(iterator._node.value().get());
				}

				if (!this->_node.has_value() && !iterator._node.has_value())
				{
					return true;
				}

				return false;
			}

			constexpr const_post_order_iterator &operator=(const const_post_order_iterator &) noexcept = default;

			constexpr const_post_order_iterator &operator=(const_post_order_iterator &&) noexcept = default;

			constexpr const_post_order_iterator &operator=(const post_order_iterator &iterator) noexcept
			{
				this->_node = iterator._node;
				this->_last_element = iterator._last_element;

				return *this;
			}

			constexpr const_post_order_iterator &operator++()
			{
				if (this->_node.has_value())
				{
					auto &node = this->_node.value().get();

					if (node.has_parent())
					{
						const auto rightSibling = node.right_valid_sibling();

						if (rightSibling.has_value())
						{
							auto leftmostChild = rightSibling;

							while (leftmostChild.value().get().valid_front_child() != std::nullopt)
							{
								leftmostChild = leftmostChild.value().get().valid_front_child();
							}

							this->_node = leftmostChild.value().get();
						}

						else
						{
							this->_node = node.parent().value().get();
						}

						return *this;
					}

					this->_node.reset();

					return *this;
				}

				throw std::out_of_range("Increment of an invalid iterator.");
			}

			constexpr const_post_order_iterator operator++(int)
			{
				const_post_order_iterator _iterator(*this);

				operator++();

				return _iterator;
			}

			constexpr const_post_order_iterator &operator--()
			{
				if (this->_node.has_value())
				{
					const auto backChild = this->_node.value().get().valid_back_child();

					if (backChild.has_value())
					{
						this->_node = backChild.value().get();

						return *this;
					}

					auto node = this->_node;

					while (node.has_value())
					{
						auto &currentNode = node.value().get();
						auto leftSibling = currentNode.left_valid_sibling();

						if (leftSibling.has_value())
						{
							this->_node = currentNode.left_valid_sibling().value().get();

							return *this;
						}

						node = node.value().get().parent();
					}

					throw std::out_of_range("Decrement of an invalid iterator.");
				}

				if (this->_last_element.has_value() && this->_last_element.value().get()._postOrder._node.has_value())
				{
					this->_node = this->_last_element.value().get()._postOrder._node.value().get();

					return *this;
				}

				throw std::out_of_range("Decrement of an invalid iterator.");
			}

			constexpr const_post_order_iterator operator--(int)
			{
				const_post_order_iterator _iterator(*this);

				operator--();

				return _iterator;
			}

			[[nodiscard]] constexpr reference operator*() const
			{
				if (this->_node.has_value())
				{
					return this->_node.value().get().value();
				}

				throw std::logic_error("Attempt of getting the reference of an invalid iterator.");
			}

			[[nodiscard]] constexpr pointer operator->() const
			{
				if (this->_node.has_value())
				{
					return &(this->_node.value().get().value());
				}

				throw std::logic_error("Attempt of getting the pointer of an invalid iterator.");
			}

			[[nodiscard]] constexpr std::optional < std::reference_wrapper < const NodeType > > node() const noexcept
			{
				if (this->_node.has_value())
				{
					return this->_node.value().get();
				}

				return std::nullopt;
			}

			[[nodiscard]] constexpr const_pre_order_iterator convert_to_pre_order() const noexcept
			{
				if (this->_last_element.has_value())
				{
					return const_pre_order_iterator(this->_node, this->_last_element.value().get());
				}

				return const_pre_order_iterator{};
			}

			constexpr void swap(const_post_order_iterator &iterator) noexcept
			{
				std::swap(this->_node, iterator._node);
				std::swap(this->_last_element, iterator._last_element);
			}

			constexpr const_post_order_iterator() noexcept = default;

			constexpr const_post_order_iterator(const post_order_iterator &iterator) noexcept :
				_last_element(iterator._last_element),
				_node(iterator._node)
			{
			}

			constexpr const_post_order_iterator(const const_post_order_iterator &) noexcept = default;

			constexpr const_post_order_iterator(const_post_order_iterator &&) noexcept = default;
		};

	private:
		nary_tree::size_type _size{};

		std::unique_ptr < NodeType > _root = nullptr;

		mutable FirstElement _begin{};

		mutable LastElement _last_element{};

		[[nodiscard]] static constexpr std::optional < std::reference_wrapper < NodeType > > &node(pre_order_iterator &iterator) noexcept
		{
			return iterator._node;
		}

		[[nodiscard]] static constexpr std::optional < std::reference_wrapper < NodeType > > &node(const_pre_order_iterator &iterator) noexcept
		{
			return iterator._node;
		}

		[[nodiscard]] static constexpr std::optional < std::reference_wrapper < LastElement > > &last_element(pre_order_iterator &iterator) noexcept
		{
			return iterator._last_element;
		}

		[[nodiscard]] static constexpr std::optional < std::reference_wrapper < LastElement > > &last_element(const_pre_order_iterator &iterator) noexcept
		{
			return iterator._last_element;
		}

		[[nodiscard]] static constexpr std::optional < std::reference_wrapper < NodeType > > &node(post_order_iterator &iterator) noexcept
		{
			return iterator._node;
		}

		[[nodiscard]] static constexpr std::optional < std::reference_wrapper < NodeType > > &node(const_post_order_iterator &iterator) noexcept
		{
			return iterator._node;
		}

		[[nodiscard]] static constexpr std::optional < std::reference_wrapper < LastElement > > &last_element(post_order_iterator &iterator) noexcept
		{
			return iterator._last_element;
		}

		[[nodiscard]] static constexpr std::optional < std::reference_wrapper < LastElement > > &last_element(const_post_order_iterator &iterator) noexcept
		{
			return iterator._last_element;
		}

		static constexpr void leaf_nodes(
			const NodeType &node,
			std::vector < std::reference_wrapper < const NodeType > > &leaf_nodes)
		{
			if (node.valid_size() == 0)
			{
				leaf_nodes.push_back(node);
			}

			else
			{
				const auto children = node.valid_children();

				for (const auto &child : children)
				{
					nary_tree::leaf_nodes(child.get(), leaf_nodes);
				}
			}
		}

		static constexpr void nodes(
			const NodeType &node,
			std::vector < std::reference_wrapper < const NodeType > > &nodes)
		{
			nodes.push_back(node);

			const auto children = node.valid_children();

			for (const auto &child : children)
			{
				nary_tree::nodes(child.get(), nodes);
			}
		}

		static constexpr void level_count(
			const NodeType &node,
			const nary_tree::size_type currentLevel,
			nary_tree::size_type &levelCount) noexcept
		{
			if (currentLevel + 1 > levelCount)
			{
				levelCount = currentLevel + 1;
			}

			const auto children = node.valid_children();

			for (const auto &child : children)
			{
				nary_tree::level_count(child.get(), currentLevel + 1, levelCount);
			}
		}

		static constexpr void nodes_per_level(
			const NodeType &node,
			const nary_tree::size_type currentLevel,
			std::vector < nary_tree::size_type > &nodesPerLevel)
		{
			if (nodesPerLevel.size() < currentLevel + 1)
			{
				nodesPerLevel.resize(currentLevel + 1, 0);
			}

			++(nodesPerLevel[currentLevel]);

			const auto children = node.valid_children();

			for (const auto &child : children)
			{
				nary_tree::nodes_per_level(child.get(), currentLevel + 1, nodesPerLevel);
			}
		}

		[[nodiscard]] static constexpr bool equal(
			const std::optional < std::reference_wrapper < const NodeType > > &node1,
			const std::optional < std::reference_wrapper < const NodeType > > &node2)
		{
			const bool node1HasValue = node1.has_value();
			const bool node2HasValue = node2.has_value();

			if (node1HasValue && node2HasValue)
			{
				const auto &_node1 = node1.value().get();
				const auto &_node2 = node2.value().get();

				if (_node1.size() != _node2.size())
				{
					return false;
				}

				constexpr auto equalChildren =
					[] (const NodeType &node1, const NodeType &node2) -> bool
					{
						const auto children1 = node1.children();
						const auto children2 = node2.children();

						for (nary_tree::size_type i = 0; i < children1.size(); i++)
						{
							if (!nary_tree::equal(children1[i], children2[i]))
							{
								return false;
							}
						}

						return true;
					};

				return (_node1 == _node2) && equalChildren(_node1, _node2);
			}

			if (!node1HasValue && !node2HasValue)
			{
				return true;
			}

			return false;
		}

		template < typename Iterator >
		static constexpr void find(
			NodeType &node,
			const nary_tree::value_type &value,
			Iterator &iterator)
		{
			if (node.value() == value)
			{
				iterator._node = node;

				return;
			}

			const auto children = node.valid_children();

			for (nary_tree::size_type i = 0; i < children.size(); i++)
			{
				nary_tree::find(children[i].get(), value, iterator);

				if (iterator._node.has_value())
				{
					return;
				}
			}
		}

		template < typename Iterator >
		static constexpr void find_with_path(
			NodeType &node,
			const nary_tree::value_type &value,
			std::vector < nary_tree::size_type > &path,
			Iterator &iterator)
		{
			if (node.value() == value)
			{
				iterator._node = node;

				return;
			}

			const auto children = node.valid_children();

			if (!children.empty())
			{
				path.push_back(nary_tree::size_type{});

				for (nary_tree::size_type i = 0; i < children.size(); i++)
				{
					path.back() = i;

					nary_tree::find_with_path(children[i].get(), value, path, iterator);

					if (iterator._node.has_value())
					{
						return;
					}
				}

				path.pop_back();
			}
		}

		constexpr void update_begin() noexcept
		{
			if (this->_root)
			{
				std::reference_wrapper < NodeType > node = *(this->_root.get());

				this->_begin._preOrder._node = node.get();

				while (node.get().valid_front_child() != std::nullopt)
				{
					node = node.get().valid_front_child().value().get();
				}

				this->_begin._postOrder._node = node.get();
			}

			else
			{
				this->_begin._preOrder._node.reset();
				this->_begin._postOrder._node.reset();
			}
		}

		constexpr void update_last_element() noexcept
		{
			if (this->_root)
			{
				std::reference_wrapper < NodeType > node = *(this->_root.get());

				this->_last_element._postOrder._node = node.get();

				while (node.get().valid_back_child() != std::nullopt)
				{
					node = node.get().valid_back_child().value().get();
				}

				this->_last_element._preOrder._node = node.get();
			}

			else
			{
				this->_last_element._preOrder._node.reset();
				this->_last_element._postOrder._node.reset();
			}
		}

		[[nodiscard]] constexpr std::vector < const_pre_order_iterator > upward_chain_begin(const_pre_order_iterator begin)
		{
			std::vector < const_pre_order_iterator > verticalChain;

			while (begin != this->begin())
			{
				verticalChain.push_back(begin);

				const auto parent = begin._node.value().get().parent();

				if (parent.has_value())
				{
					begin._node = parent.value().get();
				}
			}

			verticalChain.push_back(std::move(begin));

			return verticalChain;
		}

		[[nodiscard]] constexpr std::vector < const_pre_order_iterator > upward_chain_end(const_pre_order_iterator end)
		{
			const auto begin = this->begin();
			std::vector < const_pre_order_iterator > verticalChain;

			--end;

			while (end != begin)
			{
				verticalChain.push_back(end);

				const auto parent = end._node.value().get().parent();

				if (parent.has_value())
				{
					end._node = parent.value().get();
				}
			}

			verticalChain.push_back(std::move(end));

			return verticalChain;
		}

		[[nodiscard]] constexpr bool iterator_from_this_tree(const_pre_order_iterator &iterator) const noexcept
		{
			return iterator._last_element.has_value() && this->_last_element._preOrder == iterator._last_element.value().get()._preOrder;
		}

		[[nodiscard]] constexpr bool iterator_from_this_tree(const_post_order_iterator &iterator) const noexcept
		{
			return iterator._last_element.has_value() && this->_last_element._postOrder == iterator._last_element.value().get()._postOrder;
		}

		[[nodiscard]] static constexpr nary_tree::size_type subtree_size(const NodeType &node)
		{
			nary_tree::size_type counter{};
			const auto children = node.valid_children();

			for (const auto &child : children)
			{
				counter += nary_tree::subtree_size(child.get());
			}

			return counter + 1;
		}

		[[nodiscard]] static constexpr bool binary(const NodeType &node)
		{
			const auto children = node.valid_children();

			if (children.size() > 2)
			{
				return false;
			}

			for (const auto &child : children)
			{
				if (!nary_tree::binary(child.get()))
				{
					return false;
				}
			}

			return true;
		}

	public:
		[[nodiscard]] constexpr bool operator==(const nary_tree &tree) const
		{
			if (this->_root && tree._root)
			{
				return nary_tree::equal(*(this->_root.get()), *(tree._root.get()));
			}

			if (!this->_root && !tree._root)
			{
				return true;
			}

			return false;
		}

		[[nodiscard]] constexpr pre_order_iterator begin() noexcept
		{
			pre_order_iterator iterator{};

			iterator._node = this->_begin._preOrder._node;
			iterator._last_element = this->_last_element;

			return iterator;
		}

		[[nodiscard]] constexpr const_pre_order_iterator begin() const noexcept
		{
			const_pre_order_iterator iterator{};

			iterator._node = this->_begin._preOrder._node;
			iterator._last_element = this->_last_element;

			return iterator;
		}

		[[nodiscard]] constexpr const_pre_order_iterator cbegin() const noexcept
		{
			return this->begin();
		}

		[[nodiscard]] constexpr pre_order_iterator end() noexcept
		{
			pre_order_iterator iterator{};

			iterator._last_element = this->_last_element;

			return iterator;
		}

		[[nodiscard]] constexpr const_pre_order_iterator end() const noexcept
		{
			const_pre_order_iterator iterator{};

			iterator._last_element = this->_last_element;

			return iterator;
		}

		[[nodiscard]] constexpr const_pre_order_iterator cend() const noexcept
		{
			return this->end();
		}

		[[nodiscard]] constexpr reverse_pre_order_iterator rbegin() noexcept
		{
			return reverse_iterator(this->end());
		}

		[[nodiscard]] constexpr const_reverse_pre_order_iterator rbegin() const noexcept
		{
			return const_reverse_iterator(this->cend());
		}

		[[nodiscard]] constexpr const_reverse_pre_order_iterator crbegin() const noexcept
		{
			return const_reverse_iterator(this->cend());
		}

		[[nodiscard]] constexpr reverse_pre_order_iterator rend() noexcept
		{
			return reverse_iterator(this->begin());
		}

		[[nodiscard]] constexpr const_reverse_pre_order_iterator rend() const noexcept
		{
			return const_reverse_iterator(this->cbegin());
		}

		[[nodiscard]] constexpr const_reverse_pre_order_iterator crend() const noexcept
		{
			return const_reverse_iterator(this->cbegin());
		}

		[[nodiscard]] constexpr pre_order_iterator pre_order_begin() noexcept
		{
			pre_order_iterator iterator{};

			iterator._node = this->_begin._preOrder._node;
			iterator._last_element = this->_last_element;

			return iterator;
		}

		[[nodiscard]] constexpr const_pre_order_iterator pre_order_begin() const noexcept
		{
			const_pre_order_iterator iterator{};

			iterator._node = this->_begin._preOrder._node;
			iterator._last_element = this->_last_element;

			return iterator;
		}

		[[nodiscard]] constexpr const_pre_order_iterator const_pre_order_begin() const noexcept
		{
			return this->pre_order_begin();
		}

		[[nodiscard]] constexpr pre_order_iterator pre_order_end() noexcept
		{
			pre_order_iterator iterator{};

			iterator._last_element = this->_last_element;

			return iterator;
		}

		[[nodiscard]] constexpr const_pre_order_iterator pre_order_end() const noexcept
		{
			const_pre_order_iterator iterator{};

			iterator._last_element = this->_last_element;

			return iterator;
		}

		[[nodiscard]] constexpr const_pre_order_iterator const_pre_order_end() const noexcept
		{
			return this->pre_order_end();
		}

		[[nodiscard]] constexpr reverse_pre_order_iterator reverse_pre_order_begin() noexcept
		{
			return reverse_pre_order_iterator(this->pre_order_end());
		}

		[[nodiscard]] constexpr const_reverse_pre_order_iterator reverse_pre_order_begin() const noexcept
		{
			return const_reverse_pre_order_iterator(this->const_pre_order_end());
		}

		[[nodiscard]] constexpr const_reverse_pre_order_iterator const_reverse_pre_order_begin() const noexcept
		{
			return const_reverse_pre_order_iterator(this->const_pre_order_end());
		}

		[[nodiscard]] constexpr reverse_pre_order_iterator reverse_pre_order_end() noexcept
		{
			return reverse_pre_order_iterator(this->pre_order_begin());
		}

		[[nodiscard]] constexpr const_reverse_pre_order_iterator reverse_pre_order_end() const noexcept
		{
			return const_reverse_pre_order_iterator(this->const_pre_order_begin());
		}

		[[nodiscard]] constexpr const_reverse_pre_order_iterator const_reverse_pre_order_end() const noexcept
		{
			return const_reverse_pre_order_iterator(this->const_pre_order_begin());
		}

		[[nodiscard]] constexpr post_order_iterator post_order_begin() noexcept
		{
			post_order_iterator iterator{};

			iterator._node = this->_begin._postOrder._node;
			iterator._last_element = this->_last_element;

			return iterator;
		}

		[[nodiscard]] constexpr const_post_order_iterator post_order_begin() const noexcept
		{
			const_post_order_iterator iterator{};

			iterator._node = this->_begin._postOrder._node;
			iterator._last_element = this->_last_element;

			return iterator;
		}

		[[nodiscard]] constexpr const_post_order_iterator const_post_order_begin() const noexcept
		{
			return this->post_order_begin();
		}

		[[nodiscard]] constexpr post_order_iterator post_order_end() noexcept
		{
			post_order_iterator iterator{};

			iterator._last_element = this->_last_element;

			return iterator;
		}

		[[nodiscard]] constexpr const_post_order_iterator post_order_end() const noexcept
		{
			const_post_order_iterator iterator{};

			iterator._last_element = this->_last_element;

			return iterator;
		}

		[[nodiscard]] constexpr const_post_order_iterator const_post_order_end() const noexcept
		{
			return this->post_order_end();
		}

		[[nodiscard]] constexpr reverse_post_order_iterator reverse_post_order_begin() noexcept
		{
			return reverse_post_order_iterator(this->post_order_end());
		}

		[[nodiscard]] constexpr const_reverse_post_order_iterator reverse_post_order_begin() const noexcept
		{
			return const_reverse_post_order_iterator(this->const_post_order_end());
		}

		[[nodiscard]] constexpr const_reverse_post_order_iterator const_reverse_post_order_begin() const noexcept
		{
			return const_reverse_post_order_iterator(this->const_post_order_end());
		}

		[[nodiscard]] constexpr reverse_post_order_iterator reverse_post_order_end() noexcept
		{
			return reverse_post_order_iterator(this->post_order_begin());
		}

		[[nodiscard]] constexpr const_reverse_post_order_iterator reverse_post_order_end() const noexcept
		{
			return const_reverse_post_order_iterator(this->const_post_order_begin());
		}

		[[nodiscard]] constexpr const_reverse_post_order_iterator const_reverse_post_order_end() const noexcept
		{
			return const_reverse_post_order_iterator(this->const_post_order_begin());
		}

		constexpr void swap(nary_tree &tree) noexcept
		{
			std::swap(this->_size, tree._size);
			std::swap(this->_root, tree._root);
			std::swap(this->_begin, tree._begin);
			std::swap(this->_last_element, tree._last_element);
		}

		constexpr pre_order_iterator insert_root(const nary_tree::value_type &value)
		{
			this->_root = std::make_unique < NodeType > (value);
			
			this->update_begin();
			this->update_last_element();
			this->_size = 1;

			return this->pre_order_begin();
		}

		constexpr pre_order_iterator insert_root(nary_tree::value_type &&value)
		{
			this->_root = std::make_unique < NodeType > (std::move(value));

			this->update_begin();
			this->update_last_element();
			this->_size = 1;

			return this->pre_order_begin();
		}

		constexpr pre_order_iterator insert_or_replace_child(const_pre_order_iterator iterator, const nary_tree::value_type &value, const nary_tree::size_type position)
		{
			if (!this->iterator_from_this_tree(iterator))
			{
				throw std::logic_error("The iterator is not from this tree.");
			}

			if (this->empty())
			{
				if (iterator == this->pre_order_end())
				{
					return this->insert_root(value);
				}

				throw std::logic_error("Tree is empty and the iterator has a dangling reference.");
			}

			if (iterator._node.has_value())
			{
				auto &node = iterator._node.value().get();

				if (position > node.size())
				{
					throw std::out_of_range("\"position\" is out of bounds.");
				}

				if (node.has_child(position))
				{
					const auto &currentChild = node.child(position).value().get();

					this->_size -= nary_tree::subtree_size(currentChild);
				}

				if (position < node.size())
				{
					node.erase_child(position);
				}

				auto &newChild = node.insert_child(value, position);

				this->update_begin();
				this->update_last_element();
				++(this->_size);

				pre_order_iterator childIterator{};

				childIterator._node = newChild;
				childIterator._last_element = this->_last_element;

				return childIterator;
			}

			throw std::logic_error("Attempt of getting the node of an invalid iterator.");
		}

		constexpr pre_order_iterator insert_or_replace_child(const_pre_order_iterator iterator, nary_tree::value_type &&value, const nary_tree::size_type position)
		{
			if (!this->iterator_from_this_tree(iterator))
			{
				throw std::logic_error("The iterator is not from this tree.");
			}

			if (this->empty())
			{
				if (iterator == this->pre_order_end())
				{
					return this->insert_root(std::move(value));
				}

				throw std::logic_error("Tree is empty and the iterator has a dangling reference.");
			}

			if (iterator._node.has_value())
			{
				auto &node = iterator._node.value().get();

				if (node.has_child(position))
				{
					const auto &currentChild = node.child(position).value().get();

					this->_size -= nary_tree::subtree_size(currentChild);
				}

				if (position < node.size())
				{
					node.erase_child(position);
				}

				auto &newChild = node.insert_child(std::move(value), position);

				this->update_begin();
				this->update_last_element();
				++(this->_size);

				pre_order_iterator childIterator{};

				childIterator._node = newChild;
				childIterator._last_element = this->_last_element;

				return childIterator;
			}

			throw std::logic_error("Attempt of getting the node of an invalid iterator.");
		}

		constexpr pre_order_iterator insert_child(const_pre_order_iterator iterator, const nary_tree::value_type &value, const nary_tree::size_type position)
		{
			if (!this->iterator_from_this_tree(iterator))
			{
				throw std::logic_error("The iterator is not from this tree.");
			}

			if (this->empty())
			{
				if (iterator == this->pre_order_end())
				{
					return this->insert_root(value);
				}

				throw std::logic_error("Tree is empty and the iterator has a dangling reference.");
			}

			if (iterator._node.has_value())
			{
				auto &node = iterator._node.value().get();
				auto &newChild = node.insert_child(value, position);

				this->update_begin();
				this->update_last_element();
				++(this->_size);

				pre_order_iterator childIterator{};

				childIterator._node = newChild;
				childIterator._last_element = this->_last_element;

				return childIterator;
			}

			throw std::logic_error("Attempt of getting the node of an invalid iterator.");
		}

		constexpr pre_order_iterator insert_child(const_pre_order_iterator iterator, nary_tree::value_type &&value, const nary_tree::size_type position)
		{
			if (!this->iterator_from_this_tree(iterator))
			{
				throw std::logic_error("The iterator is not from this tree.");
			}

			if (this->empty())
			{
				if (iterator == this->pre_order_end())
				{
					return this->insert_root(std::move(value));
				}

				throw std::logic_error("Tree is empty and the iterator has a dangling reference.");
			}

			if (iterator._node.has_value())
			{
				auto &node = iterator._node.value().get();
				auto &newChild = node.insert_child(std::move(value), position);

				this->update_begin();
				this->update_last_element();
				++(this->_size);

				pre_order_iterator childIterator{};

				childIterator._node = newChild;
				childIterator._last_element = this->_last_element;

				return childIterator;
			}

			throw std::logic_error("Attempt of getting the node of an invalid iterator.");
		}

		constexpr pre_order_iterator insert_back_child(const_pre_order_iterator iterator, const nary_tree::value_type &value)
		{
			if (!this->iterator_from_this_tree(iterator))
			{
				throw std::logic_error("The iterator is not from this tree.");
			}

			if (this->empty())
			{
				if (iterator == this->end())
				{
					return this->insert_root(value);
				}

				throw std::logic_error("Tree is empty and the iterator has a dangling reference.");
			}

			if (iterator._node.has_value())
			{
				auto &node = iterator._node.value().get();
				auto &newChild = node.insert_back_child(value);

				this->update_begin();
				this->update_last_element();
				++(this->_size);

				pre_order_iterator childIterator{};

				childIterator._node = newChild;
				childIterator._last_element = this->_last_element;

				return childIterator;
			}

			throw std::logic_error("Attempt of getting the node of an invalid iterator.");
		}

		constexpr pre_order_iterator insert_back_child(const_pre_order_iterator iterator, nary_tree::value_type &&value)
		{
			if (!this->iterator_from_this_tree(iterator))
			{
				throw std::logic_error("The iterator is not from this tree.");
			}

			if (this->empty())
			{
				if (iterator == this->end())
				{
					return this->insert_root(std::move(value));
				}

				throw std::logic_error("Tree is empty and the iterator has a dangling reference.");
			}

			if (iterator._node.has_value())
			{
				auto &node = iterator._node.value().get();
				auto &newChild = node.insert_back_child(std::move(value));

				this->update_begin();
				this->update_last_element();
				++(this->_size);

				pre_order_iterator childIterator{};

				childIterator._node = newChild;
				childIterator._last_element = this->_last_element;

				return childIterator;
			}

			throw std::logic_error("Attempt of getting the node of an invalid iterator.");
		}

		constexpr pre_order_iterator insert_front_child(const_pre_order_iterator iterator, const nary_tree::value_type &value)
		{
			if (!this->iterator_from_this_tree(iterator))
			{
				throw std::logic_error("The iterator is not from this tree.");
			}

			if (this->empty())
			{
				if (iterator == this->end())
				{
					return this->insert_root(value);
				}

				throw std::logic_error("Tree is empty and the iterator has a dangling reference.");
			}

			if (iterator._node.has_value())
			{
				auto &node = iterator._node.value().get();
				auto &newChild = node.insert_front_child(value);

				this->update_begin();
				this->update_last_element();
				++(this->_size);

				pre_order_iterator childIterator{};

				childIterator._node = newChild;
				childIterator._last_element = this->_last_element;

				return childIterator;
			}

			throw std::logic_error("Attempt of getting the node of an invalid iterator.");
		}

		constexpr pre_order_iterator insert_front_child(const_pre_order_iterator iterator, nary_tree::value_type &&value)
		{
			if (!this->iterator_from_this_tree(iterator))
			{
				throw std::logic_error("The iterator is not from this tree.");
			}

			if (this->empty())
			{
				if (iterator == this->end())
				{
					return this->insert_root(std::move(value));
				}

				throw std::logic_error("Tree is empty and the iterator has a dangling reference.");
			}

			if (iterator._node.has_value())
			{
				auto &node = iterator._node.value().get();
				auto &newChild = node.insert_front_child(std::move(value));

				this->update_begin();
				this->update_last_element();
				++(this->_size);

				pre_order_iterator childIterator{};

				childIterator._node = newChild;
				childIterator._last_element = this->_last_element;

				return childIterator;
			}

			throw std::logic_error("Attempt of getting the node of an invalid iterator.");
		}

		constexpr pre_order_iterator erase(const_pre_order_iterator iterator)
		{
			if (!this->iterator_from_this_tree(iterator))
			{
				throw std::logic_error("The iterator is not from this tree.");
			}

			if (!iterator._node.has_value())
			{
				throw std::logic_error("Attempt of getting the node of an invalid iterator.");
			}

			if (this->_root)
			{
				auto &node = iterator._node.value().get();

				if (!node.has_parent())
				{
					this->_root.reset();

					this->_size = 0;
					this->update_begin();
					this->update_last_element();

					return this->begin();
				}

				pre_order_iterator returnIterator{};

				returnIterator._node = node;
				returnIterator._last_element = this->_last_element;

				--returnIterator;

				this->_size -= nary_tree::subtree_size(node);

				node.extract_and_erase_myself_on_parent();

				this->update_begin();
				this->update_last_element();

				return ++returnIterator;
			}

			throw std::logic_error("Attempt to erase an element of an empty tree.");
		}

		constexpr pre_order_iterator erase(const_pre_order_iterator begin, const_pre_order_iterator end)
		{
			if (!this->iterator_from_this_tree(begin) || !this->iterator_from_this_tree(end))
			{
				throw std::logic_error("begin, end or both are not from this tree.");
			}

			if (begin == end)
			{
				pre_order_iterator iterator{};
				
				iterator._node = end._node;
				iterator._last_element = end._last_element;
				
				return iterator;
			}

			const auto upwardChainEnd = this->upward_chain_end(end);
			const auto iteratorUpwardChainEnd = std::find(upwardChainEnd.cbegin(), upwardChainEnd.cend(), begin);

			if (iteratorUpwardChainEnd != upwardChainEnd.cend())
			{
				return this->erase(begin);
			}

			const auto upwardChainBegin = this->upward_chain_begin(begin);

			auto reverseIteratorUpwardChainBegin = upwardChainBegin.crbegin();
			auto reverseIteratorUpwardChainEnd = upwardChainEnd.crbegin();

			while (*reverseIteratorUpwardChainBegin == *reverseIteratorUpwardChainEnd)
			{
				++reverseIteratorUpwardChainBegin;
				++reverseIteratorUpwardChainEnd;
			}

			const_pre_order_iterator newBegin(*reverseIteratorUpwardChainBegin);
			const_pre_order_iterator newEnd(*reverseIteratorUpwardChainEnd);

			while (newBegin != newEnd)
			{
				newBegin = this->erase(newBegin);
			}

			return this->erase(newEnd);
		}

		[[nodiscard]] constexpr bool empty() const noexcept
		{
			return this->_root == nullptr;
		}

		[[nodiscard]] constexpr bool binary() const
		{
			if (this->_root)
			{
				return nary_tree::binary(*(this->_root));
			}

			return true;
		}

		[[nodiscard]] constexpr nary_tree::size_type size() const noexcept
		{
			return this->_size;
		}

		constexpr void clear() noexcept
		{
			this->_size = 0;
			this->_root.reset();
			this->_begin._preOrder._node.reset();
			this->_last_element._preOrder._node.reset();
			this->_begin._postOrder._node.reset();
			this->_last_element._postOrder._node.reset();
		}

		[[nodiscard]] constexpr nary_tree::size_type count(const nary_tree::value_type &value) const
		{
			nary_tree::size_type counter{};
			
			for (auto iterator = this->cbegin(); iterator != this->cend(); ++iterator)
			{
				if (*iterator == value)
				{
					counter++;
				}
			}

			return counter;
		}

		[[nodiscard]] constexpr bool contains(const nary_tree::value_type &value) const
		{
			return std::find(this->cbegin(), this->cend(), value) != this->cend();
		}

		[[nodiscard]] constexpr const_pre_order_iterator find(const nary_tree::value_type &value) const
		{
			const_pre_order_iterator iterator{};

			iterator._last_element = this->_last_element;

			if (this->_root == nullptr)
			{
				return iterator;
			}

			nary_tree::find(*(this->_root.get()), value, iterator);

			return iterator;
		}

		[[nodiscard]] constexpr pre_order_iterator find(const nary_tree::value_type &value)
		{
			pre_order_iterator iterator{};

			iterator._last_element = this->_last_element;

			if (this->_root == nullptr)
			{
				return iterator;
			}

			nary_tree::find(*(this->_root.get()), value, iterator);

			return iterator;
		}

		[[nodiscard]] constexpr std::pair < const_pre_order_iterator, std::vector < nary_tree::size_type > > find_with_path(const nary_tree::value_type &value) const
		{
			const_pre_order_iterator iterator{};

			iterator._last_element = this->_last_element;

			if (this->_root == nullptr)
			{
				return std::make_pair(iterator, std::vector < nary_tree::size_type > ());
			}

			std::vector < nary_tree::size_type > path;

			nary_tree::find_with_path(*(this->_root.get()), value, path, iterator);

			return std::make_pair(iterator, path);
		}

		[[nodiscard]] constexpr std::pair < pre_order_iterator, std::vector < nary_tree::size_type > > find_with_path(const nary_tree::value_type &value)
		{
			pre_order_iterator iterator{};

			iterator._last_element = this->_last_element;

			if (this->_root == nullptr)
			{
				return std::make_pair(iterator, std::vector < nary_tree::size_type > ());
			}

			std::vector < nary_tree::size_type > path;

			nary_tree::find_with_path(*(this->_root.get()), value, path, iterator);

			return std::make_pair(iterator, path);
		}

		[[nodiscard]] constexpr std::vector < std::reference_wrapper < const NodeType > > leaf_nodes() const
		{
			std::vector < std::reference_wrapper < const NodeType > > leafNodes;

			if (this->_root)
			{
				nary_tree::leaf_nodes(*(this->_root.get()), leafNodes);
			}

			return leafNodes;
		}

		[[nodiscard]] constexpr std::vector < std::reference_wrapper < const NodeType > > nodes() const
		{
			std::vector < std::reference_wrapper < const NodeType > > nodes;

			if (this->_root)
			{
				nary_tree::nodes(*(this->_root.get()), nodes);
			}

			return nodes;
		}

		[[nodiscard]] constexpr nary_tree::size_type levels() const noexcept
		{
			nary_tree::size_type levelCount{};

			if (this->_root)
			{
				nary_tree::level_count(*(this->_root.get()), 0, levelCount);
			}

			return levelCount;
		}

		[[nodiscard]] constexpr std::vector < nary_tree::size_type > nodes_per_level() const
		{
			std::vector < nary_tree::size_type > nodesPerLevel;

			if (this->_root)
			{
				nary_tree::nodes_per_level(*(this->_root.get()), 0, nodesPerLevel);
			}

			return nodesPerLevel;
		}

		constexpr nary_tree &operator=(const nary_tree &tree)
		{
			if (this == &tree)
			{
				return *this;
			}

			this->_size = tree._size;
			this->_root = (tree._root.get() ? std::make_unique < NodeType > (*(tree._root.get())) : nullptr);

			this->update_begin();
			this->update_last_element();

			return *this;
		}

		constexpr nary_tree &operator=(nary_tree &&) noexcept = default;

		constexpr nary_tree() noexcept = default;

		constexpr nary_tree(const nary_tree &tree) :
			_size(tree._size),
			_root(tree._root.get() ? std::make_unique < NodeType > (*(tree._root.get())) : nullptr)
		{
			this->update_begin();
			this->update_last_element();
		}

		constexpr nary_tree(nary_tree &&) noexcept = default;
	};
}