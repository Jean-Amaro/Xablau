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
import <vector>;

import xablau.graph;
import xablau.testing;

namespace xablau::unit_testing
{
	template < typename Type >
	struct GraphNaryTreeTypeDefinitions
	{
		using value_type = typename Type::value_type;

		using NodeType = Type;
	};

	class GraphNaryTreeCustomType
	{
	private:
		std::vector < size_t > data;

	public:
		explicit operator size_t() const { return this->data.back(); }

		auto operator<=>(const GraphNaryTreeCustomType &) const = default;

		GraphNaryTreeCustomType &operator=(const GraphNaryTreeCustomType &) = default;

		GraphNaryTreeCustomType &operator=(GraphNaryTreeCustomType &&) noexcept = default;

		GraphNaryTreeCustomType()
		{
			this->data.push_back(0);
		}

		GraphNaryTreeCustomType(const size_t data)
		{
			this->data.push_back(data);
		}

		GraphNaryTreeCustomType(const GraphNaryTreeCustomType &) = default;

		GraphNaryTreeCustomType(GraphNaryTreeCustomType &&) noexcept = default;
	};

	template < typename Types >
	class GraphNaryTreeTest : public ::testing::Test
	{
	public:
		void fullyBalancedTree(auto &tree)
		{
			// Level 1
			const auto root = tree.insert_root(size_t{1});

			// Level 2
			const auto l = tree.insert_front_child(root, size_t{2});
			const auto m1 = tree.insert_child(root, size_t{6}, 1);
			const auto m2 = tree.insert_child(root, size_t{10}, 2);
			const auto r = tree.insert_back_child(root, size_t{14});

			// Level 3
			const auto lr = tree.insert_front_child(l, size_t{5});
			const auto lm = tree.insert_front_child(l, size_t{4});
			const auto ll = tree.insert_front_child(l, size_t{3});

			const auto m1l = tree.insert_back_child(m1, size_t{9});
			const auto m1m = tree.insert_child(m1, size_t{8}, 0);
			const auto m1r = tree.insert_front_child(m1, size_t{7});

			const auto m2l = tree.insert_child(m2, size_t{11}, 0);
			const auto m2m = tree.insert_child(m2, size_t{12}, 1);
			const auto m2r = tree.insert_child(m2, size_t{13}, 2);

			const auto rl = tree.insert_back_child(r, size_t{15});
			const auto rm = tree.insert_back_child(r, size_t{16});
			const auto rr = tree.insert_back_child(r, size_t{17});

			EXPECT_EQ(tree.size(), 17);
			EXPECT_FALSE(tree.empty());
			EXPECT_EQ(tree.levels(), 3);

			for (size_t i = 1; i < 18; i++)
			{
				EXPECT_TRUE(tree.contains(i));
				EXPECT_EQ(tree.count(i), 1);
			}

			for (size_t i = 18; i < 30; i++)
			{
				EXPECT_FALSE(tree.contains(i));
				EXPECT_EQ(tree.count(i), 0);
			}

			const auto nodesPerLevel = tree.nodes_per_level();

			EXPECT_EQ(nodesPerLevel[0], 1);
			EXPECT_EQ(nodesPerLevel[1], 4);
			EXPECT_EQ(nodesPerLevel[2], 12);

			EXPECT_FALSE(tree.binary());
		}

		void unbalancedTree(auto &tree)
		{
			// Level 1
			const auto root = tree.insert_root(size_t{1});

			// Level 2
			const auto f = tree.insert_front_child(root, size_t{2});
			const auto b = tree.insert_back_child(root, size_t{5});
			const auto m1 = tree.insert_child(root, size_t{3}, 1);
			const auto m2 = tree.insert_child(root, size_t{4}, 2);

			// Level 3
			const auto m1f = tree.insert_front_child(m1, size_t{6});
			const auto m1b = tree.insert_back_child(m1, size_t{7});

			const auto m2f = tree.insert_front_child(m2, size_t{8});

			const auto bf = tree.insert_front_child(b, size_t{9});
			const auto bb = tree.insert_back_child(b, size_t{10});

			// Level 4
			const auto m1bf = tree.insert_child(m1b, size_t{11}, 0);

			const auto m2fb = tree.insert_back_child(m2f, size_t{12});

			const auto bff = tree.insert_front_child(bf, size_t{13});

			// Level 4
			const auto m1bff = tree.insert_child(m1bf, size_t{14}, 0);

			EXPECT_EQ(tree.size(), 14);
			EXPECT_FALSE(tree.empty());
			EXPECT_EQ(tree.levels(), 5);

			for (size_t i = 1; i < 15; i++)
			{
				EXPECT_TRUE(tree.contains(i));
				EXPECT_EQ(tree.count(i), 1);
			}

			for (size_t i = 15; i < 30; i++)
			{
				EXPECT_FALSE(tree.contains(i));
				EXPECT_EQ(tree.count(i), 0);
			}

			const auto nodesPerLevel = tree.nodes_per_level();

			EXPECT_EQ(nodesPerLevel[0], 1);
			EXPECT_EQ(nodesPerLevel[1], 4);
			EXPECT_EQ(nodesPerLevel[2], 5);
			EXPECT_EQ(nodesPerLevel[3], 3);
			EXPECT_EQ(nodesPerLevel[4], 1);

			EXPECT_FALSE(tree.binary());
		}
	};

