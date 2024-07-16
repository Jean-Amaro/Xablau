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

import xablau.graph;
import xablau.testing;

namespace xablau::unit_testing
{
	template < bool MoveDiagonally >
	struct MoveDiagonallyWrapper
	{
		static constexpr bool move_diagonally = MoveDiagonally;
	};

	template <
		typename MoveDiagonallyWrapper,
		std::floating_point DistanceType >
	struct GeometryAlgorithmAStarTypeDefinitions
	{
		static constexpr bool move_diagonally = MoveDiagonallyWrapper::move_diagonally;
		using distance_type = DistanceType;
	};

	template < typename Types >
	class GraphAlgorithmAStarTest : public ::testing::Test
	{
	public:
		std::function < bool(const char &value) > blocked =
			[] (const char &value) -> bool
			{
				return value == char{};
			};

		static auto create2DMazeDynamic()
		{
			return
				xablau::algebra::tensor_dense_dynamic <
					char,
					xablau::algebra::tensor_rank < 2 > > (std::to_array({ size_t{5}, size_t{5} }), char{1});
		}

		static auto create3DMazeDynamic()
		{
			return
				xablau::algebra::tensor_dense_dynamic <
				char,
				xablau::algebra::tensor_rank < 3 > > (std::to_array({ size_t{3}, size_t{3}, size_t{3} }), char{1});
		}

		static auto create2DMazeFixed()
		{
			xablau::algebra::tensor_dense_fixed <
				char,
				xablau::algebra::tensor_fixed_dimensionalities < 5, 5 > > tensor{};

			tensor.fill(char{1});

			return tensor;
		}

		static auto create3DMazeFixed()
		{
			xablau::algebra::tensor_dense_fixed <
				char,
				xablau::algebra::tensor_fixed_dimensionalities < 3, 3, 3 > > tensor{};

			tensor.fill(char{1});

			return tensor;
		}

		template < typename MatrixType >
		static void blockPath2D(MatrixType &matrix)
		{
			for (size_t i = 0; i < matrix.dimensionalities()[0]; i++)
			{
				matrix(i, 2) = char{};
			}
		}

		template < typename TensorType >
		static void blockPath3D(TensorType &tensor)
		{
			for (size_t i = 0; i < tensor.dimensionalities()[0]; i++)
			{
				tensor(i, 1, 0) = char{};
				tensor(i, 1, 1) = char{};
				tensor(i, 1, 2) = char{};
			}
		}

		template < bool MoveDiagonally, typename PositionType >
		static bool checkPath(
			const PositionType &origin,
			const PositionType &destination,
			const std::vector < PositionType > &path)
		{
			if (path.size() == 0)
			{
				return false;
			}

			EXPECT_EQ(path.front(), origin);
			EXPECT_EQ(path.back(), destination);

			for (size_t i = 1; i < path.size(); i++)
			{
				bool movementOccured = false;

				for (size_t j = 0; j < path[i].size(); j++)
				{
					const size_t distance = std::max(path[i][j], path[i - 1][j]) - std::min(path[i][j], path[i - 1][j]);

					if (distance > 1)
					{
						return false;
					}

					if constexpr (!MoveDiagonally)
					{
						if (distance == 1)
						{
							if (!movementOccured)
							{
								movementOccured = true;
							}

							else
							{
								return false;
							}
						}
					}
				}
			}

			return true;
		}
	};

	TYPED_TEST_SUITE_P(GraphAlgorithmAStarTest);

	TYPED_TEST_P(GraphAlgorithmAStarTest, AStarAllOpened2D)
	{
		std::array < size_t, 2 > origin{};
		std::array < size_t, 2 > destination{ 4, 4 };
		constexpr size_t expectedPathSize = TypeParam::move_diagonally ? 5 : 9;

		const auto tensorDynamic = this->create2DMazeDynamic();
		const auto tensorFixed = this->create2DMazeFixed();

		auto path =
			xablau::graph::algorithm::template A_star < TypeParam::move_diagonally, TypeParam::distance_type > (
				tensorDynamic,
				origin,
				destination,
				this->blocked);

		EXPECT_EQ(path.size(), expectedPathSize);
		EXPECT_TRUE(this->checkPath < TypeParam::move_diagonally > (origin, destination, path));

		path =
			xablau::graph::algorithm::template A_star < TypeParam::move_diagonally, TypeParam::distance_type > (
				tensorFixed,
				origin,
				destination,
				this->blocked);

		EXPECT_EQ(path.size(), expectedPathSize);
		EXPECT_TRUE(this->checkPath < TypeParam::move_diagonally > (origin, destination, path));
	}

