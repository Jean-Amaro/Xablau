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

export module xablau.graph:graph;
export import :fundamental_concepts;
export import :nary_tree;

export import <execution>;
export import <iostream>;
export import <limits>;
export import <map>;
export import <mutex>;
export import <queue>;
export import <random>;
export import <set>;
export import <stack>;
export import <thread>;
export import <tuple>;
export import <unordered_map>;
export import <unordered_set>;

export import xablau.algebra;

export namespace xablau::graph
{
	template <
		concepts::node NodeType,
		concepts::graph_container_type ContainerType,
		concepts::edge EdgeType >
	requires (concepts::graph_requirements < NodeType, ContainerType >)
	class graph final
	{
	public:
		using node_type = NodeType;
		using graph_container_type = ContainerType;
		using edge_type = EdgeType;
		using edge_weight_type = typename EdgeType::weight_type;

		using graph_container =
			typename std::conditional < ContainerType::type() == graph_container_type_value::ordered,
				std::map < NodeType, std::map < NodeType, EdgeType > >,

			typename std::conditional < ContainerType::type() == graph_container_type_value::unordered,
				std::unordered_map < NodeType, std::unordered_map < NodeType, EdgeType > >,

			typename std::conditional < ContainerType::type() == graph_container_type_value::multi_ordered,
				std::map < NodeType, std::map < NodeType, std::vector < EdgeType > > >,

				std::unordered_map < NodeType, std::unordered_map < NodeType, std::vector < EdgeType > > > >::type >::type >::type;

		using size_type = typename graph_container::size_type;

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

		static constexpr bool _equal_nodes(const NodeType &node1, const NodeType &node2)
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

		static constexpr bool _different_nodes(const NodeType &node1, const NodeType &node2)
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

	private:
		using connections_container =
			typename std::conditional <
				graph::_ordered,
				std::map < NodeType, std::set < NodeType > >,
				std::unordered_map < NodeType, std::unordered_set < NodeType > > > ::type;

		using betweenness_previous_container =
			typename std::conditional <
				graph::_ordered,
				std::map < NodeType, std::pair < size_type, std::set < NodeType > > >,
				std::unordered_map < NodeType, std::pair < size_type, std::unordered_set < NodeType > > > > ::type;

		using betweenness_previous_container_set =
			typename std::conditional <
				graph::_ordered,
				std::map < NodeType, betweenness_previous_container >,
				std::unordered_map < NodeType, betweenness_previous_container > > ::type;

		using edge_betweenness_centrality_previous_container_list =
			std::vector < std::pair <
				typename std::conditional <
					graph::_ordered,
					std::map < NodeType, std::tuple < size_type, std::set < NodeType >, edge_weight_type > >,
					std::unordered_map < NodeType, std::tuple < size_type, std::unordered_set < NodeType >, edge_weight_type > > > ::type,
				std::stack < NodeType > > >;

		using node_node_container =
			typename std::conditional <
				graph::_ordered,
				std::map < NodeType, NodeType >,
				std::unordered_map < NodeType, NodeType > > ::type;

	public:
		using degree_container =
			typename std::conditional <
				graph::_ordered,
				std::map < NodeType, edge_weight_type >,
				std::unordered_map < NodeType, edge_weight_type > > ::type;

		using node_set_container =
			typename std::conditional <
				graph::_ordered,
				std::set < NodeType >,
				std::unordered_set < NodeType > > ::type;

		using node_weight_container = degree_container;

		struct less_pair
		{
			[[nodiscard]] constexpr bool operator()(
				const std::pair < NodeType, NodeType > &pair1,
				const std::pair < NodeType, NodeType > &pair2) const
			{
				return std::minmax(pair1.first, pair1.second) < std::minmax(pair2.first, pair2.second);
			}
		};

		struct hash_pair
		{
			[[nodiscard]] constexpr auto operator()(const std::pair < NodeType, NodeType > &pair) const
			{
				return std::hash < NodeType > {}(pair.first) ^ std::hash < NodeType > {}(pair.second);
			}
		};

		struct equality_pair
		{
			[[nodiscard]] constexpr bool operator()(
				const std::pair < NodeType, NodeType > &pair1,
				const std::pair < NodeType, NodeType > &pair2) const
			{
				return
					graph::_equal_nodes(pair1.first, pair2.first) && graph::_equal_nodes(pair1.second, pair2.second) ||
					graph::_equal_nodes(pair1.first, pair2.second) && graph::_equal_nodes(pair1.second, pair2.first);
			}
		};

		using matrix_index_to_graph_node_map =
			typename std::conditional <
				graph::_ordered,
				std::map < size_type, NodeType >,
				std::unordered_map < size_type, NodeType > > ::type;

		using graph_node_to_matrix_index_map =
			typename std::conditional <
				graph::_ordered,
				std::map < NodeType, size_type >,
				std::unordered_map < NodeType, size_type > > ::type;

		using edge_betweenness_centrality_container =
			typename std::conditional <
				graph::_ordered,
				std::map < std::pair < NodeType, NodeType >, edge_weight_type, less_pair >,
				std::unordered_map < std::pair < NodeType, NodeType >, edge_weight_type, hash_pair, equality_pair > > ::type;

		using all_previous_nodes_container = connections_container;

		using next_nodes_container = connections_container;
		
		using all_previous_nodes_with_weight_container =
			typename std::conditional <
				graph::_ordered,
				std::map < NodeType, std::pair < node_set_container, edge_weight_type > >,
				std::unordered_map < NodeType, std::pair < node_set_container, edge_weight_type > > > ::type;

		using next_nodes_with_weight_container =
			typename std::conditional <
				graph::_ordered,
				std::map < NodeType, std::map < NodeType, edge_weight_type > >,
				std::unordered_map < NodeType, std::unordered_map < NodeType, edge_weight_type > > > ::type;

		using single_previous_node_container = node_node_container;

		using single_previous_node_with_weight_container =
			typename std::conditional <
				graph::_ordered,
				std::map < NodeType, std::pair < NodeType, edge_weight_type > >,
				std::unordered_map < NodeType, std::pair < NodeType, edge_weight_type > > > ::type;

	private:
		static size_type triple_sum(const size_type number) noexcept
		{
			return (number + 1) * (number / 2) + (number % 2 ? (number + 1) / 2 : 0);
		}

