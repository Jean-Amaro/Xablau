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

export module xablau.geometry:voxel_space;
export import :forward_declarations;

export import <cmath>;
export import <map>;
export import <ostream>;
export import <set>;
export import <unordered_map>;
export import <unordered_set>;

export import xablau.algebra;

namespace xablau::geometry
{
	template < xablau::algebra::concepts::standard_arithmetic Type >
	using voxel_space_spacing_type =
		typename std::conditional <
			xablau::algebra::concepts::standard_integral < Type >,
			typename std::conditional <
				xablau::algebra::concepts::standard_signed < Type >,
				intmax_t,
				uintmax_t > ::type,
			Type > ::type;

	export template <
		xablau::algebra::concepts::standard_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality,
		concepts::voxel_space_container_type ContainerType >
	requires (SpatialDimensionality::dimensionality() > 0)
	class voxel_space final
	{
	public:
		using value_type = Type;

		using size_type = size_t;

		using voxel_type = voxel < Type, SpatialDimensionality >;

		using vertex_type = vertex < Type, SpatialDimensionality >;

		using spacing_type = voxel_space_spacing_type < Type >;

		using voxel_space_type =
			typename std::conditional <
				ContainerType::type() == voxel_space_container_type_value::ordered,
				std::map < voxel_type, std::set < vertex_type > >,
				std::unordered_map < voxel_type, std::unordered_set < vertex_type > > > ::type;

	private:
		spacing_type _spacing{};

		voxel_space_type _voxel_space{};

		static void validate_spacing(const spacing_type spacing)
		{
			if constexpr (xablau::algebra::concepts::standard_integral < Type >)
			{
				if (spacing <= spacing_type{})
				{
					throw std::domain_error("\"spacing\" is not positive.");
				}
			}

			else
			{
				if (!std::isnormal(spacing) || spacing < spacing_type{})
				{
					throw std::domain_error("\"spacing\" is not normal or not positive.");
				}
			}
		}

	public:
		auto insert(const vertex_type &vertex)
		{
			auto key = this->make_voxel(vertex);
			auto inserted = this->_voxel_space[key].insert(vertex).second;

			return std::make_pair(std::move(key), std::move(inserted));
		}

		auto insert(vertex_type &&vertex)
		{
			auto key = this->make_voxel(vertex);
			auto inserted = this->_voxel_space[key].insert(std::move(vertex)).second;

			return std::make_pair(std::move(key), std::move(inserted));
		}

		bool erase_voxel(const voxel_type &voxel)
		{
			const auto iterator = this->_voxel_space.find(voxel);

			if (iterator != this->_voxel_space.end())
			{
				this->_voxel_space.erase(iterator);

				return true;
			}

			return false;
		}

		bool erase_vertex(const vertex_type &vertex)
		{
			const auto voxel = this->make_voxel(vertex);
			const auto iterator1 = this->_voxel_space.find(voxel);

			if (iterator1 != this->_voxel_space.end())
			{
				auto &vertices = iterator1->second;

				const auto iterator2 = vertices.find(vertex);

				if (iterator2 != vertices.end())
				{
					if (vertices.size() == 1)
					{
						this->_voxel_space.erase(voxel);
					}

					else
					{
						vertices.erase(iterator2);
					}

					return true;
				}
			}

			return false;
		}

		void spacing(const spacing_type spacing)
		{
			if (!this->_voxel_space.empty())
			{
				throw std::runtime_error("There are already voxels in the space.");
			}

			voxel_space::validate_spacing(spacing);

			this->_spacing = spacing;
		}

		[[nodiscard]] spacing_type spacing() const noexcept
		{
			return this->_spacing;
		}

