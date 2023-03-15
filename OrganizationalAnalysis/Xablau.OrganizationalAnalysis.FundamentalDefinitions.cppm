// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

// MIT License
//
// Copyright (c) 2023 Jean Amaro <jean.amaro@outlook.com.br>
// Copyright (c) 2023 Lucas Melchiori Pereira <lc.melchiori@gmail.com>
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

// We thank FAPESP (Fundação de Amparo à Pesquisa do Estado de São Paulo) for
// funding this research project.
// FAPESP process number: 2020/15909-8

export module xablau.organizational_analysis:fundamental_definitions;

export import <map>;
export import <set>;
export import <string>;

export import xablau.graph;

export namespace xablau::organizational_analysis
{
	namespace internals
	{
		template < typename CharType >
		constexpr auto &default_error_output()
		{
			if constexpr (std::same_as < CharType, char >)
			{
				return std::cerr;
			}

			else
			{
				return std::wcerr;
			}
		}
	}

	constexpr float indirectly_related = 1.0f;
	constexpr float related = 2.0f;
	constexpr float directly_related = 3.0f;

	template < typename CharType, typename Traits >
	struct agents
	{
		using string_type = std::basic_string < CharType, Traits >;

		struct description
		{
			string_type group{};
			string_type role{};
		};

		std::map < string_type, agents::description > descriptions{};
	};

	template < typename CharType, typename Traits >
	struct activities
	{
		using string_type = std::basic_string < CharType, Traits >;

		struct description
		{
			string_type name{};
			string_type group{};
			std::set < string_type > agents_in_charge{};
		};

		std::map < string_type, activities::description > descriptions{};

		xablau::graph::digraph <
			xablau::graph::node < string_type >,
			xablau::graph::graph_container_type < xablau::graph::graph_container_type_value::ordered >,
			xablau::graph::edge < float > > dependencies{};
	};

	template < bool InterfacesAreReciprocal, typename CharType, typename Traits >
	struct components
	{
		using string_type = std::basic_string < CharType, Traits >;

		struct description
		{
			string_type name{};
			string_type group{};
			std::set < string_type > agents_in_charge{};
		};

		std::map < string_type, components::description > descriptions{};

		typename std::conditional <
			InterfacesAreReciprocal,
			xablau::graph::graph <
				xablau::graph::node < string_type >,
				xablau::graph::graph_container_type < xablau::graph::graph_container_type_value::ordered >,
				xablau::graph::edge < float > >,
			xablau::graph::digraph <
				xablau::graph::node < string_type >,
				xablau::graph::graph_container_type < xablau::graph::graph_container_type_value::ordered >,
				xablau::graph::edge < float > > > ::type interactions{};
	};

	template < typename CharType, typename Traits >
	struct affiliations
	{
		using string_type = std::basic_string < CharType, Traits >;

		std::map < string_type, std::map < string_type, float > > responsabilities{};
	};
}