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

export module xablau.geometry:euclidean_triangle;
export import :forward_declarations;

export import std;

export import xablau.algebra;

export namespace xablau::geometry
{
	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality >
	class euclidean_triangle final
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

		template < typename SegmentType >
		[[nodiscard]] constexpr euclidean_triangle_intersection_result intersects(
			const SegmentType &segment,
			abs_type &t_param,
			const size_t unitsInTheLastPlace) const
		requires (SpatialDimensionality::dimensionality() <= 3)
		{
			const auto a = segment.vertex1 - this->vertex3;
			const auto b = this->vertex1 - this->vertex3;
			const auto c = this->vertex2 - this->vertex3;
			const auto w1 = b.template vector_product < vector_product_dimension::three > (c);
			const auto w = a.scalar_product(w1);
			const auto d = segment.vertex2 - this->vertex3;
			const auto s = d.scalar_product(w1);
			const auto w2 = a.template vector_product < vector_product_dimension::three > (d);
			const auto t = w2.scalar_product(c);
			const auto u = -w2.scalar_product(b);

			const bool wCloseToZero =
				xablau::algebra::functions::almost_equal < abs_type > ::invoke(w, abs_type{}, unitsInTheLastPlace);

			const bool sCloseToZero =
				xablau::algebra::functions::almost_equal < abs_type > ::invoke(s, abs_type{}, unitsInTheLastPlace);

			const bool tCloseToZero =
				xablau::algebra::functions::almost_equal < abs_type > ::invoke(t, abs_type{}, unitsInTheLastPlace);

			const bool uCloseToZero =
				xablau::algebra::functions::almost_equal < abs_type > ::invoke(u, abs_type{}, unitsInTheLastPlace);

			const bool stuCloseToZero =
				xablau::algebra::functions::almost_equal < abs_type > ::invoke(s + t + u, abs_type{}, unitsInTheLastPlace);

			const bool wstuCloseToZero =
				xablau::algebra::functions::almost_equal < abs_type > ::invoke(w - (s + t + u), abs_type{}, unitsInTheLastPlace);

			if (!wCloseToZero && w > abs_type{})
			{
				if (!sCloseToZero && s > abs_type{} ||
					!tCloseToZero && t < abs_type{} ||
					!uCloseToZero && u < abs_type{} ||
					!wstuCloseToZero && (s + t + u) - w > abs_type{})
				{
					return euclidean_triangle_intersection_result::does_not_intersect;
				}
			}

			else if (!wCloseToZero && w < abs_type{})
			{
				if (!sCloseToZero && s < abs_type{} ||
					!tCloseToZero && t > abs_type{} ||
					!uCloseToZero && u > abs_type{} ||
					!wstuCloseToZero && w - (s + t + u) > abs_type{})
				{
					return euclidean_triangle_intersection_result::does_not_intersect;
				}
			}

			else
			{
				if (!sCloseToZero && s > abs_type{})
				{
					if (!tCloseToZero && t < abs_type{} ||
						!uCloseToZero && u < abs_type{} ||
						!stuCloseToZero && (s + t + u) > abs_type{})
					{
						return euclidean_triangle_intersection_result::does_not_intersect;
					}
				}

				else if (!sCloseToZero && s < abs_type{})
				{
					if (!tCloseToZero && t > abs_type {} ||
						!uCloseToZero && u > abs_type{} ||
						!stuCloseToZero && -(s + t + u) > abs_type{})
					{
						return euclidean_triangle_intersection_result::does_not_intersect;
					}
				}

				else
				{
					return euclidean_triangle_intersection_result::coplanar;
				}
			}

			t_param = -(w / (s - w));

			return euclidean_triangle_intersection_result::intersects;
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

		template <
			bool EqualityOperator,
			typename VertexType1,
			typename VertexType2 >
		static bool comparison_function(
			const VertexType1 &vertex1,
			const VertexType2 &vertex2,
			bool &alreadyCompared,
			bool &result)
		{
			if (!alreadyCompared)
			{
				alreadyCompared = true;

				if constexpr (EqualityOperator)
				{
					result = (vertex1 == vertex2);
				}

				else
				{
					result = !(vertex1 != vertex2);
				}
			}

			return result;
		}

		template <
			bool EqualityOperator,
			typename TriangleType >
		bool equal_to(const TriangleType &triangle) const
		{
			std::array < bool, 9 > comparisons{}; 
			std::array < bool, 9 > results{}; 

			return
				euclidean_triangle::comparison_function < EqualityOperator > (this->vertex1, triangle.vertex1, comparisons[0], results[0]) &&
					(euclidean_triangle::comparison_function < EqualityOperator > (this->vertex2, triangle.vertex2, comparisons[4], results[4]) &&
					euclidean_triangle::comparison_function < EqualityOperator > (this->vertex3, triangle.vertex3, comparisons[8], results[8]) ||

					euclidean_triangle::comparison_function < EqualityOperator > (this->vertex2, triangle.vertex3, comparisons[5], results[5]) &&
					euclidean_triangle::comparison_function < EqualityOperator > (this->vertex3, triangle.vertex2, comparisons[7], results[7])) ||

				euclidean_triangle::comparison_function < EqualityOperator > (this->vertex1, triangle.vertex2, comparisons[1], results[1]) &&
					(euclidean_triangle::comparison_function < EqualityOperator > (this->vertex2, triangle.vertex1, comparisons[3], results[3]) &&
					euclidean_triangle::comparison_function < EqualityOperator > (this->vertex3, triangle.vertex3, comparisons[8], results[8]) ||
				
					euclidean_triangle::comparison_function < EqualityOperator > (this->vertex2, triangle.vertex3, comparisons[5], results[5]) &&
					euclidean_triangle::comparison_function < EqualityOperator > (this->vertex3, triangle.vertex1, comparisons[6], results[6])) ||

				euclidean_triangle::comparison_function < EqualityOperator > (this->vertex1, triangle.vertex3, comparisons[2], results[2]) &&
					(euclidean_triangle::comparison_function < EqualityOperator > (this->vertex2, triangle.vertex1, comparisons[3], results[3]) &&
					euclidean_triangle::comparison_function < EqualityOperator > (this->vertex3, triangle.vertex2, comparisons[7], results[7]) ||

					euclidean_triangle::comparison_function < EqualityOperator > (this->vertex2, triangle.vertex2, comparisons[4], results[4]) &&
					euclidean_triangle::comparison_function < EqualityOperator > (this->vertex3, triangle.vertex1, comparisons[6], results[6]));
		}

		template <
			comparison_operator ComparisonOperator,
			concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] auto sort_vertices(const euclidean_triangle < Type, OtherSpatialDimensionality > &triangle) const
		{
			std::array < std::reference_wrapper < const vertex < Type, SpatialDimensionality > >, 3 > thisSortedVertices
				{
					this->vertex1,
					this->vertex2,
					this->vertex3
				};

			std::array < std::reference_wrapper < const vertex < Type, OtherSpatialDimensionality > >, 3 > otherSortedVertices
				{
					triangle.vertex1,
					triangle.vertex2,
					triangle.vertex3
				};

			std::ranges::sort(thisSortedVertices,
				[] (const std::reference_wrapper < const vertex < Type, SpatialDimensionality > > &vertex1,
					const std::reference_wrapper < const vertex < Type, SpatialDimensionality > > &vertex2) -> bool
				{
					return euclidean_triangle::less_than < ComparisonOperator > (vertex1.get(), vertex2.get());
				});

			std::ranges::sort(otherSortedVertices,
				[] (const std::reference_wrapper < const vertex < Type, OtherSpatialDimensionality > > &vertex1,
					const std::reference_wrapper < const vertex < Type, OtherSpatialDimensionality > > &vertex2) -> bool
				{
					return euclidean_triangle::less_than < ComparisonOperator > (vertex1.get(), vertex2.get());
				});

			return std::make_pair(std::move(thisSortedVertices), std::move(otherSortedVertices));
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		static void check_degeneracy(const euclidean_triangle < Type, OtherSpatialDimensionality > &triangle)
		{
			if (triangle.degenerate())
			{
				throw std::domain_error("\"triangle\" is degenerate.");
			}
		}

	public:
		vertex < Type, SpatialDimensionality > vertex1;

		vertex < Type, SpatialDimensionality > vertex2;

		vertex < Type, SpatialDimensionality > vertex3;

		[[nodiscard]] explicit constexpr operator bool() const
		{
			return !this->degenerate();
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr bool operator<(const euclidean_triangle < Type, OtherSpatialDimensionality > &triangle) const
		{
			const bool thisDegenerate = this->degenerate();
			const bool otherDegenerate = triangle.degenerate();

			if (thisDegenerate && !otherDegenerate)
			{
				return true;
			}

			if (!thisDegenerate && otherDegenerate)
			{
				return false;
			}

			const auto [thisSortedVertices, otherSortedVertices] =
				this->sort_vertices < comparison_operator::less > (triangle);

			if (thisSortedVertices[0].get() < otherSortedVertices[0].get())
			{
				return true;
			}

			if (!(otherSortedVertices[0].get() < thisSortedVertices[0].get()))
			{
				if (thisSortedVertices[1].get() < otherSortedVertices[1].get())
				{
					return true;
				}

				if (!(otherSortedVertices[1].get() < thisSortedVertices[1].get()))
				{
					return thisSortedVertices[2].get() < otherSortedVertices[2].get();
				}
			}

			return false;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr bool operator<=(const euclidean_triangle < Type, OtherSpatialDimensionality > &triangle) const
		{
			const bool thisDegenerate = this->degenerate();
			const bool otherDegenerate = triangle.degenerate();

			if (thisDegenerate && !otherDegenerate)
			{
				return true;
			}

			if (!thisDegenerate && otherDegenerate)
			{
				return false;
			}

			const auto [thisSortedVertices, otherSortedVertices] =
				this->sort_vertices < comparison_operator::less_equal > (triangle);

			if (!(otherSortedVertices[0].get() <= thisSortedVertices[0].get()))
			{
				return true;
			}

			if (thisSortedVertices[0].get() <= otherSortedVertices[0].get())
			{
				if (!(otherSortedVertices[1].get() <= thisSortedVertices[1].get()))
				{
					return true;
				}

				if (thisSortedVertices[1].get() <= otherSortedVertices[1].get())
				{
					return thisSortedVertices[2].get() <= otherSortedVertices[2].get();
				}
			}

			return false;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr bool operator==(const euclidean_triangle < Type, OtherSpatialDimensionality > &triangle) const
		{
			const bool thisDegenerate = this->degenerate();
			const bool otherDegenerate = triangle.degenerate();

			if (thisDegenerate ^ otherDegenerate)
			{
				return false;
			}

			return this->equal_to < true > (triangle);
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr bool operator>=(const euclidean_triangle < Type, OtherSpatialDimensionality > &triangle) const
		{
			const bool thisDegenerate = this->degenerate();
			const bool otherDegenerate = triangle.degenerate();

			if (thisDegenerate && !otherDegenerate)
			{
				return false;
			}

			if (!thisDegenerate && otherDegenerate)
			{
				return true;
			}

			const auto [thisSortedVertices, otherSortedVertices] =
				this->sort_vertices < comparison_operator::greater_equal > (triangle);

			if (!(otherSortedVertices[0].get() >= thisSortedVertices[0].get()))
			{
				return true;
			}

			if (thisSortedVertices[0].get() >= otherSortedVertices[0].get())
			{
				if (!(otherSortedVertices[1].get() >= thisSortedVertices[1].get()))
				{
					return true;
				}

				if (thisSortedVertices[1].get() >= otherSortedVertices[1].get())
				{
					return thisSortedVertices[2].get() >= otherSortedVertices[2].get();
				}
			}

			return false;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr bool operator>(const euclidean_triangle < Type, OtherSpatialDimensionality > &triangle) const
		{
			const bool thisDegenerate = this->degenerate();
			const bool otherDegenerate = triangle.degenerate();

			if (thisDegenerate && !otherDegenerate)
			{
				return false;
			}

			if (!thisDegenerate && otherDegenerate)
			{
				return true;
			}

			const auto [thisSortedVertices, otherSortedVertices] =
				this->sort_vertices < comparison_operator::greater > (triangle);

			if (thisSortedVertices[0].get() > otherSortedVertices[0].get())
			{
				return true;
			}

			if (!(otherSortedVertices[0].get() > thisSortedVertices[0].get()))
			{
				if (thisSortedVertices[1].get() > otherSortedVertices[1].get())
				{
					return true;
				}

				if (!(otherSortedVertices[1].get() > thisSortedVertices[1].get()))
				{
					return thisSortedVertices[2].get() > otherSortedVertices[2].get();
				}
			}

			return false;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr bool operator!=(const euclidean_triangle < Type, OtherSpatialDimensionality > &triangle) const
		{
			const bool thisDegenerate = this->degenerate();
			const bool otherDegenerate = triangle.degenerate();

			if (thisDegenerate ^ otherDegenerate)
			{
				return true;
			}

			return !this->equal_to < false > (triangle);
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr auto operator+(const vector < Type, OtherSpatialDimensionality > &vector) const
		{
			using ResultSpatialDimension =
				typename greater_spatial_dimension <
					SpatialDimensionality,
					OtherSpatialDimensionality > ::type;

			return
				euclidean_triangle < Type, ResultSpatialDimension > (
					this->vertex1 + vector,
					this->vertex2 + vector,
					this->vertex3 + vector);
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr auto operator-(const vector < Type, OtherSpatialDimensionality > &vector) const
		{
			using ResultSpatialDimension =
				typename greater_spatial_dimension <
					SpatialDimensionality,
					OtherSpatialDimensionality > ::type;

			return
				euclidean_triangle < Type, ResultSpatialDimension > (
					this->vertex1 - vector,
					this->vertex2 - vector,
					this->vertex3 - vector);
		}

		constexpr euclidean_triangle &operator=(const euclidean_triangle &) = default;

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() < SpatialDimensionality::dimensionality())
		constexpr euclidean_triangle &operator=(const euclidean_triangle < Type, OtherSpatialDimensionality > &triangle)
		{
			this->vertex1 = triangle.vertex1;
			this->vertex2 = triangle.vertex2;
			this->vertex3 = triangle.vertex3;

			return *this;
		}

		constexpr euclidean_triangle &operator=(euclidean_triangle &&) noexcept = default;

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() < SpatialDimensionality::dimensionality())
		constexpr euclidean_triangle &operator=(euclidean_triangle < Type, OtherSpatialDimensionality > &&triangle) noexcept
		{
			this->vertex1 = std::move(triangle.vertex1);
			this->vertex2 = std::move(triangle.vertex2);
			this->vertex3 = std::move(triangle.vertex3);

			return *this;
		}

		[[nodiscard]] constexpr vertex < Type, SpatialDimensionality > &operator[](const size_type index)
		{
			switch (index)
			{
			case 0:
				return this->vertex1;
			case 1:
				return this->vertex2;
			case 2:
				return this->vertex3;
			default:
				throw std::domain_error("\"index\" must be [0, 2].");
			}
		}

		[[nodiscard]] constexpr const vertex < Type, SpatialDimensionality > &operator[](const size_type index) const
		{
			switch (index)
			{
			case 0:
				return this->vertex1;
			case 1:
				return this->vertex2;
			case 2:
				return this->vertex3;
			default:
				throw std::domain_error("\"index\" must be [0, 2].");
			}
		}

		[[nodiscard]] abs_type distance_edge1() const
		{
			return this->vertex1.distance_to(this->vertex2);
		}

		[[nodiscard]] abs_type distance_edge2() const
		{
			return this->vertex2.distance_to(this->vertex3);
		}

		[[nodiscard]] abs_type distance_edge3() const
		{
			return this->vertex3.distance_to(this->vertex1);
		}

		[[nodiscard]] abs_type angle1() const
		{
			return this->vertex1.angle(this->vertex2, this->vertex3);
		}

		[[nodiscard]] abs_type angle2() const
		{
			return this->vertex2.angle(this->vertex1, this->vertex3);
		}

		[[nodiscard]] abs_type angle3() const
		{
			return this->vertex3.angle(this->vertex1, this->vertex2);
		}

		[[nodiscard]] abs_type angle(const size_type index) const
		{
			switch (index)
			{
			case 0:
				return this->angle1();
			case 1:
				return this->angle2();
			case 2:
				return this->angle3();
			default:
				throw std::domain_error("\"index\" must be [0, 2].");
			}
		}

		friend std::ostream &operator<<(std::ostream &stream, const euclidean_triangle &triangle)
		{
			stream << "/_\\ ( " << triangle.vertex1 << ", " << triangle.vertex2 << ", " <<  triangle.vertex3 << " )";

			return stream;
		}

		[[nodiscard]] static consteval size_t dimensionality() noexcept
		{
			return SpatialDimensionality::dimensionality();
		}
		
		[[nodiscard]] abs_type area() const
		{
			std::array < abs_type, 3 > distances{
					this->distance_edge1(),
					this->distance_edge2(),
					this->distance_edge3()
				};

			std::ranges::sort(distances);

			const auto &a = distances[2];
			const auto &b = distances[1];
			const auto &c = distances[0];

			const auto numerator =
				(a + (b + c)) * (c - (a - b)) * (c + (a - b)) * (a + (b - c));

			if constexpr (xablau::algebra::concepts::standard_arithmetic < Type >)
			{
				if (numerator < abs_type{})
				{
					return abs_type{};
				}
			}

			return
				xablau::algebra::functions::sqrt < abs_type > ::invoke(numerator) /
					abs_type{4};
		}

		[[nodiscard]] abs_type perimeter() const
		{
			return this->distance_edge1() + this->distance_edge2() + this->distance_edge3();
		}

		[[nodiscard]] vertex < Type, SpatialDimensionality > incenter() const
		{
			euclidean_triangle::check_degeneracy(*this);

			const auto perimeter = this->perimeter();
			const auto distance_edge1 = this->distance_edge1();
			const auto distance_edge3 = this->distance_edge3();
			const auto distance_edge2 = this->distance_edge2();

			vertex < Type, SpatialDimensionality > incenter;

			for (size_type i = 0; i < incenter.dimensionality(); i++)
			{
				incenter.coordinates[i] =
					(distance_edge2 * this->vertex1.coordinates[i] +
					distance_edge3 * this->vertex2.coordinates[i] +
					distance_edge1 * this->vertex3.coordinates[i]) / perimeter;
			}

			return incenter;
		}

		[[nodiscard]] vertex < Type, SpatialDimensionality > orthocenter() const
		{
			constexpr auto _180Degrees = std::numbers::pi_v < abs_type >;

			const auto angle1 = this->angle1();

			if (angle1 == _180Degrees)
			{
				return this->vertex1;
			}

			const auto angle2 = this->angle2();

			if (angle2 == _180Degrees)
			{
				return this->vertex2;
			}

			const auto angle3 = this->angle3();

			if (angle3 == _180Degrees)
			{
				return this->vertex3;
			}

			const auto tangent1 = xablau::algebra::functions::tan < abs_type > ::invoke(angle1);
			const auto tangent2 = xablau::algebra::functions::tan < abs_type > ::invoke(angle2);
			const auto tangent3 = xablau::algebra::functions::tan < abs_type > ::invoke(angle3);
			const auto denominator = tangent1 + tangent2 + tangent3;

			vertex < Type, SpatialDimensionality > orthocenter;

			for (size_type i = 0; i < orthocenter.dimensionality(); i++)
			{
				orthocenter.coordinates[i] =
					(this->vertex1.coordinates[i] * tangent1 +
					this->vertex2.coordinates[i] * tangent2 +
					this->vertex3.coordinates[i] * tangent3) / denominator;
			}

			return orthocenter;
		}

		[[nodiscard]] vertex < Type, SpatialDimensionality > barycenter() const
		{
			vertex < Type, SpatialDimensionality > barycenter;

			for (size_type i = 0; i < barycenter.dimensionality(); i++)
			{
				barycenter.coordinates[i] =
					(this->vertex1.coordinates[i] + this->vertex2.coordinates[i] + this->vertex3.coordinates[i]) / abs_type{3};
			}

			return barycenter;
		}

		[[nodiscard]] vertex < Type, SpatialDimensionality > circumcenter() const
		{
			const auto sin1 = xablau::algebra::functions::sin < abs_type > ::invoke(abs_type{2} * this->angle1());
			const auto sin2 = xablau::algebra::functions::sin < abs_type > ::invoke(abs_type{2} * this->angle2());
			const auto sin3 = xablau::algebra::functions::sin < abs_type > ::invoke(abs_type{2} * this->angle3());
			const auto denominator = sin1 + sin2 + sin3;

			vertex < Type, SpatialDimensionality > circumcenter;

			for (size_type i = 0; i < circumcenter.dimensionality(); i++)
			{
				circumcenter.coordinates[i] =
					(this->vertex1.coordinates[i] * sin1 +
					this->vertex2.coordinates[i] * sin2 +
					this->vertex3.coordinates[i] * sin3) / denominator;
			}

			return circumcenter;
		}

		constexpr void reverse() noexcept
		{
			std::swap(this->vertex1, this->vertex2);
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool contains(
			const vertex < Type, OtherSpatialDimensionality > &vertex,
			const size_t unitsInTheLastPlace = 1) const
		{
			using ResultDimensionality =
				typename greater_spatial_dimension <
					SpatialDimensionality,
					OtherSpatialDimensionality > ::type;

			std::array < abs_type, 3 > areas{};

			euclidean_triangle < Type, ResultDimensionality > triangle(
				vertex,
				this->vertex1,
				this->vertex2);

			areas[0] = triangle.area();

			triangle.vertex3 = this->vertex3;

			areas[1] = triangle.area();

			triangle.vertex2 = this->vertex2;

			areas[2] = triangle.area();

			std::ranges::sort(areas);

			return
				xablau::algebra::functions::almost_equal < abs_type > ::invoke(
					this->area(),
					(areas[0] + areas[1]) + areas[2],
					unitsInTheLastPlace);
		}

		// https://www.sciencedirect.com/science/article/pii/S0925772109001448
		template <
			bool CalculateIntersection,
			concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() <= 3)
		[[nodiscard]] constexpr auto intersects(
			const geometry::segment < Type, OtherSpatialDimensionality > &segment,
			const size_t unitsInTheLastPlace = 1) const
		requires (SpatialDimensionality::dimensionality() <= 3)
		{
			abs_type t_param{};
			auto result = this->intersects(segment, t_param, unitsInTheLastPlace);

			if constexpr (CalculateIntersection)
			{
				using ResultDimensionality =
					typename greater_spatial_dimension <
						SpatialDimensionality,
						OtherSpatialDimensionality > ::type;

				using ResultType = vertex < Type, ResultDimensionality >;

				if (result == euclidean_triangle_intersection_result::intersects)
				{
					return
						std::make_pair(
							std::move(result),
							std::make_optional(
								ResultType(segment.vertex1 + (t_param * segment.create_vector()))));
				}

				return
					std::make_pair(
						std::move(result),
						std::optional < ResultType > {});
			}

			else
			{
				return result;
			}
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() <= SpatialDimensionality::dimensionality())
		constexpr euclidean_triangle &translate(const vector < Type, OtherSpatialDimensionality > &vector)
		{
			this->vertex1 += vector;
			this->vertex2 += vector;
			this->vertex3 += vector;

			return *this;
		}

		template < typename CoefficientType >
		requires (std::same_as < CoefficientType, Type > || std::same_as < CoefficientType, abs_type >)
		constexpr euclidean_triangle &scale_by(const CoefficientType &coefficient)
		{
			this->vertex1.scale_by(coefficient);
			this->vertex2.scale_by(coefficient);
			this->vertex3.scale_by(coefficient);

			return *this;
		}

		template < typename Range >
		requires (
			std::ranges::sized_range < Range > &&
			std::ranges::forward_range < Range > && (

				std::same_as < std::ranges::range_value_t < Range >, abs_type > ||
				std::same_as < std::ranges::range_value_t < Range >, Type >))
		constexpr euclidean_triangle &scale_by(const Range &coefficients)
		{
			this->vertex1.scale_by(coefficients);
			this->vertex2.scale_by(coefficients);
			this->vertex3.scale_by(coefficients);

			return *this;
		}

		[[nodiscard]] bool degenerate() const
		{
			return !(this->vertex1 - this->vertex2).linearly_independent_to(this->vertex3 - this->vertex2, 0);
		}

		[[nodiscard]] bool valid(const size_t unitsInTheLastPlace = 1) const
		{
			return (this->vertex1 - this->vertex2).linearly_independent_to(this->vertex3 - this->vertex2, unitsInTheLastPlace);
		}

		constexpr euclidean_triangle() noexcept = default;

		constexpr euclidean_triangle(const euclidean_triangle &) = default;

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() < SpatialDimensionality::dimensionality())
		constexpr euclidean_triangle(const euclidean_triangle < Type, OtherSpatialDimensionality > &triangle) :
			vertex1(triangle.vertex1),
			vertex2(triangle.vertex2),
			vertex3(triangle.vertex3)
		{
		}

		constexpr euclidean_triangle(euclidean_triangle &&) noexcept = default;

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() < SpatialDimensionality::dimensionality())
		constexpr euclidean_triangle(euclidean_triangle < Type, OtherSpatialDimensionality > &&triangle) noexcept :
			vertex1(std::move(triangle.vertex1)),
			vertex2(std::move(triangle.vertex2)),
			vertex3(std::move(triangle.vertex3))
		{
		}

		template <
			concepts::spatial_dimension OtherSpatialDimensionality1,
			concepts::spatial_dimension OtherSpatialDimensionality2,
			concepts::spatial_dimension OtherSpatialDimensionality3 >
		requires (
			OtherSpatialDimensionality1::dimensionality() <= SpatialDimensionality::dimensionality() &&
			OtherSpatialDimensionality2::dimensionality() <= SpatialDimensionality::dimensionality() &&
			OtherSpatialDimensionality3::dimensionality() <= SpatialDimensionality::dimensionality())
		constexpr euclidean_triangle(
			const vertex < Type, OtherSpatialDimensionality1 > &vertex1,
			const vertex < Type, OtherSpatialDimensionality2 > &vertex2,
			const vertex < Type, OtherSpatialDimensionality3 > &vertex3) :

			vertex1(vertex1),
			vertex2(vertex2),
			vertex3(vertex3)
		{
		}

		template <
			concepts::spatial_dimension OtherSpatialDimensionality1,
			concepts::spatial_dimension OtherSpatialDimensionality2,
			concepts::spatial_dimension OtherSpatialDimensionality3 >
		requires (
			OtherSpatialDimensionality1::dimensionality() <= SpatialDimensionality::dimensionality() &&
			OtherSpatialDimensionality2::dimensionality() <= SpatialDimensionality::dimensionality() &&
			OtherSpatialDimensionality3::dimensionality() <= SpatialDimensionality::dimensionality())
		constexpr euclidean_triangle(
			vertex < Type, OtherSpatialDimensionality1 > &&vertex1,
			vertex < Type, OtherSpatialDimensionality2 > &&vertex2,
			vertex < Type, OtherSpatialDimensionality3 > &&vertex3) noexcept :

			vertex1(std::move(vertex1)),
			vertex2(std::move(vertex2)),
			vertex3(std::move(vertex3))
		{
		}
	};

	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality1,
		concepts::spatial_dimension SpatialDimensionality2 >
	constexpr auto operator+(
		const vector < Type, SpatialDimensionality1 > &vector,
		const euclidean_triangle < Type, SpatialDimensionality2 > &triangle)
	{
		return triangle + vector;
	}

	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality1,
		concepts::spatial_dimension SpatialDimensionality2 >
	constexpr auto operator-(
		const vector < Type, SpatialDimensionality1 > &vector,
		const euclidean_triangle < Type, SpatialDimensionality2 > &triangle)
	{
		return triangle - vector;
	}

	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality1,
		concepts::spatial_dimension SpatialDimensionality2,
		concepts::spatial_dimension SpatialDimensionality3 >
	constexpr auto make_euclidean_triangle(
		const vertex < Type, SpatialDimensionality1 > &vertex1,
		const vertex < Type, SpatialDimensionality2 > &vertex2,
		const vertex < Type, SpatialDimensionality3 > &vertex3)
	{
		using ResultDimensionality =
			typename greater_spatial_dimension <
				SpatialDimensionality1,
				typename greater_spatial_dimension <
					SpatialDimensionality2,
					SpatialDimensionality3 > ::type > ::type;

		return
			euclidean_triangle < Type, ResultDimensionality > {
				vertex1,
				vertex2,
				vertex3 };
	}

	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality1,
		concepts::spatial_dimension SpatialDimensionality2,
		concepts::spatial_dimension SpatialDimensionality3 >
	constexpr auto make_euclidean_triangle(
		vertex < Type, SpatialDimensionality1 > &&vertex1,
		vertex < Type, SpatialDimensionality2 > &&vertex2,
		vertex < Type, SpatialDimensionality3 > &&vertex3)
	{
		using ResultDimensionality =
			typename greater_spatial_dimension <
				SpatialDimensionality1,
				typename greater_spatial_dimension <
					SpatialDimensionality2,
					SpatialDimensionality3 > ::type > ::type;

		return
			euclidean_triangle < Type, ResultDimensionality > {
				std::move(vertex1),
				std::move(vertex2),
				std::move(vertex3) };
	}
}

export namespace std
{
	template <
		xablau::geometry::concepts::geometric_assignable_arithmetic Type,
		xablau::geometry::concepts::spatial_dimension SpatialDimensionality >
	struct hash < xablau::geometry::euclidean_triangle < Type, SpatialDimensionality > >
	{
		size_t operator()(const xablau::geometry::euclidean_triangle < Type, SpatialDimensionality > &triangle) const
		{
			return
				std::hash < Type, SpatialDimensionality > {}(triangle.vertex1) ^
				std::hash < Type, SpatialDimensionality > {}(triangle.vertex2) ^
				std::hash < Type, SpatialDimensionality > {}(triangle.vertex3);
		}
	};
}