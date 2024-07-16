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

export module xablau.graph:graph;
export import :fundamental_concepts;
export import :graph_common_functions;
export import :nary_tree;
export import :node_ref;

export import std;

export import xablau.algebra;

export namespace xablau::graph
{
	template <
		typename ValueType,
		concepts::graph_container ContainerType,
		concepts::edge EdgeType >
	requires (concepts::graph_requirements < ValueType, ContainerType >)
	class graph final
	{
	public:
		using value_type = ValueType;
		using value_reference_type = xablau::graph::node_ref < const ValueType >;
		using container_type = ContainerType;
		using edge_type = EdgeType;
		using edge_weight_type = typename EdgeType::weight_type;

		using graph_container_type =
			typename std::conditional < ContainerType::type() == graph_container_type_value::ordered,
				std::map < ValueType, std::map < value_reference_type, EdgeType > >,

			typename std::conditional < ContainerType::type() == graph_container_type_value::unordered,
				std::unordered_map < ValueType, std::unordered_map < value_reference_type, EdgeType > >,

			typename std::conditional < ContainerType::type() == graph_container_type_value::multi_ordered,
				std::map < ValueType, std::map < value_reference_type, std::vector < EdgeType > > >,

				std::unordered_map < ValueType, std::unordered_map < value_reference_type, std::vector < EdgeType > > > >::type >::type >::type;

		using size_type = typename graph_container_type::size_type;

	private:
		static constexpr bool _ordered =
			ContainerType::type() == graph_container_type_value::ordered ||
			ContainerType::type() == graph_container_type_value::multi_ordered;

		static constexpr bool _unordered =
			ContainerType::type() == graph_container_type_value::unordered ||
			ContainerType::type() == graph_container_type_value::multi_unordered;

		static constexpr bool _single =
			ContainerType::type() == graph_container_type_value::ordered ||
			ContainerType::type() == graph_container_type_value::unordered;

		static constexpr bool _multi =
			ContainerType::type() == graph_container_type_value::multi_ordered ||
			ContainerType::type() == graph_container_type_value::multi_unordered;

		template < typename LocalType >
		static constexpr bool _equal_nodes(const LocalType &node1, const LocalType &node2)
		{
			if constexpr (graph::_ordered)
			{
				return !(node1 < node2) && !(node2 < node1);
			}

			else
			{
				return node1 == node2;
			}
		}

		template < typename LocalType >
		static constexpr bool _different_nodes(const LocalType &node1, const LocalType &node2)
		{
			return !graph::_equal_nodes(node1, node2);
		}

	public:
		[[nodiscard]] static consteval bool can_have_multiple_edges() noexcept
		{
			return graph::_multi;
		}

		[[nodiscard]] static consteval bool cannot_have_multiple_edges() noexcept
		{
			return graph::_single;
		}

		[[nodiscard]] static consteval bool ordered() noexcept
		{
			return graph::_ordered;
		}

		[[nodiscard]] static consteval bool unordered() noexcept
		{
			return graph::_unordered;
		}

		template < bool CopyNodes >
		using node_selection_type = typename std::conditional < CopyNodes, ValueType, value_reference_type > ::type;

	private:
		template < bool CopyNodes >
		using connections_container =
			typename std::conditional <
				graph::_ordered,
				std::map <
					node_selection_type < CopyNodes >,
					std::set < node_selection_type < CopyNodes > > >,
				std::unordered_map <
					node_selection_type < CopyNodes >,
					std::unordered_set < node_selection_type < CopyNodes > > > > ::type;

		template < bool CopyNodes >
		using betweenness_previous_container =
			typename std::conditional <
				graph::_ordered,
				std::map <
					node_selection_type < CopyNodes >,
					std::pair <
						size_type,
						std::set < node_selection_type < CopyNodes > > > >,
				std::unordered_map <
					node_selection_type < CopyNodes >,
					std::pair <
						size_type,
						std::unordered_set < node_selection_type < CopyNodes > > > > > ::type;

		template < bool CopyNodes >
		using betweenness_previous_container_set =
			typename std::conditional <
				graph::_ordered,
				std::map <
					node_selection_type < CopyNodes >,
					betweenness_previous_container < CopyNodes > >,
				std::unordered_map <
					node_selection_type < CopyNodes >,
					betweenness_previous_container < CopyNodes > > > ::type;

		template < bool CopyNodes >
		using edge_betweenness_centrality_previous_container_list =
			std::vector < std::pair <
				typename std::conditional <
					graph::_ordered,
					std::map <
						node_selection_type < CopyNodes >,
						std::tuple <
							size_type,
							std::set < node_selection_type < CopyNodes > >,
							edge_weight_type > >,
					std::unordered_map <
						node_selection_type < CopyNodes >,
						std::tuple <
							size_type,
							std::unordered_set < node_selection_type < CopyNodes > >,
							edge_weight_type > > > ::type,
				std::stack < node_selection_type < CopyNodes > > > >;

		template < bool CopyNodes >
		using node_node_container =
			typename std::conditional <
				graph::_ordered,
				std::map <
					node_selection_type < CopyNodes >,
					node_selection_type < CopyNodes > >,
				std::unordered_map <
					node_selection_type < CopyNodes >,
					node_selection_type < CopyNodes > > > ::type;

	public:
		template < bool CopyNodes >
		using degree_container =
			typename std::conditional <
				graph::_ordered,
				std::map < node_selection_type < CopyNodes >, edge_weight_type >,
				std::unordered_map < node_selection_type < CopyNodes >, edge_weight_type > > ::type;

		template < bool CopyNodes >
		using node_set_container =
			typename std::conditional <
				graph::_ordered,
				std::set < node_selection_type < CopyNodes > >,
				std::unordered_set < node_selection_type < CopyNodes > > > ::type;

