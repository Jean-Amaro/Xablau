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

export module xablau.geometry:algorithm;
import :forward_declarations;

export import std;

export import xablau.algebra;

namespace xablau::geometry::algorithm
{
	namespace internals
	{
		template <
			typename ValueType,
			size_t Dimensionality,
			typename RangeOfVertices >
		class Fischer_Gartner_Kutz_subspan final
		{
		private:
			using value_type = ValueType;

			using line_type = std::array < value_type, Dimensionality >;

			using square_matrix_type =
				std::array < std::array < value_type, Dimensionality >, Dimensionality >;

			const RangeOfVertices &_vertices;
			std::vector < bool > _membership{};
			std::array < size_t, Dimensionality + 1 > _members{};
			square_matrix_type _matrix_Q{};
			square_matrix_type _matrix_R{};
			line_type _u{};
			line_type _w{};
			size_t _r{};

			static void givens(
				value_type &c,
				value_type &s,
				const value_type a,
				const value_type b)
			{
				constexpr auto zero = value_type{0};
				constexpr auto one = value_type{1};

				if (b == zero)
				{
					c = one;
					s = zero;
				}

				else if (std::abs(b) > std::abs(a))
				{
					const auto t = a / b;
					s = one / std::sqrt(one + (t * t));
					c = s * t;
				}

				else
				{
					const auto t = b / a;
					c = one / std::sqrt(one + (t * t));
					s = c * t;
				}
			}

			void append_column()
			{
				if (this->_r >= Dimensionality)
				{
					throw std::runtime_error(""); // TODO: Create message.
				}

				for (size_t i = 0; i < Dimensionality; i++)
				{
					this->_matrix_R[this->_r][i] = 0;

					for (size_t k = 0; k < Dimensionality; ++k)
					{
						this->_matrix_R[this->_r][i] +=
							this->_matrix_Q[i][k] * this->_u[k];
					}
				}

				for (size_t j = Dimensionality - 1; j > this->_r; --j)
				{
					value_type c{};
					value_type s{};

					Fischer_Gartner_Kutz_subspan::givens(
						c,
						s,
						this->_matrix_R[this->_r][j - 1],
						this->_matrix_R[this->_r][j]);

					this->_matrix_R[this->_r][j - 1] =
						c * this->_matrix_R[this->_r][j - 1] +
						s * this->_matrix_R[this->_r][j];

					for (size_t i = 0; i < Dimensionality; i++)
					{
						const auto a = this->_matrix_Q[j - 1][i];
						const auto b = this->_matrix_Q[j][i];

						this->_matrix_Q[j - 1][i] = c * a + s * b;
						this->_matrix_Q[j][i] = c * b - s * a;
					}
				}
			}

			void Hessenberg_clear(size_t pos)
			{
				for (; pos < this->_r; pos++)
				{
					value_type c{};
					value_type s{};

					Fischer_Gartner_Kutz_subspan::givens(
						c,
						s,
						this->_matrix_R[pos][pos],
						this->_matrix_R[pos][pos + 1]);

					this->_matrix_R[pos][pos] =
						c * this->_matrix_R[pos][pos] + s * this->_matrix_R[pos][pos + 1];

					for (size_t j = pos + 1; j < this->_r; j++)
					{
						const auto a = this->_matrix_R[j][pos];
						const auto b = this->_matrix_R[j][pos + 1];

						this->_matrix_R[j][pos] = c * a + s * b;
						this->_matrix_R[j][pos + 1] = c * b - s * a;
					}

					for (size_t i = 0; i < Dimensionality; i++)
					{
						const auto a = this->_matrix_Q[pos][i];
						const auto b = this->_matrix_Q[pos + 1][i];

						this->_matrix_Q[pos][i] = c * a + s * b;
						this->_matrix_Q[pos + 1][i] = c * b - s * a;
					}
				}
			}

			void special_rank_1_update()
			{
				for (size_t i = 0; i < Dimensionality; i++)
				{
					this->_w[i] = 0;

					for (size_t k = 0; k < Dimensionality; k++)
					{
						this->_w[i] += this->_matrix_Q[i][k] * this->_u[k];
					}
				}

				for (size_t k = Dimensionality - 1; k > 0; k--)
				{
					value_type c{};
					value_type s{};

					Fischer_Gartner_Kutz_subspan::givens(c, s, this->_w[k - 1], this->_w[k]);

					this->_w[k - 1] = c * this->_w[k - 1] + s * this->_w[k];

					this->_matrix_R[k - 1][k] = -s * this->_matrix_R[k - 1][k - 1];
					this->_matrix_R[k - 1][k - 1] *= c;

					for (size_t j = k; j < this->_r; j++)
					{
						const auto a = this->_matrix_R[j][k - 1];
						const auto b = this->_matrix_R[j][k];

						this->_matrix_R[j][k - 1] = c * a + s * b;
						this->_matrix_R[j][k] = c * b - s * a;
					}

					for (size_t i = 0; i < Dimensionality; ++i)
					{
						const auto a = this->_matrix_Q[k - 1][i];
						const auto b = this->_matrix_Q[k][i];

						this->_matrix_Q[k - 1][i] = c * a + s * b;
						this->_matrix_Q[k][i] = c * b - s * a;
					}
				}

				for (size_t j = 0; j < this->_r; ++j)
				{
					this->_matrix_R[j][0] += this->_w[0];
				}

				this->Hessenberg_clear(0);
			}

		public:
			void add_point(const size_t index)
			{
				if (this->is_member(index))
				{
					throw std::runtime_error(""); // TODO: Create message.
				}

				for (size_t i = 0; i < Dimensionality; i++)
				{
					this->_u[i] =
						this->_vertices[index].coordinates[i] -
							this->_vertices[this->_members[this->_r]].coordinates[i];
				}

				this->append_column();

				this->_membership[index] = true;
				this->_members[this->_r + 1] = this->_members[this->_r];
				this->_members[this->_r] = index;
				(this->_r)++;
			}

			void remove_point(const size_t index)
			{
				if (!(this->is_member(this->global_index(index)) && this->size() > 1))
				{
					throw std::runtime_error(""); // TODO: Create message.
				}

				this->_membership[this->global_index(index)] = false;

				if (index == this->_r)
				{
					for (size_t i = 0; i < Dimensionality; i++)
					{
						this->_u[i] =
							this->_vertices[this->_members[this->_r]].coordinates[i] -
								this->_vertices[this->global_index(this->_r - 1)].coordinates[i];
					}

					(this->_r)--;

					this->special_rank_1_update();

				}

				else
				{
					const auto &dummy = this->_matrix_R[index];

					for (size_t i = index + 1; i < this->_r; i++)
					{
						this->_matrix_R[i - 1] = this->_matrix_R[i];
						this->_members[i - 1] = this->_members[i];
					}

					this->_members[this->_r - 1] = this->_members[this->_r];

					(this->_r)--;

					this->_matrix_R[this->_r] = dummy;

					this->Hessenberg_clear(index);
				}
			}

