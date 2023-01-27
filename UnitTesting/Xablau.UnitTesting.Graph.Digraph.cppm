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
import <ranges>;

import xablau.algebra;
import xablau.graph;
import xablau.testing;

namespace xablau::unit_testing
{
	template < typename NodeType, typename MapContainerType, typename EdgeType >
	struct GraphDiraphTypeDefinitions
	{
		using value_type = typename NodeType::value_type;
		using node_type = NodeType;
		using graph_container_type = MapContainerType;
		using edge_type = EdgeType;
	};

	class GraphDiraphCustomType
	{
	private:
		std::vector < size_t > data;

	public:
		explicit operator size_t() const { return this->data.back(); }

		auto operator<=>(const GraphDiraphCustomType &) const = default;

		GraphDiraphCustomType &operator=(const GraphDiraphCustomType &) = default;

		GraphDiraphCustomType &operator=(GraphDiraphCustomType &&) noexcept = default;

		GraphDiraphCustomType()
		{
			this->data.push_back(0);
		}

		GraphDiraphCustomType(const size_t data)
		{
			this->data.push_back(data);
		}

		GraphDiraphCustomType(const GraphDiraphCustomType &) = default;

		GraphDiraphCustomType(GraphDiraphCustomType &&) noexcept = default;
	};
}

namespace std
{
	template <>
	struct hash < xablau::unit_testing::GraphDiraphCustomType >
	{
		size_t operator()(const xablau::unit_testing::GraphDiraphCustomType &data) const noexcept
		{
			return std::hash < size_t > {}(static_cast < size_t > (data));
		}
	};
}

namespace xablau::unit_testing
{
	template < typename Types >
	class GraphDiraphTest : public ::testing::Test
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

		static void halfDigraph(auto &digraph)
		{
			for (size_t i = 0; i < 10; i++)
			{
				const auto valueI = GraphDiraphTest::createValue(i);

				if (digraph.contains(valueI))
				{
					EXPECT_FALSE(digraph.insert(valueI));
				}

				else
				{
					EXPECT_TRUE(digraph.insert(valueI));
				}

				for (size_t j = i; j < 10; j++)
				{
					const auto valueJ = GraphDiraphTest::createValue(j);

					EXPECT_EQ(
						digraph.insert(
							valueI,
							valueJ,
							Types::edge_type(static_cast < float > (j + 1))).value().get().weight,
						static_cast < float > (j + 1));
				}

				if constexpr (
					Types::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_ordered ||
					Types::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_unordered)
				{
					for (size_t j = i; j < 10; j++)
					{
						const auto valueJ = GraphDiraphTest::createValue(j);

						EXPECT_EQ(
							digraph.insert(
								valueI,
								valueJ,
								Types::edge_type(static_cast < float > (j + 1))).value().get().weight,
							static_cast < float > (j + 1));
					}
				}
			}
		}

		static void completeDigraph(auto &digraph)
		{
			for (size_t i = 0; i < 10; i++)
			{
				const auto valueI = GraphDiraphTest::createValue(i);

				if (digraph.contains(valueI))
				{
					EXPECT_FALSE(digraph.insert(valueI));
				}

				else
				{
					EXPECT_TRUE(digraph.insert(valueI));
				}

				for (size_t j = 0; j < 10; j++)
				{
					const auto valueJ = GraphDiraphTest::createValue(j);

					EXPECT_EQ(
						digraph.insert(
							valueI,
							valueJ,
							Types::edge_type(static_cast < float > (j + 1))).value().get().weight,
						static_cast < float > (j + 1));
				}

				if constexpr (
					Types::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_ordered ||
					Types::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_unordered)
				{
					for (size_t j = 0; j < 10; j++)
					{
						const auto valueJ = GraphDiraphTest::createValue(j);

						EXPECT_EQ(
							digraph.insert(
								valueI,
								valueJ,
								Types::edge_type(static_cast < float > (j + 1))).value().get().weight,
							static_cast < float > (j + 1));
					}
				}
			}
		}

