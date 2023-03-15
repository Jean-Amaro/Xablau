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

export module xablau.organizational_analysis:writer;
export import :fundamental_definitions;

export import <concepts>;
export import <iostream>;
export import <istream>;
export import <format>;
export import <locale>;
export import <regex>;
export import <vector>;

import xablau.algebra;

export namespace xablau::organizational_analysis::writer
{
	template < typename CharType, typename Traits >
	requires (std::same_as < CharType, char > || std::same_as < CharType, wchar_t >)
	std::basic_ostream < CharType, Traits > &write_agents(
		std::basic_ostream < CharType, Traits > &output,
		const organizational_analysis::agents < CharType, Traits > &agents,
		const CharType separator)
	{
		if constexpr (std::same_as < CharType, char >)
		{
			output << "Identification" << separator << "Group" << separator << "Role\n";
		}

		else
		{
			output << L"Identification" << separator << L"Group" << separator << L"Role\n";
		}

		for (const auto &[identification, description] : agents.descriptions)
		{
			output << identification << separator;
			output << description.group << separator;
			output << description.role << CharType{'\n'};
		}

		return output;
	}

	template < typename CharType, typename Traits >
	requires (std::same_as < CharType, char > || std::same_as < CharType, wchar_t >)
	std::basic_ostream < CharType, Traits > &write_activities(
		std::basic_ostream < CharType, Traits > &output,
		const organizational_analysis::activities < CharType, Traits > &activities,
		const organizational_analysis::agents < CharType, Traits > &agents,
		const CharType separator)
	{
		using offset_type = typename std::basic_ostream < CharType, Traits > ::off_type;

		if constexpr (std::same_as < CharType, char >)
		{
			output << "Agent in charge" << separator << "Group" << separator << "Activity" << separator;
		}

		else
		{
			output << L"Agent in charge" << separator << "Group" << separator << "Activity" << separator;
		}

		for (const auto & [identification, description] : activities.descriptions)
		{
			output << separator << identification;
		}

		output << CharType{'\n'};

		for (const auto &[identification1, description1] : activities.descriptions)
		{
			for (const auto item : description1.agents_in_charge)
			{
				output << agents.descriptions.at(item).role;
				output << (separator == CharType{','} ? CharType{';'} : CharType{','});
				output << CharType{' '};
			}

			if (!description1.agents_in_charge.empty())
			{
				output.seekp(offset_type{-2}, std::ios_base::cur);
			}

			output << separator;
			output << description1.group << separator;
			output << description1.name << separator;
			output << identification1 << separator;

			for (const auto &[identification2, description2] : activities.descriptions)
			{
				if (identification1 == identification2)
				{
					if constexpr (std::same_as < CharType, char >)
					{
						output << "####";
					}

					else
					{
						output << L"####";
					}
				}

				else
				{
					if (activities.dependencies.contains(identification1, identification2))
					{
						if constexpr (std::same_as < CharType, char >)
						{
							output << std::to_string(1);
						}

						else
						{
							output << std::to_wstring(1);
						}
					}
				}

				output << separator;
			}

			output.seekp(offset_type{-1}, std::ios_base::cur);
			output << CharType{'\n'};
		}

		return output;
	}

	template < bool InterfacesAreReciprocal, typename CharType, typename Traits >
	requires (std::same_as < CharType, char > || std::same_as < CharType, wchar_t >)
	std::basic_ostream < CharType, Traits > &write_components(
		std::basic_ostream < CharType, Traits > &output,
		const organizational_analysis::components < InterfacesAreReciprocal, CharType, Traits > &components,
		const CharType separator)
	{
		using offset_type = typename std::basic_ostream < CharType, Traits > ::off_type;

		if constexpr (std::same_as < CharType, char >)
		{
			output << "Group" << separator << "Component" << separator;
		}

		else
		{
			output << L"Group" << separator << L"Component" << separator;
		}

		for (const auto &[identification, description] : components.descriptions)
		{
			output << separator << identification;
		}

		output << CharType{'\n'};

		for (const auto &[identification1, description1] : components.descriptions)
		{
			output << description1.group << separator;
			output << description1.name << separator;
			output << identification1 << separator;

			for (const auto &[identification2, description2] : components.descriptions)
			{
				if (identification1 == identification2)
				{
					if constexpr (std::same_as < CharType, char >)
					{
						output << "####";
					}

					else
					{
						output << L"####";
					}
				}

				else
				{
					if (components.interactions.contains(identification1, identification2))
					{
						if constexpr (std::same_as < CharType, char >)
						{
							output << std::to_string(1);
						}

						else
						{
							output << std::to_wstring(1);
						}
					}
				}

				output << separator;
			}

			output.seekp(offset_type{-1}, std::ios_base::cur);
			output << CharType{'\n'};
		}

		return output;
	}

