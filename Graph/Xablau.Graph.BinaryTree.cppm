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

export module xablau.graph:binary_tree;
export import :fundamental_concepts;

export import std;

export namespace xablau::graph
{
	template < concepts::binary_node_with_parent NodeType >
	class binary_tree final
	{
	public:
		class const_pre_order_iterator;
		class pre_order_iterator;
		class const_in_order_iterator;
		class in_order_iterator;
		class const_post_order_iterator;
		class post_order_iterator;

		using const_reverse_pre_order_iterator = std::reverse_iterator < const_pre_order_iterator >;
		using reverse_pre_order_iterator = std::reverse_iterator < pre_order_iterator >;
		using const_reverse_in_order_iterator = std::reverse_iterator < const_in_order_iterator >;
		using reverse_in_order_iterator = std::reverse_iterator < in_order_iterator >;
		using const_reverse_post_order_iterator = std::reverse_iterator < const_post_order_iterator >;
		using reverse_post_order_iterator = std::reverse_iterator < post_order_iterator >;

		using const_iterator = const_in_order_iterator;
		using iterator = in_order_iterator;

		using const_reverse_iterator = const_reverse_in_order_iterator;
		using reverse_iterator = reverse_in_order_iterator;

		using value_type = typename NodeType::value_type;
		using node_type = NodeType;
		using size_type = size_t;

	private:
		struct FirstElement
		{
			pre_order_iterator _preOrder;
			in_order_iterator _inOrder;
			post_order_iterator _postOrder;
		};

		struct LastElement
		{
			pre_order_iterator _preOrder;
			in_order_iterator _inOrder;
			post_order_iterator _postOrder;
		};