		template < Dijkstra_modes Mode >
		void generic_Dijkstra(
			const NodeType &origin,
			auto &previous,
			const NodeType &destiny = NodeType{},
			std::optional < std::reference_wrapper < edge_weight_type > > smallestDistance = std::nullopt) const
		{
			if constexpr (Mode == Dijkstra_modes::single_path)
			{
				smallestDistance.value().get() = std::numeric_limits < edge_weight_type > ::max();

				if (!this->contains(origin) || !this->contains(destiny))
				{
					return;
				}
			}

			else
			{
				if (!this->contains(origin))
				{
					return;
				}
			}

			NodeType searchIndex = origin;
			std::vector < std::pair < NodeType, edge_weight_type > > distances;

			distances.reserve(this->node_count());

			constexpr auto orderingFunction =
				[] (const std::pair < NodeType, edge_weight_type > &pair1,
					const std::pair < NodeType, edge_weight_type > &pair2) -> bool
				{
					return pair1.second < pair2.second;
				};

			const auto searchFunction =
				[&searchIndex] (const std::pair < NodeType, edge_weight_type > &pair) -> bool
				{
					return graph::_equal_nodes(pair.first, searchIndex);
				};

			for (const auto &item : this->_graph)
			{
				distances.push_back(
					std::make_pair(item.first, std::numeric_limits < edge_weight_type > ::max()));
			}

			std::find_if(distances.begin(), distances.end(), searchFunction)->second = edge_weight_type{};

			if constexpr (Mode == Dijkstra_modes::edge_betweenness)
			{
				if constexpr (graph::unordered())
				{
					previous.first.reserve(this->node_count() * this->_expected_edges_by_node);
				}
			}

			else
			{
				if constexpr (graph::unordered())
				{
					previous.reserve(this->node_count() * this->_expected_edges_by_node);
				}
			}

			if constexpr (Mode == Dijkstra_modes::all_previous_and_next_nodes)
			{
				previous.insert(std::make_pair(origin, node_set_container{}));
			}

			else if constexpr (Mode == Dijkstra_modes::all_previous_and_next_nodes_and_distance)
			{
				previous.insert(
					std::make_pair(
						origin,
						std::make_pair(node_set_container{}, edge_weight_type{})));
			}

			else if constexpr (Mode == Dijkstra_modes::node_betweenness)
			{
				previous.insert(
					std::make_pair(
						origin,
						std::make_pair(1, node_set_container{})));
			}

			else if constexpr (Mode == Dijkstra_modes::edge_betweenness)
			{
				previous.first.insert(
					std::make_pair(
						origin,
						std::make_tuple(1, node_set_container{}, edge_weight_type{})));
			}

			for (size_type i = 0; i < this->node_count(); i++)
			{
				std::sort(distances.rbegin(), distances.rend(), orderingFunction);

				const auto &bestNode = distances.back();

				if (bestNode.second == std::numeric_limits < edge_weight_type > ::max())
				{
					break;
				}

				if constexpr (Mode == Dijkstra_modes::edge_betweenness)
				{
					previous.second.push(bestNode.first);
				}

				for (const auto &neighbor : this->_graph.at(bestNode.first))
				{
					searchIndex = neighbor.first;
					edge_weight_type currentDistance = distances.back().second;

					if constexpr (graph::can_have_multiple_edges())
					{
						currentDistance += std::min_element(neighbor.second.cbegin(), neighbor.second.cend())->weight();
					}

					else
					{
						currentDistance += neighbor.second.weight();
					}

					const auto search = std::find_if(distances.begin(), distances.end(), searchFunction);

					if constexpr (
						Mode == Dijkstra_modes::single_previous_and_next_node ||
						Mode == Dijkstra_modes::single_previous_and_next_node_and_distance ||
						Mode == Dijkstra_modes::single_path)
					{
						if (search != distances.end() && currentDistance < search->second)
						{
							search->second = currentDistance;

							if constexpr (Mode == Dijkstra_modes::single_previous_and_next_node_and_distance)
							{
								previous[neighbor.first] = std::make_pair(bestNode.first, currentDistance);
							}

							else
							{
								previous[neighbor.first] = bestNode.first;
							}
						}
					}

					else if constexpr (
						Mode == Dijkstra_modes::all_previous_and_next_nodes ||
						Mode == Dijkstra_modes::all_previous_and_next_nodes_and_distance ||
						Mode == Dijkstra_modes::node_betweenness ||
						Mode == Dijkstra_modes::edge_betweenness)
					{
						if (search != distances.end())
						{
							if (currentDistance < search->second)
							{
								size_type howManyPathsToGetHere{};

								if constexpr (Mode == Dijkstra_modes::node_betweenness)
								{
									howManyPathsToGetHere = previous.at(bestNode.first).first;
								}

								else if constexpr (Mode == Dijkstra_modes::edge_betweenness)
								{
									howManyPathsToGetHere = std::get < 0 > (previous.first.at(bestNode.first));
								}
								
								search->second = currentDistance;

								if constexpr (Mode == Dijkstra_modes::all_previous_and_next_nodes)
								{
									(previous[neighbor.first] = node_set_container{}).insert(bestNode.first);
								}

								else if constexpr (Mode == Dijkstra_modes::all_previous_and_next_nodes_and_distance)
								{
									(previous[neighbor.first] =
										std::make_pair(node_set_container{}, currentDistance)).
											first.insert(bestNode.first);
								}

								else if constexpr (Mode == Dijkstra_modes::node_betweenness)
								{
									(previous[neighbor.first] =
										std::make_pair(howManyPathsToGetHere, node_set_container{})).second.insert(bestNode.first);
								}

								else if constexpr (Mode == Dijkstra_modes::edge_betweenness)
								{
									std::get < 1 > (previous.first[neighbor.first] =
										std::make_tuple(howManyPathsToGetHere, node_set_container{}, edge_weight_type{})).
											insert(bestNode.first);
								}
							}

							else if (currentDistance == search->second)
							{
								if constexpr (Mode == Dijkstra_modes::all_previous_and_next_nodes)
								{
									previous[neighbor.first].insert(bestNode.first);
								}

								else if constexpr (Mode == Dijkstra_modes::all_previous_and_next_nodes_and_distance)
								{
									previous[neighbor.first].first.insert(bestNode.first);
								}

								else if constexpr (Mode == Dijkstra_modes::node_betweenness)
								{
									const size_type pathCount = previous.at(bestNode.first).first;

									previous[neighbor.first].first += pathCount;
									previous[neighbor.first].second.insert(bestNode.first);
								}

								else if constexpr (Mode == Dijkstra_modes::edge_betweenness)
								{
									const size_type pathCount = std::get < 0 > (previous.first.at(bestNode.first));

									std::get < 0 > (previous.first[neighbor.first]) += pathCount;
									std::get < 1 > (previous.first[neighbor.first]).insert(bestNode.first);
								}
							}
						}
					}
				}

				if constexpr (Mode == Dijkstra_modes::single_path)
				{
					if (graph::_equal_nodes(bestNode.first, destiny))
					{
						smallestDistance.value().get() = bestNode.second;

						break;
					}
				}

				distances.pop_back();
			}
		}

		template < Dijkstra_modes Mode >
		static void Dijkstra_create_next(auto &previous, auto &next)
		{
			if constexpr (Mode == Dijkstra_modes::all_previous_and_next_nodes)
			{
				for (const auto &[node1, otherNodes] : previous)
				{
					for (const auto &node2 : otherNodes)
					{
						next[node2].insert(node1);
					}
				}
			}

			else if constexpr (Mode == Dijkstra_modes::all_previous_and_next_nodes_and_distance)
			{
				for (const auto &[node1, otherNodesAndWeight] : previous)
				{
					for (const auto &node2 : otherNodesAndWeight.first)
					{
						next[node2][node1] = otherNodesAndWeight.second;
					}
				}
			}

			else if constexpr (Mode == Dijkstra_modes::single_previous_and_next_node)
			{
				for (const auto &[node1, node2] : previous)
				{
					next[node2].insert(node1);
				}
			}

			else if constexpr (Mode == Dijkstra_modes::single_previous_and_next_node_and_distance)
			{
				for (const auto &[node1, node2AndWeight] : previous)
				{
					next[node2AndWeight.first][node1] = node2AndWeight.second;
				}
			}
		}

		void connected_nodes(
			const NodeType &currentNode,
			node_set_container &visitedNodes) const
		{
			visitedNodes.insert(currentNode);

			for (const auto &[neighbor, edge] : this->_graph.at(currentNode))
			{
				if (!visitedNodes.contains(neighbor))
				{
					this->connected_nodes(neighbor, visitedNodes);
				}
			}
		}

		node_set_container connected_nodes(const NodeType &node) const
		{
			node_set_container visitedNodes{};

			this->connected_nodes(node, visitedNodes);

			return visitedNodes;
		}

		connections_container breadth_first_search(const NodeType &origin) const
		{
			connections_container next{};
			node_set_container visitedNodes{};
			std::queue < NodeType > queue;

			if (!this->contains(origin))
			{
				return next;
			}

			if constexpr (graph::unordered())
			{
				const auto nodeCount = this->node_count();

				next.reserve(nodeCount);
				visitedNodes.reserve(nodeCount);
			}

			visitedNodes.insert(origin);
			queue.push(origin);

			while (!queue.empty())
			{
				const auto currentNode = queue.front();

				queue.pop();

				for (const auto &[neighbor, edge] : this->_graph.at(currentNode))
				{
					if (!visitedNodes.contains(neighbor))
					{
						visitedNodes.insert(neighbor);

						if constexpr (graph::unordered())
						{
							next[currentNode].reserve(this->_expected_edges_by_node);
						}

						next[currentNode].insert(neighbor);

						queue.push(neighbor);
					}
				}
			}

			return next;
		}

