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

export module xablau.geometry:hypersphere;
export import :forward_declarations;

export import std;

export import xablau.algebra;

export namespace xablau::geometry
{
	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality >
	class hypersphere final
	{
	public:
		using value_type = Type;

		using abs_type = typename xablau::algebra::abs_type < Type > ::type;

		using size_type = typename std::array < Type, SpatialDimensionality::dimensionality() > ::size_type;

	private:
		template < size_t Dimensionality >
		[[nodiscard]] constexpr std::array < abs_type, 2 > area_and_volume() const
		{
			abs_type previousVolume{1};
			std::array < abs_type, 2 > areaAndVolume{ abs_type{2}, abs_type{2} * this->radius };

			for (size_t i = 1; i < Dimensionality; i++)
			{
				areaAndVolume[0] = abs_type{2} * std::numbers::pi_v < abs_type > * this->radius * previousVolume;

				previousVolume = areaAndVolume[1];

				areaAndVolume[1] = (areaAndVolume[0] * this->radius) / static_cast < abs_type > (i + 1);
			}

			return areaAndVolume;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		static void check_degeneracy(const hypersphere < Type, OtherSpatialDimensionality > &hypersphere)
		{
			if (hypersphere.degenerate())
			{
				throw std::domain_error("\"hypersphere\" is degenerate.");
			}
		}

	public:
		vertex < Type, SpatialDimensionality > center;

		abs_type radius = abs_type{1};

		[[nodiscard]] explicit constexpr operator bool() const
		{
			return !this->degenerate();
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr bool operator<(const hypersphere < Type, OtherSpatialDimensionality > &hypersphere) const
		{
			const bool thisDegenerate = this->degenerate();
			const bool otherDegenerate = hypersphere.degenerate();

			if (thisDegenerate && !otherDegenerate)
			{
				return true;
			}

			if (!thisDegenerate && otherDegenerate)
			{
				return false;
			}

			if (this->radius < hypersphere.radius)
			{
				return true;
			}

			if (!(hypersphere.radius < this->radius))
			{
				return this->center < hypersphere.center;
			}

			return false;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr bool operator<=(const hypersphere < Type, OtherSpatialDimensionality > &hypersphere) const
		{
			const bool thisDegenerate = this->degenerate();
			const bool otherDegenerate = hypersphere.degenerate();

			if (thisDegenerate && !otherDegenerate)
			{
				return true;
			}

			if (!thisDegenerate && otherDegenerate)
			{
				return false;
			}

			if (!(hypersphere.radius <= this->radius))
			{
				return true;
			}

			if (this->radius <= hypersphere.radius)
			{
				return this->center <= hypersphere.center;
			}

			return false;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr bool operator==(const hypersphere < Type, OtherSpatialDimensionality > &hypersphere) const
		{
			const bool thisDegenerate = this->degenerate();
			const bool otherDegenerate = hypersphere.degenerate();

			if (thisDegenerate ^ otherDegenerate)
			{
				return false;
			}

			return
				this->radius == hypersphere.radius &&
				this->center == hypersphere.center;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr bool operator>=(const hypersphere < Type, OtherSpatialDimensionality > &hypersphere) const
		{
			const bool thisDegenerate = this->degenerate();
			const bool otherDegenerate = hypersphere.degenerate();

			if (thisDegenerate && !otherDegenerate)
			{
				return false;
			}

			if (!thisDegenerate && otherDegenerate)
			{
				return true;
			}

			if (!(hypersphere.radius >= this->radius))
			{
				return true;
			}

			if (this->radius >= hypersphere.radius)
			{
				return this->center >= hypersphere.center;
			}

			return false;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr bool operator>(const hypersphere < Type, OtherSpatialDimensionality > &hypersphere) const
		{
			const bool thisDegenerate = this->degenerate();
			const bool otherDegenerate = hypersphere.degenerate();

			if (thisDegenerate && !otherDegenerate)
			{
				return false;
			}

			if (!thisDegenerate && otherDegenerate)
			{
				return true;
			}

			if (this->radius > hypersphere.radius)
			{
				return true;
			}

			if (!(hypersphere.radius > this->radius))
			{
				return this->center > hypersphere.center;
			}

			return false;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr bool operator!=(const hypersphere < Type, OtherSpatialDimensionality > &hypersphere) const
		{
			const bool thisDegenerate = this->degenerate();
			const bool otherDegenerate = hypersphere.degenerate();

			if (thisDegenerate ^ otherDegenerate)
			{
				return true;
			}

			return
				this->radius != hypersphere.radius ||
				this->center != hypersphere.center;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr auto operator+(const vector < Type, OtherSpatialDimensionality > &vector) const
		{
			using ResultSpatialDimension =
				typename greater_spatial_dimension <
					SpatialDimensionality,
					OtherSpatialDimensionality > ::type;

			return
				hypersphere < Type, ResultSpatialDimension > (
					this->center + vector,
					this->radius);
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr auto operator-(const vector < Type, OtherSpatialDimensionality > &vector) const
		{
			using ResultSpatialDimension =
				typename greater_spatial_dimension <
					SpatialDimensionality,
					OtherSpatialDimensionality > ::type;

			return
				hypersphere < Type, ResultSpatialDimension > (
					this->center - vector,
					this->radius);
		}

		[[nodiscard]] constexpr bool degenerate() const
		{
			return
				this->radius < abs_type{} ||
				!xablau::algebra::functions::normal < abs_type > {}(this->radius);
		}

		template <
			bool CalculateDistanceToSurface,
			concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] auto contains(
			const vertex < Type, OtherSpatialDimensionality > &vertex,
			const size_t unitsInTheLastPlace = 1) const
		{
			hypersphere::check_degeneracy(*this);

			hypersphere_side side = hypersphere_side::outside;
			abs_type distance = this->center.distance_to(vertex);

			if (xablau::algebra::functions::almost_equal < abs_type > ::invoke(
					this->radius - distance,
					abs_type{},
					unitsInTheLastPlace))
			{
				side = hypersphere_side::on_surface;
			}

			else if (distance < this->radius)
			{
				side = hypersphere_side::inside;
			}

			if constexpr (CalculateDistanceToSurface)
			{
				return std::make_pair(std::move(side), std::move(distance));
			}

			else
			{
				return side;
			}
		}

		template <
			concepts::spatial_dimension OtherSpatialDimensionality1,
			concepts::spatial_dimension OtherSpatialDimensionality2 >
		[[nodiscard]] abs_type spherical_distance(
			const vertex < Type, OtherSpatialDimensionality1 > &vertexOnHypersphere1,
			const vertex < Type, OtherSpatialDimensionality2 > &vertexOnHypersphere2,
			const size_t unitsInTheLastPlace = 1) const
		{
			hypersphere::check_degeneracy(*this);

			const auto vector1 = vertexOnHypersphere1 - this->center;
			const auto vector2 = vertexOnHypersphere2 - this->center;

			const auto radius1 = vector1.norm();
			const auto radius2 = vector2.norm();

			const auto vertex1OnSurface =
				xablau::algebra::functions::almost_equal < abs_type > ::invoke(
					radius1 - this->radius,
					abs_type{},
					unitsInTheLastPlace);

			const auto vertex2OnSurface =
				xablau::algebra::functions::almost_equal < abs_type > ::invoke(
					radius2 - this->radius,
					abs_type{},
					unitsInTheLastPlace);

			if (!vertex1OnSurface || !vertex2OnSurface)
			{
				throw std::domain_error("Both vertices are not on the hypersphere's surface.");
			}

			return vector1.angle_between(vector2) * this->radius;
		}

		template < size_t Dimensionality = SpatialDimensionality::dimensionality() >
		[[nodiscard]] constexpr abs_type area() const
		{
			if constexpr (Dimensionality >= size_t{2})
			{
				return this->area_and_volume < Dimensionality > ()[0];
			}

			else if constexpr (Dimensionality == size_t{1})
			{
				return abs_type{2};
			}

			else
			{
				return abs_type{};
			}
		}

		template < size_t Dimensionality = SpatialDimensionality::dimensionality() >
		[[nodiscard]] constexpr abs_type volume() const
		{
			if constexpr (Dimensionality >= size_t{1})
			{
				return this->area_and_volume < Dimensionality > ()[1];
			}

			else
			{
				return abs_type{};
			}
		}

		constexpr hypersphere &operator=(const hypersphere &) = default;

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() < SpatialDimensionality::dimensionality())
		constexpr hypersphere &operator=(const hypersphere < Type, OtherSpatialDimensionality > &hypersphere)
		{
			this->center = hypersphere.center;
			this->radius = hypersphere.radius;

			return *this;
		}

		constexpr hypersphere &operator=(hypersphere &&) noexcept = default;

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() < SpatialDimensionality::dimensionality())
		constexpr hypersphere &operator=(hypersphere < Type, OtherSpatialDimensionality > &&hypersphere) noexcept
		{
			this->center = std::move(hypersphere.center);
			this->radius = std::move(hypersphere.radius);

			return *this;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() <= SpatialDimensionality::dimensionality())
		constexpr hypersphere &operator+=(const vector < Type, OtherSpatialDimensionality > &vector)
		{
			this->center += vector;

			return *this;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() <= SpatialDimensionality::dimensionality())
		constexpr hypersphere &operator-=(const vector < Type, OtherSpatialDimensionality > &vector)
		{
			this->center -= vector;

			return *this;
		}

		[[nodiscard]] static consteval size_t dimensionality() noexcept
		{
			return SpatialDimensionality::dimensionality();
		}

		friend std::ostream &operator<<(std::ostream &stream, const hypersphere &hypersphere)
		{
			stream << "() ( " << hypersphere.center << ", R = " << hypersphere.radius << " )";

			return stream;
		}

		constexpr hypersphere() noexcept = default;

		constexpr hypersphere(const hypersphere &) = default;

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() < SpatialDimensionality::dimensionality())
		constexpr hypersphere(const hypersphere < Type, OtherSpatialDimensionality > &hypersphere) :
			center(hypersphere.center),
			radius(hypersphere.radius)
		{
		}

		constexpr hypersphere(hypersphere &&) noexcept = default;

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() < SpatialDimensionality::dimensionality())
		constexpr hypersphere(hypersphere < Type, OtherSpatialDimensionality > &&hypersphere) noexcept :
			center(std::move(hypersphere.center)),
			radius(std::move(hypersphere.radius))
		{
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() <= SpatialDimensionality::dimensionality())
		constexpr hypersphere(
			const vertex < Type, OtherSpatialDimensionality > &center,
			const abs_type &radius) :

			center(center),
			radius(radius)
		{
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() <= SpatialDimensionality::dimensionality())
		constexpr hypersphere(
			vertex < Type, OtherSpatialDimensionality > &&center,
			abs_type &&radius) noexcept :

			center(std::move(center)),
			radius(std::move(radius))
		{
		}
	};

	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality1,
		concepts::spatial_dimension SpatialDimensionality2 >
	constexpr auto operator+(
		const vector < Type, SpatialDimensionality1 > &vector,
		const hypersphere < Type, SpatialDimensionality2 > &hypersphere)
	{
		return hypersphere + vector;
	}

	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality1,
		concepts::spatial_dimension SpatialDimensionality2 >
	constexpr auto operator-(
		const vector < Type, SpatialDimensionality1 > &vector,
		const hypersphere < Type, SpatialDimensionality2 > &hypersphere)
	{
		return hypersphere - vector;
	}

	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality >
	constexpr auto make_hypersphere(
		const vertex < Type, SpatialDimensionality > &center,
		const typename xablau::algebra::abs_type < Type > ::type &radius)
	{
		return hypersphere < Type, SpatialDimensionality > {center, radius};
	}

	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality >
	constexpr auto make_hypersphere(
		vertex < Type, SpatialDimensionality > &&center,
		typename xablau::algebra::abs_type < Type > ::type &&radius)
	{
		return hypersphere < Type, SpatialDimensionality > {std::move(center), std::move(radius)};
	}
}

export namespace std
{
	template <
		xablau::geometry::concepts::geometric_assignable_arithmetic Type,
		xablau::geometry::concepts::spatial_dimension SpatialDimensionality >
	struct hash < xablau::geometry::hypersphere < Type, SpatialDimensionality > >
	{
		size_t operator()(const xablau::geometry::hypersphere < Type, SpatialDimensionality > &hypersphere) const
		{
			return
				std::hash < typename xablau::geometry::hypersphere < Type, SpatialDimensionality > ::abs_type > {}(hypersphere.radius) ^
				std::hash < xablau::geometry::vertex < Type > > {}(hypersphere.center);
		}
	};
}