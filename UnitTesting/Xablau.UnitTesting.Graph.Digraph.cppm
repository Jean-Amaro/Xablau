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
	struct GraphDigraphTypeDefinitions
	{
		using value_type = NodeType;
		using graph_container_type = MapContainerType;
		using edge_type = EdgeType;
	};

	class GraphDigraphCustomType
	{
	private:
		std::vector < size_t > data;

	public:
		explicit operator size_t() const { return this->data.back(); }

		auto operator<=>(const GraphDigraphCustomType &) const = default;

		GraphDigraphCustomType &operator=(const GraphDigraphCustomType &) = default;

		GraphDigraphCustomType &operator=(GraphDigraphCustomType &&) noexcept = default;

		GraphDigraphCustomType()
		{
			this->data.push_back(0);
		}

		GraphDigraphCustomType(const size_t data)
		{
			this->data.push_back(data);
		}

		GraphDigraphCustomType(const GraphDigraphCustomType &) = default;

		GraphDigraphCustomType(GraphDigraphCustomType &&) noexcept = default;
	};
}

namespace std
{
	template <>
	struct hash < xablau::unit_testing::GraphDigraphCustomType >
	{
		size_t operator()(const xablau::unit_testing::GraphDigraphCustomType &data) const noexcept
		{
			return std::hash < size_t > {}(static_cast < size_t > (data));
		}
	};
}

namespace xablau::unit_testing
{
	template < typename Types >
	class GraphDigraphTest : public ::testing::Test
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

		static void halfDigraph(auto &digraph)
		{
			for (size_t i = 0; i < 10; i++)
			{
				const auto valueI = GraphDigraphTest::createValue(i);

				EXPECT_EQ(digraph.insert(valueI), valueI);

				for (size_t j = i; j < 10; j++)
				{
					const auto valueJ = GraphDigraphTest::createValue(j);

					EXPECT_EQ(
						digraph.insert(
							valueI,
							valueJ,
							Types::edge_type(static_cast < float > (j + 1))).value().get().weight(),
						static_cast < float > (j + 1));
				}

				if constexpr (
					Types::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_ordered ||
					Types::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_unordered)
				{
					for (size_t j = i; j < 10; j++)
					{
						const auto valueJ = GraphDigraphTest::createValue(j);

						EXPECT_EQ(
							digraph.insert(
								valueI,
								valueJ,
								Types::edge_type(static_cast < float > (j + 1))).value().get().weight(),
							static_cast < float > (j + 1));
					}
				}
			}
		}

		static void completeDigraph(auto &digraph)
		{
			for (size_t i = 0; i < 10; i++)
			{
				const auto valueI = GraphDigraphTest::createValue(i);

				EXPECT_EQ(digraph.insert(valueI), valueI);

				for (size_t j = 0; j < 10; j++)
				{
					const auto valueJ = GraphDigraphTest::createValue(j);

					EXPECT_EQ(
						digraph.insert(
							valueI,
							valueJ,
							Types::edge_type(static_cast < float > (j + 1))).value().get().weight(),
						static_cast < float > (j + 1));
				}

				if constexpr (
					Types::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_ordered ||
					Types::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_unordered)
				{
					for (size_t j = 0; j < 10; j++)
					{
						const auto valueJ = GraphDigraphTest::createValue(j);

						EXPECT_EQ(
							digraph.insert(
								valueI,
								valueJ,
								Types::edge_type(static_cast < float > (j + 1))).value().get().weight(),
							static_cast < float > (j + 1));
					}
				}
			}
		}

		static void completeDigraphWithExtraLevels(auto &digraph)
		{
			GraphDigraphTest::completeDigraph(digraph);

			for (size_t i = 0; i < 10; i++)
			{
				const auto valueI = GraphDigraphTest::createValue(i);
				const auto valueJ = GraphDigraphTest::createValue(i + 10);
				const auto valueK = GraphDigraphTest::createValue(i + 20);

				EXPECT_EQ(
					digraph.insert(
						valueI,
						valueJ,
						Types::edge_type(float{1})).value().get().weight(), float{1});

				EXPECT_EQ(
					digraph.insert(
						valueK,
						valueI,
						Types::edge_type(float{1})).value().get().weight(), float{1});
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
							GraphDigraphTest::createValue(index1),
							GraphDigraphTest::createValue(index2),
							Types::edge_type(float{1})).value().get().weight(),
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
							GraphDigraphTest::createValue(value1),
							GraphDigraphTest::createValue(value2),
							Types::edge_type(float{1})).value().get().weight(),
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

