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

export module xablau.graph:fundamental_concepts;
export import :graph_configurations;
export import :node_configurations;

export import <functional>;
export import <iterator>;
export import <memory>;
export import <optional>;
export import <ranges>;
export import <vector>;

export import xablau.algebra;

namespace xablau::graph::concepts
{
	namespace internals
	{
		template < typename Class, typename EdgeType >
		concept group_of_edges_single = std::same_as < Class, const EdgeType >;

		template < typename Class, typename EdgeType >
		concept group_of_edges_multi =
			std::ranges::forward_range < Class > &&
			std::same_as < std::ranges::range_value_t < Class >, EdgeType > &&
			std::is_const < Class > ::value &&

			requires (Class container)
			{
				{ *std::ranges::begin(container) } -> std::same_as < const EdgeType & >;
			};

		template < typename Class, typename EdgeType, bool Multi >
		concept group_of_edges =
			Multi && group_of_edges_multi < typename std::remove_reference < Class > ::type, EdgeType > ||
			!Multi && group_of_edges_single < typename std::remove_reference < Class > ::type, EdgeType >;

		template < typename Class, typename NodeType, typename EdgeType, bool Multi >
		concept group_of_group_of_edges =
			std::ranges::forward_range < Class > &&
			std::is_const < Class > ::value &&
			std::same_as < typename std::tuple_element < 0, std::ranges::range_value_t < Class > > ::type, const NodeType > &&

			requires (Class container)
			{
				{ std::get < 1 > (*std::ranges::begin(container)) } -> group_of_edges < EdgeType, Multi >;
			};

		template < typename Class, typename EdgeType, bool Multi >
		concept optional_reference_of_group_of_edges =
			std::same_as < Class, std::optional < typename Class::value_type > > &&
			std::same_as < typename Class::value_type, std::reference_wrapper < typename Class::value_type::type > > &&
			group_of_edges <
				typename Class::value_type::type,
				EdgeType,
				Multi >;

		template < typename Class, typename NodeType, typename EdgeType, bool Multi >
		concept optional_reference_of_group_of_group_of_edges =
			std::same_as < Class, std::optional < typename Class::value_type > > &&
			std::same_as < typename Class::value_type, std::reference_wrapper < typename Class::value_type::type > > &&
			group_of_group_of_edges <
				typename Class::value_type::type,
				NodeType,
				EdgeType,
				Multi >;

		template < typename Type, template < graph::nary_tree_node_container_value > typename Unspecialized >
		struct is_specialization_nary_tree_node_container_value : std::false_type {};

		template < template < graph::nary_tree_node_container_value > typename Unspecialized, graph::nary_tree_node_container_value Arg >
		struct is_specialization_nary_tree_node_container_value < Unspecialized < Arg >, Unspecialized > : std::true_type {};

		template < typename Pointer >
		concept shared_pointer =
			std::same_as < Pointer, std::shared_ptr < typename Pointer::element_type > >;

		template < typename Pointer >
		concept unique_pointer =
			std::same_as <
				Pointer,
				std::unique_ptr < typename Pointer::element_type, typename Pointer::deleter_type > >;

		template < typename Pointer >
		concept pointer = unique_pointer < Pointer > || shared_pointer < Pointer >;

		template < typename Class >
		concept pair_of_pointer_and_binary_node_side =
			pointer < typename std::tuple_element < 0, Class > ::type > &&
			std::same_as < typename std::tuple_element < 1, Class > ::type, xablau::graph::binary_node_side >;

		template < typename Class >
		concept optional_of_pair_of_pointer_and_binary_side =
			std::same_as < Class, std::optional < typename Class::value_type > > &&
			pair_of_pointer_and_binary_node_side < typename Class::value_type >;

		template < typename Container, typename Type >
		concept bidirectional_range_of_reference_of =
			std::ranges::bidirectional_range < Container > &&
			std::same_as < typename Container::value_type, std::reference_wrapper < typename Container::value_type::type > > &&
			std::same_as < typename Container::value_type::type, Type >;

