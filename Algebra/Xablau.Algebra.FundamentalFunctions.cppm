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

export module xablau.algebra:fundamental_functions;

export namespace xablau::algebra::functions
{
	template < typename >
	struct abs;

	template < typename >
	struct exp;

	template < typename >
	struct log;

	template < typename >
	struct log2;

	template < typename >
	struct log10;

	template < typename, typename >
	struct pow;

	template < typename >
	struct sqrt;

	template < typename >
	struct sin;

	template < typename >
	struct cos;

	template < typename >
	struct tan;

	template < typename >
	struct asin;

	template < typename >
	struct acos;

	template < typename >
	struct atan;

	template < typename >
	struct sinh;

	template < typename >
	struct cosh;

	template < typename >
	struct tanh;

	template < typename >
	struct asinh;

	template < typename >
	struct acosh;

	template < typename >
	struct atanh;

	template < typename >
	struct almost_equal;

	template < typename >
	struct addition_identity;

	template < typename >
	struct multiplication_identity;

	template < typename >
	struct sign;

	template < typename >
	struct normal;

	template < typename, typename >
	struct min;

	template < typename, typename >
	struct max;

	template < typename >
	struct min_ref_return;

	template < typename >
	struct max_ref_return;

	template < typename >
	struct conjugate;
}