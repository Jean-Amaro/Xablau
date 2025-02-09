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

export module xablau.geometry:vertex;
export import :forward_declarations;

export import std;

export import xablau.algebra;

export namespace xablau::geometry
{
	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality >
	class vertex final
	{
	public:
		using value_type = Type;

		using abs_type = typename xablau::algebra::abs_type < Type > ::type;

		using size_type = typename std::array < Type, SpatialDimensionality::dimensionality() > ::size_type;

		std::array < Type, SpatialDimensionality::dimensionality() > coordinates{};

	private:
		enum class vertex_comparison_result
		{
			less,
			equal,
			greater,
			not_equal
		};

		enum class comparison_operator
		{
			less,
			less_equal,
			greater,
			greater_equal
		};

		template < bool Equal, typename VertexType >
		[[nodiscard]] constexpr vertex_comparison_result vertex_comparison_equality(const VertexType &vertex) const
		{
			size_type i = 0;
			constexpr auto limit = std::min(vertex.dimensionality(), this->dimensionality());
			const auto &_coordinates = vertex.coordinates;

			constexpr auto notEqual =
				[] (const Type &leftValue, const Type &rightValue) -> bool
				{
					if constexpr (Equal)
					{
						return !(leftValue == rightValue);
					}

					else
					{
						return leftValue != rightValue;
					}
				};

			for (; i < limit; i++)
			{
				if (notEqual(this->coordinates[i], _coordinates[i]))
				{
					return vertex_comparison_result::not_equal;
				}
			}

			for (; i < this->dimensionality(); i++)
			{
				if (notEqual(this->coordinates[i], Type{}))
				{
					return vertex_comparison_result::not_equal;
				}
			}

			for (; i < vertex.dimensionality(); i++)
			{
				if (notEqual(_coordinates[i], Type{}))
				{
					return vertex_comparison_result::not_equal;
				}
			}

			return vertex_comparison_result::equal;
		}

		template < comparison_operator ComparisonOperator, typename VertexType >
		[[nodiscard]] constexpr vertex_comparison_result vertex_comparison_inequality(const VertexType &vertex) const
		{
			size_type i = 0;
			constexpr auto limit = std::min(vertex.dimensionality(), this->dimensionality());
			const auto &_coordinates = vertex.coordinates;

			constexpr auto lessThan =
				[] (const Type &leftValue, const Type &rightValue) -> bool
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
				};

			for (; i < limit; i++)
			{
				if (lessThan(this->coordinates[i], _coordinates[i]))
				{
					return vertex_comparison_result::less;
				}

				if (lessThan(_coordinates[i], this->coordinates[i]))
				{
					return vertex_comparison_result::greater;
				}
			}

			for (; i < this->dimensionality(); i++)
			{
				if (lessThan(this->coordinates[i], Type{}))
				{
					return vertex_comparison_result::less;
				}

				if (lessThan(Type{}, this->coordinates[i]))
				{
					return vertex_comparison_result::greater;
				}
			}

			for (; i < vertex.dimensionality(); i++)
			{
				if (lessThan(Type{}, _coordinates[i]))
				{
					return vertex_comparison_result::less;
				}

				if (lessThan(_coordinates[i], Type{}))
				{
					return vertex_comparison_result::greater;
				}
			}

			return vertex_comparison_result::equal;
		}

		static constexpr Type _null_value{};

		template <
			typename VertexType,
			typename HyperplaneType,
			typename VectorType >
		static void projection_on_hyperplane(
			VertexType &vertex,
			const HyperplaneType &hyperplane,
			const VectorType &direction)
		{
			const auto scalarProductHyperplane =
				hyperplane.normal_vector.scalar_product(hyperplane.displacement_vertex.vector_from_origin());

			const auto scalarProductLine =
				hyperplane.normal_vector.scalar_product(vertex.vector_from_origin());

			const auto lambda =
				(scalarProductHyperplane - scalarProductLine) / hyperplane.normal_vector.scalar_product(direction);

			vertex += (lambda * direction);
		}

	public:
		[[nodiscard]] constexpr Type &x() noexcept
		requires (SpatialDimensionality::dimensionality() >= 1)
		{
			return this->coordinates[0];
		}

		[[nodiscard]] constexpr Type &y() noexcept
		requires (SpatialDimensionality::dimensionality() >= 2)
		{
			return this->coordinates[1];
		}