		template < typename Container, typename Type >
		concept bidirectional_range_of_optional_of_reference_of =
			std::ranges::bidirectional_range < Container > &&
			std::same_as < typename Container::value_type, std::optional < typename Container::value_type::value_type > > &&
			std::same_as < typename Container::value_type::value_type, std::reference_wrapper < typename Container::value_type::value_type::type > > &&
			std::same_as < typename Container::value_type::value_type::type, Type >;

		template < typename Class, typename SizeType >
		concept pair_of_pointer_and_size_type =
			pointer < typename std::tuple_element < 0, Class > ::type > &&
			std::same_as < typename std::tuple_element < 1, Class > ::type, SizeType >;

		template < typename Class, typename SizeType >
		concept optional_of_pair_of_pointer_and_size_type =
			std::same_as < Class, std::optional < typename Class::value_type > > &&
			pair_of_pointer_and_size_type < typename Class::value_type, SizeType >;

		template < typename Class, typename Iterator, typename Type >
		concept pair_of_iterator_and_forward_container_of =
			std::same_as < typename std::tuple_element < 0, Class > ::type, Iterator > &&
			std::ranges::forward_range < typename std::tuple_element < 1, Class > ::type > &&
			std::same_as <
				std::ranges::range_value_t < typename std::tuple_element < 1, Class > ::type >,
				Type >;

		template < typename Container, typename Type >
		concept forward_container_of_reference_wrapper_of =
			std::ranges::forward_range < Container > &&
			std::same_as < std::ranges::range_value_t < Container >, std::reference_wrapper < typename std::ranges::range_value_t < Container > ::type > > &&
			std::same_as < typename std::ranges::range_value_t < Container > ::type, Type >;

		template < typename Container, typename Type, bool Same >
		concept forward_container_of =
			std::ranges::forward_range < Container > &&
			(Same ?
				std::same_as < std::ranges::range_value_t < Container >, Type > :
				std::convertible_to < std::ranges::range_value_t < Container >, Type >);

		template < typename Type, template < graph::graph_container_type_value > typename Unspecialized >
		struct is_specialization_graph_container_type_value : std::false_type {};

		template < template < graph::graph_container_type_value > typename Unspecialized, graph::graph_container_type_value Arg >
		struct is_specialization_graph_container_type_value < Unspecialized < Arg >, Unspecialized > : std::true_type {};

		template < typename ValueType >
		concept has_less_than =
			requires ()
			{
				{ std::less < ValueType > {}(ValueType{}, ValueType{}) } -> std::convertible_to < bool >;
			};

		template < typename ValueType >
		concept has_hash_and_equality_comparison =
			requires ()
			{
				{ std::hash < ValueType > {}(ValueType{}) } -> std::unsigned_integral;
				{ std::equal_to < ValueType > {}(ValueType{}, ValueType {}) } -> std::convertible_to < bool >;
			};

		template < typename Graph >
		concept common_graph =
			node < typename Graph::node_type > &&
			edge < typename Graph::edge_type > &&
			std::unsigned_integral < typename Graph::size_type > &&

			requires ()
			{
				{ Graph::can_have_multiple_edges() } -> std::convertible_to < bool >;
				{ Graph::cannot_have_multiple_edges() } -> std::convertible_to < bool >;
				{ Graph::ordered() } -> std::convertible_to < bool >;
				{ Graph::unordered() } -> std::convertible_to < bool >;
			} &&

			requires (Graph mutable_graph)
			{
				{ mutable_graph.clear() } noexcept -> std::same_as < void >;
				{ mutable_graph.insert(std::declval < const typename Graph::node_type & > ()) } -> std::convertible_to < bool >;
				{ mutable_graph.insert(std::declval < const typename Graph::node_type & > (), std::declval < const typename Graph::node_type & > (), std::declval < const typename Graph::edge_type & > ()) } -> std::same_as < std::optional < std::reference_wrapper < const typename Graph::edge_type > > >;
				{ mutable_graph.erase(std::declval < const typename Graph::node_type & > ()) } -> std::convertible_to < bool >;
				{ mutable_graph.erase(std::declval < const typename Graph::node_type & > (), std::declval < const typename Graph::node_type & > ()) } -> std::convertible_to < bool >;
			} &&