			digraph.insert(GraphDigraphTest::createValue(10));
		}
	};

	TYPED_TEST_SUITE_P(GraphDigraphTest);

	TYPED_TEST_P(GraphDigraphTest, MultipleEdges)
	{
		using graph_type =
			typename xablau::graph::digraph <
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

	TYPED_TEST_P(GraphDigraphTest, Insertion)
	{
		xablau::graph::digraph <
			typename TypeParam::value_type,
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

	TYPED_TEST_P(GraphDigraphTest, Deletion)
	{
		using graph_type =
			typename xablau::graph::digraph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type >;

		graph_type digraph;

		this->completeDigraph(digraph);

		for (size_t i = 0; i < 10; i++)
		{
			for (size_t j = 0; j < 10; j++)
			{
				if constexpr (graph_type::can_have_multiple_edges())
				{
					EXPECT_EQ(digraph.erase(this->createValue(i), this->createValue(j)), 2);
				}

				else
				{
					EXPECT_EQ(digraph.erase(this->createValue(i), this->createValue(j)), 1);
				}
			}

			EXPECT_EQ(digraph.degree(this->createValue(i)), float{});
		}

		EXPECT_EQ(digraph.volume(), float{});
		EXPECT_EQ(digraph.node_count(), 10);
		EXPECT_EQ(digraph.edge_count(), 0);
		EXPECT_EQ(digraph.unique_edge_count(), 0);

		for (size_t i = 0; i < 10; i++)
		{
			EXPECT_EQ(digraph.erase(this->createValue(i)), 0);
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
			if constexpr (graph_type::can_have_multiple_edges())
			{
				EXPECT_EQ(digraph.erase(this->createValue(i)), 2 * ((2 * (10 - i)) - 1));
			}

			else
			{
				EXPECT_EQ(digraph.erase(this->createValue(i)), (2 * (10 - i)) - 1);
			}

			EXPECT_EQ(digraph.degree(this->createValue(i)), float{});
		}

		EXPECT_EQ(digraph.volume(), float{});
		EXPECT_EQ(digraph.node_count(), 0);
		EXPECT_EQ(digraph.edge_count(), 0);
		EXPECT_EQ(digraph.unique_edge_count(), 0);
	}

	TYPED_TEST_P(GraphDigraphTest, Degree)
	{
		xablau::graph::digraph <
			typename TypeParam::value_type,
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

	TYPED_TEST_P(GraphDigraphTest, StatisticalMoment)
	{
		xablau::graph::digraph <
			typename TypeParam::value_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > digraph;

		this->halfDigraph(digraph);

		const auto statisticalMomentOrder1 = digraph.statistical_moment(1);
		const auto statisticalMomentOrder2 = digraph.statistical_moment(2);
		const auto variance = statisticalMomentOrder2 - (statisticalMomentOrder1 * statisticalMomentOrder1);

		EXPECT_EQ(digraph.variance(), variance);

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

	TYPED_TEST_P(GraphDigraphTest, ShannonEntropy)
	{
		xablau::graph::digraph <
			typename TypeParam::value_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > digraph;

		this->halfDigraph(digraph);

		EXPECT_EQ(digraph.Shannon_entropy(), float{3.32192826});
	}

	TYPED_TEST_P(GraphDigraphTest, ClosenessCentrality)
	{
		using WeightType = typename TypeParam::edge_type::weight_type;

		xablau::graph::digraph <
			typename TypeParam::value_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > digraph;

		const auto value1 = this->createValue(1);
		const auto value2 = this->createValue(2);
		const auto value3 = this->createValue(3);
		const auto value4 = this->createValue(4);
		const auto value5 = this->createValue(5);

		digraph.insert(value1, value2);
		digraph.insert(value1, value3);
		digraph.insert(value2, value3);
		digraph.insert(value2, value4);
		digraph.insert(value2, value5);

		digraph.insert(value2, value1);
		digraph.insert(value3, value1);
		digraph.insert(value3, value2);
		digraph.insert(value4, value2);
		digraph.insert(value5, value2);

		digraph.insert(value1, value2, WeightType{2});
		digraph.insert(value1, value3, WeightType{2});
		digraph.insert(value2, value3, WeightType{2});
		digraph.insert(value2, value4, WeightType{2});
		digraph.insert(value2, value5, WeightType{2});

		digraph.insert(value2, value1, WeightType{2});
		digraph.insert(value3, value1, WeightType{2});
		digraph.insert(value3, value2, WeightType{2});
		digraph.insert(value4, value2, WeightType{2});
		digraph.insert(value5, value2, WeightType{2});

		const auto centrality = digraph.closeness_centrality < false > ();

		EXPECT_EQ(centrality.at(value1), WeightType{2} / WeightType{3});
		EXPECT_EQ(centrality.at(value2), WeightType{1});
		EXPECT_EQ(centrality.at(value3), WeightType{2} / WeightType{3});
		EXPECT_EQ(centrality.at(value4), WeightType{4} / WeightType{7});
		EXPECT_EQ(centrality.at(value5), WeightType{4} / WeightType{7});

		EXPECT_EQ(centrality.at(value1), digraph.closeness_centrality(value1));
		EXPECT_EQ(centrality.at(value2), digraph.closeness_centrality(value2));
		EXPECT_EQ(centrality.at(value3), digraph.closeness_centrality(value3));
		EXPECT_EQ(centrality.at(value4), digraph.closeness_centrality(value4));
		EXPECT_EQ(centrality.at(value5), digraph.closeness_centrality(value5));
	}

	TYPED_TEST_P(GraphDigraphTest, NodeBetweenness)
	{
		xablau::graph::digraph <
			typename TypeParam::value_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > digraph;

		using WeightType = typename TypeParam::edge_type::weight_type;

		const auto value1 = this->createValue(1);
		const auto value2 = this->createValue(2);
		const auto value3 = this->createValue(3);
		const auto value4 = this->createValue(4);
		const auto value5 = this->createValue(5);
		const auto value6 = this->createValue(6);

		digraph.insert(value1, value2);
		digraph.insert(value1, value5);
		digraph.insert(value2, value3);
		digraph.insert(value2, value4);
		digraph.insert(value3, value4);
		digraph.insert(value3, value6);
		digraph.insert(value5, value6);

		digraph.insert(value2, value1);
		digraph.insert(value5, value1);
		digraph.insert(value3, value2);
		digraph.insert(value4, value2);
		digraph.insert(value4, value3);
		digraph.insert(value6, value3);
		digraph.insert(value6, value5);

		if constexpr (
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_ordered ||
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_unordered)
		{
			digraph.insert(value1, value2, WeightType{2});
			digraph.insert(value1, value5, WeightType{2});
			digraph.insert(value2, value3, WeightType{2});
			digraph.insert(value2, value4, WeightType{2});
			digraph.insert(value3, value4, WeightType{2});
			digraph.insert(value3, value6, WeightType{2});
			digraph.insert(value5, value6, WeightType{2});

			digraph.insert(value2, value1, WeightType{2});
			digraph.insert(value5, value1, WeightType{2});
			digraph.insert(value3, value2, WeightType{2});
			digraph.insert(value4, value2, WeightType{2});
			digraph.insert(value4, value3, WeightType{2});
			digraph.insert(value6, value3, WeightType{2});
			digraph.insert(value6, value5, WeightType{2});
		}

		const auto betweennesses = digraph.node_betweenness_centrality < true > ();

		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (betweennesses.at(value1), WeightType{1.5}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (betweennesses.at(value2), WeightType{2.5}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (betweennesses.at(value3), WeightType{2.5}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (betweennesses.at(value4), WeightType{0}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (betweennesses.at(value5), WeightType{1}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (betweennesses.at(value6), WeightType{1.5}));

		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (digraph.node_betweenness_centrality(value1), betweennesses.at(value1)));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (digraph.node_betweenness_centrality(value2), betweennesses.at(value2)));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (digraph.node_betweenness_centrality(value3), betweennesses.at(value3)));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (digraph.node_betweenness_centrality(value4), betweennesses.at(value4)));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (digraph.node_betweenness_centrality(value5), betweennesses.at(value5)));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (digraph.node_betweenness_centrality(value6), betweennesses.at(value6)));
	}

	TYPED_TEST_P(GraphDigraphTest, EdgeBetweenness)
	{
		xablau::graph::digraph <
			typename TypeParam::value_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > digraph;

		using WeightType = typename TypeParam::edge_type::weight_type;

		const auto value1 = this->createValue(1);
		const auto value2 = this->createValue(2);
		const auto value3 = this->createValue(3);
		const auto value4 = this->createValue(4);
		const auto value5 = this->createValue(5);
		const auto value6 = this->createValue(6);

		digraph.insert(value1, value2);
		digraph.insert(value1, value4);
		digraph.insert(value2, value3);
		digraph.insert(value2, value5);
		digraph.insert(value3, value6);
		digraph.insert(value4, value5);
		digraph.insert(value5, value6);

		digraph.insert(value2, value1);
		digraph.insert(value4, value1);
		digraph.insert(value3, value2);
		digraph.insert(value5, value2);
		digraph.insert(value6, value3);
		digraph.insert(value5, value4);
		digraph.insert(value6, value5);

		if constexpr (
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_ordered ||
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::multi_unordered)
		{
			digraph.insert(value1, value2, WeightType{2});
			digraph.insert(value1, value4, WeightType{2});
			digraph.insert(value2, value3, WeightType{2});
			digraph.insert(value2, value5, WeightType{2});
			digraph.insert(value3, value6, WeightType{2});
			digraph.insert(value4, value5, WeightType{2});
			digraph.insert(value5, value6, WeightType{2});

			digraph.insert(value2, value1, WeightType{2});
			digraph.insert(value4, value1, WeightType{2});
			digraph.insert(value3, value2, WeightType{2});
			digraph.insert(value5, value2, WeightType{2});
			digraph.insert(value6, value3, WeightType{2});
			digraph.insert(value5, value4, WeightType{2});
			digraph.insert(value6, value5, WeightType{2});
		}

		const auto betweennesses = digraph.edge_betweenness_centrality < true > ();

		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (betweennesses.at(std::make_pair(value1, value2)), WeightType{8}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (betweennesses.at(std::make_pair(value1, value4)), WeightType{16} / WeightType{3}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (betweennesses.at(std::make_pair(value2, value3)), WeightType{8}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (betweennesses.at(std::make_pair(value2, value5)), WeightType{22} / WeightType{3}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (betweennesses.at(std::make_pair(value3, value6)), WeightType{16} / WeightType{3}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (betweennesses.at(std::make_pair(value4, value5)), WeightType{8}));
		EXPECT_TRUE(xablau::algebra::functions::almost_equal < WeightType > ::invoke < 2 > (betweennesses.at(std::make_pair(value5, value6)), WeightType{8}));
	}

	TYPED_TEST_P(GraphDigraphTest, Modularity)
	{
		using DigraphType =
			xablau::graph::digraph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type >;

		using WeightType = typename TypeParam::edge_type::weight_type;

		DigraphType digraph;

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

		digraph.insert(value1, value2);
		digraph.insert(value1, value3);
		digraph.insert(value1, value10);
		digraph.insert(value2, value3);
		digraph.insert(value4, value5);
		digraph.insert(value4, value6);
		digraph.insert(value4, value10);
		digraph.insert(value5, value6);
		digraph.insert(value7, value8);
		digraph.insert(value7, value9);
		digraph.insert(value7, value10);
		digraph.insert(value8, value9);

		digraph.insert(value2, value1);
		digraph.insert(value3, value1);
		digraph.insert(value10, value1);
		digraph.insert(value3, value2);
		digraph.insert(value5, value4);
		digraph.insert(value6, value4);
		digraph.insert(value10, value4);
		digraph.insert(value6, value5);
		digraph.insert(value8, value7);
		digraph.insert(value9, value7);
		digraph.insert(value10, value7);
		digraph.insert(value9, value8);

		if constexpr (DigraphType::can_have_multiple_edges())
		{
			digraph.insert(value1, value2);
			digraph.insert(value1, value3);
			digraph.insert(value1, value10);
			digraph.insert(value2, value3);
			digraph.insert(value4, value5);
			digraph.insert(value4, value6);
			digraph.insert(value4, value10);
			digraph.insert(value5, value6);
			digraph.insert(value7, value8);
			digraph.insert(value7, value9);
			digraph.insert(value7, value10);
			digraph.insert(value8, value9);

			digraph.insert(value2, value1);
			digraph.insert(value3, value1);
			digraph.insert(value10, value1);
			digraph.insert(value3, value2);
			digraph.insert(value5, value4);
			digraph.insert(value6, value4);
			digraph.insert(value10, value4);
			digraph.insert(value6, value5);
			digraph.insert(value8, value7);
			digraph.insert(value9, value7);
			digraph.insert(value10, value7);
			digraph.insert(value9, value8);
		}

		std::vector < DigraphType::template node_set_container < true > > communitiesClassification;

		communitiesClassification.emplace_back(DigraphType::template node_set_container < true > ({ value1, value2, value3, value10 }));
		communitiesClassification.emplace_back(DigraphType::template node_set_container < true > ({ value4, value5, value6 }));
		communitiesClassification.emplace_back(DigraphType::template node_set_container < true > ({ value7, value8, value9 }));

		EXPECT_EQ(std::round(digraph.modularity < true > (communitiesClassification) * WeightType{10000}) / WeightType{10000}, WeightType{0.4896});
	}

	TYPED_TEST_P(GraphDigraphTest, AdjacencyMatrix)
	{
		xablau::graph::digraph <
			typename TypeParam::value_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > digraph;

		this->completeDigraph(digraph);

		const auto [fixedMatrix, fixedNodeToIndex, fixedIndexToNode] =
			digraph.template adjacency_matrix <
				xablau::algebra::tensor_dense_fixed <
					float,
					xablau::algebra::tensor_fixed_dimensionalities < 10, 10 >,
					xablau::algebra::tensor_contiguity < false > >,
				true,
				true > ();

		const auto [dynamicMatrix, dynamicNodeToIndex, dynamicIndexToNode] =
			digraph.template adjacency_matrix <
				xablau::algebra::tensor_dense_dynamic <
					float,
					xablau::algebra::tensor_rank < 2 >,
					xablau::algebra::tensor_contiguity < false > >,
				true,
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
				EXPECT_EQ(fixedIndexToNode.at(fixedIndex2), iterator2->first.get());
				EXPECT_EQ(dynamicIndexToNode.at(dynamicIndex1), iterator1->first);
				EXPECT_EQ(dynamicIndexToNode.at(dynamicIndex2), iterator2->first.get());

				EXPECT_EQ(fixedMatrix(fixedIndex1, fixedIndex2), dynamicMatrix(dynamicIndex1, dynamicIndex2));
				ASSERT_GT(fixedMatrix(fixedIndex1, fixedIndex2), float{});
			}
		}
	}

	TYPED_TEST_P(GraphDigraphTest, LaplacianMatrix)
	{
		xablau::graph::digraph <
			typename TypeParam::value_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > digraph;

		this->completeDigraph(digraph);

		const auto [fixedMatrix, fixedNodeToIndex, fixedIndexToNode] =
			digraph.template laplacian_matrix <
				xablau::algebra::tensor_dense_fixed <
					float,
					xablau::algebra::tensor_fixed_dimensionalities < 10, 10 >,
					xablau::algebra::tensor_contiguity < false > >,
				true > ();

		const auto [dynamicMatrix, dynamicNodeToIndex, dynamicIndexToNode] =
			digraph.template laplacian_matrix <
				xablau::algebra::tensor_dense_dynamic <
					float,
					xablau::algebra::tensor_rank < 2 >,
					xablau::algebra::tensor_contiguity < false > >,
				true > ();

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

	TYPED_TEST_P(GraphDigraphTest, TarjanStronglyConnectedComponents)
	{
		xablau::graph::digraph <
			typename TypeParam::value_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > digraph;

		this->notStronglyConnectedDigraph(digraph);

		const auto connectedComponents =
			digraph.Tarjan_strongly_connected_components();

		using component_type =
			typename xablau::graph::digraph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type > ::template node_set_container < false >;

		std::vector < decltype(this->createValue(size_t{})) > nodes;

		for (size_t i = 0; i < 11; i++)
		{
			nodes.push_back(this->createValue(size_t{i}));
		}

		component_type group1{};
		component_type group2{};
		component_type group3{};
		component_type group4{};
		component_type group5{};
		component_type group6{};
		component_type group7{};

		group1.insert(nodes[0]);

		group2.insert(nodes[1]);
		group2.insert(nodes[2]);
		group2.insert(nodes[3]);

		group3.insert(nodes[4]);
		group3.insert(nodes[6]);

		group4.insert(nodes[5]);
		group4.insert(nodes[7]);

		group5.insert(nodes[8]);

		group6.insert(nodes[9]);

		group7.insert(nodes[10]);

		EXPECT_EQ(connectedComponents.at(nodes[0]), group1);
		EXPECT_EQ(connectedComponents.at(nodes[1]), group2);
		EXPECT_EQ(connectedComponents.at(nodes[2]), group2);
		EXPECT_EQ(connectedComponents.at(nodes[3]), group2);
		EXPECT_EQ(connectedComponents.at(nodes[4]), group3);
		EXPECT_EQ(connectedComponents.at(nodes[5]), group4);
		EXPECT_EQ(connectedComponents.at(nodes[6]), group3);
		EXPECT_EQ(connectedComponents.at(nodes[7]), group4);
		EXPECT_EQ(connectedComponents.at(nodes[8]), group5);
		EXPECT_EQ(connectedComponents.at(nodes[9]), group6);
		EXPECT_EQ(connectedComponents.at(nodes[10]), group7);
	}

	TYPED_TEST_P(GraphDigraphTest, DijkstraAllPreviousAndNextNodes)
	{
		using GraphType =
			xablau::graph::digraph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type >;

		using WeightType = typename TypeParam::edge_type::weight_type;

		GraphType digraph;

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

		digraph.insert(value0, value1);
		digraph.insert(value0, value2);
		digraph.insert(value1, value3);
		digraph.insert(value1, value4);
		digraph.insert(value2, value4);
		digraph.insert(value2, value5);
		digraph.insert(value3, value6);
		digraph.insert(value3, value7);
		digraph.insert(value4, value7);
		digraph.insert(value4, value8);
		digraph.insert(value5, value8);
		digraph.insert(value5, value9);
		digraph.insert(value6, value10);
		digraph.insert(value7, value10);
		digraph.insert(value9, value12, WeightType{3});
		digraph.insert(value10, value11);
		digraph.insert(value11, value12);

		digraph.insert(value1, value0);
		digraph.insert(value2, value0);
		digraph.insert(value3, value1);
		digraph.insert(value4, value1);
		digraph.insert(value4, value2);
		digraph.insert(value5, value2);
		digraph.insert(value6, value3);
		digraph.insert(value7, value3);
		digraph.insert(value7, value4);
		digraph.insert(value8, value4);
		digraph.insert(value8, value5);
		digraph.insert(value9, value5);
		digraph.insert(value10, value6);
		digraph.insert(value10, value7);
		digraph.insert(value12, value9, WeightType{3});
		digraph.insert(value11, value10);
		digraph.insert(value12, value11);

		if constexpr (GraphType::can_have_multiple_edges())
		{
			digraph.insert(value0, value1, WeightType{2});
			digraph.insert(value0, value2, WeightType{2});
			digraph.insert(value1, value3, WeightType{2});
			digraph.insert(value1, value4, WeightType{2});
			digraph.insert(value2, value4, WeightType{2});
			digraph.insert(value2, value5, WeightType{2});
			digraph.insert(value3, value6, WeightType{2});
			digraph.insert(value3, value7, WeightType{2});
			digraph.insert(value4, value7, WeightType{2});
			digraph.insert(value4, value8, WeightType{2});
			digraph.insert(value5, value8, WeightType{2});
			digraph.insert(value5, value9, WeightType{2});
			digraph.insert(value6, value10, WeightType{2});
			digraph.insert(value7, value10, WeightType{2});
			digraph.insert(value9, value12, WeightType{6});
			digraph.insert(value10, value11, WeightType{2});
			digraph.insert(value11, value12, WeightType{2});

			digraph.insert(value1, value0, WeightType{2});
			digraph.insert(value2, value0, WeightType{2});
			digraph.insert(value3, value1, WeightType{2});
			digraph.insert(value4, value1, WeightType{2});
			digraph.insert(value4, value2, WeightType{2});
			digraph.insert(value5, value2, WeightType{2});
			digraph.insert(value6, value3, WeightType{2});
			digraph.insert(value7, value3, WeightType{2});
			digraph.insert(value7, value4, WeightType{2});
			digraph.insert(value8, value4, WeightType{2});
			digraph.insert(value8, value5, WeightType{2});
			digraph.insert(value9, value5, WeightType{2});
			digraph.insert(value10, value6, WeightType{2});
			digraph.insert(value10, value7, WeightType{2});
			digraph.insert(value12, value9, WeightType{6});
			digraph.insert(value11, value10, WeightType{2});
			digraph.insert(value12, value11, WeightType{2});
		}

		const auto [previous, next] = digraph.Dijkstra_all_previous_and_next_nodes < true > (value0);

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

	TYPED_TEST_P(GraphDigraphTest, DijkstraAllPreviousAndNextNodesAndDistance)
	{
		using GraphType =
			xablau::graph::digraph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type >;

		using WeightType = typename TypeParam::edge_type::weight_type;

		GraphType digraph;

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

		digraph.insert(value0, value1);
		digraph.insert(value0, value2);
		digraph.insert(value1, value3);
		digraph.insert(value1, value4);
		digraph.insert(value2, value4);
		digraph.insert(value2, value5);
		digraph.insert(value3, value6);
		digraph.insert(value3, value7);
		digraph.insert(value4, value7);
		digraph.insert(value4, value8);
		digraph.insert(value5, value8);
		digraph.insert(value5, value9);
		digraph.insert(value6, value10);
		digraph.insert(value7, value10);
		digraph.insert(value9, value12, WeightType{3});
		digraph.insert(value10, value11);
		digraph.insert(value11, value12);

		digraph.insert(value1, value0);
		digraph.insert(value2, value0);
		digraph.insert(value3, value1);
		digraph.insert(value4, value1);
		digraph.insert(value4, value2);
		digraph.insert(value5, value2);
		digraph.insert(value6, value3);
		digraph.insert(value7, value3);
		digraph.insert(value7, value4);
		digraph.insert(value8, value4);
		digraph.insert(value8, value5);
		digraph.insert(value9, value5);
		digraph.insert(value10, value6);
		digraph.insert(value10, value7);
		digraph.insert(value12, value9, WeightType{3});
		digraph.insert(value11, value10);
		digraph.insert(value12, value11);

		if constexpr (GraphType::can_have_multiple_edges())
		{
			digraph.insert(value0, value1, WeightType{2});
			digraph.insert(value0, value2, WeightType{2});
			digraph.insert(value1, value3, WeightType{2});
			digraph.insert(value1, value4, WeightType{2});
			digraph.insert(value2, value4, WeightType{2});
			digraph.insert(value2, value5, WeightType{2});
			digraph.insert(value3, value6, WeightType{2});
			digraph.insert(value3, value7, WeightType{2});
			digraph.insert(value4, value7, WeightType{2});
			digraph.insert(value4, value8, WeightType{2});
			digraph.insert(value5, value8, WeightType{2});
			digraph.insert(value5, value9, WeightType{2});
			digraph.insert(value6, value10, WeightType{2});
			digraph.insert(value7, value10, WeightType{2});
			digraph.insert(value9, value12, WeightType{6});
			digraph.insert(value10, value11, WeightType{2});
			digraph.insert(value11, value12, WeightType{2});

			digraph.insert(value1, value0, WeightType{2});
			digraph.insert(value2, value0, WeightType{2});
			digraph.insert(value3, value1, WeightType{2});
			digraph.insert(value4, value1, WeightType{2});
			digraph.insert(value4, value2, WeightType{2});
			digraph.insert(value5, value2, WeightType{2});
			digraph.insert(value6, value3, WeightType{2});
			digraph.insert(value7, value3, WeightType{2});
			digraph.insert(value7, value4, WeightType{2});
			digraph.insert(value8, value4, WeightType{2});
			digraph.insert(value8, value5, WeightType{2});
			digraph.insert(value9, value5, WeightType{2});
			digraph.insert(value10, value6, WeightType{2});
			digraph.insert(value10, value7, WeightType{2});
			digraph.insert(value12, value9, WeightType{6});
			digraph.insert(value11, value10, WeightType{2});
			digraph.insert(value12, value11, WeightType{2});
		}

		const auto [previous, next] = digraph.Dijkstra_all_previous_and_next_nodes_and_distance < true > (value0);

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

	TYPED_TEST_P(GraphDigraphTest, DijkstraSinglePreviousAndNextNodes)
	{
		using GraphType =
			xablau::graph::digraph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type >;

		using WeightType = typename TypeParam::edge_type::weight_type;

		GraphType digraph;

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

		digraph.insert(value0, value1);
		digraph.insert(value0, value2);
		digraph.insert(value1, value3);
		digraph.insert(value1, value4);
		digraph.insert(value2, value4);
		digraph.insert(value2, value5);
		digraph.insert(value3, value6);
		digraph.insert(value3, value7);
		digraph.insert(value4, value7);
		digraph.insert(value4, value8);
		digraph.insert(value5, value8);
		digraph.insert(value5, value9);
		digraph.insert(value6, value10);
		digraph.insert(value7, value10);
		digraph.insert(value9, value12, WeightType{3});
		digraph.insert(value10, value11);
		digraph.insert(value11, value12);

		digraph.insert(value1, value0);
		digraph.insert(value2, value0);
		digraph.insert(value3, value1);
		digraph.insert(value4, value1);
		digraph.insert(value4, value2);
		digraph.insert(value5, value2);
		digraph.insert(value6, value3);
		digraph.insert(value7, value3);
		digraph.insert(value7, value4);
		digraph.insert(value8, value4);
		digraph.insert(value8, value5);
		digraph.insert(value9, value5);
		digraph.insert(value10, value6);
		digraph.insert(value10, value7);
		digraph.insert(value12, value9, WeightType{3});
		digraph.insert(value11, value10);
		digraph.insert(value12, value11);

		if constexpr (GraphType::can_have_multiple_edges())
		{
			digraph.insert(value0, value1, WeightType{2});
			digraph.insert(value0, value2, WeightType{2});
			digraph.insert(value1, value3, WeightType{2});
			digraph.insert(value1, value4, WeightType{2});
			digraph.insert(value2, value4, WeightType{2});
			digraph.insert(value2, value5, WeightType{2});
			digraph.insert(value3, value6, WeightType{2});
			digraph.insert(value3, value7, WeightType{2});
			digraph.insert(value4, value7, WeightType{2});
			digraph.insert(value4, value8, WeightType{2});
			digraph.insert(value5, value8, WeightType{2});
			digraph.insert(value5, value9, WeightType{2});
			digraph.insert(value6, value10, WeightType{2});
			digraph.insert(value7, value10, WeightType{2});
			digraph.insert(value9, value12, WeightType{6});
			digraph.insert(value10, value11, WeightType{2});
			digraph.insert(value11, value12, WeightType{2});

			digraph.insert(value1, value0, WeightType{2});
			digraph.insert(value2, value0, WeightType{2});
			digraph.insert(value3, value1, WeightType{2});
			digraph.insert(value4, value1, WeightType{2});
			digraph.insert(value4, value2, WeightType{2});
			digraph.insert(value5, value2, WeightType{2});
			digraph.insert(value6, value3, WeightType{2});
			digraph.insert(value7, value3, WeightType{2});
			digraph.insert(value7, value4, WeightType{2});
			digraph.insert(value8, value4, WeightType{2});
			digraph.insert(value8, value5, WeightType{2});
			digraph.insert(value9, value5, WeightType{2});
			digraph.insert(value10, value6, WeightType{2});
			digraph.insert(value10, value7, WeightType{2});
			digraph.insert(value12, value9, WeightType{6});
			digraph.insert(value11, value10, WeightType{2});
			digraph.insert(value12, value11, WeightType{2});
		}

		const auto [previous, next] = digraph.Dijkstra_single_previous_and_next_nodes < true > (value0);

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

	TYPED_TEST_P(GraphDigraphTest, DijkstraSinglePreviousAndNextNodesAndDistance)
	{
		using GraphType =
			xablau::graph::digraph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type >;

		using WeightType = typename TypeParam::edge_type::weight_type;

		GraphType digraph;

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

		digraph.insert(value0, value1);
		digraph.insert(value0, value2);
		digraph.insert(value1, value3);
		digraph.insert(value1, value4);
		digraph.insert(value2, value4);
		digraph.insert(value2, value5);
		digraph.insert(value3, value6);
		digraph.insert(value3, value7);
		digraph.insert(value4, value7);
		digraph.insert(value4, value8);
		digraph.insert(value5, value8);
		digraph.insert(value5, value9);
		digraph.insert(value6, value10);
		digraph.insert(value7, value10);
		digraph.insert(value9, value12, WeightType{3});
		digraph.insert(value10, value11);
		digraph.insert(value11, value12);

		digraph.insert(value1, value0);
		digraph.insert(value2, value0);
		digraph.insert(value3, value1);
		digraph.insert(value4, value1);
		digraph.insert(value4, value2);
		digraph.insert(value5, value2);
		digraph.insert(value6, value3);
		digraph.insert(value7, value3);
		digraph.insert(value7, value4);
		digraph.insert(value8, value4);
		digraph.insert(value8, value5);
		digraph.insert(value9, value5);
		digraph.insert(value10, value6);
		digraph.insert(value10, value7);
		digraph.insert(value12, value9, WeightType{3});
		digraph.insert(value11, value10);
		digraph.insert(value12, value11);

		if constexpr (GraphType::can_have_multiple_edges())
		{
			digraph.insert(value0, value1, WeightType{2});
			digraph.insert(value0, value2, WeightType{2});
			digraph.insert(value1, value3, WeightType{2});
			digraph.insert(value1, value4, WeightType{2});
			digraph.insert(value2, value4, WeightType{2});
			digraph.insert(value2, value5, WeightType{2});
			digraph.insert(value3, value6, WeightType{2});
			digraph.insert(value3, value7, WeightType{2});
			digraph.insert(value4, value7, WeightType{2});
			digraph.insert(value4, value8, WeightType{2});
			digraph.insert(value5, value8, WeightType{2});
			digraph.insert(value5, value9, WeightType{2});
			digraph.insert(value6, value10, WeightType{2});
			digraph.insert(value7, value10, WeightType{2});
			digraph.insert(value9, value12, WeightType{6});
			digraph.insert(value10, value11, WeightType{2});
			digraph.insert(value11, value12, WeightType{2});

			digraph.insert(value1, value0, WeightType{2});
			digraph.insert(value2, value0, WeightType{2});
			digraph.insert(value3, value1, WeightType{2});
			digraph.insert(value4, value1, WeightType{2});
			digraph.insert(value4, value2, WeightType{2});
			digraph.insert(value5, value2, WeightType{2});
			digraph.insert(value6, value3, WeightType{2});
			digraph.insert(value7, value3, WeightType{2});
			digraph.insert(value7, value4, WeightType{2});
			digraph.insert(value8, value4, WeightType{2});
			digraph.insert(value8, value5, WeightType{2});
			digraph.insert(value9, value5, WeightType{2});
			digraph.insert(value10, value6, WeightType{2});
			digraph.insert(value10, value7, WeightType{2});
			digraph.insert(value12, value9, WeightType{6});
			digraph.insert(value11, value10, WeightType{2});
			digraph.insert(value12, value11, WeightType{2});
		}

		const auto [previous, next] = digraph.Dijkstra_single_previous_and_next_nodes_and_distance < true > (value0);

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

	TYPED_TEST_P(GraphDigraphTest, DijkstraSinglePath)
	{
		using GraphType =
			xablau::graph::digraph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type >;

		using WeightType = typename TypeParam::edge_type::weight_type;

		GraphType digraph;

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

		digraph.insert(value0, value1);
		digraph.insert(value0, value2);
		digraph.insert(value1, value3);
		digraph.insert(value1, value4);
		digraph.insert(value2, value4);
		digraph.insert(value2, value5);
		digraph.insert(value3, value6);
		digraph.insert(value3, value7);
		digraph.insert(value4, value7);
		digraph.insert(value4, value8);
		digraph.insert(value5, value8);
		digraph.insert(value5, value9);
		digraph.insert(value6, value10);
		digraph.insert(value7, value10);
		digraph.insert(value9, value12, WeightType{3});
		digraph.insert(value10, value11);
		digraph.insert(value11, value12);

		digraph.insert(value1, value0);
		digraph.insert(value2, value0);
		digraph.insert(value3, value1);
		digraph.insert(value4, value1);
		digraph.insert(value4, value2);
		digraph.insert(value5, value2);
		digraph.insert(value6, value3);
		digraph.insert(value7, value3);
		digraph.insert(value7, value4);
		digraph.insert(value8, value4);
		digraph.insert(value8, value5);
		digraph.insert(value9, value5);
		digraph.insert(value10, value6);
		digraph.insert(value10, value7);
		digraph.insert(value12, value9, WeightType{3});
		digraph.insert(value11, value10);
		digraph.insert(value12, value11);

		if constexpr (GraphType::can_have_multiple_edges())
		{
			digraph.insert(value0, value1, WeightType{2});
			digraph.insert(value0, value2, WeightType{2});
			digraph.insert(value1, value3, WeightType{2});
			digraph.insert(value1, value4, WeightType{2});
			digraph.insert(value2, value4, WeightType{2});
			digraph.insert(value2, value5, WeightType{2});
			digraph.insert(value3, value6, WeightType{2});
			digraph.insert(value3, value7, WeightType{2});
			digraph.insert(value4, value7, WeightType{2});
			digraph.insert(value4, value8, WeightType{2});
			digraph.insert(value5, value8, WeightType{2});
			digraph.insert(value5, value9, WeightType{2});
			digraph.insert(value6, value10, WeightType{2});
			digraph.insert(value7, value10, WeightType{2});
			digraph.insert(value9, value12, WeightType{6});
			digraph.insert(value10, value11, WeightType{2});
			digraph.insert(value11, value12, WeightType{2});

			digraph.insert(value1, value0, WeightType{2});
			digraph.insert(value2, value0, WeightType{2});
			digraph.insert(value3, value1, WeightType{2});
			digraph.insert(value4, value1, WeightType{2});
			digraph.insert(value4, value2, WeightType{2});
			digraph.insert(value5, value2, WeightType{2});
			digraph.insert(value6, value3, WeightType{2});
			digraph.insert(value7, value3, WeightType{2});
			digraph.insert(value7, value4, WeightType{2});
			digraph.insert(value8, value4, WeightType{2});
			digraph.insert(value8, value5, WeightType{2});
			digraph.insert(value9, value5, WeightType{2});
			digraph.insert(value10, value6, WeightType{2});
			digraph.insert(value10, value7, WeightType{2});
			digraph.insert(value12, value9, WeightType{6});
			digraph.insert(value11, value10, WeightType{2});
			digraph.insert(value12, value11, WeightType{2});
		}

		const auto [distance, path] = digraph.Dijkstra_single_path < true, true > (value0, value12);

		EXPECT_EQ(distance, WeightType{6});
		EXPECT_TRUE(
			path == std::vector < typename TypeParam::value_type > ({ value0, value1, value3, value6, value10, value11, value12 }) ||
			path == std::vector < typename TypeParam::value_type > ({ value0, value1, value3, value7, value10, value11, value12 }) ||
			path == std::vector < typename TypeParam::value_type > ({ value0, value1, value4, value7, value10, value11, value12 }) ||
			path == std::vector < typename TypeParam::value_type > ({ value0, value2, value4, value7, value10, value11, value12 }) ||
			path == std::vector < typename TypeParam::value_type > ({ value0, value2, value5, value9, value12 }));
	}

	TYPED_TEST_P(GraphDigraphTest, FloydWarshall)
	{
		xablau::graph::digraph <
			typename TypeParam::value_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type > digraph;

		this->completeDigraph(digraph);

		const auto [fixedMatrix, fixedNextNode, fixedNodeToIndex, fixedIndexToNode] =
			digraph.template Floyd_Warshall <
				xablau::algebra::tensor_dense_fixed <
					float,
					xablau::algebra::tensor_fixed_dimensionalities < 10, 10 >,
					xablau::algebra::tensor_contiguity < false > >,
				true > ();

		const auto [dynamicMatrix, dynamicNextNode, dynamicNodeToIndex, dynamicIndexToNode] =
			digraph.template Floyd_Warshall <
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
				EXPECT_EQ(fixedIndexToNode.at(fixedIndex2), iterator2->first.get());
				EXPECT_EQ(dynamicIndexToNode.at(dynamicIndex1), iterator1->first);
				EXPECT_EQ(dynamicIndexToNode.at(dynamicIndex2), iterator2->first.get());

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

				EXPECT_EQ(fixedNextNode(fixedIndex1, fixedIndex2).value(), iterator2->first.get());
				EXPECT_EQ(dynamicNextNode(fixedIndex1, fixedIndex2).value(), iterator2->first.get());
			}
		}
	}

	TYPED_TEST_P(GraphDigraphTest, TravelingSalesmanProblemFewNodes)
	{
		if constexpr (
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::ordered ||
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::unordered)
		{
			xablau::graph::digraph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type > digraph;

			const auto firstNode = this->createValue(0);
			const auto secondNode = this->createValue(1);

			auto result = digraph.traveling_salesman_problem < true > (firstNode);

			EXPECT_EQ(result.first.size(), 0);
			EXPECT_EQ(result.second, float{});

			digraph.insert(firstNode);

			result = digraph.traveling_salesman_problem < true > (firstNode);

			ASSERT_EQ(result.first.size(), 1);
			EXPECT_EQ(*(result.first.begin()), firstNode);
			EXPECT_EQ(result.second, float{});

			digraph.insert(firstNode, secondNode);

			result = digraph.traveling_salesman_problem < true > (firstNode);

			ASSERT_EQ(result.first.size(), 2);
			EXPECT_EQ(*(result.first.begin()), firstNode);
			EXPECT_EQ(*(result.first.begin() + 1), secondNode);
			EXPECT_EQ(result.second, float{1});

			result = digraph.traveling_salesman_problem < true > (secondNode);

			EXPECT_EQ(result.first.size(), 0);
			EXPECT_EQ(result.second, float {});
		}
	}

	TYPED_TEST_P(GraphDigraphTest, TravelingSalesmanProblemDisjoint)
	{
		if constexpr (
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::ordered ||
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::unordered)
		{
			xablau::graph::digraph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type > digraph;

			digraph.insert(this->createValue(0), this->createValue(1));
			digraph.insert(this->createValue(1), this->createValue(2));
			digraph.insert(this->createValue(2), this->createValue(0));
			digraph.insert(this->createValue(3));

			for (size_t i = 0; i < 4; i++)
			{
				const auto result = digraph.traveling_salesman_problem < true > (this->createValue(i));

				EXPECT_EQ(result.first.size(), 0);
				EXPECT_EQ(result.second, float{});
			}
		}
	}

	TYPED_TEST_P(GraphDigraphTest, TravelingSalesmanProblemRegular)
	{
		if constexpr (
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::ordered ||
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::unordered)
		{
			xablau::graph::digraph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type > digraph;

			const float value1{1.17557f};
			const float value2{1.90211f};

			const auto node1 = this->createValue(0);
			const auto node2 = this->createValue(1);
			const auto node3 = this->createValue(2);
			const auto node4 = this->createValue(3);
			const auto node5 = this->createValue(4);

			digraph.insert(node1, node2, value1);
			digraph.insert(node2, node3, value1);
			digraph.insert(node3, node4, value1);
			digraph.insert(node4, node5, value1);
			digraph.insert(node5, node4, value1);

			digraph.insert(node3, node1, value2);
			digraph.insert(node4, node2, value2);

			std::vector < std::vector < typename TypeParam::value_type > > paths =
				{
					{ node1, node2, node3, node4, node5 },
					{},
					{},
					{},
					{ node5, node4, node2, node3, node1 }
				};

			std::vector < float > distances =
				{
					float{4} * value1,
					float{},
					float{},
					float{},
					float{2} * value1 + float{2} * value2
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
				const auto result = digraph.traveling_salesman_problem < true > (this->createValue(i));

				ASSERT_EQ(result.first.size(), paths[i].size());

				EXPECT_TRUE(comparison(result.first, paths[i]));
			}
		}
	}

	TYPED_TEST_P(GraphDigraphTest, TravelingSalesmanProblemWithDependenciesFewNodes)
	{
		if constexpr (
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::ordered ||
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::unordered)
		{
			xablau::graph::digraph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type > digraph;

			xablau::graph::digraph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type > dependencies;

			const auto firstNode = this->createValue(0);
			const auto secondNode = this->createValue(1);

			auto result = digraph.traveling_salesman_problem < true > (firstNode, dependencies);

			EXPECT_EQ(result.first.size(), 0);
			EXPECT_EQ(result.second, float{});

			digraph.insert(firstNode);
			dependencies.insert(firstNode);

			result = digraph.traveling_salesman_problem < true > (firstNode, dependencies);

			ASSERT_EQ(result.first.size(), 1);
			EXPECT_EQ(*(result.first.begin()), firstNode);
			EXPECT_EQ(result.second, float{});

			digraph.insert(firstNode, secondNode);
			dependencies.insert(firstNode, secondNode);

			result = digraph.traveling_salesman_problem < true > (firstNode, dependencies);

			ASSERT_EQ(result.first.size(), 0);
			EXPECT_EQ(result.second, float{});

			result = digraph.traveling_salesman_problem < true > (secondNode, dependencies);

			ASSERT_EQ(result.first.size(), 0);
			EXPECT_EQ(result.second, float {});

			digraph.insert(secondNode, firstNode);

			result = digraph.traveling_salesman_problem < true > (secondNode, dependencies);

			ASSERT_EQ(result.first.size(), 2);
			EXPECT_EQ(*(result.first.begin()), secondNode);
			EXPECT_EQ(*(result.first.begin() + 1), firstNode);
			EXPECT_EQ(result.second, float{1});
		}
	}

	TYPED_TEST_P(GraphDigraphTest, TravelingSalesmanProblemWithDependenciesDisjoint)
	{
		if constexpr (
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::ordered ||
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::unordered)
		{
			xablau::graph::digraph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type > digraph;

			xablau::graph::digraph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type > dependencies;

			digraph.insert(this->createValue(0), this->createValue(1));
			digraph.insert(this->createValue(1), this->createValue(2));
			digraph.insert(this->createValue(2), this->createValue(0));
			digraph.insert(this->createValue(3));

			dependencies.insert(this->createValue(0));
			dependencies.insert(this->createValue(1));
			dependencies.insert(this->createValue(2));
			dependencies.insert(this->createValue(3));

			for (size_t i = 0; i < 4; i++)
			{
				const auto result = digraph.traveling_salesman_problem < true > (this->createValue(i), dependencies);

				EXPECT_EQ(result.first.size(), 0);
				EXPECT_EQ(result.second, float{});
			}
		}
	}

	TYPED_TEST_P(GraphDigraphTest, TravelingSalesmanProblemWithDependenciesRegular)
	{
		if constexpr (
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::ordered ||
			TypeParam::graph_container_type::type() == xablau::graph::graph_container_type_value::unordered)
		{
			xablau::graph::digraph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type > digraph;

			xablau::graph::digraph <
				typename TypeParam::value_type,
				typename TypeParam::graph_container_type,
				typename TypeParam::edge_type > dependencies;

			this->completeDigraph(digraph);
			std::vector < typename TypeParam::value_type > nodes;

			for (size_t i = 0; i < digraph.node_count() - 1; i++)
			{
				dependencies.insert(this->createValue(i), this->createValue((i + 1) % digraph.node_count()));
				nodes.push_back(this->createValue(i));
			}

			nodes.push_back(this->createValue(digraph.node_count() - 1));

			std::ranges::reverse(nodes);

			const auto result = digraph.traveling_salesman_problem < true > (this->createValue(digraph.node_count() - 1), dependencies);

			EXPECT_EQ(result.first.size(), 10);
			EXPECT_EQ(result.first, nodes);
			EXPECT_EQ(result.second, float{45});
		}
	}

	TYPED_TEST_P(GraphDigraphTest, TreeBFS)
	{
		using NaryNode =
			xablau::graph::nary_tree_node <
				typename TypeParam::value_type,
				xablau::graph::nary_tree_node_container < xablau::graph::nary_tree_node_container_value::vector > >;

		using NaryTree = xablau::graph::nary_tree < NaryNode >;

		xablau::graph::digraph <
			typename TypeParam::value_type,
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

	TYPED_TEST_P(GraphDigraphTest, TreeDFS)
	{
		using NaryNode =
			xablau::graph::nary_tree_node <
				typename TypeParam::value_type,
				xablau::graph::nary_tree_node_container < xablau::graph::nary_tree_node_container_value::vector > >;

		using NaryTree = xablau::graph::nary_tree < NaryNode >;

		xablau::graph::digraph <
			typename TypeParam::value_type,
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

	TYPED_TEST_P(GraphDigraphTest, TreeDijkstra)
	{
		using NaryNode =
			xablau::graph::nary_tree_node <
				typename TypeParam::value_type,
				xablau::graph::nary_tree_node_container < xablau::graph::nary_tree_node_container_value::vector > >;

		using NaryTree = xablau::graph::nary_tree < NaryNode >;

		xablau::graph::digraph <
			typename TypeParam::value_type,
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

	TYPED_TEST_P(GraphDigraphTest, Connectivity)
	{
		using DigraphType =
			xablau::graph::digraph <
				typename TypeParam::value_type,
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

	TYPED_TEST_P(GraphDigraphTest, Nodes)
	{
		using DigraphType =
			xablau::graph::digraph <
			typename TypeParam::value_type,
			typename TypeParam::graph_container_type,
			typename TypeParam::edge_type >;

		DigraphType digraph;

		this->completeDigraphWithExtraLevels(digraph);

		const auto nodes = digraph.nodes < true > ();
		const auto sourceNodes = digraph.source_nodes < true > ();
		const auto sinkNodes = digraph.sink_nodes < true > ();

		EXPECT_EQ(nodes.size(), 30);
		EXPECT_EQ(sourceNodes.size(), 10);
		EXPECT_EQ(sinkNodes.size(), 10);

		// Every node.
		std::vector < typename TypeParam::value_type > _nodes;

		for (const auto &node : nodes)
		{
			_nodes.push_back(node);
		}

		std::ranges::sort(_nodes,
			[] (const typename TypeParam::value_type &node1, const typename TypeParam::value_type &node2) -> bool
			{
				return node1 < node2;
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
			[] (const typename TypeParam::value_type &node1, const typename TypeParam::value_type &node2) -> bool
			{
				return node1 < node2;
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
			[] (const typename TypeParam::value_type &node1, const typename TypeParam::value_type &node2) -> bool
			{
				return node1 < node2;
			});

		for (size_t i = 0; i < _nodes.size(); i++)
		{
			EXPECT_EQ(this->createValue(i + 20), _nodes[i]);
		}
	}

	TYPED_TEST_P(GraphDigraphTest, Concepts)
	{
		using digraph_type =
			typename xablau::graph::digraph <
				typename TypeParam::value_type,
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
		GraphDigraphTest,
		MultipleEdges,
		Insertion,
		Deletion,
		Degree,
		StatisticalMoment,
		ShannonEntropy,
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
		TarjanStronglyConnectedComponents,
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
		Nodes,
		Concepts);

	using GraphDigraphTestNodeTypes = std::tuple < size_t, std::string, GraphDigraphCustomType >;

	using GraphDigraphTestMapContainerTypes =
		std::tuple <
			xablau::graph::graph_container_type < xablau::graph::graph_container_type_value::ordered >,
			xablau::graph::graph_container_type < xablau::graph::graph_container_type_value::unordered >,
			xablau::graph::graph_container_type < xablau::graph::graph_container_type_value::multi_ordered >,
			xablau::graph::graph_container_type < xablau::graph::graph_container_type_value::multi_unordered > >;

	using GraphDigraphTestEdgeTypes =
		std::tuple < xablau::graph::edge < float > >;

	using GraphDigraphTestGroup =
		xablau::testing::test_bundler <
			GraphDigraphTestNodeTypes,
			GraphDigraphTestMapContainerTypes,
			GraphDigraphTestEdgeTypes > ::template test_group < GraphDigraphTypeDefinitions >;

	using GraphDigraphTypes = xablau::testing::tuple_transformation < ::testing::Types, GraphDigraphTestGroup > ::types;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		Xablau,
		GraphDigraphTest,
		GraphDigraphTypes);
}
