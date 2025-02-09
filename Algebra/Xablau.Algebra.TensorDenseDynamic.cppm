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

export module xablau.algebra:tensor_dense_dynamic;
export import :complex_functions;
export import :fundamental_concepts;
export import :fundamental_definitions;
export import :fundamental_functions;
export import :standard_functions;
export import :tensor_configurations;
export import :utils;

export import std;

export namespace xablau::algebra
{
	template <
		typename Type,
		concepts::tensor_rank Rank,
		concepts::tensor_memory_order_indices MemoryOrderIndices = tensor_memory_order_indices <>,
		concepts::tensor_type TensorType = tensor_type < tensor_type_value::main > >
	requires (
		concepts::valid_tensor_dynamic_memory_order_indices < Rank, MemoryOrderIndices > &&
		concepts::valid_tensor_type < Type, TensorType >)
	class tensor_dense_dynamic final
	{
	private:
		using _aux_reference =
			typename std::vector <
				typename std::remove_reference <
					typename std::remove_const <
						Type > ::type > ::type > ::reference;

	public:
		template <
			typename OtherType,
			concepts::tensor_fixed_dimensionalities OtherFixedDimensionalities,
			concepts::tensor_memory_order_indices OtherMemoryOrderIndices,
			concepts::tensor_type OtherTensorType >
		requires (
			concepts::valid_tensor_fixed_memory_order_indices < OtherFixedDimensionalities, OtherMemoryOrderIndices > &&
			concepts::valid_tensor_type < OtherType, OtherTensorType >)
		friend class tensor_dense_fixed;

		template <
			typename OtherType,
			concepts::tensor_rank OtherRank,
			concepts::tensor_memory_order_indices OtherMemoryOrderIndices,
			concepts::tensor_type OtherTensorType >
		requires (
			concepts::valid_tensor_dynamic_memory_order_indices < OtherRank, OtherMemoryOrderIndices > &&
			concepts::valid_tensor_type < OtherType, OtherTensorType >)
		friend class tensor_dense_dynamic;

		using value_type = Type;

		using size_type = size_t;

		using const_reference =
			typename std::vector <
				typename std::remove_reference <
					typename std::remove_const <
						Type > ::type > ::type > ::const_reference;

		using reference =
			typename std::conditional <
				std::is_const < Type > ::value,
				const_reference,
				_aux_reference > ::type;

		using memory_order_indices = MemoryOrderIndices;

		using main_type =
			tensor_dense_dynamic <
				typename std::remove_const < Type > ::type,
				Rank,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::main > >;

		template < concepts::tensor_memory_order_indices OtherMemoryOrderIndices >
		using tensor_with_memory_order_indices =
			tensor_dense_dynamic <
				Type,
				Rank,
				OtherMemoryOrderIndices,
				TensorType >;

	private:
		static const Type _null_value;

		static constexpr size_t _rank = Rank::rank();

		static constexpr bool _fixed = false;

		static constexpr bool _sparse = false;

		static constexpr tensor_type_value _type = TensorType::type();

		std::array < size_t, tensor_dense_dynamic::_rank > _dimensionalities{};

