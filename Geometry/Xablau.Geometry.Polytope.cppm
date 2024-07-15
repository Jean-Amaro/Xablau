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

export module xablau.geometry:polytope;
export import :forward_declarations;

export import std;

export import xablau.algebra;

namespace xablau::geometry
{
	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality >
	class polytope final
	{
	public:
		using value_type = Type;

		using abs_type = typename xablau::algebra::abs_type < Type > ::type;

		using size_type = typename std::array < Type, SpatialDimensionality::dimensionality() > ::size_type;

		std::vector < vertex < Type, SpatialDimensionality > > vertices;

		std::vector < std::array < size_type, SpatialDimensionality::dimensionality() > > facets;

		[[nodiscard]] static consteval size_type dimensionality() noexcept
		{
			return SpatialDimensionality::dimensionality();
		}

		constexpr polytope &operator=(const polytope &) = default;

		constexpr polytope &operator=(polytope &&) noexcept = default;

		constexpr polytope(const polytope &) = default;

		constexpr polytope(polytope &&) noexcept = default;
	};
}