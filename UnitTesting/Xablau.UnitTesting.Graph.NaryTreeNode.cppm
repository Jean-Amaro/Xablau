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

#include <gtest/gtest.h>

import <algorithm>;
import <concepts>;

import xablau.graph;
import xablau.testing;

namespace xablau::unit_testing
{
	template <
		typename Type,
		xablau::graph::concepts::nary_tree_node_container Container >
	struct GraphNaryTreeNodeTypeDefinitions
	{
		using value_type = Type;
		using container_type = Container;

		using NodeType = xablau::graph::nary_tree_node < Type, Container >;
	};

	class GraphNaryTreeNodeCustomType
	{
	private:
		std::vector < size_t > data;

	public:
		explicit operator size_t() const { return this->data.back(); }

		auto operator<=>(const GraphNaryTreeNodeCustomType &) const = default;

		GraphNaryTreeNodeCustomType &operator=(const GraphNaryTreeNodeCustomType &) = default;

		GraphNaryTreeNodeCustomType &operator=(GraphNaryTreeNodeCustomType &&) noexcept = default;

		GraphNaryTreeNodeCustomType()
		{
			this->data.push_back(0);
		}

		GraphNaryTreeNodeCustomType(const size_t data)
		{
			this->data.push_back(data);
		}

		GraphNaryTreeNodeCustomType(const GraphNaryTreeNodeCustomType &) = default;

		GraphNaryTreeNodeCustomType(GraphNaryTreeNodeCustomType &&) noexcept = default;
	};

	template < typename Types >
	class GraphNaryTreeNodeTest : public ::testing::Test
	{
	public:
		constexpr void validateEmptyRoot(auto &root) const
		{
			EXPECT_TRUE(root.empty());
			EXPECT_FALSE(root.has_parent());
			EXPECT_EQ(root.parent(), std::nullopt);
			EXPECT_EQ(root.parent_and_my_position(), std::nullopt);
			EXPECT_EQ(root.valid_back_child(), std::nullopt);
			EXPECT_EQ(root.back_child(), std::nullopt);
			EXPECT_EQ(root.valid_front_child(), std::nullopt);
			EXPECT_EQ(root.front_child(), std::nullopt);
			EXPECT_EQ(root.valid_size(), size_t{});
			EXPECT_EQ(root.size(), size_t{});
		}

		constexpr void validateChildren(auto &root, auto &children, bool checkPosition = true) const
		{
			EXPECT_FALSE(root.has_parent());
			EXPECT_EQ(root.parent_and_my_position(), std::nullopt);

			EXPECT_EQ(root.size(), children.size());
			EXPECT_EQ(root.valid_size(), children.size());

			for (size_t i = 0; i < children.size(); i++)
			{
				EXPECT_TRUE(root.has_valid_child(i));
				EXPECT_TRUE(root.has_child(i));
				EXPECT_EQ(root.child(i).value().get(), children[i].get());
				EXPECT_EQ(std::addressof(root.child(i).value().get()), std::addressof(children[i].get()));

				EXPECT_TRUE(children[i].get().has_parent());
				EXPECT_EQ(children[i].get().parent().value().get(), root);
				EXPECT_EQ(children[i].get().parent_and_my_position().value().first.get(), root);

				if (checkPosition)
				{
					EXPECT_EQ(children[i].get().parent_and_my_position().value().second, i);
				}

				EXPECT_EQ(std::addressof(children[i].get().parent().value().get()), std::addressof(root));
			}
		}

		template < bool Move, bool Value >
		constexpr void insertFront(auto &root, auto &children, auto value)
		{
			if constexpr (Move)
			{
				if constexpr (Value)
				{
					children.emplace_back(root.insert_front_child(typename Types::value_type{value}));
				}

				else
				{
					children.emplace_back(root.insert_front_child(typename Types::NodeType{value}));
				}
			}

			else
			{
				if constexpr (Value)
				{
					const typename Types::value_type childValue{value};

					children.emplace_back(root.insert_front_child(childValue));
				}

				else
				{
					const typename Types::NodeType childNode{value};

					children.emplace_back(root.insert_front_child(childNode));
				}
			}
		}

