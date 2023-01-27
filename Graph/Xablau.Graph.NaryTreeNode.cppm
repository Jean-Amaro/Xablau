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

export module xablau.graph:nary_tree_node;
export import :fundamental_concepts;
export import :node_configurations;

export import <deque>;
export import <functional>;
export import <list>;
export import <memory>;
export import <optional>;
export import <stdexcept>;
export import <type_traits>;
export import <utility>;
export import <vector>;

export namespace xablau::graph
{
	template < typename Type, concepts::nary_tree_node_container Container >
	requires (std::is_object < Type > ::value)
	class nary_tree_node final
	{
	public:
		using value_type = Type;

		Type value{};

	private:
		static constexpr nary_tree_node_container_value _container_value = Container::container();

		using container =
			typename std::conditional < _container_value == nary_tree_node_container_value::vector,
				std::vector < std::unique_ptr < nary_tree_node > >,

				typename std::conditional < _container_value == nary_tree_node_container_value::deque,
					std::deque < std::unique_ptr < nary_tree_node > >,
					std::list < std::unique_ptr < nary_tree_node > > >::type > ::type;

		std::optional < std::reference_wrapper < nary_tree_node > > _parent = std::nullopt;

		container _children;

	public:
		using size_type = typename container::size_type;

	private:
		template < typename ThisContainer >
		static constexpr auto calculateIterator(
			ThisContainer &children,
			const size_type position) noexcept
		{
			auto iterator = children.begin();

			if constexpr (nary_tree_node::_container_value == nary_tree_node_container_value::list)
			{
				for (size_t i = 0; i < position; i++)
				{
					++iterator;
				}

				return iterator;
			}

			else
			{
				return iterator + position;
			}
		}

		template < typename ChildType >
		auto &moveRollback(auto lambda, ChildType &valueChild, ChildType &newChild)
		{
			try
			{
				return lambda();
			}
			catch (const std::exception &exception)
			{
				std::swap(
					static_cast < ChildType & > (valueChild),
					static_cast < ChildType & > (newChild));

				throw exception;
			}
			catch (...)
			{
				std::swap(
					static_cast < ChildType & > (valueChild),
					static_cast < ChildType & > (newChild));

				throw std::runtime_error(""); // TODO: Create message.
			}
		}

		void updateChildren()
		{
			for (auto &child : this->_children)
			{
				if (child)
				{
					child->_parent = *this;
				}
			}
		}

	public:
		[[nodiscard]] constexpr bool operator<(const nary_tree_node &node) const
		{
			return this->value < node.value;
		}

		[[nodiscard]] constexpr bool operator<=(const nary_tree_node &node) const
		{
			return this->value <= node.value;
		}

		[[nodiscard]] constexpr bool operator==(const nary_tree_node &node) const
		{
			return this->value == node.value;
		}

		[[nodiscard]] constexpr bool operator>=(const nary_tree_node &node) const
		{
			return this->value >= node.value;
		}

		[[nodiscard]] constexpr bool operator>(const nary_tree_node &node) const
		{
			return this->value > node.value;
		}

		[[nodiscard]] constexpr bool operator!=(const nary_tree_node &node) const
		{
			return this->value != node.value;
		}

		constexpr nary_tree_node &insert_child(const Type &valueChild, const size_type position)
		{
			if (position > this->size())
			{
				throw std::out_of_range("Argument \"position\" is greater than \"size\".");
			}

			auto pointer = std::make_unique < nary_tree_node > (valueChild);
			const auto iterator = nary_tree_node::calculateIterator(this->_children, position);

			auto &child = *(this->_children.insert(iterator, std::move(pointer))->get());

			child._parent = *this;

			return child;
		}

		constexpr nary_tree_node &insert_back_child(const Type &valueChild)
		{
			auto pointer = std::make_unique < nary_tree_node > (valueChild);

			this->_children.push_back(std::move(pointer));

			auto &child = *(this->_children.back().get());

			child._parent = *this;

			return child;
		}