	TYPED_TEST_P(GraphAlgorithmAStarTest, AStarAllOpened3D)
	{
		std::array < size_t, 3 > origin{};
		std::array < size_t, 3 > destination{ 2, 2, 2 };
		constexpr size_t expectedPathSize = TypeParam::move_diagonally ? 3 : 7;

		const auto tensorDynamic = this->create3DMazeDynamic();
		const auto tensorFixed = this->create3DMazeFixed();

		auto path =
			xablau::graph::algorithm::template A_star < TypeParam::move_diagonally, TypeParam::distance_type > (
				tensorDynamic,
				origin,
				destination,
				this->blocked);

		EXPECT_EQ(path.size(), expectedPathSize);
		EXPECT_TRUE(this->checkPath < TypeParam::move_diagonally > (origin, destination, path));

		path =
			xablau::graph::algorithm::template A_star < TypeParam::move_diagonally, TypeParam::distance_type > (
				tensorFixed,
				origin,
				destination,
				this->blocked);

		EXPECT_EQ(path.size(), expectedPathSize);
		EXPECT_TRUE(this->checkPath < TypeParam::move_diagonally > (origin, destination, path));
	}

	TYPED_TEST_P(GraphAlgorithmAStarTest, AStarOriginDestinationBlocked)
	{
		std::array < size_t, 2 > origin{};
		std::array < size_t, 2 > destination{ 4, 4 };

		auto tensorDynamic = this->create2DMazeDynamic();
		auto tensorFixed = this->create2DMazeFixed();

		tensorDynamic(origin) = char{};
		tensorFixed(destination) = char{};

		EXPECT_EQ(
			(xablau::graph::algorithm::template A_star < TypeParam::move_diagonally, TypeParam::distance_type >) (
				tensorDynamic,
				origin,
				destination,
				this->blocked).size(),
			0);

		EXPECT_EQ(
			(xablau::graph::algorithm::template A_star < TypeParam::move_diagonally, TypeParam::distance_type >) (
				tensorFixed,
				origin,
				destination,
				this->blocked).size(),
			0);
	}

	TYPED_TEST_P(GraphAlgorithmAStarTest, AStarPathBlocked2D)
	{
		std::array < size_t, 2 > origin{};
		std::array < size_t, 2 > destination{ 4, 4 };

		auto tensorDynamic = this->create2DMazeDynamic();
		auto tensorFixed = this->create2DMazeFixed();

		this->blockPath2D(tensorDynamic);
		this->blockPath2D(tensorFixed);

		EXPECT_EQ(
			(xablau::graph::algorithm::template A_star < TypeParam::move_diagonally, TypeParam::distance_type >) (
				tensorDynamic,
				origin,
				destination,
				this->blocked).size(),
			0);

		EXPECT_EQ(
			(xablau::graph::algorithm::template A_star < TypeParam::move_diagonally, TypeParam::distance_type >) (
				tensorFixed,
				origin,
				destination,
				this->blocked).size(),
			0);
	}

	TYPED_TEST_P(GraphAlgorithmAStarTest, AStarPathBlocked3D)
	{
		std::array < size_t, 3 > origin{};
		std::array < size_t, 3 > destination{ 2, 2, 2 };

		auto tensorDynamic = this->create3DMazeDynamic();
		auto tensorFixed = this->create3DMazeFixed();

		this->blockPath3D(tensorDynamic);
		this->blockPath3D(tensorFixed);

		EXPECT_EQ(
			(xablau::graph::algorithm::template A_star < TypeParam::move_diagonally, TypeParam::distance_type >) (
				tensorDynamic,
				origin,
				destination,
				this->blocked).size(),
			0);

		EXPECT_EQ(
			(xablau::graph::algorithm::template A_star < TypeParam::move_diagonally, TypeParam::distance_type >) (
				tensorFixed,
				origin,
				destination,
				this->blocked).size(),
			0);
	}

