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

module;

#include "Xablau.MachineLearning.Eigen.h"

export module xablau.machine_learning:boundary_descriptor_classifier;

export import <array>;
export import <chrono>;
export import <cmath>;
export import <mutex>;
export import <ranges>;
export import <vector>;

export import xablau.algebra;
export import xablau.computer_graphics;
export import xablau.geometry;
export import xablau.graph;

export namespace xablau::machine_learning
{
	template < std::floating_point Type >
	class boundary_descriptor_classifier final
	{
	public:
		struct labeled_sample_distance_type
		{
			std::string sample_name;
			Type outer_eigenvalues_distance;
			Type complete_eigenvalues_distance;
		};

		struct result_type
		{
			std::map < std::string, std::vector < labeled_sample_distance_type > > full_result;

			std::map < std::string, Type > best_outer_eigenvalue_matches_by_category;
			std::map < std::string, Type > best_complete_eigenvalue_matches_by_category;
			std::map < std::string, Type > average_outer_eigenvalue_matches_by_category;
			std::map < std::string, Type > average_complete_eigenvalue_matches_by_category;

			std::map < Type, std::string > best_outer_eigenvalue_matches_by_value;
			std::map < Type, std::string > best_complete_eigenvalue_matches_by_value;
			std::map < Type, std::string > average_outer_eigenvalue_matches_by_value;
			std::map < Type, std::string > average_complete_eigenvalue_matches_by_value;
		};

	private:
		using matrix_type =
			xablau::algebra::tensor_dense_dynamic <
				Type,
				xablau::algebra::tensor_rank < 2 >,
				xablau::algebra::tensor_contiguity < true > >;

		struct labeled_eigenvalues_type
		{
			std::string sample_name;
			std::vector < Type > outer_eigenvalues;
			std::vector < Type > complete_eigenvalues;
		};

		using vector_of_labeled_eigenvalues = std::vector < labeled_eigenvalues_type >;

		std::string _sphere_algorithm_name;

		std::vector < geometry::vertex < Type, geometry::spatial_dimension < 3 > > > _sphere{};

		graph::graph <
			graph::node < size_t >,
			graph::graph_container_type < graph::graph_container_type_value::ordered >,
			graph::edge < Type > > _connections{};

		matrix_type _connections_distance_matrix{};

		std::map < std::string, vector_of_labeled_eigenvalues > _eigenvalues{};

		std::chrono::nanoseconds _timer_samples_insertion{};
		mutable std::chrono::nanoseconds _timer_evaluation{};

