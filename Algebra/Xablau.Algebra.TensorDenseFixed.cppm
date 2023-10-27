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

export module xablau.algebra:tensor_dense_fixed;
export import :complex_functions;
export import :fundamental_concepts;
export import :fundamental_definitions;
export import :fundamental_functions;
export import :standard_functions;
export import :tensor_configurations;
export import :utils;

export import <algorithm>;
export import <array>;
export import <concepts>;
export import <functional>;
export import <iostream>;
export import <numeric>;
export import <optional>;
export import <tuple>;
export import <type_traits>;
export import <utility>;
export import <vector>;

export namespace xablau::algebra
{
	template <
		typename Type,
		concepts::tensor_fixed_dimensionalities FixedDimensionalities,
		concepts::tensor_contiguity Contiguous,
		concepts::tensor_memory_order_indices MemoryOrderIndices = tensor_memory_order_indices <>,
		concepts::tensor_type TensorType = tensor_type < tensor_type_value::main > >
	requires (
		concepts::valid_tensor_fixed_memory_order_indices < FixedDimensionalities, MemoryOrderIndices > &&
		concepts::valid_tensor_type < Type, TensorType >)
	class tensor_dense_fixed final
	{
	public:
		template <
			typename OtherType,
			concepts::tensor_rank OtherRank,
			concepts::tensor_contiguity,
			concepts::tensor_memory_order_indices OtherMemoryOrderIndices,
			concepts::tensor_type OtherTensorType >
		requires (
			concepts::valid_tensor_dynamic_memory_order_indices < OtherRank, OtherMemoryOrderIndices > &&
			concepts::valid_tensor_type < OtherType, OtherTensorType >)
		friend class tensor_dense_dynamic;

		template <
			typename OtherType,
			concepts::tensor_fixed_dimensionalities OtherFixedDimensionalities,
			concepts::tensor_contiguity,
			concepts::tensor_memory_order_indices OtherMemoryOrderIndices,
			concepts::tensor_type OtherTensorType>
		requires (
			concepts::valid_tensor_fixed_memory_order_indices < OtherFixedDimensionalities, OtherMemoryOrderIndices > &&
			concepts::valid_tensor_type < OtherType, OtherTensorType >)
		friend class tensor_dense_fixed;

		using value_type = Type;

		using size_type = size_t;

		using const_reference = const Type &;

		using reference =
			typename std::conditional <
				std::is_const < Type > ::value,
				const_reference,
				Type & > ::type;

		using fixed_dimensionalities = FixedDimensionalities;

		using memory_order_indices = MemoryOrderIndices;

		using main_type =
			tensor_dense_fixed <
				typename std::remove_const < Type > ::type,
				FixedDimensionalities,
				Contiguous,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::main > >;

		template < concepts::tensor_memory_order_indices OtherMemoryOrderIndices >
		using tensor_with_memory_order_indices =
			tensor_dense_fixed <
				Type,
				FixedDimensionalities,
				Contiguous,
				OtherMemoryOrderIndices,
				TensorType >;

	private:
		static constexpr Type _null_value{};

		static constexpr size_t _rank = FixedDimensionalities::size();

		static constexpr bool _contiguous = Contiguous::contiguous();

		static constexpr bool _fixed = true;

		static constexpr bool _sparse = false;

		static constexpr tensor_type_value _type = TensorType::type();

		[[nodiscard]] static consteval auto create_dimensionality_mapping() noexcept
		{
			std::array < size_t, tensor_dense_fixed::_rank > dimensionalityMapping{};

			if constexpr (MemoryOrderIndices::size() == 0)
			{
				if constexpr (tensor_dense_fixed::_rank != 0)
				{
					for (size_t i = 0; i < tensor_dense_fixed::_rank; i++)
					{
						dimensionalityMapping[i] = i;
					}
				}
			}

			else
			{
				dimensionalityMapping = MemoryOrderIndices::array;
			}

			return dimensionalityMapping;
		}

		static constexpr std::array < size_t, tensor_dense_fixed::_rank > _dimensionalities = FixedDimensionalities::array;

		static constexpr std::array < size_t, tensor_dense_fixed::_rank > _dimensionality_mapping =
			tensor_dense_fixed::create_dimensionality_mapping();

		[[nodiscard]] static consteval auto create_dimensionalities_by_memory_orders() noexcept
		{
			std::array < size_t, tensor_dense_fixed::_rank > dimensionalitiesByMemoryOrders{};

			for (size_t i = 0; i < tensor_dense_fixed::_rank; i++)
			{
				dimensionalitiesByMemoryOrders[i] =
					std::ranges::find(tensor_dense_fixed::_dimensionality_mapping, i) - tensor_dense_fixed::_dimensionality_mapping.cbegin();
			}

			return dimensionalitiesByMemoryOrders;
		}

		static constexpr std::array < size_t, tensor_dense_fixed::_rank > _dimensionalities_by_memory_orders =
			tensor_dense_fixed::create_dimensionalities_by_memory_orders();

		static constexpr size_t _size =
			std::accumulate(
				tensor_dense_fixed::_dimensionalities.cbegin(),
				tensor_dense_fixed::_dimensionalities.cend(),
				size_t{1},
				std::multiplies < size_t > ());

		template < size_t index >
		requires (index < tensor_dense_fixed::_rank)
		[[nodiscard]] static consteval auto dimensionality_mapping() noexcept
		{
			return tensor_dense_fixed::_dimensionality_mapping[index];
		}

		template < size_t index >
		requires (index < tensor_dense_fixed::_rank)
		[[nodiscard]] static consteval auto dimensionality_of_order() noexcept
		{
			return tensor_dense_fixed::_dimensionalities_by_memory_orders[index];
		}

		[[nodiscard]] static consteval auto type_deduction_for_contiguous_tensor() noexcept
		{
			if constexpr (tensor_dense_fixed::_type == tensor_type_value::sub)
			{
				return std::array <
					std::optional < std::reference_wrapper < Type > >,
					tensor_dense_fixed::_size > {};
			}

			else
			{
				return std::array < Type, tensor_dense_fixed::_size > {};
			}
		}

		template < size_t rank >
		[[nodiscard]] static consteval auto type_deduction_for_non_contiguous_tensor() noexcept
		{
			if constexpr (rank == 0)
			{
				if constexpr (tensor_dense_fixed::_type == tensor_type_value::sub)
				{
					return std::optional < std::reference_wrapper < Type > > {};
				}

				else
				{
					return Type{};
				}
			}

			else
			{
				return std::array <
					decltype(tensor_dense_fixed::template type_deduction_for_non_contiguous_tensor < rank - 1 > ()),
					tensor_dense_fixed::_dimensionalities[tensor_dense_fixed::dimensionality_mapping < tensor_dense_fixed::_rank - rank > ()] > {};
			}
		}

		typename std::conditional <
			tensor_dense_fixed::_contiguous,
			decltype(tensor_dense_fixed::template type_deduction_for_contiguous_tensor()),
			decltype(tensor_dense_fixed::template type_deduction_for_non_contiguous_tensor < tensor_dense_fixed::_rank > ()) > ::type _data{};

		template < size_t TupleIndex, typename TupleDataType >
		requires (std::tuple_size < TupleDataType > ::value == tensor_dense_fixed::_rank)
		[[nodiscard]] static constexpr auto &non_contiguous_indexing(
			auto &data,
			const TupleDataType &tuple)
		{
			if constexpr (TupleIndex == std::tuple_size < TupleDataType > ::value)
			{
				return data;
			}

			else
			{
				const auto dataIndex =
					static_cast < size_t > (std::get < tensor_dense_fixed::dimensionality_mapping < TupleIndex > () > (tuple));

				return tensor_dense_fixed::template non_contiguous_indexing < TupleIndex + 1 > (data.at(dataIndex), tuple);
			}
		}

		template < size_t DimensionalityIndex >
		[[nodiscard]] static consteval size_t offset_for_contiguous_indexing() noexcept
		{
			size_t offset{1};
			constexpr size_t dimensionalityOrder = tensor_dense_fixed::_dimensionality_mapping[DimensionalityIndex];

			for (size_t i = 0; i < tensor_dense_fixed::_rank; i++)
			{
				if (dimensionalityOrder < tensor_dense_fixed::_dimensionality_mapping[i])
				{
					offset *= tensor_dense_fixed::_dimensionalities[i];
				}
			}

			return offset;
		}