		template < bool CopyNodes >
		using node_weight_container = degree_container < CopyNodes >;

		template < bool CopyNodes >
		struct less_pair
		{
			[[nodiscard]] constexpr bool operator()(
				const std::pair <
					node_selection_type < CopyNodes >,
					node_selection_type < CopyNodes > > &pair1,
				const std::pair <
					node_selection_type < CopyNodes >,
					node_selection_type < CopyNodes > > &pair2) const
			{
				return std::minmax(pair1.first, pair1.second) < std::minmax(pair2.first, pair2.second);
			}
		};

		template < bool CopyNodes >
		struct hash_pair
		{
			[[nodiscard]] constexpr auto operator()(
				const std::pair <
					node_selection_type < CopyNodes >,
					node_selection_type < CopyNodes > > &pair) const
			{
				using LocalType = node_selection_type < CopyNodes >;

				return std::hash < LocalType > {}(pair.first) ^ std::hash < LocalType > {}(pair.second);
			}
		};

		template < bool CopyNodes >
		struct equality_pair
		{
			[[nodiscard]] constexpr bool operator()(
				const std::pair <
					node_selection_type < CopyNodes >,
					node_selection_type < CopyNodes > > &pair1,
				const std::pair <
					node_selection_type < CopyNodes >,
					node_selection_type < CopyNodes > > &pair2) const
			{
				return
					graph::_equal_nodes(pair1.first, pair2.first) && graph::_equal_nodes(pair1.second, pair2.second) ||
					graph::_equal_nodes(pair1.first, pair2.second) && graph::_equal_nodes(pair1.second, pair2.first);
			}
		};

		template < bool CopyNodes >
		using matrix_index_to_graph_node_map =
			typename std::conditional <
				graph::_ordered,
				std::map < size_type, node_selection_type < CopyNodes > >,
				std::unordered_map < size_type, node_selection_type < CopyNodes > > > ::type;

		template < bool CopyNodes >
		using graph_node_to_matrix_index_map =
			typename std::conditional <
				graph::_ordered,
				std::map < node_selection_type < CopyNodes >, size_type >,
				std::unordered_map < node_selection_type < CopyNodes >, size_type > > ::type;

		template < bool CopyNodes >
		using edge_betweenness_centrality_container =
			typename std::conditional <
				graph::_ordered,
				std::map <
					std::pair <
						node_selection_type < CopyNodes >,
						node_selection_type < CopyNodes > >,
					edge_weight_type,
					less_pair < CopyNodes > >,
				std::unordered_map <
					std::pair <
						node_selection_type < CopyNodes >,
						node_selection_type < CopyNodes > >,
					edge_weight_type,
					hash_pair < CopyNodes >,
					equality_pair < CopyNodes > > > ::type;

		template < bool CopyNodes >
		using all_previous_nodes_container = connections_container < CopyNodes >;

		template < bool CopyNodes >
		using next_nodes_container = connections_container < CopyNodes >;

		template < bool CopyNodes >
		using all_previous_nodes_with_weight_container =
			typename std::conditional <
				graph::_ordered,
				std::map <
					node_selection_type < CopyNodes >,
					std::pair < node_set_container < CopyNodes >, edge_weight_type > >,
				std::unordered_map <
					node_selection_type < CopyNodes >,
					std::pair < node_set_container < CopyNodes >, edge_weight_type > > > ::type;

		template < bool CopyNodes >
		using next_nodes_with_weight_container =
			typename std::conditional <
				graph::_ordered,
				std::map <
					node_selection_type < CopyNodes >,
					std::map <
						node_selection_type < CopyNodes >,
						edge_weight_type > >,
				std::unordered_map <
					node_selection_type < CopyNodes >,
					std::unordered_map <
						node_selection_type < CopyNodes >,
						edge_weight_type > > > ::type;

		template < bool CopyNodes >
		using single_previous_node_container = node_node_container < CopyNodes >;

		template < bool CopyNodes >
		using single_previous_node_with_weight_container =
			typename std::conditional <
				graph::_ordered,
				std::map <
					node_selection_type < CopyNodes >,
					std::pair < node_selection_type < CopyNodes >, edge_weight_type > >,
				std::unordered_map <
					node_selection_type < CopyNodes >,
					std::pair < node_selection_type < CopyNodes >, edge_weight_type > > > ::type;

	private:
		template < bool CopyNodes >
		void connected_nodes(
			const ValueType &currentNode,
			node_set_container < CopyNodes > &visitedNodes) const
		{
			visitedNodes.insert(currentNode);

			for (const auto &[neighbor, edge] : this->_graph.at(currentNode))
			{
				if (!visitedNodes.contains(neighbor))
				{
					this->connected_nodes < CopyNodes > (neighbor, visitedNodes);
				}
			}
		}

		template < bool CopyNodes >
		node_set_container < CopyNodes > connected_nodes(const ValueType &node) const
		{
			node_set_container < CopyNodes > visitedNodes{};

			this->connected_nodes < CopyNodes > (node, visitedNodes);

			return visitedNodes;
		}

		edge_weight_type _weight_sum{};
		graph_container_type _graph{};
		degree_container < false > _degrees{};
		size_type _edge_count{};
		size_type _unique_edge_count{};
		size_type _expected_connections_by_node{};

	public:
		[[nodiscard]] bool operator==(const graph &graph) const
		{
			return this->_graph == graph._graph;
		}

		[[nodiscard]] bool operator<(const graph &graph) const
		requires (graph::ordered())
		{
			return this->_graph < graph._graph;
		}

		[[nodiscard]] edge_weight_type degree(const ValueType &node) const
		{
			const auto iterator = this->_degrees.find(node);

			if (iterator != this->_degrees.cend())
			{
				return iterator->second;
			}

			return edge_weight_type{};
		}

