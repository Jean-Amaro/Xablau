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

export module xablau.algebra:fundamental_concepts;
export import :complex_configurations;
export import :tensor_configurations;

export import <algorithm>;
export import <array>;
export import <complex>;
export import <concepts>;
export import <ranges>;
export import <tuple>;
export import <type_traits>;

namespace xablau::algebra::concepts
{
	namespace internals
	{
		template < typename Type, template < size_t ... > typename Unspecialized >
		struct is_specialization_size_t_pack : std::false_type{};

		template < template < size_t ... > typename Unspecialized, size_t ... Args >
		struct is_specialization_size_t_pack < Unspecialized < Args... >, Unspecialized > : std::true_type{};

		template < typename Type, template < bool > typename Unspecialized >
		struct is_specialization_bool : std::false_type{};

		template < template < bool > typename Unspecialized, bool Arg >
		struct is_specialization_bool < Unspecialized < Arg >, Unspecialized > : std::true_type{};

		template < typename Type, template < algebra::complex_ordering_value > typename Unspecialized >
		struct is_specialization_complex_ordering_value : std::false_type{};

		template < template < algebra::complex_ordering_value > typename Unspecialized, algebra::complex_ordering_value Arg >
		struct is_specialization_complex_ordering_value < Unspecialized < Arg >, Unspecialized > : std::true_type{};

		template < typename Type, template < algebra::tensor_type_value > typename Unspecialized >
		struct is_specialization_tensor_type_value : std::false_type{};

		template < template < algebra::tensor_type_value > typename Unspecialized, algebra::tensor_type_value Arg >
		struct is_specialization_tensor_type_value < Unspecialized < Arg >, Unspecialized > : std::true_type{};

		template < size_t Rank, typename MemoryOrderIndices >
		requires (
			std::ranges::contiguous_range < decltype(MemoryOrderIndices::array) > &&
			std::ranges::sized_range < decltype(MemoryOrderIndices::array) >)
		[[nodiscard]] consteval bool valid_tensor_memory_order_indices() noexcept
		{
			auto tensorMemoryOrderIndices = MemoryOrderIndices::array;

			if (tensorMemoryOrderIndices.size() != 0 && tensorMemoryOrderIndices.size() != Rank)
			{
				return false;
			}

			std::sort(tensorMemoryOrderIndices.begin(), tensorMemoryOrderIndices.end());

			for (size_t i = 0; i < tensorMemoryOrderIndices.size(); i++)
			{
				if (tensorMemoryOrderIndices[i] != i)
				{
					return false;
				}
			}

			return true;
		}

		template < size_t Rank, typename KernelSize >
		[[nodiscard]] consteval bool valid_kernel_size() noexcept
		{
			for (const auto dimensionality : KernelSize::array)
			{
				if (dimensionality % 2 == 0)
				{
					return false;
				}
			}

			return KernelSize::size() == Rank;
		}
	}

	export template < typename Type >
	concept addable =
		requires (Type value) { { value + value } -> std::same_as < Type >; };

	export template < typename Type >
	concept addable_assignable =
		addable < Type > &&
		requires (Type value) { { value += value } -> std::same_as < Type & >; };

	export template < typename Type >
	concept subtractable =
		requires (Type value) { { value - value } -> std::same_as < Type >; };

	export template < typename Type >
	concept subtractable_assignable =
		subtractable < Type > &&
		requires (Type value) { { value -= value } -> std::same_as < Type & >; };

	export template < typename Type >
	concept multiplicable =
		requires (Type value) { { value * value } -> std::same_as < Type >; };

	export template < typename Type >
	concept multiplicable_assignable =
		multiplicable < Type > &&
		requires (Type value) { { value *= value } -> std::same_as < Type & >; };

	export template < typename Type >
	concept divisable =
		requires (Type value) { { value / value } -> std::same_as < Type >; };

	export template < typename Type >
	concept divisable_assignable =
		divisable < Type > &&
		requires (Type value) { { value /= value } -> std::same_as < Type & >; };

	export template < typename Type >
	concept negatable =
		!std::unsigned_integral < Type > &&
		requires (Type value) { { -value } -> std::same_as < Type >; };

	export template < typename Type >
	concept basic_arithmetic =
		std::regular < Type > &&
		addable < Type > &&
		subtractable < Type > &&
		multiplicable < Type > &&
		divisable < Type >;

	export template < typename Type >
	concept basic_assignable_arithmetic =
		basic_arithmetic < Type > &&
		addable_assignable < Type > &&
		subtractable_assignable < Type > &&
		multiplicable_assignable < Type > &&
		divisable_assignable < Type >;

	export template < typename Type >
	concept negatable_basic_arithmetic =
		negatable < Type > &&
		basic_arithmetic < Type >;

	export template < typename Type >
	concept negatable_basic_assignable_arithmetic =
		negatable < Type > &&
		basic_assignable_arithmetic < Type >;

	export template < typename Type >
	concept standard_integral = std::integral < Type >;

	export template < typename Type >
	concept standard_floating_point = std::floating_point < Type >;

	export template < typename Type >
	concept standard_arithmetic = std::is_arithmetic < Type > ::value;

	export template < typename Type >
	concept standard_negatable = std::is_signed < Type > ::value;

	export template < typename Type >
	concept standard_signed = std::is_signed < Type > ::value;

	export template < typename Type >
	concept standard_complex =
		std::same_as < Type, std::complex < typename Type::value_type > >;

	export template < typename Type >
	concept complex_ordering =
		internals::is_specialization_complex_ordering_value < Type, algebra::complex_ordering > ::value;

	export template < typename Rank >
	concept tensor_rank = internals::is_specialization_size_t_pack < Rank, algebra::tensor_rank > ::value;

	export template < typename Contiguity >
	concept tensor_contiguity = internals::is_specialization_bool < Contiguity, algebra::tensor_contiguity > ::value;

	export template < typename MemoryOrderIndices >
	concept tensor_memory_order_indices =
		internals::is_specialization_size_t_pack < MemoryOrderIndices, algebra::tensor_memory_order_indices > ::value;

	export template < typename Dimensionalities >
	concept tensor_fixed_dimensionalities =
		internals::is_specialization_size_t_pack < Dimensionalities, algebra::tensor_fixed_dimensionalities > ::value;

	export template < typename Type >
	concept tensor_type = internals::is_specialization_tensor_type_value < Type, algebra::tensor_type > ::value;

	template < typename Type, typename TensorType >
	concept valid_tensor_type =
		!(TensorType::type() == tensor_type_value::main && !std::semiregular < Type >);

	template < typename Rank, typename MemoryOrderIndices >
	concept valid_tensor_dynamic_memory_order_indices =
		tensor_rank < Rank > &&
		tensor_memory_order_indices < MemoryOrderIndices > &&
		internals::valid_tensor_memory_order_indices < Rank::rank(), MemoryOrderIndices > ();

	template < typename FixedDimensionalities, typename MemoryOrderIndices >
	concept valid_tensor_fixed_memory_order_indices =
		tensor_fixed_dimensionalities < FixedDimensionalities > &&
		tensor_memory_order_indices < MemoryOrderIndices > &&
		internals::valid_tensor_memory_order_indices < FixedDimensionalities::size(), MemoryOrderIndices > ();

	template < size_t Rank, typename FixedDimensionalities >
	concept valid_tensor_fixed_kernel_size = internals::valid_kernel_size < Rank, FixedDimensionalities > ();
}