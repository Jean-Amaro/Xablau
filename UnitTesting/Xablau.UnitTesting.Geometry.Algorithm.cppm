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

#include <gtest/gtest.h>

import std;

import xablau.geometry;
import xablau.testing;

namespace xablau::unit_testing
{
	template < xablau::geometry::concepts::geometric_assignable_arithmetic Type >
	struct GeometryAlgorithmTypeDefinitions
	{
		using value_type = Type;
	};

	template < typename Types >
	class GeometryAlgorithmTest : public ::testing::Test
	{
	public:
		static void batchCoordinatesAreAlmostEqual(const auto &vectorOfCoordinates, const auto &coordinates2)
		{
			for (const auto &coordinates1 : vectorOfCoordinates)
			{
				for (size_t i = 0; i < coordinates1.coordinates.size(); i++)
				{
					EXPECT_LT(std::abs(coordinates1.coordinates[i] - coordinates2[i]), typename Types::value_type{0.00001});
				}
			}
		}

		static void coordinatesAreAlmostEqual(const auto &coordinates1, const auto &coordinates2)
		{
			for (size_t i = 0; i < coordinates1.coordinates.size(); i++)
			{
				EXPECT_LT(std::abs(coordinates1.coordinates[i] - coordinates2[i]), typename Types::value_type{0.00001});
			}
		}

		template < typename GeometricObjectType >
		static void translation()
		{
			std::array < GeometricObjectType, 4 > geometricObjects{
				GeometricObjectType(std::to_array({typename Types::value_type{1}})),
				GeometricObjectType(std::to_array({typename Types::value_type{0}, typename Types::value_type{1}})),
				GeometricObjectType(std::to_array({typename Types::value_type{0}, typename Types::value_type{0}, typename Types::value_type{1}})),
				GeometricObjectType(std::to_array({typename Types::value_type{0}, typename Types::value_type{0}, typename Types::value_type{0}, typename Types::value_type{1}}))};

			xablau::geometry::algorithm::translate(
				geometricObjects,
				xablau::geometry::make_vector(std::to_array({typename Types::value_type{1}, typename Types::value_type{1}, typename Types::value_type{1}, typename Types::value_type{1}})));

			EXPECT_EQ(geometricObjects[0], GeometricObjectType(std::to_array({typename Types::value_type{2}, typename Types::value_type{1}, typename Types::value_type{1}, typename Types::value_type{1}})));
			EXPECT_EQ(geometricObjects[1], GeometricObjectType(std::to_array({typename Types::value_type{1}, typename Types::value_type{2}, typename Types::value_type{1}, typename Types::value_type{1}})));
			EXPECT_EQ(geometricObjects[2], GeometricObjectType(std::to_array({typename Types::value_type{1}, typename Types::value_type{1}, typename Types::value_type{2}, typename Types::value_type{1}})));
			EXPECT_EQ(geometricObjects[3], GeometricObjectType(std::to_array({typename Types::value_type{1}, typename Types::value_type{1}, typename Types::value_type{1}, typename Types::value_type{2}})));
		}

		template < typename GeometricObjectType >
		static void scaling()
		{
			GeometricObjectType geometricObject =
				std::to_array(
					{
						typename Types::value_type(1),
						typename Types::value_type(2),
						typename Types::value_type(4),
						typename Types::value_type(8)
					});

			std::vector < GeometricObjectType > geometricObjects(100, geometricObject);

			xablau::geometry::algorithm::scale_by < std::execution::sequenced_policy > (
				geometricObjects,
				typename Types::value_type{2});

			GeometryAlgorithmTest::batchCoordinatesAreAlmostEqual(
				geometricObjects,
				std::to_array(
					{
						typename Types::value_type{2},
						typename Types::value_type{4},
						typename Types::value_type{8},
						typename Types::value_type{16}
					}));

			xablau::geometry::algorithm::scale_by < std::execution::unsequenced_policy > (
				geometricObjects,
				typename Types::value_type{0.5});

			GeometryAlgorithmTest::batchCoordinatesAreAlmostEqual(
				geometricObjects,
				std::to_array(
					{
						typename Types::value_type{1},
						typename Types::value_type{2},
						typename Types::value_type{4},
						typename Types::value_type{8}
					}));

			xablau::geometry::algorithm::scale_by < std::execution::parallel_policy > (
				geometricObjects,
				std::to_array(
					{
						typename Types::value_type{2},
						typename Types::value_type{2},
						typename Types::value_type{2},
						typename Types::value_type{2}
					}));

			GeometryAlgorithmTest::batchCoordinatesAreAlmostEqual(
				geometricObjects,
				std::to_array(
					{
						typename Types::value_type{2},
						typename Types::value_type{4},
						typename Types::value_type{8},
						typename Types::value_type{16}
					}));

			xablau::geometry::algorithm::scale_by < std::execution::parallel_unsequenced_policy > (
				geometricObjects,
				std::to_array(
					{
						static_cast < typename Types::value_type > (0.5),
						static_cast < typename Types::value_type > (0.25),
						static_cast < typename Types::value_type > (0.125),
						static_cast < typename Types::value_type > (0.0625)
					}));

			GeometryAlgorithmTest::batchCoordinatesAreAlmostEqual(
				geometricObjects,
				std::to_array(
					{
						typename Types::value_type{1},
						typename Types::value_type{1},
						typename Types::value_type{1},
						typename Types::value_type{1}
					}));

			xablau::geometry::algorithm::scale_by < std::execution::sequenced_policy > (
				geometricObjects,
				std::to_array(
					{
						static_cast < typename Types::value_type > (1),
						static_cast < typename Types::value_type > (1)
					}));

			GeometryAlgorithmTest::batchCoordinatesAreAlmostEqual(
				geometricObjects,
				std::to_array(
					{
						typename Types::value_type{1},
						typename Types::value_type{1},
						typename Types::value_type{0},
						typename Types::value_type{0}
					}));

			xablau::geometry::algorithm::scale_by < std::execution::parallel_policy > (
				geometricObjects,
				std::to_array(
					{
						static_cast < typename Types::value_type > (1),
						static_cast < typename Types::value_type > (0),
						static_cast < typename Types::value_type > (1),
						static_cast < typename Types::value_type > (1),
						static_cast < typename Types::value_type > (1)
					}));

			GeometryAlgorithmTest::batchCoordinatesAreAlmostEqual(
				geometricObjects,
				std::to_array(
					{
						typename Types::value_type{1},
						typename Types::value_type{0},
						typename Types::value_type{0},
						typename Types::value_type{0}
					}));
		}