			requires (const Graph const_graph)
			{
				{ const_graph.degree(std::declval < const typename Graph::node_type & > ()) } -> std::same_as < typename Graph::edge_type::weight_type >;
				{ const_graph.average_degree() } -> std::same_as < typename Graph::edge_type::weight_type >;
				{ const_graph.density() } -> std::same_as < typename Graph::edge_type::weight_type >;
				{ const_graph.degree_distribution(typename Graph::edge_type::weight_type{}) } -> std::same_as < typename Graph::edge_type::weight_type >;
				{ const_graph.degree_distribution() } -> internals::forward_container_of < std::pair < typename Graph::edge_type::weight_type, typename Graph::edge_type::weight_type >, false >;
				{ const_graph.statistical_moment(typename Graph::size_type{}) } -> std::same_as < typename Graph::edge_type::weight_type >;
				{ const_graph.variance() } -> std::same_as < typename Graph::edge_type::weight_type >;
				{ const_graph.volume() } -> std::same_as < typename Graph::edge_type::weight_type >;
				{ const_graph.complexity_measure() } -> std::same_as < typename Graph::edge_type::weight_type >;
				{ const_graph.Shannon_entropy() } -> std::same_as < typename Graph::edge_type::weight_type >;
				{ const_graph.connected() } -> std::convertible_to < bool >;
				{ const_graph.dismember_disconnected_nodes() } -> internals::forward_container_of < Graph, true >;
				{ const_graph.empty() } noexcept -> std::convertible_to < bool >;
				{ const_graph.node_count() } noexcept -> std::same_as < typename Graph::size_type >;
				{ const_graph.contains(std::declval < const typename Graph::node_type & > ()) } -> std::convertible_to < bool >;
				{ const_graph.contains(std::declval < const typename Graph::node_type & > (), std::declval < const typename Graph::node_type & > ()) } -> std::convertible_to < bool >;
				{ const_graph.edges(std::declval < const typename Graph::node_type & > ()) } -> internals::optional_reference_of_group_of_group_of_edges < typename Graph::node_type, typename Graph::edge_type, Graph::can_have_multiple_edges() >;
				{ const_graph.edges(std::declval < const typename Graph::node_type & > (), std::declval < const typename Graph::node_type & > ()) } -> internals::optional_reference_of_group_of_edges < typename Graph::edge_type, Graph::can_have_multiple_edges() >;
				{ const_graph.weight(std::declval < const typename Graph::node_type & > (), std::declval < const typename Graph::node_type & > ()) } -> std::same_as < typename Graph::edge_type::weight_type >;
				{ const_graph.edge_count() } noexcept -> std::same_as < typename Graph::size_type >;
				{ const_graph.edge_count(std::declval < const typename Graph::node_type & > ()) } -> std::same_as < typename Graph::size_type >;
				{ const_graph.unique_edge_count() } -> std::same_as < typename Graph::size_type >;
				{ const_graph.unique_edge_count(std::declval < const typename Graph::node_type & > ()) } -> std::same_as < typename Graph::size_type >;
				{ const_graph.nodes() } -> internals::forward_container_of < typename Graph::node_type, true >;
				{ const_graph.closeness_centrality(std::declval < const typename Graph::node_type & > ()) } -> std::same_as < typename Graph::edge_type::weight_type >;
				{ const_graph.closeness_centrality() } -> internals::forward_container_of < std::pair < typename Graph::node_type, typename Graph::edge_type::weight_type >, false >;
				{ const_graph.node_betweenness_centrality(std::declval < const typename Graph::node_type & > ()) } -> std::same_as < typename Graph::edge_type::weight_type >;
				{ const_graph.node_betweenness_centrality() } -> internals::forward_container_of < std::pair < typename Graph::node_type, typename Graph::edge_type::weight_type >, false >;
				{ const_graph.edge_betweenness_centrality() } -> internals::forward_container_of < std::pair < std::pair < typename Graph::node_type, typename Graph::node_type >, typename Graph::edge_type::weight_type >, false >;
			};
	}

