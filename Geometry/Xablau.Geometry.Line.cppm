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

export module xablau.geometry:line;
export import :forward_declarations;

export import std;

export namespace xablau::geometry
{
	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality >
	class line final
	{
	public:
		using value_type = Type;

		using abs_type = typename xablau::algebra::abs_type < Type > ::type;

		using size_type = typename std::array < Type, SpatialDimensionality::dimensionality() > ::size_type;

	private:
		enum class comparison_operator
		{
			less,
			less_equal,
			greater,
			greater_equal
		};

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		static void check_degeneracy(const line < Type, OtherSpatialDimensionality > &line)
		{
			if (line.degenerate())
			{
				throw std::domain_error("\"line\" is degenerate.");
			}
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		static void check_degeneracy_on_projection(const geometry::vector < Type, OtherSpatialDimensionality > &direction)
		{
			if (direction.null())
			{
				throw std::invalid_argument("\"direction\" is null.");
			}
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		abs_type distance_to(
			const line < Type, OtherSpatialDimensionality > &line,
			geometry::vector < Type, SpatialDimensionality > &u,
			geometry::vector < Type, OtherSpatialDimensionality > &v) const
		{
			line::check_degeneracy(*this);
			line::check_degeneracy(line);

			u = this->vector;
			v = line.vector;
			const auto w = this->vertex - line.vertex;
			const auto a = u.scalar_product(u);
			const auto b = u.scalar_product(v);
			const auto c = v.scalar_product(v);
			const auto d = u.scalar_product(w);
			const auto e = v.scalar_product(w);
			const auto determinant = a * c - b * b;

			abs_type s{};
			abs_type t{};

			if (determinant == abs_type{})
			{
				t = (b > c ? d / b : e / c);
			}

			else
			{
				s = (b * e - c * d) / determinant;
				t = (a * e - b * d) / determinant;
			}

			u *= s;
			v *= t;

			return (w + u - v).norm();
		}

		template < typename LineType >
		bool equal_to(const LineType &line) const
		{
			return
				this->vector.parallel_to(line.vector, 0) &&
				(this->vertex - line.vertex).parallel_to(line.vector, 0);
		}

		template <
			comparison_operator ComparisonOperator,
			typename LeftType,
			typename RightType >
		static bool less_than(const LeftType &leftValue, const RightType &rightValue)
		{
			if constexpr (ComparisonOperator == comparison_operator::less)
			{
				return leftValue < rightValue;
			}

			else if constexpr (ComparisonOperator == comparison_operator::less_equal)
			{
				return !(rightValue <= leftValue);
			}

			else if constexpr (ComparisonOperator == comparison_operator::greater_equal)
			{
				return !(leftValue >= rightValue);
			}

			else
			{
				return rightValue > leftValue;
			}
		}

		template < comparison_operator ComparisonOperator, typename VectorType >
		static auto create_vector_for_comparison(const VectorType &vector)
		{
			if constexpr (xablau::algebra::concepts::negatable < Type >)
			{
				for (const auto &coordinate : vector.coordinates)
				{
					const auto negatedCoordinate = -coordinate;

					if (line::template less_than < ComparisonOperator > (coordinate, negatedCoordinate))
					{
						return -vector.normalize();
					}

					if (line::template less_than < ComparisonOperator > (negatedCoordinate, coordinate))
					{
						return vector.normalize();
					}
				}
			}

			return vector.normalize();
		}

	public:
		geometry::vertex < Type, SpatialDimensionality > vertex;

		geometry::vector < Type, SpatialDimensionality > vector;

		[[nodiscard]] explicit constexpr operator bool() const
		{
			return static_cast < bool > (this->vector);
		}

		[[nodiscard]] constexpr bool degenerate() const
		{
			return this->vector.null();
		}

		geometry::vector < Type, SpatialDimensionality > &normalize_vector()
		{
			return this->vector.normalize_in_place();
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool operator<(const line < Type, OtherSpatialDimensionality > &line) const
		{
			const bool thisDegenerate = this->degenerate();
			const bool otherDegenerate = line.degenerate();

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
				return this->vertex < line.vertex;
			}

			if (this->equal_to(line))
			{
				return false;
			}

			const auto normalizedVector1 =
				line::template create_vector_for_comparison < comparison_operator::less > (this->vector);

			const auto normalizedVector2 =
				line::template create_vector_for_comparison < comparison_operator::less > (line.vector);

			if (normalizedVector1 < normalizedVector2)
			{
				return true;
			}

			if (!(normalizedVector2 < normalizedVector1))
			{
				return this->vertex < line.vertex;
			}

			return false;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool operator<=(const line < Type, OtherSpatialDimensionality > &line) const
		{
			const bool thisDegenerate = this->degenerate();
			const bool otherDegenerate = line.degenerate();

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
				return this->vertex <= line.vertex;
			}

			if (this->equal_to(line))
			{
				return true;
			}

			const auto normalizedVector1 =
				line::template create_vector_for_comparison < comparison_operator::less_equal > (this->vector);

			const auto normalizedVector2 =
				line::template create_vector_for_comparison < comparison_operator::less_equal > (line.vector);

			if (!(normalizedVector2 <= normalizedVector1))
			{
				return true;
			}

			if (normalizedVector1 <= normalizedVector2)
			{
				return this->vertex <= line.vertex;
			}

			return false;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool operator==(const line < Type, OtherSpatialDimensionality > &line) const
		{
			const bool thisDegenerate = this->degenerate();
			const bool otherDegenerate = line.degenerate();

			if (thisDegenerate ^ otherDegenerate)
			{
				return false;
			}

			if (thisDegenerate || otherDegenerate)
			{
				return this->vertex == line.vertex;
			}

			return this->equal_to(line);
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool operator>=(const line < Type, OtherSpatialDimensionality > &line) const
		{
			const bool thisDegenerate = this->degenerate();
			const bool otherDegenerate = line.degenerate();

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
				return this->vertex >= line.vertex;
			}

			if (this->equal_to(line))
			{
				return true;
			}

			const auto normalizedVector1 =
				line::template create_vector_for_comparison < comparison_operator::greater_equal > (this->vector);

			const auto normalizedVector2 =
				line::template create_vector_for_comparison < comparison_operator::greater_equal > (line.vector);

			if (!(normalizedVector2 >= normalizedVector1))
			{
				return true;
			}

			if (normalizedVector1 >= normalizedVector2)
			{
				return this->vertex >= line.vertex;
			}

			return false;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool operator>(const line < Type, OtherSpatialDimensionality > &line) const
		{
			const bool thisDegenerate = this->degenerate();
			const bool otherDegenerate = line.degenerate();

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
				return this->vertex > line.vertex;
			}

			if (this->equal_to(line))
			{
				return false;
			}

			const auto normalizedVector1 =
				line::template create_vector_for_comparison < comparison_operator::greater > (this->vector);

			const auto normalizedVector2 =
				line::template create_vector_for_comparison < comparison_operator::greater > (line.vector);

			if (normalizedVector1 > normalizedVector2)
			{
				return true;
			}

			if (!(normalizedVector2 > normalizedVector1))
			{
				return this->vertex > line.vertex;
			}

			return false;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool operator!=(const line < Type, OtherSpatialDimensionality > &line) const
		{
			const bool thisDegenerate = this->degenerate();
			const bool otherDegenerate = line.degenerate();

			if (thisDegenerate ^ otherDegenerate)
			{
				return true;
			}

			if (thisDegenerate || otherDegenerate)
			{
				return this->vertex != line.vertex;
			}

			return !this->equal_to(line);
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr auto operator+(const geometry::vector < Type, OtherSpatialDimensionality > &vector) const
		{
			using ResultSpatialDimension =
				typename greater_spatial_dimension <
					SpatialDimensionality,
					OtherSpatialDimensionality > ::type;

			return
				line < Type, ResultSpatialDimension > (
					this->vertex + vector,
					this->vector);
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr auto operator-(const geometry::vector < Type, OtherSpatialDimensionality > &vector) const
		{
			using ResultSpatialDimension =
				typename greater_spatial_dimension <
					SpatialDimensionality,
					OtherSpatialDimensionality > ::type;

			return
				line < Type, ResultSpatialDimension > (
					this->vertex - vector,
					this->vector);
		}

		constexpr line &operator=(const line &) = default;

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() < SpatialDimensionality::dimensionality())
		constexpr line &operator=(const line < Type, OtherSpatialDimensionality > &line)
		{
			this->vertex = line.vertex;
			this->vector = line.vector;

			return *this;
		}

		constexpr line &operator=(line &&) noexcept = default;

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() < SpatialDimensionality::dimensionality())
		constexpr line &operator=(line < Type, OtherSpatialDimensionality > &&line) noexcept
		{
			this->vertex = std::move(line.vertex);
			this->vector = std::move(line.vector);

			return *this;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() <= SpatialDimensionality::dimensionality())
		constexpr line &operator+=(const geometry::vector < Type, OtherSpatialDimensionality > &vector)
		{
			this->vertex += vector;

			return *this;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() <= SpatialDimensionality::dimensionality())
		constexpr line &operator-=(const geometry::vector < Type, OtherSpatialDimensionality > &vector)
		{
			this->vertex -= vector;

			return *this;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] abs_type angle_between(
			const line < Type, OtherSpatialDimensionality > &line,
			const size_t unitsInTheLastPlace = 1) const
		{
			line::check_degeneracy(*this);
			line::check_degeneracy(line);

			if (!this->intersects < false > (line, unitsInTheLastPlace))
			{
				throw std::domain_error("Both lines do not intersect each other.");
			}

			constexpr auto _180Degrees = std::numbers::pi_v < abs_type >;
			constexpr auto _90Degrees = _180Degrees / abs_type{2};
			auto angle = this->vector.angle_between(line.vector);

			if (angle > _90Degrees)
			{
				return _180Degrees - angle;
			}

			return angle;
		}

		template < bool CalculateClosestVertexOnLine, concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] auto distance_to(const geometry::vertex < Type, OtherSpatialDimensionality > &vertex) const
		{
			line::check_degeneracy(*this);

			const auto projectedVectorOnLine = std::move((vertex - this->vertex).project_on_in_place(this->vector));

			if constexpr (CalculateClosestVertexOnLine)
			{
				auto closestVertexOnLine = this->vertex + projectedVectorOnLine;
				auto distance = vertex.distance_to(closestVertexOnLine);

				return std::make_pair(std::move(distance), std::move(closestVertexOnLine));
			}

			else
			{
				const auto closestVertexOnLine = this->vertex + projectedVectorOnLine;

				return vertex.distance_to(closestVertexOnLine);
			}
		}

		// Based on http://geomalgorithms.com/a07-_distance.html
		template < bool CalculateClosestVerticesOnLines, concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] auto distance_to(const line < Type, OtherSpatialDimensionality > &line) const
		{
			geometry::vector < Type, SpatialDimensionality > u;
			geometry::vector < Type, OtherSpatialDimensionality > v;

			auto distance = this->distance_to(line, u, v);

			if constexpr (CalculateClosestVerticesOnLines)
			{
				return std::make_tuple(distance, this->vertex + u, line.vertex + v);
			}

			else
			{
				return distance;
			}
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool contains(
			const geometry::vertex < Type, OtherSpatialDimensionality > &vertex,
			const size_t unitsInTheLastPlace = 1) const
		{
			line::check_degeneracy(*this);

			const auto vetor1 = this->vertex - vertex;
			const auto &vetor2 = this->vector;

			return vetor1.parallel_to(vetor2, unitsInTheLastPlace);
		}

		template < bool CalculateIntersectionVertex, concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] auto intersects(
			const line < Type, OtherSpatialDimensionality > &line,
			const size_t unitsInTheLastPlace = 1) const
		{
			geometry::vector < Type, SpatialDimensionality > u;
			geometry::vector < Type, OtherSpatialDimensionality > v;

			if (xablau::algebra::functions::almost_equal < abs_type > ::invoke(
					this->distance_to(line, u, v),
					Type{},
					unitsInTheLastPlace))
			{
				if constexpr (CalculateIntersectionVertex)
				{
					return std::make_optional(this->vertex + u);
				}

				else
				{
					return true;
				}
			}

			if constexpr (CalculateIntersectionVertex)
			{
				return std::optional < geometry::vertex < Type, SpatialDimensionality > > {};
			}

			else
			{
				return false;
			}
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool coincides_with(
			const line < Type, OtherSpatialDimensionality > &line,
			const size_t unitsInTheLastPlace = 1) const
		{
			line::check_degeneracy(*this);
			line::check_degeneracy(line);

			return
				this->vector.parallel_to(line.vector, unitsInTheLastPlace) &&
				(this->vertex - line.vertex).parallel_to(line.vector, unitsInTheLastPlace);
		}

		template < bool CalculateIntersection, concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] auto concurres_with(
			const line < Type, OtherSpatialDimensionality > &line,
			const size_t unitsInTheLastPlace = 1) const
		{
			line::check_degeneracy(*this);
			line::check_degeneracy(line);

			if constexpr (CalculateIntersection)
			{
				if (!this->coincides_with(line, unitsInTheLastPlace))
				{
					return this->intersects < true > (line, unitsInTheLastPlace);
				}

				else
				{
					return std::optional < geometry::vertex < Type, SpatialDimensionality > > {};
				}
			}

			else
			{
				return
					!this->coincides_with(line, unitsInTheLastPlace) &&
					this->intersects < false > (line, unitsInTheLastPlace);
			}
		}

		template < bool CalculateIntersection, concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] auto perpendicular_to(
			const line < Type, OtherSpatialDimensionality > &line,
			const size_t unitsInTheLastPlace = 1) const
		{
			line::check_degeneracy(*this);
			line::check_degeneracy(line);

			if constexpr (CalculateIntersection)
			{
				if (this->vector.perpendicular_to(line.vector, unitsInTheLastPlace))
				{
					return this->intersects < true > (line, unitsInTheLastPlace);
				}

				return std::optional < geometry::vertex < Type, SpatialDimensionality > > {};
			}

			else
			{
				return
					this->vector.perpendicular_to(line.vector, unitsInTheLastPlace) &&
					this->intersects < false > (line, unitsInTheLastPlace);
			}

		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool parallel_to(
			const line < Type, OtherSpatialDimensionality > &line,
			const size_t unitsInTheLastPlace = 1) const
		{
			line::check_degeneracy(*this);
			line::check_degeneracy(line);

			return
				this->vector.parallel_to(line.vector, unitsInTheLastPlace) &&
				!(this->vertex - line.vertex).parallel_to(line.vector, unitsInTheLastPlace);
		}

		[[nodiscard]] static consteval size_t dimensionality() noexcept
		{
			return SpatialDimensionality::dimensionality();
		}

		friend std::ostream &operator<<(std::ostream &stream, const line &line)
		{
			stream << "-- ( " << line.vertex << ", " << line.vector << " )";

			return stream;
		}

		constexpr line() noexcept = default;

		constexpr line(const line &) = default;

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() < SpatialDimensionality::dimensionality())
		constexpr line(const line < Type, OtherSpatialDimensionality > &line) :
			vertex(line.vertex),
			vector(line.vector)
		{
		}

		constexpr line(line &&) noexcept = default;

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() < SpatialDimensionality::dimensionality())
		constexpr line(line < Type, OtherSpatialDimensionality > &&line) noexcept :
			vertex(std::move(line.vertex)),
			vector(std::move(line.vector))
		{
		}