		constexpr nary_tree_node &insert_front_child(const Type &valueChild)
		{
			auto pointer = std::make_unique < nary_tree_node > (valueChild);

			if constexpr (_container_value == nary_tree_node_container_value::vector)
			{
				auto &child =
					*(this->_children.insert(this->_children.begin(), std::move(pointer))->get());

				child._parent = *this;

				return child;
			}

			else
			{
				this->_children.push_front(std::move(pointer));

				auto &child = *(this->_children.front().get());

				child._parent = *this;

				return child;
			}
		}

		constexpr nary_tree_node &insert_child(Type &&valueChild, const size_type position)
		requires (std::swappable < Type >)
		{
			if (position > this->size())
			{
				throw std::out_of_range("Argument \"position\" is greater than \"size\".");
			}

			auto pointer = std::make_unique < nary_tree_node > (valueChild);

			auto lambda =
				[&] () -> auto &
				{
					auto &child =
						*(this->_children.insert(
							nary_tree_node::calculateIterator(this->_children, position),
							std::move(pointer))->get());

					child._parent = *this;

					return child;
				};

			return this->moveRollback(std::move(lambda), valueChild, pointer->value);
		}

		constexpr nary_tree_node &insert_back_child(Type &&valueChild)
		requires (std::swappable < Type >)
		{
			auto pointer = std::make_unique < nary_tree_node > (valueChild);

			auto lambda =
				[&] () -> auto &
				{
					this->_children.push_back(std::move(pointer));

					auto &child = *(this->_children.back().get());

					child._parent = *this;

					return child;
				};

			return this->moveRollback(std::move(lambda), valueChild, pointer->value);
		}

		constexpr nary_tree_node &insert_front_child(Type &&valueChild)
		requires (std::swappable < Type >)
		{
			auto pointer = std::make_unique < nary_tree_node > (valueChild);

			auto lambda =
				[&] () -> auto &
				{
					if constexpr (_container_value == nary_tree_node_container_value::vector)
					{
						auto &child =
							*(this->_children.insert(
								this->_children.begin(), std::move(pointer))->get());

						child._parent = *this;

						return child;
					}

					else
					{
						this->_children.push_front(std::move(pointer));

						auto &child = *(this->_children.front().get());

						child._parent = *this;

						return child;
					}
				};

			return this->moveRollback(std::move(lambda), valueChild, pointer->value);
		}

		constexpr nary_tree_node &insert_child(const nary_tree_node &child, const size_type position)
		{
			if (position > this->size())
			{
				throw std::out_of_range("Argument \"position\" is greater than \"size\".");
			}

			auto pointer = std::make_unique < nary_tree_node > (child);

			auto &_child =
				*(this->_children.insert(
					nary_tree_node::calculateIterator(this->_children, position),
					std::move(pointer))->get());

			_child._parent = *this;

			return _child;
		}

		constexpr nary_tree_node &insert_back_child(const nary_tree_node &child)
		{
			auto pointer = std::make_unique < nary_tree_node > (child);

			this->_children.push_back(std::move(pointer));

			auto &_child = *(this->_children.back().get());

			_child._parent = *this;

			return _child;
		}

		constexpr nary_tree_node &insert_front_child(const nary_tree_node &child)
		{
			auto pointer = std::make_unique < nary_tree_node > (child);

			if constexpr (_container_value == nary_tree_node_container_value::vector)
			{
				auto &_child =
					*(this->_children.insert(this->_children.begin(), std::move(pointer))->get());

				_child._parent = *this;

				return _child;
			}

			else
			{
				this->_children.push_front(std::move(pointer));

				auto &_child = *(this->_children.front().get());

				_child._parent = *this;

				return _child;
			}
		}

		constexpr nary_tree_node &insert_child(nary_tree_node &&child, const size_type position)
		requires (std::swappable < Type >)
		{
			if (position > this->size())
			{
				throw std::out_of_range("Argument \"position\" is greater than \"size\".");
			}

			auto pointer = std::make_unique < nary_tree_node > (child);

			auto lambda =
				[&] () -> auto &
				{
					auto &insertedChild =
						*(this->_children.insert(
							nary_tree_node::calculateIterator(this->_children, position), std::move(pointer))->get());

					insertedChild._parent = *this;

					return insertedChild;
				};

			return this->moveRollback(std::move(lambda), child, *pointer);
		}