	export template < typename Type >
	concept nary_tree_node_container =
		internals::is_specialization_nary_tree_node_container_value < Type, graph::nary_tree_node_container > ::value;

	export template < typename Node >
	concept node =
		std::same_as < decltype(Node::value), typename Node::value_type > &&
		std::is_object < typename Node::value_type > ::value &&

		requires ()
		{
			{ Node { std::declval < const typename Node::value_type & > () } } -> std::same_as < Node >;
			{ Node { std::declval < typename Node::value_type && > () } } -> std::same_as < Node >;
		};

	export template < typename Node >
	concept binary_node =
		node < Node > &&
		std::unsigned_integral < typename Node::size_type > &&

		requires (Node mutable_node)
		{
			{ mutable_node.insert_left_child(std::declval < const typename Node::value_type & > ()) } -> std::convertible_to < Node & >;
			{ mutable_node.insert_right_child(std::declval < const typename Node::value_type & > ()) } -> std::convertible_to < Node & >;
			{ mutable_node.insert_left_child(std::declval < typename Node::value_type && > ()) } -> std::convertible_to < Node & >;
			{ mutable_node.insert_right_child(std::declval < typename Node::value_type && > ()) } -> std::convertible_to < Node & >;
			{ mutable_node.insert_left_child(std::declval < const Node & > ()) } -> std::convertible_to < Node & >;
			{ mutable_node.insert_right_child(std::declval < const Node & > ()) } -> std::convertible_to < Node & >;

			{ mutable_node.erase_left_child() } noexcept -> std::same_as < void >;
			{ mutable_node.erase_right_child() } noexcept -> std::same_as < void >;

			{ mutable_node.clear() } noexcept -> std::same_as < void >;

			{ mutable_node.left_child() } noexcept -> std::same_as < std::optional < std::reference_wrapper < Node > > >;
			{ mutable_node.right_child() } noexcept -> std::same_as < std::optional < std::reference_wrapper < Node > > >;

			{ mutable_node.extract_left_child() } noexcept -> internals::pointer;
			{ mutable_node.extract_right_child() } noexcept -> internals::pointer;

			{ mutable_node.swap(std::declval < Node & > ()) } -> std::same_as < void >;
		} &&

		requires (const Node const_node)
		{
			{ const_node.empty() } noexcept -> std::convertible_to < bool >;

			{ const_node.size() } noexcept -> std::same_as < typename Node::size_type >;
			{ const_node.valid_size() } noexcept -> std::same_as < typename Node::size_type >;

			{ const_node.has_left_child() } noexcept -> std::convertible_to < bool >;
			{ const_node.has_right_child() } noexcept -> std::convertible_to < bool >;

			{ const_node.left_child() } noexcept -> std::same_as < std::optional < std::reference_wrapper < const Node > > >;
			{ const_node.right_child() } noexcept -> std::same_as < std::optional < std::reference_wrapper < const Node > > >;
		};

	export template < typename Node >
	concept binary_node_parent =
		node < Node > &&

		requires (Node mutable_node)
		{
			{ mutable_node.insert_left_child(std::declval < Node && > ()) } -> std::convertible_to < Node & >;
			{ mutable_node.insert_right_child(std::declval < Node && > ()) } -> std::convertible_to < Node & >;

			{ mutable_node.parent() } noexcept -> std::same_as < std::optional < std::reference_wrapper < Node > > >;
			{ mutable_node.parent_and_my_position() } noexcept -> std::same_as < std::optional < std::pair < std::reference_wrapper < Node >, xablau::graph::binary_node_side > > >;

			{ mutable_node.extract_myself() } noexcept -> internals::optional_of_pair_of_pointer_and_binary_side;
		} &&