	public:
		class pre_order_iterator final :
			public std::bidirectional_iterator_tag
		{
		private:
			friend binary_tree < NodeType >;

			std::optional < std::reference_wrapper < LastElement > > _last_element = std::nullopt;

			std::optional < std::reference_wrapper < NodeType > > _node = std::nullopt;

			constexpr pre_order_iterator(std::optional < std::reference_wrapper < NodeType > > node, LastElement &last_element) noexcept :
				_last_element(last_element),
				_node(std::move(node))
			{
			}

		public:
			using difference_type = std::ptrdiff_t;
			using value_type = binary_tree::value_type;
			using pointer = binary_tree::value_type *;
			using reference = binary_tree::value_type &;
			using iterator_category = std::bidirectional_iterator_tag;

			[[nodiscard]] constexpr operator bool() const
			{
				return this->_node.has_value();
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
					auto &currentNode = this->_node.value().get();

					if (currentNode.has_left_child())
					{
						this->_node = currentNode.left_child().value().get();

						return *this;
					}

					if (currentNode.has_right_child())
					{
						this->_node = currentNode.right_child().value().get();

						return *this;
					}

					auto parent = currentNode.parent_and_my_position();

					while (parent.has_value())
					{
						auto &parentReference = parent.value().first.get();
						const auto myPosition = parent.value().second;

						if (myPosition == binary_node_side::left && parentReference.has_right_child())
						{
							auto rightSibling = parentReference.right_child();

							this->_node = rightSibling.value().get();

							return *this;
						}

						parent = parent.value().first.get().parent_and_my_position();
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
					if (this->_node.value().get().has_parent())
					{
						auto parent = this->_node.value().get().parent_and_my_position();
						auto &parentReference = parent.value().first.get();
						const auto myPosition = parent.value().second;

						if (myPosition == binary_node_side::left || !parentReference.has_left_child())
						{
							this->_node = parentReference;

							return *this;
						}

						auto currentNode = parentReference.left_child();

						while (!currentNode.value().get().empty())
						{
							auto &referenceToCurrentNode = currentNode.value().get();

							if (referenceToCurrentNode.has_right_child())
							{
								currentNode = referenceToCurrentNode.right_child();
							}

							else if (referenceToCurrentNode.has_left_child())
							{
								currentNode = referenceToCurrentNode.left_child();
							}
						}

						this->_node = currentNode.value().get();

						return *this;
					}

					throw std::out_of_range("Decrement of an invalid iterator.");
				}

				if (this->_last_element.has_value() && this->_last_element.value().get()._preOrder._node.has_value())
				{
					this->_node = this->_last_element.value().get()._preOrder._node;

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
					return std::addressof(this->_node.value().get().value());
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

			[[nodiscard]] constexpr in_order_iterator convert_to_in_order() const noexcept
			{
				if (this->_last_element.has_value())
				{
					return in_order_iterator(this->_node, this->_last_element.value().get());
				}

				return in_order_iterator{};
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
			friend binary_tree < NodeType >;

			std::optional < std::reference_wrapper < LastElement > > _last_element = std::nullopt;

			std::optional < std::reference_wrapper < NodeType > > _node = std::nullopt;

			constexpr const_pre_order_iterator(std::optional < std::reference_wrapper < NodeType > > node, LastElement &last_element) noexcept :
				_last_element(last_element),
				_node(node)
			{
			}

		public:
			using difference_type = std::ptrdiff_t;
			using value_type = binary_tree::value_type;
			using pointer = const binary_tree::value_type *;
			using reference = const binary_tree::value_type &;
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
					auto &currentNode = this->_node.value().get();

					if (currentNode.has_left_child())
					{
						this->_node = currentNode.left_child().value().get();

						return *this;
					}

					if (currentNode.has_right_child())
					{
						this->_node = currentNode.right_child().value().get();

						return *this;
					}

					auto parent = currentNode.parent_and_my_position();

					while (parent.has_value())
					{
						auto &parentReference = parent.value().first.get();
						const auto myPosition = parent.value().second;

						if (myPosition == binary_node_side::left && parentReference.has_right_child())
						{
							const auto rightSibling = parentReference.right_child();

							this->_node = rightSibling.value().get();

							return *this;
						}

						parent = parent.value().first.get().parent_and_my_position();
					}

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
					if (this->_node.value().get().has_parent())
					{
						auto parent = this->_node.value().get().parent_and_my_position();
						auto &parentReference = parent.value().first.get();
						const auto myPosition = parent.value().second;

						if (myPosition == binary_node_side::left || !parentReference.has_left_child())
						{
							this->_node = parentReference;

							return *this;
						}

						auto currentNode = parentReference.left_child();

						while (!currentNode.value().get().empty())
						{
							auto &referenceToCurrentNode = currentNode.value().get();

							if (referenceToCurrentNode.has_right_child())
							{
								currentNode = referenceToCurrentNode.right_child();
							}

							else if (referenceToCurrentNode.has_left_child())
							{
								currentNode = referenceToCurrentNode.left_child();
							}
						}

						this->_node = currentNode.value().get();

						return *this;
					}

					throw std::out_of_range("Decrement of an invalid iterator.");
				}

				if (this->_last_element.has_value() && this->_last_element.value().get()._preOrder._node.has_value())
				{
					this->_node = this->_last_element.value().get()._preOrder._node;

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
					return std::addressof(this->_node.value().get().value());
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

			[[nodiscard]] constexpr const_in_order_iterator convert_to_in_order() const noexcept
			{
				if (this->_last_element.has_value())
				{
					return const_in_order_iterator(this->_node, this->_last_element.value().get());
				}

				return const_in_order_iterator{};
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

		class in_order_iterator final :
			public std::bidirectional_iterator_tag
		{
		private:
			friend binary_tree < NodeType >;

			std::optional < std::reference_wrapper < LastElement > > _last_element = std::nullopt;

			std::optional < std::reference_wrapper < NodeType > > _node = std::nullopt;

			constexpr in_order_iterator(std::optional < std::reference_wrapper < NodeType > > node, LastElement &last_element) noexcept :
				_last_element(last_element),
				_node(node)
			{
			}

		public:
			using difference_type = std::ptrdiff_t;
			using value_type = binary_tree::value_type;
			using pointer = binary_tree::value_type *;
			using reference = binary_tree::value_type &;
			using iterator_category = std::bidirectional_iterator_tag;

			[[nodiscard]] constexpr operator bool() const
			{
				return this->_node.has_value();
			}

			[[nodiscard]] constexpr bool operator==(const in_order_iterator &iterator) const noexcept
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

			constexpr in_order_iterator &operator=(const in_order_iterator &) noexcept = default;

			constexpr in_order_iterator &operator=(in_order_iterator &&) noexcept = default;

			constexpr in_order_iterator &operator++()
			{
				if (this->_node.has_value())
				{
					auto rightAndLeftmostChild = this->_node.value().get().right_child();

					if (rightAndLeftmostChild.has_value())
					{
						while (rightAndLeftmostChild.value().get().has_left_child())
						{
							rightAndLeftmostChild = rightAndLeftmostChild.value().get().left_child();
						}

						this->_node = rightAndLeftmostChild.value().get();

						return *this;
					}

					auto parent = this->_node.value().get().parent_and_my_position();

					while (parent.has_value())
					{
						if (parent.value().second == binary_node_side::left)
						{
							this->_node = parent.value().first.get();

							return *this;
						}

						parent = parent.value().first.get().parent_and_my_position();
					}

					this->_node.reset();

					return *this;
				}

				throw std::out_of_range("Increment of an invalid iterator.");
			}

			constexpr in_order_iterator operator++(int)
			{
				in_order_iterator _iterator(*this);

				operator++();

				return _iterator;
			}

			constexpr in_order_iterator &operator--()
			{
				if (this->_node.has_value())
				{
					auto leftAndRightmostChild = this->_node.value().get().left_child();

					if (leftAndRightmostChild.has_value())
					{
						while (leftAndRightmostChild.value().get().has_right_child())
						{
							leftAndRightmostChild = leftAndRightmostChild.value().get().right_child();
						}

						this->_node = leftAndRightmostChild.value().get();

						return *this;
					}

					auto parent = this->_node.value().get().parent_and_my_position();

					while (parent.has_value())
					{
						if (parent.value().second == binary_node_side::right)
						{
							this->_node = parent.value().first.get();

							return *this;
						}

						parent = parent.value().first.get().parent_and_my_position();
					}

					throw std::out_of_range("Decrement of an invalid iterator.");
				}

				if (this->_last_element.has_value() && this->_last_element.value().get()._inOrder._node.has_value())
				{
					this->_node = this->_last_element.value().get()._inOrder._node;

					return *this;
				}

				throw std::out_of_range("Decrement of an invalid iterator.");
			}

			constexpr in_order_iterator operator--(int)
			{
				in_order_iterator _iterator(*this);

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
					return std::addressof(this->_node.value().get().value());
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

			[[nodiscard]] constexpr post_order_iterator convert_to_post_order() const noexcept
			{
				if (this->_last_element.has_value())
				{
					return post_order_iterator(this->_node, this->_last_element.value().get());
				}

				return post_order_iterator{};
			}

			constexpr void swap(in_order_iterator &iterator) noexcept
			{
				std::swap(this->_node, iterator._node);
				std::swap(this->_last_element, iterator._last_element);
			}

			constexpr in_order_iterator() noexcept = default;

			constexpr in_order_iterator(const in_order_iterator &) noexcept = default;

			constexpr in_order_iterator(in_order_iterator &&) noexcept = default;
		};

		class const_in_order_iterator final :
			public std::bidirectional_iterator_tag
		{
		private:
			friend binary_tree < NodeType >;

			std::optional < std::reference_wrapper < LastElement > > _last_element = std::nullopt;

			std::optional < std::reference_wrapper < NodeType > > _node = std::nullopt;

			constexpr const_in_order_iterator(std::optional < std::reference_wrapper < NodeType > > node, LastElement &last_element) noexcept :
				_last_element(last_element),
				_node(node)
			{
			}

		public:
			using difference_type = std::ptrdiff_t;
			using value_type = binary_tree::value_type;
			using pointer = const binary_tree::value_type *;
			using reference = const binary_tree::value_type &;
			using iterator_category = std::bidirectional_iterator_tag;

			[[nodiscard]] constexpr operator bool() const
			{
				return this->_node.has_value();
			}

			[[nodiscard]] constexpr bool operator==(const const_in_order_iterator &iterator) const noexcept
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

			[[nodiscard]] constexpr bool operator==(const in_order_iterator &iterator) const noexcept
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

			constexpr const_in_order_iterator &operator=(const const_in_order_iterator &) noexcept = default;

			constexpr const_in_order_iterator &operator=(const_in_order_iterator &&) noexcept = default;

			constexpr const_in_order_iterator &operator=(const in_order_iterator &iterator) noexcept
			{
				this->_node = iterator._node;
				this->_last_element = iterator._last_element;

				return *this;
			}
			
			constexpr const_in_order_iterator &operator++()
			{
				if (this->_node.has_value())
				{
					auto rightAndLeftmostChild = this->_node.value().get().right_child();

					if (rightAndLeftmostChild.has_value())
					{
						while (rightAndLeftmostChild.value().get().has_left_child())
						{
							rightAndLeftmostChild = rightAndLeftmostChild.value().get().left_child();
						}

						this->_node = rightAndLeftmostChild.value().get();

						return *this;
					}

					auto parent = this->_node.value().get().parent_and_my_position();

					while (parent.has_value())
					{
						if (parent.value().second == binary_node_side::left)
						{
							this->_node = parent.value().first.get();

							return *this;
						}

						parent = parent.value().first.get().parent_and_my_position();
					}

					this->_node.reset();

					return *this;
				}

				throw std::out_of_range("Increment of an invalid iterator.");
			}

			constexpr const_in_order_iterator operator++(int)
			{
				const_in_order_iterator _iterator(*this);

				operator++();

				return _iterator;
			}

			constexpr const_in_order_iterator &operator--()
			{
				if (this->_node.has_value())
				{
					auto leftAndRightmostChild = this->_node.value().get().left_child();

					if (leftAndRightmostChild.has_value())
					{
						while (leftAndRightmostChild.value().get().has_right_child())
						{
							leftAndRightmostChild = leftAndRightmostChild.value().get().right_child();
						}

						this->_node = leftAndRightmostChild.value().get();

						return *this;
					}

					auto parent = this->_node.value().get().parent_and_my_position();

					while (parent.has_value())
					{
						if (parent.value().second == binary_node_side::right)
						{
							this->_node = parent.value().first.get();

							return *this;
						}

						parent = parent.value().first.get().parent_and_my_position();
					}

					throw std::out_of_range("Decrement of an invalid iterator.");
				}

				if (this->_last_element.has_value() && this->_last_element.value().get()._inOrder._node.has_value())
				{
					this->_node = this->_last_element.value().get()._inOrder._node;

					return *this;
				}

				throw std::out_of_range("Decrement of an invalid iterator.");
			}

			constexpr const_in_order_iterator operator--(int)
			{
				const_in_order_iterator _iterator(*this);

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
					return std::addressof(this->_node.value().get().value());
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

			[[nodiscard]] constexpr const_post_order_iterator convert_to_post_order() const noexcept
			{
				if (this->_last_element.has_value())
				{
					return const_post_order_iterator(this->_node, this->_last_element.value().get());
				}

				return const_post_order_iterator{};
			}

			constexpr void swap(const_in_order_iterator &iterator) noexcept
			{
				std::swap(this->_node, iterator._node);
				std::swap(this->_last_element, iterator._last_element);
			}

			constexpr const_in_order_iterator() noexcept = default;

			constexpr const_in_order_iterator(const in_order_iterator &iterator) noexcept :
				_last_element(iterator._last_element),
				_node(iterator._node)
			{
			}

			constexpr const_in_order_iterator(const const_in_order_iterator &) noexcept = default;

			constexpr const_in_order_iterator(const_in_order_iterator &&) noexcept = default;
		};

		class post_order_iterator final :
			public std::bidirectional_iterator_tag
		{
		private:
			friend binary_tree < NodeType >;

			std::optional < std::reference_wrapper < LastElement > > _last_element = std::nullopt;

			std::optional < std::reference_wrapper < NodeType > > _node = std::nullopt;

			constexpr post_order_iterator(std::optional < std::reference_wrapper < NodeType > > node, LastElement &last_element) noexcept :
				_last_element(last_element),
				_node(node)
			{
			}

		public:
			using difference_type = std::ptrdiff_t;
			using value_type = binary_tree::value_type;
			using pointer = binary_tree::value_type *;
			using reference = binary_tree::value_type &;
			using iterator_category = std::bidirectional_iterator_tag;

			[[nodiscard]] constexpr operator bool() const
			{
				return this->_node.has_value();
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
					if (this->_node.value().get().has_parent())
					{
						auto parent = this->_node.value().get().parent_and_my_position();
						auto &parentReference = parent.value().first.get();
						const auto myPosition = parent.value().second;

						if (myPosition == binary_node_side::right || !parentReference.has_right_child())
						{
							this->_node = parentReference;

							return *this;
						}

						auto currentNode = parentReference.right_child();

						while (!currentNode.value().get().empty())
						{
							auto &referenceToCurrentNode = currentNode.value().get();

							if (referenceToCurrentNode.has_left_child())
							{
								currentNode = referenceToCurrentNode.left_child();
							}

							else if (referenceToCurrentNode.has_right_child())
							{
								currentNode = referenceToCurrentNode.right_child();
							}
						}

						this->_node = currentNode.value().get();

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
					auto &currentNode = this->_node.value().get();
					
					if (currentNode.has_right_child())
					{
						this->_node = currentNode.right_child().value().get();

						return *this;
					}

					if (currentNode.has_left_child())
					{
						this->_node = currentNode.left_child().value().get();

						return *this;
					}

					auto parent = currentNode.parent_and_my_position();

					while (parent.has_value())
					{
						auto &parentReference = parent.value().first.get();
						const auto myPosition = parent.value().second;

						if (myPosition == binary_node_side::right && parentReference.has_left_child())
						{
							const auto leftSibling = parentReference.left_child();

							this->_node = leftSibling.value().get();

							return *this;
						}

						parent = parent.value().first.get().parent_and_my_position();
					}

					throw std::out_of_range("Decrement of an invalid iterator.");
				}

				if (this->_last_element.has_value() && this->_last_element.value().get()._postOrder._node.has_value())
				{
					this->_node = this->_last_element.value().get()._postOrder._node;

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
					return std::addressof(this->_node.value().get().value());
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

			[[nodiscard]] constexpr in_order_iterator convert_to_in_order() const noexcept
			{
				if (this->_last_element.has_value())
				{
					return in_order_iterator(this->_node, this->_last_element.value().get());
				}

				return in_order_iterator{};
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
			friend binary_tree < NodeType >;

			std::optional < std::reference_wrapper < LastElement > > _last_element = std::nullopt;

			std::optional < std::reference_wrapper < NodeType > > _node = std::nullopt;

			constexpr const_post_order_iterator(std::optional < std::reference_wrapper < NodeType > > node, LastElement &last_element) noexcept :
				_last_element(last_element),
				_node(node)
			{
			}

		public:
			using difference_type = std::ptrdiff_t;
			using value_type = binary_tree::value_type;
			using pointer = const binary_tree::value_type *;
			using reference = const binary_tree::value_type &;
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
					if (this->_node.value().get().has_parent())
					{
						auto parent = this->_node.value().get().parent_and_my_position();
						auto &parentReference = parent.value().first.get();
						const auto myPosition = parent.value().second;

						if (myPosition == binary_node_side::right || !parentReference.has_right_child())
						{
							this->_node = parentReference;

							return *this;
						}

						auto currentNode = parentReference.right_child();

						while (!currentNode.value().get().empty())
						{
							auto &referenceToCurrentNode = currentNode.value().get();

							if (referenceToCurrentNode.has_left_child())
							{
								currentNode = referenceToCurrentNode.left_child();
							}

							else if (referenceToCurrentNode.has_right_child())
							{
								currentNode = referenceToCurrentNode.right_child();
							}
						}

						this->_node = currentNode.value().get();

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
					auto &currentNode = this->_node.value().get();

					if (currentNode.has_right_child())
					{
						this->_node = currentNode.right_child().value().get();

						return *this;
					}

					if (this->_node.value().get().has_left_child())
					{
						this->_node = currentNode.left_child().value().get();

						return *this;
					}

					auto parent = currentNode.parent_and_my_position();

					while (parent.has_value())
					{
						auto &parentReference = parent.value().first.get();
						const auto myPosition = parent.value().second;

						if (myPosition == binary_node_side::right && parentReference.has_left_child())
						{
							auto leftSibling = parentReference.left_child();

							this->_node = leftSibling.value().get();

							return *this;
						}

						parent = parent.value().first.get().parent_and_my_position();
					}

					throw std::out_of_range("Decrement of an invalid iterator.");
				}

				if (this->_last_element.has_value() && this->_last_element.value().get()._postOrder._node.has_value())
				{
					this->_node = this->_last_element.value().get()._postOrder._node;

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
					return std::addressof(this->_node.value().get().value());
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

			[[nodiscard]] constexpr const_in_order_iterator convert_to_in_order() const noexcept
			{
				if (this->_last_element.has_value())
				{
					return const_in_order_iterator(this->_node, this->_last_element.value().get());
				}

				return const_in_order_iterator{};
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
		size_t _size{};

		std::unique_ptr < NodeType > _root = nullptr;

		mutable FirstElement _begin{};

		mutable LastElement _last_element{};

		static constexpr std::optional < std::reference_wrapper < NodeType > > &node(in_order_iterator &iterator)
		{
			return iterator._node;
		}

		static constexpr std::optional < std::reference_wrapper < NodeType > > &node(const_in_order_iterator &iterator)
		{
			return iterator._node;
		}

		static constexpr std::optional < std::reference_wrapper < LastElement > > &last_element(in_order_iterator &iterator)
		{
			return iterator._last_element;
		}

		static constexpr std::optional < std::reference_wrapper < LastElement > > &last_element(const_in_order_iterator &iterator)
		{
			return iterator._last_element;
		}

		static constexpr std::optional < std::reference_wrapper < NodeType > > &node(pre_order_iterator &iterator)
		{
			return iterator._node;
		}

		static constexpr std::optional < std::reference_wrapper < NodeType > > &node(const_pre_order_iterator &iterator)
		{
			return iterator._node;
		}

		static constexpr std::optional < std::reference_wrapper < LastElement > > &last_element(pre_order_iterator &iterator)
		{
			return iterator._last_element;
		}

		static constexpr std::optional < std::reference_wrapper < LastElement > > &last_element(const_pre_order_iterator &iterator)
		{
			return iterator._last_element;
		}

		static constexpr std::optional < std::reference_wrapper < NodeType > > &node(post_order_iterator &iterator)
		{
			return iterator._node;
		}

		static constexpr std::optional < std::reference_wrapper < NodeType > > &node(const_post_order_iterator &iterator)
		{
			return iterator._node;
		}

		static constexpr std::optional < std::reference_wrapper < LastElement > > &last_element(post_order_iterator &iterator)
		{
			return iterator._last_element;
		}

		static constexpr std::optional < std::reference_wrapper < LastElement > > &last_element(const_post_order_iterator &iterator)
		{
			return iterator._last_element;
		}

		static constexpr void leaf_nodes(
			const NodeType &node,
			std::vector < std::reference_wrapper < const NodeType > > &leafNodes)
		{
			if (node.valid_size() == 0)
			{
				leafNodes.push_back(node);
			}

			else
			{
				auto child = node.left_child();

				if (child.has_value())
				{
					binary_tree::leaf_nodes(child.value().get(), leafNodes);
				}

				child = node.right_child();

				if (child.has_value())
				{
					binary_tree::leaf_nodes(child.value().get(), leafNodes);
				}
			}
		}

		static constexpr void nodes(
			const NodeType &node,
			std::vector < std::reference_wrapper < const NodeType > > &nodes)
		{
			auto child = node.left_child();

			if (child.has_value())
			{
				binary_tree::nodes(child.value().get(), nodes);
			}

			nodes.push_back(node);

			child = node.right_child();

			if (child.has_value())
			{
				binary_tree::nodes(child.value().get(), nodes);
			}
		}

		static constexpr void level_count(
			const NodeType &node,
			const size_t currentLevel,
			size_t &levelCount) noexcept
		{
			if (currentLevel + 1 > levelCount)
			{
				levelCount = currentLevel + 1;
			}

			auto child = node.left_child();

			if (child.has_value())
			{
				binary_tree::level_count(child.value().get(), currentLevel + 1, levelCount);
			}

			child = node.right_child();

			if (child.has_value())
			{
				binary_tree::level_count(child.value().get(), currentLevel + 1, levelCount);
			}
		}

		static constexpr void nodes_per_level(
			const NodeType &node,
			const size_t currentLevel,
			std::vector < size_t > &nodesPerLevel)
		{
			if (nodesPerLevel.size() < currentLevel + 1)
			{
				nodesPerLevel.resize(currentLevel + 1, 0);
			}

			++(nodesPerLevel[currentLevel]);

			auto child = node.left_child();

			if (child)
			{
				binary_tree::nodes_per_level(child.value().get(), currentLevel + 1, nodesPerLevel);
			}

			child = node.right_child();

			if (child)
			{
				binary_tree::nodes_per_level(child.value().get(), currentLevel + 1, nodesPerLevel);
			}
		}

		static constexpr bool equal(
			const std::optional < std::reference_wrapper < const NodeType > > &node1,
			const std::optional < std::reference_wrapper < const NodeType > > &node2)
		{
			const bool node1HasValue = node1.has_value();
			const bool node2HasValue = node2.has_value();

			if (node1HasValue && node2HasValue)
			{
				const auto &_node1 = node1.value().get();
				const auto &_node2 = node2.value().get();

				return
					_node1 == _node2 &&

					binary_tree::equal(_node1.left_child(), _node2.left_child()) &&
					binary_tree::equal(_node1.right_child(), _node2.right_child());
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
			const binary_tree::value_type &value,
			Iterator &iterator)
		{
			if (node.value() == value)
			{
				iterator._node = node;

				return;
			}

			auto child = node.left_child();

			if (child.has_value())
			{
				binary_tree::find(child.value().get(), value, iterator);
			}

			if (iterator._node.has_value())
			{
				return;
			}

			child = node.right_child();

			if (child.has_value())
			{
				binary_tree::find(child.value().get(), value, iterator);
			}
		}

		template < typename Iterator >
		static constexpr void find_with_path(
			NodeType &node,
			const binary_tree::value_type &value,
			std::vector < binary_node_side > &path,
			Iterator &iterator)
		{
			if (node.value() == value)
			{
				iterator._node = node;

				return;
			}

			path.push_back(binary_node_side::left);

			auto child = node.left_child();

			if (child.has_value())
			{
				binary_tree::find_with_path(child.value().get(), value, path, iterator);
			}

			if (iterator._node.has_value())
			{
				return;
			}

			path.back() = binary_node_side::right;

			child = node.right_child();

			if (child.has_value())
			{
				binary_tree::find_with_path(child.value().get(), value, path, iterator);
			}

			if (iterator._node.has_value())
			{
				return;
			}

			path.pop_back();
		}

		constexpr void update_begin() noexcept
		{
			if (this->_root)
			{
				std::reference_wrapper < NodeType > node = *(this->_root.get());

				this->_begin._preOrder._node = node.get();

				while (node.get().has_left_child())
				{
					node = node.get().left_child().value().get();
				}

				this->_begin._inOrder._node = node.get();

				while (true)
				{
					if (node.get().has_left_child())
					{
						node = node.get().left_child().value().get();
					}

					else if (node.get().has_right_child())
					{
						node = node.get().right_child().value().get();
					}

					else
					{
						break;
					}
				}

				this->_begin._postOrder._node = node.get();
			}

			else
			{
				this->_begin._preOrder._node.reset();
				this->_begin._inOrder._node.reset();
				this->_begin._postOrder._node.reset();
			}
		}

		constexpr void update_last_element() noexcept
		{
			if (this->_root)
			{
				std::reference_wrapper < NodeType > node = *(this->_root.get());

				this->_last_element._postOrder._node = node.get();

				while (node.get().has_right_child())
				{
					node = node.get().right_child().value().get();
				}

				this->_last_element._inOrder._node = node.get();

				while (true)
				{
					if (node.get().has_right_child())
					{
						node = node.get().right_child().value().get();
					}

					else if (node.get().has_left_child())
					{
						node = node.get().left_child().value().get();
					}

					else
					{
						break;
					}
				}

				this->_last_element._preOrder._node = node.get();
			}

			else
			{
				this->_last_element._preOrder._node.reset();
				this->_last_element._inOrder._node.reset();
				this->_last_element._postOrder._node.reset();
			}
		}

		constexpr in_order_iterator erase_find_first_intersection(const_in_order_iterator begin, const_in_order_iterator end)
		{
			in_order_iterator intersection(std::nullopt, this->_last_element);
			std::set < std::uintptr_t > insertedNodes;

			--end;

			const auto find =
				[&insertedNodes] (const const_in_order_iterator &iterator) -> bool
				{
					return std::ranges::find(
						std::as_const(insertedNodes),
						reinterpret_cast < std::uintptr_t > (std::addressof(iterator._node.value().get()))) != insertedNodes.cend();
				};

			const auto findAndInsert =
				[&find, &insertedNodes, &intersection] (const const_in_order_iterator &iterator) -> bool
				{
					if (iterator._node.has_value())
					{
						if (find(iterator))
						{
							intersection._node = iterator._node;

							return true;
						}

						insertedNodes.emplace(reinterpret_cast < std::uintptr_t > (std::addressof(iterator._node.value().get())));
					}

					return false;
				};

			constexpr auto updateIterator =
				[] (const_in_order_iterator &iterator) -> void
				{
					if (iterator._node.has_value())
					{
						const auto parent = iterator._node.value().get().parent();

						if (parent.has_value())
						{
							iterator._node = parent.value().get();
						}

						else
						{
							iterator._node.reset();
						}
					}
				};

			while (begin._node.has_value() || end._node.has_value())
			{
				if (findAndInsert(begin))
				{
					return intersection;
				}

				if (findAndInsert(end))
				{
					return intersection;
				}

				updateIterator(begin);
				updateIterator(end);
			}

			throw std::logic_error("Both iterators are from different trees.");
		}

		constexpr bool iterator_from_this_tree(const_pre_order_iterator iterator) noexcept
		{
			return iterator._last_element.has_value() && this->_last_element._preOrder == iterator._last_element.value().get()._preOrder;
		}

		constexpr bool iterator_from_this_tree(const_in_order_iterator iterator) noexcept
		{
			return iterator._last_element.has_value() && this->_last_element._inOrder == iterator._last_element.value().get()._inOrder;
		}

		constexpr bool iterator_from_this_tree(const_post_order_iterator iterator) noexcept
		{
			return iterator._last_element.has_value() && this->_last_element._postOrder == iterator._last_element.value().get()._postOrder;
		}

		static constexpr size_t size_subtree(const NodeType &node) noexcept
		{
			return
				1 +
				(node.has_left_child() ? binary_tree::size_subtree(node.left_child().value().get()) : 0) +
				(node.has_right_child() ? binary_tree::size_subtree(node.right_child().value().get()) : 0);
		}

	public:
		constexpr bool operator==(const binary_tree &tree) const
		{
			if (this->_root && tree._root)
			{
				return binary_tree::equal(*(this->_root.get()), *(tree._root.get()));
			}

			if (!this->_root && !tree._root)
			{
				return true;
			}

			return false;
		}

		constexpr in_order_iterator begin() noexcept
		{
			in_order_iterator iterator{};

			iterator._node = this->_begin._inOrder._node;
			iterator._last_element = this->_last_element;

			return iterator;
		}

		constexpr const_in_order_iterator begin() const noexcept
		{
			const_in_order_iterator iterator{};

			iterator._node = this->_begin._inOrder._node;
			iterator._last_element = this->_last_element;

			return iterator;
		}

		constexpr const_in_order_iterator cbegin() const noexcept
		{
			return this->begin();
		}

		constexpr in_order_iterator end() noexcept
		{
			in_order_iterator iterator{};

			iterator._last_element = this->_last_element;

			return iterator;
		}

		constexpr const_in_order_iterator end() const noexcept
		{
			const_in_order_iterator iterator{};

			iterator._last_element = this->_last_element;

			return iterator;
		}

		constexpr const_in_order_iterator cend() const noexcept
		{
			return this->end();
		}

		constexpr reverse_in_order_iterator rbegin() noexcept
		{
			return std::reverse_iterator(this->end());
		}

		constexpr const_reverse_in_order_iterator rbegin() const noexcept
		{
			return std::reverse_iterator(this->end());
		}

		constexpr const_reverse_in_order_iterator crbegin() const noexcept
		{
			return std::reverse_iterator(this->cend());
		}

		constexpr reverse_in_order_iterator rend() noexcept
		{
			return std::reverse_iterator(this->begin());
		}

		constexpr const_reverse_in_order_iterator rend() const noexcept
		{
			return std::reverse_iterator(this->begin());
		}

		constexpr const_reverse_in_order_iterator crend() const noexcept
		{
			return std::reverse_iterator(this->cbegin());
		}

		constexpr pre_order_iterator pre_order_begin() noexcept
		{
			pre_order_iterator iterator{};

			iterator._node = this->_begin._preOrder._node;
			iterator._last_element = this->_last_element;

			return iterator;
		}

		constexpr const_pre_order_iterator pre_order_begin() const noexcept
		{
			const_pre_order_iterator iterator{};

			iterator._node = this->_begin._preOrder._node;
			iterator._last_element = this->_last_element;

			return iterator;
		}

		constexpr const_pre_order_iterator const_pre_order_begin() const noexcept
		{
			return this->pre_order_begin();
		}

		constexpr pre_order_iterator pre_order_end() noexcept
		{
			pre_order_iterator iterator{};

			iterator._last_element = this->_last_element;

			return iterator;
		}

		constexpr const_pre_order_iterator pre_order_end() const noexcept
		{
			const_pre_order_iterator iterator{};

			iterator._last_element = this->_last_element;

			return iterator;
		}

		constexpr const_pre_order_iterator const_pre_order_end() const noexcept
		{
			return this->pre_order_end();
		}

		constexpr reverse_pre_order_iterator reverse_pre_order_begin() noexcept
		{
			return std::reverse_iterator(this->pre_order_end());
		}

		constexpr const_reverse_pre_order_iterator reverse_pre_order_begin() const noexcept
		{
			return std::reverse_iterator(this->pre_order_end());
		}

		constexpr const_reverse_pre_order_iterator const_reverse_pre_order_begin() const noexcept
		{
			return this->reverse_pre_order_begin();
		}

		constexpr reverse_pre_order_iterator reverse_pre_order_end() noexcept
		{
			return std::reverse_iterator(this->pre_order_begin());
		}

		constexpr const_reverse_pre_order_iterator reverse_pre_order_end() const noexcept
		{
			return std::reverse_iterator(this->pre_order_begin());
		}

		constexpr const_reverse_pre_order_iterator const_reverse_pre_order_end() const noexcept
		{
			return this->reverse_pre_order_end();
		}

		constexpr in_order_iterator in_order_begin() noexcept
		{
			return this->begin();
		}

		constexpr const_in_order_iterator in_order_begin() const noexcept
		{
			return this->begin();
		}

		constexpr const_in_order_iterator const_in_order_begin() const noexcept
		{
			return this->cbegin();
		}

		constexpr in_order_iterator in_order_end() noexcept
		{
			return this->end();
		}

		constexpr const_in_order_iterator in_order_end() const noexcept
		{
			return this->end();
		}

		constexpr const_in_order_iterator const_in_order_end() const noexcept
		{
			return this->cend();
		}

		constexpr reverse_in_order_iterator reverse_in_order_begin() noexcept
		{
			return this->rbegin();
		}

		constexpr const_reverse_in_order_iterator reverse_in_order_begin() const noexcept
		{
			return this->rbegin();
		}

		constexpr const_reverse_in_order_iterator const_reverse_in_order_begin() const noexcept
		{
			return this->crbegin();
		}

		constexpr reverse_in_order_iterator reverse_in_order_end() noexcept
		{
			return this->rend();
		}

		constexpr const_reverse_in_order_iterator reverse_in_order_end() const noexcept
		{
			return this->rend();
		}

		constexpr const_reverse_in_order_iterator const_reverse_in_order_end() const noexcept
		{
			return this->crend();
		}

		constexpr post_order_iterator post_order_begin() noexcept
		{
			post_order_iterator iterator{};

			iterator._node = this->_begin._postOrder._node;
			iterator._last_element = this->_last_element;

			return iterator;
		}

		constexpr const_post_order_iterator post_order_begin() const noexcept
		{
			const_post_order_iterator iterator{};

			iterator._node = this->_begin._postOrder._node;
			iterator._last_element = this->_last_element;

			return iterator;
		}

		constexpr const_post_order_iterator const_post_order_begin() const noexcept
		{
			return this->post_order_begin();
		}

		constexpr post_order_iterator post_order_end() noexcept
		{
			post_order_iterator iterator{};

			iterator._last_element = this->_last_element;

			return iterator;
		}

		constexpr const_post_order_iterator post_order_end() const noexcept
		{
			const_post_order_iterator iterator{};

			iterator._last_element = this->_last_element;

			return iterator;
		}

		constexpr const_post_order_iterator const_post_order_end() const noexcept
		{
			return this->post_order_end();
		}

		constexpr reverse_post_order_iterator reverse_post_order_begin() noexcept
		{
			return reverse_post_order_iterator(this->post_order_end());
		}

		constexpr const_reverse_post_order_iterator reverse_post_order_begin() const noexcept
		{
			return const_reverse_post_order_iterator(this->post_order_end());
		}

		constexpr const_reverse_post_order_iterator const_reverse_post_order_begin() const noexcept
		{
			return this->reverse_post_order_begin();
		}

		constexpr reverse_post_order_iterator reverse_post_order_end() noexcept
		{
			return reverse_post_order_iterator(this->post_order_begin());
		}

		constexpr const_reverse_post_order_iterator reverse_post_order_end() const noexcept
		{
			return const_reverse_post_order_iterator(this->post_order_begin());
		}

		constexpr const_reverse_post_order_iterator const_reverse_post_order_end() const noexcept
		{
			return this->reverse_post_order_end();
		}

		constexpr void swap(binary_tree &tree) noexcept
		{
			std::swap(this->_size, tree._size);
			std::swap(this->_root, tree._root);
			std::swap(this->_begin, tree._begin);
			std::swap(this->_last_element, tree._last_element);
		}

		constexpr in_order_iterator insert_root(const binary_tree::value_type &value)
		{
			this->_root = std::make_unique < NodeType > (value);

			this->update_begin();
			this->update_last_element();
			this->_size = 1;

			return this->in_order_begin();
		}

		constexpr in_order_iterator insert_root(binary_tree::value_type &&value)
		{
			this->_root = std::make_unique < NodeType > (std::move(value));

			this->update_begin();
			this->update_last_element();
			this->_size = 1;

			return this->in_order_begin();
		}

		constexpr in_order_iterator insert_left_child(const_in_order_iterator iterator, const binary_tree::value_type &value)
		{
			if (!this->iterator_from_this_tree(iterator))
			{
				throw std::logic_error("The iterator is not from this tree.");
			}

			if (this->empty())
			{
				if (iterator == this->in_order_end())
				{
					return this->insert_root(value);
				}

				throw std::logic_error("Tree is empty and the iterator has a dangling reference.");
			}

			if (iterator._node.has_value())
			{
				auto &node = iterator._node.value().get();

				if (node.has_left_child())
				{
					const auto &currentLeftChild = node.left_child().value().get();

					this->_size -= binary_tree::size_subtree(currentLeftChild);
				}

				auto &newLeftChild = node.insert_left_child(value);

				this->update_begin();
				this->update_last_element();
				++(this->_size);

				return in_order_iterator(newLeftChild, this->_last_element);
			}

			throw std::logic_error("Attempt of getting the node of an invalid iterator.");
		}

		constexpr in_order_iterator insert_left_child(const_in_order_iterator iterator, binary_tree::value_type &&value)
		{
			if (!this->iterator_from_this_tree(iterator))
			{
				throw std::logic_error("The iterator is not from this tree.");
			}

			if (this->empty())
			{
				if (iterator == this->in_order_end())
				{
					return this->insert_root(std::move(value));
				}

				throw std::logic_error("Tree is empty and the iterator has a dangling reference.");
			}

			if (iterator._node.has_value())
			{
				auto &node = iterator._node.value().get();

				if (node.has_left_child())
				{
					const auto &currentLeftChild = node.left_child().value().get();

					this->_size -= binary_tree::size_subtree(currentLeftChild);
				}

				auto &newLeftChild = node.insert_left_child(std::move(value));

				this->update_begin();
				this->update_last_element();
				++(this->_size);

				return in_order_iterator(newLeftChild, this->_last_element);
			}

			throw std::logic_error("Attempt of getting the node of an invalid iterator.");
		}

		constexpr in_order_iterator insert_right_child(const_in_order_iterator iterator, const binary_tree::value_type &value)
		{
			if (!this->iterator_from_this_tree(iterator))
			{
				throw std::logic_error("The iterator is not from this tree.");
			}

			if (this->empty())
			{
				if (iterator == this->in_order_end())
				{
					return this->insert_root(value);
				}

				throw std::logic_error("Tree is empty and the iterator has a dangling reference.");
			}

			if (iterator._node.has_value())
			{
				auto &node = iterator._node.value().get();

				if (node.has_right_child())
				{
					const auto &currentRightChild = node.right_child().value().get();

					this->_size -= binary_tree::size_subtree(currentRightChild);
				}

				auto &newRightChild = node.insert_right_child(value);

				this->update_begin();
				this->update_last_element();
				++(this->_size);

				return in_order_iterator(newRightChild, this->_last_element);
			}

			throw std::logic_error("Attempt of getting the node of an invalid iterator.");
		}

		constexpr in_order_iterator insert_right_child(const_in_order_iterator iterator, binary_tree::value_type &&value)
		{
			if (!this->iterator_from_this_tree(iterator))
			{
				throw std::logic_error("The iterator is not from this tree.");
			}

			if (this->empty())
			{
				if (iterator == this->const_in_order_end())
				{
					return this->insert_root(std::move(value));
				}

				throw std::logic_error("Tree is empty and the iterator has a dangling reference.");
			}

			if (iterator._node)
			{
				auto &node = iterator._node.value().get();

				if (node.has_right_child())
				{
					const auto &currentRightChild = node.right_child().value().get();

					this->_size -= binary_tree::size_subtree(currentRightChild);
				}

				auto &newRightChild = node.insert_right_child(std::move(value));

				this->update_begin();
				this->update_last_element();
				++(this->_size);

				return in_order_iterator(newRightChild, this->_last_element);
			}

			throw std::logic_error("Attempt of getting the node of an invalid iterator.");
		}

		constexpr in_order_iterator erase(const_in_order_iterator iterator)
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
				const auto parent = node.parent_and_my_position();

				if (!parent.has_value())
				{
					this->_root.reset();

					this->_size = 0;
					this->update_begin();
					this->update_last_element();

					return this->begin();
				}

				in_order_iterator returnIterator(parent.value().first.get(), this->_last_element);

				this->_size -= binary_tree::size_subtree(node);

				node.extract_myself();

				this->update_begin();
				this->update_last_element();

				if (parent.value().second == binary_node_side::right)
				{
					return ++returnIterator;
				}

				return returnIterator;
			}

			throw std::logic_error("Attempt to erase an element of an empty tree.");
		}

		constexpr in_order_iterator erase(const_in_order_iterator begin, const_in_order_iterator end)
		{
			if (!this->iterator_from_this_tree(begin) || !this->iterator_from_this_tree(end))
			{
				throw std::logic_error("begin, end or both are not from this tree.");
			}

			if (begin == end)
			{
				in_order_iterator iterator{};

				iterator._node = end._node;
				iterator._last_element = end._last_element;

				return iterator;
			}

			return this->erase(binary_tree::erase_find_first_intersection(begin, end));
		}

		constexpr bool empty() const noexcept
		{
			return this->_root == nullptr;
		}

		constexpr bool binary() const noexcept
		{
			return true;
		}

		constexpr size_t size() const noexcept
		{
			return this->_size;
		}

		constexpr void clear() noexcept
		{
			this->_size = 0;
			this->_root.reset();
			this->_begin._preOrder._node.reset();
			this->_last_element._preOrder._node.reset();
			this->_begin._inOrder._node.reset();
			this->_last_element._inOrder._node.reset();
			this->_begin._postOrder._node.reset();
			this->_last_element._postOrder._node.reset();
		}

		constexpr size_t count(const binary_tree::value_type &value) const
		{
			size_t counter{};
			
			for (auto iterator = this->cbegin(); iterator != this->cend(); ++iterator)
			{
				if (*iterator == value)
				{
					counter++;
				}
			}

			return counter;
		}

		constexpr bool contains(const binary_tree::value_type &value) const
		{
			return std::find(this->cbegin(), this->cend(), value) != this->cend();
		}

		constexpr const_in_order_iterator find(const binary_tree::value_type &value) const
		{
			const_in_order_iterator iterator(std::nullopt, this->_last_element);

			if (this->_root == nullptr)
			{
				return iterator;
			}

			binary_tree::find(*(this->_root.get()), value, iterator);

			return iterator;
		}

		constexpr in_order_iterator find(const binary_tree::value_type &value)
		{
			in_order_iterator iterator(std::nullopt, this->_last_element);

			if (this->_root == nullptr)
			{
				return iterator;
			}

			binary_tree::find(*(this->_root.get()), value, iterator);

			return iterator;
		}

		constexpr std::pair < const_in_order_iterator, std::vector < binary_node_side > > find_with_path(const binary_tree::value_type &value) const
		{
			const_in_order_iterator iterator(std::nullopt, this->_last_element);

			if (this->_root == nullptr)
			{
				return std::make_pair(iterator, std::vector < binary_node_side > ());
			}

			std::vector < binary_node_side > path;

			binary_tree::find_with_path(*(this->_root.get()), value, path, iterator);

			return std::make_pair(iterator, path);
		}

		constexpr std::pair < in_order_iterator, std::vector < binary_node_side > > find_with_path(const binary_tree::value_type &value)
		{
			in_order_iterator iterator(std::nullopt, this->_last_element);

			if (this->_root == nullptr)
			{
				return std::make_pair(iterator, std::vector < binary_node_side > ());
			}

			std::vector < binary_node_side > path;

			binary_tree::find_with_path(*(this->_root.get()), value, path, iterator);

			return std::make_pair(iterator, path);
		}

		constexpr std::vector < std::reference_wrapper < const NodeType > > leaf_nodes() const
		{
			std::vector < std::reference_wrapper < const NodeType > > leafNodes;

			if (this->_root)
			{
				binary_tree::leaf_nodes(*(this->_root.get()), leafNodes);
			}

			return leafNodes;
		}

		constexpr std::vector < std::reference_wrapper < const NodeType > > nodes() const
		{
			std::vector < std::reference_wrapper < const NodeType > > nodes;

			if (this->_root)
			{
				binary_tree::nodes(*(this->_root.get()), nodes);
			}

			return nodes;
		}

		constexpr size_t levels() const noexcept
		{
			size_t levelCount{};

			if (this->_root)
			{
				binary_tree::level_count(*(this->_root.get()), 0, levelCount);
			}

			return levelCount;
		}

		constexpr std::vector < size_t > nodes_per_level() const
		{
			std::vector < size_t > nodesPerLevel;

			if (this->_root)
			{
				binary_tree::nodes_per_level(*(this->_root.get()), 0, nodesPerLevel);
			}

			return nodesPerLevel;
		}

		constexpr binary_tree &operator=(const binary_tree &tree)
		{
			if (this == &tree)
			{
				return *this;
			}

			this->_size = tree._size;
			this->_root.reset(tree._root.get() ? std::make_unique < NodeType > (*(tree._root.get())) : nullptr);

			this->update_begin();
			this->update_last_element();

			return *this;
		}

		constexpr binary_tree &operator=(binary_tree &&) noexcept = default;

		constexpr binary_tree() noexcept = default;

		constexpr binary_tree(const binary_tree &tree) :
			_size(tree._size),
			_root(tree._root.get() ? std::make_unique < NodeType > (*(tree._root.get())) : nullptr)
		{
			this->update_begin();
			this->update_last_element();
		}

		constexpr binary_tree(binary_tree &&) noexcept = default;
	};
}