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

export module xablau.algebra:utils;
export import :complex_concepts;
export import :fundamental_concepts;
export import :fundamental_configurations;
export import :fundamental_definitions;
export import :fundamental_functions;

export import std;

namespace xablau::algebra::utils
{
	namespace internals
	{
		template < typename AbsType, typename SquareMatrixType, typename VectorMatrixType >
		[[nodiscard]] auto Householder_matrix(const VectorMatrixType &vector)
		{
			SquareMatrixType identity{};

			if constexpr (!SquareMatrixType::fixed())
			{
				identity.resize(vector.rows(), vector.rows());
			}

			for (size_t i = 0; i < vector.rows(); i++)
			{
				identity(i, i) = xablau::algebra::functions::multiplication_identity < typename SquareMatrixType::value_type > ::invoke();
			}

			auto outer = vector * vector.conjugate_transpose();

			outer *= AbsType{2};

			return identity - outer;
		}

		template < typename AbsType, typename VectorMatrixType >
		[[nodiscard]] AbsType Householder_norm(const VectorMatrixType &vector)
		{
			AbsType norm{};

			for (size_t i = 0; i < vector.rows(); i++)
			{
				if constexpr (xablau::algebra::concepts::complex < typename VectorMatrixType::value_type >)
				{
					const auto &element = vector(i, 0);

					norm +=
						xablau::algebra::functions::pow < AbsType, AbsType > ::invoke(element.real(), AbsType{2}) +
						xablau::algebra::functions::pow < AbsType, AbsType > ::invoke(element.imag(), AbsType{2});
				}

				else if constexpr (xablau::algebra::concepts::standard_arithmetic < typename VectorMatrixType::value_type >)
				{
					norm += xablau::algebra::functions::pow < AbsType, AbsType > ::invoke(vector(i, 0), AbsType{2});
				}

				else
				{
					norm +=
						xablau::algebra::functions::pow < AbsType, AbsType > ::invoke(
							xablau::algebra::functions::abs < AbsType > ::invoke(vector(i, 0)),
							AbsType{2});
				}
			}

			norm = xablau::algebra::functions::sqrt < AbsType > ::invoke(norm);

			return norm;
		}

		template < typename AbsType, typename VectorMatrixType >
		[[nodiscard]] xablau::algebra::sign_value Householder_sign(const VectorMatrixType &vector, const size_t step)
		{
			const auto &element = vector(step, 0);

			if constexpr (xablau::algebra::concepts::complex < typename VectorMatrixType::value_type >)
			{
				const auto sign = xablau::algebra::functions::sign < AbsType > ::invoke(element.real());

				return *(sign.cbegin());
			}

			else
			{
				const auto sign = xablau::algebra::functions::sign < AbsType > ::invoke(element);

				static_assert(sign.size() == 1, "Unexpected size for \"sign_type < Type > ::type\"");

				return *(sign.cbegin());
			}
		}

		template < typename AbsType, typename VectorMatrixType >
		requires (xablau::algebra::concepts::negatable < AbsType >)
		[[nodiscard]] auto Householder_vector(const VectorMatrixType &vector, const size_t step)
		{
			auto _vector = vector.convert_to_main();
			typename VectorMatrixType::main_type u{};
			typename VectorMatrixType::main_type v{};

			if constexpr (!VectorMatrixType::fixed())
			{
				u.resize(vector.rows(), 1);
				v.resize(vector.rows(), 1);
			}

			for (size_t i = 0; i < step; i++)
			{
				_vector(i, 0) = typename VectorMatrixType::value_type{};
			}

			auto normVector = Householder_norm < AbsType > (_vector);

			if (Householder_sign < AbsType > (_vector, step) == xablau::algebra::sign_value::negative)
			{
				normVector = -normVector;
			}

			u(step, 0) = normVector;

			for (size_t i = 0; i < u.rows(); i++)
			{
				u(i, 0) = _vector(i, 0) + u(i, 0);
			}

			const auto normU = Householder_norm < AbsType > (u);

			for (size_t i = 0; i < v.rows(); i++)
			{
				v(i, 0) = u(i, 0) / normU;
			}

			return v;
		}
	}

