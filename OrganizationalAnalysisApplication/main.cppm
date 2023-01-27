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

import <clocale>;

import xablau.io;
import xablau.organizational_analysis;

int main(int argc, char** argv)
{
	using CharType = char;
	using Traits = std::char_traits < CharType >;

	try
	{
		if (argc < 25)
		{
			std::cout << "There are not enough parameters.";

			return -1;
		}

		CharType separator{argv[24][0]};

		xablau::organizational_analysis::processor < CharType, Traits > processor{};

		xablau::io::fstream < CharType > agentsInput(argv[1], std::ios_base::in);
		xablau::io::fstream < CharType > activitiesInput(argv[2], std::ios_base::in);
		xablau::io::fstream < CharType > componentsInput(argv[3], std::ios_base::in);
		xablau::io::fstream < CharType > affiliationsInput(argv[4], std::ios_base::in);

		std::cout << "\n--------------------Reading inputs---------------------\n";

		processor.read(agentsInput, activitiesInput, componentsInput, affiliationsInput);

		std::cout << "\n-------------------------------------------------------\n";
		std::cout << "\n\n";
		std::cout << "\n---------------Possible parallelizations---------------\n";

		xablau::io::fstream < CharType > parallelizationsOutput(argv[22], std::ios_base::out | std::ios_base::trunc);
		const auto parallelizations = processor.identify_parallelizations();

		size_t level{};

		for (const auto &parallelization : parallelizations)
		{
			auto iterator = parallelization.cbegin();
			parallelizationsOutput << "Level = { " << level << " }: [ ";

			for (size_t i = 1; i < parallelization.size(); i++)
			{
				parallelizationsOutput << *iterator << ", ";
				++iterator;
			}

			parallelizationsOutput << *iterator << " ]\n";

			level++;
		}

		std::cout << "\n-------------------------------------------------------\n";
		std::cout << "\n\n";
		std::cout << "\n-----------------------Priorities----------------------\n";

		xablau::io::fstream < CharType > prioritiesOutput(argv[23], std::ios_base::out | std::ios_base::trunc);
		const auto priorities = processor.identify_priorities();

		for (const auto &priority : priorities)
		{
			prioritiesOutput << "{ " << priority.first << ", " << priority.second << " }\n";
		}

		std::cout << "\n-------------------------------------------------------\n";
		std::cout << "\n\n";
		std::cout << "\n-----------------------Alignment-----------------------\n";

		bool everyProductHasAnAgentInCharge = false;
		float minimumRelationDegree = std::stof(argv[6], nullptr);

		while (!everyProductHasAnAgentInCharge)
		{
			if (minimumRelationDegree <= 0.0f)
			{
				std::cout << "Minimum relation degree between agent and component is null or negative.\n";

				if (std::atoi(argv[5]) == 1)
				{
					std::cout << "Exiting...\n";

					return -2;
				}

				else
				{
					std::cout << "Bypassing this requirement...\n";

					break;
				}
			}

			processor.attribute_agent_in_charge_for_component(minimumRelationDegree);
			const auto productsWithoutAgentsInCharge = processor.validate_agent_in_charge_for_component();

			if (productsWithoutAgentsInCharge.size() != 0)
			{
				std::cout << "The following components do not have an agent in charge:\n";

				for (const auto &productWithoutAgentsInCharge : productsWithoutAgentsInCharge)
				{
					std::cout << "\t" << productWithoutAgentsInCharge << "\n";
				}

				std::cout << "\n";

				minimumRelationDegree -= 1.0f;

				std::cout << "Trying again with a lesser value: " << minimumRelationDegree << "\n\n";
			}

			else
			{
				everyProductHasAnAgentInCharge = true;
			}
		}

		xablau::io::fstream < CharType > reportWithoutRedundanciesOutput(argv[20], std::ios_base::out | std::ios_base::trunc);
		xablau::io::fstream < CharType > reportWithRedundanciesOutput(argv[21], std::ios_base::out | std::ios_base::trunc);

		if (std::atoi(argv[5]) == 0)
		{
			processor.align_architecture_process_between_component_and_organization(
				minimumRelationDegree,
				reportWithoutRedundanciesOutput,
				reportWithRedundanciesOutput);
		}

		else
		{
			processor.align_architecture_process_between_activity_and_organization(
				minimumRelationDegree,
				reportWithoutRedundanciesOutput,
				reportWithRedundanciesOutput);
		}

		std::cout << "\n-------------------------------------------------------\n";
		std::cout << "\n\n";
		std::cout << "\n-------------------------Saving------------------------\n";

		xablau::io::fstream < CharType > agentsOutput(argv[7], std::ios_base::out | std::ios_base::trunc);
		xablau::io::fstream < CharType > activitiesOutput(argv[8], std::ios_base::out | std::ios_base::trunc);
		xablau::io::fstream < CharType > componentsOutput(argv[9], std::ios_base::out | std::ios_base::trunc);
		xablau::io::fstream < CharType > affiliationsOutput(argv[10], std::ios_base::out | std::ios_base::trunc);

		processor.write(agentsOutput, activitiesOutput, componentsOutput, affiliationsOutput, separator);

		xablau::io::fstream < CharType > activitiesMatrixOutput(argv[11], std::ios_base::out | std::ios_base::trunc);
		xablau::io::fstream < CharType > weakAffiliationsOutput(argv[12], std::ios_base::out | std::ios_base::trunc);
		xablau::io::fstream < CharType > strongAffiliationsOutput(argv[13], std::ios_base::out | std::ios_base::trunc);
		xablau::io::fstream < CharType > componentsMatrixOutput(argv[14], std::ios_base::out | std::ios_base::trunc);
		xablau::io::fstream < CharType > totalPotentialOutput(argv[15], std::ios_base::out | std::ios_base::trunc);
		xablau::io::fstream < CharType > strongTotalPotentialWithoutRedundanciesOutput(argv[16], std::ios_base::out | std::ios_base::trunc);
		xablau::io::fstream < CharType > strongTotalPotentialWithRedundanciesOutput(argv[17], std::ios_base::out | std::ios_base::trunc);
		xablau::io::fstream < CharType > comparativeWithoutRedundanciesOutput(argv[18], std::ios_base::out | std::ios_base::trunc);
		xablau::io::fstream < CharType > comparativeWithRedundanciesOutput(argv[19], std::ios_base::out | std::ios_base::trunc);

		processor.write_activities_matrix(activitiesMatrixOutput, separator);
		processor.write_weak_affiliations_matrix(weakAffiliationsOutput, separator);
		processor.write_strong_affiliations_matrix(strongAffiliationsOutput, separator);
		processor.write_components_matrix(componentsMatrixOutput, separator);
		processor.write_total_potential_matrix(totalPotentialOutput, separator);
		processor.write_strong_potential_matrix_without_redundancies(strongTotalPotentialWithoutRedundanciesOutput, separator);
		processor.write_strong_potential_matrix_with_redundancies(strongTotalPotentialWithRedundanciesOutput, separator);
		processor.write_comparative_matrix_without_redundancies(comparativeWithoutRedundanciesOutput, separator);
		processor.write_comparative_matrix_with_redundancies(comparativeWithRedundanciesOutput, separator);

		std::cout << "\n-------------------------------------------------------\n";
	}

	catch (const std::exception &exception)
	{
		std::cout << "FATAL ERROR: " << exception.what();

		return -3;
	}

	return 0;
}
