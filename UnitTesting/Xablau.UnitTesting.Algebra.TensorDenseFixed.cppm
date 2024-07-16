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

import xablau.algebra;
import xablau.testing;

namespace xablau::unit_testing
{
	template <
		typename DataType,
		xablau::algebra::concepts::tensor_fixed_dimensionalities FixedDimensionalities,
		xablau::algebra::concepts::tensor_fixed_dimensionalities HalfSizeFixedDimensionalities,
		xablau::algebra::concepts::tensor_memory_order_indices MemoryOrderIndices,
		xablau::algebra::concepts::tensor_type TensorType,
		typename KernelSizes >
	struct AlgebraTensorDenseFixedTypeDefinitions
	{
		using type = DataType;
		using fixed_dimensionalities = FixedDimensionalities;
		using half_size_fixed_dimensionalities = HalfSizeFixedDimensionalities;
		using memory_order_indices = MemoryOrderIndices;
		using tensor_type = TensorType;
		using kernel_sizes = KernelSizes;
	};

	template < typename DataTypes >
	class AlgebraTensorDenseFixedTest : public ::testing::Test
	{
	public:
		template < size_t Rank >
		constexpr std::array < size_t, Rank > calculateIndex(
			const size_t index,
			const std::array < size_t, Rank > &dimensionalities)
		{
			std::array < size_t, Rank > indices{};

			std::exclusive_scan(
				dimensionalities.crbegin(),
				dimensionalities.crend(),
				indices.rbegin(),
				size_t{1},
				std::multiplies < size_t > {});

			for (size_t i = 0; i < dimensionalities.size(); i++)
			{
				indices[i] = (index / indices[i]) % dimensionalities[i];
			}

			return indices;
		}

		template < size_t Rank >
		constexpr bool validIndex(
			const std::array < size_t, Rank > &index,
			const std::array < size_t, Rank > &dimensionalities)
		{
			for (size_t i = 0; i < Rank; i++)
			{
				if (index[i] >= dimensionalities[i])
				{
					return false;
				}
			}

			return true;
		}

		template < size_t Rank >
		constexpr std::array < size_t, Rank > sumIndices(
			const std::array < size_t, Rank > &index1,
			const std::array < size_t, Rank > &index2)
		{
			std::array < size_t, Rank > result{};

			for (size_t i = 0; i < Rank; i++)
			{
				result[i] = index1[i] + index2[i];
			}

			return result;
		}

		template < size_t Rank >
		constexpr std::array < size_t, Rank > subtractIndices(
			const std::array < size_t, Rank > &index1,
			const std::array < size_t, Rank > &index2)
		{
			std::array < size_t, Rank > result{};

			for (size_t i = 0; i < Rank; i++)
			{
				result[i] = index1[i] - index2[i];
			}

			return result;
		}

		constexpr bool subTensor(
			const auto &subTensor,
			const auto &startingCorner)
		{
			const size_t size = subTensor.size();

			for (size_t i = 0; i < size; i++)
			{
				const auto subTensorIndex = this->calculateIndex(i, subTensor.dimensionalities());
				const auto tensorIndex = this->sumIndices(startingCorner, subTensorIndex);

				if (this->validIndex(tensorIndex, this->tensor.dimensionalities()))
				{
					if (!subTensor.has_reference_to_main(subTensorIndex))
					{
						return false;
					}

					const auto &elementSubTensor = subTensor(subTensorIndex);
					const auto &elementTensor = this->tensor(tensorIndex);

					if (elementTensor != elementSubTensor)
					{
						return false;
					}
				}

				else if (
					subTensor.has_reference_to_main(subTensorIndex) ||
					subTensor(subTensorIndex) != typename DataTypes::type{})
				{
					return false;
				}
			}

			return true;
		}

		constexpr bool checkTensorConversion(
			const auto &fixedTensor,
			const auto &dynamicTensor)
		{
			const size_t size = fixedTensor.size();

			for (size_t i = 0; i < size; i++)
			{
				const auto tensorIndex = this->calculateIndex(i, fixedTensor.dimensionalities());
				const auto &elementFixedTensor = fixedTensor(tensorIndex);
				const auto &elementDynamicTensor = dynamicTensor(tensorIndex);

				if (elementFixedTensor != elementDynamicTensor)
				{
					return false;
				}
			}

			return true;
		}

		xablau::algebra::tensor_dense_fixed <
			typename DataTypes::type,
			typename DataTypes::fixed_dimensionalities,
			typename DataTypes::memory_order_indices,
			typename DataTypes::tensor_type > tensor{};

		xablau::algebra::tensor_dense_fixed <
			typename DataTypes::type,
			typename DataTypes::half_size_fixed_dimensionalities,
			typename DataTypes::memory_order_indices,
			typename DataTypes::tensor_type > halfSizeTensor{};

		xablau::algebra::tensor_dense_fixed <
			typename DataTypes::type,
			typename std::tuple_element < 0, typename DataTypes::kernel_sizes > ::type,
			typename DataTypes::memory_order_indices,
			typename DataTypes::tensor_type > squareTensor1{};

		xablau::algebra::tensor_dense_fixed <
			typename DataTypes::type,
			typename std::tuple_element < 1, typename DataTypes::kernel_sizes > ::type,
			typename DataTypes::memory_order_indices,
			typename DataTypes::tensor_type > squareTensor2{};

		xablau::algebra::tensor_dense_fixed <
			typename DataTypes::type,
			typename std::tuple_element < 2, typename DataTypes::kernel_sizes > ::type,
			typename DataTypes::memory_order_indices,
			typename DataTypes::tensor_type > squareTensor3{};

		xablau::algebra::tensor_dense_fixed <
			typename DataTypes::type,
			typename std::tuple_element < 3, typename DataTypes::kernel_sizes > ::type,
			typename DataTypes::memory_order_indices,
			typename DataTypes::tensor_type > squareTensor4{};
	};

	TYPED_TEST_SUITE_P(AlgebraTensorDenseFixedTest);