		[[nodiscard]] auto make_voxel(const vertex_type &vertex) const
		{
			voxel_type voxelCoordinates{};

			for (size_t i = 0; i < SpatialDimensionality::dimensionality(); i++)
			{
				if constexpr (xablau::algebra::concepts::standard_integral < Type >)
				{
					voxelCoordinates[i] = vertex[i] / this->_spacing;
				}

				else
				{
					const auto coordinate = std::floor(vertex[i] / this->_spacing);

					if (!std::isfinite(coordinate) ||
						coordinate < std::numeric_limits < intmax_t > ::min() ||
						coordinate > std::numeric_limits < intmax_t > ::max())
					{
						throw std::domain_error("\"vertex\"'s coordinates are not valid.");
					}

					voxelCoordinates[i] = static_cast < intmax_t > (coordinate);
				}
			}

			return voxelCoordinates;
		}

		[[nodiscard]] const auto &space() const noexcept
		{
			return this->_voxel_space;
		}

		[[nodiscard]] bool contains(const voxel_type &voxel) const
		{
			return this->_voxel_space.contains(voxel);
		}

		[[nodiscard]] const auto &vertices_of(const voxel_type &voxel) const
		{
			return this->_voxel_space.at(voxel);
		}

		void clear() noexcept
		{
			this->_voxel_space.clear();
		}

		[[nodiscard]] static consteval size_type dimensionality() noexcept
		{
			return SpatialDimensionality::dimensionality();
		}

		friend std::ostream &operator<<(std::ostream &stream, const voxel_space &voxelSpace)
		{
			size_type counter1{};
			const auto voxelCount = voxelSpace._voxel_space.size();

			stream << "|_| ( " << voxelSpace._spacing << ";\n";

			for (const auto &voxel : voxelSpace._voxel_space)
			{
				size_type counter2{};
				constexpr auto keyCount = SpatialDimensionality::dimensionality();

				stream << "\t( ";

				for (const auto key : voxel.first)
				{
					stream << key;

					counter2++;

					if (counter2 != keyCount)
					{
						stream << ", ";
					}
				}

				stream << " ): ( ";

				counter2 = 0;
				const auto vertexCount = voxel.second.size();

				for (const auto &vertice : voxel.second)
				{
					stream << vertice;

					counter2++;

					if (counter2 != vertexCount)
					{
						stream << ", ";
					}
				}

				stream << " )";

				counter1++;

				if (counter1 != voxelCount)
				{
					stream << "\n";
				}
			}

			stream << " )";

			return stream;
		}

		explicit voxel_space(const spacing_type spacing = spacing_type{1}) :
			_spacing(spacing)
		{
			voxel_space::validate_spacing(spacing);
		}

		voxel_space &operator=(const voxel_space &) = default;

		voxel_space &operator=(voxel_space &&) noexcept = default;

		voxel_space(const voxel_space &) = default;

		voxel_space(voxel_space &&) noexcept = default;
	};

	export template <
		xablau::algebra::concepts::standard_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality >
	[[nodiscard]] auto make_voxel(
		const vertex < Type, SpatialDimensionality > &vertex,
		const voxel_space_spacing_type < Type > spacing)
	{
		if constexpr (std::floating_point < Type >)
		{
			if (!std::isnormal(spacing) || spacing < voxel_space_spacing_type < Type > {})
			{
				throw std::domain_error("\"spacing\" is not normal or not positive.");
			}
		}

		else
		{
			if (spacing <= voxel_space_spacing_type < Type > {})
			{
				throw std::domain_error("\"spacing\" is not positive.");
			}
		}

		voxel < Type, SpatialDimensionality > voxelCoordinates{};

		for (size_t i = 0; i < SpatialDimensionality::dimensionality(); i++)
		{
			if constexpr (xablau::algebra::concepts::standard_integral < Type >)
			{
				voxelCoordinates[i] = vertex[i] / spacing;
			}

			else
			{
				const auto coordinate = std::floor(vertex[i] / spacing);

				if (!std::isfinite(coordinate) ||
					coordinate < std::numeric_limits < intmax_t > ::min() ||
					coordinate > std::numeric_limits < intmax_t > ::max())
				{
					throw std::domain_error("\"vertex\"'s coordinates are not valid.");
				}

				voxelCoordinates[i] = static_cast < intmax_t > (coordinate);
			}
		}

		return voxelCoordinates;
	}
}