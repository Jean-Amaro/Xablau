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

#include <gtest/gtest.h>

import std;

import xablau.graph;
import xablau.testing;

namespace xablau::unit_testing
{
	template < typename Type >
	struct GraphBinaryTreeNodeTypeDefinitions
	{
		using value_type = Type;

		using NodeType = xablau::graph::binary_tree_node < Type >;
	};

	class GraphBinaryTreeNodeCustomType
	{
	private:
		std::vector < size_t > data;

	public:
		explicit operator size_t() const { return this->data.back(); }

		auto operator<=>(const GraphBinaryTreeNodeCustomType &) const = default;

		GraphBinaryTreeNodeCustomType &operator=(const GraphBinaryTreeNodeCustomType &) = default;

		GraphBinaryTreeNodeCustomType &operator=(GraphBinaryTreeNodeCustomType &&) noexcept = default;

		GraphBinaryTreeNodeCustomType()
		{
			this->data.push_back(0);
		}

		GraphBinaryTreeNodeCustomType(const size_t data)
		{
			this->data.push_back(data);
		}

		GraphBinaryTreeNodeCustomType(const GraphBinaryTreeNodeCustomType &) = default;

		GraphBinaryTreeNodeCustomType(GraphBinaryTreeNodeCustomType &&) noexcept = default;
	};

	template < typename Types >
	class GraphBinaryTreeNodeTest : public ::testing::Test
	{
	public:
		constexpr void validateEmptyRoot(auto &root) const
		{
			EXPECT_TRUE(root.empty());
			EXPECT_FALSE(root.has_parent());
			EXPECT_EQ(root.parent(), std::nullopt);
			EXPECT_EQ(root.parent_and_my_position(), std::nullopt);
			EXPECT_FALSE(root.has_left_child());
			EXPECT_EQ(root.left_child(), std::nullopt);
			EXPECT_FALSE(root.has_right_child());
			EXPECT_EQ(root.right_child(), std::nullopt);
			EXPECT_EQ(root.valid_size(), size_t{});
			EXPECT_EQ(root.size(), size_t{2});
		}

		constexpr void validateChildren(auto &root, auto &leftChild, auto &rightChild) const
		{
			EXPECT_FALSE(root.has_parent());
			EXPECT_EQ(root.parent_and_my_position(), std::nullopt);

			EXPECT_TRUE(root.has_left_child());
			EXPECT_EQ(root.left_child().value().get(), leftChild.get());
			EXPECT_TRUE(root.has_right_child());
			EXPECT_EQ(root.right_child().value().get(), rightChild.get());
			EXPECT_EQ(root.valid_size(), size_t{2});
			EXPECT_EQ(root.size(), size_t{2});

			EXPECT_TRUE(leftChild.get().has_parent());
			EXPECT_EQ(leftChild.get().parent().value().get(), root);
			EXPECT_EQ(leftChild.get().parent_and_my_position().value().first.get(), root);
			EXPECT_EQ(leftChild.get().parent_and_my_position().value().second, xablau::graph::binary_node_side::left);
			EXPECT_EQ(leftChild.get().valid_size(), size_t{});
			EXPECT_EQ(leftChild.get().size(), size_t{2});

			EXPECT_TRUE(rightChild.get().has_parent());
			EXPECT_EQ(rightChild.get().parent().value().get(), root);
			EXPECT_EQ(rightChild.get().parent_and_my_position().value().first.get(), root);
			EXPECT_EQ(rightChild.get().parent_and_my_position().value().second, xablau::graph::binary_node_side::right);
			EXPECT_EQ(rightChild.get().valid_size(), size_t{0});
			EXPECT_EQ(rightChild.get().size(), size_t{2});
		}
	};

	TYPED_TEST_SUITE_P(GraphBinaryTreeNodeTest);

	TYPED_TEST_P(GraphBinaryTreeNodeTest, Insertion)
	{
		typename TypeParam::NodeType root{};

		this->validateEmptyRoot(root);

		// Insertion of const value_type &.
		const typename TypeParam::value_type leftChildValue{1};
		const typename TypeParam::value_type rightChildValue{2};

		std::reference_wrapper < typename TypeParam::NodeType > mutableLeftChild =
			root.insert_left_child(leftChildValue);

		std::reference_wrapper < typename TypeParam::NodeType > mutableRightChild =
			root.insert_right_child(rightChildValue);

		this->validateChildren(root, mutableLeftChild, mutableRightChild);

		// Insertion of value_type &&.
		mutableLeftChild = root.insert_left_child(typename TypeParam::value_type{3});
		mutableRightChild = root.insert_right_child(typename TypeParam::value_type{4});

		this->validateChildren(root, mutableLeftChild, mutableRightChild);

		// Insertion of const binary_tree_node &.
		const typename TypeParam::NodeType leftChildNode{typename TypeParam::value_type{1}};
		const typename TypeParam::NodeType rightChildNode{typename TypeParam::value_type{2}};

		std::reference_wrapper < const typename TypeParam::NodeType > constLeftChild = root.insert_left_child(leftChildNode);
		std::reference_wrapper < const typename TypeParam::NodeType > constRightChild = root.insert_right_child(rightChildNode);

		this->validateChildren(root, constLeftChild, constRightChild);

		// Insertion of binary_tree_node &&.
		constLeftChild = root.insert_left_child(typename TypeParam::NodeType{typename TypeParam::value_type{3}});
		constRightChild = root.insert_right_child(typename TypeParam::NodeType{typename TypeParam::value_type{4}});

		this->validateChildren(root, constLeftChild, constRightChild);
	}

