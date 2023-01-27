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

export module xablau.testing:test_bundler;

import <compare>;
import <functional>;
import <numeric>;
import <numbers>;
import <random>;
import <set>;
import <tuple>;
import <unordered_set>;
import <vector>;

namespace xablau::testing
{
	namespace internals
	{
		template < typename Type >
		struct is_specialization_tuple : std::false_type {};

		template < typename ... Types >
		struct is_specialization_tuple < std::tuple < Types ... > > : std::true_type {};

		template < typename Type, typename ... OtherTypes >
		consteval bool is_non_empty_tuple()
		{
			if constexpr (!is_specialization_tuple < Type > ::value)
			{
				return false;
			}

			else if constexpr (std::tuple_size < Type > ::value == 0)
			{
				return false;
			}

			if constexpr (sizeof...(OtherTypes) > 0)
			{
				return is_non_empty_tuple < OtherTypes... > ();
			}

			return true;
		}

		template < typename ... Types >
		concept non_empty_tuple = is_non_empty_tuple < Types ... > ();
	}

	export template < internals::non_empty_tuple ... Tuples >
	requires (sizeof...(Tuples) > 0)
	class test_bundler
	{
	public:
		static constexpr size_t elements_quantity{sizeof...(Tuples)};

		static constexpr std::array < size_t, sizeof...(Tuples) > element_on_each_tuple_quantities{ std::tuple_size < Tuples > ::value ... };

		static constexpr size_t possible_combinations_quantity{
			std::accumulate(
				test_bundler::element_on_each_tuple_quantities.cbegin(),
				test_bundler::element_on_each_tuple_quantities.cend(),
				size_t{1},
				std::multiplies{})};

		using tuple_type = std::tuple < Tuples ... >;

	private:
		using index_sequence = std::make_index_sequence < sizeof...(Tuples) >;

		template < size_t GlobalIndex >
		requires (GlobalIndex < test_bundler::possible_combinations_quantity)
		static consteval std::array < size_t, test_bundler::elements_quantity > calculate_index()
		{
			std::array < size_t, test_bundler::elements_quantity > indices{};

			std::exclusive_scan(
				test_bundler::element_on_each_tuple_quantities.crbegin(),
				test_bundler::element_on_each_tuple_quantities.crend(),
				indices.rbegin(),
				size_t{1},
				std::multiplies < size_t > {});

			for (size_t i = 0; i < test_bundler::element_on_each_tuple_quantities.size(); i++)
			{
				indices[i] = (GlobalIndex / indices[i]) % test_bundler::element_on_each_tuple_quantities[i];
			}

			return indices;
		}

		template < template < typename ... > typename, size_t, typename >
		struct fill_wrapper;

		template < template < typename ... > typename Wrapper, size_t GlobalIndex, std::size_t ... Is >
		struct fill_wrapper < Wrapper, GlobalIndex, std::index_sequence < Is ... > >
		{
			using test_case =
				Wrapper <
					typename std::tuple_element < test_bundler::calculate_index < GlobalIndex > () [Is],
						typename std::tuple_element < Is, typename test_bundler::tuple_type > ::type > ::type ... >;
		};

	public:
		template <
			template < typename ... Args > typename Wrapper,
			size_t Index >
		using test_case =
			typename test_bundler::fill_wrapper <
				Wrapper,
				Index,
				typename std::make_index_sequence < test_bundler::elements_quantity > > ::test_case;

	private:
		template < template < typename ... > typename, typename >
		struct fill_group;

		template < template < typename ... > typename Wrapper, std::size_t ... Is >
		struct fill_group < Wrapper, std::index_sequence < Is... > >
		{
			using test_group = std::tuple < typename test_bundler::test_case < Wrapper, Is > ... >;
		};

	public:
		template < template < typename ... > typename Wrapper >
		using test_group =
			typename test_bundler::fill_group <
				Wrapper,
				typename std::make_index_sequence < test_bundler::possible_combinations_quantity > > ::test_group;
	};
}