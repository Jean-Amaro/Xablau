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
import <array>;
import <concepts>;
import <format>;
import <functional>;
import <numeric>;

import xablau.algebra;
import xablau.graph;
import xablau.testing;

namespace xablau::unit_testing
{
	template < typename NodeType, typename MapContainerType, typename EdgeType >
	struct GraphGraphTypeDefinitions
	{
		using value_type = typename NodeType::value_type;
		using node_type = NodeType;
		using graph_container_type = MapContainerType;
		using edge_type = EdgeType;
	};

	class GraphGraphCustomType
	{
	private:
		std::vector < size_t > data;

	public:
		explicit operator size_t() const { return this->data.back(); }

		auto operator<=>(const GraphGraphCustomType &) const = default;

		GraphGraphCustomType &operator=(const GraphGraphCustomType &) = default;

		GraphGraphCustomType &operator=(GraphGraphCustomType &&) noexcept = default;

		GraphGraphCustomType()
		{
			this->data.push_back(0);
		}

		GraphGraphCustomType(const size_t data)
		{
			this->data.push_back(data);
		}

		GraphGraphCustomType(const GraphGraphCustomType &) = default;

		GraphGraphCustomType(GraphGraphCustomType &&) noexcept = default;
	};
}

namespace std
{
	template <>
	struct hash < xablau::unit_testing::GraphGraphCustomType >
	{
		size_t operator()(const xablau::unit_testing::GraphGraphCustomType &data) const noexcept
		{
			return std::hash < size_t > {}(static_cast < size_t > (data));
		}
	};
}

namespace xablau::unit_testing
{
	template < typename Types >
	class GraphGraphTest : public ::testing::Test
	{
	public:
		static auto createValue(const size_t value)
		{
			if constexpr (std::same_as < typename Types::value_type, std::string >)
			{
				return typename Types::node_type{std::format("{:02}", value)};
			}

			else
			{
				return typename Types::node_type{value};
			}
		}

		template < typename NodeType >
		static auto createValue(const size_t value)
		{
			if constexpr (std::same_as < typename Types::value_type, std::string >)
			{
				return NodeType{std::format("{:02}", value)};
			}

			else
			{
				return NodeType{value};
			}
		}

		static void completeGraph(auto &graph)
		{
			for (size_t i = 0; i < 10; i++)
			{
				const auto valueI = GraphGraphTest::createValue(i);

				if (graph.contains(valueI))
				{
					EXPECT_FALSE(graph.insert(valueI));
				}

				else
				{
					EXPECT_TRUE(graph.insert(valueI));
				}

				for (size_t j = 0; j < 10; j++)
				{
					const auto valueJ = GraphGraphTest::createValue(j);

					if constexpr (
						Types::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_ordered ||
						Types::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_unordered)
					{
						EXPECT_EQ(
							graph.insert(
								valueI,
								valueJ,
								Types::edge_type(static_cast < float > (j + 1))).value().get().weight,
							static_cast < float > (j + 1));
					}

					else
					{
						EXPECT_EQ(
							graph.insert(
								valueI,
								valueJ,
								Types::edge_type(static_cast < float > (j + 1))).value().get().weight,
							static_cast < float > (std::max(i, j) + 1));
					}
				}
			}
		}

		static void completeGraphWithExtraLevel(auto &graph)
		{
			GraphGraphTest::completeGraph(graph);

			for (size_t i = 0; i < 10; i++)
			{
				const auto valueI = GraphGraphTest::createValue(i);
				const auto valueJ = GraphGraphTest::createValue(i + 10);

				EXPECT_EQ(
					graph.insert(
						valueI,
						valueJ,
						Types::edge_type(float{1})).value().get().weight, float{1});
			}
		}

		static void disconnectedGraph(auto &graph)
		{
			size_t counter{};

			for (size_t i = 0; i < 10; i++)
			{
				for (size_t j = 0; j < i + 1; j++)
				{
					const auto index1 = counter;
					const auto index2 = ++counter;

					EXPECT_EQ(
						graph.insert(
							GraphGraphTest::createValue(index1),
							GraphGraphTest::createValue(index2),
							Types::edge_type(float{1})).value().get().weight,
						float{1});
				}

				counter++;
			}
		}
	};

	TYPED_TEST_SUITE_P(GraphGraphTest);

