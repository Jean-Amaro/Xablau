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

export module xablau.geometry:segment;
export import :forward_declarations;

export import <algorithm>;
export import <functional>;
export import <iostream>;
export import <optional>;

export import xablau.algebra;

export namespace xablau::geometry
{
	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality >
	class segment final
	{
	public:
		using value_type = Type;

		using abs_type = typename xablau::algebra::abs_type < Type > ::type;

		using size_type = typename vertex < Type, SpatialDimensionality > ::size_type;

		vertex < Type, SpatialDimensionality > vertex1;

		vertex < Type, SpatialDimensionality > vertex2;

	private:
		template < concepts::spatial_dimension OtherSpatialDimensionality >
		abs_type distance_to(
			const segment < Type, OtherSpatialDimensionality > &segment,
			vector < Type, SpatialDimensionality > &u,
			vector < Type, OtherSpatialDimensionality > &v) const
		{
			u = this->vertex2 - this->vertex1;
			v = segment.vertex2 - segment.vertex1;
			const auto w = this->vertex1 - segment.vertex1;
			const auto a = u.scalar_product(u);
			const auto b = u.scalar_product(v);
			const auto c = v.scalar_product(v);
			const auto d = u.scalar_product(w);
			const auto e = v.scalar_product(w);
			const auto determinant = a * c - b * b;
			abs_type sNumerator{};
			auto sDenominator = determinant;
			abs_type tNumerator{};
			auto tDenominator = determinant;

			if (determinant == abs_type{})
			{
				sDenominator = abs_type{1};
				tNumerator = e;
				tDenominator = c;
			}

			else
			{
				sNumerator = (b * e - c * d);
				tNumerator = (a * e - b * d);

				if (sNumerator < abs_type{})
				{
					sNumerator = abs_type{};
					tNumerator = e;
					tDenominator = c;
				}

				else if (sNumerator > sDenominator)
				{
					sNumerator = sDenominator;
					tNumerator = e + b;
					tDenominator = c;
				}
			}

			if (tNumerator < abs_type{})
			{
				tNumerator = abs_type{};

				if (-d < abs_type{})
				{
					sNumerator = abs_type{};
				}

				else if (-d > a)
				{
					sNumerator = sDenominator;
				}

				else
				{
					sNumerator = -d;
					sDenominator = a;
				}
			}

			else if (tNumerator > tDenominator)
			{
				const auto mdb = -d + b;
				tNumerator = tDenominator;

				if (mdb < abs_type{})
				{
					sNumerator = abs_type{};
				}

				else if (mdb > a)
				{
					sNumerator = sDenominator;
				}

				else
				{
					sNumerator = mdb;
					sDenominator = a;
				}
			}

			const auto s = sNumerator / sDenominator;
			const auto t = tNumerator / tDenominator;

			u *= s;
			v *= t;

			return (w + u - v).norm();
		}