		connections_container depth_first_search(const NodeType &origin) const
		{
			using node_pair = std::pair < NodeType, NodeType >;

			connections_container next{};
			node_set_container visitedNodes{};
			std::stack < node_pair, std::vector < node_pair > > stack;
			node_pair previousAndCurrentNodes{ origin, origin };

			if (!this->contains(origin))
			{
				return next;
			}

			if constexpr (graph::unordered())
			{
				next.reserve(this->node_count());
				visitedNodes.reserve(this->node_count());
			}

			stack.push(previousAndCurrentNodes);

			while (!stack.empty())
			{
				previousAndCurrentNodes = stack.top();

				stack.pop();

				if (!visitedNodes.contains(previousAndCurrentNodes.second))
				{
					visitedNodes.insert(previousAndCurrentNodes.second);

					if (graph::_different_nodes(previousAndCurrentNodes.first, previousAndCurrentNodes.second))
					{
						if constexpr (graph::unordered())
						{
							next[previousAndCurrentNodes.first].reserve(this->_expected_edges_by_node);
						}

						next[previousAndCurrentNodes.first].insert(previousAndCurrentNodes.second);
					}

					for (const auto &[neighbor, edge] : this->_graph.at(previousAndCurrentNodes.second))
					{
						stack.push(std::make_pair(previousAndCurrentNodes.second, neighbor));
					}
				}
			}

			return next;
		}

		static edge_weight_type betweenness_centrality_of_a_node_in_a_path(
			const NodeType &target,
			const NodeType &origin,
			const NodeType &destiny,
			const betweenness_previous_container &previous)
		{
			NodeType currentNode = destiny;
			std::queue < NodeType > queue;

			while (graph::_different_nodes(currentNode, origin))
			{
				if (graph::_equal_nodes(currentNode, target))
				{
					return
						static_cast < edge_weight_type > (previous.at(currentNode).first) /
							static_cast < edge_weight_type > (previous.at(destiny).first);
				}

				for (auto node : previous.at(currentNode).second)
				{
					queue.push(std::move(node));
				}

				currentNode = queue.front();

				queue.pop();
			}

			return edge_weight_type{};
		}

		betweenness_previous_container_set calculate_previous_for_node_betweenness_centrality() const
		{
			std::vector < std::thread > threads;
			betweenness_previous_container_set previous{};

			if constexpr (graph::unordered())
			{
				previous.reserve(this->node_count());
			}

			for (const auto &[node, edges] : this->_graph)
			{
				threads.emplace_back(
					[this, &node, &previous] () -> void
					{
						this->template generic_Dijkstra < Dijkstra_modes::node_betweenness > (node, previous[node]);
					});
			}

			for (auto &thread : threads)
			{
				thread.join();
			}

			return previous;
		}

		edge_betweenness_centrality_previous_container_list calculate_previous_for_edge_betweenness_centrality() const
		{
			size_type counter{};
			std::vector < std::thread > threads;
			edge_betweenness_centrality_previous_container_list previous(this->node_count());

			for (const auto &[node, edges] : this->_graph)
			{
				threads.emplace_back(
					[this, &node, &previous, counter] () -> void
					{
						this->template generic_Dijkstra < Dijkstra_modes::edge_betweenness > (node, previous[counter]);
					});

				counter++;
			}

			for (auto &thread : threads)
			{
				thread.join();
			}

			return previous;
		}

		template <
			xablau::algebra::concepts::xablau_matrix_dense MatrixType,
			xablau::algebra::concepts::xablau_matrix_dense NextNodeType >
		requires (std::same_as < typename MatrixType::value_type, edge_weight_type >)
		auto Floyd_Warshall() const
		{
			using MatrixValueType = MatrixType::value_type;

			size_type counter{};
			const size_type matrixLineLength = this->node_count();

			if constexpr (MatrixType::fixed())
			{
				if (matrixLineLength != MatrixType::dimensionalities()[0] ||
					matrixLineLength != MatrixType::dimensionalities()[1])
				{
					throw std::runtime_error(""); // TODO: Create message.
				}
			}

			MatrixType distanceMatrix{};
			NextNodeType nextNode{};

			if constexpr (!MatrixType::fixed())
			{
				distanceMatrix.resize(matrixLineLength, matrixLineLength);
			}

			distanceMatrix.fill(std::numeric_limits < MatrixValueType > ::max());

			if constexpr (!MatrixType::fixed())
			{
				nextNode.resize(matrixLineLength, matrixLineLength);
			}

			nextNode.fill(std::nullopt);

			graph_node_to_matrix_index_map graphNodeToMatrixIndex{};
			matrix_index_to_graph_node_map matrixIndexToGraphNode{};

			if constexpr (graph::unordered())
			{
				graphNodeToMatrixIndex.reserve(matrixLineLength);
				matrixIndexToGraphNode.reserve(matrixLineLength);
			}

			for (const auto &node : this->_graph)
			{
				graphNodeToMatrixIndex[node.first] = counter;
				matrixIndexToGraphNode[counter] = node.first;

				counter++;
			}

			for (const auto &node1 : this->_graph)
			{
				const size_type lineIndex1 = graphNodeToMatrixIndex[node1.first];

				for (const auto &node2 : node1.second)
				{
					const size_type lineIndex2 = graphNodeToMatrixIndex[node2.first];

					if constexpr (graph::can_have_multiple_edges())
					{
						distanceMatrix(lineIndex1, lineIndex2) =
							std::min_element(node2.second.cbegin(), node2.second.cend())->weight();
					}

					else
					{
						distanceMatrix(lineIndex1, lineIndex2) = node2.second.weight();
					}

					nextNode(lineIndex1, lineIndex2) = node2.first;
				}

				distanceMatrix(lineIndex1, lineIndex1) = MatrixValueType{};
				nextNode(lineIndex1, lineIndex1) = node1.first;
			}

			for (size_type k = 0; k < matrixLineLength; k++)
			{
				for (size_type i = 0; i < matrixLineLength; i++)
				{
					const auto distanceIK = distanceMatrix(i, k);

					for (size_type j = 0; j < matrixLineLength; j++)
					{
						if (distanceMatrix(i, j) > distanceIK + distanceMatrix(k, j))
						{
							distanceMatrix(i, j) = distanceIK + distanceMatrix(k, j);
							nextNode(i, j) = nextNode(i, k);
						}
					}
				}
			}

			return std::make_tuple(distanceMatrix, nextNode, graphNodeToMatrixIndex, matrixIndexToGraphNode);
		}

		edge_weight_type _weight_sum{};
		graph_container _graph{};
		degree_container _degrees{};
		size_type _edge_count{};
		size_type _unique_edge_count{};
		size_type _expected_edges_by_node{};

	public:
		[[nodiscard]] bool operator==(const graph &graph) const
		{
			return this->_graph == graph._graph;
		}

		[[nodiscard]] edge_weight_type degree(const NodeType &node) const
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
			const auto statistical_moment_order1 = this->statistical_moment(1);

			return this->statistical_moment(2) - (statistical_moment_order1 * statistical_moment_order1);
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
			const auto degree_distribution = this->degree_distribution();

			if (degree_distribution.empty())
			{
				return edge_weight_type{};
			}

			edge_weight_type acumulator{};

			for (const auto &item : degree_distribution)
			{
				if (item.second != edge_weight_type{})
				{
					acumulator -=
						item.second * xablau::algebra::functions::log2 < edge_weight_type > ::invoke(item.second);
				}
			}

			return acumulator;
		}

		[[nodiscard]] size_type triangle_count() const
		{
			size_type triangleCount{};

			for (const auto &item : this->_graph)
			{
				const auto &edges = item.second;

				if (edges.size() >= 2)
				{
					for (auto i = edges.cbegin(), j = ++(edges.cbegin());
						j != edges.cend();
						j = ++i, ++j)
					{
						if (graph::_equal_nodes(i->first, item.first))
						{
							continue;
						}

						if (graph::_equal_nodes(j->first, item.first))
						{
							++j;
						}

						size_type localTriangleCount{};

						while (j != edges.end())
						{
							size_type multipleEdges{1};

							if constexpr (graph::can_have_multiple_edges())
							{
								multipleEdges = i->second.size() * j->second.size();
							}

							if (this->_graph.at(i->first).contains(j->first))
							{
								if constexpr (graph::can_have_multiple_edges())
								{
									multipleEdges *= this->_graph.at(i->first).at(j->first).size();
								}

								localTriangleCount++;
							}

							triangleCount += (localTriangleCount * multipleEdges);

							++j;
						}
					}
				}
			}

			return triangleCount / 3;
		}

		[[nodiscard]] size_type triplet_count() const
		{
			size_type tripletCount{};

			for (const auto &item : this->_graph)
			{
				const auto &node = item.first;

				const size_type unique_edge_count =
					this->unique_edge_count(node) - (this->_graph.at(node).contains(node) ? 1 : 0);

				if (unique_edge_count)
				{
					tripletCount += this->triple_sum(unique_edge_count - 1);
				}
			}

			return tripletCount;
		}