		requires (const Node const_node)
		{
			{ const_node.has_parent() } noexcept -> std::convertible_to < bool >;

			{ const_node.parent() } noexcept -> std::same_as < std::optional < std::reference_wrapper < const Node > > >;
			{ const_node.parent_and_my_position() } noexcept -> std::same_as < std::optional < std::pair < std::reference_wrapper < const Node >, xablau::graph::binary_node_side > > >;
		};

	export template < typename Node >
	concept binary_node_with_parent = binary_node < Node > && binary_node_parent < Node >;

	export template < typename Node >
	concept nary_node =
		node < Node > &&
		std::unsigned_integral < typename Node::size_type > &&

		requires (Node mutable_node)
		{
			{ mutable_node.insert_child(std::declval < const typename Node::value_type & > (), typename Node::size_type{}) } -> std::convertible_to < Node & >;
			{ mutable_node.insert_back_child(std::declval < const typename Node::value_type & > ()) } -> std::convertible_to < Node & >;
			{ mutable_node.insert_front_child(std::declval < const typename Node::value_type & > ()) } -> std::convertible_to < Node & >;

			{ mutable_node.insert_child(std::declval < typename Node::value_type && > (), typename Node::size_type{}) } -> std::convertible_to < Node & >;
			{ mutable_node.insert_back_child(std::declval < typename Node::value_type && > ()) } -> std::convertible_to < Node & >;
			{ mutable_node.insert_front_child(std::declval < typename Node::value_type && > ()) } -> std::convertible_to < Node & >;
	
			{ mutable_node.insert_child(std::declval < const Node & > (), typename Node::size_type{}) } -> std::convertible_to < Node & >;
			{ mutable_node.insert_back_child(std::declval < const Node & > ()) } -> std::convertible_to < Node & >;
			{ mutable_node.insert_front_child(std::declval < const Node & > ()) } -> std::convertible_to < Node & >;

			{ mutable_node.invalidate_first_valid_child() } -> std::same_as < void >;
			{ mutable_node.invalidate_last_valid_child() } -> std::same_as < void >;
			{ mutable_node.invalidate_child(typename Node::size_type{}) } -> std::same_as < void >;

			{ mutable_node.erase_back_child() } -> std::same_as < void >;
			{ mutable_node.erase_front_child() } -> std::same_as < void >;
			{ mutable_node.erase_child(typename Node::size_type{}) } -> std::same_as < void >;

			{ mutable_node.valid_back_child() } noexcept -> std::same_as < std::optional < std::reference_wrapper < Node > > >;
			{ mutable_node.valid_front_child() } noexcept -> std::same_as < std::optional < std::reference_wrapper < Node > > >;
			{ mutable_node.valid_children() } -> internals::bidirectional_range_of_reference_of < Node >;

			{ mutable_node.child(typename Node::size_type{}) } -> std::same_as < std::optional < std::reference_wrapper < Node > > >;
			{ mutable_node.children() } -> internals::bidirectional_range_of_optional_of_reference_of < Node >;

			{ mutable_node.extract_and_invalidate_child(typename Node::size_type{}) } -> internals::pointer;
			{ mutable_node.extract_and_erase_child(typename Node::size_type{}) } -> internals::pointer;
			{ mutable_node.extract_and_invalidate_back_child() } -> internals::pointer;
			{ mutable_node.extract_and_erase_back_child() } -> internals::pointer;
			{ mutable_node.extract_and_invalidate_front_child() } -> internals::pointer;
			{ mutable_node.extract_and_erase_front_child() } -> internals::pointer;

			{ mutable_node.clear() } noexcept -> std::same_as < void >;

			{ mutable_node.swap(std::declval < Node & > ()) } -> std::same_as < void >;
		} &&

