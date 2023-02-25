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

import xablau.computer_graphics;
import xablau.geometry;
import xablau.machine_learning;

import <filesystem>;
import <format>;
import <iostream>;

using Type = float;

std::pair <
	std::vector < xablau::geometry::vertex < Type, xablau::geometry::spatial_dimension < 3 > > >,
	std::vector < std::pair < size_t, size_t > > > read_sphere(const std::filesystem::path &sphere)
{
	xablau::computer_graphics::object_3d < Type > object;

	object.read_OFF_format(sphere, "sphere");

	std::vector < std::pair < size_t, size_t > > connections;

	for (const auto &face : object.faces(0))
	{
		for (size_t i = 0; i < face.size(); i++)
		{
			connections.push_back(std::make_pair(face[i], face[(i + 1) % face.size()]));
		}
	}

	return std::make_pair(object.group(0), std::move(connections));
}

void read_inputs(
	const char *baseDirectory,
	const std::filesystem::path &output,
	xablau::machine_learning::boundary_descriptor_classifier < Type > &descriptorClassifier,
	const size_t neighborRadiusLimit,
	const bool generateReducedObject,
	const size_t unitsInTheLastPlace)
{
	const std::filesystem::path dataset{baseDirectory};

	for (const auto &directoryEntry : std::filesystem::directory_iterator{dataset}) 
	{
		auto model = directoryEntry.path();
		const auto categoryName = std::filesystem::relative(model, dataset);

		xablau::computer_graphics::object_3d < Type > object(categoryName.string());

		model.append("train/");

		for (const auto &objectFilename : std::filesystem::directory_iterator{model})
		{
			std::cout << objectFilename << "\n";

			object.read_OFF_format(
				objectFilename,
				std::filesystem::relative(objectFilename, model).replace_extension("").string());
		}

		object.erase_orphan_vertices();

		if (generateReducedObject)
		{
			const auto reducedObjects =
				descriptorClassifier.insert_samples < true > (object, neighborRadiusLimit, unitsInTheLastPlace).split_groups_into_objects();

			for (const auto &reducedObject : reducedObjects)
			{
				reducedObject.write_Wavefront_format < true > (output);
			}
		}

		else
		{
			descriptorClassifier.insert_samples < false > (object, neighborRadiusLimit, unitsInTheLastPlace);
		}

		std::cout << "\n";
	}
}

template < typename ResultType >
void write_reports_full_report(
	const ResultType &result,
	const std::string &evaluatedSampleName,
	const std::filesystem::path &outputResults,
	const char separator)
{
	for (const auto &[categoryName, labeledSampleDistanceVector] : result.full_result)
	{
		xablau::io::fstream < char > report(
			outputResults.string() + evaluatedSampleName + "_" + categoryName + "_full_report.csv",
			std::ios_base::out | std::ios_base::trunc);

		report.write < false > ("Sample Name");
		report.write(separator);
		report.write < false > ("Outer Eigenvalues Distance");
		report.write(separator);
		report.write < false > ("Complete Eigenvalues Distance");
		report.write('\n');

		for (const auto &[sampleName, outerEigenvaluesDistance, completeEigenvaluesDistance] : labeledSampleDistanceVector)
		{
			const auto formattedLine =
				std::format(
					"{}{}{:.{}f}{}{:.{}f}\n",
					sampleName, separator,
					outerEigenvaluesDistance, std::numeric_limits < Type > ::max_digits10, separator,
					completeEigenvaluesDistance, std::numeric_limits < Type > ::max_digits10);

			report.write < false > (formattedLine);
		}
	}
}