	TYPED_TEST_P(GraphBinaryTreeNodeTest, Deletion)
	{
		typename TypeParam::NodeType root{};

		this->validateEmptyRoot(root);

		std::reference_wrapper < typename TypeParam::NodeType > insertedLeftChild =
			root.insert_left_child(typename TypeParam::value_type{1});
		std::reference_wrapper < typename TypeParam::NodeType > insertedRightChild =
			root.insert_right_child(typename TypeParam::value_type{2});

		this->validateChildren(root, insertedLeftChild, insertedRightChild);

		root.clear();

		this->validateEmptyRoot(root);
	}

	TYPED_TEST_P(GraphBinaryTreeNodeTest, ExtractionFromRoot)
	{
		typename TypeParam::NodeType root{};

		this->validateEmptyRoot(root);

		std::reference_wrapper < typename TypeParam::NodeType > insertedLeftChild =
			root.insert_left_child(typename TypeParam::value_type{1});
		std::reference_wrapper < typename TypeParam::NodeType > insertedRightChild =
			root.insert_right_child(typename TypeParam::value_type{2});

		this->validateChildren(root, insertedLeftChild, insertedRightChild);

		EXPECT_EQ(root.extract_myself(), std::nullopt);

		auto extractedLeftChild = root.extract_left_child();
		auto extractedRightChild = root.extract_right_child();

		this->validateEmptyRoot(root);

		EXPECT_EQ(std::addressof(insertedLeftChild.get()), extractedLeftChild.get());
		EXPECT_EQ(std::addressof(insertedRightChild.get()), extractedRightChild.get());
	}

	TYPED_TEST_P(GraphBinaryTreeNodeTest, ExtractionFromChildren)
	{
		typename TypeParam::NodeType root{};

		this->validateEmptyRoot(root);

		std::reference_wrapper < typename TypeParam::NodeType > insertedLeftChild =
			root.insert_left_child(typename TypeParam::value_type{1});
		std::reference_wrapper < typename TypeParam::NodeType > insertedRightChild =
			root.insert_right_child(typename TypeParam::value_type{2});

		this->validateChildren(root, insertedLeftChild, insertedRightChild);

		auto extractedLeftChild = insertedLeftChild.get().extract_myself();
		auto extractedRightChild = insertedRightChild.get().extract_myself();

		EXPECT_NE(extractedLeftChild, std::nullopt);
		EXPECT_NE(extractedRightChild, std::nullopt);

		this->validateEmptyRoot(root);
		this->validateEmptyRoot(*(extractedLeftChild.value().first.get()));
		this->validateEmptyRoot(*(extractedRightChild.value().first.get()));

		EXPECT_EQ(extractedLeftChild.value().second, xablau::graph::binary_node_side::left);
		EXPECT_EQ(extractedRightChild.value().second, xablau::graph::binary_node_side::right);

		EXPECT_EQ(std::addressof(insertedLeftChild.get()), extractedLeftChild.value().first.get());
		EXPECT_EQ(std::addressof(insertedRightChild.get()), extractedRightChild.value().first.get());
	}

	TYPED_TEST_P(GraphBinaryTreeNodeTest, Concepts)
	{
		EXPECT_TRUE(xablau::graph::concepts::node < typename TypeParam::NodeType >);
		EXPECT_TRUE(xablau::graph::concepts::binary_node < typename TypeParam::NodeType >);
		EXPECT_TRUE(xablau::graph::concepts::binary_node_parent < typename TypeParam::NodeType >);
		EXPECT_TRUE(xablau::graph::concepts::binary_node_with_parent < typename TypeParam::NodeType >);
		EXPECT_FALSE(xablau::graph::concepts::nary_node < typename TypeParam::NodeType >);
		EXPECT_FALSE(xablau::graph::concepts::nary_node_parent < typename TypeParam::NodeType >);
		EXPECT_FALSE(xablau::graph::concepts::nary_node_with_parent < typename TypeParam::NodeType >);
		EXPECT_FALSE(xablau::graph::concepts::tree < typename TypeParam::NodeType >);
		EXPECT_FALSE(xablau::graph::concepts::binary_tree < typename TypeParam::NodeType >);
		EXPECT_FALSE(xablau::graph::concepts::nary_tree < typename TypeParam::NodeType >);
		EXPECT_FALSE(xablau::graph::concepts::edge < typename TypeParam::NodeType >);
		EXPECT_FALSE(xablau::graph::concepts::graph < typename TypeParam::NodeType >);
		EXPECT_FALSE(xablau::graph::concepts::digraph < typename TypeParam::NodeType >);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		GraphBinaryTreeNodeTest,
		Insertion,
		Deletion,
		ExtractionFromRoot,
		ExtractionFromChildren,
		Concepts);

	using GraphBinaryTreeNodeTestDataTypes = std::tuple < size_t, GraphBinaryTreeNodeCustomType >;

	using GraphBinaryTreeNodeTestGroup =
		xablau::testing::test_bundler < GraphBinaryTreeNodeTestDataTypes > ::template test_group < GraphBinaryTreeNodeTypeDefinitions >;

	using GraphBinaryTreeNodeTypes = xablau::testing::tuple_transformation < ::testing::Types, GraphBinaryTreeNodeTestGroup > ::types;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		Xablau,
		GraphBinaryTreeNodeTest,
		GraphBinaryTreeNodeTypes);
}