	TYPED_TEST_P(GraphGraphTest, MultipleEdges)
	{
		using graph_type =
			typename xablau::graph::graph <
				typename TypeParam::node_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type >;

		if constexpr (TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::ordered)
		{
			EXPECT_FALSE(graph_type::can_have_multiple_edges());
			EXPECT_TRUE(graph_type::cannot_have_multiple_edges());
			EXPECT_TRUE(graph_type::ordered());
			EXPECT_FALSE(graph_type::unordered());
		}

		else if constexpr (TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::unordered)
		{
			EXPECT_FALSE(graph_type::can_have_multiple_edges());
			EXPECT_TRUE(graph_type::cannot_have_multiple_edges());
			EXPECT_FALSE(graph_type::ordered());
			EXPECT_TRUE(graph_type::unordered());
		}

		else if constexpr (TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_ordered)
		{
			EXPECT_TRUE(graph_type::can_have_multiple_edges());
			EXPECT_FALSE(graph_type::cannot_have_multiple_edges());
			EXPECT_TRUE(graph_type::ordered());
			EXPECT_FALSE(graph_type::unordered());
		}

		else
		{
			EXPECT_TRUE(graph_type::can_have_multiple_edges());
			EXPECT_FALSE(graph_type::cannot_have_multiple_edges());
			EXPECT_FALSE(graph_type::ordered());
			EXPECT_TRUE(graph_type::unordered());
		}
	}

	TYPED_TEST_P(GraphGraphTest, Insertion)
	{
		xablau::graph::graph <
			typename TypeParam::node_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > graph;

		this->completeGraph(graph);

		EXPECT_EQ(graph.node_count(), 10);

		if constexpr (
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_ordered ||
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_unordered)
		{
			EXPECT_EQ(graph.edge_count(), 100);
		}

		else
		{
			EXPECT_EQ(graph.edge_count(), 55);
		}

		EXPECT_EQ(graph.unique_edge_count(), 55);

		for (size_t i = 0; i < 10; i++)
		{
			if constexpr (
				TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_ordered ||
				TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_unordered)
			{
				EXPECT_EQ(graph.edge_count(this->createValue(i)), 19);
			}

			else
			{
				EXPECT_EQ(graph.edge_count(this->createValue(i)), 10);
			}

			EXPECT_EQ(graph.unique_edge_count(this->createValue(i)), 10);
		}

		for (size_t i = 0; i < 20; i++)
		{
			if (i < 10)
			{
				EXPECT_TRUE(graph.contains(this->createValue(i)));
			}

			else
			{
				EXPECT_FALSE(graph.contains(this->createValue(i)));
			}

			for (size_t j = 0; j < 20; j++)
			{
				if (i < 10 && j < 10)
				{
					EXPECT_TRUE(graph.contains(this->createValue(i), this->createValue(j)));
				}

				else
				{
					EXPECT_FALSE(graph.contains(this->createValue(i), this->createValue(j)));
				}
			}
		}
	}

	TYPED_TEST_P(GraphGraphTest, Deletion)
	{
		xablau::graph::graph <
			typename TypeParam::node_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > graph;

		this->completeGraph(graph);

		for (size_t i = 0; i < 10; i++)
		{
			for (size_t j = 0; j < 10; j++)
			{
				if (j >= i)
				{
					EXPECT_TRUE(graph.erase(this->createValue(i), this->createValue(j)));
				}

				else
				{
					EXPECT_FALSE(graph.erase(this->createValue(i), this->createValue(j)));
				}
			}

			EXPECT_EQ(graph.degree(this->createValue(i)), float{});
		}

		EXPECT_EQ(graph.volume(), float{});
		EXPECT_EQ(graph.node_count(), 10);
		EXPECT_EQ(graph.edge_count(), 0);
		EXPECT_EQ(graph.unique_edge_count(), 0);

		for (size_t i = 0; i < 10; i++)
		{
			EXPECT_TRUE(graph.erase(this->createValue(i)));
		}

		EXPECT_EQ(graph.node_count(), 0);
		EXPECT_EQ(graph.volume(), float{});

		this->completeGraphWithExtraLevel(graph);

		graph.clear();

		EXPECT_EQ(graph.volume(), float{});
		EXPECT_EQ(graph.node_count(), 0);
		EXPECT_EQ(graph.edge_count(), 0);
		EXPECT_EQ(graph.unique_edge_count(), 0);

		this->completeGraph(graph);

		for (size_t i = 0; i < 10; i++)
		{
			EXPECT_TRUE(graph.erase(this->createValue(i)));
			EXPECT_EQ(graph.degree(this->createValue(i)), float{});
		}

		EXPECT_EQ(graph.volume(), float{});
		EXPECT_EQ(graph.node_count(), 0);
		EXPECT_EQ(graph.edge_count(), 0);
		EXPECT_EQ(graph.unique_edge_count(), 0);
	}