		template <
			typename VerticesType,
			typename BoundingSphereType,
			typename BoundingSphereMeshType,
			typename FacesType >
		[[nodiscard]] static auto map_faces_to_vertices_on_bounding_sphere(
			const VerticesType &originalVertices,
			const BoundingSphereType &originalBoundingSphere,
			const BoundingSphereMeshType &originalBoundingSphereMesh,
			const FacesType &faces,
			const size_t unitsInTheLastPlace)
		{
			constexpr auto pi = std::numbers::pi_v < Type >;
			auto vertices = originalVertices;
			auto boundingSphereMesh = originalBoundingSphereMesh;
			std::vector < geometry::vertex < Type, geometry::spatial_dimension < 2 > > > projectedVertices;
			std::vector < geometry::vertex < Type, geometry::spatial_dimension < 2 > > > projectedBoundingSphereMesh;

			projectedVertices.reserve(vertices.size());
			projectedBoundingSphereMesh.reserve(boundingSphereMesh.size());

			geometry::algorithm::translate(vertices, originalBoundingSphere.center.vector_to_origin());
			geometry::algorithm::translate(boundingSphereMesh, originalBoundingSphere.center.vector_to_origin());

			geometry::algorithm::scale_by(vertices, Type{1} / originalBoundingSphere.radius);
			geometry::algorithm::scale_by(boundingSphereMesh, Type{1} / originalBoundingSphere.radius);

			for (const auto &vertex : vertices)
			{
				projectedVertices.emplace_back(
					geometry::algorithm::cartesian_coordinates_to_spherical_coordinates(vertex).second);
			}

			for (const auto &vertex : boundingSphereMesh)
			{
				projectedBoundingSphereMesh.emplace_back(
					geometry::algorithm::cartesian_coordinates_to_spherical_coordinates(vertex).second);
			}

			using VectorOfProjectedTriangles =
				std::vector < geometry::euclidean_triangle < Type, geometry::spatial_dimension < 2 > > >;

			std::vector <
				std::variant <
					std::pair < std::optional < Type >, std::optional < Type > >,
					VectorOfProjectedTriangles > > projectedTriangles;

			projectedTriangles.reserve(faces.size());

			constexpr auto triangleContainsNorthPole =
				[] (const auto &triangle, const size_t unitsInTheLastPlace) -> bool
				{
					const auto segment =
						geometry::make_segment(
							geometry::make_vertex(Type{}),
							geometry::make_vertex(Type{1}));

					return
						segment.intersects < false > (triangle, unitsInTheLastPlace) ==
							geometry::euclidean_triangle_intersection_result::intersects;
				};

			constexpr auto triangleContainsSouthPole =
				[] (const auto &triangle, const size_t unitsInTheLastPlace) -> bool
				{
					const auto segment =
						geometry::make_segment(
							geometry::make_vertex(Type{}),
							geometry::make_vertex(Type{-1}));

					return
						segment.intersects < false > (triangle, unitsInTheLastPlace) ==
							geometry::euclidean_triangle_intersection_result::intersects;
				};

			for (const auto &face : faces)
			{
				if (face.size() != 3)
				{
					throw std::runtime_error(""); // TODO: Create message.
				}

				const auto triangle =
					geometry::make_euclidean_triangle(
						vertices[face[0]],
						vertices[face[1]],
						vertices[face[2]]);

				const auto projectedTriangle =
					geometry::make_euclidean_triangle(
						projectedVertices[face[0]],
						projectedVertices[face[1]],
						projectedVertices[face[2]]);

				const bool degenerateOnNorthPole = triangleContainsNorthPole(triangle, unitsInTheLastPlace);
				const bool degenerateOnSouthPole = triangleContainsSouthPole(triangle, unitsInTheLastPlace);

				if (degenerateOnNorthPole || degenerateOnSouthPole)
				{
					projectedTriangles.push_back(std::make_pair(std::optional < Type > {}, std::optional < Type > {}));

					if (degenerateOnNorthPole)
					{
						std::get < 0 > (projectedTriangles.back()).first =
							std::max({ projectedTriangle.vertex1.x(), projectedTriangle.vertex2.x(), projectedTriangle.vertex3.x() });
					}

					if (degenerateOnSouthPole)
					{
						std::get < 0 > (projectedTriangles.back()).second =
							std::min({ projectedTriangle.vertex1.x(), projectedTriangle.vertex2.x(), projectedTriangle.vertex3.x() });
					}
				}

				else
				{
					projectedTriangles.push_back(VectorOfProjectedTriangles({ projectedTriangle }));
				}
			}

			for (auto &projectedTriangle : projectedTriangles)
			{
				if (projectedTriangle.index() == 0)
				{
					continue;
				}

				auto &projectedTriangleVector = std::get < 1 > (projectedTriangle);
				auto &_projectedTriangle = projectedTriangleVector.back();

				const auto minAzimuthalValue =
					std::min({ _projectedTriangle.vertex1.y(), _projectedTriangle.vertex2.y(), _projectedTriangle.vertex3.y() });

				const auto maxAzimuthalValue =
					std::max({ _projectedTriangle.vertex1.y(), _projectedTriangle.vertex2.y(), _projectedTriangle.vertex3.y() });

				if (maxAzimuthalValue - minAzimuthalValue > pi)
				{
					_projectedTriangle.vertex1.y() -= (_projectedTriangle.vertex1.y() > pi ? Type{2} * pi : Type{});
					_projectedTriangle.vertex2.y() -= (_projectedTriangle.vertex2.y() > pi ? Type{2} * pi : Type{});
					_projectedTriangle.vertex3.y() -= (_projectedTriangle.vertex3.y() > pi ? Type{2} * pi : Type{});

					projectedTriangleVector.push_back(_projectedTriangle);

					projectedTriangleVector.back().vertex1.y() += Type{2} * pi;
					projectedTriangleVector.back().vertex2.y() += Type{2} * pi;
					projectedTriangleVector.back().vertex3.y() += Type{2} * pi;
				}
			}

			std::multimap < Type, size_t > polarScan;
			std::multimap < Type, size_t > azimuthalScan;

			for (size_t i = 0; i < projectedBoundingSphereMesh.size(); i++)
			{
				polarScan.emplace(projectedBoundingSphereMesh[i].x(), i);
				azimuthalScan.emplace(projectedBoundingSphereMesh[i].y(), i);
			}

			std::vector < std::vector < size_t > > facesToVerticesOnBoundingSphere;

			for (const auto &projectedTriangle : projectedTriangles)
			{
				facesToVerticesOnBoundingSphere.emplace_back();

				auto &currentVector = facesToVerticesOnBoundingSphere.back();

				if (projectedTriangle.index() == 0)
				{
					const auto &pair = std::get < 0 > (projectedTriangle);

					if (pair.first.has_value())
					{
						const auto value = pair.first.value();

						for (auto iterator = polarScan.cbegin();
							iterator != polarScan.cend();
							++iterator)
						{
							if (value < iterator->first)
							{
								break;
							}

							currentVector.push_back(iterator->second);
						}
					}

					if (pair.second.has_value())
					{
						const auto value = pair.second.value();

						for (auto iterator = polarScan.crbegin();
							iterator != polarScan.crend();
							++iterator)
						{
							if (value > iterator->first)
							{
								break;
							}

							currentVector.push_back(iterator->second);
						}
					}
				}

				else
				{
					std::vector < size_t > polarIndices;
					std::vector < size_t > azimuthalIndices;
					const auto &_projectedTriangles = std::get < 1 > (projectedTriangle);

					for (const auto &_projectedTriangle : _projectedTriangles)
					{
						const auto minPolarValue =
							std::min({ _projectedTriangle.vertex1.x(), _projectedTriangle.vertex2.x(), _projectedTriangle.vertex3.x() });

						const auto maxPolarValue =
							std::max({ _projectedTriangle.vertex1.x(), _projectedTriangle.vertex2.x(), _projectedTriangle.vertex3.x() });

						const auto minAzimuthalValue =
							std::min({ _projectedTriangle.vertex1.y(), _projectedTriangle.vertex2.y(), _projectedTriangle.vertex3.y() });

						const auto maxAzimuthalValue =
							std::max({ _projectedTriangle.vertex1.y(), _projectedTriangle.vertex2.y(), _projectedTriangle.vertex3.y() });

						const auto polarBegin = polarScan.lower_bound(minPolarValue);
						const auto polarEnd = polarScan.upper_bound(maxPolarValue);

						const auto azimuthalBegin = azimuthalScan.lower_bound(minAzimuthalValue);
						const auto azimuthalEnd = azimuthalScan.upper_bound(maxAzimuthalValue);

						for (auto iterator = polarBegin; iterator != polarEnd; ++iterator)
						{
							polarIndices.push_back(iterator->second);
						}

						for (auto iterator = azimuthalBegin; iterator != azimuthalEnd; ++iterator)
						{
							azimuthalIndices.push_back(iterator->second);
						}

						std::ranges::sort(polarIndices);
						std::ranges::sort(azimuthalIndices);

						std::vector < size_t > indices;

						std::set_union(
							polarIndices.cbegin(),
							polarIndices.cend(),
							azimuthalIndices.cbegin(),
							azimuthalIndices.cend(),
							std::back_inserter(indices));

						currentVector.insert(
							currentVector.cend(),
							indices.cbegin(),
							indices.cend());

						if (currentVector.size() != indices.size())
						{
							std::ranges::sort(currentVector);

							const auto iterator = std::unique(currentVector.begin(), currentVector.end());

							currentVector.erase(iterator, currentVector.end());
						}
					}
				}
			}

			return facesToVerticesOnBoundingSphere;
		}