			size_t size() const
			{
				return this->_r + 1;
			}

			bool is_member(const size_t index) const
			{
				if (index >= this->_vertices.size())
				{
					throw std::runtime_error(""); // TODO: Create message.
				}

				return this->_membership[index];
			}

			size_t global_index(const size_t index) const
			{
				if (index >= this->size())
				{
					throw std::runtime_error(""); // TODO: Create message.
				}

				return this->_members[index];
			}

			size_t any_member() const
			{
				if (this->size() == 0)
				{
					throw std::runtime_error(""); // TODO: Create message.
				}

				return this->_members[this->_r];
			}

			template <
				typename RandomAccessRange1,
				typename RandomAccessRange2 >
			value_type shortest_vector_to_span(
				const RandomAccessRange1 &p,
				RandomAccessRange2 &w) const
			{
				for (size_t i = 0; i < Dimensionality; i++)
				{
					w[i] = this->_vertices[this->_members[this->_r]].coordinates[i] - p[i];
				}

				for (size_t j = 0; j < this->_r; j++)
				{
					const auto scale =
						std::inner_product(
							w.cbegin(),
							w.cbegin() + Dimensionality,
							this->_matrix_Q[j].cbegin(),
							value_type{});

					for (size_t i = 0; i < Dimensionality; i++)
					{
						w[i] -= scale * this->_matrix_Q[j][i];
					}
				}

				return
					std::inner_product(
						w.cbegin(),
						w.cbegin() + Dimensionality,
						w.cbegin(),
						value_type{});
			}

			value_type representation_error()
			{
				value_type max{};
				std::vector < value_type > lambdas(this->size());

				for (size_t j = 0; j < this->size(); j++)
				{
					this->find_affine_coefficients(
						this->_vertices[this->global_index(j)].coordinates,
						lambdas.begin());

					value_type error = std::abs(lambdas[j] - value_type{1});

					if (error > max)
					{
						max = error;
					}

					for (size_t i = 0; i < j; i++)
					{
						error = std::abs(lambdas[i] - value_type{});

						if (error > max)
						{
							max = error;
						}
					}

					for (size_t i = j + 1; i < this->size(); i++)
					{
						error = std::abs(lambdas[i] - value_type{});

						if (error > max)
						{
							max = error;
						}
					}
				}

				return max;
			}

			template <
				typename RandomAccessRange1,
				typename RandomAccessRange2 >
			void find_affine_coefficients(
				const RandomAccessRange1 &p,
				RandomAccessRange2 &lambdas)
			{
				for (size_t i = 0; i < Dimensionality; i++)
				{
					this->_u[i] =
						p[i] -
							this->_vertices[this->_members[this->_r]].coordinates[i];
				}

				for (size_t i = 0; i < Dimensionality; i++)
				{
					this->_w[i] = value_type{};

					for (size_t k = 0; k < Dimensionality; k++)
					{
						this->_w[i] += this->_matrix_Q[i][k] * this->_u[k];
					}
				}

				using signedSize_t = typename std::make_signed < size_t > ::type;

				value_type originLambda = value_type{1};

				for (signedSize_t j = static_cast < signedSize_t > (this->_r) - 1;
					j >= 0;
					j--)
				{
					for (size_t k = j + 1; k < this->_r; ++k)
					{
						this->_w[j] -= lambdas[k] * this->_matrix_R[k][j];
					}

					lambdas[j] = this->_w[j] / this->_matrix_R[j][j];

					originLambda -= lambdas[j];
				}

				lambdas[this->_r] = originLambda;
			}

			Fischer_Gartner_Kutz_subspan(
				const RangeOfVertices &vertices,
				const size_t index) :

				_vertices(vertices),
				_membership(vertices.size())
			{
				for (size_t i = 0; i < Dimensionality; i++)
				{
					this->_matrix_Q[i][i] = value_type{1};
				}

				this->_members[0] = index;
				this->_membership[index] = true;
			}
		};

		template <
			typename ValueType,
			size_t Dimensionality,
			typename RangeOfVertices >
		class Fischer_Gartner_Kutz_smallest_enclosing_ball final
		{
		private:
			using value_type = typename ValueType;

			using subspan_type =
				Fischer_Gartner_Kutz_subspan < ValueType, Dimensionality, RangeOfVertices >;

			const RangeOfVertices &_vertices;
			bool _up_to_date{};
			std::array < value_type, Dimensionality > _center;
			value_type _radius{};
			value_type _radius_square{};
			std::unique_ptr < subspan_type > _support{};
			std::array < value_type, Dimensionality > _center_to_aff{};
			std::array < value_type, Dimensionality + 1 > _lambdas{};
			value_type _dist_to_aff{};
			value_type _dist_to_aff_square{};

			void initialize()
			{
				if (this->_vertices.size() == 0)
				{
					throw std::runtime_error(""); // TODO: Create message.
				}

				this->_center = this->_vertices[0].coordinates;
				this->_radius_square = value_type{};

				size_t farthestVertexIndex{};

				for (size_t j = 1; j < this->_vertices.size(); j++)
				{
					value_type distance{};

					for (size_t i = 0; i < Dimensionality; i++)
					{
						distance +=
							std::pow(
								this->_vertices[j].coordinates[i] - this->_center[i],
								value_type{2});
					}

					if (distance >= this->_radius_square)
					{
						this->_radius_square = distance;
						farthestVertexIndex = j;
					}
				}

				this->_radius = std::sqrt(this->_radius_square);

				this->_support =
					std::make_unique < subspan_type > (this->_vertices, farthestVertexIndex);
			}