		constexpr nary_tree_node &insert_back_child(nary_tree_node &&child)
		requires (std::swappable < Type >)
		{
			auto pointer = std::make_unique < nary_tree_node > (child);

			auto lambda =
				[&] () -> auto &
				{
					this->_children.push_back(std::move(pointer));

					auto &insertedChild = *(this->_children.back().get());

					insertedChild._parent = *this;

					return insertedChild;
				};

			return this->moveRollback(std::move(lambda), child, *pointer);
		}

		constexpr nary_tree_node &insert_front_child(nary_tree_node &&child)
		requires (std::swappable < Type >)
		{
			auto pointer = std::make_unique < nary_tree_node > (child);

			auto lambda =
				[&] () -> auto &
				{
					if constexpr (_container_value == nary_tree_node_container_value::vector)
					{
						auto &insertedChild =
							*(this->_children.insert(
								this->_children.begin(), std::move(pointer))->get());

						insertedChild._parent = *this;

						return insertedChild;
					}

					else
					{
						this->_children.push_front(std::move(pointer));

						auto &insertedChild = *(this->_children.front().get());

						insertedChild._parent = *this;

						return insertedChild;
					}
				};

			return this->moveRollback(std::move(lambda), child, *pointer);
		}

		constexpr void invalidate_last_valid_child()
		{
			for (auto iterator = this->_children.rbegin(); iterator != this->_children.rend(); ++iterator)
			{
				if (*iterator)
				{
					iterator->reset();

					return;
				}
			}
		}

		constexpr void invalidate_first_valid_child()
		{
			for (auto &child : this->_children)
			{
				if (child)
				{
					child.reset();

					return;
				}
			}
		}

		constexpr void invalidate_child(const size_type position)
		{
			if (position >= this->size())
			{
				throw std::out_of_range("Argument \"position\" is greater or equal to \"size\".");
			}

			nary_tree_node::calculateIterator(this->_children, position)->reset();
		}

		constexpr void erase_back_child()
		{
			if (this->_children.empty())
			{
				throw std::out_of_range("There is not a single child to remove.");
			}

			this->_children.pop_back();
		}

		constexpr void erase_front_child()
		{
			if (this->_children.empty())
			{
				throw std::out_of_range("There is not a single child to remove.");
			}

			if constexpr (_container_value == nary_tree_node_container_value::vector)
			{
				this->_children.erase(this->_children.begin());
			}

			else
			{
				this->_children.pop_front();
			}
		}

		constexpr void erase_child(const size_type position)
		{
			if (position >= this->size())
			{
				throw std::out_of_range("Argument \"position\" is greater or equal to \"size\".");
			}

			this->_children.erase(nary_tree_node::calculateIterator(this->_children, position));
		}

		[[nodiscard]] constexpr bool empty() const noexcept
		{
			return this->_children.empty();
		}

		[[nodiscard]] constexpr size_type valid_size() const noexcept
		{
			size_type validElements{};

			for (const auto &child : this->_children)
			{
				if (child)
				{
					validElements++;
				}
			}

			return validElements;
		}

		[[nodiscard]] constexpr size_type size() const noexcept
		{
			return this->_children.size();
		}

		constexpr void clear() noexcept
		{
			this->_children.clear();
		}

		[[nodiscard]] constexpr bool has_valid_child(const size_type position) const noexcept
		{
			return this->has_child(position) && (*nary_tree_node::calculateIterator(this->_children, position)) != nullptr;
		}

		[[nodiscard]] constexpr bool has_child(const size_type position) const noexcept
		{
			return position < this->size();
		}

		[[nodiscard]] constexpr bool has_parent() const noexcept
		{
			return this->_parent.has_value();
		}

		[[nodiscard]] constexpr std::vector < std::reference_wrapper < const nary_tree_node > > valid_children() const
		{
			std::vector < std::reference_wrapper < const nary_tree_node > > children;

			children.reserve(this->size());

			for (const auto &child : this->_children)
			{
				if (child)
				{
					children.emplace_back(*(child.get()));
				}
			}

			return children;
		}