		template <
			typename SphereType,
			typename FaceMappingType,
			typename BoundingSphereType,
			typename ObjectType >
		[[nodiscard]] static auto calculate_intersections(
			const SphereType &sphere,
			const FaceMappingType &facesToVerticesOnBoundingSphere,
			const BoundingSphereType &boundingSphere,
			const ObjectType &object,
			const size_t group,
			const size_t unitsInTheLastPlace)
		{
			std::vector <
				std::optional <
					std::pair <
						geometry::vertex < Type, geometry::spatial_dimension < 3 > >,
						geometry::vertex < Type, geometry::spatial_dimension < 3 > > > > > intersections;

			intersections.resize(sphere.size());

			for (size_t i = 0; i < facesToVerticesOnBoundingSphere.size(); i++)
			{
				for (const auto vertexIndex : facesToVerticesOnBoundingSphere[i])
				{
					const auto segment = geometry::make_segment(sphere[vertexIndex], boundingSphere.center);
					const auto triangle = object.triangular_face(group, i);
					const auto intersection = segment.intersects < true > (triangle, unitsInTheLastPlace);

					if (intersection.first == geometry::euclidean_triangle_intersection_result::intersects)
					{
						if (!intersections[vertexIndex].has_value())
						{
							intersections[vertexIndex] =
								std::make_pair(
									intersection.second.value(),
									intersection.second.value());

							continue;
						}

						if (intersection.second.value().distance_to(boundingSphere.center) >
							intersections[vertexIndex].value().first.distance_to(boundingSphere.center))
						{
							intersections[vertexIndex].value().first = intersection.second.value();
						}

						if (intersection.second.value().distance_to(boundingSphere.center) <
							intersections[vertexIndex].value().second.distance_to(boundingSphere.center))
						{
							intersections[vertexIndex].value().second = intersection.second.value();
						}
					}
				}
			}

			return intersections;
		}