		[[nodiscard]] static consteval auto create_dimensionality_mapping() noexcept
		{
			std::array < size_t, tensor_dense_dynamic::_rank > dimensionalityMapping{};

			if constexpr (MemoryOrderIndices::size() == 0)
			{
				if constexpr (tensor_dense_dynamic::_rank != 0)
				{
					for (size_t i = 0; i < tensor_dense_dynamic::_rank; i++)
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

		static constexpr std::array < size_t, tensor_dense_dynamic::_rank > _dimensionality_mapping =
			tensor_dense_dynamic::create_dimensionality_mapping();

		[[nodiscard]] static consteval auto create_dimensionalities_by_memory_orders()
		{
			std::array < size_t, tensor_dense_dynamic::_rank > dimensionalitiesByMemoryOrders{};

			for (size_t i = 0; i < tensor_dense_dynamic::_rank; i++)
			{
				dimensionalitiesByMemoryOrders[i] =
					std::ranges::find(tensor_dense_dynamic::_dimensionality_mapping, i) - tensor_dense_dynamic::_dimensionality_mapping.cbegin();
			}

			return dimensionalitiesByMemoryOrders;
		}

		static constexpr std::array < size_t, tensor_dense_dynamic::_rank > _dimensionalities_by_memory_orders =
			tensor_dense_dynamic::create_dimensionalities_by_memory_orders();

		size_t _size{};

		template < size_t Index >
		requires (Index < tensor_dense_dynamic::_rank)
		[[nodiscard]] static consteval auto dimensionality_mapping() noexcept
		{
			return tensor_dense_dynamic::_dimensionality_mapping[Index];
		}

		template < size_t Index >
		requires (Index < tensor_dense_dynamic::_rank)
		[[nodiscard]] static consteval auto dimensionality_of_order() noexcept
		{
			return tensor_dense_dynamic::_dimensionalities_by_memory_orders[Index];
		}

		[[nodiscard]] static consteval auto type_deduction() noexcept
		{
			if constexpr (tensor_dense_dynamic::_type == tensor_type_value::sub)
			{
				return std::vector < std::optional < std::reference_wrapper < Type > > > {};
			}

			else
			{
				return std::vector < Type > {};
			}
		}

		decltype(tensor_dense_dynamic::template type_deduction()) _data{};

		template < size_t DimensionalityIndex >
		requires (DimensionalityIndex < tensor_dense_dynamic::_rank)
		[[nodiscard]] constexpr size_t offset() const noexcept
		{
			if constexpr (tensor_dense_dynamic::_rank == 0)
			{
				return size_t{1};
			}

			else
			{
				size_t offset{1};
				constexpr size_t dimensionalityOrder = tensor_dense_dynamic::_dimensionality_mapping[DimensionalityIndex];

				for (size_t i = 0; i < tensor_dense_dynamic::_rank; i++)
				{
					if (dimensionalityOrder < tensor_dense_dynamic::_dimensionality_mapping[i])
					{
						offset *= this->_dimensionalities[i];
					}
				}

				return offset;
			}
		}

		template < size_t TupleIndex, size_t TupleLimit, typename TupleDataType >
		requires (
			TupleIndex <= TupleLimit &&
			TupleIndex <= std::tuple_size < TupleDataType > ::value &&
			TupleLimit <= std::tuple_size < TupleDataType > ::value)
		[[nodiscard]] constexpr size_t offset(
			const size_t currentIndex,
			const TupleDataType &tuple) const
		{
			if constexpr (TupleIndex == TupleLimit)
			{
				return currentIndex;
			}

			else
			{
				const size_t offset = this->template offset < TupleIndex > ();

				return
					this->template offset < TupleIndex + 1, TupleLimit > (
						currentIndex + static_cast < size_t > (std::get < TupleIndex > (tuple)) * offset,
						tuple);
			}
		}

		template < size_t TupleIndex, size_t TupleLimit >
		[[nodiscard]] constexpr auto indexing(auto begin, const auto &tuple) const
		{
			const auto offset = this->template offset < TupleIndex, TupleLimit > (0, tuple);

			auto _begin = begin + offset;

			if (_begin >= this->_data.end())
			{
				throw std::out_of_range(""); // TODO: Create message.
			}

			return _begin;
		}

		template < size_t TupleIndex, typename TupleDataType >
		[[nodiscard]] static constexpr size_t calculate_size_and_check_dimensionalities(
			const size_t newSize,
			const TupleDataType &newDimensionalitiesTuple,
			std::array < size_t, tensor_dense_dynamic::_rank > &newDimensionalitiesArray)
		{
			if constexpr (TupleIndex == std::tuple_size < TupleDataType > ())
			{
				return newSize;
			}

			else
			{
				const auto dimensionality = static_cast < size_t > (std::get < TupleIndex > (newDimensionalitiesTuple));

				if (dimensionality == 0)
				{
					throw std::invalid_argument(""); // TODO: Create message.
				}

				newDimensionalitiesArray[TupleIndex] = dimensionality;

				return tensor_dense_dynamic::template calculate_size_and_check_dimensionalities < TupleIndex + 1 > (
					newSize * dimensionality,
					newDimensionalitiesTuple,
					newDimensionalitiesArray);
			}
		}

		template < size_t OrderIndex >
		constexpr void resize_loop(
			auto &beginOrigin,
			auto &beginDestiny,
			const std::array < size_t, tensor_dense_dynamic::_rank > &newDimensionalities,
			const std::array < size_t, tensor_dense_dynamic::_rank > &exclusiveProductOrigin,
			const std::array < size_t, tensor_dense_dynamic::_rank > &exclusiveProductDestiny) const
		{
			const auto thisDimensionality =
				this->_dimensionalities[tensor_dense_dynamic::dimensionality_of_order < OrderIndex > ()];

			const auto newDimensionality =
				newDimensionalities[tensor_dense_dynamic::dimensionality_of_order < OrderIndex > ()];

			if constexpr (OrderIndex + 1 == tensor_dense_dynamic::_rank)
			{
				const size_t elementsToCopy = std::min(thisDimensionality, newDimensionality);

				std::copy(beginOrigin, beginOrigin + elementsToCopy, beginDestiny);

				beginOrigin += elementsToCopy;
				beginDestiny += elementsToCopy;
			}

			else
			{
				constexpr size_t mappedDimensionality = tensor_dense_dynamic::dimensionality_of_order < OrderIndex + 1 > ();
				const size_t limit = std::min(thisDimensionality, newDimensionality);

				for (size_t i = 0; i < limit; i++)
				{
					this->template resize_loop < OrderIndex + 1 > (
						beginOrigin,
						beginDestiny,
						newDimensionalities,
						exclusiveProductOrigin,
						exclusiveProductDestiny);

					const auto nextThisDimensionality = this->_dimensionalities[mappedDimensionality];
					const auto nextNewDimensionality = newDimensionalities[mappedDimensionality];

					if (nextThisDimensionality > nextNewDimensionality)
					{
						beginOrigin +=
							exclusiveProductOrigin[mappedDimensionality] * (nextThisDimensionality - nextNewDimensionality);
					}

					else if (nextThisDimensionality < nextNewDimensionality)
					{
						beginDestiny +=
							exclusiveProductDestiny[mappedDimensionality] * (nextNewDimensionality - nextThisDimensionality);
					}
				}
			}
		}

		constexpr void resize_start(
			const size_t newSize,
			const std::array < size_t, tensor_dense_dynamic::_rank > &newDimensionalities,
			const Type &value)
		{
			decltype(this->_data) newData{};

			if constexpr (tensor_dense_dynamic::_type == tensor_type_value::main)
			{
				newData.resize(newSize, value);
			}

			else
			{
				// TODO: Move this to somewhere else.
				newData.resize(newSize, std::nullopt);
			}

			auto iterator1 = this->_data.cbegin();
			auto iterator2 = newData.begin();

			std::array < size_t, tensor_dense_dynamic::_rank > reorderedDimensionalitiesOrigin{};
			std::array < size_t, tensor_dense_dynamic::_rank > reorderedDimensionalitiesDestiny{};

			for (size_t i = 0; i < tensor_dense_dynamic::_rank; i++)
			{
				reorderedDimensionalitiesOrigin[i] = this->_dimensionalities[tensor_dense_dynamic::_dimensionalities_by_memory_orders[i]];
				reorderedDimensionalitiesDestiny[i] = newDimensionalities[tensor_dense_dynamic::_dimensionalities_by_memory_orders[i]];
			}

			std::array < size_t, tensor_dense_dynamic::_rank > exclusiveProductOrigin{};
			std::array < size_t, tensor_dense_dynamic::_rank > exclusiveProductDestiny{};

			std::exclusive_scan(
				reorderedDimensionalitiesOrigin.crbegin(),
				reorderedDimensionalitiesOrigin.crend(),
				exclusiveProductOrigin.rbegin(),
				size_t{1},
				std::multiplies < size_t > ());

			std::exclusive_scan(
				reorderedDimensionalitiesDestiny.crbegin(),
				reorderedDimensionalitiesDestiny.crend(),
				exclusiveProductDestiny.rbegin(),
				size_t{1},
				std::multiplies < size_t > ());

			std::array < size_t, tensor_dense_dynamic::_rank > reorderedExclusiveProductOrigin{};
			std::array < size_t, tensor_dense_dynamic::_rank > reorderedExclusiveProductDestiny{};

			for (size_t i = 0; i < tensor_dense_dynamic::_rank; i++)
			{
				reorderedExclusiveProductOrigin[i] = exclusiveProductOrigin[tensor_dense_dynamic::_dimensionality_mapping[i]];
				reorderedExclusiveProductDestiny[i] = exclusiveProductDestiny[tensor_dense_dynamic::_dimensionality_mapping[i]];
			}

			this->template resize_loop < 0 > (
				iterator1,
				iterator2,
				newDimensionalities,
				reorderedExclusiveProductOrigin,
				reorderedExclusiveProductDestiny);

			this->_data.swap(newData);
		}

		template < typename TupleType >
		constexpr void resize_with_initialization(const TupleType &newDimensionalitiesTuple, const Type &value)
		{
			std::array < size_t, tensor_dense_dynamic::_rank > newDimensionalitiesArray{};

			const size_t newSize =
				tensor_dense_dynamic::template calculate_size_and_check_dimensionalities < 0 > (
					1,
					newDimensionalitiesTuple,
					newDimensionalitiesArray);

			if (this->_dimensionalities == newDimensionalitiesArray)
			{
				return;
			}

			this->resize_start(newSize, newDimensionalitiesArray, value);
			this->_dimensionalities.swap(newDimensionalitiesArray);
			this->_size = newSize;
		}

		template < typename ... DimensionalityTypes >
		requires (std::convertible_to < DimensionalityTypes, size_t > && ...)
		constexpr void resize_with_initialization(const Type &value, DimensionalityTypes && ... args)
		{
			const auto newDimensionalitiesTuple = std::forward_as_tuple(args...);

			this->resize_with_initialization(newDimensionalitiesTuple, value);
		}

		template < typename ... DimensionalityTypesAndInitializationValue, size_t ... DimensionalityIndices >
		constexpr void resize_with_initialization(
			std::tuple < DimensionalityTypesAndInitializationValue... > dimensionalityTypesAndInitializationValue,
			std::index_sequence < DimensionalityIndices... >)
		{
			constexpr auto initializationValueIndex = sizeof...(DimensionalityTypesAndInitializationValue) - 1;

			this->resize_with_initialization(
				std::get < initializationValueIndex > (dimensionalityTypesAndInitializationValue),
				std::get < DimensionalityIndices > (dimensionalityTypesAndInitializationValue)...);
		}

		template < typename TupleDataType >
		requires (std::tuple_size < TupleDataType > ::value == tensor_dense_dynamic::_rank)
		[[nodiscard]] constexpr const_reference const_parentheses_operator(const TupleDataType &tuple) const
		{
			const auto element =
				this->template indexing < 0, std::tuple_size < TupleDataType > ::value > (
					this->_data.cbegin(),
					tuple);

			if constexpr (tensor_dense_dynamic::_type == tensor_type_value::sub)
			{
				if (element->has_value())
				{
					return element->value().get();
				}

				else
				{
					return tensor_dense_dynamic::_null_value;
				}
			}

			else
			{
				return *element;
			}
		}

		template < typename TupleDataType >
		requires (std::tuple_size < TupleDataType > ::value == tensor_dense_dynamic::_rank)
		[[nodiscard]] constexpr reference parentheses_operator(const TupleDataType &tuple)
		{
			const auto element =
				this->template indexing < 0, std::tuple_size < TupleDataType > ::value > (
					this->_data.begin(),
					tuple);

			if constexpr (tensor_dense_dynamic::_type == tensor_type_value::sub)
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

		template < size_t TupleIndex >
		requires (TupleIndex < tensor_dense_dynamic::_rank)
		constexpr void kernel(
			auto iteratorData,
			auto iteratorKernelData,
			const auto &kernel,
			const auto &tuple,
			const std::array < size_t, tensor_dense_dynamic::_rank > &kernelSize,
			const std::array < size_t, tensor_dense_dynamic::_rank > &dimensionalities) const
		{
			size_t dataIndex{};
			size_t kernelIndex{};
			const auto boundary = dimensionalities[TupleIndex];
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

			const auto offsetData = this->offset < TupleIndex > ();
			const auto offsetKernel = kernel.offset < TupleIndex > ();

			iteratorData += dataIndex * offsetData;
			iteratorKernelData += kernelIndex * offsetKernel;

			while (dataIndex <= center + offset && dataIndex < boundary)
			{
				if constexpr (TupleIndex + 1 < tensor_dense_dynamic::_rank)
				{
					this->kernel < TupleIndex + 1 > (
						iteratorData,
						iteratorKernelData,
						kernel,
						tuple,
						kernelSize,
						dimensionalities);
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
		constexpr void kernel(
			auto &data,
			auto &kernel,
			const std::array < size_t, tensor_dense_dynamic::_rank > &dimensionalities,
			const auto &tuple,
			const std::array < size_t, tensor_dense_dynamic::_rank > &kernelSize) const
		{
			if constexpr (DynamicKernel)
			{
				if (!utils::valid_kernel_size(kernelSize))
				{
					throw std::runtime_error(""); // TODO: Create message.
				}

				kernel.resize(kernelSize);
			}

			this->template kernel < 0 > (
				data.begin(),
				kernel._data.begin(),
				kernel,
				tuple,
				kernelSize,
				dimensionalities);
		}

		template < bool DynamicKernel, typename ... DimensionalityTypes >
		requires (std::convertible_to < DimensionalityTypes, size_t > && ...)
		constexpr void kernel(
			auto &data,
			auto &kernel,
			const std::array < size_t, tensor_dense_dynamic::_rank > &dimensionalities,
			const std::array < size_t, tensor_dense_dynamic::_rank > &kernelSize,
			DimensionalityTypes && ... args) const
		{
			const auto kernelCenter = std::forward_as_tuple(args...);

			this->kernel < DynamicKernel > (data, kernel, dimensionalities, kernelCenter, kernelSize);
		}

		template <
			bool DynamicKernel,
			typename ... KernelCenterCoordinatesAndKernelSize,
			size_t ... DimensionalityIndices >
		constexpr void kernel(
			auto &data,
			auto &kernel,
			const std::array < size_t, tensor_dense_dynamic::_rank > &dimensionalities,
			std::tuple < KernelCenterCoordinatesAndKernelSize... > kernelCenterCoordinatesAndKernelSize,
			std::index_sequence < DimensionalityIndices... >) const
		{
			constexpr auto kernelSize = sizeof...(KernelCenterCoordinatesAndKernelSize) - 1;

			this->kernel < DynamicKernel > (
				data,
				kernel,
				dimensionalities,
				std::get < DimensionalityIndices > (kernelCenterCoordinatesAndKernelSize)...,
				std::get < kernelSize > (kernelCenterCoordinatesAndKernelSize));
		}

		constexpr tensor_dense_dynamic() noexcept
		requires (tensor_dense_dynamic::_type == tensor_type_value::sub)
		{
			if constexpr (tensor_dense_dynamic::_rank == 0)
			{
				this->_size = 1;
			}
		}

		template < typename ... Types >
		requires (
			sizeof...(Types) == tensor_dense_dynamic::_rank &&
			(std::convertible_to < Types, size_t > && ...) &&
			sizeof...(Types) != 0)
		explicit constexpr tensor_dense_dynamic(Types && ... args)
		requires (tensor_dense_dynamic::_type == tensor_type_value::sub)
		{
			this->resize(args...);
		}

		explicit constexpr tensor_dense_dynamic(
			const std::array < size_t, tensor_dense_dynamic::_rank > &size,
			const Type &value = tensor_dense_dynamic::_null_value)
		requires (tensor_dense_dynamic::_type == tensor_type_value::sub)
		{
			this->resize(size, value);
		}

		void assert_square_matrix() const
		requires (tensor_dense_dynamic::_rank == 2)
		{
			if (this->dimensionalities()[0] != this->dimensionalities()[1])
			{
				// TODO: Improve this message.
				throw std::domain_error("The matrix is not square.");
			}
		}

		template < size_t UnitsInTheLastPlace >
		void assert_Hermitianess() const
		{
			for (size_t i = 0; i < this->rows(); i++)
			{
				for (size_t j = i; j < this->columns(); j++)
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
		static constexpr void addition_assignment(tensor_dense_dynamic &thisTensor, const MatrixType &addend)
		{
			std::array < size_t, tensor_dense_dynamic::_rank > indices{};

			for (size_t i = 0; i < thisTensor._size; i++)
			{
				if constexpr (tensor_dense_dynamic::_type == tensor_type_value::main)
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
		requires (std::same_as < typename MatrixType::memory_order_indices, memory_order_indices >)
		static constexpr void addition_assignment(tensor_dense_dynamic &thisTensor, const MatrixType &addend)
		{
			constexpr auto binaryOperator =
				[] < typename ThisElementType, typename OtherElementType > (
					ThisElementType &thisValue,
					const OtherElementType &otherValue) -> void
				{
					if constexpr (tensor_dense_dynamic::_type == tensor_type_value::main)
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

			for (size_t i = 0; i < thisTensor._size; i++)
			{
				binaryOperator(thisTensor._data[i], addend._data[i]);
			}
		}

		template < typename MatrixType >
		static constexpr void subtraction_assignment(tensor_dense_dynamic &thisTensor, const MatrixType &subtrahend)
		{
			std::array < size_t, tensor_dense_dynamic::_rank > indices{};

			for (size_t i = 0; i < thisTensor._size; i++)
			{
				if constexpr (tensor_dense_dynamic::_type == tensor_type_value::main)
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
		requires (std::same_as < typename MatrixType::memory_order_indices, memory_order_indices >)
		static constexpr void subtraction_assignment(tensor_dense_dynamic &thisTensor, const MatrixType &subtrahend)
		{
			constexpr auto binaryOperator =
				[] < typename ThisElementType, typename OtherElementType > (
					ThisElementType &thisValue,
					const OtherElementType &otherValue) -> void
				{
					if constexpr (tensor_dense_dynamic::_type == tensor_type_value::main)
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

			for (size_t i = 0; i < thisTensor._size; i++)
			{
				binaryOperator(thisTensor._data[i], subtrahend._data[i]);
			}
		}

	public:
		[[nodiscard]] static consteval size_t rank() noexcept
		{
			return tensor_dense_dynamic::_rank;
		}

		[[nodiscard]] static consteval bool fixed() noexcept
		{
			return tensor_dense_dynamic::_fixed;
		}

		[[nodiscard]] static consteval bool sparse() noexcept
		{
			return tensor_dense_dynamic::_sparse;
		}

		[[nodiscard]] static consteval tensor_type_value type() noexcept
		{
			return tensor_dense_dynamic::_type;
		}

		[[nodiscard]] constexpr size_t size() const noexcept
		{
			return this->_size;
		}

		[[nodiscard]] constexpr const std::array < size_t, tensor_dense_dynamic::_rank > &dimensionalities() const noexcept
		{
			return this->_dimensionalities;
		}

		[[nodiscard]] static consteval const std::array < size_t, tensor_dense_dynamic::_rank > &dimensionality_mapping() noexcept
		{
			return tensor_dense_dynamic::_dimensionality_mapping;
		}

		[[nodiscard]] static consteval const std::array < size_t, tensor_dense_dynamic::_rank > &dimensionalities_by_memory_orders() noexcept
		{
			return tensor_dense_dynamic::_dimensionalities_by_memory_orders;
		}

		[[nodiscard]] constexpr const auto &data() const noexcept
		requires (tensor_dense_dynamic::_type == tensor_type_value::main)
		{
			return this->_data;
		}

		template < concepts::tensor_fixed_dimensionalities KernelSize, typename ... Types >
		requires (
			concepts::valid_tensor_fixed_kernel_size < tensor_dense_dynamic::_rank, KernelSize > &&
			sizeof...(Types) == tensor_dense_dynamic::_rank &&
			(std::convertible_to < Types, size_t > && ...))
		[[nodiscard]] constexpr auto kernel(const std::tuple < Types ... > &kernelCenter) const
		requires (tensor_dense_dynamic::_rank != 0 && !std::same_as < Type, bool >)
		{
			tensor_dense_fixed <
				typename std::remove_reference < const_reference > ::type,
				KernelSize,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::sub > > _kernel{};

			this->kernel < false > (
				this->_data,
				_kernel,
				this->_dimensionalities,
				kernelCenter,
				KernelSize::array);

			return _kernel;
		}

		template < concepts::tensor_fixed_dimensionalities KernelSize, typename ... Types >
		requires (
			concepts::valid_tensor_fixed_kernel_size < tensor_dense_dynamic::_rank, KernelSize > &&
			sizeof...(Types) == tensor_dense_dynamic::_rank &&
			(std::convertible_to < Types, size_t > && ...))
		[[nodiscard]] constexpr auto kernel(const std::tuple < Types ... > &kernelCenter)
		requires (tensor_dense_dynamic::_rank != 0 && !std::same_as < Type, bool >)
		{
			tensor_dense_fixed <
				typename std::remove_reference < reference > ::type,
				KernelSize,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::sub > > _kernel{};

			this->kernel < false > (
				this->_data,
				_kernel,
				this->_dimensionalities,
				kernelCenter,
				KernelSize::array);

			return _kernel;
		}

		template < typename ... Types >
		requires (
			sizeof...(Types) == tensor_dense_dynamic::_rank &&
			(std::convertible_to < Types, size_t > && ...))
		[[nodiscard]] constexpr auto kernel(
			const std::tuple < Types ... > &kernelCenter,
			const std::array < size_t, tensor_dense_dynamic::_rank > &kernelSize) const
		requires (tensor_dense_dynamic::_rank != 0 && !std::same_as < Type, bool >)
		{
			tensor_dense_dynamic <
				typename std::remove_reference < const_reference > ::type,
				tensor_rank < tensor_dense_dynamic::_rank >,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::sub > > _kernel{};

			this->kernel < true > (
				this->_data,
				_kernel,
				this->_dimensionalities,
				kernelCenter,
				kernelSize);

			return _kernel;
		}

		template < typename ... Types >
		requires (
			sizeof...(Types) == tensor_dense_dynamic::_rank &&
			(std::convertible_to < Types, size_t > && ...))
		[[nodiscard]] constexpr auto kernel(
			const std::tuple < Types ... > &kernelCenter,
			const std::array < size_t, tensor_dense_dynamic::_rank > &kernelSize)
		requires (tensor_dense_dynamic::_rank != 0 && !std::same_as < Type, bool >)
		{
			tensor_dense_dynamic <
				typename std::remove_reference < reference > ::type,
				tensor_rank < tensor_dense_dynamic::_rank >,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::sub > > _kernel{};

			this->kernel < true > (
				this->_data,
				_kernel,
				this->_dimensionalities,
				kernelCenter,
				kernelSize);

			return _kernel;
		}

		template < concepts::tensor_fixed_dimensionalities KernelSize, typename ... KernelCenterCoordinates >
		requires (
			concepts::valid_tensor_fixed_kernel_size < tensor_dense_dynamic::_rank, KernelSize > &&
			sizeof...(KernelCenterCoordinates) == tensor_dense_dynamic::_rank &&
			(std::convertible_to < KernelCenterCoordinates, size_t > && ...))
		[[nodiscard]] constexpr auto kernel(KernelCenterCoordinates && ... args) const
		requires (tensor_dense_dynamic::_rank != 0 && !std::same_as < Type, bool >)
		{
			tensor_dense_fixed <
				typename std::remove_reference < const_reference > ::type,
				KernelSize,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::sub > > _kernel{};

			this->kernel < false > (
				this->_data,
				_kernel,
				this->_dimensionalities,
				std::forward_as_tuple(args...),
				KernelSize::array);

			return _kernel;
		}

		template < concepts::tensor_fixed_dimensionalities KernelSize, typename ... KernelCenterCoordinates >
		requires (
			concepts::valid_tensor_fixed_kernel_size < tensor_dense_dynamic::_rank, KernelSize > &&
			sizeof...(KernelCenterCoordinates) == tensor_dense_dynamic::_rank &&
			(std::convertible_to < KernelCenterCoordinates, size_t > && ...))
		[[nodiscard]] constexpr auto kernel(KernelCenterCoordinates && ... args)
		requires (tensor_dense_dynamic::_rank != 0 && !std::same_as < Type, bool >)
		{
			tensor_dense_fixed <
				typename std::remove_reference < reference > ::type,
				KernelSize,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::sub > > _kernel{};

			this->kernel < false > (
				this->_data,
				_kernel,
				this->_dimensionalities,
				std::forward_as_tuple(args...),
				KernelSize::array);

			return _kernel;
		}

		template < typename ... KernelCenterCoordinatesAndKernelSize >
		requires (sizeof...(KernelCenterCoordinatesAndKernelSize) == (tensor_dense_dynamic::_rank + 1))
		[[nodiscard]] constexpr auto kernel(KernelCenterCoordinatesAndKernelSize && ... args) const
		requires (tensor_dense_dynamic::_rank != 0 && !std::same_as < Type, bool >)
		{
			tensor_dense_dynamic <
				typename std::remove_reference < const_reference > ::type,
				tensor_rank < tensor_dense_dynamic::_rank >,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::sub > > _kernel{};

			this->kernel < true > (
				this->_data,
				_kernel,
				this->_dimensionalities,
				std::forward_as_tuple(args...),
				std::make_index_sequence < sizeof...(args) - 1 > {});

			return _kernel;
		}

		template < typename ... KernelCenterCoordinatesAndKernelSize >
		requires (sizeof...(KernelCenterCoordinatesAndKernelSize) == (tensor_dense_dynamic::_rank + 1))
		[[nodiscard]] constexpr auto kernel(KernelCenterCoordinatesAndKernelSize && ... args)
		requires (tensor_dense_dynamic::_rank != 0 && !std::same_as < Type, bool >)
		{
			tensor_dense_dynamic <
				typename std::remove_reference < reference > ::type,
				tensor_rank < tensor_dense_dynamic::_rank >,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::sub > > _kernel{};

			this->kernel < true > (
				this->_data,
				_kernel,
				this->_dimensionalities,
				std::forward_as_tuple(args...),
				std::make_index_sequence < sizeof...(args) - 1 > {});

			return _kernel;
		}

		template < concepts::tensor_fixed_dimensionalities KernelSize >
		requires (concepts::valid_tensor_fixed_kernel_size < tensor_dense_dynamic::_rank, KernelSize >)
		[[nodiscard]] constexpr auto kernel(const std::array < size_t, tensor_dense_dynamic::_rank > &kernelCenter) const
		requires (tensor_dense_dynamic::_rank != 0 && !std::same_as < Type, bool >)
		{
			return this->kernel < KernelSize > (std::tuple_cat(kernelCenter));
		}

		template < concepts::tensor_fixed_dimensionalities KernelSize >
		requires (concepts::valid_tensor_fixed_kernel_size < tensor_dense_dynamic::_rank, KernelSize >)
		[[nodiscard]] constexpr auto kernel(const std::array < size_t, tensor_dense_dynamic::_rank > &kernelCenter)
		requires (tensor_dense_dynamic::_rank != 0 && !std::same_as < Type, bool >)
		{
			return this->kernel < KernelSize > (std::tuple_cat(kernelCenter));
		}

		[[nodiscard]] constexpr auto kernel(
			const std::array < size_t, tensor_dense_dynamic::_rank > &kernelCenter,
			const std::array < size_t, tensor_dense_dynamic::_rank > &kernelSize) const
		requires (tensor_dense_dynamic::_rank != 0 && !std::same_as < Type, bool >)
		{
			return this->kernel(std::tuple_cat(kernelCenter), kernelSize);
		}

		[[nodiscard]] constexpr auto kernel(
			const std::array < size_t, tensor_dense_dynamic::_rank > &kernelCenter,
			const std::array < size_t, tensor_dense_dynamic::_rank > &kernelSize)
		requires (tensor_dense_dynamic::_rank != 0 && !std::same_as < Type, bool >)
		{
			return this->kernel(std::tuple_cat(kernelCenter), kernelSize);
		}

		[[nodiscard]] constexpr size_t rows() const
		requires (tensor_dense_dynamic::_rank == 2)
		{
			return this->_dimensionalities[0];
		}

		[[nodiscard]] constexpr size_t columns() const
		requires (tensor_dense_dynamic::_rank == 2)
		{
			return this->_dimensionalities[1];
		}

		[[nodiscard]] constexpr auto row(const size_t index) const
		requires (tensor_dense_dynamic::_rank == 2 && !std::same_as < Type, bool >)
		{
			if (this->_size == 0)
			{
				throw std::runtime_error(""); // TODO: Create message.
			}

			if (index >= this->rows())
			{
				throw std::runtime_error(""); // TODO: Create message.
			}

			tensor_dense_dynamic <
				typename std::remove_reference < const_reference > ::type,
				tensor_rank < tensor_dense_dynamic::_rank >,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::sub > > _kernel{};

			_kernel.resize(1, this->columns());

			// TODO: Stop using naive solution.
			for (size_t i = 0; i < this->columns(); i++)
			{
				_kernel._data[i].emplace((*this)(index, i));
			}

			return _kernel;
		}

		[[nodiscard]] constexpr auto row(const size_t index)
		requires (tensor_dense_dynamic::_rank == 2 && !std::same_as < Type, bool >)
		{
			if (this->_size == 0)
			{
				throw std::runtime_error(""); // TODO: Create message.
			}

			if (index >= this->rows())
			{
				throw std::runtime_error(""); // TODO: Create message.
			}

			tensor_dense_dynamic <
				typename std::remove_reference < reference > ::type,
				tensor_rank < tensor_dense_dynamic::_rank >,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::sub > > _kernel{};

			_kernel.resize(1, this->columns());

			// TODO: Stop using naive solution.
			for (size_t i = 0; i < this->columns(); i++)
			{
				_kernel._data[i].emplace((*this)(index, i));
			}

			return _kernel;
		}

		[[nodiscard]] constexpr auto column(const size_t index) const
		requires (tensor_dense_dynamic::_rank == 2 && !std::same_as < Type, bool >)
		{
			if (this->_size == 0)
			{
				throw std::runtime_error(""); // TODO: Create message.
			}

			if (index >= this->columns())
			{
				throw std::runtime_error(""); // TODO: Create message.
			}

			tensor_dense_dynamic <
				typename std::remove_reference < const_reference > ::type,
				tensor_rank < tensor_dense_dynamic::_rank >,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::sub > > _kernel{};

			_kernel.resize(this->rows(), 1);

			// TODO: Stop using naive solution.
			for (size_t i = 0; i < this->rows(); i++)
			{
				_kernel._data[i].emplace((*this)(i, index));
			}

			return _kernel;
		}

		[[nodiscard]] constexpr auto column(const size_t index)
		requires (tensor_dense_dynamic::_rank == 2 && !std::same_as < Type, bool >)
		{
			if (this->_size == 0)
			{
				throw std::runtime_error(""); // TODO: Create message.
			}

			if (index >= this->columns())
			{
				throw std::runtime_error(""); // TODO: Create message.
			}

			tensor_dense_dynamic <
				typename std::remove_reference < reference > ::type,
				tensor_rank < tensor_dense_dynamic::_rank >,
				MemoryOrderIndices,
				tensor_type < tensor_type_value::sub > > _kernel{};

			_kernel.resize(this->rows(), 1);

			// TODO: Stop using naive solution.
			for (size_t i = 0; i < this->rows(); i++)
			{
				_kernel._data[i].emplace((*this)(i, index));
			}

			return _kernel;
		}

		template < typename ... Types >
		requires (sizeof...(Types) == tensor_dense_dynamic::_rank && (std::convertible_to < Types, size_t > && ...))
		[[nodiscard]] constexpr bool has_reference_to_main(Types && ... args) const
		requires (tensor_dense_dynamic::_type == tensor_type_value::sub)
		{
			return this->has_reference_to_main(std::forward_as_tuple(args...));
		}

		template < typename ... Types >
		requires (sizeof...(Types) == tensor_dense_dynamic::_rank && (std::convertible_to < Types, size_t > && ...))
		[[nodiscard]] constexpr bool has_reference_to_main(const std::tuple < Types ... > &tuple) const
		requires (tensor_dense_dynamic::_type == tensor_type_value::sub)
		{
			return
				this->template indexing < 0, tensor_dense_dynamic::_rank > (
					this->_data.cbegin(), tuple)->has_value();
		}

		[[nodiscard]] constexpr bool has_reference_to_main(const std::array < size_t, tensor_dense_dynamic::_rank > &args) const
		requires (tensor_dense_dynamic::_type == tensor_type_value::sub)
		{
			return this->has_reference_to_main(std::tuple_cat(args));
		}

		[[nodiscard]] constexpr main_type convert_to_main() const
		requires (tensor_dense_dynamic::_type == tensor_type_value::sub)
		{
			main_type result(this->_dimensionalities);

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

			for (size_t i = 0; i < this->_size; i++)
			{
				binaryOperator(result._data[i], this->_data[i]);
			}

			return result;
		}

		[[nodiscard]] Type determinant() const
		requires (
			std::equality_comparable < Type > &&
			concepts::negatable_basic_assignable_arithmetic < Type > &&
			tensor_dense_dynamic::_rank == 2)
		{
			this->assert_square_matrix();

			auto determinant = functions::multiplication_identity < Type > ::invoke();
			const auto [swaps, LU, _] = utils::LUP_decomposition(*this);

			for (size_t i = 0; i < this->dimensionalities()[0]; i++)
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
			concepts::basic_assignable_arithmetic < Type > &&
			tensor_dense_dynamic::_rank == 2)
		{
			this->assert_square_matrix();

			auto [_, LU, P] = utils::LUP_decomposition(*this);

			return std::make_pair(std::move(LU), std::move(P));
		}

		[[nodiscard]] auto inverse() const
		requires (
			std::equality_comparable < Type > &&
			concepts::basic_assignable_arithmetic < Type > &&
			tensor_dense_dynamic::_rank == 2)
		{
			this->assert_square_matrix();

			tensor_dense_dynamic identity(this->_dimensionalities);

			for (size_t i = 0; i < this->_dimensionalities[0]; i++)
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
			tensor_dense_dynamic::_rank == 2)
		{
			return
				utils::QR <
					typename xablau::algebra::abs_type < Type > ::type,
					tensor_dense_dynamic > (*this);
		}

		template < bool CalculateEigenvectors, bool CheckHermitianess, size_t UnitsInTheLastPlace = 0 >
		[[nodiscard]] auto eigen_Hermitian() const
		requires (
			std::equality_comparable < Type > &&
			concepts::basic_assignable_arithmetic < Type > &&
			tensor_dense_dynamic::_rank == 2)
		{
			this->assert_square_matrix();

			if constexpr (CheckHermitianess)
			{
				this->assert_Hermitianess < UnitsInTheLastPlace > ();
			}

			return
				utils::eigen_Hermitian <
					CalculateEigenvectors,
					tensor_dense_dynamic,
					typename xablau::algebra::abs_type < Type > ::type,
					tensor_dense_dynamic > (*this);
		}

		[[nodiscard]] constexpr const_reference operator*() const noexcept
		requires (
			tensor_dense_dynamic::_rank == 0 &&
			tensor_dense_dynamic::_type == tensor_type_value::main)
		{
			return *(this->_data.cbegin());
		}

		[[nodiscard]] constexpr reference operator*() noexcept
		requires (
			tensor_dense_dynamic::_rank == 0 &&
			tensor_dense_dynamic::_type == tensor_type_value::main)
		{
			return *(this->_data.begin());
		}

		template < typename MatrixType >
		requires (
			std::same_as < typename MatrixType::value_type, tensor_dense_dynamic::value_type > &&
			typename MatrixType::rank() == tensor_dense_dynamic::_rank &&
			std::same_as <
				MatrixType,
				algebra::tensor_dense_dynamic <
					typename MatrixType::value_type,
					tensor_rank < MatrixType::rank() >,
					typename MatrixType::memory_order_indices,
					tensor_type < MatrixType::type() > > >)
		[[nodiscard]] constexpr auto operator+(const MatrixType &addend) const
		requires (algebra::concepts::addable_assignable < Type >)
		{
			if (addend.dimensionalities() != this->dimensionalities())
			{
				throw std::domain_error(""); // TODO: Create message.
			}

			const auto converter =
				[this] () -> auto
				{
					if constexpr (tensor_dense_dynamic::_type == tensor_type_value::sub)
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
			std::same_as < typename MatrixType::value_type, tensor_dense_dynamic::value_type > &&
			typename MatrixType::rank() == tensor_dense_dynamic::_rank &&
			std::same_as <
				MatrixType,
				algebra::tensor_dense_dynamic <
					typename MatrixType::value_type,
					tensor_rank < MatrixType::rank() >,
					typename MatrixType::memory_order_indices,
					tensor_type < MatrixType::type() > > >)
		[[nodiscard]] constexpr auto operator-(const MatrixType &subtrahend) const
		requires (algebra::concepts::subtractable_assignable < Type >)
		{
			if (subtrahend.dimensionalities() != this->dimensionalities())
			{
				throw std::domain_error(""); // TODO: Create message.
			}

			const auto converter =
				[this] () -> auto
				{
					if constexpr (tensor_dense_dynamic::_type == tensor_type_value::sub)
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
			std::same_as < typename MatrixType::value_type, tensor_dense_dynamic::value_type > &&
			MatrixType::rank() == 2 &&
			std::same_as <
				MatrixType,
				algebra::tensor_dense_dynamic <
					typename MatrixType::value_type,
					tensor_rank < MatrixType::rank() >,
					typename MatrixType::memory_order_indices,
					tensor_type < MatrixType::type() > > >)
		[[nodiscard]] constexpr auto operator*(const MatrixType &multiplicand) const
		requires (
			tensor_dense_dynamic::_rank == 2 &&
			algebra::concepts::multiplicable < Type >)
		{
				if (multiplicand.dimensionalities()[0] != this->dimensionalities()[1])
				{
					throw std::domain_error(""); // TODO: Create message.
				}

				// TODO: Stop using naive solution.
				main_type result(this->dimensionalities()[0], multiplicand.dimensionalities()[1]);

				for (size_t i = 0; i < this->_dimensionalities[0]; i++)
				{
					for (size_t j = 0; j < multiplicand.dimensionalities()[1]; j++)
					{
						auto &cell = result(i, j);

						for (size_t k = 0; k < this->_dimensionalities[1]; k++)
						{
							cell += (*this)(i, k) * multiplicand(k, j);
						}
					}
				}

				return result;
		}

		template < typename MatrixType >
		requires (
			std::same_as < typename MatrixType::value_type, tensor_dense_dynamic::value_type > &&
			typename MatrixType::rank() == tensor_dense_dynamic::_rank &&
			std::same_as <
				MatrixType,
				algebra::tensor_dense_dynamic <
					typename MatrixType::value_type,
					tensor_rank < MatrixType::rank() >,
					typename MatrixType::memory_order_indices,
					tensor_type < MatrixType::type() > > >)
		constexpr tensor_dense_dynamic &operator+=(const MatrixType &addend)
		requires (algebra::concepts::addable_assignable < Type >)
		{
			if (addend.dimensionalities() != this->dimensionalities())
			{
				throw std::domain_error(""); // TODO: Create message.
			}

			tensor_dense_dynamic::addition_assignment(*this, addend);

			return *this;
		}

		template < typename MatrixType >
		requires (
			std::same_as < typename MatrixType::value_type, tensor_dense_dynamic::value_type > &&
			typename MatrixType::rank() == tensor_dense_dynamic::_rank &&
			std::same_as <
				MatrixType,
				algebra::tensor_dense_dynamic <
					typename MatrixType::value_type,
					tensor_rank < MatrixType::rank() >,
					typename MatrixType::memory_order_indices,
					tensor_type < MatrixType::type() > > >)
		constexpr tensor_dense_dynamic &operator-=(const MatrixType &subtrahend)
		requires (algebra::concepts::subtractable_assignable < Type >)
		{
			if (subtrahend.dimensionalities() != this->dimensionalities())
			{
				throw std::domain_error(""); // TODO: Create message.
			}

			tensor_dense_dynamic::subtraction_assignment(*this, subtrahend);

			return *this;
		}

		constexpr tensor_dense_dynamic &operator*=(const Type &multiplicand)
		requires (algebra::concepts::multiplicable_assignable < Type >)
		{
			constexpr auto binaryOperator =
				[] < typename ElementType > (ElementType &thisValue, const Type &otherValue) -> void
				{
					if constexpr (tensor_dense_dynamic::_type == tensor_type_value::sub)
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

			for (auto &value : this->_data)
			{
				binaryOperator(value, multiplicand);
			}

			return *this;
		}

		constexpr tensor_dense_dynamic &operator/=(const Type &divisor)
		requires (algebra::concepts::divisable_assignable < Type >)
		{
			constexpr auto binaryOperator =
				[] < typename ElementType > (ElementType &thisValue, const Type &otherValue) -> void
				{
					if constexpr (tensor_dense_dynamic::_type == tensor_type_value::sub)
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
				if (divisor == tensor_dense_dynamic::_null_value)
				{
					throw std::domain_error("\"divisor\" is zero.");
				}
			}

			for (auto &value : this->_data)
			{
				binaryOperator(value, divisor);
			}

			return *this;
		}

		template < typename ... Types >
		requires (sizeof...(Types) == tensor_dense_dynamic::_rank && (std::convertible_to < Types, size_t > && ...))
		[[nodiscard]] constexpr const_reference operator()(Types && ... args) const
		{
			return this->const_parentheses_operator(std::forward_as_tuple(args...));
		}

		template < typename ... Types >
		requires (sizeof...(Types) == tensor_dense_dynamic::_rank && (std::convertible_to < Types, size_t > && ...))
		[[nodiscard]] constexpr reference operator()(Types && ... args)
		{
			return this->parentheses_operator(std::forward_as_tuple(args...));
		}

		template < typename ... Types >
		requires (sizeof...(Types) == tensor_dense_dynamic::_rank && (std::convertible_to < Types, size_t > && ...))
		[[nodiscard]] constexpr const_reference operator()(const std::tuple < Types ... > &args) const
		{
			return this->const_parentheses_operator(args);
		}

		template < typename ... Types >
		requires (sizeof...(Types) == tensor_dense_dynamic::_rank && (std::convertible_to < Types, size_t > && ...))
		[[nodiscard]] constexpr reference operator()(const std::tuple < Types ... > &args)
		{
			return this->parentheses_operator(args);
		}

		[[nodiscard]] constexpr const_reference operator()(const std::array < size_t, tensor_dense_dynamic::_rank > &args) const
		{
			return this->const_parentheses_operator(std::tuple_cat(args));
		}

		[[nodiscard]] constexpr reference operator()(const std::array < size_t, tensor_dense_dynamic::_rank > &args)
		{
			return this->parentheses_operator(std::tuple_cat(args));
		}

		constexpr void fill(const Type &value)
		{
			if constexpr (tensor_dense_dynamic::_type == tensor_type_value::sub)
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
				std::fill(this->_data.begin(), this->_data.end(), value);
			}
		}

		[[nodiscard]] constexpr tensor_dense_dynamic transpose() const
		requires (tensor_dense_dynamic::_rank == 2)
		{
			// TODO: Improve performance of this method, using line copy.
			tensor_dense_dynamic result(this->_dimensionalities[1], this->_dimensionalities[0]);

			for (size_t i = 0; i < this->_dimensionalities[1]; i++)
			{
				for (size_t j = 0; j < this->_dimensionalities[0]; j++)
				{
					result(i, j) = (*this)(j, i);
				}
			}

			return result;
		}

		[[nodiscard]] constexpr tensor_dense_dynamic conjugate_transpose() const
		requires (tensor_dense_dynamic::_rank == 2)
		{
			if constexpr (xablau::algebra::concepts::complex < Type >)
			{
				// TODO: Improve performance of this method, using line copy.
				tensor_dense_dynamic result(this->_dimensionalities[1], this->_dimensionalities[0]);

				for (size_t i = 0; i < this->_dimensionalities[1]; i++)
				{
					for (size_t j = 0; j < this->_dimensionalities[0]; j++)
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

		template < typename ... DimensionalityTypes >
		requires (
			tensor_dense_dynamic::_rank != 0 &&
			sizeof...(DimensionalityTypes) == tensor_dense_dynamic::_rank &&
			(std::convertible_to < DimensionalityTypes, size_t > && ...))
		constexpr void resize(DimensionalityTypes && ... args)
		{
			this->resize(args..., tensor_dense_dynamic::_null_value);
		}

		// Thanks, https://www.cppstories.com/2020/09/variadic-pack-first.html/!
		template < typename ... DimensionalityTypesAndInitializationValue >
		requires (
			tensor_dense_dynamic::_rank != 0 &&
			sizeof...(DimensionalityTypesAndInitializationValue) == (tensor_dense_dynamic::_rank + 1))
		constexpr void resize(DimensionalityTypesAndInitializationValue && ... args)
		{
			this->resize_with_initialization(
				std::forward_as_tuple(args...),
				std::make_index_sequence < sizeof...(args) - 1 > {});
		}

		template < typename ... DimensionalityTypes >
		requires (
			tensor_dense_dynamic::_rank != 0 &&
			sizeof...(DimensionalityTypes) == tensor_dense_dynamic::_rank &&
			(std::convertible_to < DimensionalityTypes, size_t > && ...))
		constexpr void resize(
			const std::tuple < DimensionalityTypes && ... > &args,
			const Type &value = tensor_dense_dynamic::_null_value)
		{
			this->resize_with_initialization(args, value);
		}

		constexpr void resize(
			const std::array < size_t, tensor_dense_dynamic::_rank > &size,
			const Type &value = tensor_dense_dynamic::_null_value)
		requires (tensor_dense_dynamic::_rank != 0)
		{
			this->resize_with_initialization(std::tuple_cat(size), value);
		}

		constexpr void clear() noexcept
		requires (tensor_dense_dynamic::_rank != 0)
		{
			this->_dimensionalities.fill(0);
			this->_size = 0;
			this->_data.clear();
		}

		friend std::ostream &operator<<(std::ostream &stream, const tensor_dense_dynamic &tensor)
		{
			const auto &dimensionalities = tensor.dimensionalities();

			stream << "[";

			if constexpr (tensor_dense_dynamic::_rank > 0)
			{
				size_t i{};

				for (i = 0; i < dimensionalities.size() - 1; i++)
				{
					stream << dimensionalities[i] << ", ";
				}

				stream << dimensionalities[i];
			}

			stream << "] ";

			std::array < size_t, tensor_dense_dynamic::_rank > indices{};

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

			if constexpr (tensor_dense_dynamic::_rank == 0)
			{
				stream << "( " << *tensor << " )";
			}

			else
			{
				printNDimensions(0, false, printNDimensions);
			}

			return stream;
		}

		constexpr tensor_dense_dynamic &operator=(const tensor_dense_dynamic &) = default;

		constexpr tensor_dense_dynamic &operator=(tensor_dense_dynamic &&) noexcept = default;

		constexpr tensor_dense_dynamic() noexcept
		requires (tensor_dense_dynamic::_type == tensor_type_value::main)
		{
			if constexpr (tensor_dense_dynamic::_rank == 0)
			{
				this->_data.resize(1);
				this->_size = 1;
			}
		}

		template < typename... Types >
		requires (
			sizeof...(Types) == tensor_dense_dynamic::_rank &&
			(std::convertible_to < Types, size_t > && ...) &&
			sizeof...(Types) != 0)
		explicit constexpr tensor_dense_dynamic(Types && ... args)
		requires (tensor_dense_dynamic::_type == tensor_type_value::main)
		{
			this->resize(args...);
		}

		explicit constexpr tensor_dense_dynamic(
			const std::array < size_t, tensor_dense_dynamic::_rank > &dimensionalities,
			const Type &value = tensor_dense_dynamic::_null_value)
		requires (tensor_dense_dynamic::_type == tensor_type_value::main)
		{
			this->resize(dimensionalities, value);
		}

		constexpr tensor_dense_dynamic(const tensor_dense_dynamic &) = default;

		constexpr tensor_dense_dynamic(tensor_dense_dynamic &&) noexcept = default;
	};

	template <
		typename Type,
		concepts::tensor_rank Rank,
		concepts::tensor_memory_order_indices MemoryOrderIndices,
		concepts::tensor_type TensorType >
	requires (
		concepts::valid_tensor_dynamic_memory_order_indices < Rank, MemoryOrderIndices > &&
		concepts::valid_tensor_type < Type, TensorType >)
	const Type tensor_dense_dynamic < Type, Rank, MemoryOrderIndices, TensorType > ::_null_value = Type{};
}