		template <
			concepts::spatial_dimension OtherSpatialDimensionality1,
			concepts::spatial_dimension OtherSpatialDimensionality2 >
		requires (
			OtherSpatialDimensionality1::dimensionality() <= SpatialDimensionality::dimensionality() &&
			OtherSpatialDimensionality2::dimensionality() <= SpatialDimensionality::dimensionality())
		constexpr line(
			const geometry::vertex < Type, OtherSpatialDimensionality1 > &vertex,
			const geometry::vector < Type, OtherSpatialDimensionality2 > &vector) :

			vertex(vertex),
			vector(vector)
		{
		}

		template <
			concepts::spatial_dimension OtherSpatialDimensionality1,
			concepts::spatial_dimension OtherSpatialDimensionality2 >
		requires (
			OtherSpatialDimensionality1::dimensionality() <= SpatialDimensionality::dimensionality() &&
			OtherSpatialDimensionality2::dimensionality() <= SpatialDimensionality::dimensionality())
		constexpr line(
			geometry::vertex < Type, OtherSpatialDimensionality1 > &&vertex,
			geometry::vector < Type, OtherSpatialDimensionality2 > &&vector) noexcept :

			vertex(std::move(vertex)),
			vector(std::move(vector))
		{
		}

		template <
			concepts::spatial_dimension OtherSpatialDimensionality1,
			concepts::spatial_dimension OtherSpatialDimensionality2 >
		requires (
			OtherSpatialDimensionality1::dimensionality() <= SpatialDimensionality::dimensionality() &&
			OtherSpatialDimensionality2::dimensionality() <= SpatialDimensionality::dimensionality())
		constexpr line(
			const geometry::vertex < Type, OtherSpatialDimensionality1 > &startingVertex,
			const geometry::vertex < Type, OtherSpatialDimensionality2 > &endingVertex) :

