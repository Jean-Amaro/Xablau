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

export module xablau.geometry:fundamental_concepts;
export import :fundamental_configurations;
export import :vector_configurations;

export import std;

export import xablau.algebra;
import :voxel_space_configurations;

namespace xablau::geometry::concepts
{
	namespace internals
	{
		template < typename Type, template < size_t > typename Unspecialized >
		struct is_specialization_size_t : std::false_type{};

		template < template < size_t > typename Unspecialized, size_t Arg >
		struct is_specialization_size_t < Unspecialized < Arg >, Unspecialized > : std::true_type{};

		template < typename Type, template < geometry::voxel_space_container_type_value > typename Unspecialized >
		struct is_specialization_voxel_space_container_type_value : std::false_type {};

		template < template < geometry::voxel_space_container_type_value > typename Unspecialized, geometry::voxel_space_container_type_value Arg >
		struct is_specialization_voxel_space_container_type_value < Unspecialized < Arg >, Unspecialized > : std::true_type {};
	}

	export template < typename SpatialDimension >
	concept spatial_dimension = internals::is_specialization_size_t < SpatialDimension, geometry::spatial_dimension > ::value;

	export template < typename Type >
	concept geometric_assignable_arithmetic =
		xablau::algebra::concepts::basic_assignable_arithmetic < Type > &&
		std::equality_comparable < Type > &&

		xablau::algebra::concepts::basic_assignable_arithmetic < typename xablau::algebra::abs_type < Type > ::type > &&
		std::totally_ordered < typename xablau::algebra::abs_type < Type > ::type >;

	export template < typename Type >
	concept voxel_space_container_type =
		internals::is_specialization_voxel_space_container_type_value < Type, geometry::voxel_space_container_type > ::value;

	template < typename SpatialDimension, typename ElementDimension >
	concept valid_space_element_relation =
		internals::is_specialization_size_t < SpatialDimension, geometry::spatial_dimension > ::value &&
		internals::is_specialization_size_t < ElementDimension, geometry::spatial_dimension > ::value &&
		(SpatialDimension::dimensionality() >= ElementDimension::dimensionality());
}