			auto find_stop_fraction()
			{
				value_type scale{1};
				std::optional < size_t > stopper{};
				std::array < value_type, Dimensionality > center_to_point{};

				for (size_t j = 0; j < this->_vertices.size(); j++)
				{
					if (!this->_support->is_member(j))
					{
						for (size_t i = 0; i < Dimensionality; i++)
						{
							center_to_point[i] =
								this->_vertices[j].coordinates[i] - this->_center[i];
						}

						const auto dir_point_prod =
							std::inner_product(
								this->_center_to_aff.cbegin(),
								this->_center_to_aff.cbegin() + Dimensionality,
								center_to_point.cbegin(),
								value_type{});

						if (this->_dist_to_aff_square - dir_point_prod <
							std::numeric_limits < value_type > ::epsilon() * this->_radius * this->_dist_to_aff)
						{
							continue;
						}

						auto bound = this->_radius_square;

						bound -=
							std::inner_product(
								center_to_point.cbegin(),
								center_to_point.cbegin() + Dimensionality,
								center_to_point.cbegin(),
								value_type{});

						bound /=
							value_type{2} * (this->_dist_to_aff_square - dir_point_prod);

						if (bound > 0 && bound < scale)
						{
							scale = bound;
							stopper = j;
						}
					}
				}

				return std::make_pair(stopper, scale);
			}

			bool successful_drop()
			{
				size_t smallest{};
				value_type minimum{1};

				this->_support->find_affine_coefficients(
					this->_center,
					this->_lambdas);

				for (size_t i = 0; i < this->_support->size(); i++)
				{
					if (this->_lambdas[i] < minimum)
					{
						minimum = this->_lambdas[i];
						smallest = i;
					}
				}

				if (minimum <= value_type{})
				{
					this->_support->remove_point(smallest);

					return true;
				}

				return false;
			}

			void update()
			{
				value_type factor{1};
				this->_up_to_date = true;
				std::set < hypersphere < value_type, spatial_dimension < Dimensionality > > > history;

				this->initialize();

				while (true)
				{
					this->_dist_to_aff_square =
						this->_support->shortest_vector_to_span(
							this->_center,
							this->_center_to_aff);

					this->_dist_to_aff = std::sqrt(this->_dist_to_aff_square);

					hypersphere < value_type, spatial_dimension < Dimensionality > > hypersphere(
						vertex < value_type, spatial_dimension < Dimensionality > > (this->_center),
						this->_radius);

					if (history.contains(hypersphere))
					{
						factor++;
					}

					else
					{
						factor = value_type{1};
					}

					if (history.size() > this->_vertices.size())
					{
						return;
					}

					history.insert(hypersphere);

					while (this->_dist_to_aff <= factor * std::numeric_limits < value_type > ::epsilon() * this->_radius)
					{
						if (!this->successful_drop())
						{
							return;
						}
					}

					const auto [stopper, scale] = this->find_stop_fraction();

					if (stopper.has_value() &&
						this->_support->size() <= Dimensionality)
					{
						for (size_t i = 0; i < Dimensionality; i++)
						{
							this->_center[i] += scale * this->_center_to_aff[i];
						}

						const auto &stop_point =
							this->_vertices[this->_support->any_member()].coordinates;

						this->_radius_square = value_type{};

						for (size_t i = 0; i < Dimensionality; i++)
						{
							this->_radius_square +=
								std::pow(stop_point[i] - this->_center[i], value_type{2});
						}

						this->_radius = std::sqrt(this->_radius_square);

						this->_support->add_point(stopper.value());
					}

					else
					{
						for (size_t i = 0; i < Dimensionality; i++)
						{
							this->_center[i] += this->_center_to_aff[i];
						}

						const auto &stop_point =
							this->_vertices[this->_support->any_member()].coordinates;

						this->_radius_square = value_type{};

						for (size_t i = 0; i < Dimensionality; i++)
						{
							this->_radius_square +=
								std::pow(stop_point[i] - this->_center[i], value_type{2});
						}

						this->_radius = std::sqrt(this->_radius_square);

						if (!this->successful_drop())
						{
							return;
						}
					}
				}
			}

		public:
			bool empty() const
			{
				return this->_vertices.size() == 0;
			}

			value_type radius()
			{
				if (this->empty())
				{
					throw std::runtime_error(""); // TODO: Create message.
				}

				if (!this->_up_to_date)
				{
					this->update();
				}

				return this->_radius;
			}

			const auto &center()
			{
				if (this->empty())
				{
					throw std::runtime_error(""); // TODO: Create message.
				}

				if (!this->_up_to_date)
				{
					this->update();
				}

				return this->_center;
			}

			Fischer_Gartner_Kutz_smallest_enclosing_ball(const RangeOfVertices &vertices) :
				_vertices(vertices),
				_up_to_date(true),
				_support(nullptr)
			{
				this->update();
			}
		};

		template < std::floating_point Type >
		void HEALPix_pole(std::vector < std::pair < Type, Type > > &sphericalCoordinates, const size_t sideCount)
		{
			constexpr auto pi = std::numbers::pi_v < Type >;
			constexpr Type one{1};
			constexpr Type two{2};
			constexpr Type three{3};
			constexpr Type onePointFive{0.5};
			const auto floatingSideCount = static_cast < Type > (sideCount);

			for (size_t i = 1; i < sideCount; i++)
			{
				for (size_t j = 1; j <= 4 * i; j++)
				{
					const auto floating_i = static_cast < Type > (i);
					const auto floating_j = static_cast < Type > (j);

					sphericalCoordinates.push_back(
						std::make_pair(
							(pi / (two * floating_i)) * (floating_j - onePointFive),
							std::acos(one - ((floating_i * floating_i) / (three * floatingSideCount * floatingSideCount)))));
				}
			}
		}

		template < std::floating_point Type >
		void HEALPix_equator(
			std::vector < std::pair < Type, Type > > &sphericalCoordinates,
			size_t &verticesOnEquatorCount,
			const size_t sideCount)
		{
			constexpr auto pi = std::numbers::pi_v < Type >;
			constexpr Type one{1};
			constexpr Type two{2};
			constexpr Type three{3};
			constexpr Type four{4};
			const auto floatingSideCount = static_cast < Type > (sideCount);

			verticesOnEquatorCount = 4 * sideCount;

			for (size_t i = sideCount; i <= 2 * sideCount; i++)
			{
				const auto floating_i = static_cast < Type > (i);
				const auto s = std::fmod(floating_i - floatingSideCount + one, two) / two;

				for (size_t j = 1; j <= 4 * sideCount; j++)
				{
					const auto floating_j = static_cast < Type > (j);

					sphericalCoordinates.push_back(
						std::make_pair(
							(pi / (two * floatingSideCount)) * (floating_j - s),
							std::acos((four / three) - ((two * floating_i) / (three * floatingSideCount)))));
				}
			}
		}