		[[nodiscard]] constexpr Type &z() noexcept
		requires (SpatialDimensionality::dimensionality() >= 3)
		{
			return this->coordinates[2];
		}

		[[nodiscard]] constexpr Type &w() noexcept
		requires (SpatialDimensionality::dimensionality() >= 4)
		{
			return this->coordinates[3];
		}

		[[nodiscard]] constexpr const Type &x() const noexcept
		{
			if constexpr (SpatialDimensionality::dimensionality() >= 1)
			{
				return this->coordinates[0];
			}

			else
			{
				return vertex::_null_value;
			}
		}

		[[nodiscard]] constexpr const Type &y() const noexcept
		{
			if constexpr (SpatialDimensionality::dimensionality() >= 2)
			{
				return this->coordinates[1];
			}

			else
			{
				return vertex::_null_value;
			}
		}

		[[nodiscard]] constexpr const Type &z() const noexcept
		{
			if constexpr (SpatialDimensionality::dimensionality() >= 3)
			{
				return this->coordinates[2];
			}

			else
			{
				return vertex::_null_value;
			}
		}

		[[nodiscard]] constexpr const Type &w() const noexcept
		{
			if constexpr (SpatialDimensionality::dimensionality() >= 4)
			{
				return this->coordinates[3];
			}

			else
			{
				return vertex::_null_value;
			}
		}