		template < typename IntersectionType >
		void reduce_object(
			const size_t sphereVertexCount,
			const IntersectionType &intersections,
			auto &reducedObject,
			const size_t group) const
		{
			std::vector < bool > checkedVertices(sphereVertexCount, false);

			for (size_t i = 0; i < sphereVertexCount; i++)
			{
				checkedVertices[i] = true;

				if (!intersections[i].has_value())
				{
					continue;
				}

				const auto triangleNeighbors = this->_connections.triangle_neighbors(i);

				for (const auto &[neighbor1, neighbor2] : triangleNeighbors)
				{
					const auto indexNeighbor1 = neighbor1.value;
					const auto indexNeighbor2 = neighbor2.value;

					if (intersections[indexNeighbor1].has_value() && !checkedVertices[indexNeighbor1] &&
						intersections[indexNeighbor2].has_value() && !checkedVertices[indexNeighbor2])
					{
						reducedObject.insert_vertices < true > (
							2 * group,
							std::vector < geometry::vertex < Type, geometry::spatial_dimension < 3 > > > {
								intersections[i].value().first,
								intersections[indexNeighbor1].value().first,
								intersections[indexNeighbor2].value().first
							});

						reducedObject.insert_vertices < true > (
							(2 * group) + 1,
							std::vector < geometry::vertex < Type, geometry::spatial_dimension < 3 > > > {
								intersections[i].value().second,
								intersections[indexNeighbor1].value().second,
								intersections[indexNeighbor2].value().second
							});
					}
				}
			}
		}