		template < size_t DimensionalityIndex >
		[[nodiscard]] static consteval size_t offset_for_contiguous_indexing() noexcept
		requires (tensor_dense_fixed::_rank == 0)
		{
			return size_t{1};
		}

		template < size_t TupleIndex, size_t TupleLimit, typename TupleDataType >
		requires (
			TupleIndex <= TupleLimit &&
			TupleIndex <= std::tuple_size < TupleDataType > ::value &&
			TupleLimit <= std::tuple_size < TupleDataType > ::value)
		[[nodiscard]] static constexpr size_t offset_for_contiguous_indexing(
			const size_t currentIndex,
			const TupleDataType &tuple)
		{
			if constexpr (TupleIndex == TupleLimit)
			{
				return currentIndex;
			}

			else
			{
				constexpr size_t offset = tensor_dense_fixed::offset_for_contiguous_indexing < TupleIndex > ();

				return tensor_dense_fixed::template offset_for_contiguous_indexing < TupleIndex + 1, TupleLimit > (
					currentIndex + static_cast < size_t > (std::get < TupleIndex > (tuple)) * offset,
					tuple);
			}
		}

		template < size_t TupleIndex, size_t TupleLimit >
		[[nodiscard]] constexpr auto contiguous_indexing(
			auto begin,
			const auto &tuple) const
		{
			const auto offset = tensor_dense_fixed::template offset_for_contiguous_indexing < TupleIndex, TupleLimit > (0, tuple);
			auto _begin = begin + offset;

			if (_begin >= this->_data.end())
			{
				throw std::out_of_range(""); // TODO: Create message.
			}

			return _begin;
		}

		template < typename TupleDataType >
		requires (std::tuple_size < TupleDataType > ::value == tensor_dense_fixed::_rank)
		[[nodiscard]] constexpr const_reference const_parentheses_operator(const TupleDataType &tuple) const
		{
			if constexpr (tensor_dense_fixed::_contiguous)
			{
				const auto element =
					this->template contiguous_indexing < 0, std::tuple_size < TupleDataType > ::value > (this->_data.cbegin(), tuple);

				if constexpr (tensor_dense_fixed::_type == tensor_type_value::sub)
				{
					if (element->has_value())
					{
						return element->value().get();
					}

					else
					{
						return tensor_dense_fixed::_null_value;
					}
				}

				else
				{
					return *element;
				}
			}

			else
			{
				const auto &element = tensor_dense_fixed::template non_contiguous_indexing < 0 > (this->_data, tuple);

				if constexpr (tensor_dense_fixed::_type == tensor_type_value::sub)
				{
					if (element.has_value())
					{
						return element.value().get();
					}

					else
					{
						return tensor_dense_fixed::_null_value;
					}
				}

				else
				{
					return element;
				}
			}
		}

		template < typename TupleDataType >
		requires (std::tuple_size < TupleDataType > ::value == tensor_dense_fixed::_rank)
		[[nodiscard]] constexpr reference parentheses_operator(const TupleDataType &tuple)
		{
			if constexpr (tensor_dense_fixed::_contiguous)
			{
				const auto element =
					this->template contiguous_indexing < 0, std::tuple_size < TupleDataType > ::value > (this->_data.begin(), tuple);

				if constexpr (tensor_dense_fixed::_type == tensor_type_value::sub)
				{
					if (element->has_value())
					{
						return element->value().get();
					}

					else
					{
						throw std::runtime_error(""); // TODO: Create message.
					}
				}

				else
				{
					return *element;
				}
			}

			else
			{
				auto &element = tensor_dense_fixed::template non_contiguous_indexing < 0 > (this->_data, tuple);

				if constexpr (tensor_dense_fixed::_type == tensor_type_value::sub)
				{
					if (element.has_value())
					{
						return element.value().get();
					}

					else
					{
						throw std::runtime_error(""); // TODO: Create message.
					}
				}

				else
				{
					return element;
				}
			}
		}

		template < size_t CurrentRank >
		[[nodiscard]] static constexpr void fill_non_contiguous_tensor(
			auto &data,
			const Type &value)
		{
			if constexpr (CurrentRank + 1 == tensor_dense_fixed::_rank)
			{
				if constexpr (tensor_dense_fixed::_type == tensor_type_value::sub)
				{
					for (auto &cell : data)
					{
						if (cell.has_value())
						{
							cell.value().get() = value;
						}
					}
				}

				else
				{
					data.fill(value);
				}
			}

			else
			{
				for (auto &line : data)
				{
					tensor_dense_fixed::template fill_non_contiguous_tensor < CurrentRank + 1 > (line, value);
				}
			}
		}

		[[nodiscard]] constexpr auto convert_to_tensor_dense_dynamic_for_contiguous_tensor() const
		{
			tensor_dense_dynamic <
				Type,
				tensor_rank < tensor_dense_fixed::_rank >,
				tensor_contiguity < true >,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::main > > tensor{};

			if constexpr (tensor_dense_fixed::_rank == 0)
			{
				*tensor = *(*this);
			}

			else
			{
				tensor._data.reserve(this->_data.size());

				std::copy(this->_data.cbegin(), this->_data.cend(), std::back_inserter(tensor._data));

				tensor._dimensionalities = tensor_dense_fixed::_dimensionalities;
				tensor._size = tensor_dense_fixed::_size;
			}

			return tensor;
		}

		template < size_t CurrentRank >
		[[nodiscard]] static constexpr void convert_to_tensor_dense_dynamic_for_non_contiguous_tensor(
			auto &dynamicData,
			const auto &fixedData)
		{
			if constexpr (CurrentRank + 1 == tensor_dense_fixed::_rank)
			{
				dynamicData.reserve(fixedData.size());

				std::copy(fixedData.cbegin(), fixedData.cend(), std::back_inserter(dynamicData));
			}

			else
			{
				dynamicData.resize(fixedData.size());

				for (size_t i = 0; i < fixedData.size(); i++)
				{
					tensor_dense_fixed::template convert_to_tensor_dense_dynamic_for_non_contiguous_tensor < CurrentRank + 1 > (
						dynamicData[i], fixedData[i]);
				}
			}
		}

		[[nodiscard]] constexpr auto convert_to_tensor_dense_dynamic_for_non_contiguous_tensor() const
		{
			tensor_dense_dynamic <
				Type,
				tensor_rank < tensor_dense_fixed::_rank >,
				tensor_contiguity < false >,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::main > > tensor{};

			if constexpr (tensor_dense_fixed::_rank == 0)
			{
				*tensor = *(*this);
			}

			else
			{
				tensor_dense_fixed::convert_to_tensor_dense_dynamic_for_non_contiguous_tensor < 0 > (tensor._data, this->_data);
			}

			tensor._dimensionalities = tensor_dense_fixed::_dimensionalities;
			tensor._size = tensor_dense_fixed::_size;

			return tensor;
		}

		template < size_t CurrentRank >
		requires (CurrentRank < tensor_dense_fixed::_rank)
		[[nodiscard]] static constexpr void binary_operator_for_non_contiguous_tensor(
			auto &data,
			const auto &binaryOperator,
			const Type &value)
		{
			if constexpr (CurrentRank + 1 == tensor_dense_fixed::_rank)
			{
				for (auto &thisValue : data)
				{
					binaryOperator(thisValue, value);
				}
			}

			else
			{
				for (auto &line : data)
				{
					tensor_dense_fixed::
						template binary_operator_for_non_contiguous_tensor < CurrentRank + 1 > (line, binaryOperator, value);
				}
			}
		}

		template < size_t CurrentRank >
		requires (CurrentRank < tensor_dense_fixed::_rank)
		[[nodiscard]] static constexpr void element_wise_binary_operator_for_non_contiguous_tensor(
			auto &data,
			const auto &otherData,
			const auto &binaryOperator)
		{
			if constexpr (CurrentRank + 1 == tensor_dense_fixed::_rank)
			{
				for (size_t i = 0; i < data.size(); i++)
				{
					binaryOperator(data[i], otherData[i]);
				}
			}

			else
			{
				for (size_t i = 0; i < data.size(); i++)
				{
					tensor_dense_fixed::
						template element_wise_binary_operator_for_non_contiguous_tensor < CurrentRank + 1 > (
							data[i],
							otherData[i],
							binaryOperator);
				}
			}
		}

