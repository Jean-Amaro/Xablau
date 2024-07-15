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

export module xablau.geometry:hyperplane;
export import :forward_declarations;

export import std;

export import xablau.algebra;

export namespace xablau::geometry
{
	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality >
	class hyperplane final
	{
	public:
		using value_type = Type;

		using abs_type = typename xablau::algebra::abs_type < Type > ::type;

		using size_type = typename std::array < Type, SpatialDimensionality::dimensionality() > ::size_type;

	private:
		template < concepts::spatial_dimension OtherSpatialDimensionality >
		static void check_degeneracy(const hyperplane < Type, OtherSpatialDimensionality > &hyperplane)
		{
			if (hyperplane.degenerate())
			{
				throw std::domain_error("\"hyperplane\" is degenerate.");
			}
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		static void check_degeneracy(const line < Type, OtherSpatialDimensionality > &line)
		{
			if (line.degenerate())
			{
				throw std::domain_error("\"line\" is degenerate.");
			}
		}

		template < typename HyperplaneType >
		bool equal_to(const HyperplaneType &hyperplane) const
		{
			return
				this->normal_vector.codirectional_to(hyperplane.normal_vector, 0) &&
				(this->displacement_vertex - hyperplane.displacement_vertex).perpendicular_to(this->normal_vector, 0);
		}

		template < typename LineType >
		auto intersection_with_line(const LineType &line) const
		{
			const auto scalarProductHyperplane =
				this->normal_vector.scalar_product(this->displacement_vertex.vector_from_origin());

			const auto scalarProductLine =
				this->normal_vector.scalar_product(line.vertex.vector_from_origin());

			const auto lambda =
				(scalarProductHyperplane - scalarProductLine) / this->normal_vector.scalar_product(line.vector);

			return line.vertex + (lambda * line.vector);
		}

	public:
		vertex < Type, SpatialDimensionality > displacement_vertex;

		vector < Type, SpatialDimensionality > normal_vector;

		[[nodiscard]] explicit constexpr operator bool() const
		{
			return static_cast < bool > (this->normal_vector);
		}

		[[nodiscard]] constexpr bool degenerate() const
		{
			return this->normal_vector.null();
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool operator<(const hyperplane < Type, OtherSpatialDimensionality > &hyperplane) const
		{
			const bool thisDegenerate = this->degenerate();
			const bool otherDegenerate = hyperplane.degenerate();

			if (thisDegenerate && !otherDegenerate)
			{
				return true;
			}

			if (!thisDegenerate && otherDegenerate)
			{
				return false;
			}

			if (thisDegenerate || otherDegenerate)
			{
				return this->displacement_vertex < hyperplane.displacement_vertex;
			}

			if (this->equal_to(hyperplane))
			{
				return false;
			}

			if (this->normal_vector < hyperplane.normal_vector)
			{
				return true;
			}

			if (!(hyperplane.normal_vector < this->normal_vector))
			{
				return this->displacement_vertex < hyperplane.displacement_vertex;
			}

			return false;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool operator<=(const hyperplane < Type, OtherSpatialDimensionality > &hyperplane) const
		{
			const bool thisDegenerate = this->degenerate();
			const bool otherDegenerate = hyperplane.degenerate();

			if (thisDegenerate && !otherDegenerate)
			{
				return true;
			}

			if (!thisDegenerate && otherDegenerate)
			{
				return false;
			}

			if (thisDegenerate || otherDegenerate)
			{
				return this->displacement_vertex <= hyperplane.displacement_vertex;
			}

			if (this->equal_to(hyperplane))
			{
				return true;
			}

			if (!(hyperplane.normal_vector <= this->normal_vector))
			{
				return true;
			}

			if (this->normal_vector <= hyperplane.normal_vector)
			{
				return this->displacement_vertex <= hyperplane.displacement_vertex;
			}

			return false;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool operator==(const hyperplane < Type, OtherSpatialDimensionality > &hyperplane) const
		{
			const bool thisDegenerate = this->degenerate();
			const bool otherDegenerate = hyperplane.degenerate();

			if (thisDegenerate ^ otherDegenerate)
			{
				return false;
			}

			if (thisDegenerate || otherDegenerate)
			{
				return this->displacement_vertex == hyperplane.displacement_vertex;
			}

			return this->equal_to(hyperplane);
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool operator>=(const hyperplane < Type, OtherSpatialDimensionality > &hyperplane) const
		{
			const bool thisDegenerate = this->degenerate();
			const bool otherDegenerate = hyperplane.degenerate();

			if (thisDegenerate && !otherDegenerate)
			{
				return false;
			}

			if (!thisDegenerate && otherDegenerate)
			{
				return true;
			}

			if (thisDegenerate || otherDegenerate)
			{
				return this->displacement_vertex >= hyperplane.displacement_vertex;
			}

			if (this->equal_to(hyperplane))
			{
				return true;
			}

			if (!(hyperplane.normal_vector >= this->normal_vector))
			{
				return true;
			}

			if (this->normal_vector >= hyperplane.normal_vector)
			{
				return this->displacement_vertex >= hyperplane.displacement_vertex;
			}

			return false;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool operator>(const hyperplane < Type, OtherSpatialDimensionality > &hyperplane) const
		{
			const bool thisDegenerate = this->degenerate();
			const bool otherDegenerate = hyperplane.degenerate();

			if (thisDegenerate && !otherDegenerate)
			{
				return false;
			}

			if (!thisDegenerate && otherDegenerate)
			{
				return true;
			}

			if (thisDegenerate || otherDegenerate)
			{
				return this->displacement_vertex > hyperplane.displacement_vertex;
			}

			if (this->equal_to(hyperplane))
			{
				return false;
			}

			if (this->normal_vector > hyperplane.normal_vector)
			{
				return true;
			}

			if (!(hyperplane.normal_vector > this->normal_vector))
			{
				return this->displacement_vertex > hyperplane.displacement_vertex;
			}

			return false;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool operator!=(const hyperplane < Type, OtherSpatialDimensionality > &hyperplane) const
		{
			const bool thisDegenerate = this->degenerate();
			const bool otherDegenerate = hyperplane.degenerate();

			if (thisDegenerate ^ otherDegenerate)
			{
				return true;
			}

			if (thisDegenerate || otherDegenerate)
			{
				return this->displacement_vertex != hyperplane.displacement_vertex;
			}

			return !this->equal_to(hyperplane);
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr auto operator+(const vector < Type, OtherSpatialDimensionality > &vector) const
		{
			using ResultSpatialDimension =
				typename greater_spatial_dimension <
					SpatialDimensionality,
					OtherSpatialDimensionality > ::type;

			return
				hyperplane < Type, ResultSpatialDimension > (
					this->displacement_vertex + vector,
					this->normal_vector);
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr auto operator-(const vector < Type, OtherSpatialDimensionality > &vector) const
		{
			using ResultSpatialDimension =
				typename greater_spatial_dimension <
					SpatialDimensionality,
					OtherSpatialDimensionality > ::type;

			return
				hyperplane < Type, ResultSpatialDimension > (
					this->displacement_vertex - vector,
					this->normal_vector);
		}

		constexpr hyperplane &operator=(const hyperplane &) = default;

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() < SpatialDimensionality::dimensionality())
		constexpr hyperplane &operator=(const hyperplane < Type, OtherSpatialDimensionality > &hyperplane)
		{
			this->displacement_vertex = hyperplane.displacement_vertex;
			this->normal_vector = hyperplane.normal_vector;

			return *this;
		}

		constexpr hyperplane &operator=(hyperplane &&) noexcept = default;

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() < SpatialDimensionality::dimensionality())
		constexpr hyperplane &operator=(hyperplane < Type, OtherSpatialDimensionality > &&hyperplane) noexcept
		{
			this->displacement_vertex = std::move(hyperplane.displacement_vertex);
			this->normal_vector = std::move(hyperplane.normal_vector);

			return *this;
		}

		[[nodiscard]] static consteval size_t dimensionality() noexcept
		{
			return SpatialDimensionality::dimensionality();
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] hyperplane_side side_of_hyperplane(
			const vertex < Type, OtherSpatialDimensionality > &vertex,
			const size_t unitsInTheLastPlace = 1) const
		{
			hyperplane::check_degeneracy(*this);

			const auto vectorBetweenVertices = vertex - this->displacement_vertex;
			const auto result =
				(vectorBetweenVertices.norm() * this->normal_vector.normalize()).
					scalar_product(vectorBetweenVertices);

			if (xablau::algebra::functions::almost_equal < abs_type > ::invoke(
					result,
					abs_type{},
					unitsInTheLastPlace))
			{
				return hyperplane_side::contained_in_the_hyperplane;
			}

			if (result > abs_type{})
			{
				return hyperplane_side::same_sense_as_normal_vector;
			}

			return hyperplane_side::opposite_sense_as_normal_vector;
		}

		constexpr vector < Type, SpatialDimensionality > &reverse_sense_of_normal_vector()
		{
			return this->normal_vector.reverse_sense_in_place();
		}

		vector < Type, SpatialDimensionality > &normalize_normal_vector()
		{
			return this->normal_vector.normalize_in_place();
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] abs_type angle_between(
			const line < Type, OtherSpatialDimensionality > &line,
			const size_t unitsInTheLastPlace = 1) const
		{
			hyperplane::check_degeneracy(*this);
			hyperplane::check_degeneracy(line);

			if (this->parallel_to(line, unitsInTheLastPlace))
			{
				throw std::domain_error("\"line\" is parallel to this hyperplane.");
			}

			constexpr auto _180Degrees = std::numbers::pi_v < abs_type >;
			constexpr auto _90Degrees = _180Degrees / abs_type{2};
			const auto angle = this->normal_vector.angle_between(line.vector);

			if (angle > _90Degrees)
			{
				return angle - _90Degrees;
			}

			return _90Degrees - angle;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] abs_type angle_between(
			const hyperplane < Type, OtherSpatialDimensionality > &hyperplane,
			const size_t unitsInTheLastPlace = 1) const
		{
			hyperplane::check_degeneracy(*this);
			hyperplane::check_degeneracy(hyperplane);

			if (this->parallel_to(hyperplane, unitsInTheLastPlace))
			{
				throw std::domain_error("Both hyperplanes are parallel.");
			}

			constexpr auto _180Degrees = std::numbers::pi_v < abs_type >;
			constexpr auto _90Degrees = _180Degrees / abs_type{2};
			const auto angle = this->normal_vector.angle_between(hyperplane.normal_vector);

			if (angle > _90Degrees)
			{
				return _180Degrees - angle;
			}

			return angle;
		}

		template <
			bool CalculateClosestVertexOnHyperplane,
			concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] auto distance_to(const vertex < Type, OtherSpatialDimensionality > &vertex) const
		{
			hyperplane::check_degeneracy(*this);

			const auto projectedVectorOnNormalVector =
				std::move((this->displacement_vertex - vertex).project_on_in_place(this->normal_vector));

			if constexpr (CalculateClosestVertexOnHyperplane)
			{
				auto closestVertexOnHyperplane = vertex + projectedVectorOnNormalVector;
				auto distance = vertex.distance_to(closestVertexOnHyperplane);

				return
					std::make_pair(
						std::move(distance),
						std::move(closestVertexOnHyperplane));
			}

			else
			{
				return vertex.distance_to(vertex + projectedVectorOnNormalVector);
			}
		}

		template < 
			bool CalculateClosestVertexOnHyperplane,
			concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] auto distance_to(
			const line < Type, OtherSpatialDimensionality > &line,
			const size_t unitsInTheLastPlace = 1) const
		{
			hyperplane::check_degeneracy(*this);
			hyperplane::check_degeneracy(line);

			if (this->parallel_to(line, unitsInTheLastPlace))
			{
				return this->distance_to < CalculateClosestVertexOnHyperplane > (line.vertex);
			}

			if constexpr (CalculateClosestVertexOnHyperplane)
			{
				return std::make_pair(abs_type{}, this->intersection_with_line(line));
			}

			else
			{
				return abs_type{};
			}
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] abs_type distance_to(
			const hyperplane < Type, OtherSpatialDimensionality > &hyperplane,
			const size_t unitsInTheLastPlace = 1) const
		{
			hyperplane::check_degeneracy(*this);
			hyperplane::check_degeneracy(hyperplane);

			if (this->normal_vector.parallel_to(hyperplane.normal_vector, unitsInTheLastPlace))
			{
				return
					((this->displacement_vertex - hyperplane.displacement_vertex).
						project_on_in_place(this->normal_vector)).norm();
			}

			return abs_type{};
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool contains(
			const vertex < Type, OtherSpatialDimensionality > &vertex,
			const size_t unitsInTheLastPlace = 1) const
		{
			hyperplane::check_degeneracy(*this);

			return
				(this->displacement_vertex - vertex).
					perpendicular_to(this->normal_vector, unitsInTheLastPlace);
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool contains(
			const line < Type, OtherSpatialDimensionality > &line,
			const size_t unitsInTheLastPlace = 1) const
		{
			hyperplane::check_degeneracy(*this);
			hyperplane::check_degeneracy(line);

			return
				this->normal_vector.perpendicular_to(line.vector, unitsInTheLastPlace) &&
				(this->displacement_vertex - line.vertex).
					perpendicular_to(this->normal_vector, unitsInTheLastPlace);
		}

		template <
			bool CalculateIntersectionVertex,
			concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] auto intersects(
			const line < Type, OtherSpatialDimensionality > &line,
			const size_t unitsInTheLastPlace = 1) const
		{
			hyperplane::check_degeneracy(*this);
			hyperplane::check_degeneracy(line);

			if (this->normal_vector.perpendicular_to(line.vector, unitsInTheLastPlace))
			{
				if (this->contains(line.vertex, unitsInTheLastPlace))
				{
					if constexpr (CalculateIntersectionVertex)
					{
						return std::make_optional(line.vertex);
					}

					else
					{
						return true;
					}
				}

				if constexpr (CalculateIntersectionVertex)
				{
					return std::optional < vertex < Type, OtherSpatialDimensionality > > {};
				}

				else
				{
					return false;
				}
			}

			if constexpr (CalculateIntersectionVertex)
			{
				return std::make_optional(this->intersection_with_line(line));
			}

			else
			{
				return true;
			}
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool intersects(
			const hyperplane < Type, OtherSpatialDimensionality > &hyperplane,
			const size_t unitsInTheLastPlace = 1) const
		{
			hyperplane::check_degeneracy(*this);
			hyperplane::check_degeneracy(hyperplane);

			return !this->parallel_to(hyperplane, unitsInTheLastPlace);
		}

		template <
			bool CalculateIntersection,
			concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() <= 3)
		[[nodiscard]] auto intersects(
			const hyperplane < Type, OtherSpatialDimensionality > &hyperplane,
			const size_t unitsInTheLastPlace = 1) const
		requires (SpatialDimensionality::dimensionality() <= 3)
		{
			if constexpr (CalculateIntersection)
			{
				using ResultType =
					std::variant <
						line < Type, spatial_dimension < 3 > >,
						std::reference_wrapper <
							const geometry::hyperplane < Type, SpatialDimensionality > > >;

				if (this->intersects(hyperplane, unitsInTheLastPlace))
				{
					auto vectorLine =
						this->normal_vector.normalize().
							vector_product < vector_product_dimension::three > (
								hyperplane.normal_vector.normalize());

					if (!xablau::algebra::functions::almost_equal < abs_type > ::invoke(
							vectorLine.norm(),
							abs_type{},
							unitsInTheLastPlace))
					{
						ResultType result =
							line < Type, spatial_dimension < 3 > > (
								this->distance_to < true > (
									hyperplane.displacement_vertex).second,
								std::move(vectorLine));

						return std::make_optional(std::move(result));
					}

					else
					{
						return std::make_optional(ResultType(*this));
					}
				}

				return std::optional < ResultType > {};
			}

			else
			{
				return this->intersects(hyperplane, unitsInTheLastPlace);
			}
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool coincides_with(
			const hyperplane < Type, OtherSpatialDimensionality > &hyperplane,
			const size_t unitsInTheLastPlace = 1) const
		{
			hyperplane::check_degeneracy(*this);
			hyperplane::check_degeneracy(hyperplane);

			return
				this->normal_vector.parallel_to(hyperplane.normal_vector, unitsInTheLastPlace) &&
				(this->displacement_vertex - hyperplane.displacement_vertex).
					perpendicular_to(this->normal_vector, unitsInTheLastPlace);
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool coincides_with_codirectional_normal_vector(
			const hyperplane < Type, OtherSpatialDimensionality > &hyperplane,
			const size_t unitsInTheLastPlace = 1) const
		{
			hyperplane::check_degeneracy(*this);
			hyperplane::check_degeneracy(hyperplane);

			return
				this->normal_vector.codirectional_to(hyperplane.normal_vector, unitsInTheLastPlace) &&
				(this->displacement_vertex - hyperplane.displacement_vertex).
					perpendicular_to(this->normal_vector, unitsInTheLastPlace);
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool perpendicular_to(
			const line < Type, OtherSpatialDimensionality > &line,
			const size_t unitsInTheLastPlace = 1) const
		{
			hyperplane::check_degeneracy(*this);
			hyperplane::check_degeneracy(line);

			return
				this->normal_vector.parallel_to(
					line.vector,
					unitsInTheLastPlace);
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool perpendicular_to(
			const hyperplane < Type, OtherSpatialDimensionality > &hyperplane,
			const size_t unitsInTheLastPlace = 1) const
		{
			hyperplane::check_degeneracy(*this);
			hyperplane::check_degeneracy(hyperplane);

			return
				this->normal_vector.perpendicular_to(
					hyperplane.normal_vector,
					unitsInTheLastPlace);
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool parallel_to(
			const line < Type, OtherSpatialDimensionality > &line,
			const size_t unitsInTheLastPlace = 1) const
		{
			hyperplane::check_degeneracy(*this);
			hyperplane::check_degeneracy(line);

			return
				this->normal_vector.perpendicular_to(line.vector, unitsInTheLastPlace) &&
				!((this->displacement_vertex - line.vertex).
					perpendicular_to(this->normal_vector, unitsInTheLastPlace));
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool parallel_to(
			const hyperplane < Type, OtherSpatialDimensionality > &hyperplane,
			const size_t unitsInTheLastPlace = 1) const
		{
			hyperplane::check_degeneracy(*this);
			hyperplane::check_degeneracy(hyperplane);

			return
				this->normal_vector.parallel_to(hyperplane.normal_vector, unitsInTheLastPlace) &&
				!((this->displacement_vertex - hyperplane.displacement_vertex).
					perpendicular_to(this->normal_vector, unitsInTheLastPlace));
		}

		friend std::ostream &operator<<(std::ostream &stream, const hyperplane &hyperplane)
		{
			stream << "/_/ ( " << hyperplane.displacement_vertex << ", " << hyperplane.normal_vector << " )";

			return stream;
		}

		constexpr hyperplane() noexcept = default;

		constexpr hyperplane(const hyperplane &) = default;

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() < SpatialDimensionality::dimensionality())
		constexpr hyperplane(const hyperplane < Type, OtherSpatialDimensionality > &hyperplane) :
			displacement_vertex(hyperplane.displacement_vertex),
			normal_vector(hyperplane.normal_vector)
		{
		}

		constexpr hyperplane(hyperplane &&) noexcept = default;

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() < SpatialDimensionality::dimensionality())
		constexpr hyperplane(hyperplane < Type, OtherSpatialDimensionality > &&hyperplane) :
			displacement_vertex(std::move(hyperplane.displacement_vertex)),
			normal_vector(std::move(hyperplane.normal_vector))
		{
		}

		template <
			concepts::spatial_dimension OtherSpatialDimensionality1,
			concepts::spatial_dimension OtherSpatialDimensionality2 >
		requires (
			OtherSpatialDimensionality1::dimensionality() <= SpatialDimensionality::dimensionality() &&
			OtherSpatialDimensionality2::dimensionality() <= SpatialDimensionality::dimensionality())
		constexpr hyperplane(
			const vertex < Type, OtherSpatialDimensionality1 > &displacementVertex,
			const vector < Type, OtherSpatialDimensionality2 > &normalVector) :

			displacement_vertex(displacementVertex),
			normal_vector(normalVector)
		{
		}

		template <
			concepts::spatial_dimension OtherSpatialDimensionality1,
			concepts::spatial_dimension OtherSpatialDimensionality2 >
		requires (
			OtherSpatialDimensionality1::dimensionality() <= SpatialDimensionality::dimensionality() &&
			OtherSpatialDimensionality2::dimensionality() <= SpatialDimensionality::dimensionality())
		constexpr hyperplane(
			vertex < Type, OtherSpatialDimensionality1 > &&displacementVertex,
			vector < Type, OtherSpatialDimensionality2 > &&normalVector) :

			displacement_vertex(std::move(displacementVertex)),
			normal_vector(std::move(normalVector))
		{
		}
	};

	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality1,
		concepts::spatial_dimension SpatialDimensionality2 >
	constexpr auto operator+(
		const vector < Type, SpatialDimensionality1 > &vector,
		const hyperplane < Type, SpatialDimensionality2 > &hyperplane)
	{
		return hyperplane + vector;
	}

	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality1,
		concepts::spatial_dimension SpatialDimensionality2 >
	constexpr auto operator-(
		const vector < Type, SpatialDimensionality1 > &vector,
		const hyperplane < Type, SpatialDimensionality2 > &hyperplane)
	{
		return hyperplane - vector;
	}

	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality1,
		concepts::spatial_dimension SpatialDimensionality2 >
	constexpr auto make_hyperplane(
		const vertex < Type, SpatialDimensionality1 > &displacementVertex,
		const vector < Type, SpatialDimensionality2 > &normalVector)
	{
		if constexpr (SpatialDimensionality1::dimensionality() >= SpatialDimensionality2::dimensionality())
		{
			return
				hyperplane < Type, SpatialDimensionality1 > {
					displacementVertex,
					normalVector
				};
		}

		else
		{
			return
				hyperplane < Type, SpatialDimensionality2 > {
					displacementVertex,
					normalVector
				};
		}
	}

	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality1,
		concepts::spatial_dimension SpatialDimensionality2 >
	constexpr auto make_hyperplane(
		vertex < Type, SpatialDimensionality1 > &&displacementVertex,
		vector < Type, SpatialDimensionality2 > &&normalVector)
	{
		if constexpr (SpatialDimensionality1::dimensionality() >= SpatialDimensionality2::dimensionality())
		{
			return
				segment < Type, SpatialDimensionality1 > {
					std::move(displacementVertex),
					std::move(normalVector)
				};
		}

		else
		{
			return
				segment < Type, SpatialDimensionality2 > {
					std::move(displacementVertex),
					std::move(normalVector)
				};
		}
	}
}

export namespace xablau::algebra::functions
{
	template <
		xablau::geometry::concepts::geometric_assignable_arithmetic Type,
		xablau::geometry::concepts::spatial_dimension SpatialDimensionality >
	struct almost_equal < xablau::geometry::hyperplane < Type, SpatialDimensionality > >
	{
		template < size_t UnitsInTheLastPlace >
		[[nodiscard]] bool operator()(
			const xablau::geometry::hyperplane < Type, SpatialDimensionality > &value1,
			const xablau::geometry::hyperplane < Type, SpatialDimensionality > &value2) const
		{
			return almost_equal::invoke < UnitsInTheLastPlace > (value1, value2);
		}

		[[nodiscard]] bool operator()(
			const xablau::geometry::hyperplane < Type, SpatialDimensionality > &value1,
			const xablau::geometry::hyperplane < Type, SpatialDimensionality > &value2,
			const size_t unitsInTheLastPlace) const
		{
			return almost_equal::invoke(value1, value2, unitsInTheLastPlace);
		}

		template < size_t UnitsInTheLastPlace >
		[[nodiscard]] static bool invoke(
			const xablau::geometry::hyperplane < Type, SpatialDimensionality > &value1,
			const xablau::geometry::hyperplane < Type, SpatialDimensionality > &value2)
		{
			const bool degenerate1 = value1.degenerate();
			const bool degenerate2 = value2.degenerate();

			if (degenerate1 ^ degenerate2)
			{
				return false;
			}

			if (degenerate1 || degenerate2)
			{
				return
					almost_equal < xablau::geometry::vertex < Type, SpatialDimensionality > > ::invoke < UnitsInTheLastPlace > (
						value1.displacement_vertex,
						value2.displacement_vertex);
			}

			return
				value1.normal_vector.codirectional_to(value2.normal_vector, UnitsInTheLastPlace) &&
				(value1.displacement_vertex - value2.displacement_vertex).perpendicular_to(value1.normal_vector, UnitsInTheLastPlace);
		}

		[[nodiscard]] static bool invoke(
			const xablau::geometry::vector < Type, SpatialDimensionality > &value1,
			const xablau::geometry::vector < Type, SpatialDimensionality > &value2,
			const size_t unitsInTheLastPlace)
		{
			const bool degenerate1 = value1.degenerate();
			const bool degenerate2 = value2.degenerate();

			if (degenerate1 ^ degenerate2)
			{
				return false;
			}

			if (degenerate1 || degenerate2)
			{
				return
					almost_equal < xablau::geometry::vertex < Type, SpatialDimensionality > > ::invoke(
						value1.displacement_vertex,
						value2.displacement_vertex,
						unitsInTheLastPlace);
			}

			return
				value1.normal_vector.codirectional_to(value2.normal_vector, unitsInTheLastPlace) &&
				(value1.displacement_vertex - value2.displacement_vertex).perpendicular_to(value1.normal_vector, unitsInTheLastPlace);
		}
	};
}

export namespace std
{
	template <
		xablau::geometry::concepts::geometric_assignable_arithmetic Type,
		xablau::geometry::concepts::spatial_dimension SpatialDimensionality >
	struct hash < xablau::geometry::hyperplane < Type, SpatialDimensionality > >
	{
		size_t operator()(const xablau::geometry::hyperplane < Type, SpatialDimensionality > &hyperplane) const
		{
			const auto closestPointToOrigin =
				hyperplane.distance_to < true > (xablau::geometry::vertex < Type, SpatialDimensionality > {}).second;

			return
				std::hash < xablau::geometry::vertex < Type, SpatialDimensionality > > {}(closestPointToOrigin) ^
				std::hash < xablau::geometry::vector < Type, SpatialDimensionality > > {}(hyperplane.normal_vector.normalize());
		}
	};
}