		template < std::floating_point Type >
		void HEALPix_mirroring(
			std::vector < std::pair < Type, Type > > &sphericalCoordinates,
			const size_t verticesOnEquatorCount)
		{
			constexpr auto piDividedByTwo = std::numbers::pi_v < Type > / Type{2};
			const auto vertexCount = sphericalCoordinates.size() - verticesOnEquatorCount;

			for (auto &sphericalCoordinate : sphericalCoordinates)
			{
				sphericalCoordinate.second -= piDividedByTwo;
			}

			for (size_t i = 0; i < vertexCount; i++)
			{
				auto mirroredCoordinate = sphericalCoordinates[i];

				mirroredCoordinate.second = -mirroredCoordinate.second;

				sphericalCoordinates.push_back(std::move(mirroredCoordinate));
			}

			for (auto &sphericalCoordinate : sphericalCoordinates)
			{
				sphericalCoordinate.second += piDividedByTwo;
			}
		}

		template < typename ExecutionPolicy >
		consteval auto &execution_policy_selector()
		{
			if constexpr (std::same_as < ExecutionPolicy, std::execution::parallel_unsequenced_policy >)
			{
				return std::execution::par_unseq;
			}

			else if constexpr (std::same_as < ExecutionPolicy, std::execution::parallel_policy >)
			{
				return std::execution::par;
			}

			else if constexpr (std::same_as < ExecutionPolicy, std::execution::unsequenced_policy >)
			{
				return std::execution::unseq;
			}

			else
			{
				return std::execution::seq;
			}
		}
	}

	export template <
		typename ExecutionPolicy = std::execution::sequenced_policy,
		typename Range >
	requires (
		std::is_execution_policy < ExecutionPolicy > ::value &&
		std::ranges::forward_range < Range > &&
		std::ranges::sized_range < Range > &&
		std::same_as <
			std::ranges::range_value_t < Range >,
			vertex <
				typename std::ranges::range_value_t < Range > ::value_type,
				spatial_dimension < std::ranges::range_value_t < Range > ::dimensionality() > > >)
	[[nodiscard]] std::ranges::range_value_t < Range > centroid(const Range &vertices)
	{
		using VertexType = std::ranges::range_value_t < Range >;

		VertexType centroid;
		const auto size = std::ranges::size(vertices);

		if (size > 0)
		{
			centroid = std::reduce(ExecutionPolicy{}, std::ranges::cbegin(vertices), std::ranges::cend(vertices), VertexType{},
				[] (const VertexType &vertex1, const VertexType &vertex2) -> VertexType
				{
					return vertex1 + vertex2.vector_from_origin();
				});

			constexpr auto dimensionality = std::ranges::range_value_t < Range > ::dimensionality();

			for (size_t i = 0; i < dimensionality; i++)
			{
				centroid[i] /= static_cast < typename std::ranges::range_value_t < Range > ::value_type > (size);
			}
		}

		return centroid;
	}

	// https://en.wikipedia.org/wiki/N-sphere#Spherical_coordinates
	export template <
		concepts::geometric_assignable_arithmetic Type,
		size_t N >
	[[nodiscard]] vertex < Type, spatial_dimension < N + 1 > > spherical_coordinates_to_cartesian_coordinates(
		const std::array < Type, N > &angles,
		const typename vertex < Type, spatial_dimension < N + 1 > > ::abs_type &radius)
	{
		vertex < Type, spatial_dimension < N + 1 > > vertex;

		vertex[0] = radius;

		for (size_t i = 0; i < angles.size(); i++)
		{
			vertex[i + 1] = vertex[i] * std::sin(angles[i]);

			vertex[i] *= std::cos(angles[i]);
		}

		return vertex;
	}

	// https://en.wikipedia.org/wiki/N-sphere#Spherical_coordinates
	export template <
		concepts::geometric_assignable_arithmetic Type,
		size_t N >
	requires (N > 0)
	[[nodiscard]] auto cartesian_coordinates_to_spherical_coordinates(
		const vertex < Type, spatial_dimension < N > > &vertex)
	{
		std::pair < Type, std::array < Type, N - 1 > > result{};

		auto &radius = result.first;
		auto &angles = result.second;

		if constexpr (N == 1)
		{
			radius = vertex[0];
		}

		else
		{
			std::array < Type, N - 1 > denominators{};

			radius = vertex.distance_to_origin();

			denominators.back() =
				xablau::algebra::functions::pow < Type, Type > ::invoke(vertex.coordinates.back(), Type{2}) +
				xablau::algebra::functions::pow < Type, Type > ::invoke(*(vertex.coordinates.crbegin() + 1), Type{2});

			for (auto iterator = denominators.rbegin() + 1; iterator != denominators.rend(); ++iterator)
			{
				const auto vertexIndex = std::distance(iterator + 1, denominators.rend());

				*iterator =
					*(iterator - 1) + xablau::algebra::functions::pow < Type, Type > ::invoke(vertex.coordinates[vertexIndex], Type{2});
			}

			for (size_t i = 0; i < angles.size(); i++)
			{
				denominators[i] = xablau::algebra::functions::sqrt < Type > ::invoke(denominators[i]);

				if (vertex[i] < denominators[i])
				{
					angles[i] = xablau::algebra::functions::acos < Type > ::invoke(vertex[i] / denominators[i]);
				}

				else
				{
					if (vertex[i] < Type{})
					{
						angles[i] = std::numbers::pi_v < Type >;
					}
				}
			}

			if (vertex.coordinates.back() < Type{})
			{
				angles.back() = Type{2} * std::numbers::pi_v < Type > - angles.back();
			}
		}

		return result;
	}

	export template <
		typename ExecutionPolicy = std::execution::sequenced_policy,
		typename Range,
		size_t N >
	requires (
		std::is_execution_policy < ExecutionPolicy > ::value &&
		std::ranges::forward_range < Range > &&
		std::same_as <
			std::ranges::range_value_t < Range >,
			vertex <
				typename std::ranges::range_value_t < Range > ::value_type,
				spatial_dimension < std::ranges::range_value_t < Range > ::dimensionality() > > > &&
		N <= std::ranges::range_value_t < Range > ::dimensionality())
	[[nodiscard]] void translate_to(
		Range &vertices,
		const std::array < typename std::ranges::range_value_t < Range > ::value_type, N > &coordinates)
	{
		std::for_each(
			internals::execution_policy_selector < ExecutionPolicy > (),
			std::ranges::begin(vertices),
			std::ranges::end(vertices),
			[&coordinates] (std::ranges::range_value_t < Range > &vertex) -> void
			{
				vertex.translate_to(coordinates);
			});
	}

