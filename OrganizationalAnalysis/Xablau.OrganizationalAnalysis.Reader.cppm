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

export module xablau.organizational_analysis:reader;
export import :fundamental_definitions;

export import <iostream>;
export import <istream>;
export import <format>;
export import <locale>;
export import <regex>;

export namespace xablau::organizational_analysis::reader
{
	namespace internals
	{
		constexpr size_t max_line_length = 2048;

		template < typename CharType >
		consteval auto comment_expression()
		{
			if constexpr (std::same_as < CharType, char >)
			{
				return "^[\\s]*\\/\\/";
			}

			else
			{
				return L"^[\\s]*\\/\\/";
			}
		}

		template < typename CharType >
		consteval auto relation_expression()
		{
			if constexpr (std::same_as < CharType, char >)
			{
				return "^(?!\t)\\s*(.*[^\\s])\\s*<=>\\s*(.*(?![\\s|\\0]).)";
			}

			else
			{
				return L"^(?!\t)\\s*(.*[^\\s])\\s*<=>\\s*(.*(?![\\s|\\0]).)";
			}
		}

		template < typename CharType >
		consteval auto role_expression()
		{
			return relation_expression < CharType > ();
		}

		template < typename CharType >
		consteval auto activity_expression()
		{
			return relation_expression < CharType > ();
		}

		template < typename CharType >
		consteval auto component_expression()
		{
			return relation_expression < CharType > ();
		}

		template < typename CharType >
		consteval auto single_field_expression()
		{
			if constexpr (std::same_as < CharType, char >)
			{
				return "^(?!\t)\\s*(.*(?![\\s|\\0]).)";
			}

			else
			{
				return L"^(?!\t)\\s*(.*(?![\\s|\\0]).)";
			}
		}

		template < typename CharType >
		consteval auto tabbed_single_field_expression()
		{
			if constexpr (std::same_as < CharType, char >)
			{
				return "^(?!\t\t)\t\\s*(.*(?![\\s|\\0]).)";
			}

			else
			{
				return L"^(?!\t\t)\t\\s*(.*(?![\\s|\\0]).)";
			}
		}

		template < typename CharType >
		consteval auto double_tabbed_single_field_expression()
		{
			if constexpr (std::same_as < CharType, char >)
			{
				return "^(?!\t\t\t)\t\t\\s*(.*(?![\\s|\\0]).)";
			}

			else
			{
				return L"^(?!\t\t\t)\t\t\\s*(.*(?![\\s|\\0]).)";
			}
		}

		template < typename CharType >
		consteval auto group_expression()
		{
			return single_field_expression < CharType > ();
		}

		template < typename CharType >
		consteval auto component_interaction_expression()
		{
			return tabbed_single_field_expression < CharType > ();
		}

		template < typename CharType >
		consteval auto agent_in_charge_expression()
		{
			return tabbed_single_field_expression < CharType > ();
		}

		template < typename CharType >
		consteval auto dependency_expression()
		{
			return double_tabbed_single_field_expression < CharType > ();
		}

		template < typename CharType >
		consteval auto affiliation_activity_expression()
		{
			return relation_expression < CharType > ();
		}

		template < typename CharType >
		consteval auto affiliation_component_weight_expression()
		{
			if constexpr (std::same_as < CharType, char >)
			{
				return "\t\\s*(.*[^\\s])\\s*,\\s*(.*(?![\\s|\\0]).)";
			}

			else
			{
				return L"\t\\s*(.*[^\\s])\\s*,\\s*(.*(?![\\s|\\0]).)";
			}
		}
	}