template < typename ResultType >
void write_reports_best_matches(
	const ResultType &result,
	const std::string &evaluatedSampleName,
	const std::filesystem::path &outputResults,
	const char separator)
{
	typename decltype(result.best_outer_eigenvalue_matches_by_category)::const_iterator iterator1 =
		result.best_outer_eigenvalue_matches_by_category.cbegin();

	typename decltype(result.best_complete_eigenvalue_matches_by_category)::const_iterator iterator2 =
		result.best_complete_eigenvalue_matches_by_category.cbegin();

	typename decltype(result.average_outer_eigenvalue_matches_by_category)::const_iterator iterator3 =
		result.average_outer_eigenvalue_matches_by_category.cbegin();

	typename decltype(result.average_complete_eigenvalue_matches_by_category)::const_iterator iterator4 =
		result.average_complete_eigenvalue_matches_by_category.cbegin();

	xablau::io::fstream < char > report(
		outputResults.string() + evaluatedSampleName + "_best_matches.csv",
		std::ios_base::out | std::ios_base::trunc);

	report.write < false > ("Category");
	report.write(separator);
	report.write < false > ("Best Outer Eigenvalue (BOE) Distance");
	report.write(separator);
	report.write < false > ("Best BOE Sample Match");
	report.write(separator);
	report.write < false > ("Best Complete Eigenvalue (BCE) Distance");
	report.write(separator);
	report.write < false > ("Best BCE Sample Match");
	report.write(separator);
	report.write < false > ("Average Outer Eigenvalue (AOE) Distance");
	report.write(separator);
	report.write < false > ("Average Complete Eigenvalue (ACE) Distance");
	report.write('\n');

	constexpr auto sampleMatchOuter =
		[] (const auto &labeledSampleDistance1, const auto &labeledSampleDistance2) -> bool
		{
			return
				labeledSampleDistance1.outer_eigenvalues_distance <
				labeledSampleDistance2.outer_eigenvalues_distance;
		};

	constexpr auto sampleMatchComplete =
		[] (const auto &labeledSampleDistance1, const auto &labeledSampleDistance2) -> bool
		{
			return
				labeledSampleDistance1.complete_eigenvalues_distance <
				labeledSampleDistance2.complete_eigenvalues_distance;
		};

	while (
		iterator1 != result.best_outer_eigenvalue_matches_by_category.cend() &&
		iterator2 != result.best_complete_eigenvalue_matches_by_category.cend() &&
		iterator3 != result.average_outer_eigenvalue_matches_by_category.cend() &&
		iterator4 != result.average_complete_eigenvalue_matches_by_category.cend())
	{
		const auto &labeledSampleDistances = result.full_result.at(iterator1->first);

		const auto formattedLine =
			std::format(
				"{}{}{:.{}f}{}{}{}{:.{}f}{}{}{}{:.{}f}{}{:.{}f}\n",
				iterator1->first, separator,

				iterator1->second, std::numeric_limits < Type > ::max_digits10, separator,
				std::min_element(labeledSampleDistances.cbegin(), labeledSampleDistances.cend(), sampleMatchOuter)->sample_name, separator,

				iterator2->second, std::numeric_limits < Type > ::max_digits10, separator,
				std::min_element(labeledSampleDistances.cbegin(), labeledSampleDistances.cend(), sampleMatchComplete)->sample_name, separator,

				iterator3->second, std::numeric_limits < Type > ::max_digits10, separator,

				iterator4->second, std::numeric_limits < Type > ::max_digits10);

		report.write < false > (formattedLine);

		++iterator1;
		++iterator2;
		++iterator3;
		++iterator4;
	}
}

template < typename ResultType >
void write_reports(
	const ResultType &result,
	const std::string &evaluatedSampleName,
	const std::filesystem::path &outputResults,
	const char separator)
{
	write_reports_full_report(result, evaluatedSampleName, outputResults, separator);
	write_reports_best_matches(result, evaluatedSampleName, outputResults, separator);
}