	TYPED_TEST_P(GraphGraphTest, Degree)
	{
		xablau::graph::graph <
			typename TypeParam::node_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > graph;

		this->completeGraph(graph);

		float degreeSum{};
		const auto degreeDistribution = graph.degree_distribution();

		EXPECT_EQ(degreeDistribution.size(), 10);

		for (size_t i = 0; i < 10; i++)
		{
			const auto degree = graph.degree(this->createValue(i));

			degreeSum += degree;

			EXPECT_EQ(graph.degree_distribution(degree), static_cast < float > (0.1));
			EXPECT_EQ(degreeDistribution.at(degree), static_cast < float > (0.1));

			if constexpr (
				TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_ordered ||
				TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_unordered)
			{
				EXPECT_EQ(degree, 55 + (10 * (i + 1)));
			}

			else
			{
				std::array < float, 10 > sum{};

				std::generate(sum.begin(), sum.end(), [n = 1] () mutable { return static_cast < float > (n++); });
				std::fill(sum.begin(), sum.begin() + i, static_cast < float > (i + 1));

				EXPECT_EQ(degree, std::accumulate(sum.cbegin(), sum.cend(), float{} + static_cast < float > (i + 1)));
			}
		}

		EXPECT_EQ(graph.average_degree(), degreeSum / 10);
		EXPECT_EQ(graph.density(), degreeSum / 90);
		EXPECT_EQ(graph.volume(), degreeSum / 2);
	}

	TYPED_TEST_P(GraphGraphTest, StatisticalMoment)
	{
		xablau::graph::graph <
			typename TypeParam::node_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > graph;

		this->completeGraph(graph);

		if constexpr (
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_ordered ||
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_unordered)
		{
			EXPECT_TRUE(xablau::algebra::functions::almost_equal < float > ::invoke < 10 > (graph.statistical_moment(0), float{1}));
			EXPECT_TRUE(xablau::algebra::functions::almost_equal < float > ::invoke < 10 > (graph.statistical_moment(1), float{110}));
			EXPECT_TRUE(xablau::algebra::functions::almost_equal < float > ::invoke < 10 > (graph.statistical_moment(2), float{12925}));
			EXPECT_TRUE(xablau::algebra::functions::almost_equal < float > ::invoke < 10 > (graph.statistical_moment(3), float{1603250}));
		}

		else
		{
			EXPECT_TRUE(xablau::algebra::functions::almost_equal < float > ::invoke < 10 > (graph.statistical_moment(0), float{1}));
			EXPECT_TRUE(xablau::algebra::functions::almost_equal < float > ::invoke < 10 > (graph.statistical_moment(1), float{77}));
			EXPECT_TRUE(xablau::algebra::functions::almost_equal < float > ::invoke < 10 > (graph.statistical_moment(2), float{6239.20020}));
			EXPECT_TRUE(xablau::algebra::functions::almost_equal < float > ::invoke < 10 > (graph.statistical_moment(3), float{531066.812}));
		}
	}

	TYPED_TEST_P(GraphGraphTest, ShannonEntropy)
	{
		xablau::graph::graph <
			typename TypeParam::node_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > graph;

		this->completeGraph(graph);

		EXPECT_EQ(graph.Shannon_entropy(), float{3.32192826});
	}