		[[nodiscard]] edge_weight_type average_degree() const
		{
			if (this->empty())
			{
				return edge_weight_type{};
			}

			return this->_weight_sum / static_cast < edge_weight_type > (this->node_count());
		}

		[[nodiscard]] edge_weight_type density() const
		{
			const auto nodeCount = this->node_count();

			if (nodeCount < 2)
			{
				throw std::domain_error("There are less than two nodes in the graph.");
			}

			return this->_weight_sum / static_cast < edge_weight_type > (nodeCount * (nodeCount - 1));
		}

		[[nodiscard]] edge_weight_type degree_distribution(const edge_weight_type degree) const
		{
			if (this->_graph.empty())
			{
				throw std::domain_error("There are no nodes in the graph.");
			}

			edge_weight_type probability{};

			for (const auto &item : this->_graph)
			{
				if (degree == this->degree(item.first))
				{
					++probability;
				}
			}

			return probability / static_cast < edge_weight_type > (this->node_count());
		}

		[[nodiscard]] std::map < edge_weight_type, edge_weight_type > degree_distribution() const
		{
			std::map < edge_weight_type, edge_weight_type > probabilities{};

			if (this->empty())
			{
				return probabilities;
			}

			for (const auto &item : this->_graph)
			{
				++(probabilities.insert(std::make_pair(this->degree(item.first), edge_weight_type{})).first->second);
			}

			for (auto &item : probabilities)
			{
				item.second /= static_cast < edge_weight_type > (this->node_count());
			}

			return probabilities;
		}

		[[nodiscard]] edge_weight_type statistical_moment(const size_type order) const
		{
			edge_weight_type result{};
			const auto degreeDistribution = this->degree_distribution();

			for (const auto &item : degreeDistribution)
			{
				result +=
					static_cast < edge_weight_type > (xablau::algebra::functions::pow < edge_weight_type, size_type > ::invoke(item.first, order)) * item.second;
			}

			return result;
		}

		[[nodiscard]] edge_weight_type variance() const
		{
			const auto statisticalMomentOrder1 = this->statistical_moment(1);

			return this->statistical_moment(2) - (statisticalMomentOrder1 * statisticalMomentOrder1);
		}

		[[nodiscard]] edge_weight_type volume() const noexcept
		{
			return this->_weight_sum / edge_weight_type{2};
		}

		[[nodiscard]] edge_weight_type complexity_measure() const
		{
			return this->statistical_moment(2) / this->statistical_moment(1);
		}

		[[nodiscard]] edge_weight_type Shannon_entropy() const
		{
			return internals::Shannon_entropy(*this);
		}

		[[nodiscard]] size_type triangle_count() const
		{
			size_type triangleCount{};

			for (const auto &[node, connections] : this->_graph)
			{
				if (connections.size() < 2)
				{
					continue;
				}

				for (auto i = connections.cbegin(), j = ++(connections.cbegin());
					j != connections.cend();
					j = ++i, ++j)
				{
					if (std::addressof(i->first.get()) == std::addressof(node))
					{
						continue;
					}

					const auto &iMap = this->_graph.at(i->first.get());

					for (; j != connections.cend(); ++j)
					{
						if (std::addressof(j->first.get()) == std::addressof(node))
						{
							continue;
						}

						const auto ijConnection = iMap.find(j->first);

						if (ijConnection != iMap.cend())
						{
							if constexpr (graph::can_have_multiple_edges())
							{
								triangleCount += i->second.size() * j->second.size() * ijConnection->second.size();
							}

							else
							{
								triangleCount++;
							}
						}
					}
				}
			}

			return triangleCount / 3;
		}

		[[nodiscard]] size_type triplet_count() const
		{
			size_type tripletCount{};

			for (const auto &[node, connections] : this->_graph)
			{
				size_type selfConnectionCount{};
				const auto iterator = connections.find(node);

				if (iterator != connections.cend())
				{
					if constexpr (graph::can_have_multiple_edges())
					{
						selfConnectionCount = iterator->second.size();
					}

					else
					{
						selfConnectionCount = size_type{1};
					}
				}

				const size_type edgeCount = this->edge_count(node) - selfConnectionCount;

				tripletCount += edgeCount * (edgeCount - 1) / 2;
			}

			return tripletCount;
		}

		// Global analysis. Works only on weightless graphs.
		[[nodiscard]] edge_weight_type clustering_coefficient() const noexcept
		{
			size_type triangleCount{};
			size_type tripletCount{};

			std::thread threadTriangleCount(
				[this, &triangleCount] () -> void
				{
					triangleCount = this->triangle_count();
				});

			std::thread threadTripletCount(
				[this, &tripletCount] () -> void
				{
					tripletCount = this->triplet_count();
				});

			threadTriangleCount.join();
			threadTripletCount.join();

			if (tripletCount != size_type{})
			{
				return (edge_weight_type{3} * static_cast < edge_weight_type > (triangleCount)) /
					static_cast < edge_weight_type > (tripletCount);
			}

			return edge_weight_type{};
		}