		template < typename GeometricObjectType >
		static void rotationAroundXAxis()
		{
			GeometricObjectType geometricObject(
				std::to_array(
					{
						typename Types::value_type{0},
						typename Types::value_type{1},
						typename Types::value_type{1}
					}));

			std::vector < GeometricObjectType > geometricObjects(100, geometricObject);

			constexpr auto ninetyDegrees =
				std::numbers::pi_v < typename Types::value_type > / typename Types::value_type{2};

			xablau::geometry::algorithm::rotate_around_X_axis < std::execution::sequenced_policy > (geometricObjects, ninetyDegrees);

			GeometryAlgorithmTest::batchCoordinatesAreAlmostEqual(
				geometricObjects,
				std::to_array(
					{
						typename Types::value_type{0},
						typename Types::value_type{-1},
						typename Types::value_type{1}
					}));

			xablau::geometry::algorithm::rotate_around_X_axis < std::execution::parallel_policy > (geometricObjects, ninetyDegrees);

			GeometryAlgorithmTest::batchCoordinatesAreAlmostEqual(
				geometricObjects,
				std::to_array(
					{
						typename Types::value_type{0},
						typename Types::value_type{-1},
						typename Types::value_type{-1}
					}));

			xablau::geometry::algorithm::rotate_around_X_axis < std::execution::unsequenced_policy > (geometricObjects, ninetyDegrees);

			GeometryAlgorithmTest::batchCoordinatesAreAlmostEqual(
				geometricObjects,
				std::to_array(
					{
						typename Types::value_type{0},
						typename Types::value_type{1},
						typename Types::value_type{-1}
					}));

			xablau::geometry::algorithm::rotate_around_X_axis < std::execution::parallel_unsequenced_policy > (geometricObjects, ninetyDegrees);

			GeometryAlgorithmTest::batchCoordinatesAreAlmostEqual(
				geometricObjects,
				std::to_array(
					{
						typename Types::value_type{0},
						typename Types::value_type{1},
						typename Types::value_type{1}
					}));
		}

		template < typename GeometricObjectType >
		static void rotationAroundYAxis()
		{
			GeometricObjectType geometricObject(
				std::to_array(
					{
						typename Types::value_type{1},
						typename Types::value_type{0},
						typename Types::value_type{1}
					}));

			std::vector < GeometricObjectType > geometricObjects(100, geometricObject);

			constexpr auto ninetyDegrees =
				std::numbers::pi_v < typename Types::value_type > / typename Types::value_type{2};

			xablau::geometry::algorithm::rotate_around_Y_axis < std::execution::sequenced_policy > (geometricObjects, ninetyDegrees);

			GeometryAlgorithmTest::batchCoordinatesAreAlmostEqual(
				geometricObjects,
				std::to_array(
					{
						typename Types::value_type{1},
						typename Types::value_type{0},
						typename Types::value_type{-1}
					}));

			xablau::geometry::algorithm::rotate_around_Y_axis < std::execution::parallel_policy > (geometricObjects, ninetyDegrees);

			GeometryAlgorithmTest::batchCoordinatesAreAlmostEqual(
				geometricObjects,
				std::to_array(
					{
						typename Types::value_type{-1},
						typename Types::value_type{0},
						typename Types::value_type{-1}
					}));

			xablau::geometry::algorithm::rotate_around_Y_axis < std::execution::unsequenced_policy > (geometricObjects, ninetyDegrees);

			GeometryAlgorithmTest::batchCoordinatesAreAlmostEqual(
				geometricObjects,
				std::to_array(
					{
						typename Types::value_type{-1},
						typename Types::value_type{0},
						typename Types::value_type{1}
					}));

			xablau::geometry::algorithm::rotate_around_Y_axis < std::execution::parallel_unsequenced_policy > (geometricObjects, ninetyDegrees);

			GeometryAlgorithmTest::batchCoordinatesAreAlmostEqual(
				geometricObjects,
				std::to_array(
					{
						typename Types::value_type{1},
						typename Types::value_type{0},
						typename Types::value_type{1}
					}));
		}