	template < bool InterfacesAreReciprocal, typename CharType, typename Traits >
	requires (std::same_as < CharType, char > || std::same_as < CharType, wchar_t >)
	std::basic_ostream < CharType, Traits > &write_affiliations(
		std::basic_ostream < CharType, Traits > &output,
		const organizational_analysis::activities < CharType, Traits > &activities,
		const organizational_analysis::affiliations < CharType, Traits > &affiliations,
		const organizational_analysis::components < InterfacesAreReciprocal, CharType, Traits > &components,
		const CharType separator)
	{
		if constexpr (std::same_as < CharType, char >)
		{
			output << "Descriptions" << separator;
		}

		else
		{
			output << L"Descriptions" << separator;
		}

		for (const auto &[identification, description] : components.descriptions)
		{
			output << separator;
			output << description.name;
		}

		output << CharType{'\n'} << separator;

		if constexpr (std::same_as < CharType, char >)
		{
			output << "Activity by Component";
		}

		else
		{
			output << L"Activity by Component";
		}

		for (const auto &[identification, description] : components.descriptions)
		{
			output << separator;
			output << identification;
		}

		output << CharType{'\n'};

		for (const auto &[identification1, description1] : affiliations.responsabilities)
		{
			output << activities.descriptions.at(identification1).name;
			output << separator;

			output << identification1;
			output << separator;

			auto iterator = description1.cbegin();

			for (const auto &[identification2, description2] : components.descriptions)
			{
				if (iterator != description1.cend() && iterator->first == identification2)
				{
					output << iterator->second;

					++iterator;
				}

				output << separator;
			}

			output << CharType{'\n'};
		}

		return output;
	}

	template <
		typename CharType,
		typename Traits,
		xablau::algebra::concepts::xablau_matrix MatrixType >
	requires (std::same_as < CharType, char > || std::same_as < CharType, wchar_t >)
	std::basic_ostream < CharType, Traits > &write_matrix(
		std::basic_ostream < CharType, Traits > &output,
		const MatrixType &matrix,
		const CharType separator)
	requires (std::same_as < typename MatrixType::value_type, float >)
	{
		for (size_t i = 0; i < matrix.dimensionalities()[0]; i++)
		{
			for (size_t j = 0; j < matrix.dimensionalities()[1]; j++)
			{
				if (matrix(i, j) != float{})
				{
					output << matrix(i, j);
				}

				output << separator;
			}

			output << CharType{'\n'};
		}

		return output;
	}

	template <
		xablau::algebra::concepts::xablau_matrix MatrixType,
		typename CharType,
		typename Traits >
	requires (std::same_as < CharType, char > || std::same_as < CharType, wchar_t >)
	std::basic_ostream < CharType, Traits > &write_report(
		std::basic_ostream < CharType, Traits > &output,
		const MatrixType &comparativeMatrix,
		const std::map < size_t, std::basic_string < CharType, Traits > > &baseIndexToKeyMap)
	{
		std::array < std::vector < size_t >, 3 > interactions =
		{
			std::vector < size_t > (comparativeMatrix.dimensionalities()[0], size_t{}),
			std::vector < size_t > (comparativeMatrix.dimensionalities()[0], size_t{}),
			std::vector < size_t > (comparativeMatrix.dimensionalities()[0], size_t{})
		};

		for (size_t i = 0; i < comparativeMatrix.dimensionalities()[0]; i++)
		{
			for (size_t j = 0; j < comparativeMatrix.dimensionalities()[1]; j++)
			{
				const auto cell1 = comparativeMatrix(i, j);

				if (cell1 == organizational_analysis::indirectly_related)
				{
					interactions[0][i]++;
				}

				else if (cell1 == organizational_analysis::related)
				{
					interactions[1][i]++;
				}

				else if (cell1 == organizational_analysis::directly_related)
				{
					interactions[2][i]++;
				}

				const auto cell2 = comparativeMatrix(j, i);

				if (cell2 == organizational_analysis::indirectly_related)
				{
					interactions[0][i]++;
				}

				else if (cell2 == organizational_analysis::related)
				{
					interactions[1][i]++;
				}

				else if (cell2 == organizational_analysis::directly_related)
				{
					interactions[2][i]++;
				}
			}

			if constexpr (std::same_as < CharType, char >)
			{
				output <<
					"Element " << baseIndexToKeyMap.at(i) << "\n" <<
					"Attended interactions: " << interactions[1][i] << "\n" <<
					"Unexpected interactions: " << interactions[2][i] << "\n" <<
					"Unattended interactions: " << interactions[0][i] << "\n\n";
			}

			else
			{
				output <<
					L"Element " << baseIndexToKeyMap.at(i) << L"\n" <<
					L"Attended interactions: " << interactions[1][i] << L"\n" <<
					L"Unexpected interactions: " << interactions[2][i] << L"\n" <<
					L"Unattended interactions: " << interactions[0][i] << L"\n\n";
			}
		}

		if constexpr (std::same_as < CharType, char >)
		{
			output <<
				"Total of attended interactions: " << std::accumulate(interactions[1].cbegin(), interactions[1].cend(), size_t{}) << "\n" <<
				"Total of unexpected interactions: " << std::accumulate(interactions[2].cbegin(), interactions[2].cend(), size_t{}) << "\n" <<
				"Total of unattended interactions: " << std::accumulate(interactions[0].cbegin(), interactions[0].cend(), size_t{}) << "\n\n";
		}

		else
		{
			output <<
				L"Total of attended interactions: " << std::accumulate(interactions[1].cbegin(), interactions[1].cend(), size_t{}) << L"\n" <<
				L"Total of unexpected interactions: " << std::accumulate(interactions[2].cbegin(), interactions[2].cend(), size_t{}) << L"\n" <<
				L"Total of unattended interactions: " << std::accumulate(interactions[0].cbegin(), interactions[0].cend(), size_t{}) << L"\n\n";
		}

		return output;
	}
}