		template < size_t TupleIndex >
		static constexpr void kernel_for_non_contiguous_tensor(
			auto &data,
			auto &kernelData,
			const auto &tuple,
			const std::array < size_t, tensor_dense_fixed::_rank > &kernelSize)
		{
			size_t dataIndex{};
			size_t kernelIndex{};
			constexpr auto mappedDimensionality = tensor_dense_fixed::dimensionality_mapping < TupleIndex > ();
			constexpr auto boundary = tensor_dense_fixed::_dimensionalities[mappedDimensionality];
			const auto center = std::get < mappedDimensionality > (tuple);
			const size_t offset = kernelSize[mappedDimensionality] / 2;

			if (center >= offset)
			{
				dataIndex = center - offset;
			}

			else
			{
				kernelIndex = offset - center;
			}

			while (dataIndex <= center + offset && dataIndex < boundary)
			{
				if constexpr (TupleIndex + 1 == tensor_dense_fixed::_rank)
				{
					kernelData.at(kernelIndex).emplace(data.at(dataIndex));
				}

				else
				{
					tensor_dense_fixed::template kernel_for_non_contiguous_tensor < TupleIndex + 1 > (
						data.at(dataIndex),
						kernelData.at(kernelIndex),
						tuple,
						kernelSize);
				}

				dataIndex++;
				kernelIndex++;
			}
		}

		template < size_t TupleIndex >
		requires (TupleIndex < tensor_dense_fixed::_rank)
		static constexpr void kernel_for_contiguous_tensor(
			auto iteratorData,
			auto iteratorKernelData,
			const auto &kernel,
			const auto &tuple,
			const std::array < size_t, tensor_dense_fixed::_rank > &kernelSize)
		{
			size_t dataIndex{};
			size_t kernelIndex{};
			constexpr auto boundary = tensor_dense_fixed::_dimensionalities[TupleIndex];
			const auto center = std::get < TupleIndex > (tuple);
			const size_t offset = kernelSize[TupleIndex] / 2;

			if (center >= offset)
			{
				dataIndex = center - offset;
			}

			else
			{
				kernelIndex = offset - center;
			}

			constexpr auto offsetData = tensor_dense_fixed::offset_for_contiguous_indexing < TupleIndex > ();
			const auto offsetKernel = kernel.offset_for_contiguous_indexing < TupleIndex > ();

			iteratorData += dataIndex * offsetData;
			iteratorKernelData += kernelIndex * offsetKernel;

			while (dataIndex <= center + offset && dataIndex < boundary)
			{
				if constexpr (TupleIndex + 1 < tensor_dense_fixed::_rank)
				{
					tensor_dense_fixed::template kernel_for_contiguous_tensor < TupleIndex + 1 > (
						iteratorData,
						iteratorKernelData,
						kernel,
						tuple,
						kernelSize);
				}

				else
				{
					iteratorKernelData->emplace(*iteratorData);
				}

				if (dataIndex + 1 < boundary &&
					kernelIndex + 1 < kernel._dimensionalities[TupleIndex])
				{
					iteratorData += offsetData;
					iteratorKernelData += offsetKernel;
				}

				dataIndex++;
				kernelIndex++;
			}
		}

		template < bool DynamicKernel >
		static constexpr void kernel(
			auto &data,
			auto &kernel,
			const auto &tuple,
			const std::array < size_t, tensor_dense_fixed::_rank > &kernelSize)
		{
			if constexpr (DynamicKernel)
			{
				if (!utils::valid_kernel_size(kernelSize))
				{
					throw std::runtime_error(""); // TODO: Create message.
				}

				kernel.resize(kernelSize);
			}

			if constexpr (tensor_dense_fixed::_contiguous)
			{
				tensor_dense_fixed::template kernel_for_contiguous_tensor < 0 > (
					data.begin(),
					kernel._data.begin(),
					kernel,
					tuple,
					kernelSize);
			}

			else
			{
				tensor_dense_fixed::template kernel_for_non_contiguous_tensor < 0 > (
					data,
					kernel._data,
					tuple,
					kernelSize);
			}
		}

		template < bool DynamicKernel, typename ... DimensionalityTypes >
		requires (std::convertible_to < DimensionalityTypes, size_t > && ...)
		static constexpr void kernel(
			auto &data,
			auto &kernel,
			const std::array < size_t, tensor_dense_fixed::_rank > &kernelSize,
			DimensionalityTypes && ... args)
		{
			const auto kernelCenter = std::forward_as_tuple(args...);

			tensor_dense_fixed::kernel < DynamicKernel > (data, kernel, kernelCenter, kernelSize);
		}

		template <
			bool DynamicKernel,
			typename ... KernelCenterCoordinatesAndKernelSize,
			size_t ... DimensionalityIndices >
		static constexpr void kernel(
			auto &data,
			auto &kernel,
			std::tuple < KernelCenterCoordinatesAndKernelSize... > kernelCenterCoordinatesAndKernelSize,
			std::index_sequence < DimensionalityIndices... >)
		{
			constexpr auto kernelSize = sizeof...(KernelCenterCoordinatesAndKernelSize) - 1;

			tensor_dense_fixed::kernel < DynamicKernel > (
				data,
				kernel,
				std::get < DimensionalityIndices > (kernelCenterCoordinatesAndKernelSize)...,
				std::get < kernelSize > (kernelCenterCoordinatesAndKernelSize));
		}

		constexpr tensor_dense_fixed() noexcept
		requires (tensor_dense_fixed::_type == tensor_type_value::sub)
		{
		}

		template < size_t UnitsInTheLastPlace >
		void assert_Hermitianess() const
		{
			for (size_t i = 0; i < tensor_dense_fixed::rows(); i++)
			{
				for (size_t j = i; j < tensor_dense_fixed::columns(); j++)
				{
					if (!xablau::algebra::functions::almost_equal < Type > ::invoke < UnitsInTheLastPlace > (
						(*this)(i, j),
						xablau::algebra::functions::conjugate < Type > ::invoke((*this)(j, i))))
					{
						throw std::domain_error(""); // TODO: Create message.
					}
				}
			}
		}

		template < typename MatrixType >
		static constexpr void addition_assignment(tensor_dense_fixed &thisTensor, const MatrixType &addend)
		{
			std::array < size_t, tensor_dense_fixed::_rank > indices{};

			for (size_t i = 0; i < thisTensor._size; i++)
			{
				if constexpr (tensor_dense_fixed::_type == tensor_type_value::main)
				{
					thisTensor(indices) += addend(indices);
				}

				else
				{
					auto &element = thisTensor(indices);

					if (element.has_value())
					{
						element.value().get() += addend(indices);
					}
				}

				utils::increment_index(indices, thisTensor._dimensionalities);
			}
		}

		template < typename MatrixType >
		requires (
			MatrixType::contiguous() == tensor_dense_fixed::contiguous() &&
			std::same_as < typename MatrixType::memory_order_indices, memory_order_indices >)
		static constexpr void addition_assignment(tensor_dense_fixed &thisTensor, const MatrixType &addend)
		{
			constexpr auto binaryOperator =
				[] < typename ThisElementType, typename OtherElementType > (
					ThisElementType &thisValue,
					const OtherElementType &otherValue) -> void
				{
					if constexpr (tensor_dense_fixed::_type == tensor_type_value::main)
					{
						if constexpr (MatrixType::type() == tensor_type_value::main)
						{
							thisValue += otherValue;
						}

						else
						{
							if (otherValue.has_value())
							{
								thisValue += otherValue.value().get();
							}
						}
					}

					else
					{
						if (!thisValue.has_value())
						{
							return;
						}

						if constexpr (MatrixType::type() == tensor_type_value::main)
						{
							thisValue.value().get() += otherValue;
						}

						else
						{
							if (otherValue.has_value())
							{
								thisValue.value().get() += otherValue.value().get();
							}
						}
					}
				};

			if constexpr (tensor_dense_fixed::_contiguous)
			{
				for (size_t i = 0; i < thisTensor._size; i++)
				{
					binaryOperator(thisTensor._data[i], addend._data[i]);
				}
			}

			else if constexpr (tensor_dense_fixed::_rank == 0)
			{
				binaryOperator(thisTensor._data, addend._data);
			}

			else
			{
				tensor_dense_fixed::
					template element_wise_binary_operator_for_non_contiguous_tensor < 0 > (
						thisTensor._data,
						addend._data,
						binaryOperator);
			}
		}