		template < typename GeometricObjectType >
		static void rotationAroundZAxis()
		{
			GeometricObjectType geometricObject(
				std::to_array(
					{
						typename Types::value_type{1},
						typename Types::value_type{1}
					}));

			std::vector < GeometricObjectType > geometricObjects(100, geometricObject);

			constexpr auto ninetyDegrees =
				std::numbers::pi_v < typename Types::value_type > / typename Types::value_type{2};

			xablau::geometry::algorithm::rotate_around_Z_axis < std::execution::sequenced_policy > (geometricObjects, ninetyDegrees);

			GeometryAlgorithmTest::batchCoordinatesAreAlmostEqual(
				geometricObjects,
				std::to_array(
					{
						typename Types::value_type{-1},
						typename Types::value_type{1}
					}));

			xablau::geometry::algorithm::rotate_around_Z_axis < std::execution::parallel_policy > (geometricObjects, ninetyDegrees);

			GeometryAlgorithmTest::batchCoordinatesAreAlmostEqual(
				geometricObjects,
				std::to_array(
					{
						typename Types::value_type{-1},
						typename Types::value_type{-1}
					}));

			xablau::geometry::algorithm::rotate_around_Z_axis < std::execution::unsequenced_policy > (geometricObjects, ninetyDegrees);

			GeometryAlgorithmTest::batchCoordinatesAreAlmostEqual(
				geometricObjects,
				std::to_array(
					{
						typename Types::value_type{1},
						typename Types::value_type{-1}
					}));

			xablau::geometry::algorithm::rotate_around_Z_axis < std::execution::parallel_unsequenced_policy > (geometricObjects, ninetyDegrees);

			GeometryAlgorithmTest::batchCoordinatesAreAlmostEqual(
				geometricObjects,
				std::to_array(
					{
						typename Types::value_type{1},
						typename Types::value_type{1}
					}));
		}

		template < typename GeometricObjectType >
		static void rotationAroundArbitraryAxis()
		{
			GeometricObjectType geometricObject(
				std::to_array(
					{
						typename Types::value_type{1},
						typename Types::value_type{1},
						typename Types::value_type{1}
					}));

			xablau::geometry::vector < typename Types::value_type, xablau::geometry::spatial_dimension < 3 > > axis(
				std::to_array(
					{
						typename Types::value_type{1},
						typename Types::value_type{-1},
						typename Types::value_type{1}
					}));

			std::vector < GeometricObjectType > geometricObjects(100, geometricObject);

			constexpr auto ninetyDegrees =
				std::numbers::pi_v < typename Types::value_type > / typename Types::value_type{2};

			xablau::geometry::algorithm::rotate_around_arbitrary_axis < std::execution::sequenced_policy > (geometricObjects, axis, ninetyDegrees);

			GeometryAlgorithmTest::batchCoordinatesAreAlmostEqual(
				geometricObjects,
				std::to_array(
					{
						-((typename Types::value_type{2} / std::sqrt(typename Types::value_type{3})) -
							(typename Types::value_type{1} / typename Types::value_type{3})),
						typename Types::value_type{-1} / typename Types::value_type{3},
						-((typename Types::value_type{-2} / std::sqrt(typename Types::value_type{3})) -
							(typename Types::value_type{1} / typename Types::value_type{3}))
					}));

			xablau::geometry::algorithm::rotate_around_arbitrary_axis < std::execution::parallel_policy > (geometricObjects, axis, ninetyDegrees);

			GeometryAlgorithmTest::batchCoordinatesAreAlmostEqual(
				geometricObjects,
				std::to_array(
					{
						typename Types::value_type{-1} / typename Types::value_type{3},
						typename Types::value_type{-5} / typename Types::value_type{3},
						typename Types::value_type{-1} / typename Types::value_type{3}
					}));

			xablau::geometry::algorithm::rotate_around_arbitrary_axis < std::execution::unsequenced_policy > (geometricObjects, axis, ninetyDegrees);

			GeometryAlgorithmTest::batchCoordinatesAreAlmostEqual(
				geometricObjects,
				std::to_array(
					{
						-((typename Types::value_type{-2} / std::sqrt(typename Types::value_type{3})) -
							(typename Types::value_type{1} / typename Types::value_type{3})),
						typename Types::value_type{-1} / typename Types::value_type{3},
						-((typename Types::value_type{2} / std::sqrt(typename Types::value_type{3})) -
							(typename Types::value_type{1} / typename Types::value_type{3}))
					}));

			xablau::geometry::algorithm::rotate_around_arbitrary_axis < std::execution::parallel_unsequenced_policy > (geometricObjects, axis, ninetyDegrees);

			GeometryAlgorithmTest::batchCoordinatesAreAlmostEqual(
				geometricObjects,
				std::to_array(
					{
						typename Types::value_type{1},
						typename Types::value_type{1},
						typename Types::value_type{1}
					}));
		}