		template < typename IntersectionType, typename VertexType >
		[[nodiscard]] auto calculate_eigenvalues(
			const Type radius,
			const size_t sphereVertexCount,
			const IntersectionType &intersections,
			const VertexType &sphereCenter,
			const Type neighborRadiusLimit) const
		{
			Type accumulatorCenterOuter{};
			Type accumulatorCenterInner{};
			const Type maxDistance = Type{2 * radius};
			const size_t offset = sphereVertexCount + 1;
			matrix_type outerVertices(sphereVertexCount + 1, sphereVertexCount + 1);
			matrix_type completeVertices(2 * sphereVertexCount + 2, 2 * sphereVertexCount + 2);
			std::pair < std::vector < Type >, std::vector < Type > > result;

			for (size_t i = 0; i < sphereVertexCount; i++)
			{
				if (!intersections[i].has_value())
				{
					continue;
				}

				Type accumulatorOuter{};
				Type accumulatorInner{};
				const auto &intersection1 = intersections[i].value();

				for (size_t j = i + 1; j < sphereVertexCount; j++)
				{
					if (!intersections[j].has_value() ||
						this->_connections_distance_matrix(i, j) > neighborRadiusLimit)
					{
						continue;
					}

					const auto &intersection2 = intersections[j].value();
					const auto valueOuter = -intersection1.first.distance_to(intersection2.first) / maxDistance;
					const auto valueInner = -intersection1.second.distance_to(intersection2.second) / maxDistance;

					accumulatorOuter += valueOuter;
					accumulatorInner += valueInner;

					outerVertices(i, j) = valueOuter;
					outerVertices(j, i) = valueOuter;

					completeVertices(i, j) = valueOuter;
					completeVertices(j, i) = valueOuter;
					completeVertices(i + offset, j + offset) = valueInner;
					completeVertices(j + offset, i + offset) = valueInner;
				}

				const auto valueCenterOuter = -intersection1.first.distance_to(sphereCenter) / maxDistance;
				const auto valueCenterInner = -intersection1.second.distance_to(sphereCenter) / maxDistance;

				accumulatorCenterOuter += valueCenterOuter;
				accumulatorCenterInner += valueCenterInner;

				accumulatorOuter += valueCenterOuter;
				accumulatorInner += valueCenterInner;

				outerVertices(i, sphereVertexCount) = valueCenterOuter;
				outerVertices(sphereVertexCount, i) = valueCenterOuter;

				completeVertices(i, sphereVertexCount) = valueCenterOuter;
				completeVertices(sphereVertexCount, i) = valueCenterOuter;
				completeVertices(i + offset, sphereVertexCount + offset) = valueCenterInner;
				completeVertices(sphereVertexCount + offset, i + offset) = valueCenterInner;

				for (size_t j = 0; j < i; j++)
				{
					accumulatorOuter += outerVertices(i, j);
					accumulatorInner += completeVertices(i + offset, j + offset);
				}

				outerVertices(i, i) = -accumulatorOuter;

				completeVertices(i, i) = -accumulatorOuter;
				completeVertices(i + offset, i + offset) = -accumulatorInner;
			}

			outerVertices(sphereVertexCount, sphereVertexCount) = -accumulatorCenterOuter;

			completeVertices(sphereVertexCount, sphereVertexCount) = -accumulatorCenterOuter;
			completeVertices(sphereVertexCount + offset, sphereVertexCount + offset) = -accumulatorCenterInner;

			auto &outerEigenvalues = result.first;
			auto &completeEigenvalues = result.second;

			constexpr auto negativeValueRemover =
				[] (Type &value)
				{
					value = std::max(value, Type{});
				};

			outerEigenvalues = machine_learning::eigenvalues_self_adjoint(outerVertices).data();
			completeEigenvalues = machine_learning::eigenvalues_self_adjoint(completeVertices).data();

			std::ranges::for_each(outerEigenvalues, negativeValueRemover);
			std::ranges::for_each(completeEigenvalues, negativeValueRemover);

			std::sort(outerEigenvalues.rbegin(), outerEigenvalues.rend());
			std::sort(completeEigenvalues.rbegin(), completeEigenvalues.rend());

			return result;
		}

		[[nodiscard]] auto evaluate(std::pair < std::vector < Type >, std::vector < Type > > eigenvalues) const
		{
			auto &outerEigenvalues = eigenvalues.first;
			auto &completeEigenvalues = eigenvalues.second;
			std::map < std::string, std::vector < labeled_sample_distance_type > > result;
			constexpr auto negativeValueRemover =
				[] (Type &value)
				{
					value = std::max(value, Type{});
				};

			std::ranges::for_each(outerEigenvalues, negativeValueRemover);
			std::ranges::for_each(completeEigenvalues, negativeValueRemover);

			std::sort(outerEigenvalues.rbegin(), outerEigenvalues.rend());
			std::sort(completeEigenvalues.rbegin(), completeEigenvalues.rend());

			for (const auto &[categoryName, groupOfEigenvalues] : this->_eigenvalues)
			{
				const auto iterator =
					result.emplace(
						categoryName,
						std::vector < labeled_sample_distance_type > (groupOfEigenvalues.size())).first;

				std::transform(std::execution::par_unseq, groupOfEigenvalues.cbegin(), groupOfEigenvalues.cend(), iterator->second.begin(),
					[&outerEigenvalues, &completeEigenvalues] (
						const labeled_eigenvalues_type &input) -> auto
					{
						labeled_sample_distance_type labeledSampleDistance{};

						constexpr auto evaluate =
							[] (const auto &storedEigenvalues,
								const auto &eigenvalues) -> Type
							{
								Type accumulator{};

								for (size_t i = 0; i < storedEigenvalues.size(); i++)
								{
									accumulator +=
										std::abs(storedEigenvalues[i] - eigenvalues[i]);
								}

								return accumulator;
							};

						labeledSampleDistance.sample_name = input.sample_name;

						labeledSampleDistance.outer_eigenvalues_distance =
							evaluate(input.outer_eigenvalues, outerEigenvalues);

						labeledSampleDistance.complete_eigenvalues_distance =
							evaluate(input.complete_eigenvalues, completeEigenvalues);

						return labeledSampleDistance;
					});
			}

			return result;
		}