	export template <
		typename ExecutionPolicy = std::execution::sequenced_policy,
		typename Range,
		typename VectorType >
	requires (
		std::is_execution_policy < ExecutionPolicy > ::value &&
		std::ranges::forward_range < Range > && (
			std::same_as <
				std::ranges::range_value_t < Range >,
				vertex <
					typename std::ranges::range_value_t < Range > ::value_type,
					spatial_dimension < std::ranges::range_value_t < Range > ::dimensionality() > > > ||
			std::same_as <
				std::ranges::range_value_t < Range >,
				vector <
					typename std::ranges::range_value_t < Range > ::value_type,
					spatial_dimension < std::ranges::range_value_t < Range > ::dimensionality() > > >))
	void translate(
		Range &geometric_objects,
		const VectorType &vector)
	{
		std::for_each(
			internals::execution_policy_selector < ExecutionPolicy > (),
			std::ranges::begin(geometric_objects),
			std::ranges::end(geometric_objects),
			[&vector] (std::ranges::range_value_t < Range > &geometric_object) -> void
			{
				geometric_object += vector;
			});
	}

	export template <
		typename ExecutionPolicy = std::execution::sequenced_policy,
		typename Range,
		typename CoefficientType >
	requires (
		std::is_execution_policy < ExecutionPolicy > ::value &&
		std::ranges::forward_range < Range > && (
			std::same_as <
				std::ranges::range_value_t < Range >,
				vertex <
					typename std::ranges::range_value_t < Range > ::value_type,
					spatial_dimension < std::ranges::range_value_t < Range > ::dimensionality() > > > ||
			std::same_as <
				std::ranges::range_value_t < Range >,
				vector <
					typename std::ranges::range_value_t < Range > ::value_type,
					spatial_dimension < std::ranges::range_value_t < Range > ::dimensionality() > > >))
	void scale_by(
		Range &geometric_objects,
		const CoefficientType &coefficient)
	{
		std::for_each(
			internals::execution_policy_selector < ExecutionPolicy > (),
			std::ranges::begin(geometric_objects),
			std::ranges::end(geometric_objects),
			[&coefficient] (std::ranges::range_value_t < Range > &geometric_object) -> void
			{
				geometric_object.scale_by(coefficient);
			});
	}

	export template <
		typename ExecutionPolicy = std::execution::sequenced_policy,
		typename Range >
	requires (
		std::is_execution_policy < ExecutionPolicy > ::value &&
		std::ranges::forward_range < Range > && (
			std::same_as <
				std::ranges::range_value_t < Range >,
				vertex < typename std::ranges::range_value_t < Range > ::value_type, spatial_dimension < 3 > > > ||
			std::same_as <
				std::ranges::range_value_t < Range >,
				vector < typename std::ranges::range_value_t < Range > ::value_type, spatial_dimension < 3 > > >))
	void rotate_around_X_axis(
		Range &geometric_objects,
		const typename std::ranges::range_value_t < Range > ::abs_type &angle)
	{
		std::for_each(
			internals::execution_policy_selector < ExecutionPolicy > (),
			std::ranges::begin(geometric_objects),
			std::ranges::end(geometric_objects),
			[&angle] (std::ranges::range_value_t < Range > &geometric_object) -> void
			{
				geometric_object.rotate_around_X_axis(angle);
			});
	}

	export template <
		typename ExecutionPolicy = std::execution::sequenced_policy,
		typename Range >
	requires (
		std::is_execution_policy < ExecutionPolicy > ::value &&
		std::ranges::forward_range < Range > && (
			std::same_as <
				std::ranges::range_value_t < Range >,
				vertex < typename std::ranges::range_value_t < Range > ::value_type, spatial_dimension < 3 > > > ||
			std::same_as <
				std::ranges::range_value_t < Range >,
				vector < typename std::ranges::range_value_t < Range > ::value_type, spatial_dimension < 3 > > >))
	void rotate_around_Y_axis(
		Range &geometric_objects,
		const typename std::ranges::range_value_t < Range > ::abs_type &angle)
	{
		std::for_each(
			internals::execution_policy_selector < ExecutionPolicy > (),
			std::ranges::begin(geometric_objects),
			std::ranges::end(geometric_objects),
			[&angle] (std::ranges::range_value_t < Range > &geometric_object) -> void
			{
				geometric_object.rotate_around_Y_axis(angle);
			});
	}

	export template <
		typename ExecutionPolicy = std::execution::sequenced_policy,
		typename Range >
	requires (
		std::is_execution_policy < ExecutionPolicy > ::value &&
		std::ranges::forward_range < Range > && (
			std::same_as <
				std::ranges::range_value_t < Range >,
				vertex < typename std::ranges::range_value_t < Range > ::value_type, spatial_dimension < 2 > > > ||
			std::same_as <
				std::ranges::range_value_t < Range >,
				vector < typename std::ranges::range_value_t < Range > ::value_type, spatial_dimension < 2 > > > ||
			std::same_as <
				std::ranges::range_value_t < Range >,
				vertex < typename std::ranges::range_value_t < Range > ::value_type, spatial_dimension < 3 > > > ||
			std::same_as <
				std::ranges::range_value_t < Range >,
				vector < typename std::ranges::range_value_t < Range > ::value_type, spatial_dimension < 3 > > >))
	void rotate_around_Z_axis(
		Range &geometric_objects,
		const typename std::ranges::range_value_t < Range > ::abs_type &angle)
	{
		std::for_each(
			internals::execution_policy_selector < ExecutionPolicy > (),
			std::ranges::begin(geometric_objects),
			std::ranges::end(geometric_objects),
			[&angle] (std::ranges::range_value_t < Range > &geometric_object) -> void
			{
				geometric_object.rotate_around_Z_axis(angle);
			});
	}

	export template <
		typename ExecutionPolicy = std::execution::sequenced_policy,
		typename Range,
		typename AxisType >
	requires (
		std::is_execution_policy < ExecutionPolicy > ::value &&
		std::ranges::forward_range < Range > && (
			std::same_as <
				std::ranges::range_value_t < Range >,
				vertex < typename std::ranges::range_value_t < Range > ::value_type, spatial_dimension < 3 > > > ||
			std::same_as <
				std::ranges::range_value_t < Range >,
				vector < typename std::ranges::range_value_t < Range > ::value_type, spatial_dimension < 3 > > >))
	void rotate_around_arbitrary_axis(
		Range &geometric_objects,
		const AxisType &axis,
		const typename std::ranges::range_value_t < Range > ::abs_type &angle)
	{
		std::for_each(
			internals::execution_policy_selector < ExecutionPolicy > (),
			std::ranges::begin(geometric_objects),
			std::ranges::end(geometric_objects),
			[&axis, &angle] (std::ranges::range_value_t < Range > &geometric_object) -> void
			{
				geometric_object.rotate_around_arbitrary_axis(axis, angle);
			});
	}

