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

export module xablau.algebra:tensor_configurations;

export import std;

export namespace xablau::algebra
{
	enum class tensor_type_value
	{
		main,
		sub
	};

	template < size_t Rank >
	struct tensor_rank
	{
		static consteval size_t rank() noexcept
		{
			return Rank;
		}
	};

	template < size_t ... Dimensionalities >
	struct tensor_fixed_dimensionalities
	{
		static constexpr std::array < size_t, sizeof...(Dimensionalities) > array =
			{ std::forward < size_t > (Dimensionalities)... };

		static constexpr auto tuple = std::forward_as_tuple(Dimensionalities...);

		static consteval size_t size() noexcept
		{
			return sizeof...(Dimensionalities);
		}
	};

	template < size_t ... MemoryOrderIndices >
	struct tensor_memory_order_indices
	{
		static constexpr std::array < size_t, sizeof...(MemoryOrderIndices) > array =
			{ std::forward < size_t > (MemoryOrderIndices)... };

		static constexpr auto tuple = std::forward_as_tuple(MemoryOrderIndices...);

		static consteval size_t size() noexcept
		{
			return sizeof...(MemoryOrderIndices);
		}
	};

	template < tensor_type_value Type >
	struct tensor_type
	{
		static consteval tensor_type_value type() noexcept
		{
			return Type;
		}
	};

	using row_major_memory_order = tensor_memory_order_indices < 0, 1 >;

	using column_major_memory_order = tensor_memory_order_indices < 1, 0 >;
}