	TYPED_TEST_P(GraphGraphTest, AdjacencyMatrix)
	{
		xablau::graph::graph <
			typename TypeParam::node_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > graph;

		this->completeGraph(graph);

		const auto [fixedMatrix, fixedNodeToIndex, fixedIndexToNode] =
			graph.template adjacency_matrix <
				xablau::algebra::tensor_dense_fixed <
					float,
					xablau::algebra::tensor_fixed_dimensionalities < 10, 10 >,
					xablau::algebra::tensor_contiguity < false > >,
				true > ();

		const auto [dynamicMatrix, dynamicNodeToIndex, dynamicIndexToNode] =
			graph.template adjacency_matrix <
				xablau::algebra::tensor_dense_dynamic <
					float,
					xablau::algebra::tensor_rank < 2 >,
					xablau::algebra::tensor_contiguity < false > >,
				true > ();

		const auto &graphContainer = graph.container();

		for (auto iterator1 = graphContainer.cbegin(); iterator1 != graphContainer.cend(); ++iterator1)
		{
			for (auto iterator2 = iterator1->second.cbegin(); iterator2 != iterator1->second.cend(); ++iterator2)
			{
				const auto fixedIndex1 = fixedNodeToIndex.at(iterator1->first);
				const auto fixedIndex2 = fixedNodeToIndex.at(iterator2->first);
				const auto dynamicIndex1 = dynamicNodeToIndex.at(iterator1->first);
				const auto dynamicIndex2 = dynamicNodeToIndex.at(iterator2->first);

				EXPECT_EQ(fixedIndexToNode.at(fixedIndex1), iterator1->first);
				EXPECT_EQ(fixedIndexToNode.at(fixedIndex2), iterator2->first);
				EXPECT_EQ(dynamicIndexToNode.at(dynamicIndex1), iterator1->first);
				EXPECT_EQ(dynamicIndexToNode.at(dynamicIndex2), iterator2->first);

				EXPECT_EQ(fixedMatrix(fixedIndex1, fixedIndex2), dynamicMatrix(dynamicIndex1, dynamicIndex2));
				ASSERT_GT(fixedMatrix(fixedIndex1, fixedIndex2), float{});
			}
		}
	}

	TYPED_TEST_P(GraphGraphTest, LaplacianMatrix)
	{
		xablau::graph::graph <
			typename TypeParam::node_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > graph;

		this->completeGraph(graph);

		const auto [fixedMatrix, fixedNodeToIndex, fixedIndexToNode] =
			graph.template laplacian_matrix <
				xablau::algebra::tensor_dense_fixed <
					float,
					xablau::algebra::tensor_fixed_dimensionalities < 10, 10 >,
					xablau::algebra::tensor_contiguity < false > > > ();

		const auto [dynamicMatrix, dynamicNodeToIndex, dynamicIndexToNode] =
			graph.template laplacian_matrix <
				xablau::algebra::tensor_dense_dynamic <
					float,
					xablau::algebra::tensor_rank < 2 >,
					xablau::algebra::tensor_contiguity < false > > > ();

		const auto &graphContainer = graph.container();

		for (auto iterator1 = graphContainer.cbegin(); iterator1 != graphContainer.cend(); ++iterator1)
		{
			float accumulator{};
			float diagonal{};

			for (auto iterator2 = iterator1->second.cbegin(); iterator2 != iterator1->second.cend(); ++iterator2)
			{
				const auto fixedIndex1 = fixedNodeToIndex.at(iterator1->first);
				const auto fixedIndex2 = fixedNodeToIndex.at(iterator2->first);
				const auto dynamicIndex1 = dynamicNodeToIndex.at(iterator1->first);
				const auto dynamicIndex2 = dynamicNodeToIndex.at(iterator2->first);

				EXPECT_EQ(fixedIndexToNode.at(fixedIndex1), iterator1->first);
				EXPECT_EQ(fixedIndexToNode.at(fixedIndex2), iterator2->first);
				EXPECT_EQ(dynamicIndexToNode.at(dynamicIndex1), iterator1->first);
				EXPECT_EQ(dynamicIndexToNode.at(dynamicIndex2), iterator2->first);

				EXPECT_EQ(fixedMatrix(fixedIndex1, fixedIndex2), dynamicMatrix(dynamicIndex1, dynamicIndex2));

				if (fixedIndex1 == fixedIndex2)
				{
					ASSERT_GT(fixedMatrix(fixedIndex1, fixedIndex2), float{});
					diagonal = fixedMatrix(fixedIndex1, fixedIndex2);
				}

				else
				{
					ASSERT_LT(fixedMatrix(fixedIndex1, fixedIndex2), float{});
					accumulator += fixedMatrix(fixedIndex1, fixedIndex2);
				}
			}

			EXPECT_EQ(accumulator, -diagonal);
		}
	}