			vertex(startingVertex),
			vector(geometry::vector < Type, SpatialDimensionality > (startingVertex, endingVertex))
		{
		}
	};

	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality1,
		concepts::spatial_dimension SpatialDimensionality2 >
	constexpr auto operator+(
		const vector < Type, SpatialDimensionality1 > &vector,
		const line < Type, SpatialDimensionality2 > &line)
	{
		return line + vector;
	}

	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality1,
		concepts::spatial_dimension SpatialDimensionality2 >
	constexpr auto operator-(
		const vector < Type, SpatialDimensionality1 > &vector,
		const line < Type, SpatialDimensionality2 > &line)
	{
		return line - vector;
	}

	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality1,
		concepts::spatial_dimension SpatialDimensionality2 >
	constexpr auto make_line(
		const vertex < Type, SpatialDimensionality1 > &vertex,
		const vector < Type, SpatialDimensionality2 > &vector)
	{
		if constexpr (SpatialDimensionality1::dimensionality() >= SpatialDimensionality2::dimensionality())
		{
			return line < Type, SpatialDimensionality1 > {vertex, vector};
		}

		else
		{
			return line < Type, SpatialDimensionality2 > {vertex, vector};
		}
	}

	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality1,
		concepts::spatial_dimension SpatialDimensionality2 >
	constexpr auto make_line(
		vertex < Type, SpatialDimensionality1 > &&vertex,
		vector < Type, SpatialDimensionality2 > &&vector)
	{
		if constexpr (SpatialDimensionality1::dimensionality() >= SpatialDimensionality2::dimensionality())
		{
			return segment < Type, SpatialDimensionality1 > {std::move(vertex), std::move(vector)};
		}

		else
		{
			return segment < Type, SpatialDimensionality2 > {std::move(vertex), std::move(vector)};
		}
	}

	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality1,
		concepts::spatial_dimension SpatialDimensionality2 >
	constexpr auto make_line(
		const vertex < Type, SpatialDimensionality1 > &startingVertex,
		const vertex < Type, SpatialDimensionality2 > &endingVertex)
	{
		if constexpr (SpatialDimensionality1::dimensionality() >= SpatialDimensionality2::dimensionality())
		{
			return line < Type, SpatialDimensionality1 > {startingVertex, endingVertex};
		}

		else
		{
			return line < Type, SpatialDimensionality2 > {startingVertex, endingVertex};
		}
	}
}