	export template <
		typename ExecutionPolicy = std::execution::sequenced_policy,
		typename Range,
		typename AxisType,
		typename ReferenceType >
	requires (
		std::is_execution_policy < ExecutionPolicy > ::value &&
		std::ranges::forward_range < Range > &&
		std::same_as <
			std::ranges::range_value_t < Range >,
			vertex < typename std::ranges::range_value_t < Range > ::value_type, spatial_dimension < 3 > > >)
	void rotate_around_arbitrary_axis(
		Range &geometric_objects,
		const AxisType &axis,
		const ReferenceType &reference,
		const typename std::ranges::range_value_t < Range > ::abs_type &angle)
	{
		std::for_each(
			internals::execution_policy_selector < ExecutionPolicy > (),
			std::ranges::begin(geometric_objects),
			std::ranges::end(geometric_objects),
			[&axis, &reference, &angle] (std::ranges::range_value_t < Range > &geometric_object) -> void
			{
				geometric_object.rotate_around_arbitrary_axis(axis, reference, angle);
			});
	}

	export template < std::floating_point Type >
	[[nodiscard]] constexpr Type radians_to_degrees(const Type angle) noexcept
	{
		return (angle * Type{180}) / std::numbers::pi_v < Type >;
	}

	export template < std::floating_point Type >
	[[nodiscard]] constexpr Type degrees_to_radians(const Type angle) noexcept
	{
		return (angle * std::numbers::pi_v < Type >) / Type{180};
	}

	export template <
		typename ExecutionPolicy = std::execution::sequenced_policy,
		typename Range >
	requires (
		std::is_execution_policy < ExecutionPolicy > ::value &&
		std::ranges::forward_range < Range > &&
		std::same_as <
			std::ranges::range_value_t < Range >,
			vector <
				typename std::ranges::range_value_t < Range > ::value_type,
				spatial_dimension < std::ranges::range_value_t < Range > ::dimensionality() > > >)
	[[nodiscard]] std::ranges::range_value_t < Range > resultant_vector(const Range &vectors)
	{
		using VectorType = std::ranges::range_value_t < Range >;

		if constexpr (xablau::algebra::associative_and_commutative_addition < typename std::ranges::range_value_t < Range > ::value_type > ::value)
		{
			return std::reduce(internals::execution_policy_selector < ExecutionPolicy > (), std::ranges::begin(vectors), std::ranges::end(vectors), VectorType{});
		}

		else
		{
			return std::accumulate(internals::execution_policy_selector < ExecutionPolicy > (), std::ranges::begin(vectors), std::ranges::end(vectors), VectorType{});
		}
	}

	// https://en.wikipedia.org/wiki/Hammer_projection
	// Implicitly, the radius is equal to 1.
	// Implicitly, the central meridian is equal to pi.
	// latitude -> [-pi / 2, pi / 2]
	// longitude -> [-pi, pi[
	// Returns:
	//   vertex.x() -> [-2 * sqrt(2), 2 * sqrt(2)[
	//   vertex.y() -> [-sqrt(2), sqrt(2)]
	export template < std::floating_point Type >
	[[nodiscard]] vertex < Type, spatial_dimension < 2 > > Hammer_projection(const Type latitude, const Type longitude)
	{
		constexpr auto one = Type{1};
		constexpr auto two = Type{2};
		const auto denominator = std::sqrt(one + std::cos(latitude) * std::cos(longitude / two));

		vertex < Type, spatial_dimension < 2 > > result;

		result.x() = (two * std::sqrt(two) * std::cos(latitude) * std::sin(longitude / two)) / denominator;
		result.y() = (std::sqrt(two) * std::sin(latitude)) / denominator;

		return result;
	}

	// https://en.wikipedia.org/wiki/Hammer_projection
	// vertex.x() -> [-2 * sqrt(2), 2 * sqrt(2)[
	// vertex.y() -> [-sqrt(2), sqrt(2)]
	// Returns:
	//   .first -> latitude ([-pi / 2, pi / 2])
	//   .second -> longitude ([-pi, pi[)
	export template < std::floating_point Type, size_t N >
	requires (N <= 2)
	[[nodiscard]] std::pair < Type, Type > inverse_Hammer_projection(const vertex < Type, spatial_dimension < N > > &vertex) noexcept
	{
		constexpr auto one = Type{1};
		constexpr auto two = Type{2};
		constexpr auto four = Type{4};
		const Type z = std::sqrt(one - std::pow(vertex.x() / four, two) - std::pow(vertex.y() / two, two));

		return
			std::pair < Type, Type > (
				std::asin(z * vertex.y()),
				two * std::atan((z * vertex.x()) / (four * std::pow(z, two) - two)));
	}

	// https://en.wikipedia.org/wiki/Mollweide_projection
	// latitude -> [-pi / 2, pi / 2]
	// longitude -> [-pi, pi[
	// Returns (using centralMeridian = std::numbers::pi_v < Type >):
	//   vertex.x() -> [radius * -2 * sqrt(2), radius * 2 * sqrt(2)[
	//   vertex.y() -> [radius * -sqrt(2), radius * sqrt(2)]
	export template < std::floating_point Type >
	[[nodiscard]] vertex < Type, spatial_dimension < 2 > > Mollweide_projection(
		const Type latitude,
		const Type longitude,
		const size_t maxIterations,
		const Type centralMeridian = Type{},
		const Type radius = Type{1})
	{
		constexpr auto two = Type{2};
		constexpr auto piDividedByTwo = std::numbers::pi_v < Type > / two;
		auto theta = latitude;

		for (size_t i = 0; i < maxIterations; i++)
		{
			if (std::abs(theta) == piDividedByTwo)
			{
				break;
			}

			const auto numerator = two * theta + std::sin(two * theta) - std::numbers::pi_v < Type > * std::sin(latitude);
			const auto denominator = two + two * std::cos(two * theta);

			theta -= numerator / denominator;
		}

		vertex < Type, spatial_dimension < 2 > > result;

		result.x() = radius * (two * std::sqrt(two) / std::numbers::pi_v < Type >) * (longitude - centralMeridian) * std::cos(theta);
		result.y() = radius * std::sqrt(two) * std::sin(theta);

		return result;
	}