		template < typename MatrixType >
		static constexpr void subtraction_assignment(tensor_dense_fixed &thisTensor, const MatrixType &subtrahend)
		{
			std::array < size_t, tensor_dense_fixed::_rank > indices{};

			for (size_t i = 0; i < thisTensor._size; i++)
			{
				if constexpr (tensor_dense_fixed::_type == tensor_type_value::main)
				{
					thisTensor(indices) -= subtrahend(indices);
				}

				else
				{
					auto &element = thisTensor(indices);

					if (element.has_value())
					{
						element.value().get() -= subtrahend(indices);
					}
				}

				utils::increment_index(indices, thisTensor._dimensionalities);
			}
		}

		template < typename MatrixType >
		requires (
			MatrixType::contiguous() == tensor_dense_fixed::contiguous() &&
			std::same_as < typename MatrixType::memory_order_indices, memory_order_indices >)
		static constexpr void subtraction_assignment(tensor_dense_fixed &thisTensor, const MatrixType &subtrahend)
		{
			constexpr auto binaryOperator =
				[] < typename ThisElementType, typename OtherElementType > (
					ThisElementType &thisValue,
					const OtherElementType &otherValue) -> void
				{
					if constexpr (tensor_dense_fixed::_type == tensor_type_value::main)
					{
						if constexpr (MatrixType::type() == tensor_type_value::main)
						{
							thisValue -= otherValue;
						}

						else
						{
							if (otherValue.has_value())
							{
								thisValue -= otherValue.value().get();
							}
						}
					}

					else
					{
						if (!thisValue.has_value())
						{
							return;
						}

						if constexpr (MatrixType::type() == tensor_type_value::main)
						{
							thisValue.value().get() -= otherValue;
						}

						else
						{
							if (otherValue.has_value())
							{
								thisValue.value().get() -= otherValue.value().get();
							}
						}
					}
				};

			if constexpr (tensor_dense_fixed::_contiguous)
			{
				for (size_t i = 0; i < thisTensor._size; i++)
				{
					binaryOperator(thisTensor._data[i], subtrahend._data[i]);
				}
			}

			else if constexpr (tensor_dense_fixed::_rank == 0)
			{
				binaryOperator(thisTensor._data, subtrahend._data);
			}

			else
			{
				tensor_dense_fixed::
					template element_wise_binary_operator_for_non_contiguous_tensor < 0 > (
						thisTensor._data,
						subtrahend._data,
						binaryOperator);
			}
		}

	public:
		[[nodiscard]] static consteval size_t rank() noexcept
		{
			return tensor_dense_fixed::_rank;
		}

		[[nodiscard]] static consteval bool contiguous() noexcept
		{
			return tensor_dense_fixed::_contiguous;
		}

		[[nodiscard]] static consteval bool fixed() noexcept
		{
			return tensor_dense_fixed::_fixed;
		}

		[[nodiscard]] static consteval bool sparse() noexcept
		{
			return tensor_dense_fixed::_sparse;
		}

		[[nodiscard]] static consteval tensor_type_value type() noexcept
		{
			return tensor_dense_fixed::_type;
		}

		[[nodiscard]] static consteval size_t size() noexcept
		{
			return tensor_dense_fixed::_size;
		}

		[[nodiscard]] static consteval const std::array < size_t, tensor_dense_fixed::_rank > &dimensionalities() noexcept
		{
			return tensor_dense_fixed::_dimensionalities;
		}

		[[nodiscard]] static consteval const std::array < size_t, tensor_dense_fixed::_rank > &dimensionality_mapping() noexcept
		{
			return tensor_dense_fixed::_dimensionality_mapping;
		}

		[[nodiscard]] static consteval const std::array < size_t, tensor_dense_fixed::_rank > &dimensionalities_by_memory_orders() noexcept
		{
			return tensor_dense_fixed::_dimensionalities_by_memory_orders;
		}

		[[nodiscard]] constexpr const auto &data() const
		requires (tensor_dense_fixed::_type == tensor_type_value::main)
		{
			return this->_data;
		}

		[[nodiscard]] constexpr auto &data()
		requires (tensor_dense_fixed::_type == tensor_type_value::main)
		{
			return this->_data;
		}

		[[nodiscard]] constexpr auto convert_to_tensor_dense_dynamic() const
		requires (tensor_dense_fixed::_type == tensor_type_value::main)
		{
			if constexpr (tensor_dense_fixed::_contiguous)
			{
				return this->convert_to_tensor_dense_dynamic_for_contiguous_tensor();
			}

			else
			{
				return this->convert_to_tensor_dense_dynamic_for_non_contiguous_tensor();
			}
		}

		template < concepts::tensor_fixed_dimensionalities KernelSize, typename ... Types >
		requires (
			concepts::valid_tensor_fixed_kernel_size < tensor_dense_fixed::_rank, KernelSize > &&
			sizeof...(Types) == tensor_dense_fixed::_rank &&
			(std::convertible_to < Types, size_t > && ...))
		[[nodiscard]] constexpr auto kernel(const std::tuple < Types ... > &kernelCenter) const
		requires (tensor_dense_fixed::_rank != 0)
		{
			tensor_dense_fixed <
				typename std::remove_reference < const_reference > ::type,
				KernelSize,
				tensor_contiguity < tensor_dense_fixed::_contiguous >,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::sub > > _kernel{};

			tensor_dense_fixed::kernel < false > (this->_data, _kernel, kernelCenter, KernelSize::array);

			return _kernel;
		}

		template < concepts::tensor_fixed_dimensionalities KernelSize, typename ... Types >
		requires (
			concepts::valid_tensor_fixed_kernel_size < tensor_dense_fixed::_rank, KernelSize > &&
			sizeof...(Types) == tensor_dense_fixed::_rank &&
			(std::convertible_to < Types, size_t > && ...))
		[[nodiscard]] constexpr auto kernel(const std::tuple < Types ... > &kernelCenter)
		requires (tensor_dense_fixed::_rank != 0)
		{
			tensor_dense_fixed <
				typename std::remove_reference < reference > ::type,
				KernelSize,
				tensor_contiguity < tensor_dense_fixed::_contiguous >,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::sub > > _kernel{};

			tensor_dense_fixed::kernel < false > (this->_data, _kernel, kernelCenter, KernelSize::array);

			return _kernel;
		}

		template < typename ... Types >
		requires (
			sizeof...(Types) == tensor_dense_fixed::_rank &&
			(std::convertible_to < Types, size_t > && ...))
		[[nodiscard]] constexpr auto kernel(
			const std::tuple < Types ... > &kernelCenter,
			const std::array < size_t, tensor_dense_fixed::_rank > &kernelSize) const
		requires (tensor_dense_fixed::_rank != 0)
		{
			tensor_dense_dynamic <
				typename std::remove_reference < const_reference > ::type,
				tensor_rank < tensor_dense_fixed::_rank >,
				tensor_contiguity < tensor_dense_fixed::_contiguous >,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::sub > > _kernel{};

			tensor_dense_fixed::kernel < true > (this->_data, _kernel, kernelCenter, kernelSize);

			return _kernel;
		}

		template < typename ... Types >
		requires (
			sizeof...(Types) == tensor_dense_fixed::_rank &&
			(std::convertible_to < Types, size_t > && ...))
		[[nodiscard]] constexpr auto kernel(
			const std::tuple < Types ... > &kernelCenter,
			const std::array < size_t, tensor_dense_fixed::_rank > &kernelSize)
		requires (tensor_dense_fixed::_rank != 0)
		{
			tensor_dense_dynamic <
				typename std::remove_reference < reference > ::type,
				tensor_rank < tensor_dense_fixed::_rank >,
				tensor_contiguity < tensor_dense_fixed::_contiguous >,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::sub > > _kernel{};

			tensor_dense_fixed::kernel < true > (this->_data, _kernel, kernelCenter, kernelSize);

			return _kernel;
		}