	template < typename AbsType, typename QType, typename MatrixType >
	[[nodiscard]] constexpr auto QR(const MatrixType &matrix)
	{
		const auto converter =
			[&matrix] () -> auto
			{
				if constexpr (MatrixType::type() == tensor_type_value::sub)
				{
					return matrix.convert_to_main();
				}

				else
				{
					return matrix;
				}
			};

		typename QType::main_type Q{};
		typename MatrixType::main_type R = converter();

		if constexpr (!MatrixType::fixed())
		{
			Q.resize(matrix.rows(), matrix.rows());
		}

		for (size_t i = 0; i < matrix.rows(); i++)
		{
			Q(i, i) = xablau::algebra::functions::multiplication_identity < typename MatrixType::value_type > ::invoke();
		}

		for (size_t i = 0; i < matrix.columns(); i++)
		{
			const auto vector = internals::Householder_vector < AbsType > (R.column(i), i);
			const auto Qt = internals::Householder_matrix < AbsType, QType > (vector);

			Q = Q * Qt;
			R = Qt * R;
		}

		return std::make_pair(Q, R);
	}

	template <
		bool CalculateEigenvectors,
		typename EigenvaluesType,
		typename AbsType,
		typename QType,
		typename MatrixType >
	[[nodiscard]] constexpr auto eigen_Hermitian(const MatrixType &matrix)
	{
		const auto converter =
			[&matrix] () -> auto
			{
				if constexpr (MatrixType::type() == tensor_type_value::sub)
				{
					return matrix.convert_to_main();
				}

				else
				{
					return matrix;
				}
			};

		const size_t iterations = 30 * matrix.rows();
		auto result = converter();
		typename EigenvaluesType::main_type eigenvalues{};
		typename MatrixType::main_type eigenvectors{};

		if constexpr (!MatrixType::fixed())
		{
			eigenvalues.resize(matrix.rows(), 1);

			if constexpr (CalculateEigenvectors)
			{
				eigenvectors.resize(matrix.rows(), matrix.rows());
			}
		}

		if constexpr (CalculateEigenvectors)
		{
			for (size_t i = 0; i < matrix.rows(); i++)
			{
				eigenvectors(i, i) = xablau::algebra::functions::multiplication_identity < typename MatrixType::value_type > ::invoke();
			}
		}

		for (size_t i = 0; i < iterations; i++)
		{
			const auto [Q, R] = QR < AbsType, typename QType::main_type > (result);

			result = R * Q;

			if constexpr (CalculateEigenvectors)
			{
				eigenvectors = eigenvectors * Q;
			}
		}

		const auto [Q, R] = QR < AbsType, typename QType::main_type > (result);

		result = Q * R;

		if constexpr (CalculateEigenvectors)
		{
			eigenvectors = eigenvectors * Q;
		}

		for (size_t i = 0; i < matrix.rows(); i++)
		{
			eigenvalues(i, 0) = result(i, i);
		}

		if constexpr (CalculateEigenvectors)
		{
			return std::make_pair(eigenvalues, eigenvectors);
		}

		else
		{
			return eigenvalues;
		}
	}