	// https://en.wikipedia.org/wiki/Mollweide_projection
	// Using centralMeridian = 0.
	// vertex.x() -> [radius * -2 * sqrt(2), radius * 2 * sqrt(2)[
	// vertex.y() -> [radius * -sqrt(2), radius * sqrt(2)]
	// Returns:
	//   .first -> latitude ([-pi / 2, pi / 2])
	//   .second -> longitude ([-pi, pi[)
	export template < std::floating_point Type, size_t N >
	requires (N <= 2)
	[[nodiscard]] std::pair < Type, Type > inverse_Mollweide_projection(
		const vertex < Type, spatial_dimension < N > > &vertex,
		const Type centralMeridian = Type{},
		const Type radius = Type{1})
	{
		constexpr auto two = Type{2};
		const auto theta = std::asin(vertex.y() / (radius * std::sqrt(two)));

		return
			std::pair < Type, Type > (
				std::asin((two * theta + std::sin(two * theta)) / std::numbers::pi_v < Type >),
				centralMeridian + ((std::numbers::pi_v < Type > * vertex.x()) / (two * radius * std::sqrt(two) * std::cos(theta))));
	}

	// https://www.cmu.edu/biolphys/deserno/pdf/sphere_equi.pdf
	export template < std::floating_point Type >
	[[nodiscard]] std::vector < vertex < Type, spatial_dimension < 3 > > > Deserno_sphere_surface_vertices(
		const hypersphere < Type, spatial_dimension < 3 > > &sphere,
		const size_t vertexCount)
	{
		constexpr auto pi = std::numbers::pi_v < Type >;
		const auto a = (Type{4} * pi) / static_cast < Type > (vertexCount);
		const auto d = std::sqrt(a);
		const auto M_theta = static_cast < size_t > (std::round(pi / d));
		const auto d_theta = pi / static_cast < Type > (M_theta);
		const auto d_phi = a / d_theta;
		std::vector < vertex < Type, spatial_dimension < 3 > > > vertices;

		vertices.reserve(vertexCount);

		for (size_t i = 0; i < M_theta; i++)
		{
			const auto theta = pi * (i + Type{0.5}) / static_cast < Type > (M_theta);
			const auto M_phi = static_cast < size_t > (std::round((Type{2} * pi * std::sin(theta)) / d_phi));

			for (size_t j = 0; j < M_phi; j++)
			{
				const auto phi = (Type{2} * pi * j) / static_cast < Type > (M_phi);

				vertices.push_back(
					spherical_coordinates_to_cartesian_coordinates(std::to_array({ theta, phi }), sphere.radius));

				std::swap(vertices.back().x(), vertices.back().z());
			}
		}

		translate(vertices, sphere.center.vector_from_origin());

		return vertices;
	}

	// https://scholar.rose-hulman.edu/cgi/viewcontent.cgi?article=1387&context=rhumj
	export template < std::floating_point Type >
	[[nodiscard]] std::vector < vertex < Type, spatial_dimension < 3 > > > Kogan_sphere_surface_vertices(
		const hypersphere < Type, spatial_dimension < 3 > > &sphere,
		const size_t vertexCount)
	{
		const auto floatingVertexCount = static_cast < Type > (vertexCount);
		constexpr Type one{1};
		constexpr Type two{2};
		constexpr auto onePointTwo = Type{1.2};
		constexpr auto zeroPointOne = Type{0.1};
		constexpr auto pi = std::numbers::pi_v < Type >;
		const auto start = -one + one / (floatingVertexCount - one);
		const auto step = (two - two / (floatingVertexCount - one)) / (floatingVertexCount - one);
		const auto x = zeroPointOne + onePointTwo * floatingVertexCount;
		std::vector < vertex < Type, spatial_dimension < 3 > > > vertices;

		for (size_t i = 0; i < vertexCount; i++)
		{
			const auto s = start + static_cast < Type > (i) * step;
			const auto angle1 = s * x;
			const auto angle2 = pi / two * std::copysign(one, s) * (one - std::sqrt(one - std::abs(s)));

			vertex < Type, spatial_dimension < 3 > > vertex;

			vertex.x() = sphere.radius * std::cos(angle1) * std::cos(angle2);
			vertex.y() = sphere.radius * std::sin(angle1) * std::cos(angle2);
			vertex.z() = sphere.radius * std::sin(angle2);

			vertices.push_back(std::move(vertex));
		}

		translate(vertices, sphere.center.vector_from_origin());

		return vertices;
	}

	// https://arxiv.org/pdf/astro-ph/0409513.pdf
	export template < std::floating_point Type >
	[[nodiscard]] std::vector < vertex < Type, spatial_dimension < 3 > > > HEALPix_sphere_surface_vertices(
		const hypersphere < Type, spatial_dimension < 3 > > &sphere,
		const size_t sideCount)
	{
		size_t verticesOnEquatorCount{};
		std::vector < std::pair < Type, Type > > sphericalCoordinates;
		std::vector < vertex < Type, spatial_dimension < 3 > > > vertices;

		sphericalCoordinates.reserve(3 * sideCount);
		vertices.reserve(3 * sideCount);

		internals::HEALPix_pole(sphericalCoordinates, sideCount);
		internals::HEALPix_equator(sphericalCoordinates, verticesOnEquatorCount, sideCount);
		internals::HEALPix_mirroring(sphericalCoordinates, verticesOnEquatorCount);

		for (const auto &sphericalCoordinate : sphericalCoordinates)
		{
			const std::array < Type, 2 > angles{ sphericalCoordinate.second, sphericalCoordinate.first };

			vertices.push_back(spherical_coordinates_to_cartesian_coordinates(angles, sphere.radius));

			std::swap(vertices.back().x(), vertices.back().z());
		}

		translate(vertices, sphere.center.vector_from_origin());

		return vertices;
	}

