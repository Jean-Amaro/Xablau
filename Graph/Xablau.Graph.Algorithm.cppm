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

export module xablau.graph:algorithm;

export import std;

export import xablau.algebra;

namespace xablau::graph::algorithm
{
	namespace internals
	{
		template < std::floating_point DistanceType, size_t Rank, typename ComparisonType >
		class A_star_cell;

		template < std::floating_point DistanceType, size_t Rank, typename ComparisonType >
		class A_star_cell
		{
		public:
			bool inserted = false;
			bool visited = false;
			std::array < size_t, Rank > position{};
			DistanceType current_cost = std::numeric_limits < DistanceType > ::max();
			DistanceType predicted_total_cost = std::numeric_limits < DistanceType > ::max();
			std::optional < std::reference_wrapper < const A_star_cell > > previous = std::nullopt;
			std::optional < typename std::multiset < std::reference_wrapper < A_star_cell >, ComparisonType > ::iterator > position_on_queue = std::nullopt;

			constexpr bool operator<(const A_star_cell &cell) const
			{
				return this->predicted_total_cost < cell.predicted_total_cost;
			}
		};

		template < std::floating_point DistanceType, typename PositionType >
		DistanceType euclidean_distance_function(const PositionType &cell1, const PositionType &cell2)
		{
			DistanceType distance{};

			for (size_t i = 0; i < cell1.size(); i++)
			{
				distance +=
					std::pow(
						static_cast < DistanceType > (cell1[i]) - static_cast < DistanceType > (cell2[i]),
						DistanceType{2});
			}

			return std::sqrt(distance);
		}

		template < std::floating_point DistanceType, typename PositionType >
		DistanceType Manhattan_distance_function(const PositionType &cell1, const PositionType &cell2)
		{
			DistanceType distance{};

			for (size_t i = 0; i < cell1.size(); i++)
			{
				distance += static_cast < DistanceType > (cell1[i]) - static_cast < DistanceType > (cell2[i]);
			}

			return distance;
		}

		template < size_t Counter, size_t Rank, size_t ... Dimensionalities >
		requires (Counter <= Rank)
		consteval auto create_A_star_kernel_size()
		{
			if constexpr (Counter == Rank)
			{
				return xablau::algebra::tensor_fixed_dimensionalities < Dimensionalities ... > {};
			}

			else
			{
				return create_A_star_kernel_size < Counter + 1, Rank, 3, Dimensionalities ... > ();
			}
		}

		template < bool MoveDiagonally, size_t Rank >
		consteval bool valid_neighbor(const std::array < size_t, Rank > &coordinates)
		{
			if constexpr (MoveDiagonally)
			{
				return true;
			}

			bool movementOccured = false;

			for (auto coordinate : coordinates)
			{
				if (coordinate == 0 || coordinate == 2)
				{
					if (!movementOccured)
					{
						movementOccured = true;
					}

					else
					{
						return false;
					}
				}
			}

			if (!movementOccured)
			{
				return false;
			}

			return true;
		}