		[[nodiscard]] constexpr bool at_origin() const
		{
			for (const auto &item : this->coordinates)
			{
				if (item != Type{})
				{
					return false;
				}
			}

			return true;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr bool operator<(const vertex < Type, OtherSpatialDimensionality > &vertex) const
		{
			return this->vertex_comparison_inequality < comparison_operator::less > (vertex) == vertex_comparison_result::less;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr bool operator<=(const vertex < Type, OtherSpatialDimensionality > &vertex) const
		{
			return this->vertex_comparison_inequality < comparison_operator::less_equal > (vertex) != vertex_comparison_result::greater;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr bool operator==(const vertex < Type, OtherSpatialDimensionality > &vertex) const
		{
			return this->vertex_comparison_equality < true > (vertex) == vertex_comparison_result::equal;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr bool operator>=(const vertex < Type, OtherSpatialDimensionality > &vertex) const
		{
			return this->vertex_comparison_inequality < comparison_operator::greater_equal > (vertex) != vertex_comparison_result::less;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr bool operator>(const vertex < Type, OtherSpatialDimensionality > &vertex) const
		{
			return this->vertex_comparison_inequality < comparison_operator::greater > (vertex) == vertex_comparison_result::greater;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr bool operator!=(const vertex < Type, OtherSpatialDimensionality > &vertex) const
		{
			return this->template vertex_comparison_equality < false > (vertex) != vertex_comparison_result::equal;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr auto operator-(const vertex < Type, OtherSpatialDimensionality > &vertex) const
		requires (xablau::algebra::concepts::negatable < Type >)
		{
			using ResultSpatialDimension =
				typename greater_spatial_dimension <
					SpatialDimensionality,
					OtherSpatialDimensionality > ::type;

			size_type i{};
			const auto &_coordinates = vertex.coordinates;
			constexpr auto min = std::min(vertex.dimensionality(), this->dimensionality());
			vector < Type, ResultSpatialDimension > result;

			for (; i < min; i++)
			{
				result.coordinates[i] = this->coordinates[i] - _coordinates[i];
			}

			for (; i < this->dimensionality(); i++)
			{
				result.coordinates[i] = this->coordinates[i];
			}

			for (; i < vertex.dimensionality(); i++)
			{
				result.coordinates[i] = -_coordinates[i];
			}

			return result;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() <= SpatialDimensionality::dimensionality())
		constexpr vertex &operator+=(const vector < Type, OtherSpatialDimensionality > &vector)
		{
			size_type i{};
			const auto &_coordinates = vector.coordinates;
			constexpr auto min = std::min(this->dimensionality(), vector.dimensionality());

			for (; i < min; i++)
			{
				this->coordinates[i] += _coordinates[i];
			}

			return *this;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() <= SpatialDimensionality::dimensionality())
		constexpr vertex &operator-=(const vector < Type, OtherSpatialDimensionality > &vector)
		requires (xablau::algebra::concepts::negatable < Type >)
		{
			size_type i{};
			const auto &_coordinates = vector.coordinates;
			constexpr auto min = std::min(this->dimensionality(), vector.dimensionality());

			for (; i < min; i++)
			{
				this->coordinates[i] -= _coordinates[i];
			}

			return *this;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr auto operator+(const vector < Type, OtherSpatialDimensionality > &vector) const
		{
			using ResultSpatialDimension =
				typename greater_spatial_dimension <
					SpatialDimensionality,
					OtherSpatialDimensionality > ::type;

			size_type i{};
			const auto &_coordinates = vector.coordinates;
			constexpr auto min = std::min(vector.dimensionality(), this->dimensionality());
			vertex < Type, ResultSpatialDimension > result;

			for (; i < min; i++)
			{
				result.coordinates[i] = this->coordinates[i] + _coordinates[i];
			}

			for (; i < this->dimensionality(); i++)
			{
				result.coordinates[i] = this->coordinates[i];
			}

			for (; i < _coordinates.size(); i++)
			{
				result.coordinates[i] = _coordinates[i];
			}

			return result;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr auto operator-(const vector < Type, OtherSpatialDimensionality > &vector) const
		requires (xablau::algebra::concepts::negatable < Type >)
		{
			using ResultSpatialDimension =
				typename greater_spatial_dimension <
					SpatialDimensionality,
					OtherSpatialDimensionality > ::type;

			size_type i{};
			const auto &_coordinates = vector.coordinates;
			constexpr auto min = std::min(vector.dimensionality(), this->dimensionality());
			vertex < Type, ResultSpatialDimension > result;

			for (; i < min; i++)
			{
				result.coordinates[i] = this->coordinates[i] - _coordinates[i];
			}

			for (; i < this->dimensionality(); i++)
			{
				result.coordinates[i] = this->coordinates[i];
			}

			for (; i < _coordinates.size(); i++)
			{
				result.coordinates[i] = -_coordinates[i];
			}

			return result;
		}

		constexpr void fill(const Type &value)
		{
			this->coordinates.fill(value);
		}

		constexpr vertex &operator=(const vertex &) = default;

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() < SpatialDimensionality::dimensionality())
		constexpr vertex &operator=(const vertex < Type, OtherSpatialDimensionality > &vertex)
		{
			std::copy(
				vertex.coordinates.cbegin(),
				vertex.coordinates.cend(),
				this->coordinates.begin());

			std::fill(
				this->coordinates.begin() + OtherSpatialDimensionality::dimensionality(),
				this->coordinates.end(),
				Type{});

			return *this;
		}

		constexpr vertex &operator=(vertex &&) noexcept = default;

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() < SpatialDimensionality::dimensionality())
		constexpr vertex &operator=(vertex < Type, OtherSpatialDimensionality > &&vertex) noexcept
		{
			std::move(
				vertex.coordinates.cbegin(),
				vertex.coordinates.cend(),
				this->coordinates.begin());

			std::fill(
				this->coordinates.begin() + OtherSpatialDimensionality::dimensionality(),
				this->coordinates.end(),
				Type{});

			return *this;
		}

		[[nodiscard]] constexpr Type &operator[](const size_type dimension)
		{
			return this->coordinates.at(dimension);
		}

		[[nodiscard]] constexpr const Type &operator[](const size_type dimension) const noexcept
		{
			return (dimension < this->dimensionality() ? this->coordinates[dimension] : vertex::_null_value);
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool contained_in(
			const segment < Type, OtherSpatialDimensionality > &segment,
			const size_t unitsInTheLastPlace = 1) const
		{
			return segment.contains(*this, unitsInTheLastPlace);
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool contained_in(
			const line < Type, OtherSpatialDimensionality > &line,
			const size_t unitsInTheLastPlace = 1) const
		{
			return line.contains(*this, unitsInTheLastPlace);
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool contained_in(
			const hyperplane < Type, OtherSpatialDimensionality > &hyperplane,
			const size_t unitsInTheLastPlace = 1) const
		{
			return hyperplane.contains(*this, unitsInTheLastPlace);
		}

		template <
			bool CalculateDistanceToSurface,
			concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] auto contained_in(
			const hypersphere < Type, OtherSpatialDimensionality > &hypersphere,
			const size_t unitsInTheLastPlace = 1) const
		{
			return
				hypersphere.contains < CalculateDistanceToSurface > (
					*this,
					unitsInTheLastPlace);
		}

		[[nodiscard]] abs_type distance_to_origin() const
		{
			return this->distance_to(vertex());
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] abs_type distance_to(const vertex < Type, OtherSpatialDimensionality > &vertex) const
		{
			return ((*this) - vertex).norm();
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] Type Minkowski_distance_to(
			const vertex < Type, OtherSpatialDimensionality > &vertex,
			const Type order) const
		requires (std::floating_point < Type >)
		{
			size_type i{};
			const auto &_coordinates = vertex.coordinates;
			constexpr size_type min = std::min(vertex.dimensionality(), this->dimensionality());
			Type result{};

			for (; i < min; i++)
			{
				result += std::pow(this->coordinates[i] - _coordinates[i], order);
			}

			for (; i < this->dimensionality(); i++)
			{
				result += std::pow(this->coordinates[i], order);
			}

			for (; i < vertex.dimensionality(); i++)
			{
				result += std::pow(-_coordinates[i], order);
			}

			return std::pow(result, Type{1} / order);
		}

		template <
			bool CalculateClosestVertexOnLine,
			concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] auto distance_to(const line < Type, OtherSpatialDimensionality > &line) const
		{
			return line.template distance_to < CalculateClosestVertexOnLine > (*this);
		}

		// Based on http://geomalgorithms.com/a02-_lines.html
		template <
			bool CalculateClosestVertexOnSegment,
			concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] auto distance_to(const segment < Type, OtherSpatialDimensionality > &segment) const
		{
			return segment.template distance_to < CalculateClosestVertexOnSegment > (*this);
		}

		template <
			bool CalculateClosestVertexOnHyperplane,
			concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] auto distance_to(const hyperplane < Type, OtherSpatialDimensionality > &hyperplane) const
		{
			return hyperplane.template distance_to < CalculateClosestVertexOnHyperplane > (*this);
		}

		template <
			concepts::spatial_dimension OtherSpatialDimensionality1,
			concepts::spatial_dimension OtherSpatialDimensionality2 >
		[[nodiscard]] abs_type angle(
			const vertex < Type, OtherSpatialDimensionality1 > &vertex1,
			const vertex < Type, OtherSpatialDimensionality2 > &vertex2) const
		{
			const auto vector1 = vertex1 - (*this);
			const auto vector2 = vertex2 - (*this);

			return vector1.angle_between(vector2);
		}

		template <
			concepts::spatial_dimension OtherSpatialDimensionality1,
			concepts::spatial_dimension OtherSpatialDimensionality2,
			concepts::spatial_dimension OtherSpatialDimensionality3 >
		requires (
			OtherSpatialDimensionality1::dimensionality() <= 7 &&
			OtherSpatialDimensionality2::dimensionality() <= 7)
		[[nodiscard]] abs_type angle_with_sign(
			const vertex < Type, OtherSpatialDimensionality1 > &vertex1,
			const vertex < Type, OtherSpatialDimensionality2 > &vertex2,
			const vector < Type, OtherSpatialDimensionality3 > &orientation) const
		requires (SpatialDimensionality::dimensionality() <= 7)
		{
			const auto vector1 = vertex1 - (*this);
			const auto vector2 = vertex2 - (*this);

			return vector1.angle_between_with_sign(vector2, orientation);
		}

		constexpr vertex &translate_to_origin()
		{
			this->coordinates.fill(Type{});

			return *this;
		}

		template < size_t N >
		requires (N <= SpatialDimensionality::dimensionality())
		constexpr vertex &translate_to(const std::array < Type, N > &coordinates)
		{
			if constexpr (N == SpatialDimensionality::dimensionality())
			{
				this->coordinates = coordinates;
			}

			else
			{
				std::copy(
					coordinates.cbegin(),
					coordinates.cend(),
					this->coordinates.begin());
			}

			return *this;
		}

		template < typename CoefficientType >
		requires (std::same_as < CoefficientType, Type > || std::same_as < CoefficientType, abs_type >)
		constexpr vertex &scale_by(const CoefficientType &coefficient)
		{
			for (auto &coordinate : this->coordinates)
			{
				coordinate = coefficient * coordinate;
			}

			return *this;
		}

		template < typename Range >
		requires (
			std::ranges::sized_range < Range > &&
			std::ranges::forward_range < Range > && (
				std::same_as < std::ranges::range_value_t < Range >, abs_type > ||
				std::same_as < std::ranges::range_value_t < Range >, Type >))
		constexpr vertex &scale_by(const Range &coefficients)
		{
			if (this->dimensionality() <= std::ranges::size(coefficients))
			{
				std::transform(
					this->coordinates.cbegin(),
					this->coordinates.cend(),
					std::ranges::cbegin(coefficients),
					this->coordinates.begin(),

					[] (const Type &value1, const auto &value2) -> Type
					{
						return value2 * value1;
					});
			}

			else
			{
				std::transform(
					std::ranges::cbegin(coefficients),
					std::ranges::cend(coefficients),
					this->coordinates.cbegin(),
					this->coordinates.begin(),

					[] (const auto &value1, const Type &value2) -> Type
					{
						return value1 * value2;
					});

				for (size_type i = std::ranges::size(coefficients); i < this->dimensionality(); i++)
				{
					this->coordinates[i] = Type{};
				}
			}

			return *this;
		}

		template <
			vector_product_dimension Dimension,
			concepts::spatial_dimension OtherSpatialDimensionality1,
			concepts::spatial_dimension OtherSpatialDimensionality2 >
		requires (
			(Dimension == vector_product_dimension::seven ?
				SpatialDimensionality::dimensionality() <= 7 :
				SpatialDimensionality::dimensionality() <= 3) &&

			(Dimension == vector_product_dimension::seven ?
				OtherSpatialDimensionality1::dimensionality() <= 7 :
				OtherSpatialDimensionality1::dimensionality() <= 3) &&

			(Dimension == vector_product_dimension::seven ?
				OtherSpatialDimensionality2::dimensionality() <= 7 :
				OtherSpatialDimensionality2::dimensionality() <= 3))
		[[nodiscard]] constexpr auto vector_product(
			const vertex < Type, OtherSpatialDimensionality1 > &vertex1,
			const vertex < Type, OtherSpatialDimensionality2 > &vertex2) const
		{
			const auto vector1 = vertex1 - (*this);
			const auto vector2 = vertex2 - (*this);

			return vector1.template vector_product < Dimension > (vector2);
		}

		template <
			concepts::spatial_dimension OtherSpatialDimensionality1,
			concepts::spatial_dimension OtherSpatialDimensionality2 >
		[[nodiscard]] auto project_on(
			const hyperplane < Type, OtherSpatialDimensionality1 > &hyperplane,
			const vector < Type, OtherSpatialDimensionality2 > &direction) const
		{
			using ResultSpatialDimension =
				typename greater_spatial_dimension <
					SpatialDimensionality,
					typename greater_spatial_dimension <
						OtherSpatialDimensionality1,
						OtherSpatialDimensionality2 > ::type > ::type;

			const auto vectorDirectedToTheHyperplane = hyperplane.displacement_vertex - (*this);

			if (vectorDirectedToTheHyperplane.null())
			{
				return std::make_optional(vertex < Type, ResultSpatialDimension > (*this));
			}

			const auto vectorDirectedToTheClosestVertexOnHyperplane =
				vectorDirectedToTheHyperplane.project_on(hyperplane.normal_vector);

			if (vectorDirectedToTheClosestVertexOnHyperplane.scalar_product(direction) <= abs_type{})
			{
				return std::optional < vertex < Type, ResultSpatialDimension > > {};
			}

			vertex < Type, ResultSpatialDimension > result = *this;

			vertex::projection_on_hyperplane(result, hyperplane, direction);

			return std::make_optional(result);
		}

		template <
			concepts::spatial_dimension OtherSpatialDimensionality1,
			concepts::spatial_dimension OtherSpatialDimensionality2 >
		requires (
			OtherSpatialDimensionality1::dimensionality() <= SpatialDimensionality::dimensionality() &&
			OtherSpatialDimensionality2::dimensionality() <= SpatialDimensionality::dimensionality())
		std::pair < std::reference_wrapper < vertex >, bool > project_on_in_place(
			const hyperplane < Type, OtherSpatialDimensionality1 > &hyperplane,
			const vector < Type, OtherSpatialDimensionality2 > &direction)
		{
			const auto vectorDirectedToTheHyperplane = hyperplane.displacement_vertex - (*this);

			if (vectorDirectedToTheHyperplane.null())
			{
				return std::make_pair(*this, true);
			}

			const auto vectorDirectedToTheClosestVertexOnHyperplane =
				vectorDirectedToTheHyperplane.project_on(hyperplane.normal_vector);

			if (vectorDirectedToTheClosestVertexOnHyperplane.scalar_product(direction) <= abs_type{})
			{
				this->coordinates.fill(Type{});

				return std::make_pair(*this, false);
			}

			vertex::projection_on_hyperplane(*this, hyperplane, direction);

			return std::make_pair(*this, true);
		}

		friend std::ostream &operator<<(std::ostream &stream, const vertex &vertex)
		{
			auto iterator = vertex.coordinates.cbegin();
			const auto end = vertex.coordinates.cend();

			stream << "* ( ";

			if (iterator != end)
			{
				for (; iterator + 1 < end; ++iterator)
				{
					stream << *iterator << ", ";
				}

				stream << *iterator;
			}

			stream << " )";

			return stream;
		}

		[[nodiscard]] static consteval size_type dimensionality() noexcept
		{
			return SpatialDimensionality::dimensionality();
		}

		vertex &rotate_around_X_axis(const abs_type &angle)
		requires (SpatialDimensionality::dimensionality() == 3)
		{
			const auto y = this->y();
			const auto sinAngle = xablau::algebra::functions::sin < abs_type > ::invoke(angle);
			const auto cosAngle = xablau::algebra::functions::cos < abs_type > ::invoke(angle);

			this->y() = cosAngle * this->y() - sinAngle * this->z();
			this->z() = sinAngle * y + cosAngle * this->z();

			return *this;
		}

		vertex &rotate_around_Y_axis(const abs_type &angle)
		requires (SpatialDimensionality::dimensionality() == 3)
		{
			const auto x = this->x();
			const auto sinAngle = xablau::algebra::functions::sin < abs_type > ::invoke(angle);
			const auto cosAngle = xablau::algebra::functions::cos < abs_type > ::invoke(angle);

			this->x() = cosAngle * this->x() + sinAngle * this->z();
			this->z() = -sinAngle * x + cosAngle * this->z();

			return *this;
		}

		vertex &rotate_around_Z_axis(const abs_type &angle)
		requires (
			SpatialDimensionality::dimensionality() == 2 ||
			SpatialDimensionality::dimensionality() == 3)
		{
			const auto x = this->x();
			const auto sinAngle = xablau::algebra::functions::sin < abs_type > ::invoke(angle);
			const auto cosAngle = xablau::algebra::functions::cos < abs_type > ::invoke(angle);

			this->x() = cosAngle * this->x() - sinAngle * this->y();
			this->y() = sinAngle * x + cosAngle * this->y();

			return *this;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() <= 3)
		vertex &rotate_around_arbitrary_axis(const vector < Type, OtherSpatialDimensionality > &axis, const abs_type &angle)
		requires (SpatialDimensionality::dimensionality() == 3)
		{
			const auto normalizedAxis = axis.normalize();

			const auto x = this->x();
			const auto y = this->y();
			const auto z = this->z();
			const auto &ux = normalizedAxis.x();
			const auto &uy = normalizedAxis.y();
			const auto &uz = normalizedAxis.z();
			const auto sinAngle = xablau::algebra::functions::sin < abs_type > ::invoke(angle);
			const auto cosAngle = xablau::algebra::functions::cos < abs_type > ::invoke(angle);
			const auto oneMinusCosAngle = abs_type{1} - cosAngle;

			this->x() =
				x * (cosAngle + ux * ux * oneMinusCosAngle) +
				y * (ux * uy * oneMinusCosAngle - uz * sinAngle) +
				z * (ux * uz * oneMinusCosAngle + uy * sinAngle);

			this->y() =
				x * (uy * ux * oneMinusCosAngle + uz * sinAngle) +
				y * (cosAngle + uy * uy * oneMinusCosAngle) +
				z * (uy * uz * oneMinusCosAngle - ux * sinAngle);

			this->z() =
				x * (uz * ux * oneMinusCosAngle - uy * sinAngle) +
				y * (uz * uy * oneMinusCosAngle + ux * sinAngle) +
				z * (cosAngle + uz * uz * oneMinusCosAngle);

			return *this;
		}

		template <
			concepts::spatial_dimension OtherSpatialDimensionality1,
			concepts::spatial_dimension OtherSpatialDimensionality2 >
		requires (
			OtherSpatialDimensionality1::dimensionality() <= 3 &&
			OtherSpatialDimensionality2::dimensionality() <= 3)
		vertex &rotate_around_arbitrary_axis(
			const vector < Type, OtherSpatialDimensionality1 > &axis,
			const vertex < Type, OtherSpatialDimensionality2 > &reference,
			const abs_type &angle)
		requires (SpatialDimensionality::dimensionality() == 3)
		{
			const auto displacement = reference - vertex();

			*this -= displacement;

			this->rotate_around_arbitrary_axis(axis, angle);

			*this += displacement;

			return *this;
		}

		[[nodiscard]] constexpr vertex transform(const std::function < Type(const Type &) > &transformFunction) const
		{
			vertex vertex;

			std::transform(this->coordinates.cbegin(), this->coordinates.cend(), vertex.coordinates.begin(),
				[&transformFunction] (const Type &value) -> Type
				{
					return transformFunction(value);
				});

			return vertex;
		}

		template < typename Range >
		requires (
			std::ranges::sized_range < Range > &&
			std::ranges::forward_range < Range > &&
			std::same_as < std::ranges::range_value_t < Range >, std::function < Type(const vertex &) > >)
		[[nodiscard]] constexpr vertex transform(const Range &transformFunctions) const
		{
			if (std::ranges::size(transformFunctions) >= SpatialDimensionality::dimensionality())
			{
				throw std::out_of_range("\"transformFunctions.size() >= SpatialDimensionality::dimensionality()\"");
			}

			vertex vertex;

			std::transform(std::ranges::cbegin(transformFunctions), std::ranges::cend(transformFunctions), vertex.coordinates.begin(),
				[this] (const std::function < Type(const geometry::vertex &) > &transformFunction) -> Type
				{
					return transformFunction(*this);
				});

			return vertex;
		}

		constexpr vertex &transform_in_place(const std::function < Type(const Type &) > &transformFunction)
		{
			std::transform(this->coordinates.cbegin(), this->coordinates.cend(), this->coordinates.begin(),
				[&transformFunction] (const Type &value) -> Type
				{
					return transformFunction(value);
				});

			return *this;
		}

		template < typename Range >
		requires (
			std::ranges::sized_range < Range > &&
			std::ranges::forward_range < Range > &&
			std::same_as < std::ranges::range_value_t < Range >, std::function < Type(const vertex &) > >)
		constexpr vertex &transform_in_place(const Range &transformFunctions)
		{
			if (std::ranges::size(transformFunctions) >= SpatialDimensionality::dimensionality())
			{
				throw std::out_of_range("\"transformFunctions.size() >= SpatialDimensionality::dimensionality()\"");
			}

			const auto vertex = *this;

			std::transform(std::ranges::cbegin(transformFunctions), std::ranges::cend(transformFunctions), this->coordinates.begin(),
				[&vertex](const std::function < Type(const geometry::vertex &) > &transformFunction) -> Type
				{
					return transformFunction(vertex);
				});

			return *this;
		}

		[[nodiscard]] constexpr vector < Type, SpatialDimensionality > vector_from_origin() const
		{
			return vector < Type, SpatialDimensionality > (this->coordinates);
		}

		[[nodiscard]] constexpr vector < Type, SpatialDimensionality > vector_to_origin() const
		{
			return -vector < Type, SpatialDimensionality > (this->coordinates);
		}

		constexpr vertex() noexcept = default;

		constexpr vertex(const vertex &) = default;

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() < SpatialDimensionality::dimensionality())
		constexpr vertex(const vertex < Type, OtherSpatialDimensionality > &vertex)
		{
			std::copy(
				vertex.coordinates.cbegin(),
				vertex.coordinates.cend(),
				this->coordinates.begin());
		}

		constexpr vertex(vertex &&) noexcept = default;

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() < SpatialDimensionality::dimensionality())
		constexpr vertex(vertex < Type, OtherSpatialDimensionality > &&vertex) noexcept
		{
			std::move(
				vertex.coordinates.cbegin(),
				vertex.coordinates.cend(),
				this->coordinates.begin());
		}

		template < size_t N >
		requires (N == SpatialDimensionality::dimensionality())
		constexpr vertex(const std::array < Type, N > &coordinates) :
			coordinates(coordinates)
		{
		}

		template < size_t N >
		requires (N < SpatialDimensionality::dimensionality())
		constexpr vertex(const std::array < Type, N > &coordinates)
		{
			std::copy(
				coordinates.cbegin(),
				coordinates.cend(),
				this->coordinates.begin());
		}

		template < size_t N >
		requires (N == SpatialDimensionality::dimensionality())
		constexpr vertex(std::array < Type, N > &&coordinates) noexcept :
			coordinates(std::move(coordinates))
		{
		}

		template < size_t N >
		requires (N < SpatialDimensionality::dimensionality())
		constexpr vertex(std::array < Type, N > &&coordinates) noexcept
		{
			std::move(
				coordinates.cbegin(),
				coordinates.cend(),
				this->coordinates.begin());
		}
	};

	template < concepts::geometric_assignable_arithmetic ... Type >
	requires (std::same_as < Type, typename std::common_type < Type... > ::type > && ...)
	constexpr auto make_vertex(Type ... values)
	{
		return
			vertex <
				typename std::common_type < Type... > ::type,
				spatial_dimension < sizeof...(Type) > >
				{
					std::to_array({ std::forward < typename std::common_type < Type... > ::type > (values)... })
				};
	}

	template <
		concepts::geometric_assignable_arithmetic Type,
		size_t N >
	constexpr vertex < Type, spatial_dimension < N > > make_vertex(const std::array < Type, N > &array)
	{
		return vertex < Type, spatial_dimension < N > > {array};
	}

	template <
		concepts::geometric_assignable_arithmetic Type,
		size_t N >
	constexpr vertex < Type, spatial_dimension < N > > make_vertex(std::array < Type, N > &&array)
	{
		return vertex < Type, spatial_dimension < N > > {std::move(array)};
	}
}

export namespace xablau::algebra::functions
{
	template <
		xablau::geometry::concepts::geometric_assignable_arithmetic Type,
		xablau::geometry::concepts::spatial_dimension SpatialDimensionality >
	struct almost_equal < xablau::geometry::vertex < Type, SpatialDimensionality > >
	{
		template < size_t UnitsInTheLastPlace >
		[[nodiscard]] bool operator()(
			const xablau::geometry::vertex < Type, SpatialDimensionality > &value1,
			const xablau::geometry::vertex < Type, SpatialDimensionality > &value2) const
		{
			return almost_equal::invoke < UnitsInTheLastPlace > (value1, value2);
		}

		[[nodiscard]] bool operator()(
			const xablau::geometry::vertex < Type, SpatialDimensionality > &value1,
			const xablau::geometry::vertex < Type, SpatialDimensionality > &value2,
			const size_t unitsInTheLastPlace) const
		{
			return almost_equal::invoke(value1, value2, unitsInTheLastPlace);
		}

		template < size_t UnitsInTheLastPlace >
		[[nodiscard]] static bool invoke(
			const xablau::geometry::vertex < Type, SpatialDimensionality > &value1,
			const xablau::geometry::vertex < Type, SpatialDimensionality > &value2)
		{
			const auto &coordinates1 = value1.coordinates;
			const auto &coordinates2 = value2.coordinates;

			for (typename xablau::geometry::vertex < Type, SpatialDimensionality > ::size_type i{};
				i < SpatialDimensionality::dimensionality();
				i++)
			{
				if (!xablau::algebra::functions::almost_equal < Type > ::invoke < UnitsInTheLastPlace > (coordinates1[i], coordinates2[i]))
				{
					return false;
				}
			}

			return true;
		}

		[[nodiscard]] static bool invoke(
			const xablau::geometry::vertex < Type, SpatialDimensionality > &value1,
			const xablau::geometry::vertex < Type, SpatialDimensionality > &value2,
			const size_t unitsInTheLastPlace)
		{
			const auto &coordinates1 = value1.coordinates;
			const auto &coordinates2 = value2.coordinates;

			for (typename xablau::geometry::vertex < Type, SpatialDimensionality > ::size_type i{};
				i < SpatialDimensionality::dimensionality();
				i++)
			{
				if (!xablau::algebra::functions::almost_equal < Type > ::invoke(coordinates1[i], coordinates2[i], unitsInTheLastPlace))
				{
					return false;
				}
			}

			return true;
		}
	};
}

export namespace std
{
	template <
		xablau::geometry::concepts::geometric_assignable_arithmetic Type,
		xablau::geometry::concepts::spatial_dimension SpatialDimensionality >
	struct hash < xablau::geometry::vertex < Type, SpatialDimensionality > >
	{
		size_t operator()(const xablau::geometry::vertex < Type, SpatialDimensionality > &vertex) const
		{
			size_t hash{};

			auto beginIterator = vertex.coordinates.crbegin();
			const auto endIterator = vertex.coordinates.crend();

			while (beginIterator != endIterator)
			{
				if (*beginIterator != Type{})
				{
					break;
				}

				++beginIterator;
			}

			for (; beginIterator != endIterator; ++beginIterator)
			{
				hash ^= std::rotl(
					std::hash < Type > {}(*beginIterator),
					static_cast < int > (std::distance(beginIterator, endIterator)));
			}

			return hash;
		}
	};
}