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

export module xablau.graph:digraph;
export import :fundamental_concepts;
export import :graph;
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
	class digraph final
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
			if constexpr (digraph::_ordered)
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
			return !digraph::_equal_nodes(node1, node2);
		}

	public:
		[[nodiscard]] static consteval bool can_have_multiple_edges() noexcept
		{
			return digraph::_multi;
		}

		[[nodiscard]] static consteval bool cannot_have_multiple_edges() noexcept
		{
			return digraph::_single;
		}

		[[nodiscard]] static consteval bool ordered() noexcept
		{
			return digraph::_ordered;
		}

		[[nodiscard]] static consteval bool unordered() noexcept
		{
			return digraph::_unordered;
		}

		template < bool CopyNodes >
		using node_selection_type = typename std::conditional < CopyNodes, ValueType, value_reference_type > ::type;

		template < bool CopyNodes >
		using connections_container =
			typename std::conditional <
				digraph::_ordered,
				std::map <
					node_selection_type < CopyNodes >,
					std::set < node_selection_type < CopyNodes > > >,
				std::unordered_map <
					node_selection_type < CopyNodes >,
					std::unordered_set < node_selection_type < CopyNodes > > > > ::type;

	private:
		template < bool CopyNodes >
		using betweenness_previous_container =
			typename std::conditional <
				digraph::_ordered,
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
				digraph::_ordered,
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
					digraph::_ordered,
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
				digraph::_ordered,
				std::map <
					node_selection_type < CopyNodes >,
					node_selection_type < CopyNodes > >,
				std::unordered_map <
					node_selection_type < CopyNodes >,
					node_selection_type < CopyNodes > > > ::type;

		template < bool CopyNodes >
		using transposed_graph_container = connections_container < CopyNodes >;

		template < bool CopyNodes >
		using Tarjan_stack =
			std::stack <
				node_selection_type < CopyNodes >,
				std::vector < node_selection_type < CopyNodes > > >;

		template < bool CopyNodes >
		using info_on_Tarjan_stack =
			typename std::conditional <
				digraph::_ordered,
				std::map <
					node_selection_type < CopyNodes >,
					std::tuple < size_type, size_type, bool > >,
				std::unordered_map <
					node_selection_type < CopyNodes >,
					std::tuple < size_type, size_type, bool > > > ::type;

	public:
		template < bool CopyNodes >
		using degree_container =
			typename std::conditional <
				digraph::_ordered,
				std::map < node_selection_type < CopyNodes >, edge_weight_type >,
				std::unordered_map < node_selection_type < CopyNodes >, edge_weight_type > > ::type;

		template < bool CopyNodes >
		using node_set_container =
			typename std::conditional <
				digraph::_ordered,
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
					digraph::_equal_nodes(pair1.first, pair2.first) && digraph::_equal_nodes(pair1.second, pair2.second) ||
					digraph::_equal_nodes(pair1.first, pair2.second) && digraph::_equal_nodes(pair1.second, pair2.first);
			}
		};

		template < bool CopyNodes >
		using matrix_index_to_graph_node_map =
			typename std::conditional <
				digraph::_ordered,
				std::map < size_type, node_selection_type < CopyNodes > >,
				std::unordered_map < size_type, node_selection_type < CopyNodes > > > ::type;

		template < bool CopyNodes >
		using graph_node_to_matrix_index_map =
			typename std::conditional <
				digraph::_ordered,
				std::map < node_selection_type < CopyNodes >, size_type >,
				std::unordered_map < node_selection_type < CopyNodes >, size_type > > ::type;

		template < bool CopyNodes >
		using edge_betweenness_centrality_container =
			typename std::conditional <
				digraph::_ordered,
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
				digraph::_ordered,
				std::map <
					node_selection_type < CopyNodes >,
					std::pair < node_set_container < CopyNodes >, edge_weight_type > >,
				std::unordered_map <
					node_selection_type < CopyNodes >,
					std::pair < node_set_container < CopyNodes >, edge_weight_type > > > ::type;

		template < bool CopyNodes >
		using next_nodes_with_weight_container =
			typename std::conditional <
				digraph::_ordered,
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
				digraph::_ordered,
				std::map <
					node_selection_type < CopyNodes >,
					std::pair < node_selection_type < CopyNodes >, edge_weight_type > >,
				std::unordered_map <
					node_selection_type < CopyNodes >,
					std::pair < node_selection_type < CopyNodes >, edge_weight_type > > > ::type;

	private:
		void Tarjan_strongly_connected_components(
			const ValueType &node,
			const typename graph_container_type::mapped_type &neighbors,
			size_type &index,
			Tarjan_stack < false > &stack,
			std::vector < node_set_container < false > > &stronglyConnectedComponents,
			info_on_Tarjan_stack < false > &nodeIndexLowlinkOnStack) const
		{
			auto &[_index, _lowlink, _onStack] = nodeIndexLowlinkOnStack[node];

			_index = index;
			_lowlink = index;
			_onStack = true;

			index++;

			stack.push(node);

			for (const auto &[neighbor, edge] : neighbors)
			{
				if (!nodeIndexLowlinkOnStack.contains(neighbor))
				{
					digraph::Tarjan_strongly_connected_components(
						neighbor, this->_digraph.at(neighbor), index, stack, stronglyConnectedComponents, nodeIndexLowlinkOnStack);

					_lowlink = std::min(_lowlink, std::get < 1 > (nodeIndexLowlinkOnStack[neighbor]));
				}

				else if (std::get < 2 > (nodeIndexLowlinkOnStack[neighbor]))
				{
					_lowlink = std::min(_lowlink, std::get < 0 > (nodeIndexLowlinkOnStack[neighbor]));
				}
			}

			if (_lowlink == _index)
			{
				typename node_set_container < false > ::iterator iterator{};

				stronglyConnectedComponents.emplace_back();

				do
				{
					auto stronglyConnectedNode = std::move(stack.top());

					std::get < 2 > (nodeIndexLowlinkOnStack[stronglyConnectedNode]) = false;

					iterator = stronglyConnectedComponents.back().insert(std::move(stronglyConnectedNode)).first;

					stack.pop();
				} while (std::addressof(iterator->get()) != std::addressof(node));
			}
		}

		edge_weight_type _weight_sum{};
		graph_container_type _digraph{};
		transposed_graph_container < false > _transposed_digraph{};
		degree_container < false > _degrees{};
		size_type _edge_count{};
		size_type _unique_edge_count{};
		size_type _expected_connections_by_node{};

	public:
		[[nodiscard]] bool operator==(const digraph &digraph) const
		{
			return this->_digraph == digraph._digraph;
		}

		[[nodiscard]] bool operator<(const digraph &digraph) const
		requires (digraph::ordered())
		{
			return this->_digraph < digraph._digraph;
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
			if (this->_digraph.empty())
			{
				throw std::domain_error("There are no nodes in the digraph.");
			}

			edge_weight_type probability{};

			for (const auto &item : this->_digraph)
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

			for (const auto &item : this->_digraph)
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
			return this->_weight_sum;
		}

		[[nodiscard]] edge_weight_type complexity_measure() const
		{
			return this->statistical_moment(2) / this->statistical_moment(1);
		}

		[[nodiscard]] edge_weight_type Shannon_entropy() const
		{
			return internals::Shannon_entropy(*this);
		}

		[[nodiscard]] edge_weight_type closeness_centrality(const ValueType &origin) const
		{
			const auto iterator = this->_digraph.find(origin);

			if (iterator == this->_digraph.cend())
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

			if constexpr (digraph::unordered())
			{
				closeness.reserve(this->node_count());
			}

			for (const auto &[origin, edges] : this->_digraph)
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
					digraph::_ordered,
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
					digraph::_ordered,
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
					digraph::_ordered,
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
					false,
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
					false,
					GraphNodeToMatrixIndexMap,
					MatrixIndexToGraphNodeMap > (*this);
		}

		[[nodiscard]] connections_container < false > Tarjan_strongly_connected_components() const
		{
			size_t index{};
			Tarjan_stack < false > stack{};
			std::vector < node_set_container < false > > stronglyConnectedComponents;
			info_on_Tarjan_stack < false > nodeIndexLowlinkOnStack{};

			for (const auto &node : this->_digraph)
			{
				if (!nodeIndexLowlinkOnStack.contains(node.first))
				{
					this->Tarjan_strongly_connected_components(
						node.first, node.second, index, stack, stronglyConnectedComponents, nodeIndexLowlinkOnStack);
				}
			}

			connections_container < false > _stronglyConnectedNodes{};

			if constexpr (digraph::unordered())
			{
				_stronglyConnectedNodes.reserve(this->node_count());
			}

			for (const auto &stronglyConnectedComponent : stronglyConnectedComponents)
			{
				for (const auto &node : stronglyConnectedComponent)
				{
					_stronglyConnectedNodes[node] = stronglyConnectedComponent;
				}
			}

			return _stronglyConnectedNodes;
		}

		template < bool SmallerWeightSum >
		[[nodiscard]] graph < ValueType, ContainerType, EdgeType > convert_to_graph_using_weight_sum_comparison() const
		{
			const auto graphInitialization =
				[this] () -> auto
				{
					using GraphDataType = graph < ValueType, ContainerType, EdgeType >;

					if constexpr (digraph::unordered())
					{
						return GraphDataType(this->node_count(), this->_expected_connections_by_node);
					}

					else
					{
						return GraphDataType{};
					}
				};

			auto graph = graphInitialization();

			for (const auto &node : this->_digraph)
			{
				for (const auto &edge : node.second)
				{
					const auto weight = this->weight(node.first, edge.first);
					const auto transposedWeight = this->weight(edge.first, node.first);

					if constexpr (SmallerWeightSum)
					{
						if (weight < transposedWeight)
						{
							graph.insert(edge.first, node.first, weight);
						}

						else
						{
							graph.insert(edge.first, node.first, transposedWeight);
						}
					}

					else
					{
						if (weight > transposedWeight)
						{
							graph.insert(edge.first, node.first, weight);
						}

						else
						{
							graph.insert(edge.first, node.first, transposedWeight);
						}
					}
				}
			}

			return graph;
		}

		template < bool SmallestWeight >
		[[nodiscard]] graph < ValueType, ContainerType, EdgeType > convert_to_graph_using_one_weight_comparison() const
		requires (digraph::can_have_multiple_edges())
		{
			const auto graphInitialization =
				[this] () -> auto
				{
					using GraphDataType = graph < ValueType, ContainerType, EdgeType >;

					if constexpr (digraph::unordered())
					{
						return GraphDataType(this->node_count(), this->_expected_connections_by_node);
					}

					else
					{
						return GraphDataType{};
					}
				};

			auto graph = graphInitialization();

			for (const auto &node : this->_digraph)
			{
				for (const auto &edges : node.second)
				{
					const auto _transposedEdges = this->getEdges(edges.first, node.first);

					if (!_transposedEdges.has_value())
					{
						continue;
					}

					const auto &transposedEdges = _transposedEdges.value().get();

					if constexpr (SmallestWeight)
					{
						if (std::min_element(edges.second.cbegin(), edges.second.cend()) <
							std::min_element(transposedEdges.cbegin(), transposedEdges.cend()))
						{
							for (const auto &edge : edges.second)
							{
								graph.insert(node.first, edges.first, edge);
							}
						}

						else
						{
							for (const auto &edge : transposedEdges)
							{
								graph.insert(node.first, edges.first, edge);
							}
						}
					}

					else
					{
						if (std::max_element(edges.second.cbegin(), edges.second.cend()) >
							std::max_element(transposedEdges.cbegin(), transposedEdges.cend()))
						{
							for (const auto &edge : edges.second)
							{
								graph.insert(node.first, edges.first, edge);
							}
						}

						else
						{
							for (const auto &edge : transposedEdges)
							{
								graph.insert(node.first, edges.first, edge);
							}
						}
					}
				}
			}

			return graph;
		}

		template < bool CopyNodes >
		[[nodiscard]]
			std::pair <
				all_previous_nodes_container < CopyNodes >,
				next_nodes_container < CopyNodes > >
			Dijkstra_all_previous_and_next_nodes(const ValueType &origin) const
		{
			const auto iterator = this->_digraph.find(origin);

			if (iterator == this->_digraph.cend())
			{
				return {};
			}

			all_previous_nodes_container < CopyNodes > previous{};
			next_nodes_container < CopyNodes > next{};

			using MappedType =
				typename std::conditional <
					digraph::_ordered,
					std::set < node_selection_type < CopyNodes > >,
					std::unordered_set < node_selection_type < CopyNodes > > > ::type;

			internals::generic_Dijkstra <
				Dijkstra_modes::all_previous_and_next_nodes,
				MappedType > (
					*this,
					iterator->first,
					previous);

			internals::Dijkstra_create_next < Dijkstra_modes::all_previous_and_next_nodes, digraph::unordered() > (previous, next);

			return std::make_pair(std::move(previous), std::move(next));
		}

		template < bool CopyNodes >
		[[nodiscard]]
			std::pair <
				all_previous_nodes_with_weight_container < CopyNodes >,
				next_nodes_with_weight_container < CopyNodes > >
			Dijkstra_all_previous_and_next_nodes_and_distance(const ValueType &origin) const
		{
			const auto iterator = this->_digraph.find(origin);

			if (iterator == this->_digraph.cend())
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

			internals::Dijkstra_create_next < Dijkstra_modes::all_previous_and_next_nodes_and_distance, digraph::unordered() > (previous, next);

			return std::make_pair(std::move(previous), std::move(next));
		}

		template < bool CopyNodes >
		[[nodiscard]]
			std::pair <
				single_previous_node_container < CopyNodes >,
				next_nodes_container < CopyNodes > >
			Dijkstra_single_previous_and_next_nodes(const ValueType &origin) const
		{
			const auto iterator = this->_digraph.find(origin);

			if (iterator == this->_digraph.cend())
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

			internals::Dijkstra_create_next < Dijkstra_modes::single_previous_and_next_node, digraph::unordered() > (previous, next);

			return std::make_pair(std::move(previous), std::move(next));
		}

		template < bool CopyNodes >
		[[nodiscard]]
			std::pair <
				single_previous_node_with_weight_container < CopyNodes >,
				next_nodes_with_weight_container < CopyNodes > >
			Dijkstra_single_previous_and_next_nodes_and_distance(const ValueType &origin) const
		{
			const auto iterator = this->_digraph.find(origin);

			if (iterator == this->_digraph.cend())
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

			internals::Dijkstra_create_next < Dijkstra_modes::single_previous_and_next_node_and_distance, digraph::unordered() > (previous, next);

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
						xablau::algebra::tensor_contiguity < MatrixType::contiguous() >,
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
						xablau::algebra::tensor_contiguity < MatrixType::contiguous() >,
						MatrixType::memory_order_indices > > (*this);
		}

		// Adapted from https://www.geeksforgeeks.org/traveling-salesman-problem-tsp-implementation/
		template < bool CopyNodes >
		[[nodiscard]] auto traveling_salesman_problem(const ValueType &start) const
		requires (std::totally_ordered < edge_weight_type > && digraph::cannot_have_multiple_edges())
		{
			return
				internals::traveling_salesman_problem < node_selection_type < CopyNodes > > (*this, start);
		}

		template < bool CopyNodes, concepts::digraph DependencyType >
		requires (std::same_as < ValueType, typename DependencyType::value_type >)
		[[nodiscard]] auto traveling_salesman_problem(const ValueType &start, const DependencyType &dependency) const
		requires (std::totally_ordered < edge_weight_type > && digraph::cannot_have_multiple_edges())
		{
			return
				internals::traveling_salesman_problem <
					node_selection_type < CopyNodes > > (
						*this,
						digraph::_equal_nodes < ValueType >,
						digraph::_different_nodes < ValueType >,
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
			return this->convert_to_graph_using_weight_sum_comparison < false > ().connected();
		}

		[[nodiscard]] std::vector < digraph > dismember_disconnected_nodes() const
		{
			const auto vectorInitialization =
				[this] () -> digraph
				{
					if constexpr (digraph::unordered())
					{
						return digraph(this->node_count(), this->_expected_connections_by_node);
					}

					else
					{
						return digraph{};
					}
				};

			const auto graph = this->convert_to_graph_using_weight_sum_comparison < false > ();
			auto connectedGraphs = graph.dismember_disconnected_nodes();
			std::vector < digraph > connectedDigraphs(connectedGraphs.size(), vectorInitialization());

			for (typename std::vector < digraph > ::size_type i = 0; i < connectedDigraphs.size(); i++)
			{
				const auto nodes = connectedGraphs[i].nodes < false > ();

				for (const auto &node1 : nodes)
				{
					for (const auto &[node2, edges] : this->_digraph.at(node1.get()))
					{
						if constexpr (digraph::can_have_multiple_edges())
						{
							for (const auto &edge : edges)
							{
								connectedDigraphs[i].insert(node1, node2, edge);
							}
						}

						else
						{
							connectedDigraphs[i].insert(node1, node2, edges);
						}
					}
				}
			}

			return connectedDigraphs;
		}

		[[nodiscard]] digraph transpose() const
		{
			digraph localDigraph;

			if constexpr (digraph::unordered())
			{
				localDigraph._expected_connections_by_node = this->_expected_connections_by_node;
				localDigraph._digraph.reserve(this->node_count());
				localDigraph._transposed_digraph.reserve(this->node_count());
				localDigraph._degrees.reserve(this->node_count());
			}

			for (const auto &item1 : this->_digraph)
			{
				localDigraph.insert(item1.first);

				for (const auto &item2 : item1.second)
				{
					if constexpr (digraph::can_have_multiple_edges())
					{
						for (const auto &edge : item2.second)
						{
							localDigraph.insert(item2.first, item1.first, edge);
						}
					}

					else
					{
						localDigraph.insert(item2.first, item1.first, item2.second);
					}
				}
			}

			return localDigraph;
		}

		[[nodiscard]] digraph shuffle() const
		{
			digraph newDigraph;

			if constexpr (digraph::unordered())
			{
				const auto nodeCount = this->node_count();

				newDigraph._expected_connections_by_node = this->_expected_connections_by_node;
				newDigraph._digraph.reserve(nodeCount);
				newDigraph._transposed_digraph.reserve(nodeCount);
				newDigraph._degrees.reserve(nodeCount);
			}

			internals::shuffle < node_node_container < false > > (*this, newDigraph);

			return newDigraph;
		}

		[[nodiscard]] bool empty() const noexcept
		{
			return this->_digraph.empty();
		}

		[[nodiscard]] size_type node_count() const noexcept
		{
			return this->_digraph.size();
		}

		[[nodiscard]] size_type expected_connections_by_node() const
		requires (digraph::unordered())
		{
			return this->_expected_connections_by_node;
		}

		void clear() noexcept
		{
			this->_weight_sum = edge_weight_type{};
			this->_digraph.clear();
			this->_transposed_digraph.clear();
			this->_degrees.clear();
			this->_edge_count = size_type{};
			this->_unique_edge_count = size_type{};
		}

		const ValueType &insert(const ValueType &node)
		{
			auto iterator = this->_digraph.find(node);

			if (iterator != this->_digraph.end())
			{
				return iterator->first;
			}

			if constexpr (digraph::unordered())
			{
				iterator = this->_digraph.emplace(node, typename graph_container_type::mapped_type(this->_expected_connections_by_node)).first;
				this->_transposed_digraph.emplace(iterator->first, node_set_container < false > (this->_expected_connections_by_node));
			}

			else
			{
				iterator = this->_digraph.emplace(node, typename graph_container_type::mapped_type()).first;
				this->_transposed_digraph.emplace(iterator->first, node_set_container < false > {});
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

			if constexpr (digraph::can_have_multiple_edges())
			{
				auto _edge = this->_digraph.at(_node1).emplace(_node2, std::vector < EdgeType > ());

				_edge.first->second.push_back(edge);

				if (_edge.second)
				{
					++(this->_unique_edge_count);
				}

				insertedEdge = _edge.first->second.back();
			}

			else
			{
				const auto _edge = this->_digraph.at(_node1).emplace(_node2, edge);

				if (!_edge.second)
				{
					return _edge.first->second;
				}

				insertedEdge = _edge.first->second;

				++(this->_unique_edge_count);
			}

			this->_transposed_digraph.at(_node2).insert(_node1);

			this->_degrees[_node1] += weight;

			++(this->_edge_count);

			this->_weight_sum += weight;

			return insertedEdge;
		}

		size_type erase(const ValueType &node)
		{
			const auto nodeIterator = this->_digraph.find(node);

			if (nodeIterator == this->_digraph.end())
			{
				return 0;
			}

			size_type removedEdgesCount{};

			const auto transposedNodesIterator = this->_transposed_digraph.find(node);

			for (const auto &transposedNode : transposedNodesIterator->second)
			{
				const auto connections = this->_digraph.find(transposedNode);
				const auto &connection = connections->second.at(node);
				edge_weight_type weight{};

				if (nodeIterator == connections)
				{
					continue;
				}

				if constexpr (digraph::can_have_multiple_edges())
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

				this->_degrees.at(transposedNode) -= weight;
				this->_weight_sum -= weight;
				--(this->_unique_edge_count);

				connections->second.erase(transposedNodesIterator->first);
			}

			const auto &connections = nodeIterator->second;
			edge_weight_type weight{};

			for (const auto &[_node, connection] : connections)
			{
				if constexpr (digraph::can_have_multiple_edges())
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

				this->_weight_sum -= weight;
				--(this->_unique_edge_count);

				this->_transposed_digraph.at(_node).erase(nodeIterator->first);
			}

			this->_transposed_digraph.erase(nodeIterator->first);
			this->_degrees.erase(nodeIterator->first);
			this->_digraph.erase(nodeIterator);

			return removedEdgesCount;
		}

		size_type erase(const ValueType &node1, const ValueType &node2)
		{
			const auto iterator1 = this->_digraph.find(node1);

			if (iterator1 == this->_digraph.end())
			{
				return size_type{};
			}

			auto &connections = iterator1->second;
			const auto iterator2 = connections.find(node2);

			if (iterator2 == connections.end())
			{
				return size_type{};
			}

			edge_weight_type weight{};
			size_type removedEdgesCount{1};

			if constexpr (digraph::can_have_multiple_edges())
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

			this->_weight_sum -= weight;

			this->_degrees.at(iterator1->first) -= weight;

			--(this->_unique_edge_count);

			this->_transposed_digraph.at(iterator2->first.get()).erase(iterator1->first);

			connections.erase(iterator2);

			return removedEdgesCount;
		}

		[[nodiscard]] bool contains(const ValueType &node) const
		{
			return this->_digraph.contains(node);
		}

		[[nodiscard]] bool contains(const ValueType &node1, const ValueType &node2) const
		{
			const auto iterator = this->_digraph.find(node1);

			if (iterator != this->_digraph.cend()) [[likely]]
			{
				const auto &edges = iterator->second;

				return edges.contains(node2);
			}

			return false;
		}

		[[nodiscard]] const graph_container_type &container() const noexcept
		{
			return this->_digraph;
		}

		[[nodiscard]] std::optional < std::reference_wrapper < const typename graph_container_type::mapped_type > >
			connections(const ValueType &node) const
		{
			const auto iterator = this->_digraph.find(node);

			if (iterator != this->_digraph.cend())
			{
				return iterator->second;
			}

			return std::nullopt;
		}

		[[nodiscard]] std::optional < std::reference_wrapper < const typename graph_container_type::mapped_type::mapped_type > >
			edges(const ValueType &node1, const ValueType &node2) const
		{
			const auto iterator1 = this->_digraph.find(node1);

			if (iterator1 != this->_digraph.cend())
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
			const auto iterator1 = this->_digraph.find(node1);

			if (iterator1 != this->_digraph.cend())
			{
				const auto &connections = iterator1->second;
				const auto iterator2 = connections.find(node2);

				if (iterator2 != connections.cend())
				{
					if constexpr (digraph::can_have_multiple_edges())
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
			const auto iterator = this->_digraph.find(node);

			if (iterator == this->_digraph.cend())
			{
				return size_type{};
			}

			const auto &connections = iterator->second;

			if constexpr (digraph::can_have_multiple_edges())
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
			const auto iterator = this->_digraph.find(node);

			if (iterator != this->_digraph.cend())
			{
				return iterator->second.size();
			}

			return size_type{};
		}

		template < bool CopyNodes >
		[[nodiscard]] node_set_container < CopyNodes > nodes() const
		{
			node_set_container < CopyNodes > nodes{};

			if constexpr (digraph::unordered())
			{
				nodes.reserve(this->node_count());
			}

			for (const auto &[node, _] : this->_digraph)
			{
				nodes.insert(node);
			}

			return nodes;
		}

		template < bool CopyNodes >
		[[nodiscard]] node_set_container < CopyNodes > source_nodes() const
		{
			node_set_container < CopyNodes > sourceNodes{};

			if constexpr (digraph::unordered())
			{
				sourceNodes.reserve(this->node_count());
			}

			for (const auto &item : this->_digraph)
			{
				if (this->_transposed_digraph.at(item.first).empty())
				{
					sourceNodes.insert(item.first);
				}
			}

			return sourceNodes;
		}

		template < bool CopyNodes >
		[[nodiscard]] node_set_container < CopyNodes > sink_nodes() const
		{
			node_set_container < CopyNodes > sinkNodes{};

			if constexpr (digraph::unordered())
			{
				sinkNodes.reserve(this->node_count());
			}

			for (const auto &item : this->_digraph)
			{
				if (item.second.empty())
				{
					sinkNodes.insert(item.first);
				}
			}

			return sinkNodes;
		}

		friend std::ostream &operator<<(std::ostream &stream, const digraph &digraph)
		requires (digraph::cannot_have_multiple_edges())
		{
			size_type accumulator1{};
			const size_type nodeCount = digraph.node_count();

			stream << "*->* (";

			if (nodeCount != size_type{})
			{
				stream << '\n';
			}

			for (const auto &[node1, connections] : digraph._digraph)
			{
				stream << "* " << node1 << " ->\n";

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

		digraph &operator=(const digraph &) = default;

		digraph &operator=(digraph &&) noexcept = default;

		digraph() = default;

		digraph(const digraph &) = default;

		digraph(digraph &&) noexcept = default;

		explicit digraph(const size_type expectedNodes)
		requires (digraph::unordered()) :
			_digraph(expectedNodes),
			_degrees(expectedNodes)
		{
		}

		explicit digraph(const size_type expectedNodes, const size_type expectedConnectionsByNode)
		requires (digraph::unordered()) :
			_digraph(expectedNodes),
			_degrees(expectedNodes),
			_expected_connections_by_node(expectedConnectionsByNode)
		{
		}
	};
}