	template < typename CharType, typename Traits >
	requires (std::same_as < CharType, char > || std::same_as < CharType, wchar_t >)
	std::basic_istream < CharType, Traits > &read_agents(
		std::basic_istream < CharType, Traits > &input,
		organizational_analysis::agents < CharType, Traits > &agents,
		std::basic_ostream < CharType, Traits > &errorOutput = internals::default_error_output < CharType > (),
		const std::locale &locale = std::locale(""))
	{
		size_t lineCounter{};
		std::basic_string < CharType, Traits > group;
		std::basic_regex < CharType > commentExpression;
		std::basic_regex < CharType > groupExpression;
		std::basic_regex < CharType > roleExpression;

		commentExpression.imbue(locale);
		groupExpression.imbue(locale);
		roleExpression.imbue(locale);

		commentExpression.assign(internals::comment_expression < CharType > ());
		groupExpression.assign(internals::group_expression < CharType > ());
		roleExpression.assign(internals::role_expression < CharType > ());

		while (input.good())
		{
			std::basic_string < CharType, Traits > line;
			std::match_results < typename std::basic_string < CharType, Traits > ::const_iterator > match;

			line.resize(internals::max_line_length);

			input.getline(line.data(), internals::max_line_length);

			lineCounter++;

			if constexpr (std::same_as < CharType, char >)
			{
				if (std::strlen(line.c_str()) == 0)
				{
					continue;
				}
			}

			else
			{
				if (std::wcslen(line.c_str()) == 0)
				{
					continue;
				}
			}

			if (std::regex_search(line, match, commentExpression))
			{
				continue;
			}

			if (std::regex_search(line, match, roleExpression))
			{
				std::basic_string < CharType, Traits > agent = match[1].str();
				std::basic_string < CharType, Traits > role = match[2].str();

				const auto iterator =
					agents.descriptions.insert(
						std::make_pair(
							std::move(agent),
							organizational_analysis::agents < CharType, Traits > ::description())).first;

				iterator->second.role = std::move(role);
				iterator->second.group = group;
			}

			else if (std::regex_search(line, match, groupExpression))
			{
				group = match[0].str();
			}

			else if (!line.empty())
			{
				if constexpr (std::same_as < CharType, char >)
				{
					errorOutput << std::format("Unknown agent at line {}: \"{}\"\n", lineCounter, line);
				}

				else
				{
					errorOutput << std::format(L"Unknown agent at line {}: \"{}\"\n", lineCounter, line);
				}
			}
		}

		return input;
	}

	template < typename CharType, typename Traits >
	requires (std::same_as < CharType, char > || std::same_as < CharType, wchar_t >)
	std::basic_istream < CharType, Traits > &read_activities(
		std::basic_istream < CharType, Traits > &input,
		organizational_analysis::activities < CharType, Traits > &activities,
		const organizational_analysis::agents < CharType, Traits > &agents,
		std::basic_ostream < CharType, Traits > &errorOutput = internals::default_error_output < CharType > (),
		const std::locale &locale = std::locale(""))
	{
		size_t lineCounter{};
		std::basic_string < CharType, Traits > activity;
		std::basic_string < CharType, Traits > group;
		std::basic_regex < CharType > commentExpression;
		std::basic_regex < CharType > groupExpression;
		std::basic_regex < CharType > activityExpression;
		std::basic_regex < CharType > agentInChargeExpression;
		std::basic_regex < CharType > dependencyExpression;

		commentExpression.imbue(locale);
		groupExpression.imbue(locale);
		activityExpression.imbue(locale);
		agentInChargeExpression.imbue(locale);
		dependencyExpression.imbue(locale);

		commentExpression.assign(internals::comment_expression < CharType > ());
		groupExpression.assign(internals::group_expression < CharType > ());
		activityExpression.assign(internals::activity_expression < CharType > ());
		agentInChargeExpression.assign(internals::agent_in_charge_expression < CharType > ());
		dependencyExpression.assign(internals::dependency_expression < CharType > ());

		while (input.good())
		{
			std::basic_string < CharType, Traits > line;
			std::match_results < typename std::basic_string < CharType, Traits > ::const_iterator > match;

			line.resize(internals::max_line_length);

			input.getline(line.data(), internals::max_line_length);

			lineCounter++;

			if constexpr (std::same_as < CharType, char >)
			{
				if (std::strlen(line.c_str()) == 0)
				{
					continue;
				}
			}

			else
			{
				if (std::wcslen(line.c_str()) == 0)
				{
					continue;
				}
			}

			if (std::regex_search(line, match, commentExpression))
			{
				continue;
			}

			if (std::regex_search(line, match, activityExpression))
			{
				activity = match[1].str();
				std::basic_string < CharType, Traits > name = match[2].str();

				const auto iterator =
					activities.descriptions.insert(
						std::make_pair(
							activity,
							organizational_analysis::activities < CharType, Traits > ::description())).first;

				iterator->second.name = std::move(name);
				iterator->second.group = group;
			}

			else if (std::regex_search(line, match, agentInChargeExpression))
			{
				std::basic_string < CharType, Traits > agentInCharge = match[1].str();

				if (!agents.descriptions.contains(agentInCharge))
				{
					if constexpr (std::same_as < CharType, char >)
					{
						throw std::runtime_error(std::format("Could not find agent \"{}\".", agentInCharge));
					}

					else
					{
						throw std::runtime_error("Could not find an agent.");
					}
				}

				activities.descriptions.at(activity).agents_in_charge.insert(std::move(agentInCharge));
			}

			else if (std::regex_search(line, match, dependencyExpression))
			{
				std::basic_string < CharType, Traits > dependency = match[1].str();

				if (activity == dependency)
				{
					if constexpr (std::same_as < CharType, char >)
					{
						throw std::runtime_error(std::format("Activity \"{}\" is depending on itself.", activity));
					}

					else
					{
						throw std::runtime_error("An activity is depending on itself.");
					}
				}

				activities.dependencies.insert(activity, dependency);
			}

			else if (std::regex_search(line, match, groupExpression))
			{
				group = match[1].str();
			}

			else if (!line.empty())
			{
				if constexpr (std::same_as < CharType, char >)
				{
					errorOutput << std::format("Unknown activity at line {}: \"{}\"\n", lineCounter, line);
				}

				else
				{
					errorOutput << std::format(L"Unknown activity at line {}: \"{}\"\n", lineCounter, line);
				}
			}
		}

		for (const auto &[identification, description] : activities.descriptions)
		{
			activities.dependencies.insert(identification);
		}

		for (const auto &dependency : activities.dependencies.container())
		{
			if (!activities.descriptions.contains(dependency.first.value))
			{
				if constexpr (std::same_as < CharType, char >)
				{
					throw
						std::runtime_error(
							std::format(
								"Dependency \"{}\" does not have a description.",
								dependency.first.value));
				}

				else
				{
					throw std::runtime_error("A dependency does not have a description.");
				}
			}
		}

		for (const auto &[_activity, description] : activities.descriptions)
		{
			if (description.agents_in_charge.empty())
			{
				if constexpr (std::same_as < CharType, char >)
				{
					throw
						std::runtime_error(
							std::format(
								"Activity \"{}\" does not have an agent in charge.",
								_activity));
				}

				else
				{
					throw std::runtime_error("An activity does not have an agent in charge.");
				}
			}
		}

		return input;
	}

