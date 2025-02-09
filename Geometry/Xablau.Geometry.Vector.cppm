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

export module xablau.geometry:vector;
export import :forward_declarations;
export import :fundamental_functions;

export import std;

export import xablau.algebra;

export namespace xablau::geometry
{
	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality >
	class vector final
	{
	public:
		using value_type = Type;

		using abs_type = typename xablau::algebra::abs_type < Type > ::type;

		using size_type = typename std::array < Type, SpatialDimensionality::dimensionality() > ::size_type;

		std::array < Type, SpatialDimensionality::dimensionality() > coordinates{};

	private:
		enum class vector_comparison_result
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

		template < bool Equal, typename Vector >
		[[nodiscard]] constexpr vector_comparison_result vector_comparison_equality(const Vector &vector) const
		{
			size_type i{};
			constexpr auto limit = std::min(vector.dimensionality(), this->dimensionality());
			const auto &_coordinates = vector.coordinates;

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
					return vector_comparison_result::not_equal;
				}
			}

			for (; i < this->dimensionality(); i++)
			{
				if (notEqual(this->coordinates[i], Type{}))
				{
					return vector_comparison_result::not_equal;
				}
			}

			for (; i < vector.dimensionality(); i++)
			{
				if (notEqual(_coordinates[i], Type{}))
				{
					return vector_comparison_result::not_equal;
				}
			}