		requires (const Node const_node)
		{
			{ const_node.empty() } noexcept -> std::convertible_to < bool >;

			{ const_node.size() } noexcept -> std::same_as < typename Node::size_type >;
			{ const_node.valid_size() } noexcept -> std::same_as < typename Node::size_type >;

			{ const_node.has_child(typename Node::size_type{}) } noexcept -> std::convertible_to < bool >;
			{ const_node.has_valid_child(typename Node::size_type{}) } noexcept -> std::convertible_to < bool >;

			{ const_node.valid_back_child() } noexcept -> std::same_as < std::optional < std::reference_wrapper < const Node > > >;
			{ const_node.valid_front_child() } noexcept -> std::same_as < std::optional < std::reference_wrapper < const Node > > >;
			{ const_node.valid_children() } -> internals::bidirectional_range_of_reference_of < const Node >;

			{ const_node.child(typename Node::size_type{}) } -> std::same_as < std::optional < std::reference_wrapper < const Node > > >;
			{ const_node.children() } -> internals::bidirectional_range_of_optional_of_reference_of < const Node >;

			{ const_node.back_child() } -> std::same_as < std::optional < std::reference_wrapper < const Node > > >;
			{ const_node.front_child() } -> std::same_as < std::optional < std::reference_wrapper < const Node > > >;
		};

	export template < typename Node >
	concept nary_node_parent =
		node < Node > &&
		std::unsigned_integral < typename Node::size_type > &&

		requires (Node mutable_node)
		{
			{ mutable_node.insert_child(std::declval < Node && > (), size_t{}) } -> std::convertible_to < Node & >;
			{ mutable_node.insert_back_child(std::declval < Node && > ()) } -> std::convertible_to < Node & >;
			{ mutable_node.insert_front_child(std::declval < Node && > ()) } -> std::convertible_to < Node & >;

			{ mutable_node.parent() } noexcept -> std::same_as < std::optional < std::reference_wrapper < Node > > >;

			{ mutable_node.left_valid_sibling() } -> std::same_as < std::optional < std::reference_wrapper < Node > > >;
			{ mutable_node.right_valid_sibling() } -> std::same_as < std::optional < std::reference_wrapper < Node > > >;

			{ mutable_node.parent_and_my_valid_position() } -> std::same_as < std::optional < std::pair < std::reference_wrapper < Node >, size_t > > >;
			{ mutable_node.parent_and_my_position() } -> std::same_as < std::optional < std::pair < std::reference_wrapper < Node >, size_t > > >;

			{ mutable_node.extract_and_invalidate_myself_on_parent() } -> internals::optional_of_pair_of_pointer_and_size_type < typename Node::size_type >;
			{ mutable_node.extract_and_erase_myself_on_parent() } -> internals::optional_of_pair_of_pointer_and_size_type < typename Node::size_type >;
		} &&

		requires (const Node const_node)
		{
			{ const_node.has_parent() } noexcept -> std::convertible_to < bool >;

			{ const_node.parent() } noexcept -> std::same_as < std::optional < std::reference_wrapper < const Node > > >;

			{ const_node.left_valid_sibling() } -> std::same_as < std::optional < std::reference_wrapper < const Node > > >;
			{ const_node.right_valid_sibling() } -> std::same_as < std::optional < std::reference_wrapper < const Node > > >;

			{ const_node.parent_and_my_valid_position() } -> std::same_as < std::optional < std::pair < std::reference_wrapper < const Node >, size_t > > >;
			{ const_node.parent_and_my_position() } -> std::same_as < std::optional < std::pair < std::reference_wrapper < const Node >, size_t > > >;
		};

	export template < typename Node >
	concept nary_node_with_parent = nary_node < Node > && nary_node_parent < Node >;

	export template < typename Tree >
	concept tree =
		std::ranges::bidirectional_range < Tree > &&
		node < typename Tree::node_type > &&
		std::unsigned_integral < typename Tree::size_type > &&