		template < size_t Dimensionality >
		static auto verticesOnHypersphere()
		{
			std::vector <
				xablau::geometry::vertex <
					typename Types::value_type,
					xablau::geometry::spatial_dimension < Dimensionality > > > vertices(10);

			if constexpr (Dimensionality > 0)
			{
				vertices[0][0] = typename Types::value_type{-0.95562608722190012};
				vertices[1][0] = typename Types::value_type{0.37589648121585739};
				vertices[2][0] = typename Types::value_type{0.28104495376445815};
				vertices[3][0] = typename Types::value_type{0.18344676046021902};
				vertices[4][0] = typename Types::value_type{0.91930906094546350};
				vertices[5][0] = typename Types::value_type{-0.70610675374614706};
				vertices[6][0] = typename Types::value_type{0.18979461043122647};
				vertices[7][0] = typename Types::value_type{0.28714865565965764};
				vertices[8][0] = typename Types::value_type{0.46098208563493759};
				vertices[9][0] = typename Types::value_type{0.91595202490310368};
			}

			if constexpr (Dimensionality > 1)
			{
				vertices[0][1] = typename Types::value_type{-0.10635700552385019};
				vertices[1][1] = typename Types::value_type{0.29013946958830528};
				vertices[2][1] = typename Types::value_type{-0.30893887142551957};
				vertices[3][1] = typename Types::value_type{0.10769981994079414};
				vertices[4][1] = typename Types::value_type{-0.10794396801660211};
				vertices[5][1] = typename Types::value_type{0.31650746177556699};
				vertices[6][1] = typename Types::value_type{-0.49961851863155005};
				vertices[7][1] = typename Types::value_type{-0.50248725852229370};
				vertices[8][1] = typename Types::value_type{-0.64415417950987275};
				vertices[9][1] = typename Types::value_type{-0.037568285164952586};
			}

			if constexpr (Dimensionality > 2)
			{
				vertices[0][2] = typename Types::value_type{0.35856196783349104};
				vertices[1][2] = typename Types::value_type{-0.40598773155919066};
				vertices[2][2] = typename Types::value_type{-0.54753257850886561};
				vertices[3][2] = typename Types::value_type{0.053437910092471119};
				vertices[4][2] = typename Types::value_type{0.75969725638599805};
				vertices[5][2] = typename Types::value_type{-0.31101413006988743};
				vertices[6][2] = typename Types::value_type{-0.81682790612506484};
				vertices[7][2] = typename Types::value_type{-0.20328379161961729};
				vertices[8][2] = typename Types::value_type{0.36735129856257820};
				vertices[9][2] = typename Types::value_type{0.73076570940275287};
			}

			if constexpr (Dimensionality > 3)
			{
				vertices[0][3] = typename Types::value_type{-0.95385601367229222};
				vertices[1][3] = typename Types::value_type{-0.72814111758781697};
				vertices[2][3] = typename Types::value_type{-0.71764275032807401};
				vertices[3][3] = typename Types::value_type{-0.74108096560563985};
				vertices[4][3] = typename Types::value_type{0.85656300546281328};
				vertices[5][3] = typename Types::value_type{0.91814935758537564};
				vertices[6][3] = typename Types::value_type{-0.68248542741172513};
				vertices[7][3] = typename Types::value_type{-0.75615710928678248};
				vertices[8][3] = typename Types::value_type{-0.76012451551866200};
				vertices[9][3] = typename Types::value_type{-0.69316690572832429};
			}

			return vertices;
		}
	};

	TYPED_TEST_SUITE_P(GeometryAlgorithmTest);