			return vector_comparison_result::equal;
		}

		template < comparison_operator ComparisonOperator, typename Vector >
		[[nodiscard]] constexpr vector_comparison_result vector_comparison_inequality(const Vector &vector) const
		{
			size_type i{};
			constexpr auto limit = std::min(vector.dimensionality(), this->dimensionality());
			const auto &_coordinates = vector.coordinates;

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
					return vector_comparison_result::less;
				}

				if (lessThan(_coordinates[i], this->coordinates[i]))
				{
					return vector_comparison_result::greater;
				}
			}

			for (; i < this->dimensionality(); i++)
			{
				if (lessThan(this->coordinates[i], Type{}))
				{
					return vector_comparison_result::less;
				}

				if (lessThan(Type{}, this->coordinates[i]))
				{
					return vector_comparison_result::greater;
				}
			}

			for (; i < vector.dimensionality(); i++)
			{
				if (lessThan(Type{}, _coordinates[i]))
				{
					return vector_comparison_result::less;
				}

				if (lessThan(_coordinates[i], Type{}))
				{
					return vector_comparison_result::greater;
				}
			}

			return vector_comparison_result::equal;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] abs_type scalar_product_divided_by_norm(const vector < Type, OtherSpatialDimensionality > &vector) const
		{
			return this->scalar_product(vector) / (this->norm() * vector.norm());
		}

		static constexpr Type _null_value{};

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
				return vector::_null_value;
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
				return vector::_null_value;
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
				return vector::_null_value;
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
				return vector::_null_value;
			}
		}

		[[nodiscard]] constexpr explicit operator bool() const
		{
			for (const auto &item : this->coordinates)
			{
				if (item != Type{})
				{
					return true;
				}
			}

			return false;
		}

		[[nodiscard]] constexpr bool null() const
		{
			return !(*this);
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr bool operator<(const vector < Type, OtherSpatialDimensionality > &vector) const
		{
			return this->vector_comparison_inequality < comparison_operator::less > (vector) == vector_comparison_result::less;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr bool operator<=(const vector < Type, OtherSpatialDimensionality > &vector) const
		{
			return this->vector_comparison_inequality < comparison_operator::less_equal > (vector) != vector_comparison_result::greater;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr bool operator==(const vector < Type, OtherSpatialDimensionality > &vector) const
		{
			return this->vector_comparison_equality < true > (vector) == vector_comparison_result::equal;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr bool operator>=(const vector < Type, OtherSpatialDimensionality > &vector) const
		{
			return this->vector_comparison_inequality < comparison_operator::greater_equal > (vector) != vector_comparison_result::less;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr bool operator>(const vector < Type, OtherSpatialDimensionality > &vector) const
		{
			return this->vector_comparison_inequality < comparison_operator::greater > (vector) == vector_comparison_result::greater;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr bool operator!=(const vector < Type, OtherSpatialDimensionality > &vector) const
		{
			return this->vector_comparison_equality < false > (vector) != vector_comparison_result::equal;
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
			geometry::vector < Type, ResultSpatialDimension > result;

			auto &resultCoordinates = result.coordinates;

			for (; i < min; i++)
			{
				resultCoordinates[i] = this->coordinates[i] + _coordinates[i];
			}

			for (; i < vector.dimensionality(); i++)
			{
				resultCoordinates[i] = _coordinates[i];
			}

			for (; i < this->dimensionality(); i++)
			{
				resultCoordinates[i] = this->coordinates[i];
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
			geometry::vector < Type, ResultSpatialDimension > result;

			for (; i < min; i++)
			{
				result[i] = this->coordinates[i] - _coordinates[i];
			}

			for (; i < this->dimensionality(); i++)
			{
				result[i] = this->coordinates[i];
			}

			for (; i < vector.dimensionality(); i++)
			{
				result[i] = -_coordinates[i];
			}

			return result;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr auto operator+(const vertex < Type, OtherSpatialDimensionality > &vertex) const
		{
			return vertex + (*this);
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr auto operator-(const vertex < Type, OtherSpatialDimensionality > &vertex) const
		requires (xablau::algebra::concepts::negatable < Type >)
		{
			return vertex - (*this);
		}

		[[nodiscard]] constexpr vector operator-() const
		requires (xablau::algebra::concepts::negatable < Type >)
		{
			vector result{};
			auto &_coordinates = result.coordinates;

			std::transform(this->coordinates.cbegin(), this->coordinates.cend(), _coordinates.begin(),
				[] (const Type &coordinate) -> Type
				{
					return -coordinate;
				});

			return result;
		}

		template < typename MultiplicandType >
		requires (std::same_as < MultiplicandType, Type > || std::same_as < MultiplicandType, abs_type >)
		[[nodiscard]] constexpr vector operator*(const MultiplicandType &multiplicand) const
		{
			vector result{};
			auto &_coordinates = result.coordinates;

			std::transform(this->coordinates.cbegin(), this->coordinates.cend(), _coordinates.begin(),
				[&multiplicand] (const Type &coordinate) -> Type
				{
					return coordinate * multiplicand;
				});

			return result;
		}

		template < typename DivisorType >
		requires (std::same_as < DivisorType, Type > || std::same_as < DivisorType, abs_type >)
		[[nodiscard]] constexpr vector operator/(const DivisorType &divisor) const
		{
			if constexpr (std::integral < DivisorType >)
			{
				if (divisor == DivisorType{})
				{
					throw std::domain_error("\"divisor\" is zero.");
				}
			}

			vector result{};
			auto &_coordinates = result.coordinates;

			std::transform(this->coordinates.cbegin(), this->coordinates.cend(), _coordinates.begin(),
				[&divisor] (const Type &coordinate) -> Type
				{
					return coordinate / divisor;
				});

			return result;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() <= SpatialDimensionality::dimensionality())
		constexpr vector &operator+=(const vector < Type, OtherSpatialDimensionality > &vector)
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
		constexpr vector &operator-=(const vector < Type, OtherSpatialDimensionality > &vector)
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

		template < typename MultiplicandType >
		requires (std::same_as < MultiplicandType, Type > || std::same_as < MultiplicandType, abs_type >)
		constexpr vector &operator*=(const MultiplicandType &multiplicand)
		{
			for (auto &coordinate : this->coordinates)
			{
				coordinate *= multiplicand;
			}

			return *this;
		}

		template < typename DivisorType >
		requires (std::same_as < DivisorType, Type > || std::same_as < DivisorType, abs_type >)
		constexpr vector &operator/=(const DivisorType &divisor)
		{
			if constexpr (std::integral < DivisorType >)
			{
				if (divisor == DivisorType{})
				{
					throw std::domain_error("\"divisor\" is zero.");
				}
			}

			for (auto &coordinate : this->coordinates)
			{
				coordinate /= divisor;
			}

			return *this;
		}

		constexpr void fill(const Type &value)
		{
			this->coordinates.fill(value);
		}

		constexpr vector &operator=(const vector &) = default;

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() < SpatialDimensionality::dimensionality())
		constexpr vector &operator=(const vector < Type, OtherSpatialDimensionality > &vector)
		{
			std::copy(
				vector.coordinates.cbegin(),
				vector.coordinates.cend(),
				this->coordinates.begin());

			std::fill(
				this->coordinates.begin() + OtherSpatialDimensionality::dimensionality(),
				this->coordinates.end(),
				Type{});

			return *this;
		}

		constexpr vector &operator=(vector &&) noexcept = default;

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() < SpatialDimensionality::dimensionality())
		constexpr vector &operator=(vector < Type, OtherSpatialDimensionality > &&vector) noexcept
		{
			std::move(
				vector.coordinates.cbegin(),
				vector.coordinates.cend(),
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
			return (dimension < this->dimensionality() ? this->coordinates[dimension] : vector::_null_value);
		}

		[[nodiscard]] abs_type norm() const
		{
			return static_cast < abs_type > (xablau::algebra::functions::sqrt < abs_type > ::invoke(this->scalar_product(*this)));
		}

		[[nodiscard]] vector normalize() const
		{
			const auto norm = this->norm();

			if (norm == abs_type{})
			{
				return vector();
			}

			vector result{};
			auto &_coordinates = result.coordinates;

			std::transform(this->coordinates.cbegin(), this->coordinates.cend(), _coordinates.begin(),
				[&norm] (const Type &coordinate) -> Type
				{
					return coordinate / norm;
				});

			return result;
		}

		vector &normalize_in_place()
		{
			const auto norm = this->norm();

			if (norm == abs_type{})
			{
				return *this;
			}

			for (auto &coordinate : this->coordinates)
			{
				coordinate /= norm;
			}

			return *this;
		}

		[[nodiscard]] constexpr vector reverse_sense() const
		requires (xablau::algebra::concepts::negatable < Type >)
		{
			auto vector = *this;

			for (auto &coordinate : vector.coordinates)
			{
				coordinate = -coordinate;
			}

			return vector;
		}

		constexpr vector &reverse_sense_in_place()
		requires (xablau::algebra::concepts::negatable < Type >)
		{
			for (auto &coordinate : this->coordinates)
			{
				coordinate = -coordinate;
			}

			return *this;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] abs_type angle_between(const vector < Type, OtherSpatialDimensionality > &vector) const
		{
			if (this->null() || vector.null())
			{
				throw std::domain_error("One of the vectors is a null vector.");
			}

			return xablau::algebra::functions::acos < abs_type > ::invoke(this->scalar_product_divided_by_norm(vector));
		}

		template <
			concepts::spatial_dimension OtherSpatialDimensionality1,
			concepts::spatial_dimension OtherSpatialDimensionality2 >
		requires (OtherSpatialDimensionality1::dimensionality() <= 7)
		[[nodiscard]] abs_type angle_between_with_sign(
			const geometry::vector < Type, OtherSpatialDimensionality1 > &vector,
			const geometry::vector < Type, OtherSpatialDimensionality2 > &orientation) const
		requires (
			SpatialDimensionality::dimensionality() <= 7 &&
			xablau::algebra::concepts::negatable < abs_type > &&

			requires ()
			{
				{ xablau::algebra::functions::sign < abs_type > ::invoke(abs_type{}) } -> std::ranges::sized_range;
			})
		{
			if (this->null() || vector.null())
			{
				throw std::domain_error("One of the vectors is a null vector.");
			}

			const auto angle = xablau::algebra::functions::acos < abs_type > ::invoke(this->scalar_product_divided_by_norm(vector));

			constexpr vector_product_dimension dimension =
				SpatialDimensionality::dimensionality() <= 3 && OtherSpatialDimensionality1::dimensionality() <= 3 ?
					vector_product_dimension::three :
					vector_product_dimension::seven;

			const auto sign =
				xablau::algebra::functions::sign < abs_type > ::invoke(
					orientation.scalar_product(this->vector_product < dimension > (vector)));

			static_assert(sign.size() == 1, "Unexpected size for \"sign_type < Type > ::type\"");

			if (*(sign.cbegin()) == xablau::algebra::sign_value::negative)
			{
				return -angle;
			}

			return angle;
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr abs_type scalar_product(const vector < Type, OtherSpatialDimensionality > &vector) const
		{
			const auto &coordinates1 = this->coordinates;
			const auto &coordinates2 = vector.coordinates;

			if constexpr (
				xablau::algebra::associative_and_commutative_addition < Type > ::value &&
				xablau::algebra::associative_and_commutative_multiplication < Type > ::value)
			{
				if constexpr (this->dimensionality() <= vector.dimensionality())
				{
					return
						std::transform_reduce(
							coordinates1.cbegin(),
							coordinates1.cend(),
							coordinates2.cbegin(),
							abs_type{},
							xablau::geometry::functions::template scalar_product_reduction < Type > {},
							xablau::geometry::functions::template scalar_product_transformation < Type > {});
				}

				else
				{
					return
						std::transform_reduce(
							coordinates2.cbegin(),
							coordinates2.cend(),
							coordinates1.cbegin(),
							abs_type{},
							xablau::geometry::functions::template scalar_product_reduction < Type > {},
							xablau::geometry::functions::template scalar_product_transformation < Type > {});
				}
			}

			else
			{
				if constexpr (this->dimensionality() <= vector.dimensionality())
				{
					return
						std::inner_product(
							coordinates1.cbegin(),
							coordinates1.cend(),
							coordinates2.cbegin(),
							abs_type{},
							xablau::geometry::functions::template scalar_product_reduction < Type > {},
							xablau::geometry::functions::template scalar_product_transformation < Type > {});
				}

				else
				{
					return
						std::inner_product(
							coordinates2.cbegin(),
							coordinates2.cend(),
							coordinates1.cbegin(),
							abs_type{},
							xablau::geometry::functions::template scalar_product_reduction < Type > {},
							xablau::geometry::functions::template scalar_product_transformation < Type > {});
				}
			}
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool perpendicular_to(const vector < Type, OtherSpatialDimensionality > &vector, const size_t unitsInTheLastPlace = 1) const
		{
			if (this->null() || vector.null())
			{
				return true;
			}

			const auto thetaCosine = this->scalar_product_divided_by_norm(vector);

			return
				xablau::algebra::functions::almost_equal < abs_type > ::invoke(
					thetaCosine,
					abs_type{},
					unitsInTheLastPlace);
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool parallel_to(const vector < Type, OtherSpatialDimensionality > &vector, const size_t unitsInTheLastPlace = 1) const
		{
			if (this->null() || vector.null())
			{
				return true;
			}

			const auto thetaCosine = this->scalar_product_divided_by_norm(vector);

			return
				xablau::algebra::functions::almost_equal < abs_type > ::invoke(
					xablau::algebra::functions::abs < abs_type > ::invoke(thetaCosine),
					abs_type{1},
					unitsInTheLastPlace);
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool codirectional_to(const vector < Type, OtherSpatialDimensionality > &vector, const size_t unitsInTheLastPlace = 1) const
		{
			if (this->null() || vector.null())
			{
				return true;
			}

			const auto thetaCosine = this->scalar_product_divided_by_norm(vector);

			return
				xablau::algebra::functions::almost_equal < abs_type > ::invoke(
					thetaCosine,
					abs_type{1},
					unitsInTheLastPlace);
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool linearly_independent_to(const vector < Type, OtherSpatialDimensionality > &vector, const size_t unitsInTheLastPlace = 1) const
		{
			return !this->parallel_to(vector, unitsInTheLastPlace);
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] bool same_orthant_as(const vector < Type, OtherSpatialDimensionality > &vector, const size_t unitsInTheLastPlace = 1) const
		requires requires ()
		{
			{ xablau::algebra::functions::sign < abs_type > ::invoke(abs_type{}) } -> std::ranges::random_access_range;
			{ xablau::algebra::functions::sign < abs_type > ::invoke(abs_type{}) } -> std::ranges::sized_range;
		}
		{
			const auto &coordinates1 = this->coordinates;
			const auto &coordinates2 = vector.coordinates;

			constexpr size_type min = std::min(this->dimensionality(), vector.dimensionality());

			for (size_type i = 0; i < min; i++)
			{
				const bool coordinate1AlmostZero =
					xablau::algebra::functions::almost_equal < abs_type > ::invoke(
						coordinates1[i],
						Type{},
						unitsInTheLastPlace);

				const bool coordinate2AlmostZero =
					xablau::algebra::functions::almost_equal < abs_type > ::invoke(
						coordinates2[i],
						Type{},
						unitsInTheLastPlace);

				if (!coordinate1AlmostZero && !coordinate2AlmostZero)
				{
					const auto sign1 = xablau::algebra::functions::sign < Type > ::invoke(coordinates1[i]);
					const auto sign2 = xablau::algebra::functions::sign < Type > ::invoke(coordinates2[i]);
					const auto signQuantity = std::min(sign1.size(), sign2.size());

					for (size_type j = 0; j < signQuantity; j++)
					{
						if (sign1[j] != xablau::algebra::sign_value::zero &&
							sign2[j] != xablau::algebra::sign_value::zero &&
							sign1[j] != sign2[j])
						{
							return false;
						}
					}
				}
			}

			return true;
		}

		template < vector_product_dimension Dimension, concepts::spatial_dimension OtherSpatialDimensionality >
		requires (
			(Dimension == vector_product_dimension::seven ?
				SpatialDimensionality::dimensionality() <= 7 :
				SpatialDimensionality::dimensionality() <= 3) &&

			(Dimension == vector_product_dimension::seven ?
				OtherSpatialDimensionality::dimensionality() <= 7 :
				OtherSpatialDimensionality::dimensionality() <= 3))
		[[nodiscard]] constexpr auto vector_product(const vector < Type, OtherSpatialDimensionality > &vector) const
		{
			const auto &vector1 = *this;
			const auto &vector2 = vector;

			if constexpr (Dimension == vector_product_dimension::seven)
			{
				geometry::vector < Type, spatial_dimension < 7 > > _result{};
				auto &result = _result.coordinates;

				result[0] =
					vector1[1] * vector2[3] -
					vector1[3] * vector2[1] +
					vector1[2] * vector2[6] -
					vector1[6] * vector2[2] +
					vector1[4] * vector2[5] -
					vector1[5] * vector2[4];

				result[1] =
					vector1[2] * vector2[4] -
					vector1[4] * vector2[2] +
					vector1[3] * vector2[0] -
					vector1[0] * vector2[3] +
					vector1[5] * vector2[6] -
					vector1[6] * vector2[5];

				result[2] =
					vector1[3] * vector2[5] -
					vector1[5] * vector2[3] -
					vector1[4] * vector2[1] +
					vector1[1] * vector2[4] -
					vector1[6] * vector2[0] +
					vector1[0] * vector2[6];

				result[3] =
					vector1[4] * vector2[6] -
					vector1[6] * vector2[4] +
					vector1[5] * vector2[2] -
					vector1[2] * vector2[5] +
					vector1[0] * vector2[1] -
					vector1[1] * vector2[0];

				result[4] =
					vector1[5] * vector2[0] -
					vector1[0] * vector2[5] +
					vector1[6] * vector2[3] -
					vector1[3] * vector2[6] +
					vector1[1] * vector2[2] -
					vector1[2] * vector2[1];

				result[5] =
					vector1[6] * vector2[1] -
					vector1[1] * vector2[6] +
					vector1[0] * vector2[4] -
					vector1[4] * vector2[0] +
					vector1[2] * vector2[3] -
					vector1[3] * vector2[2];

				result[6] =
					vector1[0] * vector2[2] -
					vector1[2] * vector2[0] +
					vector1[1] * vector2[5] -
					vector1[5] * vector2[1] +
					vector1[3] * vector2[4] -
					vector1[4] * vector2[3];

				return _result;
			}

			else
			{
				geometry::vector < Type, spatial_dimension < 3 > > _result{};
				auto &result = _result.coordinates;

				result[0] = vector1[1] * vector2[2] - vector1[2] * vector2[1];
				result[1] = vector1[2] * vector2[0] - vector1[0] * vector2[2];
				result[2] = vector1[0] * vector2[1] - vector1[1] * vector2[0];

				return _result;
			}
		}

		template < typename CoefficientType >
		requires (std::same_as < CoefficientType, Type > || std::same_as < CoefficientType, abs_type >)
		constexpr vector &scale_by(const CoefficientType &coefficient)
		{
			for (auto &coordinate : this->coordinates)
			{
				coordinate = coefficient * coordinate;
			}

			return *this;
		}

		template < typename Range >
		requires (
			std::ranges::forward_range < Range > &&
			std::ranges::sized_range < Range > && (
				std::same_as < std::ranges::range_value_t < Range >, abs_type > ||
				std::same_as < std::ranges::range_value_t < Range >, Type >))
		constexpr vector &scale_by(const Range &coefficients)
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

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		[[nodiscard]] constexpr
			vector <
				Type,
				typename greater_spatial_dimension < SpatialDimensionality, OtherSpatialDimensionality > ::type >
			project_on(const vector < Type, OtherSpatialDimensionality > &component) const
		{
			if (component.null())
			{
				return vector();
			}

			return component * (this->scalar_product(component) / component.scalar_product(component));
		}

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() <= SpatialDimensionality::dimensionality())
		constexpr vector &project_on_in_place(const vector < Type, OtherSpatialDimensionality > &component)
		{
			if (component.null())
			{
				this->coordinates.fill(Type{});
			}

			else
			{
				*this = component * (this->scalar_product(component) / component.scalar_product(component));
			}

			return *this;
		}

		friend std::ostream &operator<<(std::ostream &stream, const vector &vector)
		{
			auto iterator = vector.coordinates.cbegin();
			const auto end = vector.coordinates.cend();

			stream << "-> ( ";

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

		vector &rotate_around_X_axis(const abs_type &angle)
		requires (SpatialDimensionality::dimensionality() == 3)
		{
			const auto y = this->y();
			const auto sinAngle = xablau::algebra::functions::sin < abs_type > ::invoke(angle);
			const auto cosAngle = xablau::algebra::functions::cos < abs_type > ::invoke(angle);

			this->y() = cosAngle * this->y() - sinAngle * this->z();
			this->z() = sinAngle * y + cosAngle * this->z();

			return *this;
		}

		vector &rotate_around_Y_axis(const abs_type &angle)
		requires (SpatialDimensionality::dimensionality() == 3)
		{
			const auto x = this->x();
			const auto sinAngle = xablau::algebra::functions::sin < abs_type > ::invoke(angle);
			const auto cosAngle = xablau::algebra::functions::cos < abs_type > ::invoke(angle);

			this->x() = cosAngle * this->x() + sinAngle * this->z();
			this->z() = -sinAngle * x + cosAngle * this->z();

			return *this;
		}

		vector &rotate_around_Z_axis(const abs_type &angle)
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
		vector &rotate_around_arbitrary_axis(const vector < Type, OtherSpatialDimensionality > &axis, const abs_type &angle)
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

		[[nodiscard]] constexpr vector transform(const std::function < Type(const Type &) > &transformFunction) const
		{
			vector vector{};

			std::transform(this->coordinates.cbegin(), this->coordinates.cend(), vector.coordinates.begin(),
				[&transformFunction] (const Type &value) -> Type
				{
					return transformFunction(value);
				});

			return vector;
		}

		template < typename Range >
		requires (
			std::ranges::sized_range < Range > &&
			std::ranges::forward_range < Range > &&
			std::same_as < std::ranges::range_value_t < Range >, std::function < Type(const vector &) > >)
		[[nodiscard]] constexpr vector transform(const Range &transformFunctions) const
		{
			if (std::ranges::size(transformFunctions) >= SpatialDimensionality::dimensionality())
			{
				throw std::out_of_range("\"transformFunctions.size() >= SpatialDimensionality::dimensionality()\"");
			}

			vector vector;

			std::transform(std::ranges::cbegin(transformFunctions), std::ranges::cend(transformFunctions), vector.coordinates.begin(),
				[this] (const std::function < Type(const geometry::vector &) > &transformFunction) -> Type
				{
					return transformFunction(*this);
				});

			return vector;
		}

		constexpr vector &transform_in_place(const std::function < Type(const Type &) > &transformFunction)
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
			std::same_as < std::ranges::range_value_t < Range >, std::function < Type(const vector &) > >)
		constexpr vector &transform_in_place(const Range &transformFunctions)
		{
			if (std::ranges::size(transformFunctions) >= SpatialDimensionality::dimensionality())
			{
				throw std::out_of_range("\"transformFunctions.size() >= SpatialDimensionality::dimensionality()\"");
			}

			const auto vector = *this;

			std::transform(std::ranges::cbegin(transformFunctions), std::ranges::cend(transformFunctions), this->coordinates.begin(),
				[&vector] (const std::function < Type(const geometry::vector &) > &transformFunction) -> Type
				{
					return transformFunction(vector);
				});

			return *this;
		}

		constexpr vector() noexcept = default;

		constexpr vector(const vector &) = default;

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() < SpatialDimensionality::dimensionality())
		constexpr vector(const vector < Type, OtherSpatialDimensionality > &vector)
		{
			std::copy(
				vector.coordinates.cbegin(),
				vector.coordinates.cend(),
				this->coordinates.begin());
		}

		constexpr vector(vector &&) noexcept = default;

		template < concepts::spatial_dimension OtherSpatialDimensionality >
		requires (OtherSpatialDimensionality::dimensionality() < SpatialDimensionality::dimensionality())
		constexpr vector(vector < Type, OtherSpatialDimensionality > &&vector) noexcept
		{
			std::move(
				vector.coordinates.cbegin(),
				vector.coordinates.cend(),
				this->coordinates.begin());
		}

		template <
			concepts::spatial_dimension OtherSpatialDimensionality1,
			concepts::spatial_dimension OtherSpatialDimensionality2 >
		requires (
			OtherSpatialDimensionality1::dimensionality() <= SpatialDimensionality::dimensionality() &&
			OtherSpatialDimensionality2::dimensionality() <= SpatialDimensionality::dimensionality())
		constexpr vector(
			const vertex < Type, OtherSpatialDimensionality1 > &startingVertex,
			const vertex < Type, OtherSpatialDimensionality2 > &endingVertex)
		requires (xablau::algebra::concepts::negatable < Type >)
		{
			size_type i{};
			const auto &coordinates1 = startingVertex.coordinates;
			const auto &coordinates2 = endingVertex.coordinates;
			constexpr size_type min = std::min(startingVertex.dimensionality(), endingVertex.dimensionality());

			for (; i < min; i++)
			{
				this->coordinates[i] = coordinates2[i] - coordinates1[i];
			}

			for (; i < coordinates1.size(); i++)
			{
				this->coordinates[i] = -coordinates1[i];
			}

			for (; i < coordinates2.size(); i++)
			{
				this->coordinates[i] =  coordinates2[i];
			}
		}

		template < size_t N >
		requires (N == SpatialDimensionality::dimensionality())
		constexpr vector(const std::array < Type, N > &coordinates) :
			coordinates(coordinates)
		{
		}

		template < size_t N >
		requires (N < SpatialDimensionality::dimensionality())
		constexpr vector(const std::array < Type, N > &coordinates)
		{
			std::copy(
				coordinates.cbegin(),
				coordinates.cend(),
				this->coordinates.begin());
		}

		template < size_t N >
		requires (N == SpatialDimensionality::dimensionality())
		constexpr vector(std::array < Type, N > &&coordinates) noexcept :
			coordinates(std::move(coordinates))
		{
		}

		template < size_t N >
		requires (N < SpatialDimensionality::dimensionality())
		constexpr vector(std::array < Type, N > &&coordinates) noexcept
		{
			std::move(
				coordinates.cbegin(),
				coordinates.cend(),
				this->coordinates.begin());
		}
	};

	template < concepts::geometric_assignable_arithmetic ... Type >
	requires (std::same_as < Type, typename std::common_type < Type... > ::type > && ...)
	constexpr auto make_vector(Type ... values)
	{
		return
			vector <
				typename std::common_type < Type... > ::type,
				spatial_dimension < sizeof...(Type) > >
				{
					std::to_array({ std::forward < typename std::common_type < Type... > ::type > (values)... })
				};
	}

	template <
		concepts::geometric_assignable_arithmetic Type,
		size_t N >
	constexpr vector < Type, spatial_dimension < N > > make_vector(const std::array < Type, N > &array)
	{
		return vector < Type, spatial_dimension < N > > {array};
	}

	template <
		concepts::geometric_assignable_arithmetic Type,
		size_t N >
	constexpr vector < Type, spatial_dimension < N > > make_vector(std::array < Type, N > &&array)
	{
		return vector < Type, spatial_dimension < N > > {std::move(array)};
	}

	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality1,
		concepts::spatial_dimension SpatialDimensionality2 >
	constexpr auto make_vector(
		const vertex < Type, SpatialDimensionality1 > &startingVertex,
		const vertex < Type, SpatialDimensionality2 > &endingVertex)
	{
		if constexpr (SpatialDimensionality1::dimensionality() >= SpatialDimensionality2::dimensionality())
		{
			return vector < Type, SpatialDimensionality1 > {startingVertex, endingVertex};
		}

		else
		{
			return vector < Type, SpatialDimensionality2 > {startingVertex, endingVertex};
		}
	}

	template <
		concepts::geometric_assignable_arithmetic Type,
		concepts::spatial_dimension SpatialDimensionality,
		typename MultiplierType >
	requires (
		std::same_as < MultiplierType, Type > ||
		std::same_as < MultiplierType, typename vector < Type, SpatialDimensionality > ::abs_type >)
	constexpr vector < Type, SpatialDimensionality > operator*(
		const MultiplierType &multiplier,
		const vector < Type, SpatialDimensionality > &vector)
	{
		geometry::vector < Type, SpatialDimensionality > result;

		std::transform(vector.coordinates.cbegin(), vector.coordinates.cend(), result.coordinates.begin(),
			[&multiplier] (const Type &coordinate) -> Type
			{
				return multiplier * coordinate;
			});

		return result;
	}
}

export namespace xablau::algebra::functions
{
	template <
		xablau::geometry::concepts::geometric_assignable_arithmetic Type,
		xablau::geometry::concepts::spatial_dimension SpatialDimensionality >
	struct almost_equal < xablau::geometry::vector < Type, SpatialDimensionality > >
	{
		template < size_t UnitsInTheLastPlace >
		[[nodiscard]] bool operator()(
			const xablau::geometry::vector < Type, SpatialDimensionality > &value1,
			const xablau::geometry::vector < Type, SpatialDimensionality > &value2) const
		{
			return almost_equal::invoke < UnitsInTheLastPlace > (value1, value2);
		}

		[[nodiscard]] bool operator()(
			const xablau::geometry::vector < Type, SpatialDimensionality > &value1,
			const xablau::geometry::vector < Type, SpatialDimensionality > &value2,
			const size_t unitsInTheLastPlace) const
		{
			return almost_equal::invoke(value1, value2, unitsInTheLastPlace);
		}

		template < size_t UnitsInTheLastPlace >
		[[nodiscard]] static bool invoke(
			const xablau::geometry::vector < Type, SpatialDimensionality > &value1,
			const xablau::geometry::vector < Type, SpatialDimensionality > &value2)
		{
			const auto &coordinates1 = value1.coordinates;
			const auto &coordinates2 = value2.coordinates;

			for (typename xablau::geometry::vector < Type, SpatialDimensionality > ::size_type i{};
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
			const xablau::geometry::vector < Type, SpatialDimensionality > &value1,
			const xablau::geometry::vector < Type, SpatialDimensionality > &value2,
			const size_t unitsInTheLastPlace)
		{
			const auto &coordinates1 = value1.coordinates;
			const auto &coordinates2 = value2.coordinates;

			for (typename xablau::geometry::vector < Type, SpatialDimensionality > ::size_type i{};
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
	struct hash < xablau::geometry::vector < Type, SpatialDimensionality > >
	{
		size_t operator()(const xablau::geometry::vector < Type, SpatialDimensionality > &vector) const
		{
			size_t hash{};

			auto beginIterator = vector.coordinates.crbegin();
			const auto endIterator = vector.coordinates.crend();

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