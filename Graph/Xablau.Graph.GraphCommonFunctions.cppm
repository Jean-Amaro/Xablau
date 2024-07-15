// MIT License
//
// Copyright (c) 2024 Jean Amaro <jean.amaro@outlook.com.br>
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

export module xablau.graph:graph_common_functions;
export import :fundamental_concepts;
export import :node_ref;

export import std;

export import xablau.algebra;

namespace xablau::graph::internals
{
	template < concepts::base_graph GraphType >
	void generic_Dijkstra_initialize_distances(const GraphType &graph, auto &distances, const auto &searchIndex)
	{
		for (const auto &[node, _] : graph.container())
		{
			if (std::addressof(node) == std::addressof(searchIndex.get()))
			{
				distances.emplace(node, typename GraphType::edge_weight_type{});
			}

			else
			{
				distances.emplace(node, std::numeric_limits < typename GraphType::edge_weight_type > ::max());
			}
		}
	}

	template < Dijkstra_modes Mode, typename NodeSetType, concepts::base_graph GraphType >
	void generic_Dijkstra_initialize_previous(const GraphType &graph, const auto &origin, auto &previous)
	{
		if constexpr (Mode == Dijkstra_modes::edge_betweenness)
		{
			if constexpr (GraphType::unordered())
			{
				previous.first.reserve(graph.node_count() * graph.expected_connections_by_node());
			}
		}

		else
		{
			if constexpr (GraphType::unordered())
			{
				previous.reserve(graph.node_count() * graph.expected_connections_by_node());
			}
		}

		if constexpr (Mode == Dijkstra_modes::all_previous_and_next_nodes)
		{
			previous.emplace(origin, NodeSetType{});
		}

		else if constexpr (Mode == Dijkstra_modes::all_previous_and_next_nodes_and_distance)
		{
			previous.emplace(
				origin,
				std::make_pair(NodeSetType{}, typename GraphType::edge_weight_type{}));
		}

		else if constexpr (Mode == Dijkstra_modes::node_betweenness)
		{
			previous.emplace(
				origin,
				std::make_pair(1, NodeSetType{}));
		}

		else if constexpr (Mode == Dijkstra_modes::edge_betweenness)
		{
			previous.first.emplace(
				origin,
				std::make_tuple(1, NodeSetType{}, typename GraphType::edge_weight_type{}));
		}
	}