	TYPED_TEST_P(AlgebraTensorDenseFixedTest, StorageCapacities)
	{
		constexpr auto rank = TypeParam::fixed_dimensionalities::size();

		std::array < size_t, rank > dimensionalities{};

		std::generate(dimensionalities.begin(), dimensionalities.end(),
			[n = 6] () mutable -> size_t
			{
				return n++;
			});

		EXPECT_EQ(this->tensor.dimensionalities(), dimensionalities);
		EXPECT_EQ(
			this->tensor.size(),
			std::accumulate(dimensionalities.cbegin(), dimensionalities.cend(), size_t{1}, std::multiplies < size_t > ()));
	}

	TYPED_TEST_P(AlgebraTensorDenseFixedTest, ElementAccess)
	{
		constexpr auto rank = TypeParam::fixed_dimensionalities::size();

		if constexpr (rank > 0)
		{
			const size_t size = this->tensor.size();
			const auto &dimensionalities = this->tensor.dimensionalities();

			for (size_t i = 0; i < size; i++)
			{
				if constexpr (std::same_as < TypeParam::type, bool >)
				{
					this->tensor(this->calculateIndex < rank > (i, dimensionalities)) = static_cast < bool > (i % 2);
				}

				else if constexpr (xablau::algebra::concepts::complex < TypeParam::type >)
				{
					this->tensor(this->calculateIndex < rank > (i, dimensionalities)) =
						TypeParam::type(
							static_cast < TypeParam::type::value_type > (i),
							static_cast < TypeParam::type::value_type > (i));
				}

				else
				{
					this->tensor(this->calculateIndex < rank > (i, dimensionalities)) = static_cast < TypeParam::type > (i);
				}
			}

			const auto &constTensor = this->tensor;

			for (size_t i = 0; i < size; i++)
			{
				if constexpr (std::same_as < TypeParam::type, bool >)
				{
					EXPECT_EQ(constTensor(this->calculateIndex < rank > (i, dimensionalities)), static_cast < bool > (i % 2));
				}

				else if constexpr (xablau::algebra::concepts::complex < TypeParam::type >)
				{
					EXPECT_EQ(
						constTensor(this->calculateIndex < rank > (i, dimensionalities)),
						TypeParam::type(
							static_cast < TypeParam::type::value_type > (i),
							static_cast < TypeParam::type::value_type > (i)));
				}

				else
				{
					EXPECT_EQ(constTensor(this->calculateIndex < rank > (i, dimensionalities)), static_cast < TypeParam::type > (i));
				}
			}
		}

		else
		{
			if constexpr (std::same_as < TypeParam::type, bool >)
			{
				*this->tensor = true;
				const auto &constTensor = this->tensor;
				EXPECT_EQ(*constTensor, true);
			}

			else if constexpr (xablau::algebra::concepts::complex < TypeParam::type >)
			{
				*this->tensor = TypeParam::type(42, 42);
				const auto &constTensor = this->tensor;
				EXPECT_EQ(*constTensor, TypeParam::type(42, 42));
			}

			else
			{
				*this->tensor = static_cast < TypeParam::type > (42);
				const auto &constTensor = this->tensor;
				EXPECT_EQ(*constTensor, static_cast < TypeParam::type > (42));
			}
		}
	}

	TYPED_TEST_P(AlgebraTensorDenseFixedTest, Fill)
	{
		constexpr auto rank = TypeParam::fixed_dimensionalities::size();

		if constexpr (rank > 0)
		{
			if constexpr (std::same_as < TypeParam::type, bool >)
			{
				this->tensor.fill(true);
			}

			else if constexpr (xablau::algebra::concepts::complex < TypeParam::type >)
			{
				this->tensor.fill(
					TypeParam::type(
						static_cast < TypeParam::type::value_type > (42),
						static_cast < TypeParam::type::value_type > (42)));
			}

			else
			{
				this->tensor.fill(static_cast < TypeParam::type > (42));
			}

			const size_t size = this->tensor.size();
			const auto &dimensionalities = this->tensor.dimensionalities();
			const auto &constTensor = this->tensor;

			for (size_t i = 0; i < size; i++)
			{
				if constexpr (std::same_as < TypeParam::type, bool >)
				{
					EXPECT_EQ(constTensor(this->calculateIndex < rank > (i, dimensionalities)), true);
				}

				else if constexpr (xablau::algebra::concepts::complex < TypeParam::type >)
				{
					EXPECT_EQ(
						constTensor(this->calculateIndex < rank > (i, dimensionalities)),
						TypeParam::type(
							static_cast < TypeParam::type::value_type > (42),
							static_cast < TypeParam::type::value_type > (42)));
				}

				else
				{
					EXPECT_EQ(constTensor(this->calculateIndex < rank > (i, dimensionalities)), static_cast < TypeParam::type > (42));
				}
			}
		}

		else
		{
			if constexpr (std::same_as < TypeParam::type, bool >)
			{
				this->tensor.fill(true);
				const auto &constTensor = this->tensor;
				EXPECT_EQ(*constTensor, true);
			}

			else if constexpr (xablau::algebra::concepts::complex < TypeParam::type >)
			{
				this->tensor.fill(TypeParam::type(42, 42));
				const auto &constTensor = this->tensor;
				EXPECT_EQ(*constTensor, TypeParam::type(42, 42));
			}

			else
			{
				this->tensor.fill(static_cast < TypeParam::type > (42));
				const auto &constTensor = this->tensor;
				EXPECT_EQ(*constTensor, static_cast < TypeParam::type > (42));
			}
		}
	}