		static void generate_results(result_type &result)
		{
			std::map < std::string, Type > bestOuterEigenvalueMatchesByCategory;
			std::map < std::string, Type > bestCompleteEigenvalueMatchesByCategory;
			std::map < std::string, Type > averageOuterEigenvalueMatchesByCategory;
			std::map < std::string, Type > averageCompleteEigenvalueMatchesByCategory;

			std::map < Type, std::string > bestOuterEigenvalueMatchesByValue;
			std::map < Type, std::string > bestCompleteEigenvalueMatchesByValue;
			std::map < Type, std::string > averageOuterEigenvalueMatchesByValue;
			std::map < Type, std::string > averageCompleteEigenvalueMatchesByValue;

			for (const auto &[category, eigenvalues] : result.full_result)
			{
				Type bestOuterEigenvalueMatch = std::numeric_limits < Type > ::max();
				Type bestCompleteEigenvalueMatch = std::numeric_limits < Type > ::max();
				Type accumulatorOuterEigenvalueMatch{};
				Type accumulatorCompleteEigenvalueMatch{};

				for (const auto &[_, outerEigenvalueMatch, completeEigenvalueMatch] : eigenvalues)
				{
					bestOuterEigenvalueMatch = std::min(bestOuterEigenvalueMatch, outerEigenvalueMatch);
					bestCompleteEigenvalueMatch = std::min(bestCompleteEigenvalueMatch, completeEigenvalueMatch);

					accumulatorOuterEigenvalueMatch += outerEigenvalueMatch;
					accumulatorCompleteEigenvalueMatch += completeEigenvalueMatch;
				}

				accumulatorOuterEigenvalueMatch /= static_cast < Type > (eigenvalues.size());
				accumulatorCompleteEigenvalueMatch /= static_cast < Type > (eigenvalues.size());

				bestOuterEigenvalueMatchesByCategory.emplace(category, bestOuterEigenvalueMatch);
				bestCompleteEigenvalueMatchesByCategory.emplace(category, bestCompleteEigenvalueMatch);
				averageOuterEigenvalueMatchesByCategory.emplace(category, accumulatorOuterEigenvalueMatch);
				averageCompleteEigenvalueMatchesByCategory.emplace(category, accumulatorCompleteEigenvalueMatch);

				bestOuterEigenvalueMatchesByValue.emplace(bestOuterEigenvalueMatch, category);
				bestCompleteEigenvalueMatchesByValue.emplace(bestCompleteEigenvalueMatch, category);
				averageOuterEigenvalueMatchesByValue.emplace(accumulatorOuterEigenvalueMatch, category);
				averageCompleteEigenvalueMatchesByValue.emplace(accumulatorCompleteEigenvalueMatch, category);
			}

			result.best_outer_eigenvalue_matches_by_category = std::move(bestOuterEigenvalueMatchesByCategory);
			result.best_complete_eigenvalue_matches_by_category = std::move(bestCompleteEigenvalueMatchesByCategory);
			result.average_outer_eigenvalue_matches_by_category = std::move(averageOuterEigenvalueMatchesByCategory);
			result.average_complete_eigenvalue_matches_by_category = std::move(averageCompleteEigenvalueMatchesByCategory);

			result.best_outer_eigenvalue_matches_by_value = std::move(bestOuterEigenvalueMatchesByValue);
			result.best_complete_eigenvalue_matches_by_value = std::move(bestCompleteEigenvalueMatchesByValue);
			result.average_outer_eigenvalue_matches_by_value = std::move(averageOuterEigenvalueMatchesByValue);
			result.average_complete_eigenvalue_matches_by_value = std::move(averageCompleteEigenvalueMatchesByValue);
		}