template < typename ResultsType >
std::array < std::pair < Type, Type >, 4 > write_category_success_rate(
	const ResultsType &results,
	const std::string &categoryName,
	const std::vector < std::string > &sampleNames,
	const size_t nFirstSuccess,
	std::vector < std::array < size_t, 4 > > &rankings,
	const std::filesystem::path &outputSuccessRate,
	const char separator)
{
	const std::string success = "SUCCESS";
	const std::string fail = "FAILURE";
	std::array < std::pair < Type, Type >, 4 > counters{};
	std::vector < std::array < size_t, 4 > > categoryRankings(sampleNames.size());
	std::map < std::string, std::array < std::pair < std::string, std::string >, 4 > > table;

	for (size_t i = 0; i < sampleNames.size(); i++)
	{
		const auto &categoryToValue1 = results[i].best_outer_eigenvalue_matches_by_category;
		const auto &valueToCategory1 = results[i].best_outer_eigenvalue_matches_by_value;
		const auto &valuePosition1 = valueToCategory1.find(categoryToValue1.at(categoryName));
		const auto distance1 = static_cast < size_t > (std::distance(valueToCategory1.cbegin(), valuePosition1));

		const auto &categoryToValue2 = results[i].best_complete_eigenvalue_matches_by_category;
		const auto &valueToCategory2 = results[i].best_complete_eigenvalue_matches_by_value;
		const auto &valuePosition2 = valueToCategory2.find(categoryToValue2.at(categoryName));
		const auto distance2 = static_cast < size_t > (std::distance(valueToCategory2.cbegin(), valuePosition2));

		const auto &categoryToValue3 = results[i].average_outer_eigenvalue_matches_by_category;
		const auto &valueToCategory3 = results[i].average_outer_eigenvalue_matches_by_value;
		const auto &valuePosition3 = valueToCategory3.find(categoryToValue3.at(categoryName));
		const auto distance3 = static_cast < size_t > (std::distance(valueToCategory3.cbegin(), valuePosition3));

		const auto &categoryToValue4 = results[i].average_complete_eigenvalue_matches_by_category;
		const auto &valueToCategory4 = results[i].average_complete_eigenvalue_matches_by_value;
		const auto &valuePosition4 = valueToCategory4.find(categoryToValue4.at(categoryName));
		const auto distance4 = static_cast < size_t > (std::distance(valueToCategory4.cbegin(), valuePosition4));

		categoryRankings[i][0] = static_cast < size_t > (distance1);
		categoryRankings[i][1] = static_cast < size_t > (distance2);
		categoryRankings[i][2] = static_cast < size_t > (distance3);
		categoryRankings[i][3] = static_cast < size_t > (distance4);

		rankings[static_cast < size_t > (distance1)][0]++;
		rankings[static_cast < size_t > (distance2)][1]++;
		rankings[static_cast < size_t > (distance3)][2]++;
		rankings[static_cast < size_t > (distance4)][3]++;

		counters[0].first += static_cast < Type > (distance1 <= nFirstSuccess);
		counters[1].first += static_cast < Type > (distance2 <= nFirstSuccess);
		counters[2].first += static_cast < Type > (distance3 <= nFirstSuccess);
		counters[3].first += static_cast < Type > (distance4 <= nFirstSuccess);

		counters[0].second += static_cast < Type > (distance1 > nFirstSuccess);
		counters[1].second += static_cast < Type > (distance2 > nFirstSuccess);
		counters[2].second += static_cast < Type > (distance3 > nFirstSuccess);
		counters[3].second += static_cast < Type > (distance4 > nFirstSuccess);

		table.emplace(
			sampleNames[i],
			std::to_array({
				std::make_pair(distance1 <= nFirstSuccess ? success : fail, std::to_string(distance1)),
				std::make_pair(distance2 <= nFirstSuccess ? success : fail, std::to_string(distance2)),
				std::make_pair(distance3 <= nFirstSuccess ? success : fail, std::to_string(distance3)),
				std::make_pair(distance4 <= nFirstSuccess ? success : fail, std::to_string(distance4))}));
	}

	xablau::io::fstream < char > report(
		outputSuccessRate.string() + "category_" + categoryName + "_success_rate.csv",
		std::ios_base::out | std::ios_base::trunc);

	report.write < false > ("Sample");
	report.write(separator);
	report.write < false > ("Best Outer Eigenvalue Method (BOE)");
	report.write(separator);
	report.write < false > ("BOE Ranking");
	report.write(separator);
	report.write < false > ("Best Complete Eigenvalue Method (BCE)");
	report.write(separator);
	report.write < false > ("BCE Ranking");
	report.write(separator);
	report.write < false > ("Average Outer Eigenvalue Method (AOE)");
	report.write(separator);
	report.write < false > ("AOE Ranking");
	report.write(separator);
	report.write < false > ("Average Complete Eigenvalue Method (ACE)");
	report.write(separator);
	report.write < false > ("ACE Ranking");
	report.write('\n');

	for (const auto &line : table)
	{
		report.write < false > (line.first);
		report.write(separator);

		for (const auto &pair : line.second)
		{
			report.write < false > (pair.first);
			report.write(separator);
			report.write < false > (pair.second);
			report.write(separator);
		}

		report.write('\n');
	}

	report.write('\n');

	report.write < false > ("Success Rate (BOE Distance):");
	report.write(separator);
	report.write < false > (std::to_string(counters[0].first / (counters[0].first + counters[0].second)));
	report.write('\n');

	report.write < false > ("Success Rate (BCE Distance):");
	report.write(separator);
	report.write < false > (std::to_string(counters[1].first / (counters[1].first + counters[1].second)));
	report.write('\n');

	report.write < false > ("Success Rate (AOE Distance):");
	report.write(separator);
	report.write < false > (std::to_string(counters[2].first / (counters[2].first + counters[2].second)));
	report.write('\n');

	report.write < false > ("Success Rate (ACE Distance):");
	report.write(separator);
	report.write < false > (std::to_string(counters[3].first / (counters[3].first + counters[3].second)));
	report.write('\n');
	report.write('\n');

	report.write < false > ("Ranking Count");
	report.write(separator);
	report.write < false > ("BOE");
	report.write(separator);
	report.write < false > ("BCE");
	report.write(separator);
	report.write < false > ("AOE");
	report.write(separator);
	report.write < false > ("ACE");
	report.write('\n');

	for (size_t i = 0; i < rankings.size(); i++)
	{
		std::array < size_t, 4 > rankingCount{};

		report.write < false > (std::to_string(i));
		report.write(separator);

		for (const auto &categoryRanking : categoryRankings)
		{
			rankingCount[0] += static_cast < size_t > (categoryRanking[0] == i);
			rankingCount[1] += static_cast < size_t > (categoryRanking[1] == i);
			rankingCount[2] += static_cast < size_t > (categoryRanking[2] == i);
			rankingCount[3] += static_cast < size_t > (categoryRanking[3] == i);
		}

		for (size_t j = 0; j < 4; j++)
		{
			report.write < false > (std::to_string(rankingCount[j]));
			report.write(separator);
		}

		report.write('\n');
	}

	return counters;
}