	template < typename CharType, typename Traits >
	requires (std::same_as < CharType, char > || std::same_as < CharType, wchar_t >)
	std::basic_istream < CharType, Traits > &read_components(
		std::basic_istream < CharType, Traits > &input,
		organizational_analysis::components < CharType, Traits > &components,
		std::basic_ostream < CharType, Traits > &errorOutput = internals::default_error_output < CharType > (),
		const std::locale &locale = std::locale(""))
	{
		size_t lineCounter{};
		std::basic_string < CharType, Traits > component;
		std::basic_string < CharType, Traits > group;
		std::basic_regex < CharType > commentExpression;
		std::basic_regex < CharType > groupExpression;
		std::basic_regex < CharType > componentExpression;
		std::basic_regex < CharType > interactionExpression;

		commentExpression.imbue(locale);
		groupExpression.imbue(locale);
		componentExpression.imbue(locale);
		interactionExpression.imbue(locale);

		commentExpression.assign(internals::comment_expression < CharType > ());
		groupExpression.assign(internals::group_expression < CharType > ());
		componentExpression.assign(internals::component_expression < CharType > ());
		interactionExpression.assign(internals::component_interaction_expression < CharType > ());

		while (input.good())
		{
			std::basic_string < CharType, Traits > line;
			std::match_results < typename std::basic_string < CharType, Traits > ::const_iterator > match;

			line.resize(internals::max_line_length);

			input.getline(line.data(), internals::max_line_length);

			lineCounter++;

			if constexpr (std::same_as < CharType, char >)
			{
				if (std::strlen(line.c_str()) == 0)
				{
					continue;
				}
			}

			else
			{
				if (std::wcslen(line.c_str()) == 0)
				{
					continue;
				}
			}

			if (std::regex_search(line, match, commentExpression))
			{
				continue;
			}

			if (std::regex_search(line, match, componentExpression))
			{
				component = match[1].str();
				std::basic_string < CharType, Traits > name = match[2].str();

				const auto iterator =
					components.descriptions.insert(
						std::make_pair(
							component,
							organizational_analysis::components < CharType, Traits > ::description())).first;

				iterator->second.name = std::move(name);
				iterator->second.group = group;
			}

			else if (std::regex_search(line, match, interactionExpression))
			{
				components.interactions.insert(component, match[1].str());
			}

			else if (std::regex_search(line, match, groupExpression))
			{
				group = match[1].str();
			}

			else if (!line.empty())
			{
				if constexpr (std::same_as < CharType, char >)
				{
					errorOutput << std::format("Unknown component at line {}: \"{}\"\n", lineCounter, line);
				}

				else
				{
					errorOutput << std::format(L"Unknown component at line {}: \"{}\"\n", lineCounter, line);
				}
			}
		}

		return input;
	}