	TYPED_TEST_P(GraphGraphTest, FloydWarshall)
	{
		xablau::graph::graph <
			typename TypeParam::node_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > graph;

		this->completeGraph(graph);

		const auto [fixedMatrix, fixedNextNode, fixedNodeToIndex, fixedIndexToNode] =
			graph.template Floyd_Warshall <
				xablau::algebra::tensor_dense_fixed <
					float,
					xablau::algebra::tensor_fixed_dimensionalities < 10, 10 >,
					xablau::algebra::tensor_contiguity < false > > > ();

		const auto [dynamicMatrix, dynamicNextNode, dynamicNodeToIndex, dynamicIndexToNode] =
			graph.template Floyd_Warshall <
				xablau::algebra::tensor_dense_dynamic <
					float,
					xablau::algebra::tensor_rank < 2 >,
					xablau::algebra::tensor_contiguity < false > > > ();

		const auto &graphContainer = graph.container();

		for (auto iterator1 = graphContainer.cbegin(); iterator1 != graphContainer.cend(); ++iterator1)
		{
			for (auto iterator2 = iterator1->second.cbegin(); iterator2 != iterator1->second.cend(); ++iterator2)
			{
				const auto fixedIndex1 = fixedNodeToIndex.at(iterator1->first);
				const auto fixedIndex2 = fixedNodeToIndex.at(iterator2->first);
				const auto dynamicIndex1 = dynamicNodeToIndex.at(iterator1->first);
				const auto dynamicIndex2 = dynamicNodeToIndex.at(iterator2->first);

				EXPECT_EQ(fixedIndexToNode.at(fixedIndex1), iterator1->first);
				EXPECT_EQ(fixedIndexToNode.at(fixedIndex2), iterator2->first);
				EXPECT_EQ(dynamicIndexToNode.at(dynamicIndex1), iterator1->first);
				EXPECT_EQ(dynamicIndexToNode.at(dynamicIndex2), iterator2->first);

				EXPECT_EQ(fixedMatrix(fixedIndex1, fixedIndex2), dynamicMatrix(dynamicIndex1, dynamicIndex2));

				if constexpr (
					TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_ordered ||
					TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_unordered)
				{
					if (fixedIndex1 == fixedIndex2)
					{
						EXPECT_EQ(fixedMatrix(fixedIndex1, fixedIndex2), float{});
					}
					
					else if (fixedIndex1 == 0 || fixedIndex2 == 0)
					{
						EXPECT_EQ(fixedMatrix(fixedIndex1, fixedIndex2), float{1});
					}

					else
					{
						EXPECT_EQ(fixedMatrix(fixedIndex1, fixedIndex2), float{2});
					}

					if (fixedIndex1 == 0 || fixedIndex1 == 1 ||
						fixedIndex2 == 0 || fixedIndex2 == 1 ||
						fixedIndex1 == fixedIndex2)
					{
						EXPECT_EQ(fixedNextNode(fixedIndex1, fixedIndex2).value(), iterator2->first);
						EXPECT_EQ(dynamicNextNode(fixedIndex1, fixedIndex2).value(), iterator2->first);
					}

					else
					{
						EXPECT_EQ(fixedNextNode(fixedIndex1, fixedIndex2).value(), graphContainer.cbegin()->first);
						EXPECT_EQ(dynamicNextNode(fixedIndex1, fixedIndex2).value(), graphContainer.cbegin()->first);
					}
				}

				else
				{
					if (fixedIndex1 > fixedIndex2)
					{
						EXPECT_EQ(fixedMatrix(fixedIndex1, fixedIndex2), static_cast < float > (fixedIndex1) + 1);
					}

					else if (fixedIndex1 == fixedIndex2)
					{
						EXPECT_EQ(fixedMatrix(fixedIndex1, fixedIndex2), float{});
					}

					else
					{
						EXPECT_EQ(fixedMatrix(fixedIndex1, fixedIndex2), static_cast < float > (fixedIndex2) + 1);
					}

					EXPECT_EQ(fixedNextNode(fixedIndex1, fixedIndex2).value(), iterator2->first);
					EXPECT_EQ(dynamicNextNode(fixedIndex1, fixedIndex2).value(), iterator2->first);
				}
			}
		}
	}