export namespace xablau::algebra::functions
{
	template <
		xablau::geometry::concepts::geometric_assignable_arithmetic Type,
		xablau::geometry::concepts::spatial_dimension SpatialDimensionality >
	struct almost_equal < xablau::geometry::line < Type, SpatialDimensionality > >
	{
		template < size_t UnitsInTheLastPlace >
		[[nodiscard]] bool operator()(
			const xablau::geometry::line < Type, SpatialDimensionality > &value1,
			const xablau::geometry::line < Type, SpatialDimensionality > &value2) const
		{
			return almost_equal::invoke < UnitsInTheLastPlace > (value1, value2);
		}

		[[nodiscard]] bool operator()(
			const xablau::geometry::line < Type, SpatialDimensionality > &value1,
			const xablau::geometry::line < Type, SpatialDimensionality > &value2,
			const size_t unitsInTheLastPlace) const
		{
			return almost_equal::invoke(value1, value2, unitsInTheLastPlace);
		}

		template < size_t UnitsInTheLastPlace >
		[[nodiscard]] static bool invoke(
			const xablau::geometry::line < Type, SpatialDimensionality > &value1,
			const xablau::geometry::line < Type, SpatialDimensionality > &value2)
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
						value1.vertex,
						value2.vertex);
			}

			return
				value1.vector.parallel_to(value2.vector, UnitsInTheLastPlace) &&
				(value1.vertex - value2.vertex).parallel_to(value2.vector, UnitsInTheLastPlace);
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
						value1.vertex,
						value2.vertex,
						unitsInTheLastPlace);
			}

			return
				value1.vector.parallel_to(value2.vector, unitsInTheLastPlace) &&
				(value1.vertex - value2.vertex).parallel_to(value2.vector, unitsInTheLastPlace);
		}
	};
}

export namespace std
{
	template <
		xablau::geometry::concepts::geometric_assignable_arithmetic Type,
		xablau::geometry::concepts::spatial_dimension SpatialDimensionality >
	struct hash < xablau::geometry::line < Type, SpatialDimensionality > >
	{
		size_t operator()(const xablau::geometry::line < Type, SpatialDimensionality > &line) const
		{
			const auto normalizedVector = line.vector.normalize();
			const auto closestPointToOrigin =
				line.distance_to < true > (xablau::geometry::vertex < Type, SpatialDimensionality > {}).second;

			return
				std::hash < xablau::geometry::vertex < Type, SpatialDimensionality > > {}(closestPointToOrigin) ^
				std::hash < xablau::geometry::vector < Type, SpatialDimensionality > > {}(normalizedVector) ^
				std::hash < xablau::geometry::vector < Type, SpatialDimensionality > > {}(-normalizedVector);
		}
	};
}