		[[nodiscard]] constexpr std::vector < std::reference_wrapper < nary_tree_node > > valid_children()
		{
			std::vector < std::reference_wrapper < nary_tree_node > > children;

			children.reserve(this->size());

			for (auto &child : this->_children)
			{
				if (child)
				{
					children.emplace_back(*(child.get()));
				}
			}

			return children;
		}

		[[nodiscard]] constexpr std::vector < std::optional < std::reference_wrapper < const nary_tree_node > > > children() const
		{
			std::vector < std::optional < std::reference_wrapper < const nary_tree_node > > > children;

			children.reserve(this->size());

			for (const auto &child : this->_children)
			{
				if (child)
				{
					children.emplace_back(*(child.get()));
				}

				else
				{
					children.emplace_back(std::nullopt);
				}
			}

			return children;
		}

		[[nodiscard]] constexpr std::vector < std::optional < std::reference_wrapper < nary_tree_node > > > children()
		{
			std::vector < std::optional < std::reference_wrapper < nary_tree_node > > > children;

			children.reserve(this->size());

			for (auto &child : this->_children)
			{
				if (child)
				{
					children.emplace_back(*(child.get()));
				}

				else
				{
					children.emplace_back(std::nullopt);
				}
			}

			return children;
		}

		[[nodiscard]] constexpr std::optional < std::reference_wrapper < const nary_tree_node > > parent() const noexcept
		{
			if (this->_parent.has_value())
			{
				return this->_parent.value().get();
			}

			return std::nullopt;
		}

		[[nodiscard]] constexpr std::optional < std::reference_wrapper < nary_tree_node > > parent() noexcept
		{
			if (this->_parent.has_value())
			{
				return this->_parent.value().get();
			}

			return std::nullopt;
		}

		[[nodiscard]] constexpr std::optional < std::reference_wrapper < const nary_tree_node > > left_valid_sibling() const
		{
			const auto parentAndPosition = this->parent_and_my_position();

			if (parentAndPosition.has_value())
			{
				const auto &parentAndPositionPair = parentAndPosition.value();
				const auto &parent = parentAndPositionPair.first.get();
				const auto position = parentAndPositionPair.second;

				for (auto iterator = std::make_reverse_iterator(nary_tree_node::calculateIterator(parent._children, position));
					iterator != parent._children.crend();
					++iterator)
				{
					if (*iterator)
					{
						return *(iterator->get());
					}
				}
			}

			return std::nullopt;
		}

		[[nodiscard]] constexpr std::optional < std::reference_wrapper < const nary_tree_node > > right_valid_sibling() const
		{
			const auto parentAndPosition = this->parent_and_my_position();

			if (parentAndPosition.has_value())
			{
				const auto &parentAndPositionPair = parentAndPosition.value();
				const auto &parent = parentAndPositionPair.first.get();
				const auto position = parentAndPositionPair.second;

				for (auto iterator = ++(nary_tree_node::calculateIterator(parent._children, position));
					iterator != parent._children.cend();
					++iterator)
				{
					if (*iterator)
					{
						return *(iterator->get());
					}
				}
			}

			return std::nullopt;
		}

		[[nodiscard]] constexpr
			std::optional <
				std::pair <
					std::reference_wrapper < const nary_tree_node >,
					size_type > > parent_and_my_valid_position() const
		{
			if (this->_parent.has_value())
			{
				auto &parent = this->_parent.value().get();

				std::pair < std::reference_wrapper < const nary_tree_node >, size_type > pair(
					parent,
					size_type{});

				size_type counter{};

				for (auto &sibling : parent._children)
				{
					if (sibling.get() == this)
					{
						pair.second = counter;

						return pair;
					}

					if (sibling.get() != nullptr)
					{
						counter++;
					}
				}
			}

			return std::nullopt;
		}