	TYPED_TEST_P(GraphGraphTest, TreeBFS)
	{
		using NaryNode =
			xablau::graph::nary_tree_node <
				typename TypeParam::node_type::value_type,
				xablau::graph::nary_tree_node_container < xablau::graph::nary_tree_node_container_value::vector > >;

		using NaryTree = xablau::graph::nary_tree < NaryNode >;

		xablau::graph::graph <
			typename TypeParam::node_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > graph;

		this->completeGraphWithExtraLevel(graph);

		for (auto iterator = graph.container().cbegin(); iterator != graph.container().cend(); ++iterator)
		{
			if (iterator->first >= this->createValue(10))
			{
				continue;
			}

			const auto tree =
				graph.template generate_tree <
					NaryTree,
					xablau::graph::tree_generation_modes::breadth_first_search > (iterator->first);

			EXPECT_EQ(tree.size(), 20);

			const auto &root = tree.cbegin().node().value().get();
			auto children = root.valid_children();

			EXPECT_EQ(root.value, iterator->first.value);
			EXPECT_EQ(children.size(), 10);

			std::ranges::sort(children,
				[] (const std::reference_wrapper < const NaryNode > &node1, const std::reference_wrapper < const NaryNode > &node2) -> bool
				{
					return node1.get() < node2.get();
				});

			EXPECT_EQ(
				std::unique(children.begin(), children.end(),
					[] (const std::reference_wrapper < const NaryNode > &node1, const std::reference_wrapper < const NaryNode > &node2) -> bool
					{
						return node1.get() == node2.get();
					}), children.end());

			for (const auto &child : children)
			{
				if (child.get().children().size() == 1)
				{
					const auto &grandchild = child.get().children().back().value().get();

					EXPECT_EQ(grandchild.children().size(), 0);
					EXPECT_GE(grandchild.value, this->createValue(10));
				}

				else if (child.get().children().size() != 0)
				{
					throw std::runtime_error(""); // TODO: Create message.
				}
			}
		}
	}

	TYPED_TEST_P(GraphGraphTest, TreeDFS)
	{
		using NaryNode =
			xablau::graph::nary_tree_node <
				typename TypeParam::node_type::value_type,
				xablau::graph::nary_tree_node_container < xablau::graph::nary_tree_node_container_value::vector > >;

		using NaryTree = xablau::graph::nary_tree < NaryNode >;

		xablau::graph::graph <
			typename TypeParam::node_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > graph;

		this->completeGraphWithExtraLevel(graph);

		for (auto iterator = graph.container().cbegin(); iterator != graph.container().cend(); ++iterator)
		{
			if (iterator->first >= this->createValue(10))
			{
				continue;
			}

			const auto tree =
				graph.template generate_tree <
					NaryTree,
					xablau::graph::tree_generation_modes::depth_first_search > (iterator->first);

			EXPECT_EQ(tree.size(), 20);

			bool tailReached = false;
			auto currentNode = tree.cbegin().node().value();
			EXPECT_EQ(currentNode.get().value, iterator->first.value);

			for (size_t i = 0; i < 10; i++)
			{
				const auto children = currentNode.get().valid_children();

				if (children.size() == 2)
				{
					const auto &[min, max] =
						std::minmax(
							children.front().get(),
							children.back().get());

					EXPECT_LT(min, this->createValue < NaryNode > (10));
					EXPECT_GE(max, this->createValue < NaryNode > (10));

					currentNode = min;
				}

				else if (children.size() == 1)
				{
					if (tailReached)
					{
						throw std::runtime_error(""); // TODO: Create message.
					}

					else
					{
						tailReached = true;
					}
				}

				else
				{
					throw std::runtime_error(""); // TODO: Create message.
				}
			}
		}
	}