		template < bool Move, bool Value >
		constexpr void insertMiddle(auto &root, auto &children, auto value, const size_t position)
		{
			if constexpr (Move)
			{
				if constexpr (Value)
				{
					children.emplace_back(root.insert_child(typename Types::value_type{value}, position));
				}

				else
				{
					children.emplace_back(root.insert_child(typename Types::NodeType{value}, position));
				}
			}

			else
			{
				if constexpr (Value)
				{
					const typename Types::value_type childValue{value};

					children.emplace_back(root.insert_child(childValue, position));
				}

				else
				{
					const typename Types::NodeType childNode{value};

					children.emplace_back(root.insert_child(childNode, position));
				}
			}
		}

		template < bool Move, bool Value >
		constexpr void insertBack(auto &root, auto &children, auto value)
		{
			if constexpr (Move)
			{
				if constexpr (Value)
				{
					children.emplace_back(root.insert_back_child(typename Types::value_type{value}));
				}

				else
				{
					children.emplace_back(root.insert_back_child(typename Types::NodeType{value}));
				}
			}

			else
			{
				if constexpr (Value)
				{
					const typename Types::value_type childValue{value};

					children.emplace_back(root.insert_back_child(childValue));
				}

				else
				{
					const typename Types::NodeType childNode{value};

					children.emplace_back(root.insert_back_child(childNode));
				}
			}
		}

		template < bool Move, bool Value >
		constexpr void insert(
			typename Types::NodeType &root,
			std::vector < std::reference_wrapper < typename Types::NodeType > > &children)
		{
			this->validateEmptyRoot(root);

			for (size_t i = 3; i > 0; i--)
			{
				this->insertFront < Move, Value > (root, children, i);
			}

			for (size_t i = 7; i < 10; i++)
			{
				this->insertBack < Move, Value > (root, children, i);
			}

			for (size_t i = 3; i < 6; i++)
			{
				this->insertMiddle < Move, Value > (root, children, i + 1, i);
			}

			std::sort(children.begin(), children.end(),
				[] (auto &node1, auto &node2) -> bool
				{
					return node1.get().value < node2.get().value;
				});

			this->validateChildren(root, children);
		}

		template < bool Move, bool Value >
		constexpr void insert()
		{
			typename Types::NodeType root{};
			std::vector < std::reference_wrapper < typename Types::NodeType > > children;

			this->insert < Move, Value > (root, children);
		}
	};

	TYPED_TEST_SUITE_P(GraphNaryTreeNodeTest);

	TYPED_TEST_P(GraphNaryTreeNodeTest, InsertionMoveValue)
	{
		this->insert < false, false > ();
	}

	TYPED_TEST_P(GraphNaryTreeNodeTest, InsertionCopyValue)
	{
		this->insert < false, true > ();
	}

	TYPED_TEST_P(GraphNaryTreeNodeTest, InsertionMoveNode)
	{
		this->insert < true, false > ();
	}

	TYPED_TEST_P(GraphNaryTreeNodeTest, InsertionCopyNode)
	{
		this->insert < true, true > ();
	}

	TYPED_TEST_P(GraphNaryTreeNodeTest, Invalidation)
	{
		typename TypeParam::NodeType root{};
		std::vector < std::reference_wrapper < typename TypeParam::NodeType > > children;

		this->insert < false, false > (root, children);

		for (size_t i = 0; i < 3; i++)
		{
			root.invalidate_first_valid_child();

			EXPECT_TRUE(root.has_child(i));
			EXPECT_FALSE(root.has_valid_child(i));
		}

		for (size_t i = 8; i > 5; i--)
		{
			root.invalidate_last_valid_child();

			EXPECT_TRUE(root.has_child(i));
			EXPECT_FALSE(root.has_valid_child(i));
		}

		for (size_t i = 3; i < 6; i++)
		{
			root.invalidate_child(i);

			EXPECT_TRUE(root.has_child(i));
			EXPECT_FALSE(root.has_valid_child(i));
		}

		EXPECT_EQ(root.valid_size(), 0);
		EXPECT_EQ(root.size(), 9);
	}

	TYPED_TEST_P(GraphNaryTreeNodeTest, Deletion)
	{
		typename TypeParam::NodeType root{};
		std::vector < std::reference_wrapper < typename TypeParam::NodeType > > children;

		this->insert < false, false > (root, children);

		for (size_t i = 0; i < 3; i++)
		{
			root.erase_front_child();
			children.erase(children.cbegin());

			this->validateChildren(root, children, false);

			EXPECT_EQ(root.front_child().value().get().value, i + 2);
		}

		for (size_t i = 8; i > 5; i--)
		{
			root.erase_back_child();
			children.pop_back();

			this->validateChildren(root, children, false);

			EXPECT_EQ(root.back_child().value().get().value, i);
		}

		root.erase_child(1);
		children.erase(children.cbegin() + 1);

		this->validateChildren(root, children, false);

		EXPECT_EQ(root.front_child().value().get().value, 4);
		EXPECT_EQ(root.back_child().value().get().value, 6);

		EXPECT_EQ(root.valid_size(), 2);
		EXPECT_EQ(root.size(), 2);

		root.clear();

		EXPECT_EQ(root.valid_size(), 0);
		EXPECT_EQ(root.size(), 0);
	}