	public:
		template < bool GenerateReducedObject >
		auto insert_samples(
			const computer_graphics::object_3d < Type > &object,
			const size_t neighborRadiusLimit = 1,
			const size_t unitsInTheLastPlace = 1)
		{
			if (neighborRadiusLimit == 0)
			{
				throw std::invalid_argument(""); // TODO: Create message.
			}

			const auto startTime = std::chrono::steady_clock::now();

			auto &categoryEigenvalues =
				this->_eigenvalues.emplace(object.name(), vector_of_labeled_eigenvalues()).first->second;

			const size_t beginIndex = categoryEigenvalues.size();

			categoryEigenvalues.resize(categoryEigenvalues.size() + object.group_count());

			const auto beginIterator = categoryEigenvalues.begin() + beginIndex;
			std::vector < size_t > indices(object.group_count());

			std::generate(indices.begin(), indices.end(),
				[n = 0] () mutable -> size_t
				{
					return n++;
				});

			computer_graphics::object_3d < Type > reducedObject;

			if constexpr (GenerateReducedObject)
			{
				reducedObject.name(object.name());

				for (size_t i = 0; i < object.group_count(); i++)
				{
					reducedObject.insert_group(object.group_name(i) + "_outer");
					reducedObject.insert_group(object.group_name(i) + "_inner");
				}
			}

			std::mutex mutex;

			std::for_each(
				std::execution::par,
				indices.cbegin(),
				indices.cend(),
				[&] (const size_t i) -> void
				{
					const auto &vertices = object.group(i);
					const auto boundingSphere = geometry::algorithm::Fischer_Gartner_Kutz_bounding_hypersphere < true > (vertices);
					auto sphere = this->_sphere;

					geometry::algorithm::scale_by(
						sphere,
						boundingSphere.radius);

					geometry::algorithm::translate(
						sphere,
						boundingSphere.center.vector_from_origin());

					const auto facesToVerticesOnBoundingSphere =
						boundary_descriptor_classifier::map_faces_to_vertices_on_bounding_sphere(
							vertices,
							boundingSphere,
							sphere,
							object.faces(i),
							unitsInTheLastPlace);

					const auto intersections =
						boundary_descriptor_classifier::calculate_intersections(
							sphere,
							facesToVerticesOnBoundingSphere,
							boundingSphere,
							object,
							i,
							unitsInTheLastPlace);

					if constexpr (GenerateReducedObject)
					{
						std::scoped_lock < std::mutex > lock(mutex);

						reducedObject.insert_vertices < false > (2 * i, sphere);
						reducedObject.insert_vertices < false > ((2 * i) + 1, sphere);

						this->reduce_object(
							this->_sphere.size(),
							intersections,
							reducedObject,
							i);
					}

					auto eigenvalues =
						this->calculate_eigenvalues(
							boundingSphere.radius,
							this->_sphere.size(),
							intersections,
							boundingSphere.center,
							static_cast < Type > (neighborRadiusLimit));

					*(beginIterator + i) =
						labeled_eigenvalues_type
						{
							object.group_name(i),
							std::move(eigenvalues.first),
							std::move(eigenvalues.second)
						};
				});

			this->_timer_samples_insertion +=
				std::chrono::duration_cast < std::chrono::nanoseconds> (std::chrono::steady_clock::now() - startTime);

			if constexpr (GenerateReducedObject)
			{
				return reducedObject;
			}
		}

