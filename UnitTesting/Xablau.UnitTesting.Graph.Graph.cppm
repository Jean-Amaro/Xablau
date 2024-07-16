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

import xablau.algebra;
import xablau.graph;
import xablau.testing;

namespace xablau::unit_testing
{
	template < typename NodeType, typename MapContainerType, typename EdgeType >
	struct GraphGraphTypeDefinitions
	{
		using value_type = NodeType;
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
				return typename Types::value_type{std::format("{:02}", value)};
			}

			else
			{
				return typename Types::value_type{value};
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

				EXPECT_EQ(graph.insert(valueI), valueI);

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
								Types::edge_type(static_cast < float > (j + 1))).value().get().weight(),
							static_cast < float > (j + 1));
					}

					else
					{
						EXPECT_EQ(
							graph.insert(
								valueI,
								valueJ,
								Types::edge_type(static_cast < float > (j + 1))).value().get().weight(),
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
						Types::edge_type(float{1})).value().get().weight(), float{1});
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
							Types::edge_type(float{1})).value().get().weight(),
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
				typename TypeParam::value_type,
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
			typename TypeParam::value_type,
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
		using graph_type =
			typename xablau::graph::graph <
			typename TypeParam::value_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type >;

		graph_type graph;

		this->completeGraph(graph);

		for (size_t i = 0; i < 10; i++)
		{
			for (size_t j = 0; j < 10; j++)
			{
				if (j >= i)
				{
					if constexpr (graph_type::can_have_multiple_edges())
					{
						if (j == i)
						{
							EXPECT_EQ(graph.erase(this->createValue(i), this->createValue(j)), 1);
						}

						else
						{
							EXPECT_EQ(graph.erase(this->createValue(i), this->createValue(j)), 2);
						}
					}

					else
					{
						EXPECT_EQ(graph.erase(this->createValue(i), this->createValue(j)), 1);
					}

				}

				else
				{
					EXPECT_EQ(graph.erase(this->createValue(i), this->createValue(j)), 0);
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
			EXPECT_EQ(graph.erase(this->createValue(i)), 0);
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
			if constexpr (graph_type::can_have_multiple_edges())
			{
				EXPECT_EQ(graph.erase(this->createValue(i)), (2 * (10 - i)) - 1);
			}

			else
			{
				EXPECT_EQ(graph.erase(this->createValue(i)), 10 - i);
			}

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
			typename TypeParam::value_type,
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
			typename TypeParam::value_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > graph;

		this->completeGraph(graph);

		const auto statisticalMomentOrder1 = graph.statistical_moment(1);
		const auto statisticalMomentOrder2 = graph.statistical_moment(2);
		const auto variance = statisticalMomentOrder2 - (statisticalMomentOrder1 * statisticalMomentOrder1);

		EXPECT_EQ(graph.variance(), variance);
		EXPECT_EQ(graph.complexity_measure(), statisticalMomentOrder2 / statisticalMomentOrder1);

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
			typename TypeParam::value_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > graph;

		this->completeGraph(graph);

		EXPECT_EQ(graph.Shannon_entropy(), float{3.32192826});
	}

	TYPED_TEST_P(GraphGraphTest, TriangleCount)
	{
		using GraphType =
			xablau::graph::graph <
			typename TypeParam::value_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type >;

		GraphType graph;

		const auto value1 = this->createValue(1);
		const auto value2 = this->createValue(2);
		const auto value3 = this->createValue(3);
		const auto value4 = this->createValue(4);
		const auto value5 = this->createValue(5);
		const auto value6 = this->createValue(6);
		const auto value7 = this->createValue(7);

		graph.insert(value1, value2);
		graph.insert(value1, value3);
		graph.insert(value1, value4);
		graph.insert(value2, value6);
		graph.insert(value2, value7);
		graph.insert(value3, value4);
		graph.insert(value3, value7);
		graph.insert(value4, value5);
		graph.insert(value4, value6);
		graph.insert(value5, value6);

		graph.insert(value1, value1);
		graph.insert(value2, value2);
		graph.insert(value3, value3);
		graph.insert(value4, value4);
		graph.insert(value5, value5);
		graph.insert(value6, value6);
		graph.insert(value7, value7);

		EXPECT_EQ(graph.triangle_count(), 2);
	}

	TYPED_TEST_P(GraphGraphTest, TriangleCountComplete)
	{
		using GraphType =
			xablau::graph::graph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type >;

		GraphType graph;

		this->completeGraph(graph);

		if constexpr (GraphType::can_have_multiple_edges())
		{
			EXPECT_EQ(graph.triangle_count(), 960);
		}

		else
		{
			EXPECT_EQ(graph.triangle_count(), 120);
		}
	}

	TYPED_TEST_P(GraphGraphTest, TripletCount)
	{
		xablau::graph::graph <
			typename TypeParam::value_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > graph;

		const auto value1 = this->createValue(1);
		const auto value2 = this->createValue(2);
		const auto value3 = this->createValue(3);
		const auto value4 = this->createValue(4);
		const auto value5 = this->createValue(5);
		const auto value6 = this->createValue(6);
		const auto value7 = this->createValue(7);

		graph.insert(value1, value2);
		graph.insert(value1, value3);
		graph.insert(value1, value4);
		graph.insert(value2, value6);
		graph.insert(value2, value7);
		graph.insert(value3, value4);
		graph.insert(value3, value7);
		graph.insert(value4, value5);
		graph.insert(value4, value6);
		graph.insert(value5, value6);

		graph.insert(value1, value1);
		graph.insert(value2, value2);
		graph.insert(value3, value3);
		graph.insert(value4, value4);
		graph.insert(value5, value5);
		graph.insert(value6, value6);
		graph.insert(value7, value7);

		EXPECT_EQ(graph.triplet_count(), 20);
	}

	TYPED_TEST_P(GraphGraphTest, TripletCountComplete)
	{
		using GraphType =
			xablau::graph::graph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type >;

		GraphType graph;

		this->completeGraph(graph);

		if constexpr (GraphType::can_have_multiple_edges())
		{
			EXPECT_EQ(graph.triplet_count(), 1530);
		}

		else
		{
			EXPECT_EQ(graph.triplet_count(), 360);
		}
	}

	TYPED_TEST_P(GraphGraphTest, ClusteringCoefficient)
	{
		using WeightType = typename TypeParam::edge_type::weight_type;

		xablau::graph::graph <
			typename TypeParam::value_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > graph;

		const auto value1 = this->createValue(1);
		const auto value2 = this->createValue(2);
		const auto value3 = this->createValue(3);
		const auto value4 = this->createValue(4);
		const auto value5 = this->createValue(5);
		const auto value6 = this->createValue(6);
		const auto value7 = this->createValue(7);

		graph.insert(value1, value2);
		graph.insert(value1, value3);
		graph.insert(value1, value4);
		graph.insert(value2, value6);
		graph.insert(value2, value7);
		graph.insert(value3, value4);
		graph.insert(value3, value7);
		graph.insert(value4, value5);
		graph.insert(value4, value6);
		graph.insert(value5, value6);

		graph.insert(value1, value1);
		graph.insert(value2, value2);
		graph.insert(value3, value3);
		graph.insert(value4, value4);
		graph.insert(value5, value5);
		graph.insert(value6, value6);
		graph.insert(value7, value7);

		const auto tripletCount = graph.triplet_count();
		const auto triangleCount = graph.triangle_count();

		EXPECT_EQ(
			graph.clustering_coefficient(),
			(WeightType{3} * static_cast < WeightType > (triangleCount)) / static_cast < WeightType > (tripletCount));
	}

	TYPED_TEST_P(GraphGraphTest, ClusteringCoefficientComplete)
	{
		using WeightType = typename TypeParam::edge_type::weight_type;

		xablau::graph::graph <
			typename TypeParam::value_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > graph;

		this->completeGraph(graph);

		const auto tripletCount = graph.triplet_count();
		const auto triangleCount = graph.triangle_count();

		EXPECT_EQ(
			graph.clustering_coefficient(),
			(WeightType{3} * static_cast < WeightType > (triangleCount)) / static_cast < WeightType > (tripletCount));
	}

	TYPED_TEST_P(GraphGraphTest, TriangleNeighbors)
	{
		using WeightType = typename TypeParam::edge_type::weight_type;

		xablau::graph::graph <
			typename TypeParam::value_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > graph;

		this->completeGraph(graph);

		for (size_t i = 0; i < graph.node_count(); i++)
		{
			auto neighbors = graph.triangle_neighbors < true > (this->createValue(i));

			std::ranges::sort(
				neighbors,
				[] (const auto &node1, const auto &node2) -> bool
				{
					return
						std::min(node1.first, node1.second) < std::min(node2.first, node2.second) ||

						std::min(node1.first, node1.second) == std::min(node2.first, node2.second) &&
						std::max(node1.first, node1.second) < std::max(node2.first, node2.second);
				});

			auto iterator = neighbors.cbegin();

			for (size_t j = 0; j < graph.node_count(); j++)
			{
				for (size_t k = j + 1; k < graph.node_count(); k++)
				{
					if (i == j || i == k)
					{
						continue;
					}

					EXPECT_EQ(this->createValue(j), iterator->first);
					EXPECT_EQ(this->createValue(k), iterator->second);

					++iterator;
				}
			}
		}
	}

	TYPED_TEST_P(GraphGraphTest, ClosenessCentrality)
	{
		using WeightType = typename TypeParam::edge_type::weight_type;

		xablau::graph::graph <
			typename TypeParam::value_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > graph;

		const auto value1 = this->createValue(1);
		const auto value2 = this->createValue(2);
		const auto value3 = this->createValue(3);
		const auto value4 = this->createValue(4);
		const auto value5 = this->createValue(5);

		graph.insert(value1, value2);
		graph.insert(value1, value3);
		graph.insert(value2, value3);
		graph.insert(value2, value4);
		graph.insert(value2, value5);

		graph.insert(value1, value2, WeightType{2});
		graph.insert(value1, value3, WeightType{2});
		graph.insert(value2, value3, WeightType{2});
		graph.insert(value2, value4, WeightType{2});
		graph.insert(value2, value5, WeightType{2});

		const auto centrality = graph.closeness_centrality < false > ();

		EXPECT_EQ(centrality.at(value1), WeightType{2} / WeightType{3});
		EXPECT_EQ(centrality.at(value2), WeightType{1});
		EXPECT_EQ(centrality.at(value3), WeightType{2} / WeightType{3});
		EXPECT_EQ(centrality.at(value4), WeightType{4} / WeightType{7});
		EXPECT_EQ(centrality.at(value5), WeightType{4} / WeightType{7});

		EXPECT_EQ(centrality.at(value1), graph.closeness_centrality(value1));
		EXPECT_EQ(centrality.at(value2), graph.closeness_centrality(value2));
		EXPECT_EQ(centrality.at(value3), graph.closeness_centrality(value3));
		EXPECT_EQ(centrality.at(value4), graph.closeness_centrality(value4));
		EXPECT_EQ(centrality.at(value5), graph.closeness_centrality(value5));
	}

	TYPED_TEST_P(GraphGraphTest, NodeBetweenness)
	{
		using WeightType = typename TypeParam::edge_type::weight_type;

		xablau::graph::graph <
			typename TypeParam::value_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > graph;

		const auto value1 = this->createValue(1);
		const auto value2 = this->createValue(2);
		const auto value3 = this->createValue(3);
		const auto value4 = this->createValue(4);
		const auto value5 = this->createValue(5);
		const auto value6 = this->createValue(6);

		graph.insert(value1, value2);
		graph.insert(value1, value5);
		graph.insert(value2, value3);
		graph.insert(value2, value4);
		graph.insert(value3, value4);
		graph.insert(value3, value6);
		graph.insert(value5, value6);

		if constexpr (
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_ordered ||
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_unordered)
		{
			graph.insert(value1, value2, WeightType{2});
			graph.insert(value1, value5, WeightType{2});
			graph.insert(value2, value3, WeightType{2});
			graph.insert(value2, value4, WeightType{2});
			graph.insert(value3, value4, WeightType{2});
			graph.insert(value3, value6, WeightType{2});
			graph.insert(value5, value6, WeightType{2});
		}

		const auto betweennesses = graph.node_betweenness_centrality < true > ();

		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (betweennesses.at(value1), WeightType{1.5}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (betweennesses.at(value2), WeightType{2.5}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (betweennesses.at(value3), WeightType{2.5}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (betweennesses.at(value4), WeightType{0}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (betweennesses.at(value5), WeightType{1}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (betweennesses.at(value6), WeightType{1.5}));

		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (graph.node_betweenness_centrality(value1), betweennesses.at(value1)));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (graph.node_betweenness_centrality(value2), betweennesses.at(value2)));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (graph.node_betweenness_centrality(value3), betweennesses.at(value3)));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (graph.node_betweenness_centrality(value4), betweennesses.at(value4)));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (graph.node_betweenness_centrality(value5), betweennesses.at(value5)));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (graph.node_betweenness_centrality(value6), betweennesses.at(value6)));
	}

	TYPED_TEST_P(GraphGraphTest, EdgeBetweenness)
	{
		using WeightType = typename TypeParam::edge_type::weight_type;

		xablau::graph::graph <
			typename TypeParam::value_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > graph;

		const auto value1 = this->createValue(1);
		const auto value2 = this->createValue(2);
		const auto value3 = this->createValue(3);
		const auto value4 = this->createValue(4);
		const auto value5 = this->createValue(5);
		const auto value6 = this->createValue(6);

		graph.insert(value1, value2);
		graph.insert(value1, value4);
		graph.insert(value2, value3);
		graph.insert(value2, value5);
		graph.insert(value3, value6);
		graph.insert(value4, value5);
		graph.insert(value5, value6);

		if constexpr (
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_ordered ||
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_unordered)
		{
			graph.insert(value1, value2, WeightType{2});
			graph.insert(value1, value4, WeightType{2});
			graph.insert(value2, value3, WeightType{2});
			graph.insert(value2, value5, WeightType{2});
			graph.insert(value3, value6, WeightType{2});
			graph.insert(value4, value5, WeightType{2});
			graph.insert(value5, value6, WeightType{2});
		}

		const auto betweennesses = graph.edge_betweenness_centrality < true > ();

		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (betweennesses.at(std::make_pair(value1, value2)), WeightType{8}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (betweennesses.at(std::make_pair(value1, value4)), WeightType{16} / WeightType{3}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (betweennesses.at(std::make_pair(value2, value3)), WeightType{8}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (betweennesses.at(std::make_pair(value2, value5)), WeightType{22} / WeightType{3}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (betweennesses.at(std::make_pair(value3, value6)), WeightType{16} / WeightType{3}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (betweennesses.at(std::make_pair(value4, value5)), WeightType{8}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (betweennesses.at(std::make_pair(value5, value6)), WeightType{8}));
	}

	TYPED_TEST_P(GraphGraphTest, Modularity)
	{
		using GraphType =
			xablau::graph::graph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type >;

		using WeightType = typename TypeParam::edge_type::weight_type;

		GraphType graph;

		const auto value1 = this->createValue(1);
		const auto value2 = this->createValue(2);
		const auto value3 = this->createValue(3);
		const auto value4 = this->createValue(4);
		const auto value5 = this->createValue(5);
		const auto value6 = this->createValue(6);
		const auto value7 = this->createValue(7);
		const auto value8 = this->createValue(8);
		const auto value9 = this->createValue(9);
		const auto value10 = this->createValue(10);

		graph.insert(value1, value2);
		graph.insert(value1, value3);
		graph.insert(value1, value10);
		graph.insert(value2, value3);
		graph.insert(value4, value5);
		graph.insert(value4, value6);
		graph.insert(value4, value10);
		graph.insert(value5, value6);
		graph.insert(value7, value8);
		graph.insert(value7, value9);
		graph.insert(value7, value10);
		graph.insert(value8, value9);

		if constexpr (GraphType::can_have_multiple_edges())
		{
			graph.insert(value1, value2);
			graph.insert(value1, value3);
			graph.insert(value1, value10);
			graph.insert(value2, value3);
			graph.insert(value4, value5);
			graph.insert(value4, value6);
			graph.insert(value4, value10);
			graph.insert(value5, value6);
			graph.insert(value7, value8);
			graph.insert(value7, value9);
			graph.insert(value7, value10);
			graph.insert(value8, value9);
		}

		std::vector < GraphType::template node_set_container < true > > communitiesClassification;

		communitiesClassification.emplace_back(GraphType::template node_set_container < true > ({ value1, value2, value3, value10 }));
		communitiesClassification.emplace_back(GraphType::template node_set_container < true > ({ value4, value5, value6 }));
		communitiesClassification.emplace_back(GraphType::template node_set_container < true > ({ value7, value8, value9 }));

		EXPECT_EQ(std::round(graph.modularity < true > (communitiesClassification) * WeightType{10000}) / WeightType{10000}, WeightType{0.4896});
	}

	TYPED_TEST_P(GraphGraphTest, AdjacencyMatrix)
	{
		xablau::graph::graph <
			typename TypeParam::value_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > graph;

		this->completeGraph(graph);

		const auto [fixedMatrix, fixedNodeToIndex, fixedIndexToNode] =
			graph.template adjacency_matrix <
				xablau::algebra::tensor_dense_fixed <
					float,
					xablau::algebra::tensor_fixed_dimensionalities < 10, 10 > >,
				true,
				true > ();

		const auto [dynamicMatrix, dynamicNodeToIndex, dynamicIndexToNode] =
			graph.template adjacency_matrix <
				xablau::algebra::tensor_dense_dynamic <
					float,
					xablau::algebra::tensor_rank < 2 > >,
				true,
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
				EXPECT_EQ(fixedIndexToNode.at(fixedIndex2), iterator2->first.get());
				EXPECT_EQ(dynamicIndexToNode.at(dynamicIndex1), iterator1->first);
				EXPECT_EQ(dynamicIndexToNode.at(dynamicIndex2), iterator2->first.get());

				EXPECT_EQ(fixedMatrix(fixedIndex1, fixedIndex2), dynamicMatrix(dynamicIndex1, dynamicIndex2));
				ASSERT_GT(fixedMatrix(fixedIndex1, fixedIndex2), float{});
			}
		}
	}

	TYPED_TEST_P(GraphGraphTest, LaplacianMatrix)
	{
		xablau::graph::graph <
			typename TypeParam::value_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > graph;

		this->completeGraph(graph);

		const auto [fixedMatrix, fixedNodeToIndex, fixedIndexToNode] =
			graph.template laplacian_matrix <
				xablau::algebra::tensor_dense_fixed <
					float,
					xablau::algebra::tensor_fixed_dimensionalities < 10, 10 > >,
				true > ();

		const auto [dynamicMatrix, dynamicNodeToIndex, dynamicIndexToNode] =
			graph.template laplacian_matrix <
				xablau::algebra::tensor_dense_dynamic <
					float,
					xablau::algebra::tensor_rank < 2 > >,
				true > ();

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
				EXPECT_EQ(fixedIndexToNode.at(fixedIndex2), iterator2->first.get());
				EXPECT_EQ(dynamicIndexToNode.at(dynamicIndex1), iterator1->first);
				EXPECT_EQ(dynamicIndexToNode.at(dynamicIndex2), iterator2->first.get());

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

	TYPED_TEST_P(GraphGraphTest, DijkstraAllPreviousAndNextNodes)
	{
		using GraphType =
			xablau::graph::graph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type >;

		using WeightType = typename TypeParam::edge_type::weight_type;

		GraphType graph;

		const auto value0 = this->createValue(0);
		const auto value1 = this->createValue(1);
		const auto value2 = this->createValue(2);
		const auto value3 = this->createValue(3);
		const auto value4 = this->createValue(4);
		const auto value5 = this->createValue(5);
		const auto value6 = this->createValue(6);
		const auto value7 = this->createValue(7);
		const auto value8 = this->createValue(8);
		const auto value9 = this->createValue(9);
		const auto value10 = this->createValue(10);
		const auto value11 = this->createValue(11);
		const auto value12 = this->createValue(12);

		graph.insert(value0, value1);
		graph.insert(value0, value2);
		graph.insert(value1, value3);
		graph.insert(value1, value4);
		graph.insert(value2, value4);
		graph.insert(value2, value5);
		graph.insert(value3, value6);
		graph.insert(value3, value7);
		graph.insert(value4, value7);
		graph.insert(value4, value8);
		graph.insert(value5, value8);
		graph.insert(value5, value9);
		graph.insert(value6, value10);
		graph.insert(value7, value10);
		graph.insert(value9, value12, WeightType{3});
		graph.insert(value10, value11);
		graph.insert(value11, value12);

		if constexpr (GraphType::can_have_multiple_edges())
		{
			graph.insert(value0, value1, WeightType{2});
			graph.insert(value0, value2, WeightType{2});
			graph.insert(value1, value3, WeightType{2});
			graph.insert(value1, value4, WeightType{2});
			graph.insert(value2, value4, WeightType{2});
			graph.insert(value2, value5, WeightType{2});
			graph.insert(value3, value6, WeightType{2});
			graph.insert(value3, value7, WeightType{2});
			graph.insert(value4, value7, WeightType{2});
			graph.insert(value4, value8, WeightType{2});
			graph.insert(value5, value8, WeightType{2});
			graph.insert(value5, value9, WeightType{2});
			graph.insert(value6, value10, WeightType{2});
			graph.insert(value7, value10, WeightType{2});
			graph.insert(value9, value12, WeightType{6});
			graph.insert(value10, value11, WeightType{2});
			graph.insert(value11, value12, WeightType{2});
		}

		const auto [previous, next] = graph.Dijkstra_all_previous_and_next_nodes < true > (value0);

		EXPECT_EQ(previous.at(value0).size(), 0);
		ASSERT_EQ(previous.at(value1).size(), 1);
		EXPECT_EQ(*(previous.at(value1).cbegin()), value0);
		ASSERT_EQ(previous.at(value2).size(), 1);
		EXPECT_EQ(*(previous.at(value2).cbegin()), value0);
		ASSERT_EQ(previous.at(value3).size(), 1);
		EXPECT_EQ(*(previous.at(value3).cbegin()), value1);
		ASSERT_EQ(previous.at(value4).size(), 2);
		EXPECT_EQ(std::min(*(previous.at(value4).cbegin()), *(++(previous.at(value4).cbegin()))), value1);
		EXPECT_EQ(std::max(*(previous.at(value4).cbegin()), *(++(previous.at(value4).cbegin()))), value2);
		ASSERT_EQ(previous.at(value5).size(), 1);
		EXPECT_EQ(*(previous.at(value5).cbegin()), value2);
		ASSERT_EQ(previous.at(value6).size(), 1);
		EXPECT_EQ(*(previous.at(value6).cbegin()), value3);
		ASSERT_EQ(previous.at(value7).size(), 2);
		EXPECT_EQ(std::min(*(previous.at(value7).cbegin()), *(++(previous.at(value7).cbegin()))), value3);
		EXPECT_EQ(std::max(*(previous.at(value7).cbegin()), *(++(previous.at(value7).cbegin()))), value4);
		ASSERT_EQ(previous.at(value8).size(), 2);
		EXPECT_EQ(std::min(*(previous.at(value8).cbegin()), *(++(previous.at(value8).cbegin()))), value4);
		EXPECT_EQ(std::max(*(previous.at(value8).cbegin()), *(++(previous.at(value8).cbegin()))), value5);
		ASSERT_EQ(previous.at(value9).size(), 1);
		EXPECT_EQ(*(previous.at(value9).cbegin()), value5);
		ASSERT_EQ(previous.at(value10).size(), 2);
		EXPECT_EQ(std::min(*(previous.at(value10).cbegin()), *(++(previous.at(value10).cbegin()))), value6);
		EXPECT_EQ(std::max(*(previous.at(value10).cbegin()), *(++(previous.at(value10).cbegin()))), value7);
		ASSERT_EQ(previous.at(value11).size(), 1);
		EXPECT_EQ(*(previous.at(value11).cbegin()), value10);
		ASSERT_EQ(previous.at(value12).size(), 2);
		EXPECT_EQ(std::min(*(previous.at(value12).cbegin()), *(++(previous.at(value12).cbegin()))), value9);
		EXPECT_EQ(std::max(*(previous.at(value12).cbegin()), *(++(previous.at(value12).cbegin()))), value11);

		ASSERT_EQ(next.at(value0).size(), 2);
		EXPECT_EQ(std::min(*(next.at(value0).cbegin()), *(++(next.at(value0).cbegin()))), value1);
		EXPECT_EQ(std::max(*(next.at(value0).cbegin()), *(++(next.at(value0).cbegin()))), value2);
		ASSERT_EQ(next.at(value1).size(), 2);
		EXPECT_EQ(std::min(*(next.at(value1).cbegin()), *(++(next.at(value1).cbegin()))), value3);
		EXPECT_EQ(std::max(*(next.at(value1).cbegin()), *(++(next.at(value1).cbegin()))), value4);
		ASSERT_EQ(next.at(value2).size(), 2);
		EXPECT_EQ(std::min(*(next.at(value2).cbegin()), *(++(next.at(value2).cbegin()))), value4);
		EXPECT_EQ(std::max(*(next.at(value2).cbegin()), *(++(next.at(value2).cbegin()))), value5);
		ASSERT_EQ(next.at(value3).size(), 2);
		EXPECT_EQ(std::min(*(next.at(value3).cbegin()), *(++(next.at(value3).cbegin()))), value6);
		EXPECT_EQ(std::max(*(next.at(value3).cbegin()), *(++(next.at(value3).cbegin()))), value7);
		ASSERT_EQ(next.at(value4).size(), 2);
		EXPECT_EQ(std::min(*(next.at(value4).cbegin()), *(++(next.at(value4).cbegin()))), value7);
		EXPECT_EQ(std::max(*(next.at(value4).cbegin()), *(++(next.at(value4).cbegin()))), value8);
		ASSERT_EQ(next.at(value5).size(), 2);
		EXPECT_EQ(std::min(*(next.at(value5).cbegin()), *(++(next.at(value5).cbegin()))), value8);
		EXPECT_EQ(std::max(*(next.at(value5).cbegin()), *(++(next.at(value5).cbegin()))), value9);
		ASSERT_EQ(next.at(value6).size(), 1);
		EXPECT_EQ(*(next.at(value6).cbegin()), value10);
		ASSERT_EQ(next.at(value7).size(), 1);
		EXPECT_EQ(*(next.at(value7).cbegin()), value10);
		EXPECT_EQ(next.at(value8).size(), 0);
		ASSERT_EQ(next.at(value9).size(), 1);
		EXPECT_EQ(*(next.at(value9).cbegin()), value12);
		ASSERT_EQ(next.at(value10).size(), 1);
		EXPECT_EQ(*(next.at(value10).cbegin()), value11);
		ASSERT_EQ(next.at(value11).size(), 1);
		EXPECT_EQ(*(next.at(value11).cbegin()), value12);
		EXPECT_EQ(next.at(value12).size(), 0);
	}

	TYPED_TEST_P(GraphGraphTest, DijkstraAllPreviousAndNextNodesAndDistance)
	{
		using GraphType =
			xablau::graph::graph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type >;

		using WeightType = typename TypeParam::edge_type::weight_type;

		GraphType graph;

		const auto value0 = this->createValue(0);
		const auto value1 = this->createValue(1);
		const auto value2 = this->createValue(2);
		const auto value3 = this->createValue(3);
		const auto value4 = this->createValue(4);
		const auto value5 = this->createValue(5);
		const auto value6 = this->createValue(6);
		const auto value7 = this->createValue(7);
		const auto value8 = this->createValue(8);
		const auto value9 = this->createValue(9);
		const auto value10 = this->createValue(10);
		const auto value11 = this->createValue(11);
		const auto value12 = this->createValue(12);

		graph.insert(value0, value1);
		graph.insert(value0, value2);
		graph.insert(value1, value3);
		graph.insert(value1, value4);
		graph.insert(value2, value4);
		graph.insert(value2, value5);
		graph.insert(value3, value6);
		graph.insert(value3, value7);
		graph.insert(value4, value7);
		graph.insert(value4, value8);
		graph.insert(value5, value8);
		graph.insert(value5, value9);
		graph.insert(value6, value10);
		graph.insert(value7, value10);
		graph.insert(value9, value12, WeightType{3});
		graph.insert(value10, value11);
		graph.insert(value11, value12);

		if constexpr (GraphType::can_have_multiple_edges())
		{
			graph.insert(value0, value1, WeightType{2});
			graph.insert(value0, value2, WeightType{2});
			graph.insert(value1, value3, WeightType{2});
			graph.insert(value1, value4, WeightType{2});
			graph.insert(value2, value4, WeightType{2});
			graph.insert(value2, value5, WeightType{2});
			graph.insert(value3, value6, WeightType{2});
			graph.insert(value3, value7, WeightType{2});
			graph.insert(value4, value7, WeightType{2});
			graph.insert(value4, value8, WeightType{2});
			graph.insert(value5, value8, WeightType{2});
			graph.insert(value5, value9, WeightType{2});
			graph.insert(value6, value10, WeightType{2});
			graph.insert(value7, value10, WeightType{2});
			graph.insert(value9, value12, WeightType{6});
			graph.insert(value10, value11, WeightType{2});
			graph.insert(value11, value12, WeightType{2});
		}

		const auto [previous, next] = graph.Dijkstra_all_previous_and_next_nodes_and_distance < true > (value0);

		EXPECT_EQ(previous.at(value0).first.size(), 0);
		EXPECT_EQ(previous.at(value0).second, WeightType{});
		ASSERT_EQ(previous.at(value1).first.size(), 1);
		EXPECT_EQ(*(previous.at(value1).first.cbegin()), value0);
		EXPECT_EQ(previous.at(value1).second, WeightType{1});
		ASSERT_EQ(previous.at(value2).first.size(), 1);
		EXPECT_EQ(*(previous.at(value2).first.cbegin()), value0);
		EXPECT_EQ(previous.at(value2).second, WeightType{1});
		ASSERT_EQ(previous.at(value3).first.size(), 1);
		EXPECT_EQ(*(previous.at(value3).first.cbegin()), value1);
		EXPECT_EQ(previous.at(value3).second, WeightType{2});
		ASSERT_EQ(previous.at(value4).first.size(), 2);
		EXPECT_EQ(std::min(*(previous.at(value4).first.cbegin()), *(++(previous.at(value4).first.cbegin()))), value1);
		EXPECT_EQ(std::max(*(previous.at(value4).first.cbegin()), *(++(previous.at(value4).first.cbegin()))), value2);
		EXPECT_EQ(previous.at(value4).second, WeightType{2});
		ASSERT_EQ(previous.at(value5).first.size(), 1);
		EXPECT_EQ(*(previous.at(value5).first.cbegin()), value2);
		EXPECT_EQ(previous.at(value5).second, WeightType{2});
		ASSERT_EQ(previous.at(value6).first.size(), 1);
		EXPECT_EQ(*(previous.at(value6).first.cbegin()), value3);
		EXPECT_EQ(previous.at(value6).second, WeightType{3});
		ASSERT_EQ(previous.at(value7).first.size(), 2);
		EXPECT_EQ(std::min(*(previous.at(value7).first.cbegin()), *(++(previous.at(value7).first.cbegin()))), value3);
		EXPECT_EQ(std::max(*(previous.at(value7).first.cbegin()), *(++(previous.at(value7).first.cbegin()))), value4);
		EXPECT_EQ(previous.at(value7).second, WeightType{3});
		ASSERT_EQ(previous.at(value8).first.size(), 2);
		EXPECT_EQ(std::min(*(previous.at(value8).first.cbegin()), *(++(previous.at(value8).first.cbegin()))), value4);
		EXPECT_EQ(std::max(*(previous.at(value8).first.cbegin()), *(++(previous.at(value8).first.cbegin()))), value5);
		EXPECT_EQ(previous.at(value8).second, WeightType{3});
		ASSERT_EQ(previous.at(value9).first.size(), 1);
		EXPECT_EQ(*(previous.at(value9).first.cbegin()), value5);
		EXPECT_EQ(previous.at(value9).second, WeightType{3});
		ASSERT_EQ(previous.at(value10).first.size(), 2);
		EXPECT_EQ(std::min(*(previous.at(value10).first.cbegin()), *(++(previous.at(value10).first.cbegin()))), value6);
		EXPECT_EQ(std::max(*(previous.at(value10).first.cbegin()), *(++(previous.at(value10).first.cbegin()))), value7);
		EXPECT_EQ(previous.at(value10).second, WeightType{4});
		ASSERT_EQ(previous.at(value11).first.size(), 1);
		EXPECT_EQ(*(previous.at(value11).first.cbegin()), value10);
		EXPECT_EQ(previous.at(value11).second, WeightType{5});
		ASSERT_EQ(previous.at(value12).first.size(), 2);
		EXPECT_EQ(std::min(*(previous.at(value12).first.cbegin()), *(++(previous.at(value12).first.cbegin()))), value9);
		EXPECT_EQ(std::max(*(previous.at(value12).first.cbegin()), *(++(previous.at(value12).first.cbegin()))), value11);
		EXPECT_EQ(previous.at(value12).second, WeightType{6});

		ASSERT_EQ(next.at(value0).size(), 2);
		EXPECT_EQ(std::min((next.at(value0).cbegin())->first, (++(next.at(value0).cbegin()))->first), value1);
		EXPECT_EQ(std::max((next.at(value0).cbegin())->first, (++(next.at(value0).cbegin()))->first), value2);
		EXPECT_EQ((next.at(value0).cbegin())->second, WeightType{1});
		EXPECT_EQ((++(next.at(value0).cbegin()))->second, WeightType{1});
		ASSERT_EQ(next.at(value1).size(), 2);
		EXPECT_EQ(std::min((next.at(value1).cbegin())->first, (++(next.at(value1).cbegin()))->first), value3);
		EXPECT_EQ(std::max((next.at(value1).cbegin())->first, (++(next.at(value1).cbegin()))->first), value4);
		EXPECT_EQ((next.at(value1).cbegin())->second, WeightType{2});
		EXPECT_EQ((++(next.at(value1).cbegin()))->second, WeightType{2});
		ASSERT_EQ(next.at(value2).size(), 2);
		EXPECT_EQ(std::min((next.at(value2).cbegin())->first, (++(next.at(value2).cbegin()))->first), value4);
		EXPECT_EQ(std::max((next.at(value2).cbegin())->first, (++(next.at(value2).cbegin()))->first), value5);
		EXPECT_EQ((next.at(value2).cbegin())->second, WeightType{2});
		EXPECT_EQ((++(next.at(value2).cbegin()))->second, WeightType{2});
		ASSERT_EQ(next.at(value3).size(), 2);
		EXPECT_EQ(std::min((next.at(value3).cbegin())->first, (++(next.at(value3).cbegin()))->first), value6);
		EXPECT_EQ(std::max((next.at(value3).cbegin())->first, (++(next.at(value3).cbegin()))->first), value7);
		EXPECT_EQ((next.at(value3).cbegin())->second, WeightType{3});
		EXPECT_EQ((++(next.at(value3).cbegin()))->second, WeightType{3});
		ASSERT_EQ(next.at(value4).size(), 2);
		EXPECT_EQ(std::min((next.at(value4).cbegin())->first, (++(next.at(value4).cbegin()))->first), value7);
		EXPECT_EQ(std::max((next.at(value4).cbegin())->first, (++(next.at(value4).cbegin()))->first), value8);
		EXPECT_EQ((next.at(value4).cbegin())->second, WeightType{3});
		EXPECT_EQ((++(next.at(value4).cbegin()))->second, WeightType{3});
		ASSERT_EQ(next.at(value5).size(), 2);
		EXPECT_EQ(std::min((next.at(value5).cbegin())->first, (++(next.at(value5).cbegin()))->first), value8);
		EXPECT_EQ(std::max((next.at(value5).cbegin())->first, (++(next.at(value5).cbegin()))->first), value9);
		EXPECT_EQ((next.at(value5).cbegin())->second, WeightType{3});
		EXPECT_EQ((++(next.at(value5).cbegin()))->second, WeightType{3});
		ASSERT_EQ(next.at(value6).size(), 1);
		EXPECT_EQ((next.at(value6).cbegin())->first, value10);
		EXPECT_EQ((next.at(value6).cbegin())->second, WeightType{4});
		ASSERT_EQ(next.at(value7).size(), 1);
		EXPECT_EQ((next.at(value7).cbegin())->first, value10);
		EXPECT_EQ((next.at(value7).cbegin())->second, WeightType{4});
		EXPECT_EQ(next.at(value8).size(), 0);
		ASSERT_EQ(next.at(value9).size(), 1);
		EXPECT_EQ((next.at(value9).cbegin())->first, value12);
		EXPECT_EQ((next.at(value9).cbegin())->second, WeightType{6});
		ASSERT_EQ(next.at(value10).size(), 1);
		EXPECT_EQ((next.at(value10).cbegin())->first, value11);
		EXPECT_EQ((next.at(value10).cbegin())->second, WeightType{5});
		ASSERT_EQ(next.at(value11).size(), 1);
		EXPECT_EQ((next.at(value11).cbegin())->first, value12);
		EXPECT_EQ((next.at(value11).cbegin())->second, WeightType{6});
		EXPECT_EQ(next.at(value12).size(), 0);
	}

	TYPED_TEST_P(GraphGraphTest, DijkstraSinglePreviousAndNextNodes)
	{
		using GraphType =
			xablau::graph::graph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type >;

		using WeightType = typename TypeParam::edge_type::weight_type;

		GraphType graph;

		const auto value0 = this->createValue(0);
		const auto value1 = this->createValue(1);
		const auto value2 = this->createValue(2);
		const auto value3 = this->createValue(3);
		const auto value4 = this->createValue(4);
		const auto value5 = this->createValue(5);
		const auto value6 = this->createValue(6);
		const auto value7 = this->createValue(7);
		const auto value8 = this->createValue(8);
		const auto value9 = this->createValue(9);
		const auto value10 = this->createValue(10);
		const auto value11 = this->createValue(11);
		const auto value12 = this->createValue(12);

		graph.insert(value0, value1);
		graph.insert(value0, value2);
		graph.insert(value1, value3);
		graph.insert(value1, value4);
		graph.insert(value2, value4);
		graph.insert(value2, value5);
		graph.insert(value3, value6);
		graph.insert(value3, value7);
		graph.insert(value4, value7);
		graph.insert(value4, value8);
		graph.insert(value5, value8);
		graph.insert(value5, value9);
		graph.insert(value6, value10);
		graph.insert(value7, value10);
		graph.insert(value9, value12, WeightType{3});
		graph.insert(value10, value11);
		graph.insert(value11, value12);

		if constexpr (GraphType::can_have_multiple_edges())
		{
			graph.insert(value0, value1, WeightType{2});
			graph.insert(value0, value2, WeightType{2});
			graph.insert(value1, value3, WeightType{2});
			graph.insert(value1, value4, WeightType{2});
			graph.insert(value2, value4, WeightType{2});
			graph.insert(value2, value5, WeightType{2});
			graph.insert(value3, value6, WeightType{2});
			graph.insert(value3, value7, WeightType{2});
			graph.insert(value4, value7, WeightType{2});
			graph.insert(value4, value8, WeightType{2});
			graph.insert(value5, value8, WeightType{2});
			graph.insert(value5, value9, WeightType{2});
			graph.insert(value6, value10, WeightType{2});
			graph.insert(value7, value10, WeightType{2});
			graph.insert(value9, value12, WeightType{6});
			graph.insert(value10, value11, WeightType{2});
			graph.insert(value11, value12, WeightType{2});
		}

		const auto [previous, next] = graph.Dijkstra_single_previous_and_next_nodes < true > (value0);

		EXPECT_EQ(previous.at(value1), value0);
		EXPECT_EQ(previous.at(value2), value0);
		EXPECT_EQ(previous.at(value3), value1);
		EXPECT_TRUE(previous.at(value4) == value1 || previous.at(value4) == value2);
		EXPECT_EQ(previous.at(value5), value2);
		EXPECT_EQ(previous.at(value6), value3);
		EXPECT_TRUE(previous.at(value7) == value3 || previous.at(value7) == value4);
		EXPECT_TRUE(previous.at(value8) == value4 || previous.at(value8) == value5);
		EXPECT_EQ(previous.at(value9), value5);
		EXPECT_TRUE(previous.at(value10) == value6 || previous.at(value10) == value7);
		EXPECT_EQ(previous.at(value11), value10);
		EXPECT_TRUE(previous.at(value12) == value9 || previous.at(value12) == value11);

		ASSERT_EQ(next.at(value0).size(), 2);
		EXPECT_EQ(std::min(*(next.at(value0).cbegin()), *(++(next.at(value0).cbegin()))), value1);
		EXPECT_EQ(std::max(*(next.at(value0).cbegin()), *(++(next.at(value0).cbegin()))), value2);

		if (next.at(value1).size() == 2)
		{
			EXPECT_EQ(std::min(*(next.at(value1).cbegin()), *(++(next.at(value1).cbegin()))), value3);
			EXPECT_EQ(std::max(*(next.at(value1).cbegin()), *(++(next.at(value1).cbegin()))), value4);
			ASSERT_EQ(next.at(value2).size(), 1);
			EXPECT_EQ(*(next.at(value2).cbegin()), value5);
		}

		else
		{
			ASSERT_EQ(next.at(value2).size(), 2);
			EXPECT_EQ(std::min(*(next.at(value2).cbegin()), *(++(next.at(value2).cbegin()))), value4);
			EXPECT_EQ(std::max(*(next.at(value2).cbegin()), *(++(next.at(value2).cbegin()))), value5);
			ASSERT_EQ(next.at(value1).size(), 1);
			EXPECT_EQ(*(next.at(value1).cbegin()), value3);
		}

		bool Value4HasValue7 = false;
		bool Value4HasValue8 = false;

		if (next.at(value3).size() == 2)
		{
			EXPECT_EQ(std::min(*(next.at(value3).cbegin()), *(++(next.at(value3).cbegin()))), value6);
			EXPECT_EQ(std::max(*(next.at(value3).cbegin()), *(++(next.at(value3).cbegin()))), value7);
		}

		else if (next.at(value3).size() == 1)
		{
			Value4HasValue7 = true;
			EXPECT_EQ(*(next.at(value3).cbegin()), value6);
		}

		else
		{
			FAIL();
		}

		if (next.at(value5).size() == 2)
		{
			EXPECT_EQ(std::min(*(next.at(value5).cbegin()), *(++(next.at(value5).cbegin()))), value8);
			EXPECT_EQ(std::max(*(next.at(value5).cbegin()), *(++(next.at(value5).cbegin()))), value9);
		}

		else if (next.at(value5).size() == 1)
		{
			Value4HasValue8 = true;
			EXPECT_EQ(*(next.at(value5).cbegin()), value9);
		}

		else
		{
			FAIL();
		}

		if (Value4HasValue7 && Value4HasValue8)
		{
			ASSERT_EQ(next.at(value4).size(), 2);
			EXPECT_EQ(std::min(*(next.at(value4).cbegin()), *(++(next.at(value4).cbegin()))), value7);
			EXPECT_EQ(std::max(*(next.at(value4).cbegin()), *(++(next.at(value4).cbegin()))), value8);
		}

		else if (Value4HasValue7)
		{
			ASSERT_EQ(next.at(value4).size(), 1);
			EXPECT_EQ(*(next.at(value4).cbegin()), value7);
		}

		else if (Value4HasValue8)
		{
			ASSERT_EQ(next.at(value4).size(), 1);
			EXPECT_EQ(*(next.at(value4).cbegin()), value8);
		}

		else
		{
			EXPECT_EQ(next.at(value4).size(), 0);
		}

		if (next.at(value6).size() == 1)
		{
			EXPECT_EQ(*(next.at(value6).cbegin()), value10);
			EXPECT_EQ(next.at(value7).size(), 0);
		}

		else
		{
			EXPECT_EQ(next.at(value6).size(), 0);
			ASSERT_EQ(next.at(value7).size(), 1);
			EXPECT_EQ(*(next.at(value7).cbegin()), value10);
		}

		EXPECT_EQ(next.at(value8).size(), 0);

		if (next.at(value9).size() == 1)
		{
			EXPECT_EQ(*(next.at(value9).cbegin()), value12);
		}

		else
		{
			EXPECT_EQ(next.at(value9).size(), 0);
			ASSERT_EQ(next.at(value11).size(), 1);
			EXPECT_EQ(*(next.at(value11).cbegin()), value12);
		}

		ASSERT_EQ(next.at(value10).size(), 1);
		EXPECT_EQ(*(next.at(value10).cbegin()), value11);
		EXPECT_EQ(next.at(value12).size(), 0);
	}

	TYPED_TEST_P(GraphGraphTest, DijkstraSinglePreviousAndNextNodesAndDistance)
	{
		using GraphType =
			xablau::graph::graph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type >;

		using WeightType = typename TypeParam::edge_type::weight_type;

		GraphType graph;

		const auto value0 = this->createValue(0);
		const auto value1 = this->createValue(1);
		const auto value2 = this->createValue(2);
		const auto value3 = this->createValue(3);
		const auto value4 = this->createValue(4);
		const auto value5 = this->createValue(5);
		const auto value6 = this->createValue(6);
		const auto value7 = this->createValue(7);
		const auto value8 = this->createValue(8);
		const auto value9 = this->createValue(9);
		const auto value10 = this->createValue(10);
		const auto value11 = this->createValue(11);
		const auto value12 = this->createValue(12);

		graph.insert(value0, value1);
		graph.insert(value0, value2);
		graph.insert(value1, value3);
		graph.insert(value1, value4);
		graph.insert(value2, value4);
		graph.insert(value2, value5);
		graph.insert(value3, value6);
		graph.insert(value3, value7);
		graph.insert(value4, value7);
		graph.insert(value4, value8);
		graph.insert(value5, value8);
		graph.insert(value5, value9);
		graph.insert(value6, value10);
		graph.insert(value7, value10);
		graph.insert(value9, value12, WeightType{3});
		graph.insert(value10, value11);
		graph.insert(value11, value12);

		if constexpr (GraphType::can_have_multiple_edges())
		{
			graph.insert(value0, value1, WeightType{2});
			graph.insert(value0, value2, WeightType{2});
			graph.insert(value1, value3, WeightType{2});
			graph.insert(value1, value4, WeightType{2});
			graph.insert(value2, value4, WeightType{2});
			graph.insert(value2, value5, WeightType{2});
			graph.insert(value3, value6, WeightType{2});
			graph.insert(value3, value7, WeightType{2});
			graph.insert(value4, value7, WeightType{2});
			graph.insert(value4, value8, WeightType{2});
			graph.insert(value5, value8, WeightType{2});
			graph.insert(value5, value9, WeightType{2});
			graph.insert(value6, value10, WeightType{2});
			graph.insert(value7, value10, WeightType{2});
			graph.insert(value9, value12, WeightType{6});
			graph.insert(value10, value11, WeightType{2});
			graph.insert(value11, value12, WeightType{2});
		}

		const auto [previous, next] = graph.Dijkstra_single_previous_and_next_nodes_and_distance < true > (value0);

		EXPECT_EQ(previous.at(value1).first, value0);
		EXPECT_EQ(previous.at(value1).second, WeightType{1});
		EXPECT_EQ(previous.at(value2).first, value0);
		EXPECT_EQ(previous.at(value2).second, WeightType{1});
		EXPECT_EQ(previous.at(value3).first, value1);
		EXPECT_EQ(previous.at(value3).second, WeightType{2});
		EXPECT_TRUE(previous.at(value4).first == value1 || previous.at(value4).first == value2);
		EXPECT_EQ(previous.at(value4).second, WeightType{2});
		EXPECT_EQ(previous.at(value5).first, value2);
		EXPECT_EQ(previous.at(value5).second, WeightType{2});
		EXPECT_EQ(previous.at(value6).first, value3);
		EXPECT_EQ(previous.at(value6).second, WeightType{3});
		EXPECT_TRUE(previous.at(value7).first == value3 || previous.at(value7).first == value4);
		EXPECT_EQ(previous.at(value7).second, WeightType{3});
		EXPECT_TRUE(previous.at(value8).first == value4 || previous.at(value8).first == value5);
		EXPECT_EQ(previous.at(value8).second, WeightType{3});
		EXPECT_EQ(previous.at(value9).first, value5);
		EXPECT_EQ(previous.at(value9).second, WeightType{3});
		EXPECT_TRUE(previous.at(value10).first == value6 || previous.at(value10).first == value7);
		EXPECT_EQ(previous.at(value10).second, WeightType{4});
		EXPECT_EQ(previous.at(value11).first, value10);
		EXPECT_EQ(previous.at(value11).second, WeightType{5});
		EXPECT_TRUE(previous.at(value12).first == value9 || previous.at(value12).first == value11);
		EXPECT_EQ(previous.at(value12).second, WeightType{6});

		ASSERT_EQ(next.at(value0).size(), 2);
		EXPECT_EQ(std::min((next.at(value0).cbegin())->first, (++(next.at(value0).cbegin()))->first), value1);
		EXPECT_EQ(std::max((next.at(value0).cbegin())->first, (++(next.at(value0).cbegin()))->first), value2);
		EXPECT_EQ((next.at(value0).cbegin())->second, WeightType{1});
		EXPECT_EQ((++(next.at(value0).cbegin()))->second, WeightType{1});

		if (next.at(value1).size() == 2)
		{
			EXPECT_EQ(std::min((next.at(value1).cbegin())->first, (++(next.at(value1).cbegin()))->first), value3);
			EXPECT_EQ(std::max((next.at(value1).cbegin())->first, (++(next.at(value1).cbegin()))->first), value4);
			EXPECT_EQ((next.at(value1).cbegin())->second, WeightType{2});
			EXPECT_EQ((++(next.at(value1).cbegin()))->second, WeightType{2});
			ASSERT_EQ(next.at(value2).size(), 1);
			EXPECT_EQ((next.at(value2).cbegin())->first, value5);
			EXPECT_EQ((next.at(value2).cbegin())->second, WeightType{2});
		}

		else
		{
			ASSERT_EQ(next.at(value2).size(), 2);
			EXPECT_EQ(std::min((next.at(value2).cbegin())->first, (++(next.at(value2).cbegin()))->first), value4);
			EXPECT_EQ(std::max((next.at(value2).cbegin())->first, (++(next.at(value2).cbegin()))->first), value5);
			EXPECT_EQ((next.at(value2).cbegin())->second, WeightType{2});
			EXPECT_EQ((++(next.at(value2).cbegin()))->second, WeightType{2});
			ASSERT_EQ(next.at(value1).size(), 1);
			EXPECT_EQ((next.at(value1).cbegin())->first, value3);
			EXPECT_EQ((next.at(value1).cbegin())->second, WeightType{2});
		}

		bool Value4HasValue7 = false;
		bool Value4HasValue8 = false;

		if (next.at(value3).size() == 2)
		{
			EXPECT_EQ(std::min((next.at(value3).cbegin())->first, (++(next.at(value3).cbegin()))->first), value6);
			EXPECT_EQ(std::max((next.at(value3).cbegin())->first, (++(next.at(value3).cbegin()))->first), value7);
			EXPECT_EQ((next.at(value3).cbegin())->second, WeightType{3});
			EXPECT_EQ((++(next.at(value3).cbegin()))->second, WeightType{3});
		}

		else if (next.at(value3).size() == 1)
		{
			Value4HasValue7 = true;
			EXPECT_EQ((next.at(value3).cbegin())->first, value6);
			EXPECT_EQ((next.at(value3).cbegin())->second, WeightType{3});
		}

		else
		{
			FAIL();
		}

		if (next.at(value5).size() == 2)
		{
			EXPECT_EQ(std::min((next.at(value5).cbegin())->first, (++(next.at(value5).cbegin()))->first), value8);
			EXPECT_EQ(std::max((next.at(value5).cbegin())->first, (++(next.at(value5).cbegin()))->first), value9);
			EXPECT_EQ((next.at(value5).cbegin())->second, WeightType{3});
			EXPECT_EQ((++(next.at(value5).cbegin()))->second, WeightType{3});
		}

		else if (next.at(value5).size() == 1)
		{
			Value4HasValue8 = true;
			EXPECT_EQ((next.at(value5).cbegin())->first, value9);
			EXPECT_EQ((next.at(value5).cbegin())->second, WeightType{3});
		}

		else
		{
			FAIL();
		}

		if (Value4HasValue7 && Value4HasValue8)
		{
			ASSERT_EQ(next.at(value4).size(), 2);
			EXPECT_EQ(std::min((next.at(value4).cbegin())->first, (++(next.at(value4).cbegin()))->first), value7);
			EXPECT_EQ(std::max((next.at(value4).cbegin())->first, (++(next.at(value4).cbegin()))->first), value8);
			EXPECT_EQ((next.at(value4).cbegin())->second, WeightType{3});
			EXPECT_EQ((++(next.at(value4).cbegin()))->second, WeightType{3});
		}

		else if (Value4HasValue7)
		{
			ASSERT_EQ(next.at(value4).size(), 1);
			EXPECT_EQ((next.at(value4).cbegin())->first, value7);
			EXPECT_EQ((next.at(value4).cbegin())->second, WeightType{3});
		}

		else if (Value4HasValue8)
		{
			ASSERT_EQ(next.at(value4).size(), 1);
			EXPECT_EQ((next.at(value4).cbegin())->first, value8);
			EXPECT_EQ((next.at(value4).cbegin())->second, WeightType{3});
		}

		else
		{
			EXPECT_EQ(next.at(value4).size(), 0);
		}

		if (next.at(value6).size() == 1)
		{
			EXPECT_EQ((next.at(value6).cbegin())->first, value10);
			EXPECT_EQ((next.at(value6).cbegin())->second, WeightType{4});
			EXPECT_EQ(next.at(value7).size(), 0);
		}

		else
		{
			EXPECT_EQ(next.at(value6).size(), 0);
			ASSERT_EQ(next.at(value7).size(), 1);
			EXPECT_EQ((next.at(value7).cbegin())->first, value10);
			EXPECT_EQ((next.at(value7).cbegin())->second, WeightType{4});
		}

		EXPECT_EQ(next.at(value8).size(), 0);

		if (next.at(value9).size() == 1)
		{
			EXPECT_EQ((next.at(value9).cbegin())->first, value12);
			EXPECT_EQ((next.at(value9).cbegin())->second, WeightType{6});
		}

		else
		{
			EXPECT_EQ(next.at(value9).size(), 0);
			ASSERT_EQ(next.at(value11).size(), 1);
			EXPECT_EQ((next.at(value11).cbegin())->first, value12);
			EXPECT_EQ((next.at(value11).cbegin())->second, WeightType{6});
		}

		ASSERT_EQ(next.at(value10).size(), 1);
		EXPECT_EQ((next.at(value10).cbegin())->first, value11);
		EXPECT_EQ((next.at(value10).cbegin())->second, WeightType{5});
		EXPECT_EQ(next.at(value12).size(), 0);
	}

	TYPED_TEST_P(GraphGraphTest, DijkstraSinglePath)
	{
		using GraphType =
			xablau::graph::graph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type >;

		using WeightType = typename TypeParam::edge_type::weight_type;

		GraphType graph;

		const auto value0 = this->createValue(0);
		const auto value1 = this->createValue(1);
		const auto value2 = this->createValue(2);
		const auto value3 = this->createValue(3);
		const auto value4 = this->createValue(4);
		const auto value5 = this->createValue(5);
		const auto value6 = this->createValue(6);
		const auto value7 = this->createValue(7);
		const auto value8 = this->createValue(8);
		const auto value9 = this->createValue(9);
		const auto value10 = this->createValue(10);
		const auto value11 = this->createValue(11);
		const auto value12 = this->createValue(12);

		graph.insert(value0, value1);
		graph.insert(value0, value2);
		graph.insert(value1, value3);
		graph.insert(value1, value4);
		graph.insert(value2, value4);
		graph.insert(value2, value5);
		graph.insert(value3, value6);
		graph.insert(value3, value7);
		graph.insert(value4, value7);
		graph.insert(value4, value8);
		graph.insert(value5, value8);
		graph.insert(value5, value9);
		graph.insert(value6, value10);
		graph.insert(value7, value10);
		graph.insert(value9, value12, WeightType{3});
		graph.insert(value10, value11);
		graph.insert(value11, value12);

		if constexpr (GraphType::can_have_multiple_edges())
		{
			graph.insert(value0, value1, WeightType{2});
			graph.insert(value0, value2, WeightType{2});
			graph.insert(value1, value3, WeightType{2});
			graph.insert(value1, value4, WeightType{2});
			graph.insert(value2, value4, WeightType{2});
			graph.insert(value2, value5, WeightType{2});
			graph.insert(value3, value6, WeightType{2});
			graph.insert(value3, value7, WeightType{2});
			graph.insert(value4, value7, WeightType{2});
			graph.insert(value4, value8, WeightType{2});
			graph.insert(value5, value8, WeightType{2});
			graph.insert(value5, value9, WeightType{2});
			graph.insert(value6, value10, WeightType{2});
			graph.insert(value7, value10, WeightType{2});
			graph.insert(value9, value12, WeightType{6});
			graph.insert(value10, value11, WeightType{2});
			graph.insert(value11, value12, WeightType{2});
		}

		const auto [distance, path] = graph.Dijkstra_single_path < true, true > (value0, value12);

		EXPECT_EQ(distance, WeightType{6});
		EXPECT_TRUE(
			path == std::vector < typename TypeParam::value_type > ({ value0, value1, value3, value6, value10, value11, value12 }) ||
			path == std::vector < typename TypeParam::value_type > ({ value0, value1, value3, value7, value10, value11, value12 }) ||
			path == std::vector < typename TypeParam::value_type > ({ value0, value1, value4, value7, value10, value11, value12 }) ||
			path == std::vector < typename TypeParam::value_type > ({ value0, value2, value4, value7, value10, value11, value12 }) ||
			path == std::vector < typename TypeParam::value_type > ({ value0, value2, value5, value9, value12 }));
	}

	TYPED_TEST_P(GraphGraphTest, FloydWarshall)
	{
		xablau::graph::graph <
			typename TypeParam::value_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > graph;

		this->completeGraph(graph);

		const auto [fixedMatrix, fixedNextNode, fixedNodeToIndex, fixedIndexToNode] =
			graph.template Floyd_Warshall <
				xablau::algebra::tensor_dense_fixed <
					float,
					xablau::algebra::tensor_fixed_dimensionalities < 10, 10 > >,
				true > ();

		const auto [dynamicMatrix, dynamicNextNode, dynamicNodeToIndex, dynamicIndexToNode] =
			graph.template Floyd_Warshall <
				xablau::algebra::tensor_dense_dynamic <
					float,
					xablau::algebra::tensor_rank < 2 > >,
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
				EXPECT_EQ(fixedIndexToNode.at(fixedIndex2), iterator2->first.get());
				EXPECT_EQ(dynamicIndexToNode.at(dynamicIndex1), iterator1->first);
				EXPECT_EQ(dynamicIndexToNode.at(dynamicIndex2), iterator2->first.get());

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
						EXPECT_EQ(fixedNextNode(fixedIndex1, fixedIndex2).value(), iterator2->first.get());
						EXPECT_EQ(dynamicNextNode(fixedIndex1, fixedIndex2).value(), iterator2->first.get());
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

					EXPECT_EQ(fixedNextNode(fixedIndex1, fixedIndex2).value(), iterator2->first.get());
					EXPECT_EQ(dynamicNextNode(fixedIndex1, fixedIndex2).value(), iterator2->first.get());
				}
			}
		}
	}

	TYPED_TEST_P(GraphGraphTest, TravelingSalesmanProblemFewNodes)
	{
		if constexpr (
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::ordered ||
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::unordered)
		{
			xablau::graph::graph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type > graph;

			const auto firstNode = this->createValue(0);
			const auto secondNode = this->createValue(1);

			auto result = graph.traveling_salesman_problem < true > (firstNode);

			EXPECT_EQ(result.first.size(), 0);
			EXPECT_EQ(result.second, float{});

			graph.insert(firstNode);

			result = graph.traveling_salesman_problem < true > (firstNode);

			ASSERT_EQ(result.first.size(), 1);
			EXPECT_EQ(*(result.first.begin()), firstNode);
			EXPECT_EQ(result.second, float{});

			graph.insert(firstNode, secondNode);

			result = graph.traveling_salesman_problem < true > (firstNode);

			ASSERT_EQ(result.first.size(), 2);
			EXPECT_EQ(*(result.first.begin()), firstNode);
			EXPECT_EQ(*(result.first.begin() + 1), secondNode);
			EXPECT_EQ(result.second, float{1});

			result = graph.traveling_salesman_problem < true > (secondNode);

			ASSERT_EQ(result.first.size(), 2);
			EXPECT_EQ(*(result.first.begin()), secondNode);
			EXPECT_EQ(*(result.first.begin() + 1), firstNode);
			EXPECT_EQ(result.second, float{1});
		}
	}

	TYPED_TEST_P(GraphGraphTest, TravelingSalesmanProblemDisjoint)
	{
		if constexpr (
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::ordered ||
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::unordered)
		{
			xablau::graph::graph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type > graph;

			graph.insert(this->createValue(0), this->createValue(1));
			graph.insert(this->createValue(1), this->createValue(2));
			graph.insert(this->createValue(2), this->createValue(0));
			graph.insert(this->createValue(3));

			for (size_t i = 0; i < 4; i++)
			{
				const auto result = graph.traveling_salesman_problem < true > (this->createValue(i));

				EXPECT_EQ(result.first.size(), 0);
				EXPECT_EQ(result.second, float{});
			}
		}
	}

	TYPED_TEST_P(GraphGraphTest, TravelingSalesmanProblemRegular)
	{
		if constexpr (
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::ordered ||
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::unordered)
		{
			xablau::graph::graph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type > graph;

			const float value1{1.17557f};
			const float value2{1.90211f};

			const auto node1 = this->createValue(0);
			const auto node2 = this->createValue(1);
			const auto node3 = this->createValue(2);
			const auto node4 = this->createValue(3);
			const auto node5 = this->createValue(4);

			graph.insert(node1, node2, value1);
			graph.insert(node2, node3, value1);
			graph.insert(node3, node4, value1);
			graph.insert(node4, node5, value1);

			graph.insert(node1, node3, value2);
			graph.insert(node2, node4, value2);

			std::vector < std::vector < typename TypeParam::value_type > > paths =
				{
					{ node1, node2, node3, node4, node5 },
					{ node2, node1, node3, node4, node5 },
					{ node3, node1, node2, node4, node5 },
					{},
					{ node5, node4, node3, node2, node1 }
				};

			std::vector < float > distances =
				{
					float{4} * value1,
					float{3} * value1 + value2,
					float{2} * value1 + float{2} * value2,
					float{},
					float{4} * value1
				};

			constexpr auto comparison =
				[] (const std::vector < typename TypeParam::value_type > &calculated,
					const std::vector < typename TypeParam::value_type > &result) -> bool
				{
					for (size_t i = 0; i < result.size(); i++)
					{
						if (calculated[i] != result[i])
						{
							return false;
						}
					}

					return true;
				};

			for (size_t i = 0; i < 5; i++)
			{
				const auto result = graph.traveling_salesman_problem < true > (this->createValue(i));

				ASSERT_EQ(result.first.size(), paths[i].size());

				EXPECT_TRUE(comparison(result.first, paths[i]));
			}
		}
	}

	TYPED_TEST_P(GraphGraphTest, TravelingSalesmanProblemWithDependenciesFewNodes)
	{
		if constexpr (
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::ordered ||
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::unordered)
		{
			xablau::graph::graph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type > graph;

			xablau::graph::digraph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type > dependencies;

			const auto firstNode = this->createValue(0);
			const auto secondNode = this->createValue(1);

			auto result = graph.traveling_salesman_problem < true > (firstNode, dependencies);

			EXPECT_EQ(result.first.size(), 0);
			EXPECT_EQ(result.second, float{});

			graph.insert(firstNode);
			dependencies.insert(firstNode);

			result = graph.traveling_salesman_problem < true > (firstNode, dependencies);

			ASSERT_EQ(result.first.size(), 1);
			EXPECT_EQ(*(result.first.begin()), firstNode);
			EXPECT_EQ(result.second, float{});

			graph.insert(firstNode, secondNode);
			dependencies.insert(firstNode, secondNode);

			result = graph.traveling_salesman_problem < true > (firstNode, dependencies);

			ASSERT_EQ(result.first.size(), 0);
			EXPECT_EQ(result.second, float{});

			result = graph.traveling_salesman_problem < true > (secondNode, dependencies);

			ASSERT_EQ(result.first.size(), 2);
			EXPECT_EQ(*(result.first.begin()), secondNode);
			EXPECT_EQ(*(result.first.begin() + 1), firstNode);
			EXPECT_EQ(result.second, float{1});
		}
	}

	TYPED_TEST_P(GraphGraphTest, TravelingSalesmanProblemWithDependenciesDisjoint)
	{
		if constexpr (
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::ordered ||
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::unordered)
		{
			xablau::graph::graph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type > graph;

			xablau::graph::digraph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type > dependencies;

			graph.insert(this->createValue(0), this->createValue(1));
			graph.insert(this->createValue(1), this->createValue(2));
			graph.insert(this->createValue(2), this->createValue(0));
			graph.insert(this->createValue(3));

			dependencies.insert(this->createValue(0));
			dependencies.insert(this->createValue(1));
			dependencies.insert(this->createValue(2));
			dependencies.insert(this->createValue(3));

			for (size_t i = 0; i < 4; i++)
			{
				const auto result = graph.traveling_salesman_problem < true > (this->createValue(i), dependencies);

				EXPECT_EQ(result.first.size(), 0);
				EXPECT_EQ(result.second, float{});
			}
		}
	}

	TYPED_TEST_P(GraphGraphTest, TravelingSalesmanProblemWithDependenciesRegular)
	{
		if constexpr (
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::ordered ||
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::unordered)
		{
			xablau::graph::graph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type > graph;

			xablau::graph::digraph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type > dependencies;

			this->completeGraph(graph);
			std::vector < typename TypeParam::value_type > nodes;

			for (size_t i = 0; i < graph.node_count() - 1; i++)
			{
				dependencies.insert(this->createValue(i), this->createValue((i + 1) % graph.node_count()));
				nodes.push_back(this->createValue(i));
			}

			nodes.push_back(this->createValue(graph.node_count() - 1));

			std::ranges::reverse(nodes);

			const auto result = graph.traveling_salesman_problem < true > (this->createValue(graph.node_count() - 1), dependencies);

			EXPECT_EQ(result.first.size(), 10);
			EXPECT_EQ(result.first, nodes);
			EXPECT_EQ(result.second, float{54});
		}
	}

	TYPED_TEST_P(GraphGraphTest, TreeBFS)
	{
		using NaryNode =
			xablau::graph::nary_tree_node <
				typename TypeParam::value_type,
				xablau::graph::nary_tree_node_container < xablau::graph::nary_tree_node_container_value::vector > >;

		using NaryTree = xablau::graph::nary_tree < NaryNode >;

		xablau::graph::graph <
			typename TypeParam::value_type,
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

			EXPECT_EQ(root.value(), iterator->first);
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
					EXPECT_GE(grandchild.value(), this->createValue(10));
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
				typename TypeParam::value_type,
				xablau::graph::nary_tree_node_container < xablau::graph::nary_tree_node_container_value::vector > >;

		using NaryTree = xablau::graph::nary_tree < NaryNode >;

		xablau::graph::graph <
			typename TypeParam::value_type,
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
			EXPECT_EQ(currentNode.get().value(), iterator->first);

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
				typename TypeParam::value_type,
				xablau::graph::nary_tree_node_container < xablau::graph::nary_tree_node_container_value::vector > >;

		using NaryTree = xablau::graph::nary_tree < NaryNode >;

		xablau::graph::graph <
			typename TypeParam::value_type,
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

			EXPECT_EQ(root.value(), iterator->first);

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
							EXPECT_GE(grandchild.value(), this->createValue(10));
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
								EXPECT_GE(grandchild.value(), this->createValue(10));
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
				typename TypeParam::value_type,
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
				typename TypeParam::value_type,
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
		TriangleCount,
		TriangleCountComplete,
		TripletCount,
		TripletCountComplete,
		ClusteringCoefficient,
		ClusteringCoefficientComplete,
		TriangleNeighbors,
		ClosenessCentrality,
		NodeBetweenness,
		EdgeBetweenness,
		Modularity,
		AdjacencyMatrix,
		LaplacianMatrix,
		DijkstraAllPreviousAndNextNodes,
		DijkstraAllPreviousAndNextNodesAndDistance,
		DijkstraSinglePreviousAndNextNodes,
		DijkstraSinglePreviousAndNextNodesAndDistance,
		DijkstraSinglePath,
		FloydWarshall,
		TravelingSalesmanProblemFewNodes,
		TravelingSalesmanProblemDisjoint,
		TravelingSalesmanProblemRegular,
		TravelingSalesmanProblemWithDependenciesFewNodes,
		TravelingSalesmanProblemWithDependenciesDisjoint,
		TravelingSalesmanProblemWithDependenciesRegular,
		TreeBFS,
		TreeDFS,
		TreeDijkstra,
		Connectivity,
		Concepts);

	using GraphGraphTestNodeTypes = std::tuple < size_t, std::string, GraphGraphCustomType >;

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