	TYPED_TEST_P(AlgebraTensorDenseFixedTest, MatricesMultiplication)
	{
		constexpr auto rank = TypeParam::fixed_dimensionalities::size();

		if constexpr (
			rank == 2 && (
				std::floating_point < typename TypeParam::type > ||
				xablau::algebra::concepts::complex < typename TypeParam::type >))
		{
			const size_t size = this->halfSizeTensor.size();
			const auto &dimensionalities = this->halfSizeTensor.dimensionalities();

			for (size_t i = 0; i < size; i++)
			{
				if constexpr (xablau::algebra::concepts::complex < TypeParam::type >)
				{
					this->halfSizeTensor(this->calculateIndex < rank > (i, dimensionalities)) =
						TypeParam::type(
							static_cast < TypeParam::type::value_type > (i),
							static_cast < TypeParam::type::value_type > (i));
				}

				else
				{
					this->halfSizeTensor(this->calculateIndex < rank > (i, dimensionalities)) = static_cast < TypeParam::type > (i);
				}
			}

			const auto resultTensor = this->halfSizeTensor * this->halfSizeTensor.transpose();

			EXPECT_EQ(resultTensor.dimensionalities()[0], 3);
			EXPECT_EQ(resultTensor.dimensionalities()[1], 3);

			if constexpr (std::floating_point < typename TypeParam::type >)
			{
				EXPECT_EQ(resultTensor(0, 0), typename TypeParam::type{14});
				EXPECT_EQ(resultTensor(0, 1), typename TypeParam::type{38});
				EXPECT_EQ(resultTensor(0, 2), typename TypeParam::type{62});

				EXPECT_EQ(resultTensor(1, 0), typename TypeParam::type{38});
				EXPECT_EQ(resultTensor(1, 1), typename TypeParam::type{126});
				EXPECT_EQ(resultTensor(1, 2), typename TypeParam::type{214});

				EXPECT_EQ(resultTensor(2, 0), typename TypeParam::type{62});
				EXPECT_EQ(resultTensor(2, 1), typename TypeParam::type{214});
				EXPECT_EQ(resultTensor(2, 2), typename TypeParam::type{366});
			}

			else
			{
				EXPECT_EQ(resultTensor(0, 0), typename TypeParam::type{2} * typename TypeParam::type(0, 14));
				EXPECT_EQ(resultTensor(0, 1), typename TypeParam::type{2} * typename TypeParam::type(0, 38));
				EXPECT_EQ(resultTensor(0, 2), typename TypeParam::type{2} * typename TypeParam::type(0, 62));

				EXPECT_EQ(resultTensor(1, 0), typename TypeParam::type{2} * typename TypeParam::type(0, 38));
				EXPECT_EQ(resultTensor(1, 1), typename TypeParam::type{2} * typename TypeParam::type(0, 126));
				EXPECT_EQ(resultTensor(1, 2), typename TypeParam::type{2} * typename TypeParam::type(0, 214));

				EXPECT_EQ(resultTensor(2, 0), typename TypeParam::type{2} * typename TypeParam::type(0, 62));
				EXPECT_EQ(resultTensor(2, 1), typename TypeParam::type{2} * typename TypeParam::type(0, 214));
				EXPECT_EQ(resultTensor(2, 2), typename TypeParam::type{2} * typename TypeParam::type(0, 366));
			}
		}
	}

	TYPED_TEST_P(AlgebraTensorDenseFixedTest, Transposition)
	{
		constexpr auto rank = TypeParam::fixed_dimensionalities::size();

		if constexpr (rank == 2)
		{
			const size_t size = this->tensor.size();
			const auto &dimensionalities = this->tensor.dimensionalities();

			for (size_t i = 0; i < size; i++)
			{
				if constexpr (std::same_as < TypeParam::type, bool >)
				{
					this->tensor(this->calculateIndex < rank > (i, dimensionalities)) = static_cast < bool > (i % 2);
				}

				else if constexpr (xablau::algebra::concepts::complex < TypeParam::type >)
				{
					this->tensor(this->calculateIndex < rank > (i, dimensionalities)) =
						TypeParam::type(
							static_cast < TypeParam::type::value_type > (i),
							static_cast < TypeParam::type::value_type > (i));
				}

				else
				{
					this->tensor(this->calculateIndex < rank > (i, dimensionalities)) = static_cast < TypeParam::type > (i);
				}
			}

			const auto transposedTensor = this->tensor.transpose();

			for (size_t i = 0; i < dimensionalities[0]; i++)
			{
				for (size_t j = 0; j < dimensionalities[1]; j++)
				{
					EXPECT_EQ(this->tensor(i, j), transposedTensor(j, i));
				}
			}
		}
	}

	TYPED_TEST_P(AlgebraTensorDenseFixedTest, ConjugateTransposition)
	{
		constexpr auto rank = TypeParam::fixed_dimensionalities::size();

		if constexpr (rank == 2)
		{
			const size_t size = this->tensor.size();
			const auto &dimensionalities = this->tensor.dimensionalities();

			for (size_t i = 0; i < size; i++)
			{
				if constexpr (std::same_as < TypeParam::type, bool >)
				{
					this->tensor(this->calculateIndex < rank > (i, dimensionalities)) = static_cast < bool > (i % 2);
				}

				else if constexpr (xablau::algebra::concepts::complex < TypeParam::type >)
				{
					this->tensor(this->calculateIndex < rank > (i, dimensionalities)) =
						TypeParam::type(
							static_cast < TypeParam::type::value_type > (i),
							static_cast < TypeParam::type::value_type > (i));
				}

				else
				{
					this->tensor(this->calculateIndex < rank > (i, dimensionalities)) = static_cast < TypeParam::type > (i);
				}
			}

			const auto transposedTensor = this->tensor.conjugate_transpose();

			for (size_t i = 0; i < dimensionalities[0]; i++)
			{
				for (size_t j = 0; j < dimensionalities[1]; j++)
				{
					if constexpr (xablau::algebra::concepts::complex < TypeParam::type >)
					{
						EXPECT_EQ(std::conj(this->tensor(i, j)), transposedTensor(j, i));
					}

					else
					{
						EXPECT_EQ(this->tensor(i, j), transposedTensor(j, i));
					}
				}
			}
		}
	}