	public:
		[[nodiscard]] explicit constexpr operator bool() const
		{
			return !this->null();
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr bool operator<(const segment < Type, OtherSpatialDimensionality > &segment) const
		{
			const bool thisDegenerate = this->degenerate();
			const bool otherDegenerate = segment.degenerate();

			if (thisDegenerate && !otherDegenerate)
			{
				return true;
			}

			if (!thisDegenerate && otherDegenerate)
			{
				return false;
			}

			const auto &[min1, max1] = std::minmax(this->vertex1, this->vertex2);
			const auto &[min2, max2] = std::minmax(segment.vertex1, segment.vertex2);

			return (min1 < min2 ? true : (!(min2 < min1) ? max1 < max2 : false));
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr bool operator<=(const segment < Type, OtherSpatialDimensionality > &segment) const
		{
			using VertexType1 = vertex < Type, SpatialDimensionality >;
			using VertexType2 = vertex < Type, OtherSpatialDimensionality >;

			const bool thisDegenerate = this->degenerate();
			const bool otherDegenerate = segment.degenerate();

			if (thisDegenerate && !otherDegenerate)
			{
				return true;
			}

			if (!thisDegenerate && otherDegenerate)
			{
				return false;
			}

			const auto &[min1, max1] =
				this->vertex1 <= this->vertex2 ?
					std::pair < const VertexType1 &, const VertexType1 & > (this->vertex1, this->vertex2) :
					std::pair < const VertexType1 &, const VertexType1 & > (this->vertex2, this->vertex1);

			const auto &[min2, max2] =
				segment.vertex1 <= segment.vertex2 ?
					std::pair < const VertexType2 &, const VertexType2 & > (segment.vertex1, segment.vertex2) :
					std::pair < const VertexType2 &, const VertexType2 & > (segment.vertex2, segment.vertex1);

			return (!(min2 <= min1) ? true : (min1 <= min2 ? max1 <= max2 : false));
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr bool operator==(const segment < Type, OtherSpatialDimensionality > &segment) const
		{
			if (this->degenerate() ^ segment.degenerate())
			{
				return false;
			}

			return
				this->vertex1 == segment.vertex1 && this->vertex2 == segment.vertex2 ||
				this->vertex2 == segment.vertex1 && this->vertex1 == segment.vertex2;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr bool operator>=(const segment < Type, OtherSpatialDimensionality > &segment) const
		{
			using VertexType1 = vertex < Type, SpatialDimensionality >;
			using VertexType2 = vertex < Type, OtherSpatialDimensionality >;

			const bool thisDegenerate = this->degenerate();
			const bool otherDegenerate = segment.degenerate();

			if (thisDegenerate && !otherDegenerate)
			{
				return false;
			}

			if (!thisDegenerate && otherDegenerate)
			{
				return true;
			}

			const auto &[min1, max1] =
				this->vertex2 >= this->vertex1 ?
					std::pair < const VertexType1 &, const VertexType1 & > (this->vertex1, this->vertex2) :
					std::pair < const VertexType1 &, const VertexType1 & > (this->vertex2, this->vertex1);

			const auto &[min2, max2] =
				segment.vertex2 >= segment.vertex1 ?
					std::pair < const VertexType2 &, const VertexType2 & > (segment.vertex1, segment.vertex2) :
					std::pair < const VertexType2 &, const VertexType2 & > (segment.vertex2, segment.vertex1);

			return (!(min2 >= min1) ? true : (min1 >= min2 ? max1 >= max2 : false));
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr bool operator>(const segment < Type, OtherSpatialDimensionality > &segment) const
		{
			using VertexType1 = vertex < Type, SpatialDimensionality >;
			using VertexType2 = vertex < Type, OtherSpatialDimensionality >;

			const bool thisDegenerate = this->degenerate();
			const bool otherDegenerate = segment.degenerate();

			if (thisDegenerate && !otherDegenerate)
			{
				return false;
			}

			if (!thisDegenerate && otherDegenerate)
			{
				return true;
			}

			const auto &[min1, max1] =
				this->vertex2 > this->vertex1 ?
					std::pair < const VertexType1 &, const VertexType1 & > (this->vertex1, this->vertex2) :
					std::pair < const VertexType1 &, const VertexType1 & > (this->vertex2, this->vertex1);

			const auto &[min2, max2] =
				segment.vertex2 > segment.vertex1 ?
					std::pair < const VertexType2 &, const VertexType2 & > (segment.vertex1, segment.vertex2) :
					std::pair < const VertexType2 &, const VertexType2 & > (segment.vertex2, segment.vertex1);

			return (min1 > min2 ? true : (!(min2 > min1) ? max1 > max2 : false));
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr bool operator!=(const segment < Type, OtherSpatialDimensionality > &segment) const
		{
			if (this->degenerate() ^ segment.degenerate())
			{
				return true;
			}

			return
				(this->vertex1 != segment.vertex1 || this->vertex2 != segment.vertex2) &&
				(this->vertex2 != segment.vertex1 || this->vertex1 != segment.vertex2);
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr auto operator+(const vector < Type, OtherSpatialDimensionality > &vector) const
		{
			using ResultSpatialDimension =
				typename greater_spatial_dimension <
					SpatialDimensionality,
					OtherSpatialDimensionality > ::type;

			return
				segment < Type, ResultSpatialDimension > (
					this->vertex1 + vector,
					this->vertex2 + vector);
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr auto operator-(const vector < Type, OtherSpatialDimensionality > &vector) const
		{
			using ResultSpatialDimension =
				typename greater_spatial_dimension <
					SpatialDimensionality,
					OtherSpatialDimensionality > ::type;

			return
				segment < Type, ResultSpatialDimension > (
					this->vertex1 - vector,
					this->vertex2 - vector);
		}

		constexpr segment &operator=(const segment &) = default;

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() < SpatialDimensionality::dimensionality())
		constexpr segment &operator=(const segment < Type, OtherSpatialDimensionality > &segment)
		{
			this->vertex1 = segment.vertex1;
			this->vertex2 = segment.vertex2;

			return *this;
		}

		constexpr segment &operator=(segment &&) noexcept = default;

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() < SpatialDimensionality::dimensionality())
		constexpr segment &operator=(segment < Type, OtherSpatialDimensionality > &&segment) noexcept
		{
			this->vertex1 = std::move(segment.vertex1);
			this->vertex2 = std::move(segment.vertex2);

			return *this;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() <= SpatialDimensionality::dimensionality())
		constexpr segment &operator+=(const vector < Type, OtherSpatialDimensionality > &vector)
		{
			this->vertex1 += vector;
			this->vertex2 += vector;

			return *this;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() <= SpatialDimensionality::dimensionality())
		constexpr segment &operator-=(const vector < Type, OtherSpatialDimensionality > &vector)
		{
			this->vertex1 -= vector;
			this->vertex2 -= vector;

			return *this;
		}

		[[nodiscard]] constexpr bool null() const
		{
			return this->vertex1 == this->vertex2;
		}

		[[nodiscard]] constexpr bool degenerate() const
		{
			return this->null();
		}

		[[nodiscard]] abs_type distance() const
		{
			return this->vertex1.distance_to(this->vertex2);
		}

		// Based on http://geomalgorithms.com/a02-_lines.html
		template <
			bool CalculateClosestVertexOnSegment,
			concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] auto distance_to(const vertex < Type, OtherSpatialDimensionality > &vertex) const
		{
			const auto vector1 = this->create_vector();
			const auto vector2 = vertex - this->vertex1;
			const auto scalarProduct = vector2.scalar_product(vector1);

			if (scalarProduct <= abs_type{})
			{
				auto distance = vertex.distance_to(this->vertex1);

				if constexpr (CalculateClosestVertexOnSegment)
				{
					return std::make_pair(distance, this->vertex1);
				}

				else
				{
					return distance;
				}
			}

			const auto squaredLengthOfVector1 = vector1.scalar_product(vector1);

			if (scalarProduct >= squaredLengthOfVector1)
			{
				auto distance = vertex.distance_to(this->vertex2);

				if constexpr (CalculateClosestVertexOnSegment)
				{
					return std::make_pair(distance, this->vertex2);
				}

				else
				{
					return distance;
				}
			}

			const auto scalarFactor = scalarProduct / squaredLengthOfVector1;
			auto vertexOnSegment = this->vertex1 + (scalarFactor * vector1);
			auto distance = vertex.distance_to(vertexOnSegment);

			if constexpr (CalculateClosestVertexOnSegment)
			{
				return std::make_pair(distance, std::move(vertexOnSegment));
			}

			else
			{
				return distance;
			}
		}

		// Based on http://geomalgorithms.com/a07-_distance.html
		template <
			bool CalculateClosestVerticesOnSegments,
			concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] auto distance_to(const segment < Type, OtherSpatialDimensionality > &segment) const
		{
			vector < Type, SpatialDimensionality > u;
			vector < Type, OtherSpatialDimensionality > v;

			auto dP = this->distance_to(segment, u, v);

			if constexpr (CalculateClosestVerticesOnSegments)
			{
				return std::make_tuple(std::move(dP), this->vertex1 + u, segment.vertex1 + v);
			}

			else
			{
				return dP;
			}
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool contains(
			const vertex < Type, OtherSpatialDimensionality > &vertex,
			const size_t unitsInTheLastPlace = 1) const
		{
			const auto segmentDistance = this->distance();
			const auto partialDistance1 = (this->vertex2 - vertex).norm();
			const auto partialDistance2 = (vertex - this->vertex1).norm();

			return
				xablau::algebra::functions::almost_equal < abs_type > ::invoke(
					segmentDistance,
					partialDistance1 + partialDistance2,
					unitsInTheLastPlace);
		}

		template <
			bool CalculateVertexIntersection,
			concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] auto intersects(
			const segment < Type, OtherSpatialDimensionality > &segment,
			const size_t unitsInTheLastPlace = 1) const
		{
			if constexpr (CalculateVertexIntersection)
			{
				vector < Type, SpatialDimensionality > u;
				vector < Type, OtherSpatialDimensionality > v;
				std::optional < xablau::geometry::vertex < Type, SpatialDimensionality > > result{};

				if (xablau::algebra::functions::almost_equal < abs_type > ::invoke(
						this->distance_to(segment, u, v),
						abs_type{},
						unitsInTheLastPlace))
				{
					result = this->vertex1 + u;
				}

				return result;
			}

			else
			{
				return
					xablau::algebra::functions::almost_equal < abs_type > ::invoke(
						this->distance_to < false > (segment),
						abs_type{},
						unitsInTheLastPlace);
			}
		}

		// https://www.sciencedirect.com/science/article/pii/S0925772109001448
		template <
			bool CalculateIntersection,
			concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() <= 3)
		[[nodiscard]] constexpr auto intersects(
			const geometry::euclidean_triangle < Type, OtherSpatialDimensionality > &triangle,
			const size_t unitsInTheLastPlace = 1) const
		requires (SpatialDimensionality::dimensionality() <= 3)
		{
			return triangle.template intersects < CalculateIntersection > (*this, unitsInTheLastPlace);
		}

		[[nodiscard]] constexpr vector < Type, SpatialDimensionality > create_vector() const
		{
			return this->vertex2 - this->vertex1;
		}

		[[nodiscard]] constexpr line < Type, SpatialDimensionality > create_line() const
		{
			return line < Type, SpatialDimensionality > (
				this->vertex1,
				this->create_vector());
		}

		[[nodiscard]] static consteval size_type dimensionality() noexcept
		{
			return SpatialDimensionality::dimensionality();
		}

		friend std::ostream &operator<<(std::ostream &stream, const segment &segment)
		{
			stream << "*--* ( " << segment.vertex1 << ", " << segment.vertex2 << " )";

			return stream;
		}

		constexpr segment() noexcept = default;

		constexpr segment(const segment &) = default;

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() < SpatialDimensionality::dimensionality())
		constexpr segment(const segment < Type, OtherSpatialDimensionality > &segment) :
			vertex1(segment.vertex1),
			vertex2(segment.vertex2)
		{
		}

		constexpr segment(segment &&) noexcept = default;

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() < SpatialDimensionality::dimensionality())
		constexpr segment(segment < Type, OtherSpatialDimensionality > &&segment) noexcept :
			vertex1(std::move(segment.vertex1)),
			vertex2(std::move(segment.vertex2))
		{
		}

		template <
			concepts::spatial_dimension OtherSpatialDimensionality1,
			concepts::spatial_dimension OtherSpatialDimensionality2 >
		requires (
			OtherSpatialDimensionality1::dimensionality() <= SpatialDimensionality::dimensionality() &&
			OtherSpatialDimensionality2::dimensionality() <= SpatialDimensionality::dimensionality())
		constexpr segment(
			const vertex < Type, OtherSpatialDimensionality1 > &vertex1,
			const vertex < Type, OtherSpatialDimensionality2 > &vertex2) :

			vertex1(vertex1),
			vertex2(vertex2)
		{
		}

		template <
			concepts::spatial_dimension OtherSpatialDimensionality1,
			concepts::spatial_dimension OtherSpatialDimensionality2 >
		requires (
			OtherSpatialDimensionality1::dimensionality() <= SpatialDimensionality::dimensionality() &&
			OtherSpatialDimensionality2::dimensionality() <= SpatialDimensionality::dimensionality())
		constexpr segment(
			vertex < Type, OtherSpatialDimensionality1 > &&vertex1,
			vertex < Type, OtherSpatialDimensionality2 > &&vertex2) noexcept :

			vertex1(std::move(vertex1)),
			vertex2(std::move(vertex2))
		{
		}
	};

	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality1,
		concepts::spatial_dimension SpatialDimensionality2 >
	constexpr auto operator+(
		const vector < Type, SpatialDimensionality1 > &vector,
		const segment < Type, SpatialDimensionality2 > &segment)
	{
		return segment + vector;
	}

	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality1,
		concepts::spatial_dimension SpatialDimensionality2 >
	constexpr auto operator-(
		const vector < Type, SpatialDimensionality1 > &vector,
		const segment < Type, SpatialDimensionality2 > &segment)
	{
		return segment - vector;
	}

	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality1,
		concepts::spatial_dimension SpatialDimensionality2 >
	constexpr auto make_segment(
		const vertex < Type, SpatialDimensionality1 > &vertex1,
		const vertex < Type, SpatialDimensionality2 > &vertex2)
	{
		if constexpr (SpatialDimensionality1::dimensionality() >= SpatialDimensionality2::dimensionality())
		{
			return segment < Type, SpatialDimensionality1 > {vertex1, vertex2};
		}

		else
		{
			return segment < Type, SpatialDimensionality2 > {vertex1, vertex2};
		}
	}

	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality1,
		concepts::spatial_dimension SpatialDimensionality2 >
	constexpr auto make_segment(
		vertex < Type, SpatialDimensionality1 > &&vertex1,
		vertex < Type, SpatialDimensionality2 > &&vertex2)
	{
		if constexpr (SpatialDimensionality1::dimensionality() >= SpatialDimensionality2::dimensionality())
		{
			return segment < Type, SpatialDimensionality1 > {std::move(vertex1), std::move(vertex2)};
		}

		else
		{
			return segment < Type, SpatialDimensionality2 > {std::move(vertex1), std::move(vertex2)};
		}
	}
}

export namespace xablau::algebra::functions
{
	template <
		xablau::geometry::concepts::geometric_assignable_arithmetic Type,
		xablau::geometry::concepts::spatial_dimension SpatialDimensionality >
	struct almost_equal < xablau::geometry::segment < Type, SpatialDimensionality > >
	{
		template < size_t UnitsInTheLastPlace >
		[[nodiscard]] bool operator()(
			const xablau::geometry::segment < Type, SpatialDimensionality > &value1,
			const xablau::geometry::segment < Type, SpatialDimensionality > &value2) const
		{
			return almost_equal::invoke < UnitsInTheLastPlace > (value1, value2);
		}