void write_total_success_rate(
	const std::vector < std::array < std::pair < Type, Type >, 4 > > &resultSuccessesFailures,
	const std::vector < std::array < size_t, 4 > > &rankings,
	const std::vector < std::string > &categoryNames,
	const std::filesystem::path &outputSuccessRate,
	const char separator)
{
	xablau::io::fstream < char > report(
		outputSuccessRate.string() + "total_success_rate.csv",
		std::ios_base::out | std::ios_base::trunc);

	report.write < false > ("Category");
	report.write(separator);
	report.write < false > ("Best Outer Eigenvalue Successes");
	report.write(separator);
	report.write < false > ("Best Outer Eigenvalue Failures");
	report.write(separator);
	report.write < false > ("Best Outer Eigenvalue Success Ratio");
	report.write(separator);
	report.write < false > ("Best Complete Eigenvalue Successes");
	report.write(separator);
	report.write < false > ("Best Complete Eigenvalue Failures");
	report.write(separator);
	report.write < false > ("Best Complete Eigenvalue Success Ratio");
	report.write(separator);
	report.write < false > ("Average Outer Eigenvalue Successes");
	report.write(separator);
	report.write < false > ("Average Outer Eigenvalue Failures");
	report.write(separator);
	report.write < false > ("Average Outer Eigenvalue Success Ratio");
	report.write(separator);
	report.write < false > ("Average Complete Eigenvalue Successes");
	report.write(separator);
	report.write < false > ("Average Complete Eigenvalue Failures");
	report.write(separator);
	report.write < false > ("Average Complete Eigenvalue Success Ratio");
	report.write('\n');

	for (size_t i = 0; i < categoryNames.size(); i++)
	{
		report.write < false > (categoryNames[i]);
		report.write(separator);

		for (size_t j = 0; j < 4; j++)
		{
			report.write < false > (std::to_string(static_cast < size_t > (resultSuccessesFailures[i][j].first)));
			report.write(separator);
			report.write < false > (std::to_string(static_cast < size_t > (resultSuccessesFailures[i][j].second)));
			report.write(separator);
			report.write < false > (std::to_string(resultSuccessesFailures[i][j].first / (resultSuccessesFailures[i][j].first + resultSuccessesFailures[i][j].second)));
			report.write(separator);
		}

		report.write('\n');
	}

	report.write('\n');

	std::array < Type, 4 > successes{};
	std::array < Type, 4 > failures{};

	for (size_t i = 0; i < resultSuccessesFailures.size(); i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			successes[j] += resultSuccessesFailures[i][j].first;
			failures[j] += resultSuccessesFailures[i][j].second;
		}
	}

	report.write < false > ("Total Success Rate (Best Outer Eigenvalue Distance):");
	report.write(separator);
	report.write < false > (std::to_string(successes[0] / (successes[0] + failures[0])));
	report.write('\n');

	report.write < false > ("Total Success Rate (Best Complete Eigenvalue Distance):");
	report.write(separator);
	report.write < false > (std::to_string(successes[1] / (successes[1] + failures[1])));
	report.write('\n');

	report.write < false > ("Total Success Rate (Average Outer Eigenvalue Distance):");
	report.write(separator);
	report.write < false > (std::to_string(successes[2] / (successes[2] + failures[2])));
	report.write('\n');

	report.write < false > ("Total Success Rate (Average Complete Eigenvalue Distance):");
	report.write(separator);
	report.write < false > (std::to_string(successes[3] / (successes[3] + failures[3])));
	report.write('\n');
	report.write('\n');

	report.write < false > ("Ranking");
	report.write(separator);
	report.write < false > ("Count (Best Outer Eigenvalue Distance)");
	report.write(separator);
	report.write < false > ("Count (Best Complete Eigenvalue Distance)");
	report.write(separator);
	report.write < false > ("Count (Average Outer Eigenvalue Distance)");
	report.write(separator);
	report.write < false > ("Count (Average Complete Eigenvalue Distance)");
	report.write('\n');

	for (size_t i = 0; i < rankings.size(); i++)
	{
		report.write < false > (std::to_string(i));
		report.write(separator);

		for (size_t j = 0; j < 4; j++)
		{
			report.write < false > (std::to_string(rankings[i][j]));
			report.write(separator);
		}

		report.write('\n');
	}
}