	TYPED_TEST_P(AlgebraTensorDenseFixedTest, Kernel)
	{
		constexpr auto rank = TypeParam::fixed_dimensionalities::size();

		if constexpr (rank > 0)
		{
			const size_t size = this->tensor.size();
			const auto &dimensionalities = this->tensor.dimensionalities();

			for (size_t i = 0; i < size; i++)
			{
				if constexpr (std::same_as < TypeParam::type, bool >)
				{
					this->tensor(this->calculateIndex < rank > (i, dimensionalities)) = static_cast < bool > (i % 2);
				}

				else if constexpr (xablau::algebra::concepts::complex < TypeParam::type >)
				{
					this->tensor(this->calculateIndex < rank > (i, dimensionalities)) =
						TypeParam::type(
							static_cast < TypeParam::type::value_type > (i),
							static_cast < TypeParam::type::value_type > (i));
				}

				else
				{
					this->tensor(this->calculateIndex < rank > (i, dimensionalities)) = static_cast < TypeParam::type > (i);
				}
			}

			std::array < size_t, rank > offset{};

			using KernelSize1 = typename std::tuple_element < 0, TypeParam::kernel_sizes > ::type;

			offset.fill(0);

			for (size_t i = 0; i < size; i++)
			{
				const auto center = this->calculateIndex < rank > (i, dimensionalities);
				const auto mutableKernel = this->tensor.kernel < KernelSize1 > (center);
				const auto constKernel = std::as_const(this->tensor).kernel < KernelSize1 > (center);

				// Tries to attribute negative numbers on (size_t)s. Ugly, but not UB.
				const auto startingCorner = this->subtractIndices(center, offset);

				EXPECT_EQ(mutableKernel.dimensionalities(), KernelSize1::array);
				EXPECT_EQ(this->subTensor(mutableKernel, startingCorner), true);

				EXPECT_EQ(constKernel.dimensionalities(), KernelSize1::array);
				EXPECT_EQ(this->subTensor(constKernel, startingCorner), true);
			}

			using KernelSize2 = typename std::tuple_element < 1, TypeParam::kernel_sizes > ::type;

			offset.fill(1);

			for (size_t i = 0; i < size; i++)
			{
				const auto center = this->calculateIndex < rank > (i, dimensionalities);
				const auto mutableKernel = this->tensor.kernel < KernelSize2 > (center);
				const auto constKernel = std::as_const(this->tensor).kernel < KernelSize2 > (center);

				// Tries to attribute negative numbers on (size_t)s. Ugly, but not UB.
				const auto startingCorner = this->subtractIndices(center, offset);

				EXPECT_EQ(mutableKernel.dimensionalities(), KernelSize2::array);
				EXPECT_EQ(this->subTensor(mutableKernel, startingCorner), true);

				EXPECT_EQ(constKernel.dimensionalities(), KernelSize2::array);
				EXPECT_EQ(this->subTensor(constKernel, startingCorner), true);
			}

			using KernelSize3 = typename std::tuple_element < 3, TypeParam::kernel_sizes > ::type;

			offset.fill(2);

			for (size_t i = 0; i < size; i++)
			{
				const auto center = this->calculateIndex < rank > (i, dimensionalities);
				const auto mutableKernel = this->tensor.kernel < KernelSize3 > (center);
				const auto constKernel = std::as_const(this->tensor).kernel < KernelSize3 > (center);

				// Tries to attribute negative numbers on (size_t)s. Ugly, but not UB.
				const auto startingCorner = this->subtractIndices(center, offset);

				EXPECT_EQ(mutableKernel.dimensionalities(), KernelSize3::array);
				EXPECT_EQ(this->subTensor(mutableKernel, startingCorner), true);

				EXPECT_EQ(constKernel.dimensionalities(), KernelSize3::array);
				EXPECT_EQ(this->subTensor(constKernel, startingCorner), true);
			}
		}
	}

	TYPED_TEST_P(AlgebraTensorDenseFixedTest, MatrixRow)
	{
		constexpr auto rank = TypeParam::fixed_dimensionalities::size();

		if constexpr (rank == 2)
		{
			const size_t size = this->tensor.size();
			const auto &dimensionalities = this->tensor.dimensionalities();

			for (size_t i = 0; i < size; i++)
			{
				if constexpr (std::same_as < TypeParam::type, bool >)
				{
					this->tensor(this->calculateIndex < rank > (i, dimensionalities)) = static_cast < bool > (i % 2);
				}

				else if constexpr (xablau::algebra::concepts::complex < TypeParam::type >)
				{
					this->tensor(this->calculateIndex < rank > (i, dimensionalities)) =
						TypeParam::type(
							static_cast < TypeParam::type::value_type > (i),
							static_cast < TypeParam::type::value_type > (i));
				}

				else
				{
					this->tensor(this->calculateIndex < rank > (i, dimensionalities)) = static_cast < TypeParam::type > (i);
				}
			}

			for (size_t i = 0; i < dimensionalities[0]; i++)
			{
				const auto mutableRow = this->tensor.row(i);
				const auto constRow = std::as_const(this->tensor).row(i);

				for (size_t j = 0; j < dimensionalities[1]; j++)
				{
					EXPECT_EQ(this->tensor(i, j), mutableRow(0, j));
					EXPECT_EQ(this->tensor(i, j), constRow(0, j));
				}
			}
		}
	}

	TYPED_TEST_P(AlgebraTensorDenseFixedTest, MatrixColumn)
	{
		constexpr auto rank = TypeParam::fixed_dimensionalities::size();

		if constexpr (rank == 2)
		{
			const size_t size = this->tensor.size();
			const auto &dimensionalities = this->tensor.dimensionalities();

			for (size_t i = 0; i < size; i++)
			{
				if constexpr (std::same_as < TypeParam::type, bool >)
				{
					this->tensor(this->calculateIndex < rank > (i, dimensionalities)) = static_cast < bool > (i % 2);
				}

				else if constexpr (xablau::algebra::concepts::complex < TypeParam::type >)
				{
					this->tensor(this->calculateIndex < rank > (i, dimensionalities)) =
						TypeParam::type(
							static_cast < TypeParam::type::value_type > (i),
							static_cast < TypeParam::type::value_type > (i));
				}

				else
				{
					this->tensor(this->calculateIndex < rank > (i, dimensionalities)) = static_cast < TypeParam::type > (i);
				}
			}

			for (size_t i = 0; i < dimensionalities[1]; i++)
			{
				const auto mutableColumn = this->tensor.column(i);
				const auto constColumn = std::as_const(this->tensor).column(i);

				for (size_t j = 0; j < dimensionalities[0]; j++)
				{
					EXPECT_EQ(this->tensor(j, i), mutableColumn(j, 0));
					EXPECT_EQ(this->tensor(j, i), constColumn(j, 0));
				}
			}
		}
	}

