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

import <array>;
import <concepts>;

import xablau.graph;
import xablau.testing;

namespace xablau::unit_testing
{
	template < typename Type >
	struct GraphBinaryTreeTypeDefinitions
	{
		using value_type = typename Type::value_type;

		using NodeType = Type;
	};

	class GraphBinaryTreeCustomType
	{
	private:
		std::vector < size_t > data;

	public:
		explicit operator size_t() const { return this->data.back(); }

		auto operator<=>(const GraphBinaryTreeCustomType &) const = default;

		GraphBinaryTreeCustomType &operator=(const GraphBinaryTreeCustomType &) = default;

		GraphBinaryTreeCustomType &operator=(GraphBinaryTreeCustomType &&) noexcept = default;

		GraphBinaryTreeCustomType()
		{
			this->data.push_back(0);
		}

		GraphBinaryTreeCustomType(const size_t data)
		{
			this->data.push_back(data);
		}

		GraphBinaryTreeCustomType(const GraphBinaryTreeCustomType &) = default;

		GraphBinaryTreeCustomType(GraphBinaryTreeCustomType &&) noexcept = default;
	};

	template < typename Types >
	class GraphBinaryTreeTest : public ::testing::Test
	{
	public:
		void fullyBalancedTree(auto &tree)
		{
			const auto root = tree.insert_root(size_t{4});
			const auto l = tree.insert_left_child(root, size_t{2});
			const auto r = tree.insert_right_child(root, size_t{6});
			tree.insert_left_child(l, size_t{1});
			tree.insert_right_child(l, size_t{3});
			tree.insert_left_child(r, size_t{5});
			tree.insert_right_child(r, size_t{7});

			EXPECT_EQ(tree.size(), 7);
			EXPECT_FALSE(tree.empty());
			EXPECT_EQ(tree.levels(), 3);

			for (size_t i = 1; i < 8; i++)
			{
				EXPECT_TRUE(tree.contains(i));
				EXPECT_EQ(tree.count(i), 1);
			}

			const auto nodesPerLevel = tree.nodes_per_level();

			EXPECT_EQ(nodesPerLevel[0], 1);
			EXPECT_EQ(nodesPerLevel[1], 2);
			EXPECT_EQ(nodesPerLevel[2], 4);

			EXPECT_TRUE(tree.binary());
		}

		void unbalancedTree(auto &tree)
		{
			// Level 1
			const auto root = tree.insert_root(size_t{11});

			// Level 2
			const auto l = tree.insert_left_child(root, size_t{6});

			// Level 3
			const auto ll = tree.insert_left_child(l, size_t{1});
			const auto lr = tree.insert_right_child(l, size_t{8});

			// Level 4
			const auto llr = tree.insert_right_child(ll, size_t{2});
			const auto lrl = tree.insert_left_child(lr, size_t{7});
			const auto lrr = tree.insert_right_child(lr, size_t{9});

			// Level 5
			const auto llrr = tree.insert_right_child(llr, size_t{5});
			const auto lrrr = tree.insert_right_child(lrr, size_t{10});

			// Level 6
			const auto llrrl = tree.insert_left_child(llrr, size_t{4});

			// Level 7
			const auto llrrll = tree.insert_left_child(llrrl, size_t{3});

			EXPECT_EQ(tree.size(), 11);
			EXPECT_FALSE(tree.empty());
			EXPECT_EQ(tree.levels(), 7);

			for (size_t i = 1; i < 12; i++)
			{
				EXPECT_TRUE(tree.contains(i));
				EXPECT_EQ(tree.count(i), 1);
			}

			for (size_t i = 12; i < 20; i++)
			{
				EXPECT_FALSE(tree.contains(i));
				EXPECT_EQ(tree.count(i), 0);
			}

			const auto nodesPerLevel = tree.nodes_per_level();

			EXPECT_EQ(nodesPerLevel[0], 1);
			EXPECT_EQ(nodesPerLevel[1], 1);
			EXPECT_EQ(nodesPerLevel[2], 2);
			EXPECT_EQ(nodesPerLevel[3], 3);
			EXPECT_EQ(nodesPerLevel[4], 2);
			EXPECT_EQ(nodesPerLevel[5], 1);
			EXPECT_EQ(nodesPerLevel[6], 1);

			EXPECT_TRUE(tree.binary());
		}
	};

	TYPED_TEST_SUITE_P(GraphBinaryTreeTest);

