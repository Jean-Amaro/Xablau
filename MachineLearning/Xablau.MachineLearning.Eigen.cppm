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

#include <Eigen/Eigen>

import xablau.algebra;

template < std::floating_point Type >
auto generic_eigenvalues(
	const xablau::algebra::tensor_dense_dynamic <
		Type,
		xablau::algebra::tensor_rank < 2 >,
		xablau::algebra::tensor_contiguity < true > > &matrix)
{
	xablau::algebra::tensor_dense_dynamic <
		Type,
		xablau::algebra::tensor_rank < 2 >,
		xablau::algebra::tensor_contiguity < true > > result(matrix.rows(), 1);

	Eigen::Matrix < Type, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor > eigenMatrix;

	eigenMatrix.resize(matrix.rows(), matrix.columns());

	for (size_t i = 0; i < matrix.rows(); i++)
	{
		for (size_t j = 0; j < matrix.columns(); j++)
		{
			eigenMatrix(i, j) = matrix(i, j);
		}
	}

	const Eigen::SelfAdjointEigenSolver < Eigen::Matrix < Type, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor > > solver(
		eigenMatrix);

	for (size_t i = 0; i < matrix.rows(); i++)
	{
		result(i, 0) = solver.eigenvalues()[i];
	}

	return result;
}

namespace xablau::machine_learning
{
	[[nodiscard]]
		xablau::algebra::tensor_dense_dynamic <
			float,
			xablau::algebra::tensor_rank < 2 >,
			xablau::algebra::tensor_contiguity < true > >
		eigenvalues_self_adjoint(
			const xablau::algebra::tensor_dense_dynamic <
				float,
				xablau::algebra::tensor_rank < 2 >,
				xablau::algebra::tensor_contiguity < true > > &matrix)
	{
		return generic_eigenvalues < float > (matrix);
	}

	[[nodiscard]]
		xablau::algebra::tensor_dense_dynamic <
			double,
			xablau::algebra::tensor_rank < 2 >,
			xablau::algebra::tensor_contiguity < true > >
		eigenvalues_self_adjoint(
			const xablau::algebra::tensor_dense_dynamic <
				double,
				xablau::algebra::tensor_rank < 2 >,
				xablau::algebra::tensor_contiguity < true > > &matrix)
	{
		return generic_eigenvalues < double > (matrix);
	}

	[[nodiscard]]
		xablau::algebra::tensor_dense_dynamic <
			long double,
			xablau::algebra::tensor_rank < 2 >,
			xablau::algebra::tensor_contiguity < true > >
		eigenvalues_self_adjoint(
			const xablau::algebra::tensor_dense_dynamic <
				long double,
				xablau::algebra::tensor_rank < 2 >,
				xablau::algebra::tensor_contiguity < true > > &matrix)
	{
		return generic_eigenvalues < long double > (matrix);
	}
}