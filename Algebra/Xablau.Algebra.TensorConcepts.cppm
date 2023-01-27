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

export module xablau.algebra:tensor_concepts;
export import :fundamental_concepts;
export import :tensor_dense_dynamic;
export import :tensor_dense_fixed;

export import <algorithm>;
export import <array>;
export import <complex>;
export import <concepts>;
export import <ranges>;
export import <type_traits>;
export import <tuple>;

namespace xablau::algebra::concepts
{
	namespace internals
	{
		template < typename >
		struct is_tensor_dense_dynamic : std::false_type {};

		template <
			typename Type,
			concepts::tensor_rank Rank,
			concepts::tensor_contiguity Contiguous,
			concepts::tensor_memory_order_indices MemoryOrderIndices,
			concepts::tensor_type TensorType >
		struct is_tensor_dense_dynamic <
			xablau::algebra::tensor_dense_dynamic <
				Type,
				Rank,
				Contiguous,
				MemoryOrderIndices,
				TensorType > > : std::true_type {};

		template < typename >
		struct is_tensor_dense_fixed : std::false_type {};

		template <
			typename Type,
			concepts::tensor_fixed_dimensionalities FixedDimensionalities,
			concepts::tensor_contiguity Contiguous,
			concepts::tensor_memory_order_indices MemoryOrderIndices,
			concepts::tensor_type TensorType >
		struct is_tensor_dense_fixed <
			xablau::algebra::tensor_dense_fixed <
				Type,
				FixedDimensionalities,
				Contiguous,
				MemoryOrderIndices,
				TensorType > > : std::true_type {};

		template < typename TensorDynamic >
		concept xablau_tensor_dense_dynamic_implementation_check =
			internals::is_tensor_dense_dynamic < typename std::remove_cv < TensorDynamic > ::type > ::value;

		template < typename TensorFixed >
		concept xablau_tensor_dense_fixed_implementation_check =
			internals::is_tensor_dense_fixed < typename std::remove_cv < TensorFixed > ::type > ::value;
	}

	export template < typename Tensor >
	concept xablau_tensor =
		internals::xablau_tensor_dense_dynamic_implementation_check < Tensor > ||
		internals::xablau_tensor_dense_fixed_implementation_check < Tensor >;

	export template < typename Tensor >
	concept xablau_tensor_dense = xablau_tensor < Tensor > && Tensor::sparse() == false;

	export template < typename Tensor >
	concept xablau_tensor_sparse = xablau_tensor < Tensor > && Tensor::sparse() == true;

	export template < typename Tensor >
	concept xablau_tensor_dynamic = xablau_tensor < Tensor > && Tensor::fixed() == false;

	export template < typename Tensor >
	concept xablau_tensor_fixed = xablau_tensor < Tensor > && Tensor::fixed() == true;

	export template < typename Tensor >
	concept xablau_tensor_main = xablau_tensor < Tensor > && Tensor::type() == tensor_type_value::main;

	export template < typename Tensor >
	concept xablau_tensor_sub = xablau_tensor < Tensor > && Tensor::type() == tensor_type_value::sub;

	export template < typename Matrix >
	concept xablau_matrix = xablau_tensor < Matrix > && Matrix::rank() == 2;

	export template < typename Matrix >
	concept xablau_matrix_dense = xablau_tensor_dense < Matrix > && Matrix::rank() == 2;

	export template < typename Matrix >
	concept xablau_matrix_sparse = xablau_tensor_sparse < Matrix > && Matrix::rank() == 2;

	export template < typename Matrix >
	concept xablau_matrix_dynamic = xablau_tensor_dynamic < Matrix > && Matrix::rank() == 2;

	export template < typename Matrix >
	concept xablau_matrix_fixed = xablau_tensor_fixed < Matrix > && Matrix::rank() == 2;

	export template < typename Matrix >
	concept xablau_matrix_fixed_square =
		xablau_matrix_fixed < Matrix > &&
		Matrix::dimensionalities()[0] == Matrix::dimensionalities()[1];

	export template < typename Matrix >
	concept xablau_matrix_main = xablau_tensor_main < Matrix > && Matrix::rank() == 2;

	export template < typename Matrix >
	concept xablau_matrix_sub = xablau_tensor_sub < Matrix > && Matrix::rank() == 2;
}