	TYPED_TEST_P(AlgebraTensorDenseFixedTest, TensorDenseDynamicConversion)
	{
		constexpr auto rank = TypeParam::fixed_dimensionalities::size();

		if constexpr (rank > 0)
		{
			const size_t size = this->tensor.size();
			const auto &dimensionalities = this->tensor.dimensionalities();

			for (size_t i = 0; i < size; i++)
			{
				if constexpr (std::same_as < TypeParam::type, bool >)
				{
					this->tensor(this->calculateIndex < rank > (i, dimensionalities)) = static_cast < bool > (i % 2);
				}

				else if constexpr (xablau::algebra::concepts::complex < TypeParam::type >)
				{
					this->tensor(this->calculateIndex < rank > (i, dimensionalities)) =
						TypeParam::type(
							static_cast < TypeParam::type::value_type > (i),
							static_cast < TypeParam::type::value_type > (i));
				}

				else
				{
					this->tensor(this->calculateIndex < rank > (i, dimensionalities)) = static_cast < TypeParam::type > (i);
				}
			}
		}

		else
		{
			if constexpr (std::same_as < TypeParam::type, bool >)
			{
				*this->tensor = true;
			}

			else if constexpr (xablau::algebra::concepts::complex < TypeParam::type >)
			{
				*this->tensor = TypeParam::type(42, 42);
			}

			else
			{
				*this->tensor = static_cast < TypeParam::type > (42);
			}
		}

		const auto dynamicTensor = this->tensor.convert_to_tensor_dense_dynamic();

		EXPECT_EQ(this->checkTensorConversion(this->tensor, dynamicTensor), true);
		EXPECT_EQ(this->tensor.dimensionalities(), dynamicTensor.dimensionalities());
		EXPECT_EQ(this->tensor.size(), dynamicTensor.size());
	}

	TYPED_TEST_P(AlgebraTensorDenseFixedTest, DeterminantReal)
	{
		constexpr auto rank = TypeParam::fixed_dimensionalities::size();

		if constexpr (rank == 2 && std::floating_point < TypeParam::type >)
		{
			constexpr auto primeNumbers =
				std::to_array({
					2, 3, 5, 7, 11,
					13, 17, 19, 23, 29,
					31, 37, 41, 43, 47,
					53, 59, 61, 67, 71,
					73, 79, 83, 89, 97 });

			constexpr auto results =
				std::to_array({
					TypeParam::type(2),
					TypeParam::type(-78),
					TypeParam::type(-4656) });

			const size_t size1 = this->squareTensor1.size();

			for (size_t i = 0; i < size1; i++)
			{
				this->squareTensor1(this->calculateIndex < rank > (i, this->squareTensor1.dimensionalities())) =
					TypeParam::type(primeNumbers[i]);
			}

			const size_t size2 = this->squareTensor2.size();

			for (size_t i = 0; i < size2; i++)
			{
				this->squareTensor2(this->calculateIndex < rank > (i, this->squareTensor2.dimensionalities())) =
					TypeParam::type(primeNumbers[i]);
			}

			const size_t size3 = this->squareTensor4.size();

			for (size_t i = 0; i < size3; i++)
			{
				this->squareTensor4(this->calculateIndex < rank > (i, this->squareTensor4.dimensionalities())) =
					TypeParam::type(primeNumbers[i]);
			}

			const bool almostEqual1 = xablau::algebra::functions::almost_equal < TypeParam::type > ::invoke < 10 > (results[0], this->squareTensor1.determinant());
			const bool almostEqual2 = xablau::algebra::functions::almost_equal < TypeParam::type > ::invoke < 10 > (results[1], this->squareTensor2.determinant());
			const bool almostEqual3 = xablau::algebra::functions::almost_equal < TypeParam::type > ::invoke < 10 > (results[2], this->squareTensor4.determinant());

			EXPECT_EQ(almostEqual1 && almostEqual2 && almostEqual3, true);
		}
	}

	TYPED_TEST_P(AlgebraTensorDenseFixedTest, DeterminantComplex)
	{
		constexpr auto rank = TypeParam::fixed_dimensionalities::size();

		if constexpr (rank == 2 && xablau::algebra::concepts::complex < TypeParam::type >)
		{
			constexpr auto primeNumbers =
				std::to_array({
					2, 3, 5, 7, 11,
					13, 17, 19, 23, 29,
					31, 37, 41, 43, 47,
					53, 59, 61, 67, 71,
					73, 79, 83, 89, 97,
					101, 103, 107, 109, 113,
					127, 131, 137, 139, 149,
					151, 157, 163, 167, 173,
					179, 181, 191, 193, 197,
					199, 211, 223, 227, 229 });

			constexpr auto results =
				std::to_array({
					TypeParam::type(2, 3),
					TypeParam::type(1372, -924),
					TypeParam::type(-578168, -203176) });

			const size_t size1 = this->squareTensor1.size();

			for (size_t i = 0; i < size1; i++)
			{
				this->squareTensor1(this->calculateIndex < rank > (i, this->squareTensor1.dimensionalities())) =
					TypeParam::type(
						static_cast < TypeParam::type::value_type > (primeNumbers[2 * i]),
						static_cast < TypeParam::type::value_type > (primeNumbers[2 * i + 1]));
			}

			const size_t size2 = this->squareTensor2.size();

			for (size_t i = 0; i < size2; i++)
			{
				this->squareTensor2(this->calculateIndex < rank > (i, this->squareTensor2.dimensionalities())) =
					TypeParam::type(
						static_cast < TypeParam::type::value_type > (primeNumbers[2 * i]),
						static_cast < TypeParam::type::value_type > (primeNumbers[2 * i + 1]));
			}

			const size_t size3 = this->squareTensor4.size();

			for (size_t i = 0; i < size3; i++)
			{
				this->squareTensor4(this->calculateIndex < rank > (i, this->squareTensor4.dimensionalities())) =
					TypeParam::type(
						static_cast < TypeParam::type::value_type > (primeNumbers[2 * i]),
						static_cast < TypeParam::type::value_type > (primeNumbers[2 * i + 1]));
			}

			const bool almostEqual1 = xablau::algebra::functions::almost_equal < TypeParam::type > ::invoke < 38 > (results[0], this->squareTensor1.determinant());
			const bool almostEqual2 = xablau::algebra::functions::almost_equal < TypeParam::type > ::invoke < 38 > (results[1], this->squareTensor2.determinant());
			const bool almostEqual3 = xablau::algebra::functions::almost_equal < TypeParam::type > ::invoke < 38 > (results[2], this->squareTensor4.determinant());

			EXPECT_EQ(almostEqual1 && almostEqual2 && almostEqual3, true);
		}
	}