		[[nodiscard]] std::vector < std::pair < NodeType, NodeType > > triangle_neighbors(const NodeType &node) const
		{
			std::vector < std::pair < NodeType, NodeType > > result;

			const auto iterator = this->_graph.find(node);

			if (iterator == this->_graph.cend())
			{
				return result;
			}

			std::vector < std::reference_wrapper < const NodeType > > neighbors;

			for (const auto &neighbor : iterator->second)
			{
				neighbors.push_back(neighbor.first);
			}

			for (size_t i = 0; i < neighbors.size(); i++)
			{
				for (size_t j = i + 1; j < neighbors.size(); j++)
				{
					if (this->_graph.at(neighbors[i]).contains(neighbors[j]))
					{
						result.emplace_back(neighbors[i], neighbors[j]);
					}
				}
			}

			return result;
		}

		// Local analysis. Works only on weightless digraphs.
		[[nodiscard]] edge_weight_type clustering_coefficient(const NodeType &node) const noexcept
		{
			const auto nodeCount = this->edge_count(node);

			if (nodeCount < 2)
			{
				return edge_weight_type{};
			}

			size_type triangleCount{};

			const auto iterator = this->_graph.find(node);

			if (iterator != this->_graph.end() && iterator->second.size() >= 2)
			{
				for (auto i = iterator->second.begin(), j = ++iterator->second.begin();
					j != iterator->second.end();
					j = ++i, ++j)
				{
					size_type localTriangleCount{};

					if (graph::_equal_nodes(i->first, iterator->first))
					{
						continue;
					}

					if (graph::_equal_nodes(j->first, iterator->first))
					{
						++j;
					}

					while (j != iterator->second.end())
					{
						size_type multipleEdges{1};

						if constexpr (graph::can_have_multiple_edges())
						{
							multipleEdges = i->second.size() * j->second.size();
						}

						if (this->_graph.at(i->first).contains(j->first))
						{
							if constexpr (graph::can_have_multiple_edges())
							{
								multipleEdges *= this->_graph.at(i->first).at(j->first).size();
							}

							localTriangleCount++;
						}

						triangleCount += (localTriangleCount * multipleEdges);

						++j;
					}
				}
			}

			return (triangleCount * edge_weight_type{2}) / (nodeCount * (nodeCount - 1));
		}