		template < bool CopyNodes >
		[[nodiscard]]
			std::vector <
				std::pair <
					typename std::conditional < CopyNodes, ValueType, std::reference_wrapper < const ValueType > > ::type,
					typename std::conditional < CopyNodes, ValueType, std::reference_wrapper < const ValueType > > ::type > > triangle_neighbors(const ValueType &node) const
		{
			std::vector <
				std::pair <
					typename std::conditional < CopyNodes, ValueType, std::reference_wrapper < const ValueType > > ::type,
					typename std::conditional < CopyNodes, ValueType, std::reference_wrapper < const ValueType > > ::type > > result;

			const auto iterator = this->_graph.find(node);

			if (iterator == this->_graph.cend())
			{
				return result;
			}

			for (auto nodeIterator1 = iterator->second.cbegin(); nodeIterator1 != iterator->second.cend(); ++nodeIterator1)
			{
				auto nodeIterator2 = nodeIterator1;

				while ((++nodeIterator2) != iterator->second.cend())
				{
					if (std::addressof(iterator->first) != std::addressof(nodeIterator1->first.get()) &&
						std::addressof(iterator->first) != std::addressof(nodeIterator2->first.get()) &&
						this->_graph.at(nodeIterator1->first.get()).contains(nodeIterator2->first))
					{
						result.emplace_back(nodeIterator1->first, nodeIterator2->first);
					}
				}
			}

			return result;
		}

		[[nodiscard]] edge_weight_type closeness_centrality(const ValueType &origin) const
		{
			const auto iterator = this->_graph.find(origin);

			if (iterator == this->_graph.cend())
			{
				return edge_weight_type{};
			}

			edge_weight_type closeness{};
			single_previous_node_with_weight_container < false > previous{};

			internals::generic_Dijkstra <
				Dijkstra_modes::single_previous_and_next_node_and_distance > (
					*this,
					iterator->first,
					previous);

			for (const auto &[_, nodeAndDistance] : previous)
			{
				closeness += nodeAndDistance.second;
			}

			return static_cast < edge_weight_type > (this->node_count() - 1) / closeness;
		}

		template < bool CopyNodes >
		[[nodiscard]] node_weight_container < CopyNodes > closeness_centrality() const
		{
			std::mutex mutex;
			std::vector < std::thread > threads;
			node_weight_container < CopyNodes > closeness{};

			if constexpr (graph::unordered())
			{
				closeness.reserve(this->node_count());
			}

			for (const auto &[origin, edges] : this->_graph)
			{
				// TODO: Deal with possible throws in these threads.
				threads.emplace_back(
					[this, &origin, &closeness, &mutex] () -> void
					{
						auto _closeness = this->closeness_centrality(origin);

						const std::lock_guard < std::mutex > lock(mutex);

						closeness.emplace(origin, std::move(_closeness));
					});
			}

			for (auto &thread : threads)
			{
				thread.join();
			}

			return closeness;
		}

		// Thanks, Anuradha Bhatia!
		// https://www.youtube.com/watch?v=ptqt2zr9ZRE
		[[nodiscard]] edge_weight_type node_betweenness_centrality(const ValueType &node) const
		{
			using MappedType =
				typename std::conditional <
					graph::_ordered,
					std::set < node_selection_type < false > >,
					std::unordered_set < node_selection_type < false > > > ::type;

			using PreviousContainerSet = betweenness_previous_container_set < false >;

			return internals::node_betweenness_centrality < MappedType, PreviousContainerSet > (*this, node);
		}

		// Thanks, Anuradha Bhatia!
		// https://www.youtube.com/watch?v=ptqt2zr9ZRE
		template < bool CopyNodes >
		[[nodiscard]] node_weight_container < CopyNodes > node_betweenness_centrality() const
		{
			using MappedType =
				typename std::conditional <
					graph::_ordered,
					std::set < node_selection_type < false > >,
					std::unordered_set < node_selection_type < false > > > ::type;

			using PreviousContainerSet = betweenness_previous_container_set < false >;

			return
				internals::node_betweenness_centrality <
					MappedType,
					PreviousContainerSet,
					node_weight_container < CopyNodes > > (*this);
		}

		// Thanks, Anuradha Bhatia!
		// https://www.youtube.com/watch?v=LtQoPEKKRYM
		template < bool CopyNodes >
		[[nodiscard]] edge_betweenness_centrality_container < CopyNodes > edge_betweenness_centrality() const
		{
			using MappedType =
				typename std::conditional <
					graph::_ordered,
					std::set < node_selection_type < false > >,
					std::unordered_set < node_selection_type < false > > > ::type;

			using PreviousContainerList = edge_betweenness_centrality_previous_container_list < false >;

			return
				internals::edge_betweenness_centrality <
					MappedType,
					PreviousContainerList,
					edge_betweenness_centrality_container < CopyNodes > > (*this);
		}

		// https://journals.aps.org/pre/pdf/10.1103/PhysRevE.69.026113
		// Weights cannot be negative.
		template < bool CheckCommunitiesClassification, typename CommunityType >
		requires (
			std::same_as < CommunityType, std::vector < node_set_container < false > > > ||
			std::same_as < CommunityType, std::vector < node_set_container < true > > >)
		[[nodiscard]] edge_weight_type modularity(const CommunityType &communitiesClassification) const
		{
			return
				internals::modularity <
					CheckCommunitiesClassification,
					node_set_container < false > > (*this, communitiesClassification, this->_weight_sum);
		}

		template <
			xablau::algebra::concepts::xablau_matrix_dense MatrixType,
			bool AllowAutoEdges,
			bool CopyNodes >
		requires (std::same_as < typename MatrixType::value_type, edge_weight_type >)
		[[nodiscard]] auto adjacency_matrix() const
		{
			using MatrixValueType = typename MatrixType::value_type;
			using GraphNodeToMatrixIndexMap = graph_node_to_matrix_index_map < CopyNodes >;
			using MatrixIndexToGraphNodeMap = matrix_index_to_graph_node_map < CopyNodes >;

			return
				internals::adjacency_matrix <
					MatrixType,
					AllowAutoEdges,
					true,
					GraphNodeToMatrixIndexMap,
					MatrixIndexToGraphNodeMap > (*this);
		}