	TYPED_TEST_P(GraphNaryTreeNodeTest, Sibling)
	{
		typename TypeParam::NodeType root{};
		std::vector < std::reference_wrapper < typename TypeParam::NodeType > > children;

		this->insert < false, false > (root, children);

		EXPECT_EQ(children[0].get().left_valid_sibling(), std::nullopt);
		EXPECT_EQ(children[0].get().right_valid_sibling().value().get().value, 2);

		for (size_t i = 1; i < children.size() - 1; i++)
		{
			EXPECT_EQ(children[i].get().left_valid_sibling().value().get().value, i);
			EXPECT_EQ(children[i].get().right_valid_sibling().value().get().value, i + 2);
		}

		EXPECT_EQ(children[8].get().left_valid_sibling().value().get().value, 8);
		EXPECT_EQ(children[8].get().right_valid_sibling(), std::nullopt);
	}

	TYPED_TEST_P(GraphNaryTreeNodeTest, ExtractionAndInvalidation)
	{
		typename TypeParam::NodeType root{};
		std::vector < std::reference_wrapper < typename TypeParam::NodeType > > children;

		this->insert < false, false > (root, children);

		auto extractedNode = root.extract_and_invalidate_front_child();

		EXPECT_TRUE(root.has_child(0));
		EXPECT_FALSE(root.has_valid_child(0));
		EXPECT_EQ(std::addressof(*extractedNode), std::addressof(children[0].get()));

		extractedNode = root.extract_and_invalidate_back_child();

		EXPECT_TRUE(root.has_child(8));
		EXPECT_FALSE(root.has_valid_child(8));
		EXPECT_EQ(std::addressof(*extractedNode), std::addressof(children[8].get()));

		for (size_t i = 3; i < 6; i++)
		{
			extractedNode = root.extract_and_invalidate_child(i);

			EXPECT_TRUE(root.has_child(i));
			EXPECT_FALSE(root.has_valid_child(i));
			EXPECT_EQ(std::addressof(*extractedNode), std::addressof(children[i].get()));
		}

		EXPECT_EQ(root.valid_size(), 4);
		EXPECT_EQ(root.size(), 9);
	}

	TYPED_TEST_P(GraphNaryTreeNodeTest, ExtractionAndDeletion)
	{
		typename TypeParam::NodeType root{};
		std::vector < std::reference_wrapper < typename TypeParam::NodeType > > children;

		this->insert < false, false > (root, children);

		for (size_t i = 0; i < 3; i++)
		{
			auto extractedNode = root.extract_and_erase_front_child();
			auto extractedReference = children.front();

			children.erase(children.cbegin());

			this->validateChildren(root, children, false);

			EXPECT_EQ(std::addressof(*extractedNode), std::addressof(extractedReference.get()));
		}

		for (size_t i = 8; i > 5; i--)
		{
			auto extractedNode = root.extract_and_erase_back_child();
			auto extractedReference = children.back();

			children.pop_back();

			this->validateChildren(root, children, false);

			EXPECT_EQ(std::addressof(*extractedNode), std::addressof(extractedReference.get()));
		}

		auto extractedNode = root.extract_and_erase_child(1);
		auto extractedReference = children[1];

		children.erase(children.cbegin() + 1);

		this->validateChildren(root, children, false);

		EXPECT_EQ(std::addressof(*extractedNode), std::addressof(extractedReference.get()));

		EXPECT_EQ(root.valid_size(), 2);
		EXPECT_EQ(root.size(), 2);
	}