	TYPED_TEST_SUITE_P(GraphNaryTreeTest);

	TYPED_TEST_P(GraphNaryTreeTest, Insertion)
	{
		xablau::graph::nary_tree < typename TypeParam::NodeType > tree;

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->fullyBalancedTree(tree);
		this->unbalancedTree(tree);
	}

	TYPED_TEST_P(GraphNaryTreeTest, InsertionOrReplacement)
	{
		xablau::graph::nary_tree < typename TypeParam::NodeType > tree;

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->fullyBalancedTree(tree);

		tree.insert_or_replace_child(tree.find(1), size_t{15}, 4);
		EXPECT_EQ(tree.size(), 18);

		tree.insert_or_replace_child(tree.find(1), size_t{14}, 3);
		EXPECT_EQ(tree.size(), 15);

		tree.insert_or_replace_child(tree.find(2), size_t{4}, 2);
		EXPECT_EQ(tree.size(), 15);

		tree.insert_or_replace_child(tree.find(2), size_t{5}, 1);
		EXPECT_EQ(tree.size(), 15);

		for (size_t i = 1; i < 16; i++)
		{
			EXPECT_TRUE(tree.contains(i));
			EXPECT_EQ(tree.count(i), 1);
		}

		for (size_t i = 16; i < 30; i++)
		{
			EXPECT_FALSE(tree.contains(i));
			EXPECT_EQ(tree.count(i), 0);
		}

		EXPECT_EQ(tree.size(), 15);
	}