	TYPED_TEST_P(GeometryAlgorithmTest, Centroid)
	{
		using VertexType = xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > >;

		std::array < VertexType, 27 > vertices{
			VertexType(std::to_array({typename TypeParam::value_type{-1}, typename TypeParam::value_type{-1}, typename TypeParam::value_type{-1}})),
			VertexType(std::to_array({typename TypeParam::value_type{-1}, typename TypeParam::value_type{-1}, typename TypeParam::value_type{0}})),
			VertexType(std::to_array({typename TypeParam::value_type{-1}, typename TypeParam::value_type{-1}, typename TypeParam::value_type{1}})),

			VertexType(std::to_array({typename TypeParam::value_type{-1}, typename TypeParam::value_type{0}, typename TypeParam::value_type{-1}})),
			VertexType(std::to_array({typename TypeParam::value_type{-1}, typename TypeParam::value_type{0}, typename TypeParam::value_type{0}})),
			VertexType(std::to_array({typename TypeParam::value_type{-1}, typename TypeParam::value_type{0}, typename TypeParam::value_type{1}})),

			VertexType(std::to_array({typename TypeParam::value_type{-1}, typename TypeParam::value_type{1}, typename TypeParam::value_type{-1}})),
			VertexType(std::to_array({typename TypeParam::value_type{-1}, typename TypeParam::value_type{1}, typename TypeParam::value_type{0}})),
			VertexType(std::to_array({typename TypeParam::value_type{-1}, typename TypeParam::value_type{1}, typename TypeParam::value_type{1}})),

			VertexType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{-1}, typename TypeParam::value_type{-1}})),
			VertexType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{-1}, typename TypeParam::value_type{0}})),
			VertexType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{-1}, typename TypeParam::value_type{1}})),

			VertexType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{0}, typename TypeParam::value_type{-1}})),
			VertexType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{0}, typename TypeParam::value_type{0}})),
			VertexType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{0}, typename TypeParam::value_type{1}})),

			VertexType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{1}, typename TypeParam::value_type{-1}})),
			VertexType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{1}, typename TypeParam::value_type{0}})),
			VertexType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{1}, typename TypeParam::value_type{1}})),

			VertexType(std::to_array({typename TypeParam::value_type{1}, typename TypeParam::value_type{-1}, typename TypeParam::value_type{-1}})),
			VertexType(std::to_array({typename TypeParam::value_type{1}, typename TypeParam::value_type{-1}, typename TypeParam::value_type{0}})),
			VertexType(std::to_array({typename TypeParam::value_type{1}, typename TypeParam::value_type{-1}, typename TypeParam::value_type{1}})),

			VertexType(std::to_array({typename TypeParam::value_type{1}, typename TypeParam::value_type{0}, typename TypeParam::value_type{-1}})),
			VertexType(std::to_array({typename TypeParam::value_type{1}, typename TypeParam::value_type{0}, typename TypeParam::value_type{0}})),
			VertexType(std::to_array({typename TypeParam::value_type{1}, typename TypeParam::value_type{0}, typename TypeParam::value_type{1}})),

			VertexType(std::to_array({typename TypeParam::value_type{1}, typename TypeParam::value_type{1}, typename TypeParam::value_type{-1}})),
			VertexType(std::to_array({typename TypeParam::value_type{1}, typename TypeParam::value_type{1}, typename TypeParam::value_type{0}})),
			VertexType(std::to_array({typename TypeParam::value_type{1}, typename TypeParam::value_type{1}, typename TypeParam::value_type{1}}))};

		EXPECT_EQ(
			xablau::geometry::algorithm::centroid(vertices),
			VertexType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{0}, typename TypeParam::value_type{0}})));
	}

	TYPED_TEST_P(GeometryAlgorithmTest, SphericalCartesianCoordinates)
	{
		for (typename TypeParam::value_type value1 = xablau::geometry::algorithm::degrees_to_radians(typename TypeParam::value_type{10});
			value1 <= std::numbers::pi_v < typename TypeParam::value_type > - xablau::geometry::algorithm::degrees_to_radians(typename TypeParam::value_type{10});
			value1 += xablau::geometry::algorithm::degrees_to_radians(typename TypeParam::value_type{10}))
		{
			for (typename TypeParam::value_type value2 = xablau::geometry::algorithm::degrees_to_radians(typename TypeParam::value_type{10});
				value2 < std::numbers::pi_v < typename TypeParam::value_type > - xablau::geometry::algorithm::degrees_to_radians(typename TypeParam::value_type{10});
				value2 += xablau::geometry::algorithm::degrees_to_radians(typename TypeParam::value_type{10}))
			{
				for (typename TypeParam::value_type value3 = xablau::geometry::algorithm::degrees_to_radians(typename TypeParam::value_type{10});
					value3 < typename TypeParam::value_type{2} * std::numbers::pi_v < typename TypeParam::value_type > - xablau::geometry::algorithm::degrees_to_radians(typename TypeParam::value_type{10});
					value3 += xablau::geometry::algorithm::degrees_to_radians(typename TypeParam::value_type{10}))
				{
					const auto result =
						xablau::geometry::algorithm::cartesian_coordinates_to_spherical_coordinates(
							xablau::geometry::algorithm::spherical_coordinates_to_cartesian_coordinates(
								std::to_array({ value1, value2, value3 }),
								typename TypeParam::value_type{1}));

					EXPECT_LT(std::abs(result.first - typename TypeParam::value_type{1}), typename TypeParam::value_type(0.001));
					EXPECT_LT(std::abs(result.second[0] - value1), typename TypeParam::value_type(0.001));
					EXPECT_LT(std::abs(result.second[1] - value2), typename TypeParam::value_type(0.001));
					EXPECT_LT(std::abs(result.second[2] - value3), typename TypeParam::value_type(0.001));
				}
			}
		}
	}

	TYPED_TEST_P(GeometryAlgorithmTest, VertexAbsoluteTranslation)
	{
		using VertexType = xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > >;

		std::array < VertexType, 27 > vertices{
			VertexType(std::to_array({typename TypeParam::value_type{-1}, typename TypeParam::value_type{-1}, typename TypeParam::value_type{-1}})),
			VertexType(std::to_array({typename TypeParam::value_type{-1}, typename TypeParam::value_type{-1}, typename TypeParam::value_type{0}})),
			VertexType(std::to_array({typename TypeParam::value_type{-1}, typename TypeParam::value_type{-1}, typename TypeParam::value_type{1}})),

			VertexType(std::to_array({typename TypeParam::value_type{-1}, typename TypeParam::value_type{0}, typename TypeParam::value_type{-1}})),
			VertexType(std::to_array({typename TypeParam::value_type{-1}, typename TypeParam::value_type{0}, typename TypeParam::value_type{0}})),
			VertexType(std::to_array({typename TypeParam::value_type{-1}, typename TypeParam::value_type{0}, typename TypeParam::value_type{1}})),

			VertexType(std::to_array({typename TypeParam::value_type{-1}, typename TypeParam::value_type{1}, typename TypeParam::value_type{-1}})),
			VertexType(std::to_array({typename TypeParam::value_type{-1}, typename TypeParam::value_type{1}, typename TypeParam::value_type{0}})),
			VertexType(std::to_array({typename TypeParam::value_type{-1}, typename TypeParam::value_type{1}, typename TypeParam::value_type{1}})),

			VertexType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{-1}, typename TypeParam::value_type{-1}})),
			VertexType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{-1}, typename TypeParam::value_type{0}})),
			VertexType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{-1}, typename TypeParam::value_type{1}})),

			VertexType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{0}, typename TypeParam::value_type{-1}})),
			VertexType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{0}, typename TypeParam::value_type{0}})),
			VertexType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{0}, typename TypeParam::value_type{1}})),

			VertexType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{1}, typename TypeParam::value_type{-1}})),
			VertexType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{1}, typename TypeParam::value_type{0}})),
			VertexType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{1}, typename TypeParam::value_type{1}})),

			VertexType(std::to_array({typename TypeParam::value_type{1}, typename TypeParam::value_type{-1}, typename TypeParam::value_type{-1}})),
			VertexType(std::to_array({typename TypeParam::value_type{1}, typename TypeParam::value_type{-1}, typename TypeParam::value_type{0}})),
			VertexType(std::to_array({typename TypeParam::value_type{1}, typename TypeParam::value_type{-1}, typename TypeParam::value_type{1}})),

			VertexType(std::to_array({typename TypeParam::value_type{1}, typename TypeParam::value_type{0}, typename TypeParam::value_type{-1}})),
			VertexType(std::to_array({typename TypeParam::value_type{1}, typename TypeParam::value_type{0}, typename TypeParam::value_type{0}})),
			VertexType(std::to_array({typename TypeParam::value_type{1}, typename TypeParam::value_type{0}, typename TypeParam::value_type{1}})),

			VertexType(std::to_array({typename TypeParam::value_type{1}, typename TypeParam::value_type{1}, typename TypeParam::value_type{-1}})),
			VertexType(std::to_array({typename TypeParam::value_type{1}, typename TypeParam::value_type{1}, typename TypeParam::value_type{0}})),
			VertexType(std::to_array({typename TypeParam::value_type{1}, typename TypeParam::value_type{1}, typename TypeParam::value_type{1}}))};

		xablau::geometry::algorithm::translate_to(vertices, std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{0}, typename TypeParam::value_type{0}}));

		this->batchCoordinatesAreAlmostEqual(
			vertices,
			std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{0}, typename TypeParam::value_type{0}}));
	}

	TYPED_TEST_P(GeometryAlgorithmTest, VertexTranslation)
	{
		this->translation < xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 4 > > > ();
	}

	TYPED_TEST_P(GeometryAlgorithmTest, VectorTranslation)
	{
		this->translation < xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 4 > > > ();
	}

	TYPED_TEST_P(GeometryAlgorithmTest, VertexScaling)
	{
		this->scaling < xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 4 > > > ();
	}

	TYPED_TEST_P(GeometryAlgorithmTest, VectorScaling)
	{
		this->scaling < xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 4 > > > ();
	}

	TYPED_TEST_P(GeometryAlgorithmTest, VertexRotationAroundXAxis)
	{
		this->rotationAroundXAxis < xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > > ();
	}

	TYPED_TEST_P(GeometryAlgorithmTest, VectorRotationAroundXAxis)
	{
		this->rotationAroundXAxis < xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > > ();
	}

	TYPED_TEST_P(GeometryAlgorithmTest, VertexRotationAroundYAxis)
	{
		this->rotationAroundYAxis < xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > > ();
	}

	TYPED_TEST_P(GeometryAlgorithmTest, VectorRotationAroundYAxis)
	{
		this->rotationAroundYAxis < xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > > ();
	}

	TYPED_TEST_P(GeometryAlgorithmTest, VertexRotationAroundZAxis)
	{
		this->rotationAroundZAxis < xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > > ();
	}

	TYPED_TEST_P(GeometryAlgorithmTest, VectorRotationAroundZAxis)
	{
		this->rotationAroundZAxis < xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 2 > > > ();
	}

	TYPED_TEST_P(GeometryAlgorithmTest, VertexRotationAroundArbitraryAxis)
	{
		this->rotationAroundArbitraryAxis < xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > > ();
	}

	TYPED_TEST_P(GeometryAlgorithmTest, VectorRotationAroundArbitraryAxis)
	{
		this->rotationAroundArbitraryAxis < xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > > > ();
	}

	TYPED_TEST_P(GeometryAlgorithmTest, DegreeRadiansConversion)
	{
		constexpr auto angleDegree0 = typename TypeParam::value_type{};
		constexpr auto angleDegree45 = typename TypeParam::value_type{45};
		constexpr auto angleDegree90 = typename TypeParam::value_type{90};
		constexpr auto angleDegree180 = typename TypeParam::value_type{180};
		constexpr auto angleDegree360 = typename TypeParam::value_type{360};

		constexpr auto angleRadians0 = typename TypeParam::value_type{};
		constexpr auto angleRadians45 = std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{4};
		constexpr auto angleRadians90 = std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2};
		constexpr auto angleRadians180 = std::numbers::pi_v < typename TypeParam::value_type >;
		constexpr auto angleRadians360 = typename TypeParam::value_type{2} * std::numbers::pi_v < typename TypeParam::value_type >;

		EXPECT_EQ(xablau::geometry::algorithm::degrees_to_radians(angleDegree0), angleRadians0);
		EXPECT_EQ(xablau::geometry::algorithm::degrees_to_radians(angleDegree45), angleRadians45);
		EXPECT_EQ(xablau::geometry::algorithm::degrees_to_radians(angleDegree90), angleRadians90);
		EXPECT_EQ(xablau::geometry::algorithm::degrees_to_radians(angleDegree180), angleRadians180);
		EXPECT_EQ(xablau::geometry::algorithm::degrees_to_radians(angleDegree360), angleRadians360);

		EXPECT_EQ(xablau::geometry::algorithm::radians_to_degrees(angleRadians0), angleDegree0);
		EXPECT_EQ(xablau::geometry::algorithm::radians_to_degrees(angleRadians45), angleDegree45);
		EXPECT_EQ(xablau::geometry::algorithm::radians_to_degrees(angleRadians90), angleDegree90);
		EXPECT_EQ(xablau::geometry::algorithm::radians_to_degrees(angleRadians180), angleDegree180);
		EXPECT_EQ(xablau::geometry::algorithm::radians_to_degrees(angleRadians360), angleDegree360);
	}

	TYPED_TEST_P(GeometryAlgorithmTest, ResultantVector)
	{
		using VectorType = xablau::geometry::vector < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 4 > >;

		std::array < VectorType, 4 > geometricObjects{
			VectorType(std::to_array({typename TypeParam::value_type{1}})),
			VectorType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{1}})),
			VectorType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{0}, typename TypeParam::value_type{1}})),
			VectorType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{0}, typename TypeParam::value_type{0}, typename TypeParam::value_type{1}}))};

		EXPECT_EQ(
			xablau::geometry::algorithm::resultant_vector(geometricObjects),
			VectorType(std::to_array({typename TypeParam::value_type{1}, typename TypeParam::value_type{1}, typename TypeParam::value_type{1}, typename TypeParam::value_type{1}})));
	}

	TYPED_TEST_P(GeometryAlgorithmTest, HammerProjection)
	{
		for (typename TypeParam::value_type latitude = -std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2} + xablau::geometry::algorithm::degrees_to_radians(typename TypeParam::value_type{10});
			latitude <= std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2} - xablau::geometry::algorithm::degrees_to_radians(typename TypeParam::value_type{10});
			latitude += xablau::geometry::algorithm::degrees_to_radians(typename TypeParam::value_type{10}))
		{
			for (typename TypeParam::value_type longitude = -std::numbers::pi_v < typename TypeParam::value_type > + xablau::geometry::algorithm::degrees_to_radians(typename TypeParam::value_type{10});
				longitude <= std::numbers::pi_v < typename TypeParam::value_type > - xablau::geometry::algorithm::degrees_to_radians(typename TypeParam::value_type{10});
				longitude += xablau::geometry::algorithm::degrees_to_radians(typename TypeParam::value_type{10}))
			{
				const auto result = xablau::geometry::algorithm::inverse_Hammer_projection(xablau::geometry::algorithm::Hammer_projection(latitude, longitude));

				EXPECT_LT(std::abs(result.first - latitude), typename TypeParam::value_type{0.001});
				EXPECT_LT(std::abs(result.second - longitude), typename TypeParam::value_type{0.001});
			}
		}
	}

	TYPED_TEST_P(GeometryAlgorithmTest, MollweideProjection)
	{
		for (typename TypeParam::value_type latitude = -std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2} + xablau::geometry::algorithm::degrees_to_radians(typename TypeParam::value_type{10});
			latitude <= std::numbers::pi_v < typename TypeParam::value_type > / typename TypeParam::value_type{2} - xablau::geometry::algorithm::degrees_to_radians(typename TypeParam::value_type{10});
			latitude += xablau::geometry::algorithm::degrees_to_radians(typename TypeParam::value_type{10}))
		{
			for (typename TypeParam::value_type longitude = -std::numbers::pi_v < typename TypeParam::value_type > + xablau::geometry::algorithm::degrees_to_radians(typename TypeParam::value_type{10});
				longitude <= std::numbers::pi_v < typename TypeParam::value_type > - xablau::geometry::algorithm::degrees_to_radians(typename TypeParam::value_type{10});
				longitude += xablau::geometry::algorithm::degrees_to_radians(typename TypeParam::value_type{10}))
			{
				const auto result = xablau::geometry::algorithm::inverse_Mollweide_projection(xablau::geometry::algorithm::Mollweide_projection(latitude, longitude, 1000));

				EXPECT_LT(std::abs(result.first - latitude), typename TypeParam::value_type{0.001});
				EXPECT_LT(std::abs(result.second - longitude), typename TypeParam::value_type{0.001});
			}
		}
	}

	TYPED_TEST_P(GeometryAlgorithmTest, BoundingHypersphereWithArithmeticMeanVertexCenter)
	{
		using VertexType = xablau::geometry::vertex < typename TypeParam::value_type, xablau::geometry::spatial_dimension < 3 > >;

		std::array < VertexType, 27 > vertices{
			VertexType(std::to_array({typename TypeParam::value_type{-1}, typename TypeParam::value_type{-1}, typename TypeParam::value_type{-1}})),
			VertexType(std::to_array({typename TypeParam::value_type{-1}, typename TypeParam::value_type{-1}, typename TypeParam::value_type{0}})),
			VertexType(std::to_array({typename TypeParam::value_type{-1}, typename TypeParam::value_type{-1}, typename TypeParam::value_type{1}})),

			VertexType(std::to_array({typename TypeParam::value_type{-1}, typename TypeParam::value_type{0}, typename TypeParam::value_type{-1}})),
			VertexType(std::to_array({typename TypeParam::value_type{-1}, typename TypeParam::value_type{0}, typename TypeParam::value_type{0}})),
			VertexType(std::to_array({typename TypeParam::value_type{-1}, typename TypeParam::value_type{0}, typename TypeParam::value_type{1}})),

			VertexType(std::to_array({typename TypeParam::value_type{-1}, typename TypeParam::value_type{1}, typename TypeParam::value_type{-1}})),
			VertexType(std::to_array({typename TypeParam::value_type{-1}, typename TypeParam::value_type{1}, typename TypeParam::value_type{0}})),
			VertexType(std::to_array({typename TypeParam::value_type{-1}, typename TypeParam::value_type{1}, typename TypeParam::value_type{1}})),

			VertexType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{-1}, typename TypeParam::value_type{-1}})),
			VertexType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{-1}, typename TypeParam::value_type{0}})),
			VertexType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{-1}, typename TypeParam::value_type{1}})),

			VertexType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{0}, typename TypeParam::value_type{-1}})),
			VertexType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{0}, typename TypeParam::value_type{0}})),
			VertexType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{0}, typename TypeParam::value_type{1}})),

			VertexType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{1}, typename TypeParam::value_type{-1}})),
			VertexType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{1}, typename TypeParam::value_type{0}})),
			VertexType(std::to_array({typename TypeParam::value_type{0}, typename TypeParam::value_type{1}, typename TypeParam::value_type{1}})),

			VertexType(std::to_array({typename TypeParam::value_type{1}, typename TypeParam::value_type{-1}, typename TypeParam::value_type{-1}})),
			VertexType(std::to_array({typename TypeParam::value_type{1}, typename TypeParam::value_type{-1}, typename TypeParam::value_type{0}})),
			VertexType(std::to_array({typename TypeParam::value_type{1}, typename TypeParam::value_type{-1}, typename TypeParam::value_type{1}})),

			VertexType(std::to_array({typename TypeParam::value_type{1}, typename TypeParam::value_type{0}, typename TypeParam::value_type{-1}})),
			VertexType(std::to_array({typename TypeParam::value_type{1}, typename TypeParam::value_type{0}, typename TypeParam::value_type{0}})),
			VertexType(std::to_array({typename TypeParam::value_type{1}, typename TypeParam::value_type{0}, typename TypeParam::value_type{1}})),

			VertexType(std::to_array({typename TypeParam::value_type{1}, typename TypeParam::value_type{1}, typename TypeParam::value_type{-1}})),
			VertexType(std::to_array({typename TypeParam::value_type{1}, typename TypeParam::value_type{1}, typename TypeParam::value_type{0}})),
			VertexType(std::to_array({typename TypeParam::value_type{1}, typename TypeParam::value_type{1}, typename TypeParam::value_type{1}}))};

		EXPECT_EQ(
			xablau::geometry::algorithm::arithmetic_mean_vertex_center_bounding_hypersphere(vertices),
			xablau::geometry::make_hypersphere(VertexType{}, std::sqrt(typename TypeParam::value_type{3})));
	}

	TYPED_TEST_P(GeometryAlgorithmTest, BoundingHypersphereFischerGartnerKutz)
	{
		const auto result0 =
			xablau::geometry::algorithm::Fischer_Gartner_Kutz_bounding_hypersphere < false > (
				this->verticesOnHypersphere < 0 > ());

		const auto result1 =
			xablau::geometry::algorithm::Fischer_Gartner_Kutz_bounding_hypersphere < false > (
				this->verticesOnHypersphere < 1 > ());

		const auto result2 =
			xablau::geometry::algorithm::Fischer_Gartner_Kutz_bounding_hypersphere < false > (
				this->verticesOnHypersphere < 2 > ());

		const auto result3 =
			xablau::geometry::algorithm::Fischer_Gartner_Kutz_bounding_hypersphere < false > (
				this->verticesOnHypersphere < 3 > ());

		const auto result4 =
			xablau::geometry::algorithm::Fischer_Gartner_Kutz_bounding_hypersphere < false > (
				this->verticesOnHypersphere < 4 > ());

		xablau::geometry::hypersphere <
			typename TypeParam::value_type,
			xablau::geometry::spatial_dimension < 0 > > hypersphereNull{};

		constexpr typename TypeParam::value_type radius1 = typename TypeParam::value_type{ 0.93746757408368186 };
		constexpr typename TypeParam::value_type radius2 = typename TypeParam::value_type{ 0.93746790988856354 };
		constexpr typename TypeParam::value_type radius3 = typename TypeParam::value_type{ 1.04137316400145807 };
		constexpr typename TypeParam::value_type radius4 = typename TypeParam::value_type{ 1.35262651257145272 };

		std::array < typename TypeParam::value_type, 1 > center1{ typename TypeParam::value_type{-0.01815851313821837} };
		std::array < typename TypeParam::value_type, 2 > center2{ typename TypeParam::value_type{-0.01815851313821837}, typename TypeParam::value_type{-0.10715048677022615} };
		std::array < typename TypeParam::value_type, 3 > center3{ typename TypeParam::value_type{0.06499171546452802}, typename TypeParam::value_type{-0.19177534947978087}, typename TypeParam::value_type{0.17014468078752742} };
		std::array < typename TypeParam::value_type, 4 > center4{ typename TypeParam::value_type{-0.01111209359966836}, typename TypeParam::value_type{-0.12077145528849417}, typename TypeParam::value_type{0.26333433373912074}, typename TypeParam::value_type{0.00958351204053834} };

		EXPECT_EQ(result0, hypersphereNull);

		EXPECT_LT(std::abs(result1.radius - radius1), typename TypeParam::value_type{0.00001});
		EXPECT_LT(std::abs(result2.radius - radius2), typename TypeParam::value_type{0.00001});
		EXPECT_LT(std::abs(result3.radius - radius3), typename TypeParam::value_type{0.00001});
		EXPECT_LT(std::abs(result4.radius - radius4), typename TypeParam::value_type{0.00001});

		this->coordinatesAreAlmostEqual(result1.center, center1);
		this->coordinatesAreAlmostEqual(result2.center, center2);
		this->coordinatesAreAlmostEqual(result3.center, center3);
		this->coordinatesAreAlmostEqual(result4.center, center4);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		GeometryAlgorithmTest,
		Centroid,
		SphericalCartesianCoordinates,
		VertexAbsoluteTranslation,
		VertexTranslation,
		VectorTranslation,
		VertexScaling,
		VectorScaling,
		VertexRotationAroundXAxis,
		VectorRotationAroundXAxis,
		VertexRotationAroundYAxis,
		VectorRotationAroundYAxis,
		VertexRotationAroundZAxis,
		VectorRotationAroundZAxis,
		VertexRotationAroundArbitraryAxis,
		VectorRotationAroundArbitraryAxis,
		DegreeRadiansConversion,
		ResultantVector,
		HammerProjection,
		MollweideProjection,
		BoundingHypersphereWithArithmeticMeanVertexCenter,
		BoundingHypersphereFischerGartnerKutz);

	using GeometryAlgorithmTestDataTypes = std::tuple < float, double, long double >;

	using GeometryAlgorithmTestGroup =
		xablau::testing::test_bundler < GeometryAlgorithmTestDataTypes > ::template test_group < GeometryAlgorithmTypeDefinitions >;

	using GeometryAlgorithmTypes = xablau::testing::tuple_transformation < ::testing::Types, GeometryAlgorithmTestGroup > ::types;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		Xablau,
		GeometryAlgorithmTest,
		GeometryAlgorithmTypes);
}
