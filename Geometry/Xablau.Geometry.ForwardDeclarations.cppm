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

export module xablau.geometry:forward_declarations;
export import :euclidean_triangle_configurations;
export import :fundamental_concepts;
export import :hyperplane_configurations;
export import :hypersphere_configurations;
export import :vector_configurations;
export import :voxel_space_configurations;

export import std;

export import xablau.algebra;

export namespace xablau::geometry
{
	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality >
	class euclidean_triangle;

	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality >
	class hyperplane;

	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality >
	class hypersphere;

	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality >
	class line;

	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality >
	class polytope;

	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality >
	class segment;

	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality >
	class vector;

	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality >
	class vertex;

	template <
		xablau::algebra::concepts::standard_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality >
	using voxel =
		vertex <
			typename std::conditional <
				xablau::algebra::concepts::standard_signed < Type >,
				std::intmax_t,
				std::uintmax_t > ::type,
			SpatialDimensionality >;

	template <
		xablau::algebra::concepts::standard_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality,
		concepts::voxel_space_container_type ContainerType >
	requires (SpatialDimensionality::dimensionality() > 0)
	class voxel_space;
}