		[[nodiscard]] bool operator()(
			const xablau::geometry::segment < Type, SpatialDimensionality > &value1,
			const xablau::geometry::segment < Type, SpatialDimensionality > &value2,
			const size_t unitsInTheLastPlace) const
		{
			return almost_equal::invoke(value1, value2, unitsInTheLastPlace);
		}

		template < size_t UnitsInTheLastPlace >
		[[nodiscard]] static bool invoke(
			const xablau::geometry::segment < Type, SpatialDimensionality > &value1,
			const xablau::geometry::segment < Type, SpatialDimensionality > &value2)
		{
			using VertexType = xablau::geometry::vertex < Type, SpatialDimensionality >;

			if (value1.degenerate() ^ value2.degenerate())
			{
				return false;
			}

			return
				(almost_equal < VertexType > ::invoke < UnitsInTheLastPlace > (value1.vertex1, value2.vertex1) &&
					almost_equal < VertexType > ::invoke < UnitsInTheLastPlace > (value1.vertex2, value2.vertex2)) ||

				(almost_equal < VertexType > ::invoke < UnitsInTheLastPlace > (value1.vertex2, value2.vertex1) &&
					almost_equal < VertexType > ::invoke < UnitsInTheLastPlace > (value1.vertex1, value2.vertex2));
		}