		requires (Tree mutable_tree)
		{
			{ mutable_tree.begin() } noexcept -> std::same_as < typename Tree::iterator >;
			{ mutable_tree.end() } noexcept -> std::same_as < typename Tree::iterator >;
			{ mutable_tree.rbegin() } noexcept -> std::same_as < typename Tree::reverse_iterator >;
			{ mutable_tree.rend() } noexcept -> std::same_as < typename Tree::reverse_iterator >;

			{ mutable_tree.swap(std::declval < Tree & > ()) } noexcept -> std::same_as < void >;

			{ mutable_tree.insert_root(std::declval < const typename Tree::value_type & > ()) } -> std::same_as < typename Tree::iterator >;
			{ mutable_tree.insert_root(std::declval < typename Tree::value_type && > ()) } -> std::same_as < typename Tree::iterator >;

			{ mutable_tree.find(std::declval < const typename Tree::value_type & > ()) } -> std::same_as < typename Tree::iterator >;
		} &&

		requires (const Tree const_tree)
		{
			{ const_tree.begin() } noexcept -> std::same_as < typename Tree::const_iterator >;
			{ const_tree.cbegin() } noexcept -> std::same_as < typename Tree::const_iterator >;
			{ const_tree.end() } noexcept -> std::same_as < typename Tree::const_iterator >;
			{ const_tree.cend() } noexcept -> std::same_as < typename Tree::const_iterator >;
			{ const_tree.rbegin() } noexcept -> std::same_as < typename Tree::const_reverse_iterator >;
			{ const_tree.crbegin() } noexcept -> std::same_as < typename Tree::const_reverse_iterator >;
			{ const_tree.rend() } noexcept -> std::same_as < typename Tree::const_reverse_iterator >;
			{ const_tree.crend() } noexcept -> std::same_as < typename Tree::const_reverse_iterator >;

			{ const_tree.empty() } noexcept -> std::convertible_to < bool >;
			{ const_tree.binary() } -> std::convertible_to < bool >;
			{ const_tree.size() } noexcept -> std::same_as < typename Tree::size_type >;
			{ const_tree.count(std::declval < const typename Tree::value_type & > ()) } -> std::same_as < typename Tree::size_type >;
			{ const_tree.contains(std::declval < const typename Tree::value_type & > ()) } -> std::convertible_to < bool >;
			{ const_tree.find(std::declval < const typename Tree::value_type & > ()) } -> std::same_as < typename Tree::const_iterator >;
			{ const_tree.leaf_nodes() } -> internals::forward_container_of_reference_wrapper_of < const typename Tree::node_type >;
			{ const_tree.nodes() } -> internals::forward_container_of_reference_wrapper_of < const typename Tree::node_type >;
			{ const_tree.levels() } noexcept -> std::same_as < typename Tree::size_type >;
			{ const_tree.nodes_per_level() } -> internals::forward_container_of < typename Tree::size_type, true >;
		};

	export template < typename Tree >
	concept binary_tree =
		tree < Tree > &&
		binary_node < typename Tree::node_type > &&

		requires (Tree mutable_tree)
		{
			{ mutable_tree.insert_left_child(typename Tree::const_iterator{}, std::declval < const typename Tree::value_type & > ()) } -> std::same_as < typename Tree::iterator >;
			{ mutable_tree.insert_left_child(typename Tree::const_iterator{}, std::declval < typename Tree::value_type && > ()) } -> std::same_as < typename Tree::iterator >;
			{ mutable_tree.insert_right_child(typename Tree::const_iterator{}, std::declval < const typename Tree::value_type & > ()) } -> std::same_as < typename Tree::iterator >;
			{ mutable_tree.insert_right_child(typename Tree::const_iterator{}, std::declval < typename Tree::value_type && > ()) } -> std::same_as < typename Tree::iterator >;

			{ mutable_tree.find_with_path(std::declval < const typename Tree::value_type & > ()) } -> internals::pair_of_iterator_and_forward_container_of < typename Tree::iterator, xablau::graph::binary_node_side >;
		} &&

		requires (const Tree const_tree)
		{
			{ const_tree.find_with_path(std::declval < const typename Tree::value_type & > ()) } -> internals::pair_of_iterator_and_forward_container_of < typename Tree::const_iterator, xablau::graph::binary_node_side >;
		};

	export template < typename Tree >
	concept nary_tree =
		tree < Tree > &&
		nary_node < typename Tree::node_type > &&