	template < Dijkstra_modes Mode, typename NodeSetType = void, concepts::base_graph GraphType >
	void generic_Dijkstra(
		const GraphType &graph,
		const typename GraphType::value_type &origin,
		auto &previous,
		const std::optional < std::reference_wrapper < const typename GraphType::value_type > > destiny = std::nullopt,
		const std::optional < std::reference_wrapper < typename GraphType::edge_weight_type > > smallestDistance = std::nullopt)
	{
		std::reference_wrapper < const typename GraphType::value_type > searchIndex = origin;

		constexpr auto orderingFunction =
			[] (const std::pair < std::reference_wrapper < const typename GraphType::value_type >, typename GraphType::edge_weight_type > &pair1,
				const std::pair < std::reference_wrapper < const typename GraphType::value_type >, typename GraphType::edge_weight_type > &pair2) -> bool
			{
				return pair1.second < pair2.second;
			};

		const auto searchFunction =
			[&searchIndex] (const std::pair < std::reference_wrapper < const typename GraphType::value_type >, typename GraphType::edge_weight_type > &pair) -> bool
			{
				return std::addressof(pair.first.get()) == std::addressof(searchIndex.get());
			};

		std::multiset <
			std::pair < std::reference_wrapper < const typename GraphType::value_type >, typename GraphType::edge_weight_type >,
			decltype(orderingFunction) > distances(orderingFunction);

		generic_Dijkstra_initialize_distances(graph, distances, searchIndex);
		generic_Dijkstra_initialize_previous < Mode, NodeSetType > (graph, origin, previous);

		for (typename GraphType::size_type i = 0; i < graph.node_count(); i++)
		{
			const auto iterator = distances.cbegin();
			const auto &[bestNode, bestDistance] = *iterator;

			if (bestDistance == std::numeric_limits < typename GraphType::edge_weight_type > ::max())
			{
				break;
			}

			if constexpr (Mode == Dijkstra_modes::edge_betweenness)
			{
				previous.second.push(bestNode);
			}

			for (const auto &neighbor : graph.container().at(bestNode))
			{
				typename GraphType::edge_weight_type currentDistance = bestDistance;

				searchIndex = neighbor.first;

				if constexpr (GraphType::can_have_multiple_edges())
				{
					currentDistance += std::min_element(neighbor.second.cbegin(), neighbor.second.cend())->weight();
				}

				else
				{
					currentDistance += neighbor.second.weight();
				}

				const auto search = std::find_if(distances.begin(), distances.end(), searchFunction);

				if (search == distances.end())
				{
					continue;
				}

				if constexpr (
					Mode == Dijkstra_modes::single_previous_and_next_node ||
					Mode == Dijkstra_modes::single_previous_and_next_node_and_distance ||
					Mode == Dijkstra_modes::single_path)
				{
					if (currentDistance < search->second)
					{
						auto nodeHandle = distances.extract(search);

						nodeHandle.value().second = currentDistance;

						distances.insert(std::move(nodeHandle));

						if constexpr (Mode == Dijkstra_modes::single_previous_and_next_node_and_distance)
						{
							previous.insert_or_assign(neighbor.first, std::make_pair(bestNode, currentDistance));
						}

						else
						{
							previous.insert_or_assign(neighbor.first, bestNode);
						}
					}
				}

				else if constexpr (
					Mode == Dijkstra_modes::all_previous_and_next_nodes ||
					Mode == Dijkstra_modes::all_previous_and_next_nodes_and_distance ||
					Mode == Dijkstra_modes::node_betweenness ||
					Mode == Dijkstra_modes::edge_betweenness)
				{
					if (currentDistance < search->second)
					{
						typename GraphType::size_type howManyPathsToGetHere{};
						auto nodeHandle = distances.extract(search);

						if constexpr (Mode == Dijkstra_modes::node_betweenness)
						{
							howManyPathsToGetHere = previous.at(bestNode).first;
						}

						else if constexpr (Mode == Dijkstra_modes::edge_betweenness)
						{
							howManyPathsToGetHere = std::get < 0 > (previous.first.at(bestNode));
						}

						nodeHandle.value().second = currentDistance;

						distances.insert(std::move(nodeHandle));

						if constexpr (Mode == Dijkstra_modes::all_previous_and_next_nodes)
						{
							previous.insert_or_assign(neighbor.first, NodeSetType{}).first->second.insert(bestNode);
						}

						else if constexpr (Mode == Dijkstra_modes::all_previous_and_next_nodes_and_distance)
						{
							previous.insert_or_assign(neighbor.first, std::make_pair(NodeSetType{}, currentDistance)).first->second.first.insert(bestNode);
						}

						else if constexpr (Mode == Dijkstra_modes::node_betweenness)
						{
							previous.insert_or_assign(neighbor.first, std::make_pair(howManyPathsToGetHere, NodeSetType{})).first->second.second.insert(bestNode);
						}

						else if constexpr (Mode == Dijkstra_modes::edge_betweenness)
						{
							std::get < 1 > (previous.first.insert_or_assign(neighbor.first, std::make_tuple(howManyPathsToGetHere, NodeSetType{}, typename GraphType::edge_weight_type{})).first->second).insert(bestNode);
						}
					}

					else if (currentDistance == search->second)
					{
						if constexpr (Mode == Dijkstra_modes::all_previous_and_next_nodes)
						{
							previous[neighbor.first].insert(bestNode);
						}

						else if constexpr (Mode == Dijkstra_modes::all_previous_and_next_nodes_and_distance)
						{
							previous[neighbor.first].first.insert(bestNode);
						}

						else if constexpr (Mode == Dijkstra_modes::node_betweenness)
						{
							const typename GraphType::size_type pathCount = previous.at(bestNode).first;

							previous[neighbor.first].first += pathCount;
							previous[neighbor.first].second.insert(bestNode);
						}

						else if constexpr (Mode == Dijkstra_modes::edge_betweenness)
						{
							const typename GraphType::size_type pathCount = std::get < 0 > (previous.first.at(bestNode));

							std::get < 0 > (previous.first[neighbor.first]) += pathCount;
							std::get < 1 > (previous.first[neighbor.first]).insert(bestNode);
						}
					}
				}
			}

			if constexpr (Mode == Dijkstra_modes::single_path)
			{
				if (std::addressof(bestNode.get()) == std::addressof(destiny.value().get()))
				{
					smallestDistance.value().get() = bestDistance;

					break;
				}
			}

			distances.erase(iterator);
		}
	}