		template < xablau::algebra::concepts::xablau_matrix_dense MatrixType, bool CopyNodes >
		requires (std::same_as < typename MatrixType::value_type, edge_weight_type >)
		[[nodiscard]] auto laplacian_matrix() const
		{
			using MatrixValueType = typename MatrixType::value_type;
			using GraphNodeToMatrixIndexMap = graph_node_to_matrix_index_map < CopyNodes >;
			using MatrixIndexToGraphNodeMap = matrix_index_to_graph_node_map < CopyNodes >;

			return
				internals::laplacian_matrix <
					MatrixType,
					true,
					GraphNodeToMatrixIndexMap,
					MatrixIndexToGraphNodeMap > (*this);
		}

		template < bool CopyNodes >
		[[nodiscard]]
			std::pair <
				all_previous_nodes_container < CopyNodes >,
				next_nodes_container < CopyNodes > >
			Dijkstra_all_previous_and_next_nodes(const ValueType &origin) const
		{
			const auto iterator = this->_graph.find(origin);

			if (iterator == this->_graph.cend())
			{
				return {};
			}

			all_previous_nodes_container < CopyNodes > previous{};
			next_nodes_container < CopyNodes > next{};

			using MappedType =
				typename std::conditional <
					graph::_ordered,
					std::set < node_selection_type < CopyNodes > >,
					std::unordered_set < node_selection_type < CopyNodes > > > ::type;

			internals::generic_Dijkstra <
				Dijkstra_modes::all_previous_and_next_nodes,
				MappedType > (
					*this,
					iterator->first,
					previous);

			internals::Dijkstra_create_next < Dijkstra_modes::all_previous_and_next_nodes, graph::unordered() > (previous, next);

			return std::make_pair(std::move(previous), std::move(next));
		}

		template < bool CopyNodes >
		[[nodiscard]]
			std::pair <
				all_previous_nodes_with_weight_container < CopyNodes >,
				next_nodes_with_weight_container < CopyNodes > >
			Dijkstra_all_previous_and_next_nodes_and_distance(const ValueType &origin) const
		{
			const auto iterator = this->_graph.find(origin);

			if (iterator == this->_graph.cend())
			{
				return {};
			}

			all_previous_nodes_with_weight_container < CopyNodes > previous{};
			next_nodes_with_weight_container < CopyNodes > next{};

			internals::generic_Dijkstra <
				Dijkstra_modes::all_previous_and_next_nodes_and_distance,
				node_set_container < CopyNodes > > (
					*this,
					iterator->first,
					previous);

			internals::Dijkstra_create_next < Dijkstra_modes::all_previous_and_next_nodes_and_distance, graph::unordered() > (previous, next);

			return std::make_pair(std::move(previous), std::move(next));
		}

		template < bool CopyNodes >
		[[nodiscard]]
			std::pair <
				single_previous_node_container < CopyNodes >,
				next_nodes_container < CopyNodes > >
			Dijkstra_single_previous_and_next_nodes(const ValueType &origin) const
		{
			const auto iterator = this->_graph.find(origin);

			if (iterator == this->_graph.cend())
			{
				return {};
			}

			single_previous_node_container < CopyNodes > previous{};
			next_nodes_container < CopyNodes > next{};

			internals::generic_Dijkstra <
				Dijkstra_modes::single_previous_and_next_node > (
					*this,
					iterator->first,
					previous);

			internals::Dijkstra_create_next < Dijkstra_modes::single_previous_and_next_node, graph::unordered() > (previous, next);

			return std::make_pair(std::move(previous), std::move(next));
		}

		template < bool CopyNodes >
		[[nodiscard]]
			std::pair <
				single_previous_node_with_weight_container < CopyNodes >,
				next_nodes_with_weight_container < CopyNodes > >
			Dijkstra_single_previous_and_next_nodes_and_distance(const ValueType &origin) const
		{
			const auto iterator = this->_graph.find(origin);

			if (iterator == this->_graph.cend())
			{
				return {};
			}

			single_previous_node_with_weight_container < CopyNodes > previous{};
			next_nodes_with_weight_container < CopyNodes > next{};

			internals::generic_Dijkstra <
				Dijkstra_modes::single_previous_and_next_node_and_distance > (
					*this,
					iterator->first,
					previous);

			internals::Dijkstra_create_next < Dijkstra_modes::single_previous_and_next_node_and_distance, graph::unordered() > (previous, next);

			return std::make_pair(std::move(previous), std::move(next));
		}

		template < bool CreatePath, bool CopyNodes >
		[[nodiscard]] auto Dijkstra_single_path(const ValueType &origin, const ValueType &destiny) const
		{
			return
				internals::Dijkstra_single_path <
					CreatePath,
					node_selection_type < CopyNodes >,
					node_node_container < false > > (*this, origin, destiny);
		}

		// It cannot have negative cycles.
		template < xablau::algebra::concepts::xablau_matrix_dynamic MatrixType, bool CopyNodes >
		requires (std::same_as < typename MatrixType::value_type, edge_weight_type >)
		[[nodiscard]] auto Floyd_Warshall() const
		{
			return
				internals::Floyd_Warshall <
					graph_node_to_matrix_index_map < CopyNodes >,
					matrix_index_to_graph_node_map < CopyNodes >,
					MatrixType,
					xablau::algebra::tensor_dense_dynamic <
						std::optional < node_selection_type < CopyNodes > >,
						xablau::algebra::tensor_rank < MatrixType::rank() >,
						MatrixType::memory_order_indices > > (*this);
		}