		[[nodiscard]] static bool invoke(
			const xablau::geometry::segment < Type, SpatialDimensionality > &value1,
			const xablau::geometry::segment < Type, SpatialDimensionality > &value2,
			const size_t unitsInTheLastPlace)
		{
			using VertexType = xablau::geometry::vertex < Type, SpatialDimensionality >;

			if (value1.degenerate() ^ value2.degenerate())
			{
				return false;
			}

			return
				(almost_equal < VertexType > ::invoke(value1.vertex1, value2.vertex1, unitsInTheLastPlace) &&
					almost_equal < VertexType > ::invoke(value1.vertex2, value2.vertex2, unitsInTheLastPlace)) ||

				(almost_equal < VertexType > ::invoke(value1.vertex2, value2.vertex1, unitsInTheLastPlace) &&
					almost_equal < VertexType > ::invoke(value1.vertex1, value2.vertex2, unitsInTheLastPlace));
		}
	};
}

export namespace std
{
	template <
		xablau::geometry::concepts::geometric_assignable_arithmetic Type,
		xablau::geometry::concepts::spatial_dimension SpatialDimensionality >
	struct hash < xablau::geometry::segment < Type, SpatialDimensionality > >
	{
		size_t operator()(const xablau::geometry::segment < Type, SpatialDimensionality > &segment) const
		{
			return
				std::hash < xablau::geometry::vertex < Type, SpatialDimensionality > > ()(segment.vertex1) ^
				std::hash < xablau::geometry::vertex < Type, SpatialDimensionality > > ()(segment.vertex2);
		}
	};
}