void validate(
	const char *baseDirectory,
	const std::filesystem::path &output,
	const xablau::machine_learning::boundary_descriptor_classifier < Type > &descriptorClassifier,
	const size_t neighborRadiusLimit,
	const size_t nFirstSuccess,
	const std::filesystem::path &outputResults,
	const std::filesystem::path &outputSuccessRate,
	const char separator,
	const bool generateReducedObject,
	const size_t unitsInTheLastPlace)
{
	std::vector < std::string > categoryNames;
	std::vector < std::array < size_t, 4 > > rankings(descriptorClassifier.category_count());
	std::vector < std::array < std::pair < Type, Type >, 4 > > resultSuccessesFailures;
	const std::filesystem::path dataset{baseDirectory};

	for (const auto &directoryEntry : std::filesystem::directory_iterator{dataset})
	{
		std::vector < std::string > sampleNames;
		auto model = directoryEntry.path();
		const auto categoryName = std::filesystem::relative(model, dataset);

		categoryNames.push_back(categoryName.string());

		xablau::computer_graphics::object_3d < Type > object(categoryName.string());

		model.append("test/");

		for (const auto &objectFilename : std::filesystem::directory_iterator{model})
		{
			std::cout << objectFilename << "\n";

			sampleNames.push_back(
				std::filesystem::relative(objectFilename, model).replace_extension("").string());

			object.read_OFF_format(objectFilename, sampleNames.back());
		}

		object.erase_orphan_vertices();

		std::vector < xablau::machine_learning::boundary_descriptor_classifier < Type > ::result_type > results;

		if (generateReducedObject)
		{
			auto [_results, reducedObjects] = descriptorClassifier.evaluate < true > (object, neighborRadiusLimit, unitsInTheLastPlace);
			const auto splitedReducedObjects = reducedObjects.split_groups_into_objects();

			for (const auto &reducedObject : splitedReducedObjects)
			{
				reducedObject.write_Wavefront_format < true > (output);
			}

			results = std::move(_results);
		}

		else
		{
			results = descriptorClassifier.evaluate < false > (object, neighborRadiusLimit, unitsInTheLastPlace);
		}

		for (size_t i = 0; i < results.size(); i++)
		{
			write_reports(
				results[i],
				sampleNames[i],
				outputResults,
				separator);
		}

		resultSuccessesFailures.push_back(
			write_category_success_rate(
				results,
				categoryName.string(),
				sampleNames,
				nFirstSuccess,
				rankings,
				outputSuccessRate,
				separator));

		std::cout << "\n";
	}

	write_total_success_rate(resultSuccessesFailures, rankings, categoryNames, outputSuccessRate, separator);
}