		[[nodiscard]] constexpr
			std::optional <
				std::pair <
					std::reference_wrapper < const nary_tree_node >,
					size_type > > parent_and_my_position() const
		{
			if (this->_parent.has_value())
			{
				auto &parent = this->_parent.value().get();

				std::pair < std::reference_wrapper < const nary_tree_node >, size_type > pair(
					parent,
					size_type{});

				size_type counter{};

				for (auto &sibling : parent._children)
				{
					if (sibling.get() == this)
					{
						pair.second = counter;

						return pair;
					}

					counter++;
				}
			}

			return std::nullopt;
		}

		[[nodiscard]] std::optional < std::reference_wrapper < const nary_tree_node > > child(const size_type position) const
		{
			if (position >= this->size())
			{
				return std::nullopt;
			}

			auto iterator = nary_tree_node::calculateIterator(this->_children, position);

			if (*iterator)
			{
				return *(iterator->get());
			}

			return std::nullopt;
		}

		[[nodiscard]] std::optional < std::reference_wrapper < const nary_tree_node > > back_child() const noexcept
		{
			if (!this->_children.empty() && this->_children.back() != nullptr)
			{
				return *(this->_children.back());
			}

			return std::nullopt;
		}

		[[nodiscard]] std::optional < std::reference_wrapper < const nary_tree_node > > front_child() const
		{
			if (!this->_children.empty() && this->_children.front() != nullptr)
			{
				return *(this->_children.front());
			}

			return std::nullopt;
		}

		[[nodiscard]] std::optional < std::reference_wrapper < const nary_tree_node > > valid_back_child() const noexcept
		{
			for (auto iterator = this->_children.crbegin(); iterator != this->_children.crend(); ++iterator)
			{
				if (*iterator)
				{
					return *(*iterator);
				}
			}

			return std::nullopt;
		}

		[[nodiscard]] std::optional < std::reference_wrapper < const nary_tree_node > > valid_front_child() const noexcept
		{
			for (const auto &child : this->_children)
			{
				if (child)
				{
					return *child;
				}
			}

			return std::nullopt;
		}

		[[nodiscard]] constexpr std::optional < std::reference_wrapper < nary_tree_node > > left_valid_sibling()
		{
			const auto parentAndPosition = this->parent_and_my_position();

			if (parentAndPosition.has_value())
			{
				const auto &parentAndPositionPair = parentAndPosition.value();
				auto &parent = parentAndPositionPair.first.get();
				const auto position = parentAndPositionPair.second;

				for (auto iterator = std::make_reverse_iterator(nary_tree_node::calculateIterator(parent._children, position));
					iterator != parent._children.rend();
					++iterator)
				{
					if (*iterator)
					{
						return *(iterator->get());
					}
				}
			}

			return std::nullopt;
		}

		[[nodiscard]] constexpr std::optional < std::reference_wrapper < nary_tree_node > > right_valid_sibling()
		{
			const auto parentAndPosition = this->parent_and_my_position();

			if (parentAndPosition.has_value())
			{
				const auto &parentAndPositionPair = parentAndPosition.value();
				auto &parent = parentAndPositionPair.first.get();
				const auto position = parentAndPositionPair.second;

				for (auto iterator = ++(nary_tree_node::calculateIterator(parent._children, position));
					iterator != parent._children.end();
					++iterator)
				{
					if (*iterator)
					{
						return *(iterator->get());
					}
				}
			}

			return std::nullopt;
		}

		[[nodiscard]] constexpr
			std::optional <
				std::pair <
					std::reference_wrapper < nary_tree_node >,
					size_type > > parent_and_my_valid_position()
		{
			if (this->_parent.has_value())
			{
				auto &parent = this->_parent.value().get();

				std::pair < std::reference_wrapper < nary_tree_node >, size_type > pair(
					parent,
					size_type{});

				size_type counter{};

				for (auto &sibling : parent._children)
				{
					if (sibling.get() == this)
					{
						pair.second = counter;

						return pair;
					}

					if (sibling.get() != nullptr)
					{
						counter++;
					}
				}
			}

			return std::nullopt;
		}