	TYPED_TEST_P(AlgebraTensorDenseFixedTest, InverseReal)
	{
		constexpr auto rank = TypeParam::fixed_dimensionalities::size();

		if constexpr (rank == 2 && std::floating_point < TypeParam::type >)
		{
			constexpr auto primeNumbers =
				std::to_array({
					2, 3, 5,
					7, 11, 13,
					17, 19, 23 });

			constexpr auto result =
				std::to_array({
					-6, -26, 16,
					-60, 39, -9,
					54, -13, -1 });

			std::array < size_t, rank > dimensionalities{ 3, 3 };

			decltype(this->squareTensor2) resultTensor{};

			const size_t size = this->squareTensor2.size();

			for (size_t i = 0; i < size; i++)
			{
				this->squareTensor2(this->calculateIndex < rank > (i, dimensionalities)) = TypeParam::type(primeNumbers[i]);

				resultTensor(this->calculateIndex < rank > (i, dimensionalities)) =
					(typename TypeParam::type{1} / typename TypeParam::type{78}) * static_cast < typename TypeParam::type > (result[i]);
			}

			const auto inverseMatrix = this->squareTensor2.inverse();

			for (size_t i = 0; i < size; i++)
			{
				EXPECT_TRUE(
					xablau::algebra::functions::almost_equal < typename TypeParam::type > ::invoke < 10 > (
						inverseMatrix(this->calculateIndex < rank > (i, dimensionalities)),
						resultTensor(this->calculateIndex < rank > (i, dimensionalities))));
			}
		}
	}

	TYPED_TEST_P(AlgebraTensorDenseFixedTest, InverseComplex)
	{
		constexpr auto rank = TypeParam::fixed_dimensionalities::size();

		if constexpr (rank == 2 && xablau::algebra::concepts::complex < TypeParam::type >)
		{
			constexpr auto primeNumbers =
				std::to_array({
					2, 3, 5, 7, 11, 13,
					17, 19, 23, 29, 31, 37,
					41, 43, 47, 53, 59, 61 });

			constexpr auto result =
				std::to_array({
					-88, -180, 218, 258, -98, -118,
					264, 428, -332, -375, 126, 137,
					-144, -240, 112, 143, -40, -47 });

			std::array < size_t, rank > dimensionalities{ 3, 3 };

			decltype(this->squareTensor2) resultTensor{};

			const size_t size = this->squareTensor2.size();

			for (size_t i = 0; i < size; i++)
			{
				this->squareTensor2(this->calculateIndex < rank > (i, dimensionalities)) =
					TypeParam::type(
						static_cast < typename TypeParam::type::value_type > (primeNumbers[2 * i]),
						static_cast < typename TypeParam::type::value_type > (primeNumbers[2 * i + 1]));

				resultTensor(this->calculateIndex < rank > (i, dimensionalities)) =
					TypeParam::type(
						typename TypeParam::type::value_type{2} / typename TypeParam::type::value_type{12215},
						typename TypeParam::type::value_type{41} / typename TypeParam::type::value_type{48860}) *
							TypeParam::type(
								static_cast < typename TypeParam::type::value_type > (result[2 * i]),
								static_cast < typename TypeParam::type::value_type > (result[2 * i + 1]));
			}

			const auto inverseMatrix = this->squareTensor2.inverse();

			for (size_t i = 0; i < size; i++)
			{
				EXPECT_TRUE(
					xablau::algebra::functions::almost_equal < typename TypeParam::type > ::invoke < 10 > (
						inverseMatrix(this->calculateIndex < rank > (i, dimensionalities)),
						resultTensor(this->calculateIndex < rank > (i, dimensionalities))));
			}
		}
	}

	TYPED_TEST_P(AlgebraTensorDenseFixedTest, EigenHermitianReal)
	{
		constexpr auto rank = TypeParam::fixed_dimensionalities::size();

		if constexpr (rank == 2 && std::floating_point < TypeParam::type >)
		{
			constexpr auto input =
				std::to_array({
					52, 30, 49, 28,
					30, 50, 8, 44,
					49, 8, 46, 16,
					28, 44, 16, 22 });

			std::array < size_t, rank > dimensionalities{ 4, 4 };

			const size_t size = this->squareTensor3.size();

			for (size_t i = 0; i < size; i++)
			{
				this->squareTensor3(this->calculateIndex < rank > (i, dimensionalities)) =
					static_cast < typename TypeParam::type > (input[i]);
			}

			const auto [eigenvalues, eigenvectors] = this->squareTensor3.eigen_Hermitian < true, true > ();

			xablau::algebra::tensor_dense_fixed <
				typename TypeParam::type,
				xablau::algebra::tensor_fixed_dimensionalities < 4, 4 >,
				typename TypeParam::memory_order_indices,
				typename TypeParam::tensor_type > diagonalEigenvalues{};

			for (size_t i = 0; i < 4; i++)
			{
				diagonalEigenvalues(i, i) = eigenvalues(i, 0);
			}

			const auto resultMatrix = eigenvectors * diagonalEigenvalues * eigenvectors.inverse();

			for (size_t i = 0; i < size; i++)
			{
				EXPECT_LT(
					std::abs(
						resultMatrix(this->calculateIndex < rank > (i, dimensionalities)) -
							this->squareTensor3(this->calculateIndex < rank > (i, dimensionalities))),
					typename TypeParam::type(0.001));
			}
		}
	}