		// Global analysis. Works only on weightless digraphs.
		[[nodiscard]] edge_weight_type clustering_coefficient() const noexcept
		{
			size_type triangleCount{};
			size_type tripletCount{};

			std::thread threadTriangleCount(
				[this, &triangleCount] () -> void
				{
					triangleCount = this->triangleCount();
				});

			std::thread threadTripletCount(
				[this, &tripletCount] () -> void
				{
					tripletCount = this->tripletCount();
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

		// Global analysis. Works only on weightless digraphs.
		[[nodiscard]] edge_weight_type average_clustering_coefficient() const noexcept
		{
			if (this->_graph.empty())
			{
				return edge_weight_type{};
			}

			edge_weight_type averageCoefficient{};

			for (const auto &item : this->_graph)
			{
				averageCoefficient += this->clustering_coefficient(item.first);
			}

			return averageCoefficient / static_cast < edge_weight_type > (this->node_count());
		}

		[[nodiscard]] edge_weight_type closeness_centrality(const NodeType &origin) const
		{
			edge_weight_type closeness{};
			single_previous_node_with_weight_container previous{};

			this->template generic_Dijkstra < Dijkstra_modes::single_previous_and_next_node_and_distance > (origin, previous);

			for (const auto &[node, nodeAndDistance] : previous)
			{
				closeness += nodeAndDistance.second;
			}

			return closeness / static_cast < edge_weight_type > (this->node_count());
		}

		[[nodiscard]] node_weight_container closeness_centrality() const
		{
			std::mutex mutex;
			std::vector < std::thread > threads;
			node_weight_container closeness{};

			if constexpr (graph::unordered())
			{
				closeness.reserve(this->node_count());
			}

			for (const auto &[origin, edges] : this->_graph)
			{
				threads.emplace_back(std::thread(
					[this, &origin, &closeness, &mutex] () -> void
					{
						auto _closeness = this->closeness_centrality(origin);

						const std::lock_guard < std::mutex > lock(mutex);

						closeness[origin] = std::move(_closeness);
					}));
			}

			for (auto &thread : threads)
			{
				thread.join();
			}

			return closeness;
		}

		[[nodiscard]] edge_weight_type node_betweenness_centrality(const NodeType &node) const
		{
			const auto previous = this->calculate_previous_for_node_betweenness_centrality();

			edge_weight_type betweennessCentrality{};

			for (auto iterator1 = previous.cbegin(); iterator1 != previous.cend(); ++iterator1)
			{
				const auto &node1 = iterator1->first;
				auto iterator2 = iterator1;

				++iterator2;

				for (; iterator2 != previous.cend(); ++iterator2)
				{
					const auto &node2 = iterator2->first;

					if (graph::_different_nodes(node, node1) &&
						graph::_different_nodes(node, node2) &&
						iterator1->second.contains(node1) &&
						iterator1->second.contains(node2) &&
						iterator1->second.contains(node))
					{
						betweennessCentrality +=
							graph::betweenness_centrality_of_a_node_in_a_path(node, node1, node2, iterator1->second);
					}
				}
			}

			return betweennessCentrality;
		}

		[[nodiscard]] node_weight_container node_betweenness_centrality() const
		{
			const auto previous = this->calculate_previous_for_node_betweenness_centrality();

			node_weight_container betweennessCentrality{};

			if constexpr (graph::unordered())
			{
				betweennessCentrality.reserve(this->node_count());
			}

			for (auto iterator1 = previous.cbegin(); iterator1 != previous.cend(); ++iterator1)
			{
				const auto &node1 = iterator1->first;
				auto iterator2 = iterator1;

				++iterator2;

				for (; iterator2 != previous.cend(); ++iterator2)
				{
					const auto &node2 = iterator2->first;

					for (const auto &[target, edges] : this->_graph)
					{
						if (graph::_different_nodes(target, node1) &&
							graph::_different_nodes(target, node2) &&
							iterator1->second.contains(node1) &&
							iterator1->second.contains(node2) &&
							iterator1->second.contains(target))
						{
							betweennessCentrality[target] +=
								graph::betweenness_centrality_of_a_node_in_a_path(target, node1, node2, iterator1->second);
						}
					}
				}
			}

			return betweennessCentrality;
		}

		[[nodiscard]] edge_betweenness_centrality_container edge_betweenness_centrality() const
		{
			auto previous = this->calculate_previous_for_edge_betweenness_centrality();

			edge_betweenness_centrality_container betweennessCentrality{};

			if constexpr (graph::unordered())
			{
				betweennessCentrality.reserve(this->node_count());
			}

			for (auto iterator = previous.begin(); iterator != previous.end(); ++iterator)
			{
				auto &nodeTree = iterator->first;
				auto &stack = iterator->second;

				while (!stack.empty())
				{
					const auto &currentNode = stack.top();

					const size_type pathCount = std::get < 0 > (nodeTree.at(currentNode));
					const auto &immediatePreviousSet = std::get < 1 > (nodeTree.at(currentNode));
					const auto &currentBetweennessWeight = std::get < 2 > (nodeTree.at(currentNode));

					for (const auto &immediatePrevious : immediatePreviousSet)
					{
						const size_type immediatePreviousPathCount = std::get < 0 > (nodeTree.at(immediatePrevious));
						auto &immediatePreviousBetweennessWeight = std::get < 2 > (nodeTree.at(immediatePrevious));

						const edge_weight_type factor =
							static_cast < edge_weight_type > (immediatePreviousPathCount) / static_cast < edge_weight_type > (pathCount);

						edge_weight_type localImmediatePreviousBetweennessWeight =
							(edge_weight_type{1} + currentBetweennessWeight) * factor;

						betweennessCentrality[std::make_pair(currentNode, immediatePrevious)] +=
							localImmediatePreviousBetweennessWeight;

						immediatePreviousBetweennessWeight += localImmediatePreviousBetweennessWeight;
					}

					stack.pop();
				}
			}

			return betweennessCentrality;
		}

		// https://journals.aps.org/pre/pdf/10.1103/PhysRevE.69.026113
		// Weights cannot be negative.
		template < bool CheckCommunitiesClassification, typename CommunityDataType >
		[[nodiscard]] edge_weight_type modularity(
			const typename std::conditional <
				graph::unordered(),
				std::unordered_map < CommunityDataType, node_set_container >,
				std::map < CommunityDataType, node_set_container > > ::type &communitiesClassification) const
		{
			if (this->_weight_sum == edge_weight_type{})
			{
				throw std::domain_error("There are no edges in the graph.");
			}

			if constexpr (CheckCommunitiesClassification)
			{
				node_set_container insertedNodes{};

				for (const auto &item1 : communitiesClassification)
				{
					for (const auto &item2 : item1.second)
					{
						if (this->_graph.contains(item2))
						{
							throw std::runtime_error("There is a node in \"communitiesClassification\" not present in the graph.");
						}

						insertedNodes.insert(item2);
					}
				}

				if (insertedNodes.size() != this->_graph.size())
				{
					throw std::runtime_error("Not every node in the graph is present in \"communitiesClassification\".");
				}
			}

			// this->_weight_sum is already multiplied by two.
			const edge_weight_type inverseOfTwoTimesOfWeightSum = edge_weight_type{1} / this->_weight_sum;
			edge_weight_type sum{};
			std::mutex mutex;

			std::for_each(std::execution::par, communitiesClassification.cbegin(), communitiesClassification.cend(),
				[&] (const std::pair < CommunityDataType, node_set_container > &pair) -> void
				{
					edge_weight_type weightSum{};
					edge_weight_type degreeSum{};

					for (const auto &item1 : pair.second)
					{
						degreeSum += this->degree(item1);

						for (const auto &item2 : pair.second)
						{
							weightSum += this->weight(item1, item2);
						}
					}

					weightSum *= inverseOfTwoTimesOfWeightSum;
					degreeSum *= inverseOfTwoTimesOfWeightSum;

					const std::lock_guard < std::mutex > lock(mutex);

					sum += weightSum - degreeSum * degreeSum;
				});

			return sum;
		}

		template <
			xablau::algebra::concepts::xablau_matrix_dense MatrixType,
			bool AllowAutoEdges >
		requires (std::same_as < typename MatrixType::value_type, edge_weight_type >)
		[[nodiscard]] auto adjacency_matrix() const
		{
			using MatrixValueType = typename MatrixType::value_type;

			size_type counter{};
			const auto matrixLineLength = this->node_count();

			if constexpr (MatrixType::fixed())
			{
				if (matrixLineLength != MatrixType::dimensionalities()[0] ||
					matrixLineLength != MatrixType::dimensionalities()[1])
				{
					throw std::runtime_error(""); // TODO: Create message.
				}
			}

			MatrixType adjacencyMatrix{};

			if constexpr (!MatrixType::fixed())
			{
				adjacencyMatrix.resize(matrixLineLength, matrixLineLength);
			}

			graph_node_to_matrix_index_map graphNodeToMatrixIndex{};
			matrix_index_to_graph_node_map matrixIndexToGraphNode{};

			if constexpr (graph::unordered())
			{
				graphNodeToMatrixIndex.reserve(matrixLineLength);
				matrixIndexToGraphNode.reserve(matrixLineLength);
			}

			for (const auto &item : this->_graph)
			{
				graphNodeToMatrixIndex[item.first] = counter;
				matrixIndexToGraphNode[counter] = item.first;

				counter++;
			}

			std::for_each(this->_graph.cbegin(), this->_graph.cend(),
				[&adjacencyMatrix, &graphNodeToMatrixIndex] (const typename graph_container::value_type &node1) -> void
				{
					const auto lineIndex1 = graphNodeToMatrixIndex.at(node1.first);

					for (const auto &node2 : node1.second)
					{
						const auto lineIndex2 = graphNodeToMatrixIndex.at(node2.first);

						if constexpr (graph::can_have_multiple_edges())
						{
							auto &value = adjacencyMatrix(lineIndex1, lineIndex2);

							for (const auto &edge : node2.second)
							{
								value += edge.weight();
							}
						}

						else
						{
							adjacencyMatrix(lineIndex1, lineIndex2) = node2.second.weight();
						}
					}
				});

			if constexpr (AllowAutoEdges)
			{
				for (size_type i = 0; i < matrixLineLength; i++)
				{
					adjacencyMatrix(i, i) *= MatrixValueType{2};
				}
			}

			else
			{
				for (size_type i = 0; i < matrixLineLength; i++)
				{
					adjacencyMatrix(i, i) = MatrixValueType{};
				}
			}

			return std::make_tuple(adjacencyMatrix, graphNodeToMatrixIndex, matrixIndexToGraphNode);
		}

		template < xablau::algebra::concepts::xablau_matrix_dense MatrixType >
		[[nodiscard]] auto laplacian_matrix() const
		{
			using MatrixValueType = typename MatrixType::value_type;

			auto [laplacianMatrix, graphNodeToMatrixIndex, matrixIndexToGraphNode] =
				this->template adjacency_matrix < MatrixType, false > ();

			std::for_each(matrixIndexToGraphNode.begin(), matrixIndexToGraphNode.end(),
				[this, &laplacianMatrix](const std::pair < const size_type, NodeType > &pair) -> void
				{
					auto &diagonalElement = laplacianMatrix(pair.first, pair.first);
					const auto loopWeight = this->weight(pair.second, pair.second);

					diagonalElement = (loopWeight + loopWeight) - this->degree(pair.second);
				});

			laplacianMatrix *= MatrixValueType{-1};

			return std::make_tuple(laplacianMatrix, graphNodeToMatrixIndex, matrixIndexToGraphNode);
		}

		[[nodiscard]] std::pair < all_previous_nodes_container, next_nodes_container > Dijkstra_all_previous_and_next_nodes(const NodeType &origin) const
		{
			all_previous_nodes_container previous{};
			next_nodes_container next{};

			this->template generic_Dijkstra < Dijkstra_modes::all_previous_and_next_nodes > (origin, previous);
			this->template Dijkstra_create_next < Dijkstra_modes::all_previous_and_next_nodes > (previous, next);

			return std::make_pair(previous, next);
		}

		[[nodiscard]] std::pair < all_previous_nodes_with_weight_container, next_nodes_with_weight_container > Dijkstra_all_previous_and_next_nodes_and_distance(
			const NodeType &origin) const
		{
			all_previous_nodes_with_weight_container previous{};
			next_nodes_with_weight_container next{};

			this->template generic_Dijkstra < Dijkstra_modes::all_previous_and_next_nodes_and_distance > (origin, previous);
			this->template Dijkstra_create_next < Dijkstra_modes::all_previous_and_next_nodes_and_distance > (previous, next);

			return std::make_pair(previous, next);
		}

		[[nodiscard]] std::pair < single_previous_node_container, next_nodes_container > Dijkstra_single_previous_and_next_nodes(const NodeType &origin) const
		{
			single_previous_node_container previous{};
			next_nodes_container next{};

			this->template generic_Dijkstra < Dijkstra_modes::single_previous_and_next_node > (origin, previous);
			this->template Dijkstra_create_next < Dijkstra_modes::single_previous_and_next_node > (previous, next);

			return std::make_pair(previous, next);
		}

		[[nodiscard]] std::pair < single_previous_node_with_weight_container, next_nodes_with_weight_container >
			Dijkstra_single_previous_and_next_nodes_and_distance(const NodeType &origin) const
		{
			single_previous_node_with_weight_container previous{};
			next_nodes_with_weight_container next{};

			this->template generic_Dijkstra < Dijkstra_modes::single_previous_and_next_node_and_distance > (origin, previous);
			this->template Dijkstra_create_next < Dijkstra_modes::single_previous_and_next_node_and_distance > (previous, next);

			return std::make_pair(previous, next);
		}

		template < bool CreatePath >
		[[nodiscard]] auto Dijkstra_single_path(const NodeType &origin, const NodeType &destiny) const
		{
			std::vector < NodeType > path;
			
			if (!this->contains(origin) || !this->contains(destiny))
			{
				if constexpr (CreatePath)
				{
					return std::make_pair(std::numeric_limits < edge_weight_type > ::max(), std::vector < NodeType > ());
				}

				else
				{
					return std::numeric_limits < edge_weight_type > ::max();
				}
			}

			if (graph::_equal_nodes(origin, destiny))
			{
				if constexpr (CreatePath)
				{
					path.push_back(destiny);

					return std::make_pair(edge_weight_type{}, path);
				}

				else
				{
					return edge_weight_type{};
				}
			}

			edge_weight_type smallestDistance{};

			node_node_container previous{};

			this->template generic_Dijkstra < Dijkstra_modes::single_path > (origin, previous, destiny, smallestDistance);

			if constexpr (CreatePath)
			{
				NodeType current = destiny;

				while (true)
				{
					const auto iterator = previous.find(current);

					path.push_back(current);

					if (iterator != previous.cend())
					{
						current = iterator->second;
					}

					else
					{
						break;
					}
				}

				std::reverse(path.begin(), path.end());

				return std::make_pair(smallestDistance, path);
			}

			else
			{
				return smallestDistance;
			}
		}

		// It cannot have negative cycles.
		template < xablau::algebra::concepts::xablau_matrix_dynamic MatrixType >
		requires (std::same_as < typename MatrixType::value_type, edge_weight_type >)
		[[nodiscard]] auto Floyd_Warshall() const
		{
			return
				this->template Floyd_Warshall <
					MatrixType,
					xablau::algebra::tensor_dense_dynamic <
						std::optional < NodeType >,
						xablau::algebra::tensor_rank < MatrixType::rank() >,
						xablau::algebra::tensor_contiguity < MatrixType::contiguous() >,
						MatrixType::memory_order_indices > > ();
		}

		// It cannot have negative cycles.
		template < xablau::algebra::concepts::xablau_matrix_fixed MatrixType >
		requires (std::same_as < typename MatrixType::value_type, edge_weight_type >)
		[[nodiscard]] auto Floyd_Warshall() const
		{
			return
				this->template Floyd_Warshall <
					MatrixType,
					xablau::algebra::tensor_dense_fixed <
						std::optional < NodeType >,
						MatrixType::fixed_dimensionalities,
						xablau::algebra::tensor_contiguity < MatrixType::contiguous() >,
						MatrixType::memory_order_indices > > ();
		}

		// Adapted from https://www.geeksforgeeks.org/traveling-salesman-problem-tsp-implementation/
		[[nodiscard]] auto traveling_salesman_problem(const NodeType &start) const
		requires (std::totally_ordered < edge_weight_type > && graph::cannot_have_multiple_edges())
		{
			const auto iterator = this->_graph.find(start);

			if (iterator == this->_graph.cend())
			{
				return
					std::make_pair(
						std::vector < NodeType > (),
						edge_weight_type{});
			}

			if (this->node_count() == size_type{1})
			{
				return
					std::make_pair(
						std::vector < NodeType > (1, iterator->first),
						edge_weight_type{});
			}

			std::reference_wrapper < const NodeType > thisStart = iterator->first;
			std::vector < std::reference_wrapper < const NodeType > > nodes;

			constexpr auto comparison =
				[] (const std::reference_wrapper < const NodeType > &node1,
					const std::reference_wrapper < const NodeType > &node2) -> bool
				{
					return std::less < const NodeType * > {}(std::addressof(node1.get()), std::addressof(node2.get()));
				};

			nodes.reserve(this->node_count());

			for (const auto &[node, _] : this->_graph)
			{
				if (graph::_different_nodes(thisStart, node))
				{
					nodes.emplace_back(node);
				}
			}

			std::ranges::sort(nodes, comparison);

			nodes.insert(nodes.begin(), thisStart);

			bool foundSolution = false;
			std::vector < std::reference_wrapper < const NodeType > > bestPath;
			edge_weight_type minimumWeight = std::numeric_limits < edge_weight_type > ::max();

			do
			{
				bool validPath = true;
				edge_weight_type currentPathweight{};

				for (auto previous = nodes.begin(), next = ++(nodes.begin()); next != nodes.end(); ++previous, ++next)
				{
					if (const auto connection = this->edges(previous->get(), next->get());
						connection.has_value())
					{
						currentPathweight += connection.value().get().weight();
					}

					else
					{
						validPath = false;

						break;
					}
				}

				if (validPath)
				{
					foundSolution = true;

					if (currentPathweight <= minimumWeight)
					{
						minimumWeight = currentPathweight;
						bestPath = nodes;
					}
				}

			} while (std::next_permutation(nodes.begin() + 1, nodes.end(), comparison));

			if (foundSolution)
			{
				std::vector < NodeType > result;

				result.reserve(bestPath.size());

				for (const auto &node : bestPath)
				{
					result.push_back(node.get());
				}

				return std::make_pair(std::move(result), minimumWeight);
			}

			else
			{
				return
					std::make_pair(
						std::vector < NodeType > (),
						edge_weight_type{});
			}
		}

		template < concepts::digraph DependencyType >
		requires (std::same_as < typename graph::node_type, typename DependencyType::node_type >)
		[[nodiscard]] auto traveling_salesman_problem(const NodeType &start, const DependencyType &dependency) const
		requires (std::totally_ordered < edge_weight_type > && graph::cannot_have_multiple_edges())
		{
			if (this->node_count() != dependency.node_count())
			{
				throw std::runtime_error(""); // TODO: Create message.
			}

			if (this->node_count() != dependency.node_count())
			{
				throw std::runtime_error(""); // TODO: Create message.
			}

			for (const auto &[node, _] : this->_graph)
			{
				if (!dependency.contains(node))
				{
					throw std::runtime_error(""); // TODO: Create message.
				}
			}

			const auto iterator = this->_graph.find(start);

			if (iterator == this->_graph.cend())
			{
				return
					std::make_pair(
						std::vector < NodeType > (),
						edge_weight_type{});
			}

			if (this->node_count() == size_type{1})
			{
				return
					std::make_pair(
						std::vector < NodeType > (1, iterator->first),
						edge_weight_type{});
			}

			std::vector < std::tuple < size_type, std::reference_wrapper < const NodeType > > > nodes;
			std::vector <
				std::pair <
					typename decltype(nodes)::difference_type,
					typename decltype(nodes)::difference_type > > groups;

			constexpr auto priorityAndNodeComparison =
				[] (const std::tuple < size_type, std::reference_wrapper < const NodeType > > &node1,
					const std::tuple < size_type, std::reference_wrapper < const NodeType > > &node2) -> bool
				{
					return
						std::get < 0 > (node1) < std::get < 0 > (node2) ||

						std::get < 0 > (node1) == std::get < 0 > (node2) &&
						std::less < const NodeType * > {}(
							std::addressof(std::get < 1 > (node1).get()),
							std::addressof(std::get < 1 > (node2).get()));
				};

			auto dependencyCopy = dependency;

			nodes.reserve(this->node_count());
			groups.reserve(this->node_count());

			for (size_type priority{}, accumulator{}; !dependencyCopy.empty();)
			{
				const auto sinkNodes = dependencyCopy.sink_nodes();
				const auto sinkNodeCount = std::distance(sinkNodes.cbegin(), sinkNodes.cend());

				if (sinkNodeCount == 0)
				{
					throw std::runtime_error(""); // TODO: Create message.s
				}

				groups.emplace_back(
					static_cast < typename decltype(nodes)::difference_type > (accumulator),
					static_cast < typename decltype(nodes)::difference_type > (accumulator + sinkNodeCount));

				for (const auto &sinkNode : sinkNodes)
				{
					nodes.emplace_back(priority, this->_graph.find(sinkNode)->first);
					dependencyCopy.erase(sinkNode);
				}

				accumulator += static_cast < size_type > (sinkNodeCount);

				priority++;
			}

			std::ranges::sort(nodes, priorityAndNodeComparison);

			if (groups[0].second - groups[0].first != 1 ||
				graph::_different_nodes(std::get < 1 > (*(nodes.cbegin() + groups[0].first)).get(), iterator->first))
			{
				return
					std::make_pair(
						std::vector < NodeType > (),
						edge_weight_type{});
			}

			bool foundSolution = false;
			decltype(nodes) bestPath;
			edge_weight_type minimumWeight = std::numeric_limits < edge_weight_type > ::max();
			const auto permuteNodes =
				[&groups, &nodes] () -> bool
				{
					constexpr auto nodeComparison =
						[] (const std::tuple < size_type, std::reference_wrapper < const NodeType > > &node1,
							const std::tuple < size_type, std::reference_wrapper < const NodeType > > &node2) -> bool
						{
							return
								std::less < const NodeType * > {}(
									std::addressof(std::get < 1 > (node1).get()),
									std::addressof(std::get < 1 > (node2).get()));
						};

					for (auto iterator = groups.crbegin(); iterator != groups.crend(); ++iterator)
					{
						if (std::next_permutation(nodes.begin() + iterator->first, nodes.begin() + iterator->second, nodeComparison))
						{
							return true;
						}
					}

					return false;
				};

			do
			{
				bool validPath = true;
				edge_weight_type currentPathweight{};
				dependencyCopy = dependency;

				for (auto previous = nodes.begin(), next = ++(nodes.begin()); next != nodes.end(); ++previous, ++next)
				{
					const auto connection = this->edges(std::get < 1 > (*previous).get(), std::get < 1 > (*next).get());
					const auto sinkNodes = dependencyCopy.sink_nodes();
					const auto searchFunction =
						[&previous] (const NodeType &sinkNode) -> bool
						{
							return graph::_equal_nodes(std::get < 1 > (*previous).get(), sinkNode);
						};

					if (connection.has_value() && std::find_if(sinkNodes.cbegin(), sinkNodes.cend(), searchFunction) != sinkNodes.cend())
					{
						currentPathweight += connection.value().get().weight();
					}

					else
					{
						validPath = false;

						break;
					}

					dependencyCopy.erase(std::get < 1 > (*previous).get());
				}

				if (validPath)
				{
					foundSolution = true;

					if (currentPathweight <= minimumWeight)
					{
						minimumWeight = currentPathweight;
						bestPath = nodes;
					}
				}

			} while (permuteNodes());

			if (foundSolution)
			{
				std::vector < NodeType > result;

				result.reserve(bestPath.size());

				for (const auto &[_, node] : bestPath)
				{
					result.push_back(node.get());
				}

				return std::make_pair(std::move(result), minimumWeight);
			}

			else
			{
				return
					std::make_pair(
						std::vector < NodeType > (),
						edge_weight_type{});
			}
		}

		template < concepts::nary_tree TreeType, tree_generation_modes TreeGenerationMode >
		requires (std::same_as < typename TreeType::value_type, typename graph::node_type::value_type >)
		[[nodiscard]] TreeType generate_tree(const NodeType &origin) const
		{
			connections_container next{};

			if constexpr (TreeGenerationMode == tree_generation_modes::breadth_first_search)
			{
				next = this->breadth_first_search(origin);
			}

			else if constexpr (TreeGenerationMode == tree_generation_modes::depth_first_search)
			{
				next = this->depth_first_search(origin);
			}

			else if constexpr (TreeGenerationMode == tree_generation_modes::Dijkstra)
			{
				auto [_previous, _next] = this->Dijkstra_single_previous_and_next_nodes(origin);

				next = std::move(_next);
			}

			TreeType tree{};
			std::queue < typename TreeType::iterator > queue;

			queue.push(tree.insert_root(origin.value));

			while (!queue.empty())
			{
				const auto currentNodeInTree = queue.front();
				const auto &currentNodeInGraph = *currentNodeInTree;

				if (next.contains(currentNodeInGraph))
				{
					const auto &edges = next.at(currentNodeInGraph);

					for (const auto &edge : edges)
					{
						queue.emplace(tree.insert_back_child(currentNodeInTree, edge.value));
					}
				}

				queue.pop();
			}

			return tree;
		}

		[[nodiscard]] bool connected() const
		{
			if (this->_graph.empty())
			{
				return true;
			}

			return this->connected_nodes(this->_graph.begin()->first).size() == this->node_count();
		}

		[[nodiscard]] std::vector < graph > dismember_disconnected_nodes() const
		{
			std::vector < graph > connectedGraphs;

			for (const auto &item1 : this->_graph)
			{
				bool newGraph = true;

				for (typename std::vector < graph > ::size_type i = 0; i < connectedGraphs.size(); i++)
				{
					if (connectedGraphs[i].contains(item1.first))
					{
						newGraph = false;

						break;
					}
				}

				if (newGraph)
				{
					auto nodes = this->connected_nodes(item1.first);
					graph graph;

					if constexpr (graph::unordered())
					{
						graph._graph.reserve(this->node_count());
						graph._degrees.reserve(this->node_count());
						graph._expected_edges_by_node = this->_expected_edges_by_node;
					}

					for (const auto &item2 : nodes)
					{
						const auto _edges = this->edges(item2);

						if (_edges.has_value())
						{
							for (const auto &item3 : _edges.value().get())
							{
								if constexpr (graph::can_have_multiple_edges())
								{
									for (const auto &edge : item3.second)
									{
										graph.insert(item2, item3.first, edge);
									}
								}
								
								else
								{
									graph.insert(item2, item3.first, item3.second);
								}
							}
						}
					}

					connectedGraphs.push_back(std::move(graph));
				}
			}

			return connectedGraphs;
		}

		[[nodiscard]] node_weight_container random_walk() const
		{
			if (this->_graph.empty())
			{
				return node_weight_container{};
			}

			size_type counter{};
			graph_node_to_matrix_index_map graphNodeToMatrixIndex{};
			matrix_index_to_graph_node_map matrixIndexToGraphNode{};

			for (const auto &item : this->_graph)
			{
				matrixIndexToGraphNode[counter] = item.first;
				graphNodeToMatrixIndex[item.first] = counter;

				counter++;
			}

			const auto matrixSize = this->_graph.size();
			xablau::algebra::tensor_dense_dynamic <
				edge_weight_type,
				xablau::algebra::tensor_rank < 2 >,
				xablau::algebra::tensor_contiguity < true > > matrix(matrixSize, matrixSize);

			std::for_each(std::execution::par_unseq, this->_graph.cbegin(), this->_graph.cend(),
				[&matrix, &graphNodeToMatrixIndex] (
					const typename graph_container::value_type &item1) -> void
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

			node_weight_container centrality{};

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
			std::random_device randomDevice{};
			std::default_random_engine engine(randomDevice());
			node_node_container indicesMap{};
			std::vector < NodeType > indices;

			indices.reserve(this->_graph.size());

			for (const auto &node : this->_graph)
			{
				indices.push_back(node.first);
			}

			std::shuffle(indices.begin(), indices.end(), engine);

			size_type counter{};

			for (const auto &node : this->_graph)
			{
				indicesMap[node.first] = indices[counter];

				counter++;
			}

			graph newGraph;

			if constexpr (graph::unordered())
			{
				newGraph._expected_edges_by_node = this->_expected_edges_by_node;
				newGraph._graph.reserve(this->node_count());
				newGraph._degrees.reserve(this->node_count());
			}

			for (const auto &node1 : this->_graph)
			{
				for (const auto &node2 : node1.second)
				{
					if constexpr (graph::can_have_multiple_edges())
					{
						for (const auto &edge : node2.second)
						{
							newGraph.insert(indicesMap.at(node1.first), indicesMap.at(node2.first), edge);
						}
					}

					else
					{
						newGraph.insert(indicesMap.at(node1.first), indicesMap.at(node2.first), node2.second);
					}
				}
			}

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

		void clear() noexcept
		{
			this->_weight_sum = edge_weight_type{};
			this->_graph.clear();
			this->_degrees.clear();
			this->_edge_count = size_type{};
			this->_unique_edge_count = size_type{};
		}

		bool insert(const NodeType &node)
		{
			if (this->contains(node))
			{
				return false;
			}

			if constexpr (graph::unordered())
			{
				this->_graph[node] = typename graph_container::mapped_type(this->_expected_edges_by_node);
			}

			else
			{
				this->_graph[node] = typename graph_container::mapped_type{};
			}

			this->_degrees[node] = edge_weight_type{};

			return true;
		}

		std::optional < std::reference_wrapper < const EdgeType > >
			insert(
				const NodeType &node1,
				const NodeType &node2,
				const EdgeType &edge = EdgeType{edge_weight_type{1}})
		{
			const auto weight = edge.weight();

			if (weight == edge_weight_type{})
			{
				return std::nullopt;
			}

			this->insert(node1);
			this->insert(node2);

			const auto &_node2 = this->_graph.find(node2)->first;

			if constexpr (graph::can_have_multiple_edges())
			{
				std::optional < std::reference_wrapper < const EdgeType > > insertedEdge;

				auto _edge = this->_graph.at(node1).insert(std::make_pair(_node2, std::vector < EdgeType > ()));

				_edge.first->second.push_back(edge);

				if (_edge.second)
				{
					++(this->_unique_edge_count);
				}

				if (this->_graph.find(node1) == this->_graph.find(_node2))
				{
					insertedEdge = _edge.first->second.back();
				}

				else
				{
					auto transposedEdge = this->_graph.at(_node2).insert(std::make_pair(node1, std::vector < EdgeType > ()));

					transposedEdge.first->second.push_back(edge);

					insertedEdge = transposedEdge.first->second.back();
				}

				this->_degrees.at(node1) += weight;
				this->_degrees.at(_node2) += weight;

				++(this->_edge_count);

				this->_weight_sum += weight + weight;

				return insertedEdge;
			}

			else
			{
				const auto _edge = this->_graph.at(node1).insert(std::make_pair(_node2, edge));

				if (!_edge.second)
				{
					return _edge.first->second;
				}

				const auto &insertedEdge = this->_graph.at(_node2).insert(std::make_pair(node1, edge)).first->second;

				this->_degrees.at(node1) += weight;
				this->_degrees.at(_node2) += weight;

				++(this->_edge_count);
				++(this->_unique_edge_count);

				this->_weight_sum += weight + weight;

				return insertedEdge;
			}
		}

		bool erase(const NodeType &node)
		{
			if (this->contains(node))
			{
				const auto transposedNodesIterator = this->_graph.find(node);

				for (auto iterator = transposedNodesIterator->second.cbegin();
					iterator != transposedNodesIterator->second.cend();
					++iterator)
				{
					const auto &transposedNode = iterator->first;
					const auto edges = this->_graph.find(transposedNode);
					const auto &edge = edges->second.at(node);
					const bool nodesAreNotTheSame = transposedNodesIterator != edges;

					if constexpr (graph::can_have_multiple_edges())
					{
						const auto _weight =
							std::accumulate(edge.cbegin(), edge.cend(), edge_weight_type{},
								[] (const EdgeType &edge1, const EdgeType &edge2) -> edge_weight_type
								{
									return edge1.weight() + edge2.weight();
								});

						this->_weight_sum -= _weight + _weight;
						this->_edge_count -= edge.size();

						if (nodesAreNotTheSame)
						{
							this->_degrees.at(transposedNode) -= _weight;
						}
					}

					else
					{
						this->_weight_sum -= edge.weight() + edge.weight();
						--(this->_edge_count);

						if (nodesAreNotTheSame)
						{
							this->_degrees.at(transposedNode) -= edge.weight();
						}
					}

					--(this->_unique_edge_count);

					if (nodesAreNotTheSame)
					{
						edges->second.erase(node);
					}
				}

				this->_graph.erase(node);
				this->_degrees.erase(node);

				return true;
			}

			return false;
		}

		bool erase(const NodeType &node1, const NodeType &node2)
		{
			const auto iterator1 = this->_graph.find(node1);

			if (iterator1 != this->_graph.end())
			{
				const auto &edges = iterator1->second;
				const auto iterator2 = edges.find(node2);

				if (iterator2 != edges.end())
				{
					const bool nodesAreTheSame = this->_graph.find(node1) == this->_graph.find(node2);
					edge_weight_type weight{};

					if constexpr (graph::can_have_multiple_edges())
					{
						const auto &_edges = iterator2->second;

						weight =
							std::accumulate(_edges.cbegin(), _edges.cend(), edge_weight_type{},
								[] (const EdgeType &edge1, const EdgeType &edge2) -> edge_weight_type
								{
									return edge1.weight() + edge2.weight();
								});

						this->_edge_count -= _edges.size();
					}

					else
					{
						weight = iterator2->second.weight();

						--(this->_edge_count);
					}
					
					this->_weight_sum -= weight + weight;

					this->_degrees.at(node1) -= weight;
					this->_degrees.at(node2) -= weight;

					--(this->_unique_edge_count);

					if (nodesAreTheSame)
					{
						this->_graph.at(node1).erase(node2);
					}

					else
					{
						this->_graph.at(node1).erase(node2);
						this->_graph.at(node2).erase(node1);
					}

					return true;
				}
			}

			return false;
		}

		[[nodiscard]] bool contains(const NodeType &node) const
		{
			return this->_graph.contains(node);
		}

		[[nodiscard]] bool contains(const NodeType &node1, const NodeType &node2) const
		{
			const auto iterator = this->_graph.find(node1);

			if (iterator != this->_graph.cend())
			{
				const auto &edges = iterator->second;

				return edges.contains(node2);
			}

			return false;
		}

		[[nodiscard]] const graph_container &container() const noexcept
		{
			return this->_graph;
		}

		[[nodiscard]] std::optional < std::reference_wrapper < const typename graph_container::mapped_type > >
			edges(const NodeType &node) const
		{
			const auto iterator = this->_graph.find(node);

			if (iterator != this->_graph.cend())
			{
				return iterator->second;
			}

			return std::nullopt;
		}

		[[nodiscard]] std::optional < std::reference_wrapper < const typename graph_container::mapped_type::mapped_type > >
			edges(const NodeType &node1, const NodeType &node2) const
		{
			const auto iterator1 = this->_graph.find(node1);

			if (iterator1 != this->_graph.cend())
			{
				const auto &edges = iterator1->second;
				const auto iterator2 = edges.find(node2);

				if (iterator2 != edges.cend())
				{
					return iterator2->second;
				}
			}

			return std::nullopt;
		}

		[[nodiscard]] edge_weight_type weight(const NodeType &node1, const NodeType &node2) const
		{
			const auto iterator1 = this->_graph.find(node1);

			if (iterator1 != this->_graph.cend())
			{
				const auto &edges = iterator1->second;
				const auto iterator2 = edges.find(node2);

				if (iterator2 != edges.cend())
				{
					if constexpr (graph::can_have_multiple_edges())
					{
						const auto &_edges = iterator2->second;

						return
							std::accumulate(_edges.cbegin(), _edges.cend(), edge_weight_type{},
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

		[[nodiscard]] size_type edge_count(const NodeType &node) const
		{
			const auto iterator = this->_graph.find(node);

			if (iterator != this->_graph.cend())
			{
				if constexpr (graph::can_have_multiple_edges())
				{
					size_type edgeCount{};

					for (const auto &nodes : iterator->second)
					{
						edgeCount += nodes.second.size();
					}

					return edgeCount;
				}

				else
				{
					return iterator->second.size();
				}
			}

			return size_type{};
		}

		[[nodiscard]] size_type unique_edge_count() const noexcept
		{
			return this->_unique_edge_count;
		}

		[[nodiscard]] size_type unique_edge_count(const NodeType &node) const
		{
			const auto iterator = this->_graph.find(node);

			if (iterator != this->_graph.cend())
			{
				return iterator->second.size();
			}

			return size_type{};
		}

		[[nodiscard]] node_set_container nodes() const
		{
			node_set_container nodes{};

			if constexpr (graph::unordered())
			{
				nodes.reserve(this->node_count());
			}

			for (const auto &item : this->_graph)
			{
				nodes.insert(item.first);
			}

			return nodes;
		}

		friend std::ostream &operator<<(std::ostream &stream, const graph &graph)
		{
			size_type accumulator1{};
			const size_type nodeCount = graph.node_count();

			stream << "*--* (";

			if (nodeCount != size_type{})
			{
				stream << "\n";
			}

			for (const auto &nodes : graph._graph)
			{
				stream << "* " << nodes.first << " --\n";

				size_type accumulator2{};
				const size_type edgeCount = nodes.second.size();

				for (const auto &edges : nodes.second)
				{
					stream << "\t * " << edges.first << ": " << edges.second;

					accumulator2++;

					if (accumulator2 != edgeCount)
					{
						stream << "\n";
					}
				}

				accumulator1++;

				if (accumulator1 != nodeCount)
				{
					stream << "\n";
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

		explicit graph(const size_type expectedNodes, const size_type expectedEdgesByNode)
		requires (graph::unordered()) :
			_graph(expectedNodes),
			_degrees(expectedNodes),
			_expected_edges_by_node(expectedEdgesByNode)
		{
		}
	};
}