		[[nodiscard]] constexpr
			std::optional <
				std::pair <
					std::reference_wrapper < nary_tree_node >,
					size_type > > parent_and_my_position()
		{
			if (this->_parent.has_value())
			{
				auto &parent = this->_parent.value().get();

				std::pair < std::reference_wrapper < nary_tree_node >, size_type > pair(
					parent,
					size_type{});

				size_type counter{};

				for (auto &sibling : parent._children)
				{
					if (sibling.get() == this)
					{
						pair.second = counter;

						return pair;
					}

					counter++;
				}
			}

			return std::nullopt;
		}

		[[nodiscard]] constexpr std::optional < std::reference_wrapper < nary_tree_node > > child(const size_type position)
		{
			if (position >= this->size())
			{
				return std::nullopt;
			}

			const auto iterator = nary_tree_node::calculateIterator(this->_children, position);

			if (*iterator)
			{
				return *(iterator->get());
			}

			return std::nullopt;
		}

		[[nodiscard]] constexpr std::optional < std::reference_wrapper < nary_tree_node > > back_child()
		{
			if (!this->_children.empty() && this->_children.back() != nullptr)
			{
				return *(this->_children.back());
			}

			return std::nullopt;
		}

		[[nodiscard]] constexpr std::optional < std::reference_wrapper < nary_tree_node > > front_child()
		{
			if (!this->_children.empty() && this->_children.front() != nullptr)
			{
				return *(this->_children.front());
			}

			return std::nullopt;
		}

		[[nodiscard]] constexpr std::optional < std::reference_wrapper < nary_tree_node > > valid_back_child() noexcept
		{
			for (auto iterator = this->_children.rbegin(); iterator != this->_children.rend(); ++iterator)
			{
				if (*iterator)
				{
					return *(*iterator);
				}
			}

			return std::nullopt;
		}

		[[nodiscard]] constexpr std::optional < std::reference_wrapper < nary_tree_node > > valid_front_child() noexcept
		{
			for (auto &child : this->_children)
			{
				if (child)
				{
					return *child;
				}
			}

			return std::nullopt;
		}

		constexpr
			std::optional <
				std::pair <
					std::unique_ptr < nary_tree_node >,
					size_type > > extract_and_invalidate_myself_on_parent()
		{
			if (this->_parent.has_value())
			{
				size_type counter{};
				std::unique_ptr < nary_tree_node > myself{};
				auto &parent = this->_parent.value().get();

				for (auto &sibling : parent._children)
				{
					if (sibling.get() == this)
					{
						sibling.swap(myself);

						this->_parent.reset();

						return std::make_pair(std::move(myself), counter);
					}

					counter++;
				}
			}

			return std::nullopt;
		}

		constexpr
			std::optional <
				std::pair <
					std::unique_ptr < nary_tree_node >,
					size_type > > extract_and_erase_myself_on_parent()
		{
			if (this->_parent.has_value())
			{
				size_type counter{};
				std::unique_ptr < nary_tree_node > myself{};
				auto &parent = this->_parent.value().get();

				for (auto iterator = parent._children.begin();
					iterator != parent._children.end();
					++iterator)
				{
					if (iterator->get() == this)
					{
						iterator->swap(myself);

						parent._children.erase(iterator);

						this->_parent.reset();

						return std::make_pair(std::move(myself), counter);
					}

					counter++;
				}
			}

			return std::nullopt;
		}

		constexpr std::unique_ptr < nary_tree_node > extract_and_invalidate_child(const size_type position)
		{
			if (position >= this->size())
			{
				return nullptr;
			}

			return std::move(*(nary_tree_node::calculateIterator(this->_children, position)));
		}

		constexpr std::unique_ptr < nary_tree_node > extract_and_erase_child(const size_type position)
			noexcept(std::is_nothrow_copy_assignable < Type > ::value)
		{
			if (position >= this->size())
			{
				return nullptr;
			}

			auto iterator = nary_tree_node::calculateIterator(this->_children, position);
			auto pointer = std::move(*iterator);

			this->_children.erase(iterator);

			return pointer;
		}