		// It cannot have negative cycles.
		template < xablau::algebra::concepts::xablau_matrix_fixed MatrixType, bool CopyNodes >
		requires (std::same_as < typename MatrixType::value_type, edge_weight_type >)
		[[nodiscard]] auto Floyd_Warshall() const
		{
			return
				internals::Floyd_Warshall <
					graph_node_to_matrix_index_map < CopyNodes >,
					matrix_index_to_graph_node_map < CopyNodes >,
					MatrixType,
					xablau::algebra::tensor_dense_fixed <
						std::optional < node_selection_type < CopyNodes > >,
						MatrixType::fixed_dimensionalities,
						MatrixType::memory_order_indices > > (*this);
		}

		// Adapted from https://www.geeksforgeeks.org/traveling-salesman-problem-tsp-implementation/
		template < bool CopyNodes >
		[[nodiscard]] auto traveling_salesman_problem(const ValueType &start) const
		requires (std::totally_ordered < edge_weight_type > && graph::cannot_have_multiple_edges())
		{
			return
				internals::traveling_salesman_problem < node_selection_type < CopyNodes > > (*this, start);
		}

		template < bool CopyNodes, concepts::digraph DependencyType >
		requires (std::same_as < ValueType, typename DependencyType::value_type >)
		[[nodiscard]] auto traveling_salesman_problem(const ValueType &start, const DependencyType &dependency) const
		requires (std::totally_ordered < edge_weight_type > && graph::cannot_have_multiple_edges())
		{
			return
				internals::traveling_salesman_problem <
					node_selection_type < CopyNodes > > (
						*this,
						graph::_equal_nodes < ValueType >,
						graph::_different_nodes < ValueType >,
						start,
						dependency);
		}

		template < concepts::nary_tree TreeType, tree_generation_modes TreeGenerationMode >
		requires (std::same_as < typename TreeType::value_type, ValueType >)
		[[nodiscard]] TreeType generate_tree(const ValueType &origin) const
		{
			return
				internals::generate_tree <
					connections_container < false >,
					node_set_container < false >,
					TreeType,
					TreeGenerationMode > (*this, origin);
		}

		[[nodiscard]] bool connected() const
		{
			if (this->_graph.empty())
			{
				return true;
			}

			return this->connected_nodes < false > (this->_graph.begin()->first).size() == this->node_count();
		}

		[[nodiscard]] std::vector < graph > dismember_disconnected_nodes() const
		{
			std::vector < graph > connectedGraphs;

			for (const auto &[thisNode, _] : this->_graph)
			{
				bool newGraph = true;

				for (const auto &connectedGraph : connectedGraphs)
				{
					if (connectedGraph.contains(thisNode))
					{
						newGraph = false;

						break;
					}
				}

				if (newGraph)
				{
					auto nodes = this->connected_nodes < false > (thisNode);
					graph graph;

					if constexpr (graph::unordered())
					{
						graph._graph.reserve(this->node_count());
						graph._degrees.reserve(this->node_count());
						graph._expected_connections_by_node = this->_expected_connections_by_node;
					}

					for (const auto &node1 : nodes)
					{
						const auto connections = this->connections(node1);

						if (connections.has_value())
						{
							for (const auto &[node2, edges] : connections.value().get())
							{
								if constexpr (graph::can_have_multiple_edges())
								{
									for (const auto &edge : edges)
									{
										graph.insert(node1, node2, edge);
									}
								}
								
								else
								{
									graph.insert(node1, node2, edges);
								}
							}
						}
					}

					connectedGraphs.push_back(std::move(graph));
				}
			}

			return connectedGraphs;
		}

		template < bool CopyNodes >
		[[nodiscard]] node_weight_container < CopyNodes > random_walk() const
		{
			if (this->_graph.empty())
			{
				return node_weight_container < CopyNodes > {};
			}

			size_type counter{};
			graph_node_to_matrix_index_map < false > graphNodeToMatrixIndex{};
			matrix_index_to_graph_node_map < false > matrixIndexToGraphNode{};

			for (const auto &[node, _] : this->_graph)
			{
				matrixIndexToGraphNode[counter] = node;
				graphNodeToMatrixIndex[node] = counter;

				counter++;
			}

			const auto matrixSize = this->_graph.size();
			xablau::algebra::tensor_dense_dynamic <
				edge_weight_type,
				xablau::algebra::tensor_rank < 2 > > matrix(matrixSize, matrixSize);

			std::for_each(std::execution::par_unseq, this->_graph.cbegin(), this->_graph.cend(),
				[&matrix, &graphNodeToMatrixIndex] (
					const typename graph_container_type::value_type &item1) -> void
				{
					for (const auto &item2 : item1.second)
					{
						if constexpr (graph::can_have_multiple_edges())
						{
							matrix(graphNodeToMatrixIndex.at(item1.first), graphNodeToMatrixIndex.at(item2.first)) =
								edge_weight_type{1} / std::min_element(item2.second.cbegin(), item2.second.cend())->weight();
						}

						else
						{
							matrix(graphNodeToMatrixIndex.at(item1.first), graphNodeToMatrixIndex.at(item2.first)) =
								edge_weight_type{1} / item2.second.weight();
						}
					}
				});

			size_type indexGreatestAbsoluteEigenvalue{};
			edge_weight_type greatestAbsoluteEigenvalue{};
			const auto [eigenvalues, eigenvectors] = matrix.eigen_Hermitian < true, false > ();

			for (size_type i = 0; i < eigenvalues.rows(); i++)
			{
				const auto &eigenvalue = eigenvalues(i, 0);

				if (eigenvalue > greatestAbsoluteEigenvalue)
				{
					greatestAbsoluteEigenvalue = eigenvalue;
					indexGreatestAbsoluteEigenvalue = i;
				}
			}

			node_weight_container < CopyNodes > centrality{};

			if constexpr (graph::unordered())
			{
				centrality.reserve(this->node_count());
			}

			for (size_type i = 0; i < matrixSize; i++)
			{
				centrality[matrixIndexToGraphNode.at(i)] =
					xablau::algebra::functions::abs < edge_weight_type > ::invoke(eigenvectors(i, indexGreatestAbsoluteEigenvalue));
			}

			return centrality;
		}