		template <
			bool MoveDiagonally,
			std::floating_point DistanceType,
			size_t CurrentIndex,
			size_t Rank,
			typename KernelType,
			typename QueueType,
			typename MazeTensorType,
			typename SearchTensorType,
			typename PositionType,
			size_t ... Dimensionalities >
		requires (CurrentIndex <= Rank)
		void visit_neighbors_for_A_star(
			KernelType &kernel,
			QueueType &queue,
			const MazeTensorType &mazeTensor,
			const SearchTensorType &searchTensor,
			const PositionType &currentPosition,
			const PositionType &destination,
			const std::function < bool(const typename MazeTensorType::value_type &) > &blocked)
		{
			if constexpr (CurrentIndex == Rank)
			{
				constexpr PositionType neighborPosition = { std::forward < size_t > (Dimensionalities)... };

				constexpr auto conversionFunction =
					[] (const size_t value) -> DistanceType
					{
						return static_cast < DistanceType > (value);
					};

				if constexpr (!valid_neighbor < MoveDiagonally > (neighborPosition))
				{
					return;
				}

				else
				{
					if (!kernel.has_reference_to_main(Dimensionalities...))
					{
						return;
					}

					auto &neighborCell = kernel(Dimensionalities...);

					if (!neighborCell.visited)
					{
						PositionType neighborAbsolutePosition{};

						std::ranges::transform(
							currentPosition,
							neighborPosition,
							neighborAbsolutePosition.begin(),
							[] (const size_t left, const size_t right)
							{
								return (left + right) - 1;
							});

						if (blocked(mazeTensor(neighborAbsolutePosition)))
						{
							neighborCell.visited = true;

							return;
						}

						const auto &originCell = searchTensor(currentPosition);

						DistanceType cost = originCell.current_cost;

						if constexpr (MoveDiagonally)
						{
							cost += euclidean_distance_function < DistanceType > (currentPosition, neighborAbsolutePosition);
						}

						else
						{
							cost += DistanceType{1};
						}

						if (neighborCell.current_cost > cost)
						{
							typename QueueType::node_type node{};

							neighborCell.current_cost = cost;
							neighborCell.previous = originCell;

							if (neighborCell.inserted)
							{
								node = queue.extract(neighborCell.position_on_queue.value());
							}

							if constexpr (MoveDiagonally)
							{
								neighborCell.predicted_total_cost =
									cost + euclidean_distance_function < DistanceType > (destination, neighborAbsolutePosition);
							}

							else
							{
								neighborCell.predicted_total_cost = cost + DistanceType{1};
							}

							if (neighborCell.inserted)
							{
								neighborCell.position_on_queue = queue.insert(std::move(node));
							}
						}

						if (!neighborCell.inserted)
						{
							neighborCell.inserted = true;
							neighborCell.position = neighborAbsolutePosition;
							neighborCell.position_on_queue = queue.insert(neighborCell);
						}
					}
				}
			}

			else
			{
				// TODO: Remove these multiple calls and workaround MSVC bugs.
				visit_neighbors_for_A_star <
					MoveDiagonally,
					DistanceType,
					CurrentIndex + 1,
					Rank,
					KernelType,
					QueueType,
					MazeTensorType,
					SearchTensorType,
					PositionType,
					0,
					Dimensionalities ... > (
						kernel,
						queue,
						mazeTensor,
						searchTensor,
						currentPosition,
						destination,
						blocked);

				visit_neighbors_for_A_star <
					MoveDiagonally,
					DistanceType,
					CurrentIndex + 1,
					Rank,
					KernelType,
					QueueType,
					MazeTensorType,
					SearchTensorType,
					PositionType,
					1,
					Dimensionalities ... > (
						kernel,
						queue,
						mazeTensor,
						searchTensor,
						currentPosition,
						destination,
						blocked);

				visit_neighbors_for_A_star <
					MoveDiagonally,
					DistanceType,
					CurrentIndex + 1,
					Rank,
					KernelType,
					QueueType,
					MazeTensorType,
					SearchTensorType,
					PositionType,
					2,
					Dimensionalities ... > (
						kernel,
						queue,
						mazeTensor,
						searchTensor,
						currentPosition,
						destination,
						blocked);
			}
		}

		template <
			bool MoveDiagonally,
			std::floating_point DistanceType,
			size_t Rank,
			typename MazeTensorType,
			typename SearchTensorType,
			typename QueueType >
		std::pair <
			bool,
			std::optional <
				std::reference_wrapper <
					const internals::A_star_cell < DistanceType, Rank, typename QueueType::key_compare > > > > run_A_star(
			const MazeTensorType &mazeTensor,
			SearchTensorType &searchTensor,
			QueueType &queue,
			std::array < size_t, Rank > currentPosition,
			const std::array < size_t, Rank > &destination,
			const std::function < bool(const typename MazeTensorType::value_type &) > &blocked)
		{
			using KernelSize = decltype(create_A_star_kernel_size < 0, Rank > ());

			constexpr auto conversionFunction =
				[] (const size_t value) -> DistanceType
				{
					return static_cast < DistanceType > (value);
				};

			std::reference_wrapper <
				internals::A_star_cell <
					DistanceType,
					MazeTensorType::rank(),
					typename QueueType::key_compare > > currentCell = searchTensor(currentPosition);

			while (true)
			{
				if (currentPosition == destination)
				{
					return
						std::pair < bool, std::optional < std::reference_wrapper < const A_star_cell < DistanceType, Rank, typename QueueType::key_compare > > > > (
							true,
							currentCell);
				}

				auto kernel = searchTensor.kernel < KernelSize > (currentPosition);

				visit_neighbors_for_A_star < MoveDiagonally, DistanceType, 0, Rank > (
					kernel,
					queue,
					mazeTensor,
					searchTensor,
					currentPosition,
					destination,
					blocked);

				currentCell.get().visited = true;

				if (!queue.empty())
				{
					currentCell = *(queue.begin());

					currentPosition = currentCell.get().position;
					currentCell.get().position_on_queue = std::nullopt;

					queue.erase(queue.begin());
				}

				else
				{
					break;
				}
			}

			return
				std::pair < bool, std::optional < std::reference_wrapper < const A_star_cell < DistanceType, Rank, typename QueueType::key_compare > > > > (
					false,
					std::nullopt);
		}