		requires (Tree mutable_tree)
		{
			{ mutable_tree.insert_or_replace_child(typename Tree::const_iterator{}, std::declval < const typename Tree::value_type & > (), typename Tree::size_type{}) } -> std::same_as < typename Tree::iterator >;
			{ mutable_tree.insert_or_replace_child(typename Tree::const_iterator{}, std::declval < typename Tree::value_type && > (), typename Tree::size_type{}) } -> std::same_as < typename Tree::iterator >;

			{ mutable_tree.insert_child(typename Tree::const_iterator{}, std::declval < const typename Tree::value_type & > (), typename Tree::size_type{}) } -> std::same_as < typename Tree::iterator >;
			{ mutable_tree.insert_child(typename Tree::const_iterator{}, std::declval < typename Tree::value_type && > (), typename Tree::size_type{}) } -> std::same_as < typename Tree::iterator >;
			{ mutable_tree.insert_back_child(typename Tree::const_iterator {}, std::declval < const typename Tree::value_type & >()) } -> std::same_as < typename Tree::iterator >;
			{ mutable_tree.insert_back_child(typename Tree::const_iterator{}, std::declval < typename Tree::value_type && > ()) } -> std::same_as < typename Tree::iterator >;
			{ mutable_tree.insert_front_child(typename Tree::const_iterator{}, std::declval < const typename Tree::value_type & > ()) } -> std::same_as < typename Tree::iterator >;
			{ mutable_tree.insert_front_child(typename Tree::const_iterator{}, std::declval < typename Tree::value_type && > ()) } -> std::same_as < typename Tree::iterator >;

			{ mutable_tree.find_with_path(std::declval < const typename Tree::value_type & > ()) } -> internals::pair_of_iterator_and_forward_container_of < typename Tree::iterator, typename Tree::size_type >;
		} &&

		requires (const Tree const_tree)
		{
			{ const_tree.find_with_path(std::declval < const typename Tree::value_type & > ()) } -> internals::pair_of_iterator_and_forward_container_of < typename Tree::const_iterator, typename Tree::size_type >;
		};

	export template < typename Type >
	concept graph_container_type = internals::is_specialization_graph_container_type_value < Type, graph::graph_container_type > ::value;

	export template < typename Edge >
	concept edge =
		std::regular < Edge > &&
		std::same_as < decltype(Edge::weight), typename Edge::weight_type > &&
		xablau::algebra::concepts::basic_assignable_arithmetic < typename Edge::weight_type > &&

		requires ()
		{
			{ Edge { typename Edge::weight_type{} } } noexcept -> std::same_as < Edge >;
		};

	template < typename NodeType, typename ContainerType >
	concept graph_requirements =
		((ContainerType::type() == graph_container_type_value::ordered || ContainerType::type() == graph_container_type_value::multi_ordered) ?
		internals::has_less_than < NodeType > :
		internals::has_hash_and_equality_comparison < NodeType >);

	export template < typename Graph >
	concept graph =
		internals::common_graph < Graph > &&

		requires (const Graph const_graph)
		{
			{ const_graph.triangle_count() } -> std::same_as < typename Graph::size_type >;
			{ const_graph.triplet_count() } -> std::same_as < typename Graph::size_type >;
			{ const_graph.clustering_coefficient(std::declval < const typename Graph::node_type & > ()) } -> std::same_as < typename Graph::edge_type::weight_type >;
			{ const_graph.clustering_coefficient() } -> std::same_as < typename Graph::edge_type::weight_type >;
			{ const_graph.average_clustering_coefficient() } -> std::same_as < typename Graph::edge_type::weight_type >;
		};

	export template < typename Graph >
	concept digraph =
		internals::common_graph < Graph > &&

		requires (const Graph const_graph)
		{
			{ const_graph.source_nodes() } -> internals::forward_container_of < typename Graph::node_type, true >;
			{ const_graph.sink_nodes() } -> internals::forward_container_of < typename Graph::node_type, true >;
		};
}