	// Thanks, http://lampx.tugraz.at/~hadley/num/ch2/2.3a.php!
	template < typename MatrixType >
	[[nodiscard]] constexpr auto LUP_decomposition(const MatrixType &matrix)
	{
		// TODO: Use a better algorithm.
		using Type = typename MatrixType::value_type;

		size_t row{};
		size_t swaps{};
		const auto lines = std::min(matrix.rows(), matrix.columns());
		auto LU = matrix;
		MatrixType P{};

		if constexpr (!MatrixType::fixed())
		{
			LU.resize(lines, lines);
			P.resize(lines, lines);
		}

		for (size_t i = 0; i < lines; i++)
		{
			P(i, i) = xablau::algebra::functions::multiplication_identity < typename MatrixType::value_type > ::invoke();
		}

		for (size_t i = 0; i < lines; i++)
		{
			typename abs_type < Type > ::type maxU{};

			for (size_t r = i; r < lines; r++)
			{
				size_t q{};
				Type diagonalElementU = LU(r, i);

				while (q < i)
				{
					diagonalElementU -= LU(r, q) * LU(q, r);
					q++;
				}

				const auto absMaxU = functions::abs < Type > ::invoke(diagonalElementU);

				if (absMaxU > maxU)
				{
					maxU = absMaxU;
					row = r;
				}
			}

			if (i != row)
			{
				swaps++;

				for (size_t q = 0; q < lines; q++)
				{
					std::swap(P(i, q), P(row, q));
					std::swap(LU(i, q), LU(row, q));
				}
			}

			auto j = i;

			while (j < lines)
			{
				size_t q{};

				while (q < i)
				{
					LU(i, j) -= LU(i, q) * LU(q, j);
					q++;
				}

				j++;
			}

			j = i + 1;

			while (j < lines)
			{
				size_t q{};

				while (q < i)
				{
					LU(j, i) -= LU(j, q) * LU(q, i);
					q++;
				}

				if (LU(i, i) != Type{}) [[likely]]
				{
					LU(j, i) /= LU(i, i);
				}

				else [[unlikely]]
				{
					LU(j, i) = Type{};
				}

				j++;
			}
		}

		return std::make_tuple(swaps, LU, P);
	}

	// Thanks, http://lampx.tugraz.at/~hadley/num/ch2/2.1.php!
	template < typename MatrixType, typename SolutionType >
	[[nodiscard]] constexpr SolutionType LU_solve(const MatrixType &LU, const SolutionType &b)
	{
		// TODO: Use a better algorithm.
		using Type = typename MatrixType::value_type;
		using SignedSizeT = std::make_signed < size_t > ::type;

		SignedSizeT N = static_cast < SignedSizeT > (LU.columns());
		SignedSizeT Nb = static_cast < SignedSizeT > (b.columns());

		SolutionType x{};
		SolutionType y{};

		if constexpr (!MatrixType::fixed())
		{
			if (LU.columns() != b.rows())
			{
				throw std::invalid_argument(""); // TODO: Create message.
			}

			x.resize(b.dimensionalities());
			y.resize(b.dimensionalities());
		}

		for (SignedSizeT q = 0; q < Nb; q++)
		{
			for (SignedSizeT i = 0; i < N; i++)
			{
				y(i, q) = b(i, q);

				SignedSizeT j = 0;

				while (j < i)
				{
					y(i, q) += -LU(i, j) * y(j, q);
					j++;
				}
			}

			for (SignedSizeT i = N - 1; i > -1; i--)
			{
				x(i, q) = y(i, q);

				SignedSizeT j = i + 1;

				while (j < N)
				{
					x(i, q) += -LU(i, j) * x(j, q);
					j++;
				}

				if (LU(i, i) != Type{}) [[likely]]
				{
					x(i, q) /= LU(i, i);
				}

				else [[unlikely]]
				{
					x(i, q) = Type{};
				}
			}
		}

		return x;
	}

	template < size_t Rank >
	[[nodiscard]] constexpr bool valid_kernel_size(const std::array < size_t, Rank > &kernel_size) noexcept
	{
		for (const auto dimensionality : kernel_size)
		{
			if (dimensionality % 2 == 0)
			{
				return false;
			}
		}

		return true;
	}

	template < size_t Rank >
	constexpr void increment_index(
		std::array < size_t, Rank > &indices,
		const std::array < size_t, Rank > &dimensionalities)
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
	}
}