	TYPED_TEST_P(GraphBinaryTreeTest, Insertion)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->fullyBalancedTree(tree);
		this->unbalancedTree(tree);
	}

	TYPED_TEST_P(GraphBinaryTreeTest, ForwardPreOrderIncrementIteratorBalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;
		constexpr auto preOrderIndices = std::to_array({ 4, 2, 1, 3, 6, 5, 7 });

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->fullyBalancedTree(tree);

		auto orderIterator = preOrderIndices.begin();

		for (auto iterator = tree.pre_order_begin(); iterator != tree.pre_order_end(); ++iterator)
		{
			EXPECT_EQ(*iterator, *orderIterator);

			++orderIterator;
		}

		auto orderConstIterator = preOrderIndices.cbegin();

		for (auto iterator = tree.const_pre_order_begin(); iterator != tree.const_pre_order_end(); ++iterator)
		{
			EXPECT_EQ(*iterator, *orderConstIterator);

			++orderConstIterator;
		}
	}

	TYPED_TEST_P(GraphBinaryTreeTest, ForwardInOrderIncrementIteratorBalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;
		constexpr auto inOrderIndices = std::to_array({ 1, 2, 3, 4, 5, 6, 7 });

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->fullyBalancedTree(tree);

		auto orderIterator = inOrderIndices.begin();

		for (auto iterator = tree.in_order_begin(); iterator != tree.in_order_end(); ++iterator)
		{
			EXPECT_EQ(*iterator, *orderIterator);

			++orderIterator;
		}

		auto orderConstIterator = inOrderIndices.cbegin();

		for (auto iterator = tree.const_in_order_begin(); iterator != tree.const_in_order_end(); ++iterator)
		{
			EXPECT_EQ(*iterator, *orderConstIterator);

			++orderConstIterator;
		}
	}

	TYPED_TEST_P(GraphBinaryTreeTest, ForwardPostOrderIncrementIteratorBalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;
		constexpr auto postOrderIndices = std::to_array({ 1, 3, 2, 5, 7, 6, 4 });

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->fullyBalancedTree(tree);

		auto orderIterator = postOrderIndices.begin();

		for (auto iterator = tree.post_order_begin(); iterator != tree.post_order_end(); ++iterator)
		{
			EXPECT_EQ(*iterator, *orderIterator);

			++orderIterator;
		}

		auto orderConstIterator = postOrderIndices.cbegin();

		for (auto iterator = tree.const_post_order_begin(); iterator != tree.const_post_order_end(); ++iterator)
		{
			EXPECT_EQ(*iterator, *orderConstIterator);

			++orderConstIterator;
		}
	}

	TYPED_TEST_P(GraphBinaryTreeTest, ForwardPreOrderIncrementIteratorUnbalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;
		constexpr auto preOrderIndices = std::to_array({ 11, 6, 1, 2, 5, 4, 3, 8, 7, 9, 10 });

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->unbalancedTree(tree);

		auto orderIterator = preOrderIndices.begin();

		for (auto iterator = tree.pre_order_begin(); iterator != tree.pre_order_end(); ++iterator)
		{
			EXPECT_EQ(*iterator, *orderIterator);

			++orderIterator;
		}

		auto orderConstIterator = preOrderIndices.cbegin();

		for (auto iterator = tree.const_pre_order_begin(); iterator != tree.const_pre_order_end(); ++iterator)
		{
			EXPECT_EQ(*iterator, *orderConstIterator);

			++orderConstIterator;
		}
	}

	TYPED_TEST_P(GraphBinaryTreeTest, ForwardInOrderIncrementIteratorUnbalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;
		constexpr auto inOrderIndices = std::to_array({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 });

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->unbalancedTree(tree);

		auto orderIterator = inOrderIndices.begin();

		for (auto iterator = tree.in_order_begin(); iterator != tree.in_order_end(); ++iterator)
		{
			EXPECT_EQ(*iterator, *orderIterator);

			++orderIterator;
		}

		auto orderConstIterator = inOrderIndices.cbegin();

		for (auto iterator = tree.const_in_order_begin(); iterator != tree.const_in_order_end(); ++iterator)
		{
			EXPECT_EQ(*iterator, *orderConstIterator);

			++orderConstIterator;
		}
	}

	TYPED_TEST_P(GraphBinaryTreeTest, ForwardPostOrderIncrementIteratorUnbalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;
		constexpr auto postOrderIndices = std::to_array({ 3, 4, 5, 2, 1, 7, 10, 9, 8, 6, 11 });

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->unbalancedTree(tree);

		auto orderIterator = postOrderIndices.begin();

		for (auto iterator = tree.post_order_begin(); iterator != tree.post_order_end(); ++iterator)
		{
			EXPECT_EQ(*iterator, *orderIterator);

			++orderIterator;
		}

		auto orderConstIterator = postOrderIndices.cbegin();

		for (auto iterator = tree.const_post_order_begin(); iterator != tree.const_post_order_end(); ++iterator)
		{
			EXPECT_EQ(*iterator, *orderConstIterator);

			++orderConstIterator;
		}
	}

	TYPED_TEST_P(GraphBinaryTreeTest, ReversePreOrderIncrementIteratorBalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;
		constexpr auto preOrderIndices = std::to_array({ 4, 2, 1, 3, 6, 5, 7 });

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->fullyBalancedTree(tree);

		auto orderIterator = preOrderIndices.rbegin();

		for (auto iterator = tree.reverse_pre_order_begin(); iterator != tree.reverse_pre_order_end(); ++iterator)
		{
			EXPECT_EQ(*iterator, *orderIterator);

			++orderIterator;
		}

		auto orderConstIterator = preOrderIndices.crbegin();

		for (auto iterator = tree.const_reverse_pre_order_begin(); iterator != tree.const_reverse_pre_order_end(); ++iterator)
		{
			EXPECT_EQ(*iterator, *orderConstIterator);

			++orderConstIterator;
		}
	}

	TYPED_TEST_P(GraphBinaryTreeTest, ReverseInOrderIncrementIteratorBalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;
		constexpr auto inOrderIndices = std::to_array({ 1, 2, 3, 4, 5, 6, 7 });

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->fullyBalancedTree(tree);

		auto orderIterator = inOrderIndices.rbegin();

		for (auto iterator = tree.reverse_in_order_begin(); iterator != tree.reverse_in_order_end(); ++iterator)
		{
			EXPECT_EQ(*iterator, *orderIterator);

			++orderIterator;
		}

		auto orderConstIterator = inOrderIndices.crbegin();

		for (auto iterator = tree.const_reverse_in_order_begin(); iterator != tree.const_reverse_in_order_end(); ++iterator)
		{
			EXPECT_EQ(*iterator, *orderConstIterator);

			++orderConstIterator;
		}
	}

	TYPED_TEST_P(GraphBinaryTreeTest, ReversePostOrderIncrementIteratorBalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;
		constexpr auto postOrderIndices = std::to_array({ 1, 3, 2, 5, 7, 6, 4 });

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->fullyBalancedTree(tree);

		auto orderIterator = postOrderIndices.rbegin();

		for (auto iterator = tree.reverse_post_order_begin(); iterator != tree.reverse_post_order_end(); ++iterator)
		{
			EXPECT_EQ(*iterator, *orderIterator);

			++orderIterator;
		}

		auto orderConstIterator = postOrderIndices.crbegin();

		for (auto iterator = tree.const_reverse_post_order_begin(); iterator != tree.const_reverse_post_order_end(); ++iterator)
		{
			EXPECT_EQ(*iterator, *orderConstIterator);

			++orderConstIterator;
		}
	}

	TYPED_TEST_P(GraphBinaryTreeTest, ReversePreOrderIncrementIteratorUnbalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;
		constexpr auto preOrderIndices = std::to_array({ 11, 6, 1, 2, 5, 4, 3, 8, 7, 9, 10 });

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->unbalancedTree(tree);

		auto orderIterator = preOrderIndices.rbegin();

		for (auto iterator = tree.reverse_pre_order_begin(); iterator != tree.reverse_pre_order_end(); ++iterator)
		{
			EXPECT_EQ(*iterator, *orderIterator);

			++orderIterator;
		}

		auto orderConstIterator = preOrderIndices.crbegin();

		for (auto iterator = tree.const_reverse_pre_order_begin(); iterator != tree.const_reverse_pre_order_end(); ++iterator)
		{
			EXPECT_EQ(*iterator, *orderConstIterator);

			++orderConstIterator;
		}
	}

	TYPED_TEST_P(GraphBinaryTreeTest, ReverseInOrderIncrementIteratorUnbalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;
		constexpr auto inOrderIndices = std::to_array({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 });

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->unbalancedTree(tree);

		auto orderIterator = inOrderIndices.rbegin();

		for (auto iterator = tree.reverse_in_order_begin(); iterator != tree.reverse_in_order_end(); ++iterator)
		{
			EXPECT_EQ(*iterator, *orderIterator);

			++orderIterator;
		}

		auto orderConstIterator = inOrderIndices.crbegin();

		for (auto iterator = tree.const_reverse_in_order_begin(); iterator != tree.const_reverse_in_order_end(); ++iterator)
		{
			EXPECT_EQ(*iterator, *orderConstIterator);

			++orderConstIterator;
		}
	}

	TYPED_TEST_P(GraphBinaryTreeTest, ReversePostOrderIncrementIteratorUnbalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;
		constexpr auto postOrderIndices = std::to_array({ 3, 4, 5, 2, 1, 7, 10, 9, 8, 6, 11 });

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->unbalancedTree(tree);

		auto orderIterator = postOrderIndices.rbegin();

		for (auto iterator = tree.reverse_post_order_begin(); iterator != tree.reverse_post_order_end(); ++iterator)
		{
			EXPECT_EQ(*iterator, *orderIterator);

			++orderIterator;
		}

		auto orderConstIterator = postOrderIndices.crbegin();

		for (auto iterator = tree.const_reverse_post_order_begin(); iterator != tree.const_reverse_post_order_end(); ++iterator)
		{
			EXPECT_EQ(*iterator, *orderConstIterator);

			++orderConstIterator;
		}
	}

	TYPED_TEST_P(GraphBinaryTreeTest, ForwardPreOrderDecrementIteratorBalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;
		constexpr auto preOrderIndices = std::to_array({ 4, 2, 1, 3, 6, 5, 7 });

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->fullyBalancedTree(tree);

		auto orderIterator = preOrderIndices.rbegin();
		auto iterator = --tree.pre_order_end();

		while (true)
		{
			EXPECT_EQ(*iterator, *orderIterator);

			if (iterator == tree.pre_order_begin())
			{
				break;
			}

			--iterator;
			++orderIterator;
		}

		auto orderConstIterator = preOrderIndices.crbegin();
		auto constIterator = --tree.const_pre_order_end();

		while (true)
		{
			EXPECT_EQ(*constIterator, *orderConstIterator);

			if (constIterator == tree.const_pre_order_begin())
			{
				break;
			}

			--constIterator;
			++orderConstIterator;
		}
	}

	TYPED_TEST_P(GraphBinaryTreeTest, ForwardInOrderDecrementIteratorBalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;
		constexpr auto inOrderIndices = std::to_array({ 1, 2, 3, 4, 5, 6, 7 });

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->fullyBalancedTree(tree);

		auto orderIterator = inOrderIndices.rbegin();
		auto iterator = --tree.in_order_end();

		while (true)
		{
			EXPECT_EQ(*iterator, *orderIterator);

			if (iterator == tree.in_order_begin())
			{
				break;
			}

			--iterator;
			++orderIterator;
		}

		auto orderConstIterator = inOrderIndices.crbegin();
		auto constIterator = --tree.const_in_order_end();

		while (true)
		{
			EXPECT_EQ(*constIterator, *orderConstIterator);

			if (constIterator == tree.in_order_begin())
			{
				break;
			}

			--constIterator;
			++orderConstIterator;
		}
	}

	TYPED_TEST_P(GraphBinaryTreeTest, ForwardPostOrderDecrementIteratorBalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;
		constexpr auto postOrderIndices = std::to_array({ 1, 3, 2, 5, 7, 6, 4 });

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->fullyBalancedTree(tree);

		auto orderIterator = postOrderIndices.rbegin();
		auto iterator = --tree.post_order_end();

		while (true)
		{
			EXPECT_EQ(*iterator, *orderIterator);

			if (iterator == tree.post_order_begin())
			{
				break;
			}

			--iterator;
			++orderIterator;
		}

		auto orderConstIterator = postOrderIndices.crbegin();
		auto constIterator = --tree.const_post_order_end();

		while (true)
		{
			EXPECT_EQ(*constIterator, *orderConstIterator);

			if (constIterator == tree.post_order_begin())
			{
				break;
			}

			--constIterator;
			++orderConstIterator;
		}
	}

	TYPED_TEST_P(GraphBinaryTreeTest, ForwardPreOrderDecrementIteratorUnbalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;
		constexpr auto preOrderIndices = std::to_array({ 11, 6, 1, 2, 5, 4, 3, 8, 7, 9, 10 });

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->unbalancedTree(tree);

		auto orderIterator = preOrderIndices.rbegin();
		auto iterator = --tree.pre_order_end();

		while (true)
		{
			EXPECT_EQ(*iterator, *orderIterator);

			if (iterator == tree.pre_order_begin())
			{
				break;
			}

			--iterator;
			++orderIterator;
		}

		auto orderConstIterator = preOrderIndices.crbegin();
		auto constIterator = --tree.const_pre_order_end();

		while (true)
		{
			EXPECT_EQ(*constIterator, *orderConstIterator);

			if (constIterator == tree.const_pre_order_begin())
			{
				break;
			}

			--constIterator;
			++orderConstIterator;
		}
	}

	TYPED_TEST_P(GraphBinaryTreeTest, ForwardInOrderDecrementIteratorUnbalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;
		constexpr auto inOrderIndices = std::to_array({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 });

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->unbalancedTree(tree);

		auto orderIterator = inOrderIndices.rbegin();
		auto iterator = --tree.in_order_end();

		while (true)
		{
			EXPECT_EQ(*iterator, *orderIterator);

			if (iterator == tree.in_order_begin())
			{
				break;
			}

			--iterator;
			++orderIterator;
		}

		auto orderConstIterator = inOrderIndices.crbegin();
		auto constIterator = --tree.const_in_order_end();

		while (true)
		{
			EXPECT_EQ(*constIterator, *orderConstIterator);

			if (constIterator == tree.in_order_begin())
			{
				break;
			}

			--constIterator;
			++orderConstIterator;
		}
	}

	TYPED_TEST_P(GraphBinaryTreeTest, ForwardPostOrderDecrementIteratorUnbalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;
		constexpr auto postOrderIndices = std::to_array({ 3, 4, 5, 2, 1, 7, 10, 9, 8, 6, 11 });

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->unbalancedTree(tree);

		auto orderIterator = postOrderIndices.rbegin();
		auto iterator = --tree.post_order_end();

		while (true)
		{
			EXPECT_EQ(*iterator, *orderIterator);

			if (iterator == tree.post_order_begin())
			{
				break;
			}

			--iterator;
			++orderIterator;
		}

		auto orderConstIterator = postOrderIndices.crbegin();
		auto constIterator = --tree.const_post_order_end();

		while (true)
		{
			EXPECT_EQ(*constIterator, *orderConstIterator);

			if (constIterator == tree.post_order_begin())
			{
				break;
			}

			--constIterator;
			++orderConstIterator;
		}
	}

	TYPED_TEST_P(GraphBinaryTreeTest, ReversePreOrderDecrementIteratorBalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;
		constexpr auto preOrderIndices = std::to_array({ 4, 2, 1, 3, 6, 5, 7 });

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->fullyBalancedTree(tree);

		auto orderIterator = preOrderIndices.begin();
		auto iterator = --tree.reverse_pre_order_end();

		while (true)
		{
			EXPECT_EQ(*iterator, *orderIterator);

			if (iterator == tree.reverse_pre_order_begin())
			{
				break;
			}

			--iterator;
			++orderIterator;
		}

		auto orderConstIterator = preOrderIndices.cbegin();
		auto constIterator = --tree.const_reverse_pre_order_end();

		while (true)
		{
			EXPECT_EQ(*constIterator, *orderConstIterator);

			if (constIterator == tree.const_reverse_pre_order_begin())
			{
				break;
			}

			--constIterator;
			++orderConstIterator;
		}
	}

	TYPED_TEST_P(GraphBinaryTreeTest, ReverseInOrderDecrementIteratorBalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;
		constexpr auto inOrderIndices = std::to_array({ 1, 2, 3, 4, 5, 6, 7 });

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->fullyBalancedTree(tree);

		auto orderIterator = inOrderIndices.begin();
		auto iterator = --tree.reverse_in_order_end();

		while (true)
		{
			EXPECT_EQ(*iterator, *orderIterator);

			if (iterator == tree.reverse_in_order_begin())
			{
				break;
			}

			--iterator;
			++orderIterator;
		}

		auto orderConstIterator = inOrderIndices.cbegin();
		auto constIterator = --tree.const_reverse_in_order_end();

		while (true)
		{
			EXPECT_EQ(*constIterator, *orderConstIterator);

			if (constIterator == tree.const_reverse_in_order_begin())
			{
				break;
			}

			--constIterator;
			++orderConstIterator;
		}
	}

	TYPED_TEST_P(GraphBinaryTreeTest, ReversePostOrderDecrementIteratorBalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;
		constexpr auto postOrderIndices = std::to_array({ 1, 3, 2, 5, 7, 6, 4 });

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->fullyBalancedTree(tree);

		auto orderIterator = postOrderIndices.begin();
		auto iterator = --tree.reverse_post_order_end();

		while (true)
		{
			EXPECT_EQ(*iterator, *orderIterator);

			if (iterator == tree.reverse_post_order_begin())
			{
				break;
			}

			--iterator;
			++orderIterator;
		}

		auto orderConstIterator = postOrderIndices.cbegin();
		auto constIterator = --tree.const_reverse_post_order_end();

		while (true)
		{
			EXPECT_EQ(*constIterator, *orderConstIterator);

			if (constIterator == tree.const_reverse_post_order_begin())
			{
				break;
			}

			--constIterator;
			++orderConstIterator;
		}
	}

	TYPED_TEST_P(GraphBinaryTreeTest, ReversePreOrderDecrementIteratorUnbalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;
		constexpr auto preOrderIndices = std::to_array({ 11, 6, 1, 2, 5, 4, 3, 8, 7, 9, 10 });

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->unbalancedTree(tree);

		auto orderIterator = preOrderIndices.begin();
		auto iterator = --tree.reverse_pre_order_end();

		while (true)
		{
			EXPECT_EQ(*iterator, *orderIterator);

			if (iterator == tree.reverse_pre_order_begin())
			{
				break;
			}

			--iterator;
			++orderIterator;
		}

		auto orderConstIterator = preOrderIndices.cbegin();
		auto constIterator = --tree.const_reverse_pre_order_end();

		while (true)
		{
			EXPECT_EQ(*constIterator, *orderConstIterator);

			if (constIterator == tree.const_reverse_pre_order_begin())
			{
				break;
			}

			--constIterator;
			++orderConstIterator;
		}
	}

	TYPED_TEST_P(GraphBinaryTreeTest, ReverseInOrderDecrementIteratorUnbalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;
		constexpr auto inOrderIndices = std::to_array({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 });

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->unbalancedTree(tree);

		auto orderIterator = inOrderIndices.begin();
		auto iterator = --tree.reverse_in_order_end();

		while (true)
		{
			EXPECT_EQ(*iterator, *orderIterator);

			if (iterator == tree.reverse_in_order_begin())
			{
				break;
			}

			--iterator;
			++orderIterator;
		}

		auto orderConstIterator = inOrderIndices.cbegin();
		auto constIterator = --tree.const_reverse_in_order_end();

		while (true)
		{
			EXPECT_EQ(*constIterator, *orderConstIterator);

			if (constIterator == tree.const_reverse_in_order_begin())
			{
				break;
			}

			--constIterator;
			++orderConstIterator;
		}
	}

	TYPED_TEST_P(GraphBinaryTreeTest, ReversePostOrderDecrementIteratorUnbalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;
		constexpr auto postOrderIndices = std::to_array({ 3, 4, 5, 2, 1, 7, 10, 9, 8, 6, 11 });

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->unbalancedTree(tree);

		auto orderIterator = postOrderIndices.begin();
		auto iterator = --tree.reverse_post_order_end();

		while (true)
		{
			EXPECT_EQ(*iterator, *orderIterator);

			if (iterator == tree.reverse_post_order_begin())
			{
				break;
			}

			--iterator;
			++orderIterator;
		}

		auto orderConstIterator = postOrderIndices.cbegin();
		auto constIterator = --tree.const_reverse_post_order_end();

		while (true)
		{
			EXPECT_EQ(*constIterator, *orderConstIterator);

			if (constIterator == tree.const_reverse_post_order_begin())
			{
				break;
			}

			--constIterator;
			++orderConstIterator;
		}
	}

	TYPED_TEST_P(GraphBinaryTreeTest, NodesBalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;
		constexpr auto leafIndices = std::to_array({ 1, 3, 5, 7 });

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->fullyBalancedTree(tree);

		auto nodes = tree.nodes();
		auto leafNodes = tree.leaf_nodes();

		EXPECT_EQ(nodes.size(), tree.size());
		EXPECT_EQ(nodes.size(), 7);
		EXPECT_EQ(leafNodes.size(), leafIndices.size());

		for (size_t i = 0; i < nodes.size(); i++)
		{
			EXPECT_EQ(nodes[i].get().value, i + 1);
		}

		for (size_t i = 0; i < leafNodes.size(); i++)
		{
			EXPECT_EQ(leafNodes[i].get().value, leafIndices[i]);
		}
	}

	TYPED_TEST_P(GraphBinaryTreeTest, NodesUnbalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;
		constexpr auto leafIndices = std::to_array({ 3, 7, 10 });

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->unbalancedTree(tree);

		auto nodes = tree.nodes();
		auto leafNodes = tree.leaf_nodes();

		EXPECT_EQ(nodes.size(), tree.size());
		EXPECT_EQ(nodes.size(), 11);
		EXPECT_EQ(leafNodes.size(), leafIndices.size());

		for (size_t i = 0; i < nodes.size(); i++)
		{
			EXPECT_EQ(nodes[i].get().value, i + 1);
		}

		for (size_t i = 0; i < leafNodes.size(); i++)
		{
			EXPECT_EQ(leafNodes[i].get().value, leafIndices[i]);
		}
	}

	TYPED_TEST_P(GraphBinaryTreeTest, FindBalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->fullyBalancedTree(tree);

		for (size_t i = 0; i < tree.size(); i++)
		{
			EXPECT_NE(std::ranges::find(tree, i + 1), tree.end());
		}

		for (size_t i = tree.size(); i < 2 * tree.size(); i++)
		{
			EXPECT_EQ(std::ranges::find(tree, i + 1), tree.end());
		}
	}

	TYPED_TEST_P(GraphBinaryTreeTest, FindUnbalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->unbalancedTree(tree);

		for (size_t i = 0; i < tree.size(); i++)
		{
			EXPECT_NE(std::ranges::find(tree, i + 1), tree.end());
		}

		for (size_t i = tree.size(); i < 2 * tree.size(); i++)
		{
			EXPECT_EQ(std::ranges::find(tree, i + 1), tree.end());
		}
	}

	TYPED_TEST_P(GraphBinaryTreeTest, FindWithPathBalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->fullyBalancedTree(tree);

		for (size_t i = 0; i < 2 * tree.size(); i++)
		{
			EXPECT_EQ(std::ranges::find(tree, i + 1), tree.find_with_path(i + 1).first);
		}

		constexpr auto l = xablau::graph::binary_node_side::left;
		constexpr auto r = xablau::graph::binary_node_side::right;

		EXPECT_EQ(tree.find_with_path(1).second, std::vector < xablau::graph::binary_node_side > ({ l, l }));
		EXPECT_EQ(tree.find_with_path(2).second, std::vector < xablau::graph::binary_node_side > ({ l }));
		EXPECT_EQ(tree.find_with_path(3).second, std::vector < xablau::graph::binary_node_side > ({ l, r }));
		EXPECT_EQ(tree.find_with_path(4).second, std::vector < xablau::graph::binary_node_side > ({}));
		EXPECT_EQ(tree.find_with_path(5).second, std::vector < xablau::graph::binary_node_side > ({ r, l }));
		EXPECT_EQ(tree.find_with_path(6).second, std::vector < xablau::graph::binary_node_side > ({ r }));
		EXPECT_EQ(tree.find_with_path(7).second, std::vector < xablau::graph::binary_node_side > ({ r, r }));
	}

	TYPED_TEST_P(GraphBinaryTreeTest, FindWithPathUnbalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->unbalancedTree(tree);

		for (size_t i = 0; i < 2 * tree.size(); i++)
		{
			EXPECT_EQ(std::ranges::find(tree, i + 1), tree.find_with_path(i + 1).first);
		}

		constexpr auto l = xablau::graph::binary_node_side::left;
		constexpr auto r = xablau::graph::binary_node_side::right;

		EXPECT_EQ(tree.find_with_path(1).second, std::vector < xablau::graph::binary_node_side > ({ l, l }));
		EXPECT_EQ(tree.find_with_path(2).second, std::vector < xablau::graph::binary_node_side > ({ l, l, r }));
		EXPECT_EQ(tree.find_with_path(3).second, std::vector < xablau::graph::binary_node_side > ({ l, l, r, r, l, l }));
		EXPECT_EQ(tree.find_with_path(4).second, std::vector < xablau::graph::binary_node_side > ({ l, l, r, r, l }));
		EXPECT_EQ(tree.find_with_path(5).second, std::vector < xablau::graph::binary_node_side > ({ l, l, r, r }));
		EXPECT_EQ(tree.find_with_path(6).second, std::vector < xablau::graph::binary_node_side > ({ l }));
		EXPECT_EQ(tree.find_with_path(7).second, std::vector < xablau::graph::binary_node_side > ({ l, r, l }));
		EXPECT_EQ(tree.find_with_path(8).second, std::vector < xablau::graph::binary_node_side > ({ l, r }));
		EXPECT_EQ(tree.find_with_path(9).second, std::vector < xablau::graph::binary_node_side > ({ l, r, r }));
		EXPECT_EQ(tree.find_with_path(10).second, std::vector < xablau::graph::binary_node_side > ({ l, r, r, r }));
		EXPECT_EQ(tree.find_with_path(11).second, std::vector < xablau::graph::binary_node_side > ({}));
	}

	TYPED_TEST_P(GraphBinaryTreeTest, EraseIndividualElementBalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->fullyBalancedTree(tree);

		EXPECT_EQ(*tree.erase(tree.find(2)), 4);
		EXPECT_EQ(tree.size(), 4);

		EXPECT_EQ(*(--tree.erase(tree.find(6))), 4);
		EXPECT_EQ(tree.size(), 1);

		EXPECT_EQ(tree.erase(tree.find(4)), tree.end());
		EXPECT_EQ(tree.size(), 0);
	}

	TYPED_TEST_P(GraphBinaryTreeTest, EraseIndividualElementUnbalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->unbalancedTree(tree);

		EXPECT_EQ(*tree.erase(tree.find(10)), 11);
		EXPECT_EQ(tree.size(), 10);

		EXPECT_EQ(*tree.erase(tree.find(3)), 4);
		EXPECT_EQ(tree.size(), 9);

		EXPECT_EQ(*tree.erase(tree.find(5)), 6);
		EXPECT_EQ(tree.size(), 7);

		EXPECT_EQ(*tree.erase(tree.find(6)), 11);
		EXPECT_EQ(tree.size(), 1);

		EXPECT_EQ(tree.erase(tree.find(11)), tree.end());
		EXPECT_EQ(tree.size(), 0);
	}

	TYPED_TEST_P(GraphBinaryTreeTest, EraseRangeBalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->fullyBalancedTree(tree);

		EXPECT_EQ(*tree.erase(tree.find(1), tree.find(3)), 4);
		EXPECT_EQ(tree.size(), 4);

		EXPECT_EQ(tree.erase(tree.find(7), tree.end()), tree.end());
		EXPECT_EQ(tree.size(), 3);

		EXPECT_EQ(tree.erase(tree.begin(), tree.end()), tree.end());
		EXPECT_EQ(tree.size(), 0);
	}

	TYPED_TEST_P(GraphBinaryTreeTest, EraseRangeUnbalanced)
	{
		xablau::graph::binary_tree < typename TypeParam::NodeType > tree;

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->unbalancedTree(tree);

		EXPECT_EQ(*tree.erase(tree.find(3), tree.find(5)), 5);
		EXPECT_EQ(tree.size(), 9);

		EXPECT_EQ(*tree.erase(tree.find(1), tree.find(2)), 6);
		EXPECT_EQ(tree.size(), 6);

		EXPECT_EQ(*tree.erase(tree.find(7), tree.find(9)), 11);
		EXPECT_EQ(tree.size(), 2);

		EXPECT_EQ(tree.erase(tree.begin(), tree.end()), tree.end());
		EXPECT_EQ(tree.size(), 0);
	}

	TYPED_TEST_P(GraphBinaryTreeTest, Concepts)
	{
		using tree_type = typename xablau::graph::binary_tree < typename TypeParam::NodeType >;

		EXPECT_FALSE(xablau::graph::concepts::node < tree_type >);
		EXPECT_FALSE(xablau::graph::concepts::binary_node < tree_type >);
		EXPECT_FALSE(xablau::graph::concepts::binary_node_parent < tree_type >);
		EXPECT_FALSE(xablau::graph::concepts::binary_node_with_parent < tree_type >);
		EXPECT_FALSE(xablau::graph::concepts::nary_node < tree_type >);
		EXPECT_FALSE(xablau::graph::concepts::nary_node_parent < tree_type >);
		EXPECT_FALSE(xablau::graph::concepts::nary_node_with_parent < tree_type >);
		EXPECT_TRUE(xablau::graph::concepts::tree < tree_type >);
		EXPECT_TRUE(xablau::graph::concepts::binary_tree < tree_type >);
		EXPECT_FALSE(xablau::graph::concepts::nary_tree < tree_type >);
		EXPECT_FALSE(xablau::graph::concepts::edge < tree_type >);
		EXPECT_FALSE(xablau::graph::concepts::graph < tree_type >);
		EXPECT_FALSE(xablau::graph::concepts::digraph < tree_type >);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		GraphBinaryTreeTest,
		Insertion,
		ForwardPreOrderIncrementIteratorBalanced,
		ForwardInOrderIncrementIteratorBalanced,
		ForwardPostOrderIncrementIteratorBalanced,
		ForwardPreOrderIncrementIteratorUnbalanced,
		ForwardInOrderIncrementIteratorUnbalanced,
		ForwardPostOrderIncrementIteratorUnbalanced,
		ReversePreOrderIncrementIteratorBalanced,
		ReverseInOrderIncrementIteratorBalanced,
		ReversePostOrderIncrementIteratorBalanced,
		ReversePreOrderIncrementIteratorUnbalanced,
		ReverseInOrderIncrementIteratorUnbalanced,
		ReversePostOrderIncrementIteratorUnbalanced,
		ForwardPreOrderDecrementIteratorBalanced,
		ForwardInOrderDecrementIteratorBalanced,
		ForwardPostOrderDecrementIteratorBalanced,
		ForwardPreOrderDecrementIteratorUnbalanced,
		ForwardInOrderDecrementIteratorUnbalanced,
		ForwardPostOrderDecrementIteratorUnbalanced,
		ReversePreOrderDecrementIteratorBalanced,
		ReverseInOrderDecrementIteratorBalanced,
		ReversePostOrderDecrementIteratorBalanced,
		ReversePreOrderDecrementIteratorUnbalanced,
		ReverseInOrderDecrementIteratorUnbalanced,
		ReversePostOrderDecrementIteratorUnbalanced,
		NodesBalanced,
		NodesUnbalanced,
		FindBalanced,
		FindUnbalanced,
		FindWithPathBalanced,
		FindWithPathUnbalanced,
		EraseIndividualElementBalanced,
		EraseIndividualElementUnbalanced,
		EraseRangeBalanced,
		EraseRangeUnbalanced,
		Concepts);

	using GraphBinaryTreeTestDataTypes =
		std::tuple <
			xablau::graph::binary_tree_node < size_t >,
			xablau::graph::binary_tree_node < GraphBinaryTreeCustomType > >;

	using GraphBinaryTreeTestGroup =
		xablau::testing::test_bundler < GraphBinaryTreeTestDataTypes > ::template test_group < GraphBinaryTreeTypeDefinitions >;

	using GraphBinaryTreeTypes = xablau::testing::tuple_transformation < ::testing::Types, GraphBinaryTreeTestGroup > ::types;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		Xablau,
		GraphBinaryTreeTest,
		GraphBinaryTreeTypes);
}