		template < concepts::tensor_fixed_dimensionalities KernelSize, typename ... KernelCenterCoordinates >
		requires (
			concepts::valid_tensor_fixed_kernel_size < tensor_dense_fixed::_rank, KernelSize > &&
			sizeof...(KernelCenterCoordinates) == tensor_dense_fixed::_rank &&
			(std::convertible_to < KernelCenterCoordinates, size_t > && ...))
		[[nodiscard]] constexpr auto kernel(KernelCenterCoordinates && ... args) const
		requires (tensor_dense_fixed::_rank != 0)
		{
			tensor_dense_fixed <
				typename std::remove_reference < const_reference > ::type,
				KernelSize,
				tensor_contiguity < tensor_dense_fixed::_contiguous >,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::sub > > _kernel{};

			tensor_dense_fixed::kernel < false > (this->_data, _kernel, std::forward_as_tuple(args...), KernelSize::array);

			return _kernel;
		}

		template < concepts::tensor_fixed_dimensionalities KernelSize, typename ... KernelCenterCoordinates >
		requires (
			concepts::valid_tensor_fixed_kernel_size < tensor_dense_fixed::_rank, KernelSize > &&
			sizeof...(KernelCenterCoordinates) == tensor_dense_fixed::_rank &&
			(std::convertible_to < KernelCenterCoordinates, size_t > && ...))
		[[nodiscard]] constexpr auto kernel(KernelCenterCoordinates && ... args)
		requires (tensor_dense_fixed::_rank != 0)
		{
			tensor_dense_fixed <
				typename std::remove_reference < reference > ::type,
				KernelSize,
				tensor_contiguity < tensor_dense_fixed::_contiguous >,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::sub > > _kernel{};

			tensor_dense_fixed::kernel < false > (this->_data, _kernel, std::forward_as_tuple(args...), KernelSize::array);

			return _kernel;
		}

		template < typename ... KernelCenterCoordinatesAndKernelSize >
		requires (sizeof...(KernelCenterCoordinatesAndKernelSize) == (tensor_dense_fixed::_rank + 1))
		[[nodiscard]] constexpr auto kernel(KernelCenterCoordinatesAndKernelSize && ... args) const
		requires (tensor_dense_fixed::_rank != 0)
		{
			tensor_dense_dynamic <
				typename std::remove_reference < const_reference > ::type,
				tensor_rank < tensor_dense_fixed::_rank >,
				tensor_contiguity < tensor_dense_fixed::_contiguous >,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::sub > > _kernel{};

			tensor_dense_fixed::kernel < true > (
				this->_data,
				_kernel,
				std::forward_as_tuple(args...),
				std::make_index_sequence < sizeof...(args) - 1 > {});

			return _kernel;
		}

		template < typename ... KernelCenterCoordinatesAndKernelSize >
		requires (sizeof...(KernelCenterCoordinatesAndKernelSize) == (tensor_dense_fixed::_rank + 1))
		[[nodiscard]] constexpr auto kernel(KernelCenterCoordinatesAndKernelSize && ... args)
		requires (tensor_dense_fixed::_rank != 0)
		{
			tensor_dense_dynamic <
				typename std::remove_reference < reference > ::type,
				tensor_rank < tensor_dense_fixed::_rank >,
				tensor_contiguity < tensor_dense_fixed::_contiguous >,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::sub > > _kernel{};

			tensor_dense_fixed::kernel < true > (
				this->_data,
				_kernel,
				std::forward_as_tuple(args...),
				std::make_index_sequence < sizeof...(args) - 1 > {});

			return _kernel;
		}

		template < concepts::tensor_fixed_dimensionalities KernelSize >
		requires (concepts::valid_tensor_fixed_kernel_size < tensor_dense_fixed::_rank, KernelSize >)
		[[nodiscard]] constexpr auto kernel(const std::array < size_t, tensor_dense_fixed::_rank > &kernelCenter) const
		requires (tensor_dense_fixed::_rank != 0)
		{
			return this->kernel < KernelSize > (std::tuple_cat(kernelCenter));
		}

		template < concepts::tensor_fixed_dimensionalities KernelSize >
		requires (concepts::valid_tensor_fixed_kernel_size < tensor_dense_fixed::_rank, KernelSize >)
		[[nodiscard]] constexpr auto kernel(const std::array < size_t, tensor_dense_fixed::_rank > &kernelCenter)
		requires (tensor_dense_fixed::_rank != 0)
		{
			return this->kernel < KernelSize > (std::tuple_cat(kernelCenter));
		}

		[[nodiscard]] constexpr auto kernel(
			const std::array < size_t, tensor_dense_fixed::_rank > &kernelCenter,
			const std::array < size_t, tensor_dense_fixed::_rank > &kernelSize) const
		requires (tensor_dense_fixed::_rank != 0)
		{
			return this->kernel(std::tuple_cat(kernelCenter), kernelSize);
		}

		[[nodiscard]] constexpr auto kernel(
			const std::array < size_t, tensor_dense_fixed::_rank > &kernelCenter,
			const std::array < size_t, tensor_dense_fixed::_rank > &kernelSize)
		requires (tensor_dense_fixed::_rank != 0)
		{
			return this->kernel(std::tuple_cat(kernelCenter), kernelSize);
		}

		[[nodiscard]] static consteval size_t rows()
		requires (tensor_dense_fixed::_rank == 2)
		{
			return tensor_dense_fixed::_dimensionalities[0];
		}

		[[nodiscard]] static consteval size_t columns()
		requires (tensor_dense_fixed::_rank == 2)
		{
			return tensor_dense_fixed::_dimensionalities[1];
		}

		[[nodiscard]] constexpr auto row(const size_t index) const
		requires (
			tensor_dense_fixed::_rank == 2 &&
			tensor_dense_fixed::_size != 0)
		{
			if (index >= tensor_dense_fixed::rows())
			{
				throw std::runtime_error(""); // TODO: Create message.
			}

			tensor_dense_fixed <
				typename std::remove_reference < const_reference > ::type,
				tensor_fixed_dimensionalities < 1, tensor_dense_fixed::columns() >,
				tensor_contiguity < true >,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::sub > > _kernel{};

			// TODO: Stop using naive solution.
			for (size_t i = 0; i < tensor_dense_fixed::columns(); i++)
			{
				_kernel._data[i].emplace((*this)(index, i));
			}

			return _kernel;
		}

		[[nodiscard]] constexpr auto row(const size_t index)
		requires (
			tensor_dense_fixed::_rank == 2 &&
			tensor_dense_fixed::_size != 0)
		{
			if (index >= tensor_dense_fixed::rows())
			{
				throw std::runtime_error(""); // TODO: Create message.
			}

			tensor_dense_fixed <
				typename std::remove_reference < reference > ::type,
				tensor_fixed_dimensionalities < 1, tensor_dense_fixed::columns() >,
				tensor_contiguity < true >,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::sub > > _kernel{};

			// TODO: Stop using naive solution.
			for (size_t i = 0; i < tensor_dense_fixed::columns(); i++)
			{
				_kernel._data[i].emplace((*this)(index, i));
			}

			return _kernel;
		}

		[[nodiscard]] constexpr auto column(const size_t index) const
		requires (
			tensor_dense_fixed::_rank == 2 &&
			tensor_dense_fixed::_size != 0)
		{
			if (index >= tensor_dense_fixed::columns())
			{
				throw std::runtime_error(""); // TODO: Create message.
			}

			tensor_dense_fixed <
				typename std::remove_reference < const_reference > ::type,
				tensor_fixed_dimensionalities < tensor_dense_fixed::rows(), 1 >,
				tensor_contiguity < true >,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::sub > > _kernel{};

			// TODO: Stop using naive solution.
			for (size_t i = 0; i < tensor_dense_fixed::rows(); i++)
			{
				_kernel._data[i].emplace((*this)(i, index));
			}

			return _kernel;
		}