int main(int argc, char **argv)
{
	if (argc < 8)
	{
		return -1;
	}

	constexpr auto unitsInTheLastPlace = static_cast < size_t > (Type{0.0001} / std::numeric_limits < Type > ::epsilon());
	const bool generateReducedObject = argv[7][0] == '1' ? true : false;
	const std::string outputPathString(argv[6]);
	const std::filesystem::path spherePath{argv[2]};
	const std::filesystem::path outputPath{outputPathString};
	const std::filesystem::path outputTraining{ outputPathString + "/train/"};
	const std::filesystem::path outputEvaluation{ outputPathString + "/test/"};
	const std::filesystem::path outputResults{ outputPathString + "/results/"};
	const std::filesystem::path outputSuccessRate{ outputPathString + "/success_rate/"};

	const auto [sphere, connections] = read_sphere(spherePath);

	xablau::machine_learning::boundary_descriptor_classifier < Type > descriptorClassifier(
		spherePath.lexically_normal().replace_extension("").string(),
		sphere,
		connections,
		unitsInTheLastPlace);

	std::filesystem::remove_all(outputPath);

	if (!std::filesystem::create_directory(outputPath) ||
		!std::filesystem::create_directory(outputResults) ||
		!std::filesystem::create_directory(outputSuccessRate))
	{
		return -2;
	}

	if (generateReducedObject && (
		!std::filesystem::create_directory(outputTraining) ||
		!std::filesystem::create_directory(outputEvaluation)))
	{
		return -2;
	}

	const char separator = std::strcmp(argv[5], "\\t") == 0 ? '\t' : argv[5][0];

	read_inputs(
		argv[1],
		outputTraining,
		descriptorClassifier,
		std::stoll(argv[3]),
		generateReducedObject,
		unitsInTheLastPlace);

	validate(
		argv[1],
		outputEvaluation,
		descriptorClassifier,
		std::stoll(argv[3]),
		std::stoll(argv[4]),
		outputResults,
		outputSuccessRate,
		separator,
		generateReducedObject,
		unitsInTheLastPlace);

	xablau::io::fstream < char > inferno(
		spherePath.string() + ".txt",
		std::ios_base::out | std::ios_base::trunc);

	inferno <<
		"It took " <<
		std::chrono::duration_cast < std::chrono::milliseconds > (descriptorClassifier.timer_samples_insertion()).count() <<
		" milliseconds to add every sample.\n" <<

		"It took " <<
		std::chrono::duration_cast < std::chrono::milliseconds > (descriptorClassifier.timer_evaluation()).count() <<
		" milliseconds to evaluate every test case.\n";

	return 0;
}