		[[nodiscard]] graph shuffle() const
		{
			graph newGraph;

			if constexpr (graph::unordered())
			{
				const auto nodeCount = this->node_count();

				newGraph._expected_connections_by_node = this->_expected_connections_by_node;
				newGraph._graph.reserve(nodeCount);
				newGraph._degrees.reserve(nodeCount);
			}

			internals::shuffle < node_node_container < false > > (*this, newGraph);

			return newGraph;
		}

		[[nodiscard]] bool empty() const noexcept
		{
			return this->_graph.empty();
		}

		[[nodiscard]] size_type node_count() const noexcept
		{
			return this->_graph.size();
		}

		[[nodiscard]] size_type expected_connections_by_node() const
		requires (graph::unordered())
		{
			return this->_expected_connections_by_node;
		}

		void clear() noexcept
		{
			this->_weight_sum = edge_weight_type{};
			this->_graph.clear();
			this->_degrees.clear();
			this->_edge_count = size_type{};
			this->_unique_edge_count = size_type{};
		}

		const ValueType &insert(const ValueType &node)
		{
			auto iterator = this->_graph.find(node);

			if (iterator != this->_graph.end())
			{
				return iterator->first;
			}

			if constexpr (graph::unordered())
			{
				iterator = this->_graph.emplace(node, typename graph_container_type::mapped_type(this->_expected_connections_by_node)).first;
			}

			else
			{
				iterator = this->_graph.emplace(node, typename graph_container_type::mapped_type()).first;
			}

			this->_degrees[iterator->first] = edge_weight_type{};

			return iterator->first;
		}

		std::optional < std::reference_wrapper < const EdgeType > >
			insert(
				const ValueType &node1,
				const ValueType &node2,
				const EdgeType &edge = EdgeType{})
		{
			const auto weight = edge.weight();

			if (weight == edge_weight_type{})
			{
				return std::nullopt;
			}

			const auto &_node1 = this->insert(node1);
			const auto &_node2 = this->insert(node2);
			std::optional < std::reference_wrapper < const EdgeType > > insertedEdge{};

			if constexpr (graph::can_have_multiple_edges())
			{
				auto _edge = this->_graph.at(_node1).emplace(_node2, std::vector < EdgeType > ());

				_edge.first->second.push_back(edge);

				if (_edge.second)
				{
					++(this->_unique_edge_count);
				}

				if (std::addressof(_node1) == std::addressof(_node2))
				{
					insertedEdge = _edge.first->second.back();
				}

				else
				{
					auto transposedEdge = this->_graph.at(_node2).emplace(_node1, std::vector < EdgeType > ());

					transposedEdge.first->second.push_back(edge);

					insertedEdge = transposedEdge.first->second.back();
				}
			}

			else
			{
				const auto _edge = this->_graph.at(_node1).emplace(_node2, edge);

				if (!_edge.second)
				{
					return _edge.first->second;
				}

				insertedEdge = this->_graph.at(_node2).emplace(_node1, edge).first->second;

				++(this->_unique_edge_count);
			}

			this->_degrees[_node1] += weight;
			this->_degrees[_node2] += weight;

			++(this->_edge_count);

			this->_weight_sum += weight + weight;

			return insertedEdge;
		}

		size_type erase(const ValueType &node)
		{
			const auto transposedNodesIterator = this->_graph.find(node);

			if (transposedNodesIterator == this->_graph.end())
			{
				return 0;
			}

			size_type removedEdgesCount{};

			for (auto iterator = transposedNodesIterator->second.cbegin();
				iterator != transposedNodesIterator->second.cend();
				++iterator)
			{
				const auto &transposedNode = iterator->first.get();
				const auto connections = this->_graph.find(transposedNode);
				auto &connection = connections->second.at(transposedNodesIterator->first);
				const bool nodesAreNotTheSame = transposedNodesIterator != connections;
				edge_weight_type weight{};

				if constexpr (graph::can_have_multiple_edges())
				{
					weight =
						std::accumulate(connection.cbegin(), connection.cend(), edge_weight_type{},
							[] (const EdgeType &connection1, const EdgeType &connection2) -> edge_weight_type
							{
								return connection1.weight() + connection2.weight();
							});

					this->_edge_count -= connection.size();

					removedEdgesCount += connection.size();
				}

				else
				{
					weight = connection.weight();

					--(this->_edge_count);

					removedEdgesCount++;
				}

				this->_weight_sum -= weight + weight;
				--(this->_unique_edge_count);

				if (nodesAreNotTheSame)
				{
					this->_degrees.at(transposedNode) -= weight;
					connections->second.erase(transposedNodesIterator->first);
				}
			}

			this->_degrees.erase(transposedNodesIterator->first);
			this->_graph.erase(transposedNodesIterator);

			return removedEdgesCount;
		}

		size_type erase(const ValueType &node1, const ValueType &node2)
		{
			const auto iterator1 = this->_graph.find(node1);

			if (iterator1 == this->_graph.end())
			{
				return size_type{};
			}

			auto &connections = iterator1->second;
			const auto iterator2 = connections.find(node2);

			if (iterator2 == connections.end())
			{
				return size_type{};
			}

			const bool nodesAreTheSame = std::addressof(iterator1->first) == std::addressof(iterator2->first.get());
			edge_weight_type weight{};
			size_type removedEdgesCount{1};

			if constexpr (graph::can_have_multiple_edges())
			{
				const auto &edges = iterator2->second;

				removedEdgesCount = edges.size();

				weight =
					std::accumulate(edges.cbegin(), edges.cend(), edge_weight_type{},
						[] (const EdgeType &edge1, const EdgeType &edge2) -> edge_weight_type
						{
							return edge1.weight() + edge2.weight();
						});

				this->_edge_count -= removedEdgesCount;
			}

			else
			{
				weight = iterator2->second.weight();

				--(this->_edge_count);
			}

			this->_weight_sum -= weight + weight;

			this->_degrees.at(iterator1->first) -= weight;
			this->_degrees.at(iterator2->first.get()) -= weight;

			--(this->_unique_edge_count);

			if (!nodesAreTheSame)
			{
				this->_graph.at(iterator2->first.get()).erase(iterator1->first);
			}

			connections.erase(iterator2);

			return removedEdgesCount;
		}