		[[nodiscard]] constexpr auto column(const size_t index)
		requires (
			tensor_dense_fixed::_rank == 2 &&
			tensor_dense_fixed::_size != 0)
		{
			if (index >= tensor_dense_fixed::columns())
			{
				throw std::runtime_error(""); // TODO: Create message.
			}

			tensor_dense_fixed <
				typename std::remove_reference < reference > ::type,
				tensor_fixed_dimensionalities < tensor_dense_fixed::rows(), 1 >,
				tensor_contiguity < true >,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::sub > > _kernel{};

			// TODO: Stop using naive solution.
			for (size_t i = 0; i < tensor_dense_fixed::rows(); i++)
			{
				_kernel._data[i].emplace((*this)(i, index));
			}

			return _kernel;
		}

		template < size_t Index >
		requires (Index < tensor_dense_fixed::rows())
		[[nodiscard]] constexpr auto row() const
		requires (
			tensor_dense_fixed::_rank == 2 &&
			tensor_dense_fixed::_size != 0)
		{
			tensor_dense_fixed <
				typename std::remove_reference < const_reference > ::type,
				tensor_fixed_dimensionalities < 1, tensor_dense_fixed::columns() >,
				tensor_contiguity < true >,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::sub > > _kernel{};

			// TODO: Stop using naive solution.
			for (size_t i = 0; i < tensor_dense_fixed::columns(); i++)
			{
				_kernel._data[i].emplace((*this)(Index, i));
			}

			return _kernel;
		}

		template < size_t Index >
		requires (Index < tensor_dense_fixed::rows())
		[[nodiscard]] constexpr auto row()
		requires (
			tensor_dense_fixed::_rank == 2 &&
			tensor_dense_fixed::_size != 0)
		{
			tensor_dense_fixed <
				typename std::remove_reference < reference > ::type,
				tensor_fixed_dimensionalities < 1, tensor_dense_fixed::columns() >,
				tensor_contiguity < true >,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::sub > > _kernel{};

			// TODO: Stop using naive solution.
			for (size_t i = 0; i < tensor_dense_fixed::columns(); i++)
			{
				_kernel._data[i].emplace((*this)(Index, i));
			}

			return _kernel;
		}

		template < size_t Index >
		requires (Index < tensor_dense_fixed::columns())
		[[nodiscard]] constexpr auto column() const
		requires (
			tensor_dense_fixed::_rank == 2 &&
			tensor_dense_fixed::_size != 0)
		{
			tensor_dense_fixed <
				typename std::remove_reference < const_reference > ::type,
				tensor_fixed_dimensionalities < tensor_dense_fixed::rows(), 1 >,
				tensor_contiguity < true >,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::sub > > _kernel{};

			// TODO: Stop using naive solution.
			for (size_t i = 0; i < tensor_dense_fixed::rows(); i++)
			{
				_kernel._data[i].emplace((*this)(i, Index));
			}

			return _kernel;
		}

		template < size_t Index >
		requires (Index < tensor_dense_fixed::columns())
		[[nodiscard]] constexpr auto column()
		requires (
			tensor_dense_fixed::_rank == 2 &&
			tensor_dense_fixed::_size != 0)
		{
			tensor_dense_fixed <
				typename std::remove_reference < reference > ::type,
				tensor_fixed_dimensionalities < tensor_dense_fixed::rows(), 1 >,
				tensor_contiguity < true >,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::sub > > _kernel{};

			// TODO: Stop using naive solution.
			for (size_t i = 0; i < tensor_dense_fixed::rows(); i++)
			{
				_kernel._data[i].emplace((*this)(i, Index));
			}

			return _kernel;
		}

		template < typename ... Types >
		requires (sizeof...(Types) == tensor_dense_fixed::_rank && (std::convertible_to < Types, size_t > && ...))
		[[nodiscard]] constexpr bool has_reference_to_main(Types && ... args) const
		requires (tensor_dense_fixed::_type == tensor_type_value::sub)
		{
			return this->has_reference_to_main(std::forward_as_tuple(args...));
		}

		template < typename ... Types >
		requires (sizeof...(Types) == tensor_dense_fixed::_rank && (std::convertible_to < Types, size_t > && ...))
		[[nodiscard]] constexpr bool has_reference_to_main(const std::tuple < Types ... > &tuple) const
		requires (tensor_dense_fixed::_type == tensor_type_value::sub)
		{
			if constexpr (tensor_dense_fixed::_contiguous)
			{
				return
					tensor_dense_fixed::template contiguous_indexing < 0, tensor_dense_fixed::_rank > (
						this->_data.cbegin(), tuple)->has_value();
			}

			else
			{
				const auto &data = tensor_dense_fixed::template non_contiguous_indexing < 0 > (this->_data, tuple);

				return data.has_value();
			}
		}

		[[nodiscard]] constexpr bool has_reference_to_main(const std::array < size_t, tensor_dense_fixed::_rank > &args) const
		requires (tensor_dense_fixed::_type == tensor_type_value::sub)
		{
			return this->has_reference_to_main(std::tuple_cat(args));
		}

		[[nodiscard]] constexpr main_type convert_to_main() const
		requires (tensor_dense_fixed::_type == tensor_type_value::sub)
		{
			main_type result{};

			constexpr auto binaryOperator =
				[] < typename ThisElementType, typename OtherElementType > (
					ThisElementType &thisValue,
					const OtherElementType &otherValue) -> void
				{
					if (otherValue.has_value())
					{
						thisValue = otherValue.value().get();
					}
				};

			if constexpr (tensor_dense_fixed::_contiguous)
			{
				for (size_t i = 0; i < this->_size; i++)
				{
					binaryOperator(result._data[i], this->_data[i]);
				}
			}

			else if constexpr (tensor_dense_fixed::_rank == 0)
			{
				binaryOperator(result._data, this->_data);
			}

			else
			{
				tensor_dense_fixed::
					template element_wise_binary_operator_for_non_contiguous_tensor < 0 > (result._data, this->_data, binaryOperator);
			}

			return result;
		}

		[[nodiscard]] Type determinant() const
		requires (
			std::equality_comparable < Type > &&
			algebra::concepts::negatable_basic_assignable_arithmetic < Type > &&
			tensor_dense_fixed::_rank == 2 &&
			tensor_dense_fixed::dimensionalities()[0] == tensor_dense_fixed::dimensionalities()[1])
		{
			auto determinant = functions::multiplication_identity < Type > ::invoke();
			const auto [swaps, LU, _] = utils::LUP_decomposition(*this);

			for (size_t i = 0; i < tensor_dense_fixed::dimensionalities()[0]; i++)
			{
				determinant *= LU(i, i);
			}

			if (swaps % 2 == 0)
			{
				return determinant;
			}

			return -determinant;
		}

		[[nodiscard]] auto LUP() const
		requires (
			std::equality_comparable < Type > &&
			algebra::concepts::basic_assignable_arithmetic < Type > &&
			tensor_dense_fixed::_rank == 2 &&
			tensor_dense_fixed::dimensionalities()[0] == tensor_dense_fixed::dimensionalities()[1])
		{
			auto [_, LU, P] = utils::LUP_decomposition(*this);

			return std::make_pair(std::move(LU), std::move(P));
		}

		[[nodiscard]] auto inverse() const
		requires (
			std::equality_comparable < Type > &&
			algebra::concepts::basic_assignable_arithmetic < Type > &&
			tensor_dense_fixed::_rank == 2 &&
			tensor_dense_fixed::dimensionalities()[0] == tensor_dense_fixed::dimensionalities()[1])
		{
			tensor_dense_fixed identity{};

			for (size_t i = 0; i < tensor_dense_fixed::dimensionalities()[0]; i++)
			{
				identity(i, i) = xablau::algebra::functions::multiplication_identity < Type > ::invoke();
			}

			const auto [_, LU, P] = utils::LUP_decomposition(*this);
			const auto Pb = P * identity;

			return utils::LU_solve(LU, Pb);
		}

		[[nodiscard]] auto QR() const
		requires (
			std::equality_comparable < Type > &&
			concepts::basic_assignable_arithmetic < Type > &&
			tensor_dense_fixed::_rank == 2)
		{
			using QType =
				tensor_dense_fixed <
					typename std::remove_const < Type > ::type,
					xablau::algebra::tensor_fixed_dimensionalities <
						tensor_dense_fixed::dimensionalities()[0],
						tensor_dense_fixed::dimensionalities()[0] >,
					tensor_contiguity < tensor_dense_fixed::_contiguous >,
					MemoryOrderIndices,
					tensor_type < tensor_type_value::main > >;

			return
				utils::QR <
					typename xablau::algebra::abs_type < Type > ::type,
					QType > (*this);
		}