	TYPED_TEST_P(GraphAlgorithmAStarTest, AStarDiagonal2D)
	{
		std::array < size_t, 2 > origin{};
		std::array < size_t, 2 > destination{ 4, 4 };

		auto tensorDynamic = this->create2DMazeDynamic();
		auto tensorFixed = this->create2DMazeFixed();

		this->blockPath2D(tensorDynamic);
		this->blockPath2D(tensorFixed);

		tensorDynamic(2, 2) = char{1};
		tensorFixed(2, 2) = char{1};

		tensorDynamic(2, 3) = char{0};
		tensorFixed(2, 3) = char{0};

		tensorDynamic(3, 3) = char{0};
		tensorFixed(3, 3) = char{0};

		if constexpr (TypeParam::move_diagonally)
		{
			auto path =
				xablau::graph::algorithm::template A_star < TypeParam::move_diagonally, TypeParam::distance_type > (
					tensorDynamic,
					origin,
					destination,
					this->blocked);

			EXPECT_EQ(path.size(), 7);
			EXPECT_TRUE(this->checkPath < TypeParam::move_diagonally > (origin, destination, path));

			path =
				xablau::graph::algorithm::template A_star < TypeParam::move_diagonally, TypeParam::distance_type > (
					tensorFixed,
					origin,
					destination,
					this->blocked);

			EXPECT_EQ(path.size(), 7);
			EXPECT_TRUE(this->checkPath < TypeParam::move_diagonally > (origin, destination, path));
		}

		else
		{
			EXPECT_EQ(
				(xablau::graph::algorithm::template A_star < TypeParam::move_diagonally, TypeParam::distance_type >) (
					tensorDynamic,
					origin,
					destination,
					this->blocked).size(),
				0);

			EXPECT_EQ(
				(xablau::graph::algorithm::template A_star < TypeParam::move_diagonally, TypeParam::distance_type >) (
					tensorFixed,
					origin,
					destination,
					this->blocked).size(),
				0);
		}
	}

	TYPED_TEST_P(GraphAlgorithmAStarTest, AStarDiagonal3D)
	{
		std::array < size_t, 3 > origin{};
		std::array < size_t, 3 > destination{ 2, 2, 0 };

		auto tensorDynamic = this->create3DMazeDynamic();
		auto tensorFixed = this->create3DMazeFixed();

		this->blockPath3D(tensorDynamic);
		this->blockPath3D(tensorFixed);

		tensorDynamic(1, 1, 2) = char{1};
		tensorFixed(1, 1, 2) = char{1};

		tensorDynamic(1, 0, 2) = char{0};
		tensorFixed(1, 0, 2) = char{0};

		tensorDynamic(2, 2, 1) = char{0};
		tensorFixed(2, 2, 1) = char{0};

		if constexpr (TypeParam::move_diagonally)
		{
			auto path =
				xablau::graph::algorithm::template A_star < TypeParam::move_diagonally, TypeParam::distance_type > (
					tensorDynamic,
					origin,
					destination,
					this->blocked);

			EXPECT_EQ(path.size(), 5);
			EXPECT_TRUE(this->checkPath < TypeParam::move_diagonally > (origin, destination, path));

			path =
				xablau::graph::algorithm::template A_star < TypeParam::move_diagonally, TypeParam::distance_type > (
					tensorFixed,
					origin,
					destination,
					this->blocked);

			EXPECT_EQ(path.size(), 5);
			EXPECT_TRUE(this->checkPath < TypeParam::move_diagonally > (origin, destination, path));
		}

		else
		{
			EXPECT_EQ(
				(xablau::graph::algorithm::template A_star < TypeParam::move_diagonally, TypeParam::distance_type >) (
					tensorDynamic,
					origin,
					destination,
					this->blocked).size(),
				0);

			EXPECT_EQ(
				(xablau::graph::algorithm::template A_star < TypeParam::move_diagonally, TypeParam::distance_type >) (
					tensorFixed,
					origin,
					destination,
					this->blocked).size(),
				0);
		}
	}