	TYPED_TEST_P(AlgebraTensorDenseFixedTest, EigenHermitianComplex)
	{
		constexpr auto rank = TypeParam::fixed_dimensionalities::size();

		if constexpr (rank == 2 && xablau::algebra::concepts::complex < TypeParam::type >)
		{
			constexpr auto inputReal =
				std::to_array({
					52, 30, 49, 28,
					30, 50, 8, 44,
					49, 8, 46, 16,
					28, 44, 16, 22 });

			constexpr auto inputComplex =
				std::to_array({
					0, -2, -3, -4,
					2, 0, -6, -7,
					3, 6, 0, -9,
					4, 7, 9, 0 });

			std::array < size_t, rank > dimensionalities{ 4, 4 };

			const size_t size = this->squareTensor3.size();

			for (size_t i = 0; i < size; i++)
			{
				this->squareTensor3(this->calculateIndex < rank > (i, dimensionalities)) =
					typename TypeParam::type(
						static_cast < typename TypeParam::type::value_type > (inputReal[i]),
						static_cast < typename TypeParam::type::value_type > (inputComplex[i]));
			}

			const auto [eigenvalues, eigenvectors] = this->squareTensor3.eigen_Hermitian < true, true > ();

			xablau::algebra::tensor_dense_fixed <
				typename TypeParam::type,
				xablau::algebra::tensor_fixed_dimensionalities < 4, 4 >,
				typename TypeParam::memory_order_indices,
				typename TypeParam::tensor_type > diagonalEigenvalues{};

			for (size_t i = 0; i < 4; i++)
			{
				diagonalEigenvalues(i, i) = eigenvalues(i, 0);
			}

			const auto resultMatrix = eigenvectors * diagonalEigenvalues * eigenvectors.inverse();

			for (size_t i = 0; i < size; i++)
			{
				EXPECT_LT(
					std::abs(
						resultMatrix(this->calculateIndex < rank > (i, dimensionalities)) -
							this->squareTensor3(this->calculateIndex < rank > (i, dimensionalities))),
					typename TypeParam::type::value_type(0.001));
			}
		}
	}

	TYPED_TEST_P(AlgebraTensorDenseFixedTest, Concepts)
	{
		using TensorType = decltype(this->tensor);

		constexpr auto rank = TypeParam::fixed_dimensionalities::size();

		EXPECT_TRUE(xablau::algebra::concepts::xablau_tensor < TensorType >);
		EXPECT_TRUE(xablau::algebra::concepts::xablau_tensor_dense < TensorType >);
		EXPECT_FALSE(xablau::algebra::concepts::xablau_tensor_sparse < TensorType >);
		EXPECT_FALSE(xablau::algebra::concepts::xablau_tensor_dynamic < TensorType >);
		EXPECT_TRUE(xablau::algebra::concepts::xablau_tensor_fixed < TensorType >);
		EXPECT_TRUE(xablau::algebra::concepts::xablau_tensor_main < TensorType >);
		EXPECT_FALSE(xablau::algebra::concepts::xablau_tensor_sub < TensorType >);

		if constexpr (rank == 2)
		{
			EXPECT_TRUE(xablau::algebra::concepts::xablau_matrix < TensorType >);
			EXPECT_TRUE(xablau::algebra::concepts::xablau_matrix_dense < TensorType >);
			EXPECT_FALSE(xablau::algebra::concepts::xablau_matrix_sparse < TensorType >);
			EXPECT_FALSE(xablau::algebra::concepts::xablau_matrix_dynamic < TensorType >);
			EXPECT_TRUE(xablau::algebra::concepts::xablau_matrix_fixed < TensorType >);
			EXPECT_TRUE(xablau::algebra::concepts::xablau_matrix_main < TensorType >);
			EXPECT_FALSE(xablau::algebra::concepts::xablau_matrix_sub < TensorType >);
		}
	}

	REGISTER_TYPED_TEST_SUITE_P(
		AlgebraTensorDenseFixedTest,
		StorageCapacities,
		ElementAccess,
		Fill,
		MatricesMultiplication,
		Transposition,
		ConjugateTransposition,
		Kernel,
		MatrixRow,
		MatrixColumn,
		TensorDenseDynamicConversion,
		DeterminantReal,
		DeterminantComplex,
		InverseReal,
		InverseComplex,
		EigenHermitianReal,
		EigenHermitianComplex,
		Concepts);

	using AlgebraTensorDenseFixedTestDataTypes =
		std::tuple <
			bool,
			char,
			unsigned char,
			long long int,
			unsigned long long int,
			float,
			long double,
			std::complex < float >,
			std::complex < long double > >;

	using AlgebraTensorFixedDimensionalities0 = std::tuple < xablau::algebra::tensor_fixed_dimensionalities <> >;
	using AlgebraTensorFixedDimensionalities1 = std::tuple < xablau::algebra::tensor_fixed_dimensionalities < 6 > >;
	using AlgebraTensorFixedDimensionalities2 = std::tuple < xablau::algebra::tensor_fixed_dimensionalities < 6, 7 > >;
	using AlgebraTensorFixedDimensionalities3 = std::tuple < xablau::algebra::tensor_fixed_dimensionalities < 6, 7, 8 > >;

	using AlgebraTensorHalfSizeFixedDimensionalities0 = std::tuple < xablau::algebra::tensor_fixed_dimensionalities <> >;
	using AlgebraTensorHalfSizeFixedDimensionalities1 = std::tuple < xablau::algebra::tensor_fixed_dimensionalities < 3 > >;
	using AlgebraTensorHalfSizeFixedDimensionalities2 = std::tuple < xablau::algebra::tensor_fixed_dimensionalities < 3, 4 > >;
	using AlgebraTensorHalfSizeFixedDimensionalities3 = std::tuple < xablau::algebra::tensor_fixed_dimensionalities < 3, 4, 5 > >;