		template < bool CalculateEigenvectors, bool CheckHermitianess, size_t UnitsInTheLastPlace = 0 >
		[[nodiscard]] auto eigen_Hermitian() const
		requires (
			std::equality_comparable < Type > &&
			concepts::basic_assignable_arithmetic < Type > &&
			tensor_dense_fixed::_rank == 2 &&
			tensor_dense_fixed::dimensionalities()[0] == tensor_dense_fixed::dimensionalities()[1])
		{
			using EigenvaluesVector =
				tensor_dense_fixed <
					typename std::remove_const < Type > ::type,
					xablau::algebra::tensor_fixed_dimensionalities < tensor_dense_fixed::dimensionalities()[0], 1 >,
					tensor_contiguity < tensor_dense_fixed::_contiguous >,
					MemoryOrderIndices,
					tensor_type < tensor_type_value::main > >;

			if constexpr (CheckHermitianess)
			{
				this->assert_Hermitianess < UnitsInTheLastPlace > ();
			}

			return
				utils::eigen_Hermitian <
					CalculateEigenvectors,
					EigenvaluesVector,
					typename xablau::algebra::abs_type < Type > ::type,
					tensor_dense_fixed > (*this);
		}

		[[nodiscard]] constexpr const_reference operator*() const noexcept
		requires (
			tensor_dense_fixed::_rank == 0 &&
			tensor_dense_fixed::_type == tensor_type_value::main)
		{
			if constexpr (tensor_dense_fixed::_contiguous)
			{
				return *(this->_data.cbegin());
			}

			else
			{
				return this->_data;
			}
		}

		[[nodiscard]] constexpr reference operator*() noexcept
		requires (
			tensor_dense_fixed::_rank == 0 &&
			tensor_dense_fixed::_type == tensor_type_value::main)
		{
			if constexpr (tensor_dense_fixed::_contiguous)
			{
				return *(this->_data.begin());
			}

			else
			{
				return this->_data;
			}
		}

		template < typename MatrixType >
		requires (
			std::same_as < typename MatrixType::value_type, tensor_dense_fixed::value_type > &&
			std::same_as <
				MatrixType,
				algebra::tensor_dense_fixed <
					typename MatrixType::value_type,
					typename MatrixType::fixed_dimensionalities,
					tensor_contiguity < MatrixType::contiguous() >,
					typename MatrixType::memory_order_indices,
					tensor_type < MatrixType::type() > > > &&
			MatrixType::dimensionalities() == tensor_dense_fixed::dimensionalities())
		[[nodiscard]] constexpr auto operator+(const MatrixType &addend) const
		requires (algebra::concepts::addable_assignable < Type >)
		{
			const auto converter =
				[this] () -> auto
				{
					if constexpr (tensor_dense_fixed::_type == tensor_type_value::sub)
					{
						return this->convert_to_main();
					}

					else
					{
						return *this;
					}
				};

			main_type result = converter();

			result += addend;

			return result;
		}

		template < typename MatrixType >
		requires (
			std::same_as < typename MatrixType::value_type, tensor_dense_fixed::value_type > &&
			std::same_as <
				MatrixType,
				algebra::tensor_dense_fixed <
					typename MatrixType::value_type,
					typename MatrixType::fixed_dimensionalities,
					tensor_contiguity < MatrixType::contiguous() >,
					typename MatrixType::memory_order_indices,
					tensor_type < MatrixType::type() > > > &&
			MatrixType::dimensionalities() == tensor_dense_fixed::dimensionalities())
		[[nodiscard]] constexpr auto operator-(const MatrixType &subtrahend) const
		requires (algebra::concepts::subtractable_assignable < Type >)
		{
			const auto converter =
				[this] () -> auto
				{
					if constexpr (tensor_dense_fixed::_type == tensor_type_value::sub)
					{
						return this->convert_to_main();
					}

					else
					{
						return *this;
					}
				};

			main_type result = converter();

			result -= subtrahend;

			return result;
		}

		template < typename MatrixType >
		requires (
			std::same_as < typename MatrixType::value_type, tensor_dense_fixed::value_type > &&
			MatrixType::rank() == 2 &&
			MatrixType::dimensionalities()[0] == tensor_dense_fixed::dimensionalities()[1] &&
			std::same_as <
				MatrixType,
				algebra::tensor_dense_fixed <
					typename MatrixType::value_type,
					typename MatrixType::fixed_dimensionalities,
					tensor_contiguity < MatrixType::contiguous() >,
					typename MatrixType::memory_order_indices,
					tensor_type < MatrixType::type() > > >)
		[[nodiscard]] constexpr auto operator*(const MatrixType &multiplicand) const
		requires (
			tensor_dense_fixed::_rank == 2 &&
			algebra::concepts::multiplicable < Type >)
		{
			// TODO: Stop using naive solution.
			algebra::tensor_dense_fixed <
				typename std::remove_const < Type > ::type,
				tensor_fixed_dimensionalities <
					tensor_dense_fixed::_dimensionalities[0],
					MatrixType::dimensionalities()[1] >,
				Contiguous,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::main > > result{};

			for (size_t i = 0; i < tensor_dense_fixed::_dimensionalities[0]; i++)
			{
				for (size_t j = 0; j < MatrixType::dimensionalities()[1]; j++)
				{
					auto &cell = result(i, j);

					for (size_t k = 0; k < tensor_dense_fixed::_dimensionalities[1]; k++)
					{
						cell += (*this)(i, k) * multiplicand(k, j);
					}
				}
			}

			return result;
		}

		template < typename MatrixType >
		requires (
			std::same_as < typename MatrixType::value_type, tensor_dense_fixed::value_type > &&
			std::same_as <
				MatrixType,
				algebra::tensor_dense_fixed <
					typename MatrixType::value_type,
					typename MatrixType::fixed_dimensionalities,
					tensor_contiguity < MatrixType::contiguous() >,
					typename MatrixType::memory_order_indices,
					tensor_type < MatrixType::type() > > > &&
			MatrixType::dimensionalities() == tensor_dense_fixed::dimensionalities())
		constexpr tensor_dense_fixed &operator+=(const MatrixType &addend)
		requires (algebra::concepts::addable_assignable < Type >)
		{
			tensor_dense_fixed::addition_assignment(*this, addend);

			return *this;
		}

		template < typename MatrixType >
		requires (
			std::same_as < typename MatrixType::value_type, tensor_dense_fixed::value_type > &&
			std::same_as <
				MatrixType,
				algebra::tensor_dense_fixed <
					typename MatrixType::value_type,
					typename MatrixType::fixed_dimensionalities,
					tensor_contiguity < MatrixType::contiguous() >,
					typename MatrixType::memory_order_indices,
					tensor_type < MatrixType::type() > > > &&
			MatrixType::dimensionalities() == tensor_dense_fixed::dimensionalities())
		constexpr tensor_dense_fixed &operator-=(const MatrixType &subtrahend)
		requires (algebra::concepts::subtractable_assignable < Type >)
		{
			tensor_dense_fixed::subtraction_assignment(*this, subtrahend);

			return *this;
		}

		constexpr tensor_dense_fixed &operator*=(const Type &multiplicand)
		requires (algebra::concepts::multiplicable_assignable < Type >)
		{
			constexpr auto binaryOperator =
				[] < typename ElementType > (ElementType &thisValue, const Type &otherValue) -> void
				{
					if constexpr (tensor_dense_fixed::_type == tensor_type_value::sub)
					{
						if (thisValue.has_value())
						{
							thisValue.value().get() *= otherValue;
						}
					}

					else
					{
						thisValue *= otherValue;
					}
				};

			if constexpr (tensor_dense_fixed::_contiguous)
			{
				for (auto &value : this->_data)
				{
					binaryOperator(value, multiplicand);
				}
			}

			else if constexpr (tensor_dense_fixed::_rank == 0)
			{
				binaryOperator(this->_data, multiplicand);
			}

			else
			{
				tensor_dense_fixed::
					template binary_operator_for_non_contiguous_tensor < 0 > (this->_data, binaryOperator, multiplicand);
			}

			return *this;
		}