		static void completeDigraphWithExtraLevels(auto &digraph)
		{
			GraphDiraphTest::completeDigraph(digraph);

			for (size_t i = 0; i < 10; i++)
			{
				const auto valueI = GraphDiraphTest::createValue(i);
				const auto valueJ = GraphDiraphTest::createValue(i + 10);
				const auto valueK = GraphDiraphTest::createValue(i + 20);

				EXPECT_EQ(
					digraph.insert(
						valueI,
						valueJ,
						Types::edge_type(float{1})).value().get().weight, float{1});

				EXPECT_EQ(
					digraph.insert(
						valueK,
						valueI,
						Types::edge_type(float{1})).value().get().weight, float{1});
			}
		}

		static void disconnectedDigraph(auto &digraph)
		{
			size_t counter{};

			for (size_t i = 0; i < 10; i++)
			{
				for (size_t j = 0; j < i + 1; j++)
				{
					const auto index1 = counter;
					const auto index2 = ++counter;

					EXPECT_EQ(
						digraph.insert(
							GraphDiraphTest::createValue(index1),
							GraphDiraphTest::createValue(index2),
							Types::edge_type(float{1})).value().get().weight,
						float{1});
				}

				counter++;
			}
		}

		static void notStronglyConnectedDigraph(auto &digraph)
		{
			const auto insertConnection =
				[&digraph] (auto value1, auto value2) -> void
				{
					EXPECT_EQ(
						digraph.insert(
							GraphDiraphTest::createValue(value1),
							GraphDiraphTest::createValue(value2),
							Types::edge_type(float{1})).value().get().weight,
						float{1});
				};

			insertConnection(0, 1);
			insertConnection(1, 2);
			insertConnection(2, 3);
			insertConnection(3, 1);
			insertConnection(4, 2);
			insertConnection(4, 3);
			insertConnection(4, 6);
			insertConnection(5, 3);
			insertConnection(5, 7);
			insertConnection(6, 4);
			insertConnection(6, 5);
			insertConnection(7, 5);
			insertConnection(8, 6);
			insertConnection(8, 7);
			insertConnection(8, 8);
			insertConnection(9, 8);

			digraph.insert(GraphDiraphTest::createValue(10));
		}
	};

	TYPED_TEST_SUITE_P(GraphDiraphTest);

	TYPED_TEST_P(GraphDiraphTest, MultipleEdges)
	{
		using graph_type =
			typename xablau::graph::digraph <
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

	TYPED_TEST_P(GraphDiraphTest, Insertion)
	{
		xablau::graph::digraph <
			typename TypeParam::node_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > digraph;

		this->completeDigraph(digraph);

		EXPECT_EQ(digraph.node_count(), 10);

		if constexpr (
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_ordered ||
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_unordered)
		{
			EXPECT_EQ(digraph.edge_count(), 200);
		}

		else
		{
			EXPECT_EQ(digraph.edge_count(), 100);
		}

		EXPECT_EQ(digraph.unique_edge_count(), 100);

		for (size_t i = 0; i < 10; i++)
		{
			if constexpr (
				TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_ordered ||
				TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_unordered)
			{
				EXPECT_EQ(digraph.edge_count(this->createValue(i)), 20);
			}

			else
			{
				EXPECT_EQ(digraph.edge_count(this->createValue(i)), 10);
			}

			EXPECT_EQ(digraph.unique_edge_count(this->createValue(i)), 10);
		}

		for (size_t i = 0; i < 20; i++)
		{
			if (i < 10)
			{
				EXPECT_TRUE(digraph.contains(this->createValue(i)));
			}

			else
			{
				EXPECT_FALSE(digraph.contains(this->createValue(i)));
			}

			for (size_t j = 0; j < 20; j++)
			{
				if (i < 10 && j < 10)
				{
					EXPECT_TRUE(digraph.contains(this->createValue(i), this->createValue(j)));
				}

				else
				{
					EXPECT_FALSE(digraph.contains(this->createValue(i), this->createValue(j)));
				}
			}
		}
	}

	TYPED_TEST_P(GraphDiraphTest, Deletion)
	{
		xablau::graph::digraph <
			typename TypeParam::node_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > digraph;

		this->completeDigraph(digraph);

		for (size_t i = 0; i < 10; i++)
		{
			for (size_t j = 0; j < 10; j++)
			{
				EXPECT_TRUE(digraph.erase(this->createValue(i), this->createValue(j)));
			}

			EXPECT_EQ(digraph.degree(this->createValue(i)), float{});
		}

		EXPECT_EQ(digraph.volume(), float{});
		EXPECT_EQ(digraph.node_count(), 10);
		EXPECT_EQ(digraph.edge_count(), 0);
		EXPECT_EQ(digraph.unique_edge_count(), 0);

		for (size_t i = 0; i < 10; i++)
		{
			EXPECT_TRUE(digraph.erase(this->createValue(i)));
		}

		EXPECT_EQ(digraph.node_count(), 0);
		EXPECT_EQ(digraph.volume(), float{});

		this->completeDigraphWithExtraLevels(digraph);

		digraph.clear();

		EXPECT_EQ(digraph.volume(), float{});
		EXPECT_EQ(digraph.node_count(), 0);
		EXPECT_EQ(digraph.edge_count(), 0);
		EXPECT_EQ(digraph.unique_edge_count(), 0);

		this->completeDigraph(digraph);

		for (size_t i = 0; i < 10; i++)
		{
			EXPECT_TRUE(digraph.erase(this->createValue(i)));
			EXPECT_EQ(digraph.degree(this->createValue(i)), float{});
		}

		EXPECT_EQ(digraph.volume(), float {});
		EXPECT_EQ(digraph.node_count(), 0);
		EXPECT_EQ(digraph.edge_count(), 0);
		EXPECT_EQ(digraph.unique_edge_count(), 0);
	}

	TYPED_TEST_P(GraphDiraphTest, Degree)
	{
		xablau::graph::digraph <
			typename TypeParam::node_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > digraph;

		this->halfDigraph(digraph);

		float degreeSum{};
		const auto degreeDistribution = digraph.degree_distribution();

		EXPECT_EQ(degreeDistribution.size(), 10);

		for (size_t i = 0; i < 10; i++)
		{
			const auto degree = digraph.degree(this->createValue(i));

			degreeSum += degree;

			EXPECT_EQ(digraph.degree_distribution(degree), static_cast < float > (0.1));
			EXPECT_EQ(degreeDistribution.at(degree), static_cast < float > (0.1));

			std::array < float, 10 > sum{};

			std::generate(sum.begin(), sum.end(), [n = 1] () mutable { return static_cast < float > (n++); });
			std::fill(sum.begin(), sum.begin() + i, float{});

			if constexpr (
				TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_ordered ||
				TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_unordered)
			{
				EXPECT_EQ(degree, 2 * std::accumulate(sum.cbegin(), sum.cend(), float{}));
			}

			else
			{
				EXPECT_EQ(degree, std::accumulate(sum.cbegin(), sum.cend(), float{}));
			}
		}

		EXPECT_EQ(digraph.average_degree(), degreeSum / 10);
		EXPECT_EQ(digraph.density(), degreeSum / 90);
		EXPECT_EQ(digraph.volume(), degreeSum);
	}

	TYPED_TEST_P(GraphDiraphTest, StatisticalMoment)
	{
		xablau::graph::digraph <
			typename TypeParam::node_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > digraph;

		this->halfDigraph(digraph);

		if constexpr (
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_ordered ||
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_unordered)
		{
			EXPECT_TRUE(xablau::algebra::functions::almost_equal < float > ::invoke < 10 > (digraph.statistical_moment(0), float{1}));
			EXPECT_TRUE(xablau::algebra::functions::almost_equal < float > ::invoke < 10 > (digraph.statistical_moment(1), float{77}));
			EXPECT_TRUE(xablau::algebra::functions::almost_equal < float > ::invoke < 10 > (digraph.statistical_moment(2), float{6806.79980}));
			EXPECT_TRUE(xablau::algebra::functions::almost_equal < float > ::invoke < 10 > (digraph.statistical_moment(3), float{643253.625}));
		}

		else
		{
			EXPECT_TRUE(xablau::algebra::functions::almost_equal < float > ::invoke < 10 > (digraph.statistical_moment(0), float{1}));
			EXPECT_TRUE(xablau::algebra::functions::almost_equal < float > ::invoke < 10 > (digraph.statistical_moment(1), float{38.5}));
			EXPECT_TRUE(xablau::algebra::functions::almost_equal < float > ::invoke < 10 > (digraph.statistical_moment(2), float{1701.69995}));
			EXPECT_TRUE(xablau::algebra::functions::almost_equal < float > ::invoke < 10 > (digraph.statistical_moment(3), float{80406.7031}));
		}
	}

	TYPED_TEST_P(GraphDiraphTest, ShannonEntropy)
	{
		xablau::graph::digraph <
			typename TypeParam::node_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > digraph;

		this->halfDigraph(digraph);

		EXPECT_EQ(digraph.Shannon_entropy(), float{3.32192826});
	}

	TYPED_TEST_P(GraphDiraphTest, AdjacencyMatrix)
	{
		xablau::graph::digraph <
			typename TypeParam::node_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > digraph;

		this->completeDigraph(digraph);

		const auto [fixedMatrix, fixedNodeToIndex, fixedIndexToNode] =
			digraph.template adjacency_matrix <
				xablau::algebra::tensor_dense_fixed <
					float,
					xablau::algebra::tensor_fixed_dimensionalities < 10, 10 >,
					xablau::algebra::tensor_contiguity < false > >,
				true > ();

		const auto [dynamicMatrix, dynamicNodeToIndex, dynamicIndexToNode] =
			digraph.template adjacency_matrix <
				xablau::algebra::tensor_dense_dynamic <
					float,
					xablau::algebra::tensor_rank < 2 >,
					xablau::algebra::tensor_contiguity < false > >,
				true > ();

		const auto &graphContainer = digraph.container();

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

	TYPED_TEST_P(GraphDiraphTest, LaplacianMatrix)
	{
		xablau::graph::digraph <
			typename TypeParam::node_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > digraph;

		this->completeDigraph(digraph);

		const auto [fixedMatrix, fixedNodeToIndex, fixedIndexToNode] =
			digraph.template laplacian_matrix <
				xablau::algebra::tensor_dense_fixed <
					float,
					xablau::algebra::tensor_fixed_dimensionalities < 10, 10 >,
					xablau::algebra::tensor_contiguity < false > > > ();

		const auto [dynamicMatrix, dynamicNodeToIndex, dynamicIndexToNode] =
			digraph.template laplacian_matrix <
				xablau::algebra::tensor_dense_dynamic <
					float,
					xablau::algebra::tensor_rank < 2 >,
					xablau::algebra::tensor_contiguity < false > > > ();

		const auto &graphContainer = digraph.container();

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

	TYPED_TEST_P(GraphDiraphTest, TarjanStronglyConnectedComponents)
	{
		xablau::graph::digraph <
			typename TypeParam::node_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > digraph;

		this->notStronglyConnectedDigraph(digraph);

		const auto connectedComponents =
			digraph.Tarjan_strongly_connected_components();

		using component_type =
			typename xablau::graph::digraph <
				typename TypeParam::node_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type > ::node_set_container;

		component_type group1{};
		component_type group2{};
		component_type group3{};
		component_type group4{};
		component_type group5{};
		component_type group6{};
		component_type group7{};

		group1.insert(this->createValue(size_t{0}));

		group2.insert(this->createValue(size_t{1}));
		group2.insert(this->createValue(size_t{2}));
		group2.insert(this->createValue(size_t{3}));

		group3.insert(this->createValue(size_t{4}));
		group3.insert(this->createValue(size_t{6}));

		group4.insert(this->createValue(size_t{5}));
		group4.insert(this->createValue(size_t{7}));

		group5.insert(this->createValue(size_t{8}));

		group6.insert(this->createValue(size_t{9}));

		group7.insert(this->createValue(size_t{10}));

		EXPECT_EQ(connectedComponents.at(this->createValue(size_t{0})), group1);
		EXPECT_EQ(connectedComponents.at(this->createValue(size_t{1})), group2);
		EXPECT_EQ(connectedComponents.at(this->createValue(size_t{2})), group2);
		EXPECT_EQ(connectedComponents.at(this->createValue(size_t{3})), group2);
		EXPECT_EQ(connectedComponents.at(this->createValue(size_t{4})), group3);
		EXPECT_EQ(connectedComponents.at(this->createValue(size_t{5})), group4);
		EXPECT_EQ(connectedComponents.at(this->createValue(size_t{6})), group3);
		EXPECT_EQ(connectedComponents.at(this->createValue(size_t{7})), group4);
		EXPECT_EQ(connectedComponents.at(this->createValue(size_t{8})), group5);
		EXPECT_EQ(connectedComponents.at(this->createValue(size_t{9})), group6);
		EXPECT_EQ(connectedComponents.at(this->createValue(size_t{10})), group7);
	}

	TYPED_TEST_P(GraphDiraphTest, FloydWarshall)
	{
		xablau::graph::digraph <
			typename TypeParam::node_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > digraph;

		this->completeDigraph(digraph);

		const auto [fixedMatrix, fixedNextNode, fixedNodeToIndex, fixedIndexToNode] =
			digraph.template Floyd_Warshall <
				xablau::algebra::tensor_dense_fixed <
					float,
					xablau::algebra::tensor_fixed_dimensionalities < 10, 10 >,
					xablau::algebra::tensor_contiguity < false > > > ();

		const auto [dynamicMatrix, dynamicNextNode, dynamicNodeToIndex, dynamicIndexToNode] =
			digraph.template Floyd_Warshall <
				xablau::algebra::tensor_dense_dynamic <
					float,
					xablau::algebra::tensor_rank < 2 >,
					xablau::algebra::tensor_contiguity < false > > > ();

		const auto &graphContainer = digraph.container();

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

				if (fixedIndex1 == fixedIndex2)
				{
					EXPECT_EQ(fixedMatrix(fixedIndex1, fixedIndex2), float{});
				}

				else
				{
					EXPECT_EQ(
						fixedMatrix(fixedIndex1, fixedIndex2),
						static_cast < float > (fixedIndex2 + 1));
				}

				EXPECT_EQ(fixedNextNode(fixedIndex1, fixedIndex2).value(), iterator2->first);
				EXPECT_EQ(dynamicNextNode(fixedIndex1, fixedIndex2).value(), iterator2->first);
			}
		}
	}

	TYPED_TEST_P(GraphDiraphTest, TreeBFS)
	{
		using NaryNode =
			xablau::graph::nary_tree_node <
				typename TypeParam::node_type::value_type,
				xablau::graph::nary_tree_node_container < xablau::graph::nary_tree_node_container_value::vector > >;

		using NaryTree = xablau::graph::nary_tree < NaryNode >;

		xablau::graph::digraph <
			typename TypeParam::node_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > digraph;

		this->completeDigraphWithExtraLevels(digraph);

		for (auto iterator = digraph.container().cbegin(); iterator != digraph.container().cend(); ++iterator)
		{
			if (iterator->first >= this->createValue(10))
			{
				continue;
			}

			const auto tree =
				digraph.template generate_tree <
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

	TYPED_TEST_P(GraphDiraphTest, TreeDFS)
	{
		using NaryNode =
			xablau::graph::nary_tree_node <
				typename TypeParam::node_type::value_type,
				xablau::graph::nary_tree_node_container < xablau::graph::nary_tree_node_container_value::vector > >;

		using NaryTree = xablau::graph::nary_tree < NaryNode >;

		xablau::graph::digraph <
			typename TypeParam::node_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > digraph;

		this->completeDigraphWithExtraLevels(digraph);

		for (auto iterator = digraph.container().cbegin(); iterator != digraph.container().cend(); ++iterator)
		{
			if (iterator->first >= this->createValue(10))
			{
				continue;
			}

			const auto tree =
				digraph.template generate_tree <
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

	TYPED_TEST_P(GraphDiraphTest, TreeDijkstra)
	{
		using NaryNode =
			xablau::graph::nary_tree_node <
				typename TypeParam::node_type::value_type,
				xablau::graph::nary_tree_node_container < xablau::graph::nary_tree_node_container_value::vector > >;

		using NaryTree = xablau::graph::nary_tree < NaryNode >;

		xablau::graph::digraph <
			typename TypeParam::node_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > digraph;

		this->completeDigraphWithExtraLevels(digraph);

		for (auto iterator = digraph.container().cbegin(); iterator != digraph.container().cend(); ++iterator)
		{
			if (iterator->first >= this->createValue(10))
			{
				continue;
			}

			const auto tree =
				digraph.template generate_tree <
					NaryTree,
					xablau::graph::tree_generation_modes::Dijkstra > (iterator->first);

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

	TYPED_TEST_P(GraphDiraphTest, Connectivity)
	{
		using DigraphType =
			xablau::graph::digraph <
				typename TypeParam::node_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type >;

		DigraphType digraph;

		this->completeDigraphWithExtraLevels(digraph);

		EXPECT_TRUE(digraph.connected());
		EXPECT_EQ(digraph.dismember_disconnected_nodes().size(), 1);

		digraph.clear();

		this->disconnectedDigraph(digraph);

		EXPECT_FALSE(digraph.connected());

		auto groups = digraph.dismember_disconnected_nodes();

		EXPECT_EQ(groups.size(), 10);

		std::sort(groups.begin(), groups.end(),
			[] (const DigraphType &graph1, const DigraphType &graph2) -> bool
			{
				return graph1.node_count() < graph2.node_count();
			});

		for (size_t i = 0; i < groups.size(); i++)
		{
			EXPECT_EQ(groups[i].node_count(), i + 2);
		}
	}

	TYPED_TEST_P(GraphDiraphTest, Nodes)
	{
		using DigraphType =
			xablau::graph::digraph <
			typename TypeParam::node_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type >;

		DigraphType digraph;

		this->completeDigraphWithExtraLevels(digraph);

		const auto nodes = digraph.nodes();
		const auto sourceNodes = digraph.source_nodes();
		const auto sinkNodes = digraph.sink_nodes();

		EXPECT_EQ(nodes.size(), 30);
		EXPECT_EQ(sourceNodes.size(), 10);
		EXPECT_EQ(sinkNodes.size(), 10);

		// Every node.
		std::vector < typename TypeParam::node_type > _nodes;

		for (const auto &node : nodes)
		{
			_nodes.push_back(node);
		}

		std::ranges::sort(_nodes,
			[] (const typename TypeParam::node_type &node1, const typename TypeParam::node_type &node2) -> bool
			{
				return node1.value < node2.value;
			});

		for (size_t i = 0; i < _nodes.size(); i++)
		{
			EXPECT_EQ(this->createValue(i), _nodes[i]);
		}

		// Sink nodes.
		_nodes.clear();

		for (const auto &node : sinkNodes)
		{
			_nodes.push_back(node);
		}

		std::ranges::sort(_nodes,
			[] (const typename TypeParam::node_type &node1, const typename TypeParam::node_type &node2) -> bool
			{
				return node1.value < node2.value;
			});

		for (size_t i = 0; i < _nodes.size(); i++)
		{
			EXPECT_EQ(this->createValue(i + 10), _nodes[i]);
		}

		// Source node.
		_nodes.clear();

		for (const auto &node : sourceNodes)
		{
			_nodes.push_back(node);
		}

		std::ranges::sort(_nodes,
			[] (const typename TypeParam::node_type &node1, const typename TypeParam::node_type &node2) -> bool
			{
				return node1.value < node2.value;
			});

		for (size_t i = 0; i < _nodes.size(); i++)
		{
			EXPECT_EQ(this->createValue(i + 20), _nodes[i]);
		}
	}

	TYPED_TEST_P(GraphDiraphTest, Concepts)
	{
		using digraph_type =
			typename xablau::graph::digraph <
				typename TypeParam::node_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type >;

		EXPECT_FALSE(xablau::graph::concepts::node < digraph_type >);
		EXPECT_FALSE(xablau::graph::concepts::binary_node < digraph_type >);
		EXPECT_FALSE(xablau::graph::concepts::binary_node_parent < digraph_type >);
		EXPECT_FALSE(xablau::graph::concepts::binary_node_with_parent < digraph_type >);
		EXPECT_FALSE(xablau::graph::concepts::nary_node < digraph_type >);
		EXPECT_FALSE(xablau::graph::concepts::nary_node_parent < digraph_type >);
		EXPECT_FALSE(xablau::graph::concepts::nary_node_with_parent < digraph_type >);
		EXPECT_FALSE(xablau::graph::concepts::tree < digraph_type >);
		EXPECT_FALSE(xablau::graph::concepts::binary_tree < digraph_type >);
		EXPECT_FALSE(xablau::graph::concepts::nary_tree < digraph_type >);
		EXPECT_FALSE(xablau::graph::concepts::edge < digraph_type >);
		EXPECT_FALSE(xablau::graph::concepts::graph < digraph_type >);
		EXPECT_TRUE(xablau::graph::concepts::digraph < digraph_type >);

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
		GraphDiraphTest,
		MultipleEdges,
		Insertion,
		Deletion,
		Degree,
		StatisticalMoment,
		ShannonEntropy,
		AdjacencyMatrix,
		LaplacianMatrix,
		TarjanStronglyConnectedComponents,
		FloydWarshall,
		TreeBFS,
		TreeDFS,
		TreeDijkstra,
		Connectivity,
		Nodes,
		Concepts);

	using GraphDiraphTestNodeTypes =
		std::tuple <
			xablau::graph::node < size_t >,
			xablau::graph::node < std::string >,
			xablau::graph::node < GraphDiraphCustomType > >;

	using GraphDiraphTestMapContainerTypes =
		std::tuple <
			xablau::graph::graph_container_type < xablau::graph::graph_container_type_value::ordered >,
			xablau::graph::graph_container_type < xablau::graph::graph_container_type_value::unordered >,
			xablau::graph::graph_container_type < xablau::graph::graph_container_type_value::multi_ordered >,
			xablau::graph::graph_container_type < xablau::graph::graph_container_type_value::multi_unordered > >;

	using GraphDiraphTestEdgeTypes =
		std::tuple < xablau::graph::edge < float > >;

	using GraphDiraphTestGroup =
		xablau::testing::test_bundler <
			GraphDiraphTestNodeTypes,
			GraphDiraphTestMapContainerTypes,
			GraphDiraphTestEdgeTypes > ::template test_group < GraphDiraphTypeDefinitions >;

	using GraphDiraphTypes = xablau::testing::tuple_transformation < ::testing::Types, GraphDiraphTestGroup > ::types;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		Xablau,
		GraphDiraphTest,
		GraphDiraphTypes);
}