	TYPED_TEST_P(GraphGraphTest, TreeDijkstra)
	{
		using NaryNode =
			xablau::graph::nary_tree_node <
				typename TypeParam::node_type::value_type,
				xablau::graph::nary_tree_node_container < xablau::graph::nary_tree_node_container_value::vector > >;

		using NaryTree = xablau::graph::nary_tree < NaryNode >;

		xablau::graph::graph <
			typename TypeParam::node_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > graph;

		this->completeGraphWithExtraLevel(graph);

		for (auto iterator = graph.container().cbegin(); iterator != graph.container().cend(); ++iterator)
		{
			if (iterator->first >= this->createValue(10))
			{
				continue;
			}

			const auto tree =
				graph.template generate_tree <
					NaryTree,
					xablau::graph::tree_generation_modes::Dijkstra > (iterator->first);

			EXPECT_EQ(tree.size(), 20);

			constexpr bool multi =
				TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_ordered ||
				TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_unordered;

			const auto &root = tree.cbegin().node().value().get();
			auto children = root.valid_children();

			EXPECT_EQ(root.value, iterator->first.value);

			const auto regularFlow =
				[&] () -> void
				{
					EXPECT_EQ(children.size(), 10);

					std::ranges::sort(children,
						[] (const std::reference_wrapper < const NaryNode > &node1, const std::reference_wrapper < const NaryNode > &node2) -> bool
						{
							return node1.get() < node2.get();
						});

					EXPECT_EQ(
						std::unique(children.begin(), children.end(),
							[] (const std::reference_wrapper < const NaryNode > &node1, const std::reference_wrapper < const NaryNode > &node2) -> bool
							{
								return node1.get() == node2.get();
							}), children.end());

					for (const auto &child : children)
					{
						if (child.get().children().size() == 1)
						{
							const auto &grandchild = child.get().children().back().value().get();

							EXPECT_EQ(grandchild.children().size(), 0);
							EXPECT_GE(grandchild.value, this->createValue(10));
						}

						else if (child.get().children().size() != 0)
						{
							throw std::runtime_error(""); // TODO: Create message.
						}
					}
				};

			if constexpr (multi)
			{
				if (iterator->first >= this->createValue(2))
				{
					EXPECT_EQ(children.size(), 3);

					std::ranges::sort(children,
						[] (const std::reference_wrapper < const NaryNode > &node1, const std::reference_wrapper < const NaryNode > &node2) -> bool
						{
							return node1.get() < node2.get();
						});

					EXPECT_EQ(
						std::unique(children.begin(), children.end(),
							[] (const std::reference_wrapper < const NaryNode > &node1, const std::reference_wrapper < const NaryNode > &node2) -> bool
							{
								return node1.get() == node2.get();
							}), children.end());

					EXPECT_EQ(children[0].get().valid_children().size(), 8);
					EXPECT_EQ(children[1].get().valid_children().size(), 1);
					EXPECT_EQ(children[2].get().valid_children().size(), 0);

					for (size_t i = 0; i < 2; i++)
					{
						for (const auto &child : children[i].get().valid_children())
						{
							if (child.get().valid_children().size() == 1)
							{
								const auto &grandchild = child.get().valid_children().back().get();

								EXPECT_EQ(grandchild.valid_children().size(), 0);
								EXPECT_GE(grandchild.value, this->createValue(10));
							}

							else if (child.get().valid_children().size() != 0)
							{
								throw std::runtime_error(""); // TODO: Create message.
							}
						}
					}
				}

				else
				{
					regularFlow();
				}
			}

			else
			{
				regularFlow();
			}
		}
	}

	TYPED_TEST_P(GraphGraphTest, Connectivity)
	{
		using GraphType =
			xablau::graph::graph <
				typename TypeParam::node_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type >;

		GraphType graph;

		this->completeGraphWithExtraLevel(graph);

		EXPECT_TRUE(graph.connected());
		EXPECT_EQ(graph.dismember_disconnected_nodes().size(), 1);

		graph.clear();

		this->disconnectedGraph(graph);

		EXPECT_FALSE(graph.connected());

		auto groups = graph.dismember_disconnected_nodes();

		EXPECT_EQ(groups.size(), 10);

		std::sort(groups.begin(), groups.end(),
			[] (const GraphType &graph1, const GraphType &graph2) -> bool
			{
				return graph1.node_count() < graph2.node_count();
			});

		for (size_t i = 0; i < groups.size(); i++)
		{
			EXPECT_EQ(groups[i].node_count(), i + 2);
		}
	}