	TYPED_TEST_P(GraphNaryTreeTest, ForwardPreOrderIncrementIteratorBalanced)
	{
		xablau::graph::nary_tree < typename TypeParam::NodeType > tree;
		constexpr auto preOrderIndices = std::to_array({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 });

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

	TYPED_TEST_P(GraphNaryTreeTest, ForwardPostOrderIncrementIteratorBalanced)
	{
		xablau::graph::nary_tree < typename TypeParam::NodeType > tree;
		constexpr auto postOrderIndices = std::to_array({ 3, 4, 5, 2, 7, 8, 9, 6, 11, 12, 13, 10, 15, 16, 17, 14, 1 });

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

	TYPED_TEST_P(GraphNaryTreeTest, ForwardPreOrderIncrementIteratorUnbalanced)
	{
		xablau::graph::nary_tree < typename TypeParam::NodeType > tree;
		constexpr auto preOrderIndices = std::to_array({ 1, 2, 3, 6, 7, 11, 14, 4, 8, 12, 5, 9, 13, 10 });

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

	TYPED_TEST_P(GraphNaryTreeTest, ForwardPostOrderIncrementIteratorUnbalanced)
	{
		xablau::graph::nary_tree < typename TypeParam::NodeType > tree;
		constexpr auto postOrderIndices = std::to_array({ 2, 6, 14, 11, 7, 3, 12, 8, 4, 13, 9, 10, 5, 1 });

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

	TYPED_TEST_P(GraphNaryTreeTest, ReversePreOrderIncrementIteratorBalanced)
	{
		xablau::graph::nary_tree < typename TypeParam::NodeType > tree;
		constexpr auto preOrderIndices = std::to_array({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 });

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

	TYPED_TEST_P(GraphNaryTreeTest, ReversePostOrderIncrementIteratorBalanced)
	{
		xablau::graph::nary_tree < typename TypeParam::NodeType > tree;
		constexpr auto postOrderIndices = std::to_array({ 3, 4, 5, 2, 7, 8, 9, 6, 11, 12, 13, 10, 15, 16, 17, 14, 1 });

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

	TYPED_TEST_P(GraphNaryTreeTest, ReversePreOrderIncrementIteratorUnbalanced)
	{
		xablau::graph::nary_tree < typename TypeParam::NodeType > tree;
		constexpr auto preOrderIndices = std::to_array({ 1, 2, 3, 6, 7, 11, 14, 4, 8, 12, 5, 9, 13, 10 });

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

	TYPED_TEST_P(GraphNaryTreeTest, ReversePostOrderIncrementIteratorUnbalanced)
	{
		xablau::graph::nary_tree < typename TypeParam::NodeType > tree;
		constexpr auto postOrderIndices = std::to_array({ 2, 6, 14, 11, 7, 3, 12, 8, 4, 13, 9, 10, 5, 1 });

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

	TYPED_TEST_P(GraphNaryTreeTest, ForwardPreOrderDecrementIteratorBalanced)
	{
		xablau::graph::nary_tree < typename TypeParam::NodeType > tree;
		constexpr auto preOrderIndices = std::to_array({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 });

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

	TYPED_TEST_P(GraphNaryTreeTest, ForwardPostOrderDecrementIteratorBalanced)
	{
		xablau::graph::nary_tree < typename TypeParam::NodeType > tree;
		constexpr auto postOrderIndices = std::to_array({ 3, 4, 5, 2, 7, 8, 9, 6, 11, 12, 13, 10, 15, 16, 17, 14, 1 });

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

	TYPED_TEST_P(GraphNaryTreeTest, ForwardPreOrderDecrementIteratorUnbalanced)
	{
		xablau::graph::nary_tree < typename TypeParam::NodeType > tree;
		constexpr auto preOrderIndices = std::to_array({ 1, 2, 3, 6, 7, 11, 14, 4, 8, 12, 5, 9, 13, 10 });

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

	TYPED_TEST_P(GraphNaryTreeTest, ForwardPostOrderDecrementIteratorUnbalanced)
	{
		xablau::graph::nary_tree < typename TypeParam::NodeType > tree;
		constexpr auto postOrderIndices = std::to_array({ 2, 6, 14, 11, 7, 3, 12, 8, 4, 13, 9, 10, 5, 1 });

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

	TYPED_TEST_P(GraphNaryTreeTest, ReversePreOrderDecrementIteratorBalanced)
	{
		xablau::graph::nary_tree < typename TypeParam::NodeType > tree;
		constexpr auto preOrderIndices = std::to_array({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 });

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

	TYPED_TEST_P(GraphNaryTreeTest, ReversePostOrderDecrementIteratorBalanced)
	{
		xablau::graph::nary_tree < typename TypeParam::NodeType > tree;
		constexpr auto postOrderIndices = std::to_array({ 3, 4, 5, 2, 7, 8, 9, 6, 11, 12, 13, 10, 15, 16, 17, 14, 1 });

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

	TYPED_TEST_P(GraphNaryTreeTest, ReversePreOrderDecrementIteratorUnbalanced)
	{
		xablau::graph::nary_tree < typename TypeParam::NodeType > tree;
		constexpr auto preOrderIndices = std::to_array({ 1, 2, 3, 6, 7, 11, 14, 4, 8, 12, 5, 9, 13, 10 });

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

	TYPED_TEST_P(GraphNaryTreeTest, ReversePostOrderDecrementIteratorUnbalanced)
	{
		xablau::graph::nary_tree < typename TypeParam::NodeType > tree;
		constexpr auto postOrderIndices = std::to_array({ 2, 6, 14, 11, 7, 3, 12, 8, 4, 13, 9, 10, 5, 1 });

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

	TYPED_TEST_P(GraphNaryTreeTest, NodesBalanced)
	{
		xablau::graph::nary_tree < typename TypeParam::NodeType > tree;
		constexpr auto leafIndices = std::to_array({ 3, 4, 5, 7, 8, 9, 11, 12, 13, 15, 16, 17 });

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->fullyBalancedTree(tree);

		auto nodes = tree.nodes();
		auto leafNodes = tree.leaf_nodes();

		EXPECT_EQ(nodes.size(), tree.size());
		EXPECT_EQ(nodes.size(), 17);
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

	TYPED_TEST_P(GraphNaryTreeTest, NodesUnbalanced)
	{
		xablau::graph::nary_tree < typename TypeParam::NodeType > tree;
		constexpr auto nodesIndices = std::to_array({ 1, 2, 3, 6, 7, 11, 14, 4, 8, 12, 5, 9, 13, 10 });
		constexpr auto leafIndices = std::to_array({ 2, 6, 14, 12, 13, 10 });

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->unbalancedTree(tree);

		auto nodes = tree.nodes();
		auto leafNodes = tree.leaf_nodes();

		EXPECT_EQ(nodes.size(), tree.size());
		EXPECT_EQ(nodes.size(), 14);
		EXPECT_EQ(leafNodes.size(), leafIndices.size());

		for (size_t i = 0; i < nodes.size(); i++)
		{
			EXPECT_EQ(nodes[i].get().value, nodesIndices[i]);
		}

		for (size_t i = 0; i < leafNodes.size(); i++)
		{
			EXPECT_EQ(leafNodes[i].get().value, leafIndices[i]);
		}
	}

	TYPED_TEST_P(GraphNaryTreeTest, FindBalanced)
	{
		xablau::graph::nary_tree < typename TypeParam::NodeType > tree;

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

	TYPED_TEST_P(GraphNaryTreeTest, FindUnbalanced)
	{
		xablau::graph::nary_tree < typename TypeParam::NodeType > tree;

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

	TYPED_TEST_P(GraphNaryTreeTest, FindWithPathBalanced)
	{
		xablau::graph::nary_tree < typename TypeParam::NodeType > tree;

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->fullyBalancedTree(tree);

		for (size_t i = 0; i < 2 * tree.size(); i++)
		{
			EXPECT_EQ(std::ranges::find(tree, i + 1), tree.find_with_path(i + 1).first);
		}

		EXPECT_EQ(tree.find_with_path(1).second, std::vector < size_t > ({}));
		EXPECT_EQ(tree.find_with_path(2).second, std::vector < size_t > ({ 0 }));
		EXPECT_EQ(tree.find_with_path(3).second, std::vector < size_t > ({ 0, 0 }));
		EXPECT_EQ(tree.find_with_path(4).second, std::vector < size_t > ({ 0, 1 }));
		EXPECT_EQ(tree.find_with_path(5).second, std::vector < size_t > ({ 0, 2 }));
		EXPECT_EQ(tree.find_with_path(6).second, std::vector < size_t > ({ 1 }));
		EXPECT_EQ(tree.find_with_path(7).second, std::vector < size_t > ({ 1, 0 }));
		EXPECT_EQ(tree.find_with_path(8).second, std::vector < size_t > ({ 1, 1 }));
		EXPECT_EQ(tree.find_with_path(9).second, std::vector < size_t > ({ 1, 2 }));
		EXPECT_EQ(tree.find_with_path(10).second, std::vector < size_t > ({ 2 }));
		EXPECT_EQ(tree.find_with_path(11).second, std::vector < size_t > ({ 2, 0 }));
		EXPECT_EQ(tree.find_with_path(12).second, std::vector < size_t > ({ 2, 1 }));
		EXPECT_EQ(tree.find_with_path(13).second, std::vector < size_t > ({ 2, 2 }));
		EXPECT_EQ(tree.find_with_path(14).second, std::vector < size_t > ({ 3 }));
		EXPECT_EQ(tree.find_with_path(15).second, std::vector < size_t > ({ 3, 0 }));
		EXPECT_EQ(tree.find_with_path(16).second, std::vector < size_t > ({ 3, 1 }));
		EXPECT_EQ(tree.find_with_path(17).second, std::vector < size_t > ({ 3, 2 }));
	}

	TYPED_TEST_P(GraphNaryTreeTest, FindWithPathUnbalanced)
	{
		xablau::graph::nary_tree < typename TypeParam::NodeType > tree;

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->unbalancedTree(tree);

		for (size_t i = 0; i < 2 * tree.size(); i++)
		{
			EXPECT_EQ(std::ranges::find(tree, i + 1), tree.find_with_path(i + 1).first);
		}

		EXPECT_EQ(tree.find_with_path(1).second, std::vector < size_t > ({}));
		EXPECT_EQ(tree.find_with_path(2).second, std::vector < size_t > ({ 0 }));
		EXPECT_EQ(tree.find_with_path(3).second, std::vector < size_t > ({ 1 }));
		EXPECT_EQ(tree.find_with_path(4).second, std::vector < size_t > ({ 2 }));
		EXPECT_EQ(tree.find_with_path(5).second, std::vector < size_t > ({ 3 }));
		EXPECT_EQ(tree.find_with_path(6).second, std::vector < size_t > ({ 1, 0 }));
		EXPECT_EQ(tree.find_with_path(7).second, std::vector < size_t > ({ 1, 1 }));
		EXPECT_EQ(tree.find_with_path(8).second, std::vector < size_t > ({ 2, 0 }));
		EXPECT_EQ(tree.find_with_path(9).second, std::vector < size_t > ({ 3, 0 }));
		EXPECT_EQ(tree.find_with_path(10).second, std::vector < size_t > ({ 3, 1 }));
		EXPECT_EQ(tree.find_with_path(11).second, std::vector < size_t > ({ 1, 1, 0 }));
		EXPECT_EQ(tree.find_with_path(12).second, std::vector < size_t > ({ 2, 0, 0 }));
		EXPECT_EQ(tree.find_with_path(13).second, std::vector < size_t > ({ 3, 0, 0 }));
		EXPECT_EQ(tree.find_with_path(14).second, std::vector < size_t > ({ 1, 1, 0, 0 }));
	}

	TYPED_TEST_P(GraphNaryTreeTest, EraseIndividualElementBalanced)
	{
		xablau::graph::nary_tree < typename TypeParam::NodeType > tree;

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->fullyBalancedTree(tree);

		EXPECT_EQ(*tree.erase(tree.find(5)), 6);
		EXPECT_EQ(tree.size(), 16);

		EXPECT_EQ(*tree.erase(tree.find(3)), 4);
		EXPECT_EQ(tree.size(), 15);

		EXPECT_EQ(*tree.erase(tree.find(4)), 6);
		EXPECT_EQ(tree.size(), 14);

		EXPECT_EQ(*tree.erase(tree.find(2)), 6);
		EXPECT_EQ(tree.size(), 13);

		EXPECT_EQ(*tree.erase(tree.find(6)), 10);
		EXPECT_EQ(tree.size(), 9);

		EXPECT_EQ(*tree.erase(tree.find(10)), 14);
		EXPECT_EQ(tree.size(), 5);

		EXPECT_EQ(tree.erase(tree.find(14)), tree.end());
		EXPECT_EQ(tree.size(), 1);

		EXPECT_EQ(tree.erase(tree.find(1)), tree.end());
		EXPECT_EQ(tree.size(), 0);
	}

	TYPED_TEST_P(GraphNaryTreeTest, EraseIndividualElementUnbalanced)
	{
		xablau::graph::nary_tree < typename TypeParam::NodeType > tree;

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->unbalancedTree(tree);

		EXPECT_EQ(*tree.erase(tree.find(12)), 5);
		EXPECT_EQ(tree.size(), 13);

		EXPECT_EQ(*tree.erase(tree.find(3)), 4);
		EXPECT_EQ(tree.size(), 8);

		EXPECT_EQ(*tree.erase(tree.find(9)), 10);
		EXPECT_EQ(tree.size(), 6);

		EXPECT_EQ(*tree.erase(tree.find(4)), 5);
		EXPECT_EQ(tree.size(), 4);

		EXPECT_EQ(tree.erase(tree.find(1)), tree.end());
		EXPECT_EQ(tree.size(), 0);
	}

	TYPED_TEST_P(GraphNaryTreeTest, EraseRangeBalanced)
	{
		xablau::graph::nary_tree < typename TypeParam::NodeType > tree;

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->fullyBalancedTree(tree);

		EXPECT_EQ(*tree.erase(tree.find(3), tree.find(6)), 6);
		EXPECT_EQ(tree.size(), 14);

		EXPECT_EQ(*tree.erase(tree.find(7), tree.find(9)), 9);
		EXPECT_EQ(tree.size(), 12);

		EXPECT_EQ(*tree.erase(tree.find(2), tree.find(10)), 10);
		EXPECT_EQ(tree.size(), 9);

		EXPECT_EQ(tree.erase(tree.find(10), tree.end()), tree.end());
		EXPECT_EQ(tree.size(), 1);

		EXPECT_EQ(tree.erase(tree.begin(), tree.end()), tree.end());
		EXPECT_EQ(tree.size(), 0);
	}

	TYPED_TEST_P(GraphNaryTreeTest, EraseRangeUnbalanced)
	{
		xablau::graph::nary_tree < typename TypeParam::NodeType > tree;

		EXPECT_TRUE(tree.empty());
		EXPECT_TRUE(tree.binary());

		this->unbalancedTree(tree);

		EXPECT_EQ(*tree.erase(tree.find(2), tree.find(3)), 3);
		EXPECT_EQ(tree.size(), 13);

		EXPECT_EQ(*tree.erase(tree.find(3), tree.find(5)), 5);
		EXPECT_EQ(tree.size(), 5);

		EXPECT_EQ(tree.erase(tree.find(9), tree.end()), tree.end());
		EXPECT_EQ(tree.size(), 2);

		EXPECT_EQ(tree.erase(tree.begin(), tree.end()), tree.end());
		EXPECT_EQ(tree.size(), 0);
	}

	TYPED_TEST_P(GraphNaryTreeTest, Concepts)
	{
		using tree_type = typename xablau::graph::nary_tree < typename TypeParam::NodeType >;

		EXPECT_FALSE(xablau::graph::concepts::node < tree_type >);
		EXPECT_FALSE(xablau::graph::concepts::binary_node < tree_type >);
		EXPECT_FALSE(xablau::graph::concepts::binary_node_parent < tree_type >);
		EXPECT_FALSE(xablau::graph::concepts::binary_node_with_parent < tree_type >);
		EXPECT_FALSE(xablau::graph::concepts::nary_node < tree_type >);
		EXPECT_FALSE(xablau::graph::concepts::nary_node_parent < tree_type >);
		EXPECT_FALSE(xablau::graph::concepts::nary_node_with_parent < tree_type >);
		EXPECT_TRUE(xablau::graph::concepts::tree < tree_type >);
		EXPECT_FALSE(xablau::graph::concepts::binary_tree < tree_type >);
		EXPECT_TRUE(xablau::graph::concepts::nary_tree < tree_type >);
		EXPECT_FALSE(xablau::graph::concepts::edge < tree_type >);
		EXPECT_FALSE(xablau::graph::concepts::graph < tree_type >);
		EXPECT_FALSE(xablau::graph::concepts::digraph < tree_type >);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		GraphNaryTreeTest,
		Insertion,
		InsertionOrReplacement,
		ForwardPreOrderIncrementIteratorBalanced,
		ForwardPostOrderIncrementIteratorBalanced,
		ForwardPreOrderIncrementIteratorUnbalanced,
		ForwardPostOrderIncrementIteratorUnbalanced,
		ReversePreOrderIncrementIteratorBalanced,
		ReversePostOrderIncrementIteratorBalanced,
		ReversePreOrderIncrementIteratorUnbalanced,
		ReversePostOrderIncrementIteratorUnbalanced,
		ForwardPreOrderDecrementIteratorBalanced,
		ForwardPostOrderDecrementIteratorBalanced,
		ForwardPreOrderDecrementIteratorUnbalanced,
		ForwardPostOrderDecrementIteratorUnbalanced,
		ReversePreOrderDecrementIteratorBalanced,
		ReversePostOrderDecrementIteratorBalanced,
		ReversePreOrderDecrementIteratorUnbalanced,
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

	using GraphNaryTreeTestDataTypes =
		std::tuple <
			xablau::graph::nary_tree_node < size_t, xablau::graph::nary_tree_node_container < xablau::graph::nary_tree_node_container_value::deque > >,
			xablau::graph::nary_tree_node < size_t, xablau::graph::nary_tree_node_container < xablau::graph::nary_tree_node_container_value::list > >,
			xablau::graph::nary_tree_node < size_t, xablau::graph::nary_tree_node_container < xablau::graph::nary_tree_node_container_value::vector > >,
			xablau::graph::nary_tree_node < GraphNaryTreeCustomType, xablau::graph::nary_tree_node_container < xablau::graph::nary_tree_node_container_value::deque > >,
			xablau::graph::nary_tree_node < GraphNaryTreeCustomType, xablau::graph::nary_tree_node_container < xablau::graph::nary_tree_node_container_value::list > >,
			xablau::graph::nary_tree_node < GraphNaryTreeCustomType, xablau::graph::nary_tree_node_container < xablau::graph::nary_tree_node_container_value::vector > > >;

	using GraphNaryTreeTestGroup =
		xablau::testing::test_bundler < GraphNaryTreeTestDataTypes > ::template test_group < GraphNaryTreeTypeDefinitions >;

	using GraphNaryTreeTypes = xablau::testing::tuple_transformation < ::testing::Types, GraphNaryTreeTestGroup > ::types;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		Xablau,
		GraphNaryTreeTest,
		GraphNaryTreeTypes);
}