	using AlgebraTensorDenseDynamicTestMemoryOrderIndices0 =
		std::tuple < xablau::algebra::tensor_memory_order_indices < > >;

	using AlgebraTensorDenseDynamicTestMemoryOrderIndices1 =
		std::tuple < xablau::algebra::tensor_memory_order_indices < 0 > >;

	using AlgebraTensorDenseDynamicTestMemoryOrderIndices2 =
		std::tuple <
			xablau::algebra::tensor_memory_order_indices < 0, 1 >,
			xablau::algebra::tensor_memory_order_indices < 1, 0 > >;

	using AlgebraTensorDenseDynamicTestMemoryOrderIndices3 =
		std::tuple <
			xablau::algebra::tensor_memory_order_indices < 0, 1, 2 >,
			xablau::algebra::tensor_memory_order_indices < 0, 2, 1 >,

			xablau::algebra::tensor_memory_order_indices < 1, 0, 2 >,
			xablau::algebra::tensor_memory_order_indices < 1, 2, 0 >,

			xablau::algebra::tensor_memory_order_indices < 2, 0, 1 >,
			xablau::algebra::tensor_memory_order_indices < 2, 1, 0 > >;

	using AlgebraTensorDenseDynamicTestType =
		std::tuple < xablau::algebra::tensor_type < xablau::algebra::tensor_type_value::main > >;

	using AlgebraTensorKernelSizes0 =
		std::tuple < std::tuple <
			xablau::algebra::tensor_fixed_dimensionalities <>,
			xablau::algebra::tensor_fixed_dimensionalities <>,
			xablau::algebra::tensor_fixed_dimensionalities <>,
			xablau::algebra::tensor_fixed_dimensionalities <> > >;

	using AlgebraTensorKernelSizes1 =
		std::tuple < std::tuple <
			xablau::algebra::tensor_fixed_dimensionalities < 1 >,
			xablau::algebra::tensor_fixed_dimensionalities < 3 >,
			xablau::algebra::tensor_fixed_dimensionalities < 4 >,
			xablau::algebra::tensor_fixed_dimensionalities < 5 > > >;

	using AlgebraTensorKernelSizes2 =
		std::tuple < std::tuple <
			xablau::algebra::tensor_fixed_dimensionalities < 1, 1 >,
			xablau::algebra::tensor_fixed_dimensionalities < 3, 3 >,
			xablau::algebra::tensor_fixed_dimensionalities < 4, 4 >,
			xablau::algebra::tensor_fixed_dimensionalities < 5, 5 > > >;

	using AlgebraTensorKernelSizes3 =
		std::tuple < std::tuple <
			xablau::algebra::tensor_fixed_dimensionalities < 1, 1, 1 >,
			xablau::algebra::tensor_fixed_dimensionalities < 3, 3, 3 >,
			xablau::algebra::tensor_fixed_dimensionalities < 4, 4, 4 >,
			xablau::algebra::tensor_fixed_dimensionalities < 5, 5, 5 > > >;

	using AlgebraTensorDenseDynamicTestRank0Group =
		xablau::testing::test_bundler <
			AlgebraTensorDenseFixedTestDataTypes,
			AlgebraTensorFixedDimensionalities0,
			AlgebraTensorHalfSizeFixedDimensionalities0,
			AlgebraTensorDenseDynamicTestMemoryOrderIndices0,
			AlgebraTensorDenseDynamicTestType,
			AlgebraTensorKernelSizes0 > ::template test_group < AlgebraTensorDenseFixedTypeDefinitions >;

	using AlgebraTensorDenseDynamicTestRank1Group =
		xablau::testing::test_bundler <
			AlgebraTensorDenseFixedTestDataTypes,
			AlgebraTensorFixedDimensionalities1,
			AlgebraTensorHalfSizeFixedDimensionalities1,
			AlgebraTensorDenseDynamicTestMemoryOrderIndices1,
			AlgebraTensorDenseDynamicTestType,
			AlgebraTensorKernelSizes1 > ::template test_group < AlgebraTensorDenseFixedTypeDefinitions >;

	using AlgebraTensorDenseDynamicTestRank2Group =
		xablau::testing::test_bundler <
			AlgebraTensorDenseFixedTestDataTypes,
			AlgebraTensorFixedDimensionalities2,
			AlgebraTensorHalfSizeFixedDimensionalities2,
			AlgebraTensorDenseDynamicTestMemoryOrderIndices2,
			AlgebraTensorDenseDynamicTestType,
			AlgebraTensorKernelSizes2 > ::template test_group < AlgebraTensorDenseFixedTypeDefinitions >;

	using AlgebraTensorDenseDynamicTestRank3Group =
		xablau::testing::test_bundler <
			AlgebraTensorDenseFixedTestDataTypes,
			AlgebraTensorFixedDimensionalities3,
			AlgebraTensorHalfSizeFixedDimensionalities3,
			AlgebraTensorDenseDynamicTestMemoryOrderIndices3,
			AlgebraTensorDenseDynamicTestType,
			AlgebraTensorKernelSizes3 > ::template test_group < AlgebraTensorDenseFixedTypeDefinitions >;

	using AlgebraTensorDenseDynamicTestEveryGroup =
		xablau::testing::tuple_concatenator < xablau::testing::tuple_concatenator < xablau::testing::tuple_concatenator <
			AlgebraTensorDenseDynamicTestRank0Group,
			AlgebraTensorDenseDynamicTestRank1Group > ::tuple,
			AlgebraTensorDenseDynamicTestRank2Group > ::tuple,
			AlgebraTensorDenseDynamicTestRank3Group > ::tuple;

	using AlgebraTensorDenseDynamicTypes = xablau::testing::tuple_transformation < ::testing::Types, AlgebraTensorDenseDynamicTestEveryGroup > ::types;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		Xablau,
		AlgebraTensorDenseFixedTest,
		AlgebraTensorDenseDynamicTypes);
}