	template < typename CharType, typename Traits >
	requires (std::same_as < CharType, char > || std::same_as < CharType, wchar_t >)
	std::basic_istream < CharType, Traits > &read_affiliations(
		std::basic_istream < CharType, Traits > &input,
		organizational_analysis::affiliations < CharType, Traits > &affiliations,
		const organizational_analysis::activities < CharType, Traits > &activities,
		const organizational_analysis::components < CharType, Traits > &components,
		std::basic_ostream < CharType, Traits > &errorOutput = internals::default_error_output < CharType > (),
		const std::locale &locale = std::locale(""))
	{
		size_t lineCounter{};
		std::basic_string < CharType, Traits > activity;
		std::basic_regex < CharType > commentExpression;
		std::basic_regex < CharType > affiliationActivityExpression;
		std::basic_regex < CharType > affiliationComponentWeightExpression;

		commentExpression.imbue(locale);
		affiliationActivityExpression.imbue(locale);
		affiliationComponentWeightExpression.imbue(locale);

		commentExpression.assign(internals::comment_expression < CharType > ());
		affiliationActivityExpression.assign(internals::affiliation_activity_expression < CharType > ());
		affiliationComponentWeightExpression.assign(internals::affiliation_component_weight_expression < CharType > ());

		while (input.good())
		{
			std::basic_string < CharType, Traits > line;
			std::match_results < typename std::basic_string < CharType, Traits > ::const_iterator > match;

			line.resize(internals::max_line_length);

			input.getline(line.data(), internals::max_line_length);

			lineCounter++;

			if constexpr (std::same_as < CharType, char >)
			{
				if (std::strlen(line.c_str()) == 0)
				{
					continue;
				}
			}

			else
			{
				if (std::wcslen(line.c_str()) == 0)
				{
					continue;
				}
			}

			if (std::regex_search(line, match, commentExpression))
			{
				continue;
			}

			if (std::regex_search(line, match, affiliationActivityExpression))
			{
				activity = match[1].str();

				if (!activities.descriptions.contains(activity))
				{
					if constexpr (std::same_as < CharType, char >)
					{
						throw std::runtime_error(std::format("Could not find activity \"{}\".", activity));
					}

					else
					{
						throw std::runtime_error("Could not find an activity.");
					}
				}

				affiliations.responsabilities.insert(
					std::make_pair(
						activity,
						std::map < std::basic_string < CharType, Traits >, float > ()));
			}

			else if (std::regex_search(line, match, affiliationComponentWeightExpression))
			{
				std::basic_string < CharType, Traits > component = match[1].str();
				float weight = std::stof(match[2].str(), nullptr);

				if (!components.descriptions.contains(component))
				{
					if constexpr (std::same_as < CharType, char >)
					{
						throw std::runtime_error(std::format("Could not find component \"{}\".", component));
					}

					else
					{
						throw std::runtime_error("Could not find a component.");
					}

				}

				if (weight == float{})
				{
					if constexpr (std::same_as < CharType, char >)
					{
						throw
							std::runtime_error(
								std::format(
									"Activity \"{}\" has null connection with component \"{}\".",
									activity,
									component));
					}

					else
					{
						throw std::runtime_error("An activity has a null connection with a component.");
					}
				}

				affiliations.responsabilities.at(activity).emplace(std::move(component), std::move(weight));
			}

			else if (!line.empty())
			{
				if constexpr (std::same_as < CharType, char >)
				{
					errorOutput << std::format("Unknown affliation at line {}: \"{}\"\n", lineCounter, line);
				}

				else
				{
					errorOutput << std::format(L"Unknown affliation at line {}: \"{}\"\n", lineCounter, line);
				}
			}
		}

		for (const auto &[identification, description] : activities.descriptions)
		{
			affiliations.responsabilities.insert(std::make_pair(identification, std::map < std::basic_string < CharType, Traits >, float > ()));
		}

		return input;
	}
}