		constexpr std::unique_ptr < nary_tree_node > extract_and_invalidate_back_child() noexcept
		{
			if (this->_children.empty())
			{
				return nullptr;
			}

			return std::move(this->_children.back());
		}

		constexpr std::unique_ptr < nary_tree_node > extract_and_erase_back_child()
			noexcept(std::is_nothrow_copy_assignable < Type > ::value)
		{
			if (this->_children.empty())
			{
				return nullptr;
			}

			std::unique_ptr < nary_tree_node > pointer(std::move(this->_children.back()));

			this->_children.pop_back();

			return pointer;
		}

		constexpr std::unique_ptr < nary_tree_node > extract_and_invalidate_front_child() noexcept
		{
			if (this->_children.empty())
			{
				return nullptr;
			}

			return std::move(this->_children.front());
		}

		constexpr std::unique_ptr < nary_tree_node > extract_and_erase_front_child()
			noexcept(std::is_nothrow_copy_assignable < Type > ::value)
		{
			if (this->_children.empty())
			{
				return nullptr;
			}

			std::unique_ptr < nary_tree_node > pointer(std::move(this->_children.front()));

			if constexpr (_container_value == nary_tree_node_container_value::vector)
			{
				this->_children.erase(this->_children.begin());
			}

			else
			{
				this->_children.pop_front();
			}

			return pointer;
		}

		constexpr void swap(nary_tree_node &node)
			noexcept(
				std::is_nothrow_move_constructible < Type >::value &&
				std::is_nothrow_move_assignable < Type > ::value)
		{
			std::swap(this->value, node.value);
			std::swap(this->_parent, node._parent);
			std::swap(this->_children, node._children);
		}

		constexpr nary_tree_node &operator=(const nary_tree_node &node)
		{
			if (this == &node)
			{
				return *this;
			}

			container newChildren;

			if constexpr (_container_value == nary_tree_node_container_value::vector)
			{
				newChildren.reserve(node.size());
			}

			for (const auto &child : node._children)
			{
				newChildren.push_back(child ? std::make_unique < nary_tree_node > (*(child.get())) : nullptr);
			}

			this->node.value = node.value;

			this->_children = std::move(newChildren);

			this->updateChildren();

			return *this;
		}

		constexpr nary_tree_node &operator=(nary_tree_node &&node)
		{
			this->value = std::move(node.value);
			this->_children = std::move(node._children);
			this->updateChildren();

			node.extract_and_invalidate_myself_on_parent();

			return *this;
		}

		constexpr nary_tree_node()
			noexcept(std::is_nothrow_default_constructible < Type > ::value) = default;

		constexpr nary_tree_node(const Type &data)
			noexcept(std::is_nothrow_copy_constructible < Type > ::value) :

			value(data)
		{
		}

		constexpr nary_tree_node(Type &&data)
			noexcept(std::is_nothrow_move_constructible < Type > ::value) :

			value(std::move(data))
		{
		}

		constexpr nary_tree_node(const nary_tree_node &node) :
			value(node.value)
		{
			const auto &children = node._children;

			if constexpr (_container_value == nary_tree_node_container_value::vector)
			{
				this->_children.reserve(children.size());
			}

			for (const auto &child : children)
			{
				if (child)
				{
					this->_children.push_back(
						std::make_unique < nary_tree_node > (*(child.get())));
				}

				else
				{
					this->_children.emplace_back(nullptr);
				}
			}

			this->updateChildren();
		}

		constexpr nary_tree_node(nary_tree_node &&node)
			noexcept(std::is_nothrow_move_constructible < Type >::value) :

			value(std::move(node.value)),
			_children(std::move(node._children))
		{
			this->updateChildren();

			node.extract_and_invalidate_myself_on_parent();
		}
	};
}

export namespace std
{
	template < typename Type, xablau::graph::concepts::nary_tree_node_container Container >
	requires (std::is_object < Type > ::value)
	struct hash < xablau::graph::nary_tree_node < Type, Container > >
	{
		size_t operator()(const xablau::graph::nary_tree_node < Type, Container > &node) const
		{
			return std::hash < Type > {}(node.value);
		}
	};
}