	TYPED_TEST_P(GraphNaryTreeNodeTest, SelfExtractionAndInvalidation)
	{
		typename TypeParam::NodeType root{};
		std::vector < std::reference_wrapper < typename TypeParam::NodeType > > children;

		this->insert < false, false > (root, children);

		for (size_t i = 0; i < children.size(); i++)
		{
			auto extractedData = children[i].get().extract_and_invalidate_myself_on_parent();

			EXPECT_TRUE(root.has_child(i));
			EXPECT_FALSE(root.has_valid_child(i));
			EXPECT_EQ(root.child(i), std::nullopt);
			EXPECT_TRUE(extractedData.has_value());
			EXPECT_FALSE(extractedData.value().first.get()->has_parent());
			EXPECT_EQ(std::addressof(*(extractedData.value().first.get())), std::addressof(children[i].get()));
			EXPECT_EQ(extractedData.value().second, i);
		}

		EXPECT_EQ(root.valid_size(), 0);
		EXPECT_EQ(root.size(), children.size());
	}

	TYPED_TEST_P(GraphNaryTreeNodeTest, SelfExtractionAndDeletion)
	{
		typename TypeParam::NodeType root{};
		std::vector < std::reference_wrapper < typename TypeParam::NodeType > > children;

		this->insert < false, false > (root, children);

		const size_t childrenCount = children.size();

		for (size_t i = 0; i < childrenCount; i++)
		{
			auto extractedData = children[i].get().extract_and_erase_myself_on_parent();

			EXPECT_EQ(root.valid_size(), childrenCount - i - 1);
			EXPECT_EQ(root.size(), childrenCount - i - 1);
			EXPECT_TRUE(extractedData.has_value());
			EXPECT_FALSE(extractedData.value().first.get()->has_parent());
			EXPECT_EQ(std::addressof(*(extractedData.value().first.get())), std::addressof(children[i].get()));
			EXPECT_EQ(extractedData.value().second, 0);
		}
	}

	TYPED_TEST_P(GraphNaryTreeNodeTest, Concepts)
	{
		EXPECT_TRUE(xablau::graph::concepts::node < typename TypeParam::NodeType >);
		EXPECT_FALSE(xablau::graph::concepts::binary_node < typename TypeParam::NodeType >);
		EXPECT_FALSE(xablau::graph::concepts::binary_node_parent < typename TypeParam::NodeType >);
		EXPECT_FALSE(xablau::graph::concepts::binary_node_with_parent < typename TypeParam::NodeType >);
		EXPECT_TRUE(xablau::graph::concepts::nary_node < typename TypeParam::NodeType >);
		EXPECT_TRUE(xablau::graph::concepts::nary_node_parent < typename TypeParam::NodeType >);
		EXPECT_TRUE(xablau::graph::concepts::nary_node_with_parent < typename TypeParam::NodeType >);
		EXPECT_FALSE(xablau::graph::concepts::tree < typename TypeParam::NodeType >);
		EXPECT_FALSE(xablau::graph::concepts::binary_tree < typename TypeParam::NodeType >);
		EXPECT_FALSE(xablau::graph::concepts::nary_tree < typename TypeParam::NodeType >);
		EXPECT_FALSE(xablau::graph::concepts::edge < typename TypeParam::NodeType >);
		EXPECT_FALSE(xablau::graph::concepts::graph < typename TypeParam::NodeType >);
		EXPECT_FALSE(xablau::graph::concepts::digraph < typename TypeParam::NodeType >);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		GraphNaryTreeNodeTest,
		InsertionMoveValue,
		InsertionCopyValue,
		InsertionMoveNode,
		InsertionCopyNode,
		Invalidation,
		Deletion,
		Sibling,
		ExtractionAndInvalidation,
		ExtractionAndDeletion,
		SelfExtractionAndInvalidation,
		SelfExtractionAndDeletion,
		Concepts);

	using GraphNaryTreeNodeTestDataTypes = std::tuple < size_t, GraphNaryTreeNodeCustomType >;

	using GraphNaryTreeNodeTestContainerTypes =
		std::tuple <
			xablau::graph::nary_tree_node_container < xablau::graph::nary_tree_node_container_value::deque >,
			xablau::graph::nary_tree_node_container < xablau::graph::nary_tree_node_container_value::list >,
			xablau::graph::nary_tree_node_container < xablau::graph::nary_tree_node_container_value::vector > >;

	using GraphNaryTreeNodeTestGroup =
		xablau::testing::test_bundler <
			GraphNaryTreeNodeTestDataTypes,
			GraphNaryTreeNodeTestContainerTypes > ::template test_group < GraphNaryTreeNodeTypeDefinitions >;

	using GraphNaryTreeNodeTypes = xablau::testing::tuple_transformation < ::testing::Types, GraphNaryTreeNodeTestGroup > ::types;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		Xablau,
		GraphNaryTreeNodeTest,
		GraphNaryTreeNodeTypes);
}