		template < typename MazeTensorType, typename CellType >
		constexpr auto create_search_tensor()
		{
			if constexpr (algebra::concepts::xablau_tensor_dynamic < MazeTensorType >)
			{
				return
					algebra::tensor_dense_dynamic <
						CellType,
						xablau::algebra::tensor_rank < MazeTensorType::rank() >,
						typename MazeTensorType::memory_order_indices > ();
			}

			else
			{
				return
					algebra::tensor_dense_fixed <
						CellType,
						typename MazeTensorType::fixed_dimensionalities,
						typename MazeTensorType::memory_order_indices > ();
			}
		}
	}

	export template <
		bool MoveDiagonally = true,
		std::floating_point DistanceType = float,
		xablau::algebra::concepts::xablau_tensor MazeTensorType >
	std::vector < std::array < size_t, MazeTensorType::rank() > > A_star(
		const MazeTensorType &maze,
		const std::array < size_t, MazeTensorType::rank() > &origin,
		const std::array < size_t, MazeTensorType::rank() > &destination,
		const std::function < bool(const typename MazeTensorType::value_type &) > &blocked)
	{
		if (blocked(maze(origin)) || blocked(maze(destination)))
		{
			return std::vector < std::array < size_t, MazeTensorType::rank() > > ();
		}

		const auto &mazeDimensionalities = maze.dimensionalities();

		for (size_t i = 0; i < mazeDimensionalities.size(); i++)
		{
			if (origin[i] >= mazeDimensionalities[i] || destination[i] >= mazeDimensionalities[i])
			{
				return std::vector < std::array < size_t, MazeTensorType::rank() > > ();
			}
		}

		constexpr auto refLocalAStarCellComparison =
			[] < typename AStarCell > (const AStarCell &cell1, const AStarCell &cell2) -> bool
			{
				return cell1.get() < cell2.get();
			};

		using LocalAStarCell = internals::A_star_cell < DistanceType, MazeTensorType::rank(), decltype(refLocalAStarCellComparison) >;

		auto searchTensor = internals::create_search_tensor < MazeTensorType, LocalAStarCell > ();

		if constexpr (algebra::concepts::xablau_tensor_dynamic < MazeTensorType >)
		{
			searchTensor.resize(maze.dimensionalities(), LocalAStarCell{});
		}

		auto &startCell = searchTensor(destination);

		startCell.inserted = true;
		startCell.visited = true;
		startCell.current_cost = DistanceType{};

		if constexpr (MoveDiagonally)
		{
			startCell.predicted_total_cost = internals::euclidean_distance_function < DistanceType > (origin, destination);
		}

		else
		{
			startCell.predicted_total_cost = internals::Manhattan_distance_function < DistanceType > (origin, destination);
		}

		startCell.position = destination;

		std::multiset < std::reference_wrapper < LocalAStarCell >, decltype(refLocalAStarCellComparison) > queue(refLocalAStarCellComparison);

		auto [pathFound, currentCell] =
			internals::run_A_star < MoveDiagonally, DistanceType, MazeTensorType::rank() > (
				maze,
				searchTensor,
				queue,
				destination,
				origin,
				blocked);

		std::vector < std::array < size_t, MazeTensorType::rank() > > path{};

		if (pathFound)
		{
			while (currentCell.has_value())
			{
				const auto &currentCellRef = currentCell.value().get();

				path.push_back(currentCellRef.position);

				currentCell = currentCellRef.previous;
			}
		}

		return path;
	}
}