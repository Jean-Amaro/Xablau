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

export module xablau.graph:graph_configurations;

namespace xablau::graph
{
	export enum class graph_container_type_value
	{
		ordered,
		unordered,
		multi_ordered,
		multi_unordered
	};

	export enum class tree_generation_modes
	{
		breadth_first_search,
		depth_first_search,
		Dijkstra
	};

	enum class Dijkstra_modes
	{
		all_previous_and_next_nodes,
		all_previous_and_next_nodes_and_distance,
		single_previous_and_next_node,
		single_previous_and_next_node_and_distance,
		single_path,
		node_betweenness,
		edge_betweenness
	};

	export template < graph_container_type_value Type >
	struct graph_container_type
	{
		static consteval graph_container_type_value type() noexcept
		{
			return Type;
		}
	};
}