	TYPED_TEST_P(GraphAlgorithmAStarTest, AStarMaze2D)
	{
		std::array < size_t, 2 > origin{};
		std::array < size_t, 2 > destination{ 4, 4 };
		constexpr size_t expectedPathSize = TypeParam::move_diagonally ? 13 : 17;

		auto tensorDynamic = this->create2DMazeDynamic();
		auto tensorFixed = this->create2DMazeFixed();

		for (size_t i = 0; i < 4; i++)
		{
			tensorDynamic(1, i) = char{};
			tensorFixed(1, i) = char{};
		}

		for (size_t i = 1; i < 5; i++)
		{
			tensorDynamic(3, i) = char{};
			tensorFixed(3, i) = char{};
		}

		auto path =
			xablau::graph::algorithm::template A_star < TypeParam::move_diagonally, TypeParam::distance_type > (
				tensorDynamic,
				origin,
				destination,
				this->blocked);

		EXPECT_EQ(path.size(), expectedPathSize);
		EXPECT_TRUE(this->checkPath < TypeParam::move_diagonally > (origin, destination, path));

		path =
			xablau::graph::algorithm::template A_star < TypeParam::move_diagonally, TypeParam::distance_type > (
				tensorFixed,
				origin,
				destination,
				this->blocked);

		EXPECT_EQ(path.size(), expectedPathSize);
		EXPECT_TRUE(this->checkPath < TypeParam::move_diagonally > (origin, destination, path));
	}

	TYPED_TEST_P(GraphAlgorithmAStarTest, AStarMaze3D)
	{
		std::array < size_t, 3 > origin{};
		std::array < size_t, 3 > destination{ 0, 2, 0 };
		constexpr size_t expectedPathSize = TypeParam::move_diagonally ? 7 : 11;

		auto tensorDynamic = this->create3DMazeDynamic();
		auto tensorFixed = this->create3DMazeFixed();

		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				tensorDynamic(i, 1, j) = char{};
				tensorFixed(i, 1, j) = char{};
			}
		}

		for (size_t i = 1; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				for (size_t k = 0; k < 2; k++)
				{
					tensorDynamic(i, j, k) = char{};
					tensorFixed(i, j, k) = char{};
				}
			}
		}

		auto path =
			xablau::graph::algorithm::template A_star < TypeParam::move_diagonally, TypeParam::distance_type > (
				tensorDynamic,
				origin,
				destination,
				this->blocked);

		EXPECT_EQ(path.size(), expectedPathSize);
		EXPECT_TRUE(this->checkPath < TypeParam::move_diagonally > (origin, destination, path));

		path =
			xablau::graph::algorithm::template A_star < TypeParam::move_diagonally, TypeParam::distance_type > (
				tensorFixed,
				origin,
				destination,
				this->blocked);

		EXPECT_EQ(path.size(), expectedPathSize);
		EXPECT_TRUE(this->checkPath < TypeParam::move_diagonally > (origin, destination, path));
	}

	REGISTER_TYPED_TEST_SUITE_P(
		GraphAlgorithmAStarTest,
		AStarAllOpened2D,
		AStarAllOpened3D,
		AStarOriginDestinationBlocked,
		AStarPathBlocked2D,
		AStarPathBlocked3D,
		AStarDiagonal2D,
		AStarDiagonal3D,
		AStarMaze2D,
		AStarMaze3D);

	using GraphAlgorithmAStarTestMoveDiagonally = std::tuple < MoveDiagonallyWrapper < false >, MoveDiagonallyWrapper < true > >;
	using GraphAlgorithmAStarTestDistanceTypes = std::tuple < float, double, long double >;

	using GraphAlgorithmTestGroup =
		xablau::testing::test_bundler <
			GraphAlgorithmAStarTestMoveDiagonally,
			GraphAlgorithmAStarTestDistanceTypes > ::template test_group < GeometryAlgorithmAStarTypeDefinitions >;

	using GeometryAlgorithmTypes = xablau::testing::tuple_transformation < ::testing::Types, GraphAlgorithmTestGroup > ::types;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		Xablau,
		GraphAlgorithmAStarTest,
		GeometryAlgorithmTypes);
}