		template < bool GenerateReducedObject >
		auto evaluate(
			const computer_graphics::object_3d < Type > &object,
			const size_t neighborRadiusLimit = 1,
			const size_t unitsInTheLastPlace = 1) const
		{
			if (neighborRadiusLimit == 0)
			{
				throw std::invalid_argument(""); // TODO: Create message.
			}

			const auto startTime = std::chrono::steady_clock::now();

			std::vector < size_t > indices(object.group_count());
			std::vector < result_type > results(object.group_count());

			std::generate(indices.begin(), indices.end(),
				[n = 0] () mutable -> size_t
				{
					return n++;
				});

			computer_graphics::object_3d < Type > reducedObject;

			if constexpr (GenerateReducedObject)
			{
				reducedObject.name(object.name());

				for (size_t i = 0; i < object.group_count(); i++)
				{
					reducedObject.insert_group(object.group_name(i) + "_outer");
					reducedObject.insert_group(object.group_name(i) + "_inner");
				}
			}

			std::mutex mutex;

			std::for_each(
				std::execution::par,
				indices.cbegin(),
				indices.cend(),
				[&] (const size_t i) -> void
				{
					const auto &vertices = object.group(i);
					const auto boundingSphere = geometry::algorithm::Fischer_Gartner_Kutz_bounding_hypersphere < true > (vertices);
					auto sphere = this->_sphere;

					geometry::algorithm::scale_by(
						sphere,
						boundingSphere.radius);

					geometry::algorithm::translate(
						sphere,
						boundingSphere.center.vector_from_origin());

					const auto facesToVerticesOnBoundingSphere =
						boundary_descriptor_classifier::map_faces_to_vertices_on_bounding_sphere(
							vertices,
							boundingSphere,
							sphere,
							object.faces(i),
							unitsInTheLastPlace);

					const auto intersections =
						boundary_descriptor_classifier::calculate_intersections(
							sphere,
							facesToVerticesOnBoundingSphere,
							boundingSphere,
							object,
							i,
							unitsInTheLastPlace);

					if constexpr (GenerateReducedObject)
					{
						std::scoped_lock < std::mutex > lock(mutex);

						reducedObject.insert_vertices < false > (2 * i, sphere);
						reducedObject.insert_vertices < false > ((2 * i) + 1, sphere);

						this->reduce_object(
							this->_sphere.size(),
							intersections,
							reducedObject,
							i);
					}

					results[i].full_result =
						this->evaluate(
							this->calculate_eigenvalues(
								boundingSphere.radius,
								this->_sphere.size(),
								intersections,
								boundingSphere.center,
								static_cast < Type > (neighborRadiusLimit)));

					boundary_descriptor_classifier::generate_results(results[i]);
				});

			this->_timer_evaluation +=
				std::chrono::duration_cast < std::chrono::nanoseconds > (std::chrono::steady_clock::now() - startTime);

			if constexpr (GenerateReducedObject)
			{
				return std::make_pair(results, reducedObject);
			}

			else
			{
				return results;
			}
		}

		size_t category_count() const
		{
			return this->_eigenvalues.size();
		}

		const auto &eigenvalues()
		{
			return this->_eigenvalues;
		}

		std::chrono::nanoseconds timer_samples_insertion() const
		{
			return this->_timer_samples_insertion;
		}

		std::chrono::nanoseconds timer_evaluation() const
		{
			return this->_timer_evaluation;
		}

		boundary_descriptor_classifier(
			const std::string &sphereAlgorithmName,
			const std::vector < geometry::vertex < Type, geometry::spatial_dimension < 3 > > > &sphere,
			const std::vector < std::pair < size_t, size_t > > &connections,
			const size_t unitsInTheLastPlace = 1) :

			_sphere_algorithm_name(sphereAlgorithmName),
			_sphere(sphere)
		{
			if (sphereAlgorithmName.empty())
			{
				throw std::invalid_argument(""); // TODO: Create message.
			}

			if (sphere.empty())
			{
				throw std::invalid_argument(""); // TODO: Create message.
			}

			for (const auto &vertex : sphere)
			{
				if (!xablau::algebra::functions::almost_equal < Type > ::invoke(
						vertex.distance_to_origin(),
						Type{1},
						unitsInTheLastPlace))
				{
					throw std::runtime_error(""); // TODO: Create message.
				}
			}

			for (const auto [first, second] : connections)
			{
				if (first == second)
				{
					throw std::runtime_error(""); // TODO: Create message.
				}

				if (first >= sphere.size() || second >= sphere.size())
				{
					throw std::runtime_error(""); // TODO: Create message.
				}

				this->_connections.insert(first, second);
			}

			if (this->_connections.node_count() != this->_sphere.size())
			{
				throw std::runtime_error(""); // TODO: Create message.
			}

			std::tie(this->_connections_distance_matrix, std::ignore, std::ignore, std::ignore) =
				this->_connections.template Floyd_Warshall < matrix_type > ();
		}

		boundary_descriptor_classifier &operator=(const boundary_descriptor_classifier &) = default;

		boundary_descriptor_classifier &operator=(boundary_descriptor_classifier &&) noexcept = default;

		boundary_descriptor_classifier(const boundary_descriptor_classifier &) = default;

		boundary_descriptor_classifier(boundary_descriptor_classifier &&) noexcept = default;
	};
}