	// https://perswww.kuleuven.be/~u0017946/publications/Papers97/art97a-Saff-Kuijlaars-MI/Saff-Kuijlaars-MathIntel97.pdf
	export template < std::floating_point Type >
	[[nodiscard]] std::vector < vertex < Type, spatial_dimension < 3 > > > Saff_Kuijlaars_sphere_surface_vertices(
		const hypersphere < Type, spatial_dimension < 3 > > &sphere,
		const size_t vertexCount)
	{
		Type azimuthalAngle{};
		std::vector < vertex < Type, spatial_dimension < 3 > > > vertices;
		const auto floatingVertexCount = static_cast < Type > (vertexCount);
		constexpr Type one{1};
		constexpr Type two{2};
		constexpr auto threePointSix = Type{3.6};
		constexpr auto piTimesTwo = std::numbers::pi_v < Type > * Type{2};

		vertices.reserve(vertexCount);

		for (size_t i = 1; i <= vertexCount; i++)
		{
			const auto floating_i = static_cast < Type > (i);
			const auto h = -one + (two * (floating_i - one)) / (floatingVertexCount - one);
			const auto polarAngle = std::acos(h);

			if (i > 1 && i < vertexCount)
			{
				azimuthalAngle =
					std::fmod(
						azimuthalAngle + threePointSix / std::sqrt(floatingVertexCount * (one - h * h)),
						piTimesTwo);
			}

			const std::array < Type, 2 > angles{ polarAngle, azimuthalAngle };

			vertices.push_back(spherical_coordinates_to_cartesian_coordinates(angles, sphere.radius));

			std::swap(vertices.back().x(), vertices.back().z());
		}

		translate(vertices, sphere.center.vector_from_origin());

		return vertices;
	}

	export template < std::floating_point Type >
	[[nodiscard]] std::vector < vertex < Type, spatial_dimension < 3 > > > Rusin_sphere_surface_vertices(
		const hypersphere < Type, spatial_dimension < 3 > > &sphere,
		const size_t vertexCount)
	{
		size_t n{};
		constexpr Type one{1};
		constexpr Type two{2};
		constexpr auto pi = std::numbers::pi_v < Type >;

		for (n = 1; n < vertexCount; n++)
		{
			const auto _n = static_cast < Type > (n);
			const auto expectedVertices = static_cast < size_t > (two - ((two * _n * std::sin(pi / _n)) / (std::cos(pi / _n) - one)));

			if (expectedVertices >= vertexCount)
			{
				break;
			}
		}

		const auto polarAngle = pi / static_cast < Type > (n);
		std::vector < vertex < Type, spatial_dimension < 3 > > > vertices;

		vertices.push_back(vertex < Type, spatial_dimension < 3 > > (std::to_array({ Type{}, Type{}, Type{1} })));

		for (size_t i = 1; i < n; i++)
		{
			const auto _i = static_cast < Type > (i);
			const auto azimuthalRadius = std::sin(_i * polarAngle);
			const auto circleLength = two * pi * azimuthalRadius;
			const auto pointCountPerCircle = static_cast < size_t > (std::floor(circleLength / polarAngle));
			const auto azimuthalAngle = (two * pi) / pointCountPerCircle;
			const auto z = std::cos(_i * polarAngle);

			for (size_t j = 0; j < pointCountPerCircle; j++)
			{
				const auto _j = static_cast < Type > (j);
				const auto x = std::cos(_j * azimuthalAngle) * azimuthalRadius;
				const auto y = std::sin(_j * azimuthalAngle) * azimuthalRadius;

				vertices.push_back(vertex < Type, spatial_dimension < 3 > > (std::to_array({ x, y, z })));
			}
		}

		vertices.push_back(vertex < Type, spatial_dimension < 3 > > (std::to_array({ Type{}, Type{}, Type{-1} })));

		scale_by(vertices, sphere.radius);
		translate(vertices, sphere.center.vector_from_origin());

		return vertices;
	}

	export template <
		typename ExecutionPolicy = std::execution::sequenced_policy,
		typename Range >
	requires (
		std::is_execution_policy < ExecutionPolicy > ::value &&
		std::ranges::forward_range < Range > &&
		std::ranges::sized_range < Range > &&
		std::same_as <
			std::ranges::range_value_t < Range >,
			vertex <
				typename std::ranges::range_value_t < Range > ::value_type,
				spatial_dimension < std::ranges::range_value_t < Range > ::dimensionality() > > >)
	[[nodiscard]] auto arithmetic_mean_vertex_center_bounding_hypersphere(const Range &vertices)
	{
		typename std::ranges::range_value_t < Range > ::abs_type radius{};
		auto center = centroid < ExecutionPolicy > (vertices);

		for (const auto &vertex : vertices)
		{
			radius =
				xablau::algebra::functions::max <
					typename std::ranges::range_value_t < Range > ::abs_type,
					typename std::ranges::range_value_t < Range > ::abs_type > ::invoke(
						radius,
						center.distance_to(vertex));
		}

		return make_hypersphere(std::move(center), std::move(radius));
	}

	// https://link.springer.com/chapter/10.1007/978-3-540-39658-1_57
	// Shamelessly adapted from: https://github.com/hbf/miniball
	export template < bool RandomizeVertices, typename Range >
	requires (
		std::ranges::random_access_range < Range > &&
		std::ranges::sized_range < Range > &&
		std::same_as <
			std::ranges::range_value_t < Range >,
			vertex <
				typename std::ranges::range_value_t < Range > ::value_type,
				spatial_dimension < std::ranges::range_value_t < Range > ::dimensionality() > > > &&
		std::floating_point < typename std::ranges::range_value_t < Range > ::value_type >)
	[[nodiscard]] auto Fischer_Gartner_Kutz_bounding_hypersphere(const Range &vertices)
	{
		using HypersphereType =
			xablau::geometry::hypersphere <
				typename std::ranges::range_value_t < Range > ::value_type,
				spatial_dimension < std::ranges::range_value_t < Range > ::dimensionality() > >;

		using RadiusType = typename HypersphereType::abs_type;

		xablau::geometry::hypersphere <
			typename std::ranges::range_value_t < Range > ::value_type,
			spatial_dimension < std::ranges::range_value_t < Range > ::dimensionality() > > result{};

		if constexpr (std::ranges::range_value_t < Range > ::dimensionality() > 0)
		{
			if (vertices.size() == 0)
			{
				return result;
			}

			if constexpr (RandomizeVertices)
			{
				// BUG: Investigate why the order of vertices impacts so much on the result and remove this "gambiarra".
				std::random_device randomDevice;
				std::default_random_engine engine(randomDevice());
				auto _vertices = vertices;

				std::ranges::shuffle(_vertices, engine);

				internals::Fischer_Gartner_Kutz_smallest_enclosing_ball <
					typename std::ranges::range_value_t < Range > ::value_type,
					std::ranges::range_value_t < Range > ::dimensionality(),
					Range > seb(_vertices);

				result.radius = seb.radius();
				result.center = seb.center();
			}

			else
			{
				internals::Fischer_Gartner_Kutz_smallest_enclosing_ball <
					typename std::ranges::range_value_t < Range > ::value_type,
					std::ranges::range_value_t < Range > ::dimensionality(),
					Range > seb(vertices);

				result.radius = seb.radius();
				result.center = seb.center();
			}
		}

		return result;
	}
}