		[[nodiscard]] bool contains(const ValueType &node) const
		{
			return this->_graph.contains(node);
		}

		[[nodiscard]] bool contains(const ValueType &node1, const ValueType &node2) const
		{
			const auto iterator = this->_graph.find(node1);

			if (iterator != this->_graph.cend()) [[likely]]
			{
				const auto &edges = iterator->second;

				return edges.contains(node2);
			}

			return false;
		}

		[[nodiscard]] const graph_container_type &container() const noexcept
		{
			return this->_graph;
		}

		[[nodiscard]] std::optional < std::reference_wrapper < const typename graph_container_type::mapped_type > >
			connections(const ValueType &node) const
		{
			const auto iterator = this->_graph.find(node);

			if (iterator != this->_graph.cend())
			{
				return iterator->second;
			}

			return std::nullopt;
		}

		[[nodiscard]] std::optional < std::reference_wrapper < const typename graph_container_type::mapped_type::mapped_type > >
			edges(const ValueType &node1, const ValueType &node2) const
		{
			const auto iterator1 = this->_graph.find(node1);

			if (iterator1 != this->_graph.cend())
			{
				const auto &connections = iterator1->second;
				const auto iterator2 = connections.find(node2);

				if (iterator2 != connections.cend())
				{
					return iterator2->second;
				}
			}

			return std::nullopt;
		}

		[[nodiscard]] edge_weight_type weight(const ValueType &node1, const ValueType &node2) const
		{
			const auto iterator1 = this->_graph.find(node1);

			if (iterator1 != this->_graph.cend())
			{
				const auto &connections = iterator1->second;
				const auto iterator2 = connections.find(node2);

				if (iterator2 != connections.cend())
				{
					if constexpr (graph::can_have_multiple_edges())
					{
						const auto &edges = iterator2->second;

						return
							std::accumulate(edges.cbegin(), edges.cend(), edge_weight_type{},
								[] (const EdgeType &edge1, const EdgeType &edge2) -> edge_weight_type
								{
									return edge1.weight() + edge2.weight();
								});
					}

					else
					{
						return iterator2->second.weight();
					}
				}
			}

			return edge_weight_type{};
		}

		[[nodiscard]] size_type edge_count() const noexcept
		{
			return this->_edge_count;
		}

		[[nodiscard]] size_type edge_count(const ValueType &node) const
		{
			const auto iterator = this->_graph.find(node);

			if (iterator == this->_graph.cend())
			{
				return size_type{};
			}

			const auto &connections = iterator->second;

			if constexpr (graph::can_have_multiple_edges())
			{
				size_type edgeCount{};

				for (const auto &[_, edges] : connections)
				{
					edgeCount += edges.size();
				}

				return edgeCount;
			}

			else
			{
				return connections.size();
			}
		}

		[[nodiscard]] size_type unique_edge_count() const noexcept
		{
			return this->_unique_edge_count;
		}

		[[nodiscard]] size_type unique_edge_count(const ValueType &node) const
		{
			const auto iterator = this->_graph.find(node);

			if (iterator != this->_graph.cend())
			{
				return iterator->second.size();
			}

			return size_type{};
		}

		template < bool CopyNodes >
		[[nodiscard]] node_set_container < CopyNodes > nodes() const
		{
			node_set_container < CopyNodes > nodes{};

			if constexpr (graph::unordered())
			{
				nodes.reserve(this->node_count());
			}

			for (const auto &[node, _] : this->_graph)
			{
				nodes.insert(node);
			}

			return nodes;
		}

		friend std::ostream &operator<<(std::ostream &stream, const graph &graph)
		requires (graph::cannot_have_multiple_edges())
		{
			size_type accumulator1{};
			const size_type nodeCount = graph.node_count();

			stream << "*--* (";

			if (nodeCount != size_type{})
			{
				stream << '\n';
			}

			for (const auto &[node1, connections] : graph._graph)
			{
				stream << "* " << node1 << " --\n";

				size_type accumulator2{};
				const size_type edgeCount = connections.size();

				for (const auto &[node2, edge] : connections)
				{
					stream << "\t * " << node2.get() << ": " << edge;

					accumulator2++;

					if (accumulator2 != edgeCount)
					{
						stream << '\n';
					}
				}

				accumulator1++;

				if (accumulator1 != nodeCount)
				{
					stream << '\n';
				}
			}

			stream << " )";

			return stream;
		}

		graph &operator=(const graph &) = default;

		graph &operator=(graph &&) noexcept = default;

		graph() = default;

		graph(const graph &) = default;

		graph(graph &&) noexcept = default;

		explicit graph(const size_type expectedNodes)
		requires (graph::unordered()) :
			_graph(expectedNodes),
			_degrees(expectedNodes)
		{
		}

		explicit graph(const size_type expectedNodes, const size_type expectedConnectionsByNode)
		requires (graph::unordered()) :
			_graph(expectedNodes),
			_degrees(expectedNodes),
			_expected_connections_by_node(expectedConnectionsByNode)
		{
		}
	};
}