	TYPED_TEST_P(GraphGraphTest, Concepts)
	{
		using graph_type =
			typename xablau::graph::graph <
				typename TypeParam::node_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type >;

		EXPECT_FALSE(xablau::graph::concepts::node < graph_type >);
		EXPECT_FALSE(xablau::graph::concepts::binary_node < graph_type >);
		EXPECT_FALSE(xablau::graph::concepts::binary_node_parent < graph_type >);
		EXPECT_FALSE(xablau::graph::concepts::binary_node_with_parent < graph_type >);
		EXPECT_FALSE(xablau::graph::concepts::nary_node < graph_type >);
		EXPECT_FALSE(xablau::graph::concepts::nary_node_parent < graph_type >);
		EXPECT_FALSE(xablau::graph::concepts::nary_node_with_parent < graph_type >);
		EXPECT_FALSE(xablau::graph::concepts::tree < graph_type >);
		EXPECT_FALSE(xablau::graph::concepts::binary_tree < graph_type >);
		EXPECT_FALSE(xablau::graph::concepts::nary_tree < graph_type >);
		EXPECT_FALSE(xablau::graph::concepts::edge < graph_type >);
		EXPECT_TRUE(xablau::graph::concepts::graph < graph_type >);
		EXPECT_FALSE(xablau::graph::concepts::digraph < graph_type >);

		EXPECT_FALSE(xablau::graph::concepts::node < typename TypeParam::edge_type >);
		EXPECT_FALSE(xablau::graph::concepts::binary_node < typename TypeParam::edge_type >);
		EXPECT_FALSE(xablau::graph::concepts::binary_node_parent < typename TypeParam::edge_type >);
		EXPECT_FALSE(xablau::graph::concepts::binary_node_with_parent < typename TypeParam::edge_type >);
		EXPECT_FALSE(xablau::graph::concepts::nary_node < typename TypeParam::edge_type >);
		EXPECT_FALSE(xablau::graph::concepts::nary_node_parent < typename TypeParam::edge_type >);
		EXPECT_FALSE(xablau::graph::concepts::nary_node_with_parent < typename TypeParam::edge_type >);
		EXPECT_FALSE(xablau::graph::concepts::tree < typename TypeParam::edge_type >);
		EXPECT_FALSE(xablau::graph::concepts::binary_tree < typename TypeParam::edge_type >);
		EXPECT_FALSE(xablau::graph::concepts::nary_tree < typename TypeParam::edge_type >);
		EXPECT_TRUE(xablau::graph::concepts::edge < typename TypeParam::edge_type >);
		EXPECT_FALSE(xablau::graph::concepts::graph < typename TypeParam::edge_type >);
		EXPECT_FALSE(xablau::graph::concepts::digraph < typename TypeParam::edge_type >);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		GraphGraphTest,
		MultipleEdges,
		Insertion,
		Deletion,
		Degree,
		StatisticalMoment,
		ShannonEntropy,
		AdjacencyMatrix,
		LaplacianMatrix,
		FloydWarshall,
		TreeBFS,
		TreeDFS,
		TreeDijkstra,
		Connectivity,
		Concepts);

	using GraphGraphTestNodeTypes =
		std::tuple <
			xablau::graph::node < size_t >,
			xablau::graph::node < std::string >,
			xablau::graph::node < GraphGraphCustomType > >;

	using GraphGraphTestMapContainerTypes =
		std::tuple <
			xablau::graph::graph_container_type < xablau::graph::graph_container_type_value::ordered >,
			xablau::graph::graph_container_type < xablau::graph::graph_container_type_value::unordered >,
			xablau::graph::graph_container_type < xablau::graph::graph_container_type_value::multi_ordered >,
			xablau::graph::graph_container_type < xablau::graph::graph_container_type_value::multi_unordered > >;

	using GraphGraphTestEdgeTypes =
		std::tuple < xablau::graph::edge < float > >;

	using GraphGraphTestGroup =
		xablau::testing::test_bundler <
			GraphGraphTestNodeTypes,
			GraphGraphTestMapContainerTypes,
			GraphGraphTestEdgeTypes > ::template test_group < GraphGraphTypeDefinitions >;

	using GraphGraphTypes = xablau::testing::tuple_transformation < ::testing::Types, GraphGraphTestGroup > ::types;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		Xablau,
		GraphGraphTest,
		GraphGraphTypes);
}