	template < Dijkstra_modes Mode, bool Unordered >
	void Dijkstra_create_next(auto &previous, auto &next)
	{
		if constexpr (Unordered)
		{
			next.reserve(previous.size());
		}

		for (const auto [node, _] : previous)
		{
			next[node];
		}

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

	template < typename ConnectionsContainerType, typename VisitedNodesContainerType, concepts::base_graph GraphType >
	[[nodiscard]] ConnectionsContainerType breadth_first_search(const GraphType &graph, const typename GraphType::value_type &origin)
	{
		ConnectionsContainerType next{};
		VisitedNodesContainerType visitedNodes{};
		std::queue < std::reference_wrapper < const typename GraphType::value_type > > queue;

		if constexpr (GraphType::unordered())
		{
			const auto nodeCount = graph.node_count();

			next.reserve(nodeCount);
			visitedNodes.reserve(nodeCount);
		}

		visitedNodes.insert(origin);
		queue.push(origin);

		while (!queue.empty())
		{
			const auto currentNode = std::move(queue.front());

			queue.pop();

			for (const auto &[neighbor, _] : graph.container().at(currentNode))
			{
				if (!visitedNodes.contains(neighbor))
				{
					visitedNodes.insert(neighbor);

					if constexpr (GraphType::unordered())
					{
						next[currentNode].reserve(graph.expected_connections_by_node());
					}

					next[currentNode].insert(neighbor);

					queue.push(neighbor);
				}
			}
		}

		return next;
	}

	template < typename ConnectionsContainerType, typename VisitedNodesContainerType, concepts::base_graph GraphType >
	[[nodiscard]] ConnectionsContainerType depth_first_search(const GraphType &graph, const typename GraphType::value_type &origin)
	{
		using node_pair =
			std::pair <
				std::reference_wrapper < const typename GraphType::value_type >,
				std::reference_wrapper < const typename GraphType::value_type > >;

		ConnectionsContainerType next{};
		VisitedNodesContainerType visitedNodes{};
		std::stack < node_pair, std::vector < node_pair > > stack;
		node_pair previousAndCurrentNodes{ origin, origin };

		if constexpr (GraphType::unordered())
		{
			next.reserve(graph.node_count());
			visitedNodes.reserve(graph.node_count());
		}

		stack.push(previousAndCurrentNodes);

		while (!stack.empty())
		{
			previousAndCurrentNodes = std::move(stack.top());

			const auto &previousNode = previousAndCurrentNodes.first;
			const auto &currentNode = previousAndCurrentNodes.second;

			stack.pop();

			if (!visitedNodes.contains(currentNode))
			{
				visitedNodes.insert(currentNode);

				if (std::addressof(previousNode.get()) != std::addressof(currentNode.get()))
				{
					if constexpr (GraphType::unordered())
					{
						next[previousNode].reserve(graph.expected_connections_by_node());
					}

					next[previousNode].insert(currentNode);
				}

				for (const auto &[neighbor, _] : graph.container().at(currentNode))
				{
					stack.push(std::make_pair(currentNode, neighbor));
				}
			}
		}

		return next;
	}

	template < concepts::base_graph GraphType, typename BetweennessPreviousContainerType >
	[[nodiscard]] typename GraphType::edge_weight_type betweenness_centrality_of_a_node_in_a_path(
		const typename GraphType::value_type &target,
		const typename GraphType::value_type &origin,
		const typename GraphType::value_type &destiny,
		const BetweennessPreviousContainerType &previous)
	{
		std::reference_wrapper < const typename GraphType::value_type > currentNode = destiny;
		std::queue < std::reference_wrapper < const typename GraphType::value_type > > queue;

		while (std::addressof(currentNode.get()) != std::addressof(origin))
		{
			if (std::addressof(currentNode.get()) == std::addressof(target))
			{
				return
					static_cast < typename GraphType::edge_weight_type > (previous.at(currentNode).first) /
						static_cast < typename GraphType::edge_weight_type > (previous.at(destiny).first);
			}

			for (const auto &node : previous.at(currentNode).second)
			{
				queue.push(node);
			}

			currentNode = queue.front();

			queue.pop();
		}

		return typename GraphType::edge_weight_type{};
	}

	template < typename BetweennessPreviousContainerSetType, typename MappedType, concepts::base_graph GraphType >
	[[nodiscard]] BetweennessPreviousContainerSetType calculate_previous_for_node_betweenness_centrality(const GraphType &graph)
	{
		std::vector < std::thread > threads;
		BetweennessPreviousContainerSetType previous{};

		if constexpr (GraphType::unordered())
		{
			previous.reserve(graph.node_count());
		}

		for (const auto &[node, _] : graph.container())
		{
			previous.emplace(node, typename BetweennessPreviousContainerSetType::mapped_type{});
		}

		for (const auto &[node, _] : graph.container())
		{
			// TODO: Deal with possible throws in these threads.
			threads.emplace_back(
				[&graph, &node, &previous] () -> void
				{
					internals::generic_Dijkstra <
						Dijkstra_modes::node_betweenness,
						MappedType > (
							graph,
							node,
							previous[node]);
				});
		}

		for (auto &thread : threads)
		{
			thread.join();
		}

		return previous;
	}

	template < typename EdgeBetweennessCentralityPreviousContainerListType, typename MappedType, concepts::base_graph GraphType >
	[[nodiscard]] EdgeBetweennessCentralityPreviousContainerListType calculate_previous_for_edge_betweenness_centrality(const GraphType &graph)
	{
		typename GraphType::size_type counter{};
		std::vector < std::thread > threads;
		EdgeBetweennessCentralityPreviousContainerListType previous(graph.node_count());

		for (const auto &[node, _] : graph.container())
		{
			// TODO: Deal with possible throws in these threads.
			threads.emplace_back(
				[&graph, &node, &previous, counter] () -> void
				{
					internals::generic_Dijkstra <
						Dijkstra_modes::edge_betweenness,
						MappedType > (
							graph,
							node,
							previous[counter]);
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
		typename GraphNodeToMatrixIndexMap,
		typename MatrixIndexToGraphNodeMap,
		xablau::algebra::concepts::xablau_matrix_dense MatrixType,
		xablau::algebra::concepts::xablau_matrix_dense NextNodeType,
		concepts::base_graph GraphType >
	requires (std::same_as < typename MatrixType::value_type, typename GraphType::edge_weight_type >)
	[[nodiscard]] auto Floyd_Warshall(const GraphType &graph)
	{
		using MatrixValueType = MatrixType::value_type;

		typename GraphType::size_type counter{};
		const typename GraphType::size_type matrixLineLength = graph.node_count();

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

		GraphNodeToMatrixIndexMap graphNodeToMatrixIndex{};
		MatrixIndexToGraphNodeMap matrixIndexToGraphNode{};

		if constexpr (GraphType::unordered())
		{
			graphNodeToMatrixIndex.reserve(matrixLineLength);
			matrixIndexToGraphNode.reserve(matrixLineLength);
		}

		for (const auto &[node, _] : graph.container())
		{
			graphNodeToMatrixIndex[node] = counter;
			matrixIndexToGraphNode[counter] = node;

			counter++;
		}

		for (const auto &[node1, connections] : graph.container())
		{
			const typename GraphType::size_type lineIndex1 = graphNodeToMatrixIndex[node1];

			for (const auto &[node2, edges] : connections)
			{
				const typename GraphType::size_type lineIndex2 = graphNodeToMatrixIndex[node2];

				if constexpr (GraphType::can_have_multiple_edges())
				{
					distanceMatrix(lineIndex1, lineIndex2) =
						std::min_element(edges.cbegin(), edges.cend())->weight();
				}

				else
				{
					distanceMatrix(lineIndex1, lineIndex2) = edges.weight();
				}

				nextNode(lineIndex1, lineIndex2) = node2;
			}

			distanceMatrix(lineIndex1, lineIndex1) = MatrixValueType{};
			nextNode(lineIndex1, lineIndex1) = node1;
		}

		for (typename GraphType::size_type k = 0; k < matrixLineLength; k++)
		{
			for (typename GraphType::size_type i = 0; i < matrixLineLength; i++)
			{
				const auto distanceIK = distanceMatrix(i, k);

				for (typename GraphType::size_type j = 0; j < matrixLineLength; j++)
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

	template < concepts::base_graph GraphType >
	[[nodiscard]] typename GraphType::edge_weight_type Shannon_entropy(const GraphType &graph)
	{
		const auto degree_distribution = graph.degree_distribution();

		if (degree_distribution.empty())
		{
			return typename GraphType::edge_weight_type{};
		}

		typename GraphType::edge_weight_type acumulator{};

		for (const auto &item : degree_distribution)
		{
			if (item.second != typename GraphType::edge_weight_type{})
			{
				acumulator -=
					item.second * xablau::algebra::functions::log2 < typename GraphType::edge_weight_type > ::invoke(item.second);
			}
		}

		return acumulator;
	}

	template < typename MappedType, typename PreviousContainerSet, concepts::base_graph GraphType >
	[[nodiscard]] typename GraphType::edge_weight_type node_betweenness_centrality(
		const GraphType &graph,
		const typename GraphType::value_type &node)
	{
		const auto iterator = graph.container().find(node);

		if (iterator == graph.container().cend())
		{
			return typename GraphType::edge_weight_type{};
		}

		typename GraphType::edge_weight_type betweennessCentrality{};
		const auto &target = iterator->first;
		const auto previous =
			internals::calculate_previous_for_node_betweenness_centrality <
				PreviousContainerSet,
				MappedType > (graph);

		for (auto iterator1 = previous.cbegin(); iterator1 != previous.cend(); ++iterator1)
		{
			const auto &node1 = iterator1->first;
			auto iterator2 = iterator1;

			++iterator2;

			for (; iterator2 != previous.cend(); ++iterator2)
			{
				const auto &node2 = iterator2->first;

				if (std::addressof(target) != std::addressof(node1.get()) &&
					std::addressof(target) != std::addressof(node2.get()) &&
					iterator1->second.contains(node1) &&
					iterator1->second.contains(node2) &&
					iterator1->second.contains(target))
				{
					betweennessCentrality +=
						internals::betweenness_centrality_of_a_node_in_a_path < GraphType > (target, node1, node2, iterator1->second);
				}
			}
		}

		return betweennessCentrality;
	}

	template < typename MappedType, typename PreviousContainerSet, typename NodeWeightContainer, concepts::base_graph GraphType >
	[[nodiscard]] NodeWeightContainer node_betweenness_centrality(const GraphType &graph)
	{
		const auto previous =
			internals::calculate_previous_for_node_betweenness_centrality <
				PreviousContainerSet,
				MappedType > (graph);

		NodeWeightContainer betweennessCentrality{};

		if constexpr (GraphType::unordered())
		{
			betweennessCentrality.reserve(graph.node_count());
		}

		for (auto iterator1 = previous.cbegin(); iterator1 != previous.cend(); ++iterator1)
		{
			const auto &node1 = iterator1->first;
			auto iterator2 = iterator1;

			++iterator2;

			for (; iterator2 != previous.cend(); ++iterator2)
			{
				const auto &node2 = iterator2->first;

				for (const auto &[target, edges] : graph.container())
				{
					if (std::addressof(target) != std::addressof(node1.get()) &&
						std::addressof(target) != std::addressof(node2.get()) &&
						iterator1->second.contains(node1) &&
						iterator1->second.contains(node2) &&
						iterator1->second.contains(target))
					{
						betweennessCentrality[target] +=
							internals::betweenness_centrality_of_a_node_in_a_path < GraphType > (target, node1, node2, iterator1->second);
					}
				}
			}
		}

		return betweennessCentrality;
	}

	template < typename MappedType, typename PreviousContainerList, typename EdgeBetweennessContainer, concepts::base_graph GraphType >
	[[nodiscard]] EdgeBetweennessContainer edge_betweenness_centrality(const GraphType &graph)
	{
		auto previous =
			internals::calculate_previous_for_edge_betweenness_centrality <
				PreviousContainerList,
				MappedType > (graph);

		EdgeBetweennessContainer betweennessCentrality{};

		if constexpr (GraphType::unordered())
		{
			betweennessCentrality.reserve(graph.node_count());
		}

		for (auto iterator = previous.begin(); iterator != previous.end(); ++iterator)
		{
			auto &nodeTree = iterator->first;
			auto &stack = iterator->second;

			while (!stack.empty())
			{
				const auto &currentNode = stack.top();

				const typename GraphType::size_type pathCount = std::get < 0 > (nodeTree.at(currentNode));
				const auto &immediatePreviousSet = std::get < 1 > (nodeTree.at(currentNode));
				const auto &currentBetweennessWeight = std::get < 2 > (nodeTree.at(currentNode));

				for (const auto &immediatePrevious : immediatePreviousSet)
				{
					const typename GraphType::size_type immediatePreviousPathCount = std::get < 0 > (nodeTree.at(immediatePrevious));
					auto &immediatePreviousBetweennessWeight = std::get < 2 > (nodeTree.at(immediatePrevious));

					const typename GraphType::edge_weight_type factor =
						static_cast < typename GraphType::edge_weight_type > (immediatePreviousPathCount) /
							static_cast < typename GraphType::edge_weight_type > (pathCount);

					typename GraphType::edge_weight_type localImmediatePreviousBetweennessWeight =
						(typename GraphType::edge_weight_type{1} + currentBetweennessWeight) * factor;

					betweennessCentrality[std::make_pair(currentNode, immediatePrevious)] +=
						localImmediatePreviousBetweennessWeight;

					immediatePreviousBetweennessWeight += localImmediatePreviousBetweennessWeight;
				}

				stack.pop();
			}
		}

		return betweennessCentrality;
	}

	template <
		bool CheckCommunitiesClassification,
		typename NodeSetContainer,
		typename CommunityType,
		concepts::base_graph GraphType >
	[[nodiscard]] typename GraphType::edge_weight_type modularity(
		const GraphType &graph,
		const CommunityType &communitiesClassification,
		const typename GraphType::edge_weight_type weightSum)
	{
		if (weightSum == typename GraphType::edge_weight_type{})
		{
			throw std::domain_error("There are no edges in the graph.");
		}

		if constexpr (CheckCommunitiesClassification)
		{
			NodeSetContainer insertedNodes{};

			for (const auto &nodeSet : communitiesClassification)
			{
				for (const auto &node : nodeSet)
				{
					constexpr auto message = "There is a node in \"communitiesClassification\" not present in the graph.";

					if (!graph.container().contains(static_cast < const typename GraphType::value_type & > (node)))
					{
						throw std::runtime_error(message);
					}

					insertedNodes.insert(node);
				}
			}

			if (insertedNodes.size() != graph.container().size())
			{
				throw std::runtime_error("Not every node in the graph is present in \"communitiesClassification\".");
			}
		}

		const typename GraphType::edge_weight_type inverseOfTwoTimesOfWeightSum = typename GraphType::edge_weight_type{1} / weightSum;
		typename GraphType::edge_weight_type sum{};
		std::mutex mutex;

		// TODO: Deal with possible throws in these threads.
		std::for_each(std::execution::par, communitiesClassification.cbegin(), communitiesClassification.cend(),
			[&] (const typename CommunityType::value_type &nodeSet) -> void
			{
				typename GraphType::edge_weight_type weightSum{};
				typename GraphType::edge_weight_type degreeSum{};

				for (const auto &node1 : nodeSet)
				{
					degreeSum += graph.degree(node1);

					for (const auto &node2 : nodeSet)
					{
						weightSum +=
							graph.weight(
								static_cast < const typename GraphType::value_type & > (node1),
								static_cast < const typename GraphType::value_type & > (node2));
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
		bool AllowAutoEdges,
		bool Graph,
		typename GraphNodeToMatrixIndexMap,
		typename MatrixIndexToGraphNodeMap,
		concepts::base_graph GraphType >
	[[nodiscard]] auto adjacency_matrix(const GraphType &graph)
	{
		using MatrixValueType = typename MatrixType::value_type;

		typename GraphType::size_type counter{};
		const auto matrixLineLength = graph.node_count();

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

		GraphNodeToMatrixIndexMap graphNodeToMatrixIndex{};
		MatrixIndexToGraphNodeMap matrixIndexToGraphNode{};

		if constexpr (GraphType::unordered())
		{
			graphNodeToMatrixIndex.reserve(matrixLineLength);
			matrixIndexToGraphNode.reserve(matrixLineLength);
		}

		for (const auto &[node, _] : graph.container())
		{
			graphNodeToMatrixIndex.emplace(node, counter);
			matrixIndexToGraphNode.emplace(counter, node);

			counter++;
		}

		std::for_each(graph.container().cbegin(), graph.container().cend(),
			[&adjacencyMatrix, &graphNodeToMatrixIndex] (const auto &node1) -> void
			{
				const auto lineIndex1 = graphNodeToMatrixIndex.at(node1.first);

				for (const auto &node2 : node1.second)
				{
					const auto lineIndex2 = graphNodeToMatrixIndex.at(node2.first);

					if constexpr (GraphType::can_have_multiple_edges())
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

		if constexpr (AllowAutoEdges && Graph)
		{
			for (typename GraphType::size_type i = 0; i < matrixLineLength; i++)
			{
				adjacencyMatrix(i, i) *= MatrixValueType{2};
			}
		}

		else if constexpr (!AllowAutoEdges)
		{
			for (typename GraphType::size_type i = 0; i < matrixLineLength; i++)
			{
				adjacencyMatrix(i, i) = MatrixValueType{};
			}
		}

		return std::make_tuple(std::move(adjacencyMatrix), std::move(graphNodeToMatrixIndex), std::move(matrixIndexToGraphNode));
	}

	template <
		xablau::algebra::concepts::xablau_matrix_dense MatrixType,
		bool Graph,
		typename GraphNodeToMatrixIndexMap,
		typename MatrixIndexToGraphNodeMap,
		concepts::base_graph GraphType >
	[[nodiscard]] auto laplacian_matrix(const GraphType &graph)
	{
		using MatrixValueType = typename MatrixType::value_type;

		auto [laplacianMatrix, graphNodeToMatrixIndex, matrixIndexToGraphNode] =
			internals::adjacency_matrix < MatrixType, false, Graph, GraphNodeToMatrixIndexMap, MatrixIndexToGraphNodeMap > (graph);

		std::for_each(matrixIndexToGraphNode.begin(), matrixIndexToGraphNode.end(),
			[graph, &laplacianMatrix] (const std::pair < const typename GraphType::size_type, typename GraphType::value_type > &pair) -> void
			{
				auto &diagonalElement = laplacianMatrix(pair.first, pair.first);
				const auto loopWeight = graph.weight(pair.second, pair.second);

				if constexpr (Graph)
				{
					diagonalElement = (loopWeight + loopWeight) - graph.degree(pair.second);
				}

				else
				{
					diagonalElement = loopWeight - graph.degree(pair.second);
				}
			});

		laplacianMatrix *= MatrixValueType{-1};

		return std::make_tuple(std::move(laplacianMatrix), std::move(graphNodeToMatrixIndex), std::move(matrixIndexToGraphNode));
	}

	template < bool CreatePath, typename NodeSelectionType, typename PreviousContainerSet, concepts::base_graph GraphType >
	[[nodiscard]] auto Dijkstra_single_path(
		const GraphType &graph,
		const typename GraphType::value_type &origin,
		const typename GraphType::value_type &destiny)
	{
		std::vector < NodeSelectionType > path;

		const auto iteratorOrigin = graph.container().find(origin);
		const auto iteratorDestiny = graph.container().find(destiny);

		if (iteratorOrigin == graph.container().cend() || iteratorDestiny == graph.container().cend())
		{
			if constexpr (CreatePath)
			{
				return
					std::make_pair(
						std::numeric_limits < typename GraphType::edge_weight_type > ::max(),
						std::vector < typename GraphType::value_type > ());
			}

			else
			{
				return std::numeric_limits < typename GraphType::edge_weight_type > ::max();
			}
		}

		if (std::addressof(iteratorOrigin->first) == std::addressof(iteratorDestiny->first))
		{
			if constexpr (CreatePath)
			{
				path.push_back(destiny);

				return std::make_pair(typename GraphType::edge_weight_type{}, std::move(path));
			}

			else
			{
				return typename GraphType::edge_weight_type{};
			}
		}

		typename GraphType::edge_weight_type smallestDistance{};

		PreviousContainerSet previous{};

		internals::generic_Dijkstra <
			Dijkstra_modes::single_path > (
				graph,
				iteratorOrigin->first,
				previous,
				iteratorDestiny->first,
				smallestDistance);

		if constexpr (CreatePath)
		{
			std::reference_wrapper < const typename GraphType::value_type > current = iteratorDestiny->first;

			while (true)
			{
				const auto iterator = previous.find(current.get());

				path.push_back(current.get());

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

			return std::make_pair(smallestDistance, std::move(path));
		}

		else
		{
			return smallestDistance;
		}
	}

	template < typename NodeSelectionType, concepts::base_graph GraphType >
	[[nodiscard]] auto traveling_salesman_problem(const GraphType &graph, const typename GraphType::value_type &start)
	{
		const auto iterator = graph.container().find(start);

		if (iterator == graph.container().cend())
		{
			return
				std::make_pair(
					std::vector < NodeSelectionType > (),
					typename GraphType::edge_weight_type{});
		}

		if (graph.node_count() == typename GraphType::size_type{1})
		{
			return
				std::make_pair(
					std::vector < NodeSelectionType > (1, iterator->first),
					typename GraphType::edge_weight_type{});
		}

		std::reference_wrapper < const typename GraphType::value_type > thisStart = iterator->first;
		std::vector < std::reference_wrapper < const typename GraphType::value_type > > nodes;

		constexpr auto comparison =
			[] (const std::reference_wrapper < const typename GraphType::value_type > &node1,
				const std::reference_wrapper < const typename GraphType::value_type > &node2) -> bool
			{
				return std::less < const typename GraphType::value_type * > {}(std::addressof(node1.get()), std::addressof(node2.get()));
			};

		nodes.reserve(graph.node_count());

		for (const auto &[node, _] : graph.container())
		{
			if (std::addressof(thisStart.get()) != std::addressof(node))
			{
				nodes.emplace_back(node);
			}
		}

		std::ranges::sort(nodes, comparison);

		nodes.insert(nodes.begin(), thisStart);

		bool foundSolution = false;
		std::vector < std::reference_wrapper < const typename GraphType::value_type > > bestPath;
		typename GraphType::edge_weight_type minimumWeight = std::numeric_limits < typename GraphType::edge_weight_type > ::max();

		do
		{
			bool validPath = true;
			typename GraphType::edge_weight_type currentPathweight{};

			for (auto previous = nodes.begin(), next = ++(nodes.begin()); next != nodes.end(); ++previous, ++next)
			{
				if (const auto connection = graph.edges(previous->get(), next->get());
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

				if (currentPathweight < minimumWeight)
				{
					minimumWeight = currentPathweight;
					bestPath = nodes;
				}
			}
		} while (std::next_permutation(nodes.begin() + 1, nodes.end(), comparison));

		if (foundSolution)
		{
			std::vector < NodeSelectionType > result;

			result.reserve(bestPath.size());

			for (const auto &node : bestPath)
			{
				result.push_back(node.get());
			}

			return std::make_pair(std::move(result), minimumWeight);
		}

		return
			std::make_pair(
				std::vector < NodeSelectionType > (),
				typename GraphType::edge_weight_type{});
	}

	template < typename NodeSelectionType, concepts::digraph DependencyType, concepts::base_graph GraphType >
	[[nodiscard]] auto traveling_salesman_problem(
		const GraphType &graph,
		const auto &equalNodes,
		const auto &differentNodes,
		const typename GraphType::value_type &start,
		const DependencyType &dependency)
	{
		if (graph.node_count() != dependency.node_count())
		{
			throw std::runtime_error(""); // TODO: Create message.
		}

		if (graph.node_count() != dependency.node_count())
		{
			throw std::runtime_error(""); // TODO: Create message.
		}

		for (const auto &[node, _] : graph.container())
		{
			if (!dependency.contains(node))
			{
				throw std::runtime_error(""); // TODO: Create message.
			}
		}

		const auto iterator = graph.container().find(start);

		if (iterator == graph.container().cend())
		{
			return
				std::make_pair(
					std::vector < NodeSelectionType > (),
					typename GraphType::edge_weight_type{});
		}

		if (graph.node_count() == typename GraphType::size_type{1})
		{
			return
				std::make_pair(
					std::vector < NodeSelectionType > (1, iterator->first),
					typename GraphType::edge_weight_type{});
		}

		std::vector < std::tuple < typename GraphType::size_type, std::reference_wrapper < const typename GraphType::value_type > > > nodes;
		std::vector <
			std::pair <
				typename decltype(nodes)::difference_type,
				typename decltype(nodes)::difference_type > > groups;

		constexpr auto priorityAndNodeComparison =
			[] (const std::tuple < typename GraphType::size_type, std::reference_wrapper < const typename GraphType::value_type > > &node1,
				const std::tuple < typename GraphType::size_type, std::reference_wrapper < const typename GraphType::value_type > > &node2) -> bool
			{
				return
					std::get < 0 > (node1) < std::get < 0 > (node2) ||

					std::get < 0 > (node1) == std::get < 0 > (node2) &&
					std::less < const typename GraphType::value_type * > {}(
						std::addressof(std::get < 1 > (node1).get()),
						std::addressof(std::get < 1 > (node2).get()));
			};

		auto dependencyCopy = dependency;

		nodes.reserve(graph.node_count());
		groups.reserve(graph.node_count());

		for (typename GraphType::size_type priority{}, accumulator{}; !dependencyCopy.empty();)
		{
			const auto sinkNodes = dependencyCopy.template sink_nodes < false > ();
			const auto sinkNodeCount = std::distance(sinkNodes.cbegin(), sinkNodes.cend());

			if (sinkNodeCount == 0)
			{
				throw std::runtime_error(""); // TODO: Create message.
			}

			groups.emplace_back(
				static_cast < typename decltype(nodes)::difference_type > (accumulator),
				static_cast < typename decltype(nodes)::difference_type > (accumulator + sinkNodeCount));

			for (const auto &sinkNode : sinkNodes)
			{
				nodes.emplace_back(priority, graph.container().find(sinkNode.get())->first);
				dependencyCopy.erase(sinkNode.get());
			}

			accumulator += static_cast <typename GraphType::size_type > (sinkNodeCount);

			priority++;
		}

		std::ranges::sort(nodes, priorityAndNodeComparison);

		if (groups[0].second - groups[0].first != 1 ||
			differentNodes(std::get < 1 > (*(nodes.cbegin() + groups[0].first)).get(), iterator->first))
		{
			return
				std::make_pair(
					std::vector < NodeSelectionType > (),
					typename GraphType::edge_weight_type{});
		}

		bool foundSolution = false;
		decltype(nodes) bestPath;
		typename GraphType::edge_weight_type minimumWeight = std::numeric_limits < typename GraphType::edge_weight_type > ::max();
		const auto permuteNodes =
			[&groups, &nodes] () -> bool
			{
				constexpr auto nodeComparison =
					[] (const std::tuple < typename GraphType::size_type, std::reference_wrapper < const typename GraphType::value_type > > &node1,
						const std::tuple < typename GraphType::size_type, std::reference_wrapper < const typename GraphType::value_type > > &node2) -> bool
					{
						return
							std::less < const typename GraphType::value_type * > {}(
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
			typename GraphType::edge_weight_type currentPathweight{};
			dependencyCopy = dependency;

			for (auto previous = nodes.begin(), next = ++(nodes.begin()); next != nodes.end(); ++previous, ++next)
			{
				const auto connection = graph.edges(std::get < 1 > (*previous).get(), std::get < 1 > (*next).get());
				const auto sinkNodes = dependencyCopy.template sink_nodes < false > ();
				const auto searchFunction =
					[&equalNodes, &previous] < typename SinkNodeType > (const SinkNodeType &sinkNode) -> bool
					{
						return equalNodes(std::get < 1 > (*previous).get(), sinkNode.get());
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
			std::vector < NodeSelectionType > result;

			result.reserve(bestPath.size());

			for (const auto &[_, node] : bestPath)
			{
				result.push_back(node.get());
			}

			return std::make_pair(std::move(result), minimumWeight);
		}

		return
			std::make_pair(
				std::vector < NodeSelectionType > (),
				typename GraphType::edge_weight_type{});
	}

	template <
		typename ConnectionsContainerType,
		typename NodeSetType,
		concepts::nary_tree TreeType,
		tree_generation_modes TreeGenerationMode,
		concepts::base_graph GraphType >
	[[nodiscard]] TreeType generate_tree(const GraphType &graph, const typename GraphType::value_type &origin)
	{
		const auto iterator = graph.container().find(origin);

		if (iterator == graph.container().cend())
		{
			return TreeType{};
		}

		const auto &node = iterator->first;

		ConnectionsContainerType next{};

		if constexpr (TreeGenerationMode == tree_generation_modes::breadth_first_search)
		{
			next =
				internals::breadth_first_search <
					ConnectionsContainerType,
					NodeSetType > (graph, node);
		}

		else if constexpr (TreeGenerationMode == tree_generation_modes::depth_first_search)
		{
			next =
				internals::depth_first_search <
					ConnectionsContainerType,
					NodeSetType > (graph, node);
		}

		else if constexpr (TreeGenerationMode == tree_generation_modes::Dijkstra)
		{
			auto [_, _next] = graph.Dijkstra_single_previous_and_next_nodes < false > (node);

			next = std::move(_next);
		}

		TreeType tree{};
		std::queue < typename TreeType::iterator > queue;

		queue.push(tree.insert_root(node));

		while (!queue.empty())
		{
			const auto &currentNodeInTree = queue.front();
			const auto &currentNodeInGraph = *currentNodeInTree;

			if (next.contains(currentNodeInGraph))
			{
				const auto &connections = next.at(currentNodeInGraph);

				for (const auto &connection : connections)
				{
					queue.emplace(tree.insert_back_child(currentNodeInTree, connection));
				}
			}

			queue.pop();
		}

		return tree;
	}

	template <
		typename NodeNodeContainer,
		concepts::base_graph GraphType >
	[[nodiscard]] void shuffle(const GraphType &graph, GraphType &newGraph)
	{
		std::random_device randomDevice{};
		std::default_random_engine engine(randomDevice());
		NodeNodeContainer indicesMap{};
		std::vector < std::reference_wrapper < const typename GraphType::value_type > > indices;

		indices.reserve(graph.node_count());

		for (const auto &[node, _] : graph.container())
		{
			indices.push_back(node);
		}

		std::shuffle(indices.begin(), indices.end(), engine);

		typename GraphType::size_type counter{};

		for (const auto &[node, _] : graph.container())
		{
			indicesMap.emplace(node, indices[counter]);

			counter++;
		}

		for (const auto &[node1, connections] : graph.container())
		{
			for (const auto &[node2, edges] : connections)
			{
				if constexpr (GraphType::can_have_multiple_edges())
				{
					for (const auto &edge : edges)
					{
						newGraph.insert(indicesMap.at(node1), indicesMap.at(node2), edge);
					}
				}

				else
				{
					newGraph.insert(indicesMap.at(node1), indicesMap.at(node2), edges);
				}
			}
		}
	}
}