		constexpr tensor_dense_fixed &operator/=(const Type &divisor)
		requires (algebra::concepts::divisable_assignable < Type >)
		{
			constexpr auto binaryOperator =
				[] < typename ElementType > (ElementType &thisValue, const Type &otherValue) -> void
				{
					if constexpr (tensor_dense_fixed::_type == tensor_type_value::sub)
					{
						if (thisValue.has_value())
						{
							thisValue.value().get() /= otherValue;
						}
					}

					else
					{
						thisValue /= otherValue;
					}
				};

			if constexpr (std::integral < Type >)
			{
				if (divisor == Type{})
				{
					throw std::domain_error("\"divisor\" is zero.");
				}
			}

			if constexpr (tensor_dense_fixed::_contiguous)
			{
				for (auto &value : this->_data)
				{
					binaryOperator(value, divisor);
				}
			}

			else if constexpr (tensor_dense_fixed::_rank == 0)
			{
				binaryOperator(this->_data, divisor);
			}

			else
			{
				tensor_dense_fixed::
					template binary_operator_for_non_contiguous_tensor < 0 > (this->_data, binaryOperator, divisor);
			}

			return *this;
		}

		template < typename ... Types >
		requires (sizeof...(Types) == tensor_dense_fixed::_rank && (std::convertible_to < Types, size_t > && ...))
		[[nodiscard]] constexpr const_reference operator()(Types && ... args) const
		{
			return this->const_parentheses_operator(std::forward_as_tuple(args...));
		}

		template < typename ... Types >
		requires (sizeof...(Types) == tensor_dense_fixed::_rank && (std::convertible_to < Types, size_t > && ...))
		[[nodiscard]] constexpr reference operator()(Types && ... args)
		{
			return this->parentheses_operator(std::forward_as_tuple(args...));
		}

		template < typename ... Types >
		requires (sizeof...(Types) == tensor_dense_fixed::_rank && (std::convertible_to < Types, size_t > && ...))
		[[nodiscard]] constexpr const_reference operator()(const std::tuple < Types ... > &args) const
		{
			return this->const_parentheses_operator(args);
		}

		template < typename ... Types >
		requires (sizeof...(Types) == tensor_dense_fixed::_rank && (std::convertible_to < Types, size_t > && ...))
		[[nodiscard]] constexpr reference operator()(const std::tuple < Types ... > &args)
		{
			return this->parentheses_operator(args);
		}

		[[nodiscard]] constexpr const_reference operator()(const std::array < size_t, tensor_dense_fixed::_rank > &args) const
		{
			return this->const_parentheses_operator(std::tuple_cat(args));
		}

		[[nodiscard]] constexpr reference operator()(const std::array < size_t, tensor_dense_fixed::_rank > &args)
		{
			return this->parentheses_operator(std::tuple_cat(args));
		}

		constexpr void fill(const Type &value)
		{
			if constexpr (tensor_dense_fixed::_contiguous)
			{
				if constexpr (tensor_dense_fixed::_type == tensor_type_value::sub)
				{
					for (auto &data : this->_data)
					{
						if (data.has_value())
						{
							data.value().get() = value;
						}
					}
				}

				else
				{
					this->_data.fill(value);
				}
			}

			else if constexpr (tensor_dense_fixed::_rank == 0)
			{
				if constexpr (tensor_dense_fixed::_type == tensor_type_value::sub)
				{
					if (this->_data.has_value())
					{
						this->_data.value().get() = value;
					}
				}

				else
				{
					this->_data = value;
				}
			}

			else
			{
				tensor_dense_fixed::template fill_non_contiguous_tensor < 0 > (this->_data, value);
			}
		}

		[[nodiscard]] constexpr auto transpose() const
		requires (tensor_dense_fixed::_rank == 2)
		{
			// TODO: Stop using naive solution.
			algebra::tensor_dense_fixed <
				Type,
				tensor_fixed_dimensionalities <
					tensor_dense_fixed::_dimensionalities[1],
					tensor_dense_fixed::_dimensionalities[0] >,
				Contiguous,
				MemoryOrderIndices,
				TensorType > result{};

			for (size_t i = 0; i < tensor_dense_fixed::_dimensionalities[1]; i++)
			{
				for (size_t j = 0; j < tensor_dense_fixed::_dimensionalities[0]; j++)
				{
					result(i, j) = (*this)(j, i);
				}
			}

			return result;
		}

		[[nodiscard]] constexpr auto conjugate_transpose() const
		requires (tensor_dense_fixed::_rank == 2)
		{
			if constexpr (xablau::algebra::concepts::complex < Type >)
			{
				// TODO: Stop using naive solution.
				algebra::tensor_dense_fixed <
					Type,
					tensor_fixed_dimensionalities <
						tensor_dense_fixed::_dimensionalities[1],
						tensor_dense_fixed::_dimensionalities[0] >,
					Contiguous,
					MemoryOrderIndices,
					TensorType > result{};

				for (size_t i = 0; i < tensor_dense_fixed::_dimensionalities[1]; i++)
				{
					for (size_t j = 0; j < tensor_dense_fixed::_dimensionalities[0]; j++)
					{
						if constexpr (xablau::algebra::concepts::xablau_complex < Type >)
						{
							result(i, j) = xablau::algebra::conj((*this)(j, i));
						}

						else
						{
							result(i, j) = std::conj((*this)(j, i));
						}
					}
				}

				return result;
			}

			else
			{
				return this->transpose();
			}
		}

		friend std::ostream &operator<<(std::ostream &stream, const tensor_dense_fixed &tensor)
		{
			constexpr auto &dimensionalities = tensor_dense_fixed::dimensionalities();

			stream << "[";

			if constexpr (tensor_dense_fixed::_rank > 0)
			{
				size_t i{};

				for (i = 0; i < dimensionalities.size() - 1; i++)
				{
					stream << dimensionalities[i] << ", ";
				}

				stream << dimensionalities[i];
			}

			stream << "] ";

			std::array < size_t, tensor_dense_fixed::_rank > indices{};

			const auto incrementIndex =
				[&dimensionalities, &indices] () -> void
				{
					auto iteratorIndex = indices.rbegin();
					auto iteratorDimensionality = dimensionalities.crbegin();

					while (iteratorIndex != indices.rend() && iteratorDimensionality != dimensionalities.crend())
					{
						(*iteratorIndex)++;

						if (*iteratorIndex >= *iteratorDimensionality)
						{
							*iteratorIndex = 0;
						}

						else
						{
							return;
						}

						++iteratorIndex;
						++iteratorDimensionality;
					}
				};

			const auto printNDimensions =
				[&] (const size_t leastness, const bool printCommaInTheEnd, auto &&printNDimensions) -> void
				{
					if (leastness == indices.size())
					{
						return;
					}

					const bool printElement = (leastness == indices.size() - 1);
					const auto currentDimensionSize = *(dimensionalities.cbegin() + leastness);

					stream << "( ";

					for (size_t i = 0; i < currentDimensionSize - 1; i++)
					{
						printNDimensions(leastness + 1, true, printNDimensions);

						if (printElement)
						{
							stream << tensor(indices) << ", ";

							incrementIndex();
						}
					}

					printNDimensions(leastness + 1, false, printNDimensions);

					if (printElement)
					{
						stream << tensor(indices);

						incrementIndex();
					}

					stream << " )";

					if (printCommaInTheEnd)
					{
						stream << ", ";
					}
				};

			if constexpr (tensor_dense_fixed::_rank == 0)
			{
				stream << "( " << *tensor << " )";
			}

			else
			{
				printNDimensions(0, false, printNDimensions);
			}

			return stream;
		}

		constexpr tensor_dense_fixed &operator=(const tensor_dense_fixed &) = default;

		constexpr tensor_dense_fixed &operator=(tensor_dense_fixed &&) noexcept = default;

		constexpr tensor_dense_fixed() noexcept
		requires (tensor_dense_fixed::_type == tensor_type_value::main)
		{
		}

		constexpr tensor_dense_fixed(const tensor_dense_fixed &) = default;

		constexpr tensor_dense_fixed(tensor_dense_fixed &&) noexcept = default;
	};
}