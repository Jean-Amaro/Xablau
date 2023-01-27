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

export module xablau.computer_graphics:object_3d;

export import <algorithm>;
export import <concepts>;
export import <format>;
export import <ranges>;
export import <string>;
export import <vector>;

export import xablau.geometry;
export import xablau.io;

export namespace xablau::computer_graphics
{
	template < std::floating_point Type >
	class object_3d final
	{
	private:
		template < typename ExecutionPolicy >
		static consteval auto &execution_policy_selector()
		{
			if constexpr (std::same_as < ExecutionPolicy, std::execution::parallel_unsequenced_policy >)
			{
				return std::execution::par_unseq;
			}

			else if constexpr (std::same_as < ExecutionPolicy, std::execution::parallel_policy >)
			{
				return std::execution::par;
			}

			else if constexpr (std::same_as < ExecutionPolicy, std::execution::unsequenced_policy >)
			{
				return std::execution::unseq;
			}

			else
			{
				return std::execution::seq;
			}
		}

		[[nodiscard]] size_t offset_face_index(const size_t group) const
		{
			size_t offset = 0;

			for (size_t i = 0; i < group; i++)
			{
				offset += this->vertex_count(i);
			}

			return offset;
		}

		void write_OFF_vertices(io::fstream < char > &file, const size_t group) const
		{
			for (size_t i = 0; i < this->vertex_count(group); i++)
			{
				const auto &_group = this->_object[group];

				std::string string =
					std::format(
						"{:.{}f} {:.{}f} {:.{}f}",
						_group[i].x(), std::numeric_limits < Type > ::max_digits10,
						_group[i].y(), std::numeric_limits < Type > ::max_digits10,
						_group[i].z(), std::numeric_limits < Type > ::max_digits10);

				string.append("\n");

				file.write < false > (string);
			}

			file.write('\n');
		}

		void write_OFF_faces(io::fstream < char > &file) const
		{
			for (size_t i = 0; i < this->group_count(); i++)
			{
				if (this->vertex_count(i) > 0)
				{
					const size_t offset = this->offset_face_index(i);

					for (size_t j = 0; j < this->face_count(i); j++)
					{
						std::string string = std::format("{}  ", this->_faces[i][j].size());

						for (const auto vertexIndex : this->_faces[i][j])
						{
							const size_t faceIndex = vertexIndex + offset;

							string.append(std::format("{} ", faceIndex));
						}

						string.append("\n");

						file.write < false > (string);
					}

					file.write('\n');
				}

				file.write('\n');
			}
		}

		void write_Wavefront_vertices(io::fstream < char > &file, const size_t group) const
		{
			for (size_t i = 0; i < this->vertex_count(group); i++)
			{
				const auto &_group = this->_object[group];

				std::string string =
					std::format(
						"v  {:.{}f} {:.{}f} {:.{}f}",
						_group[i].x(), std::numeric_limits < Type > ::max_digits10,
						_group[i].y(), std::numeric_limits < Type > ::max_digits10,
						_group[i].z(), std::numeric_limits < Type > ::max_digits10);

				string.append("\n");

				file.write < false > (string);
			}

			file.write('\n');
		}

		template < bool SeparateGroupsInObjects >
		void write_Wavefront_faces(io::fstream < char > &file) const
		{
			if constexpr (!SeparateGroupsInObjects)
			{
				file.write < false > (std::format("o {}\n", this->_name));
			}

			for (size_t i = 0; i < this->group_count(); i++)
			{
				if (this->vertex_count(i) > 0)
				{
					if constexpr (SeparateGroupsInObjects)
					{
						file.write < false > (std::format("o {}\n", this->_group_names[i]));
					}

					else
					{
						file.write < false > (std::format("\tg {}\n", this->_group_names[i]));
					}

					const size_t offset = this->offset_face_index(i) + 1;

					for (size_t j = 0; j < this->face_count(i); j++)
					{
						std::string string = "\tf  ";

						for (const auto vertexIndex : this->_faces[i][j])
						{
							const size_t faceIndex = vertexIndex + offset;

							string.append(std::to_string(faceIndex));

							string.append(" ");
						}

						string.append("\n");

						file.write < false > (string);
					}

					file.write('\n');
				}

				file.write('\n');
			}
		}

		template < bool ReadAsUnsignedInteger >
		static auto read_OFF_line(io::fstream < char > &file)
		{
			using ValueType =
				std::conditional <
					ReadAsUnsignedInteger,
					size_t,
					Type > ::type;

			bool endOfLine = false;

			std::string field;

			std::vector < ValueType > values;

			const auto delimiter =
				[&endOfLine] (const char charactere) -> bool
				{
					if (charactere == ' ')
					{
						return true;
					}

					if (charactere == '\n' || charactere == '\0')
					{
						endOfLine = true;

						return true;
					}

					return false;
				};

			while (!endOfLine && file.good())
			{
				field.clear();

				file.read(field, delimiter);

				if (field.size() == 0)
				{
					continue;
				}

				if (field[0] == '#')
				{
					break;
				}

				ValueType value{};

				std::from_chars(field.data(), field.data() + field.size(), value);

				values.push_back(value);
			}

			if (!endOfLine && file.good())
			{
				file.ignore(std::numeric_limits < std::streamsize > ::max(), '\n');
			}

			return values;
		}

		static void read_OFF_vertices(
			io::fstream < char > &file,
			const size_t vertexCount,
			auto &vertices)
		{
			for (size_t i = 0; i < vertexCount && file.good(); i++)
			{
				std::vector < Type > coordinates{};

				while (coordinates.size() == 0 && file.good())
				{
					coordinates = object_3d::read_OFF_line < false > (file);
				}

				if (coordinates.size() != 3)
				{
					throw std::runtime_error(""); // TODO: Create message.
				}

				vertices.push_back(
					geometry::make_vertex(
						coordinates[0],
						coordinates[1],
						coordinates[2]));
			}
		}

		static void read_OFF_faces(
			io::fstream < char > &file,
			const size_t faceCount,
			auto &faces)
		{
			for (size_t i = 0; i < faceCount && file.good(); i++)
			{
				std::vector < size_t > indices{};

				while (indices.size() == 0 && file.good())
				{
					indices = object_3d::read_OFF_line < true > (file);
				}

				if (indices.size() == 0)
				{
					throw std::runtime_error(""); // TODO: Create message.
				}

				if (indices[0] != indices.size() - 1)
				{
					throw std::runtime_error(""); // TODO: Create message.
				}

				indices.erase(indices.cbegin());

				faces.push_back(std::move(indices));
			}
		}

		template < bool MoveVertex, typename VertexType >
		size_t generic_insert_vertex(const size_t group, VertexType vertex)
		{
			if constexpr (MoveVertex)
			{
				this->_object.at(group).push_back(std::move(vertex));
			}

			else
			{
				this->_object.at(group).push_back(vertex);
			}

			size_t vertexIndex = this->vertex_count(group) - 1;

			this->_vertices_to_face_mapping[group].emplace_back();

			return vertexIndex;
		}

		std::string _name{"object_3d"};
		std::vector < std::string > _group_names{};
		std::vector < std::vector < std::unordered_set < size_t > > > _vertices_to_face_mapping{}; // _vertices_to_face_mapping[group][vertex][face] = faceID;
		std::vector < std::vector < geometry::vertex < Type, geometry::spatial_dimension < 3 > > > > _object{}; // _object[group][vertex].;
		std::vector < std::vector < std::vector < size_t > > > _faces{}; // face[group][face][vertexCountOnFace] = vertexID;

	public:
		[[nodiscard]] const std::string &name() const noexcept
		{
			return this->_name;
		}

		[[nodiscard]] std::string &name() noexcept
		{
			return this->_name;
		}

		std::string &name(const std::string &name)
		{
			this->_name = name;

			return this->_name;
		}

		[[nodiscard]] const std::vector < std::string > &group_names() const
		{
			return this->_group_names;
		}

		[[nodiscard]] const std::string &group_name(const size_t group) const
		{
			return this->_group_names.at(group);
		}

		const std::string &change_group_name(const size_t group, const std::string &newName)
		{
			if (group >= this->_group_names.size())
			{
				throw std::runtime_error(""); // TODO: Create message.
			}

			const auto groupToBeRenamed = this->_group_names.begin() + group;
			const auto groupAlreadyWithNewName =
				std::find(this->_group_names.begin(), this->_group_names.end(), newName);

			if (groupAlreadyWithNewName != this->_group_names.end() &&
				groupToBeRenamed != groupAlreadyWithNewName)
			{
				throw std::runtime_error(""); // TODO: Create message.
			}

			*groupToBeRenamed = newName;

			return *groupToBeRenamed;
		}

		[[nodiscard]] const std::vector < std::vector < geometry::vertex < Type, geometry::spatial_dimension < 3 > > > > &object() const noexcept
		{
			return this->_object;
		}

		[[nodiscard]] const std::vector < geometry::vertex < Type, geometry::spatial_dimension < 3 > > > &group(const size_t group) const
		{
			return this->_object.at(group);
		}

		[[nodiscard]] const geometry::vertex < Type, geometry::spatial_dimension < 3 > > &vertex(const size_t group, const size_t vertex) const
		{
			return this->_object.at(group).at(vertex);
		}

		[[nodiscard]] geometry::vertex < Type, geometry::spatial_dimension < 3 > > &vertex(const size_t group, const size_t vertex)
		{
			return this->_object.at(group).at(vertex);
		}

		[[nodiscard]] const std::vector < std::vector < std::vector < size_t > > > &faces() const noexcept
		{
			return this->_faces;
		}

		[[nodiscard]] const std::vector < std::vector < size_t > > &faces(const size_t group) const
		{
			return this->_faces.at(group);
		}

		[[nodiscard]] const std::vector < size_t > &face(const size_t group, const size_t face) const
		{
			return this->_faces.at(group).at(face);
		}

		[[nodiscard]] geometry::euclidean_triangle < Type, geometry::spatial_dimension < 3 > > triangular_face(
			const size_t group,
			const size_t face) const
		{
			const auto &triangularFace = this->_faces.at(group).at(face);

			if (triangularFace.size() != 3)
			{
				throw std::logic_error("The face is not triangular.");
			}

			const auto &_group = this->_object[group];

			return
				geometry::euclidean_triangle < Type, geometry::spatial_dimension < 3 > > (
					_group[triangularFace[0]],
					_group[triangularFace[1]],
					_group[triangularFace[2]]);
		}

		[[nodiscard]] size_t group_count() const noexcept
		{
			return this->_group_names.size();
		}

		[[nodiscard]] size_t vertex_count() const noexcept
		{
			size_t vertex_count = 0;

			for (const auto &group : this->_object)
			{
				vertex_count += group.size();
			}

			return vertex_count;
		}

		[[nodiscard]] size_t vertex_count(const size_t group) const noexcept
		{
			if (group >= this->_object.size())
			{
				return 0;
			}

			return this->_object[group].size();
		}

		[[nodiscard]] size_t face_count() const noexcept
		{
			size_t faceCount{};

			for (const auto &face : this->_faces)
			{
				faceCount += face.size();
			}

			return faceCount;
		}

		[[nodiscard]] size_t face_count(const size_t group) const noexcept
		{
			if (group >= this->_faces.size())
			{
				return 0;
			}

			return this->_faces[group].size();
		}

		size_t insert_group(const std::string &groupName)
		{
			if (groupName.empty())
			{
				throw std::logic_error("\"groupName\" is empty.");
			}

			if (std::ranges::find(this->_group_names, groupName) != this->_group_names.cend())
			{
				throw std::logic_error(
					std::format("The name \"{}\" is already present in the object.", groupName));
			}

			this->_group_names.push_back(groupName);
			this->_object.push_back(std::vector < geometry::vertex < Type, geometry::spatial_dimension < 3 > > > ());
			this->_vertices_to_face_mapping.push_back(std::vector < std::unordered_set < size_t > > ());
			this->_faces.push_back(std::vector < std::vector < size_t > > ());

			return this->group_count() - 1;
		}

		template < size_t N >
		requires (N <= 3)
		size_t insert_vertex(const size_t group, const geometry::vertex < Type, geometry::spatial_dimension < N > > &vertex)
		{
			return this->generic_insert_vertex < false > (group, vertex);
		}

		template < size_t N >
		requires (N <= 3)
		size_t insert_vertex(const size_t group, geometry::vertex < Type, geometry::spatial_dimension < N > > &&vertex)
		{
			return this->generic_insert_vertex < true > (group, std::move(vertex));
		}

		template < bool AlsoInsertFace, size_t N >
		requires (N <= 3)
		auto insert_vertices(
			const size_t group,
			const std::vector < geometry::vertex < Type, geometry::spatial_dimension < N > > > &vertices)
		{
			if (vertices.empty())
			{
				throw std::logic_error("Argument \"vertices\" is empty.");
			}

			size_t vertexIndex = this->vertex_count(group);

			auto &_group = this->_object.at(group);

			_group.insert(_group.end(), vertices.begin(), vertices.end());

			auto &verticesToFaceMapping =
				this->_vertices_to_face_mapping.at(group);

			verticesToFaceMapping.resize(verticesToFaceMapping.size() + vertices.size());

			if constexpr (AlsoInsertFace)
			{
				std::vector < size_t > vertexIndices(vertices.size());

				for (size_t i = 0; i < vertexIndices.size(); i++)
				{
					vertexIndices[i] = vertexIndex + i;
				}

				return std::make_pair(vertexIndex, this->insert_face(group, vertexIndices));
			}

			else
			{
				return vertexIndex;
			}
		}

		size_t insert_face(const size_t group, const std::vector < size_t > &face)
		{
			if (face.empty())
			{
				throw std::logic_error("Argument \"face\" is empty.");
			}

			const auto vertexCount = this->vertex_count(group);

			for (size_t i = 0; i < face.size(); i++)
			{
				if (face[i] >= vertexCount)
				{
					throw std::out_of_range(
						std::format(
							"\"face[{}]\" is out of range: {} >= {}.",
							i,
							face[i],
							vertexCount));
				}
			}

			size_t newFaceIndex = this->face_count(group);

			this->_faces[group].push_back(face);

			auto &vertexToFaceGroupMapping = this->_vertices_to_face_mapping.at(group);

			for (const auto vertex : face)
			{
				vertexToFaceGroupMapping.at(vertex).insert(newFaceIndex);
			}

			return newFaceIndex;
		}

		size_t insert_face(const size_t group, const size_t vertex1, const size_t vertex2, const size_t vertex3)
		{
			return this->insert_face(group, { vertex1, vertex2, vertex3 });
		}

		template < size_t N >
		requires (N <= 3)
		void translate_to(
			const size_t group,
			const size_t vertex,
			const geometry::vertex < Type, geometry::spatial_dimension < N > > &newPosition)
		{
			this->_object.at(group).at(vertex) = newPosition;
		}

		template <
			typename ExecutionPolicy = std::execution::parallel_unsequenced_policy,
			size_t N >
		requires (N <= 3)
		void translate(const geometry::vector < Type, geometry::spatial_dimension < N > > &displacement)
		{
			for (auto &group : this->_object)
			{
				geometry::algorithm::translate < ExecutionPolicy > (group, displacement);
			}
		}

		template <
			typename ExecutionPolicy = std::execution::parallel_unsequenced_policy,
			size_t N >
		requires (N <= 3)
		void translate(const size_t group, const geometry::vector < Type, geometry::spatial_dimension < N > > &displacement)
		{
			geometry::algorithm::translate < ExecutionPolicy > (this->_object.at(group), displacement);
		}

		template < size_t N >
		requires (N <= 3)
		void translate(const size_t group, const size_t vertex, const geometry::vector < Type, geometry::spatial_dimension < N > > &displacement)
		{
			this->_object.at(group).at(vertex) += displacement;
		}

		template <
			typename ExecutionPolicy = std::execution::parallel_unsequenced_policy,
			size_t N >
		requires (N <= 3)
		void rotate_around_arbitrary_axis(
			const geometry::vector < Type, geometry::spatial_dimension < N > > &axis,
			const Type angle)
		{
			for (auto &group : this->_object)
			{
				geometry::algorithm::rotate_around_arbitrary_axis < ExecutionPolicy > (group, axis, angle);
			}
		}

		template <
			typename ExecutionPolicy = std::execution::parallel_unsequenced_policy,
			size_t N >
		requires (N <= 3)
		void rotate_around_arbitrary_axis(
			const size_t group,
			const geometry::vector < Type, geometry::spatial_dimension < N > > &axis,
			const Type angle)
		{
			geometry::algorithm::rotate_around_arbitrary_axis < ExecutionPolicy > (this->_object.at(group), axis, angle);
		}

		void erase_group(const size_t group)
		{
			if (group >= this->group_count())
			{
				throw std::out_of_range(
					std::format(
						"\"group\" is out of range: {} >= {}.",
						group,
						this->group_count()));
			}

			this->_group_names.erase(this->_group_names.begin() + group);
			this->_vertices_to_face_mapping.erase(this->_vertices_to_face_mapping.begin() + group);
			this->_object.erase(this->_object.begin() + group);
			this->_faces.erase(this->_faces.begin() + group);
		}

		void erase_vertex(const size_t group, const size_t vertex)
		{
			auto &_group = this->_object.at(group);
			auto &_faceGroup = this->_faces.at(group);

			if (vertex >= this->vertex_count(group))
			{
				throw std::out_of_range(
					std::format(
						"\"vertex\" is out of range on group {}: {} >= {}.",
						group,
						vertex,
						this->vertex_count(group)));
			}

			_group.erase(_group.begin() + vertex);

			for (size_t i = 0; i < this->face_count(group);)
			{
				auto &face = _faceGroup[i];

				if (std::find(face.cbegin(), face.cend(), vertex) != face.cend())
				{
					this->erase_face(group, i);
				}

				else
				{
					for (auto &_vertexIndex : face)
					{
						if (_vertexIndex > vertex)
						{
							_vertexIndex--;
						}
					}

					i++;
				}
			}

			auto &verticesToFaceMapping =
				this->_vertices_to_face_mapping.at(group);

			verticesToFaceMapping.erase(verticesToFaceMapping.begin() + vertex);
		}

		size_t erase_orphan_vertices(const size_t group)
		{
			std::vector < size_t > verticesToBeErased;
			const auto &mapping = this->_vertices_to_face_mapping.at(group);

			for (size_t i = 0; i < mapping.size(); i++)
			{
				if (mapping[i].size() == 0)
				{
					verticesToBeErased.push_back(i);
				}
			}

			for (auto iterator = verticesToBeErased.crbegin();
				iterator != verticesToBeErased.crend();
				++iterator)
			{
				this->erase_vertex(group, *iterator);
			}

			return verticesToBeErased.size();
		}

		template < typename ExecutionPolicy = std::execution::parallel_unsequenced_policy >
		requires (std::is_execution_policy < ExecutionPolicy > ::value)
		size_t erase_orphan_vertices()
		{
			std::vector < size_t > indices(this->group_count());
			std::vector < size_t > erasedVertices(this->group_count());

			std::generate(indices.begin(), indices.end(),
				[n = 0] () mutable -> size_t
				{
					return n++;
				});

			std::for_each(
				object_3d::execution_policy_selector < ExecutionPolicy > (),
				indices.cbegin(),
				indices.cend(),
				[&] (const size_t index) -> void
				{
					erasedVertices[index] = this->erase_orphan_vertices(index);
				});

			return std::reduce(erasedVertices.cbegin(), erasedVertices.cend());
		}

		void erase_face(const size_t group, const size_t face)
		{
			auto &verticesToFaceMappingGroup = this->_vertices_to_face_mapping.at(group);
			const auto &faceToBeRemoved = this->_faces.at(group).at(face);
			auto &facesGroup = this->_faces.at(group);

			for (const auto vertexIndex : faceToBeRemoved)
			{
				verticesToFaceMappingGroup.at(vertexIndex).erase(face);
			}

			for (auto &faces : verticesToFaceMappingGroup)
			{
				std::vector < size_t > vertexIndices;

				for (auto iterador = faces.begin(); iterador != faces.end();)
				{
					const size_t currentFaceIndex = (*iterador);

					if (currentFaceIndex > face)
					{
						iterador = faces.erase(iterador);
						vertexIndices.push_back(currentFaceIndex - 1);
					}

					else
					{
						++iterador;
					}
				}

				faces.insert(vertexIndices.begin(), vertexIndices.end());
			}

			facesGroup.erase(facesGroup.begin() + face);
		}

		size_t read_OFF_format(const std::filesystem::path &filename, const std::string &groupName)
		{
			std::string header;
			std::vector < geometry::vertex < Type, geometry::spatial_dimension < 3 > > > vertices;
			std::vector < std::vector < size_t > > faces;
			io::fstream < char > file(filename, std::ios_base::in);
			constexpr auto delimiter =
				[] (const char charactere) -> bool
				{
					return charactere == '\n';
				};

			file.read(header, delimiter);

			if (header != "OFF")
			{
				throw std::runtime_error(""); // TODO: Create message.
			}

			std::vector < size_t > metaData{};

			while (metaData.size() == 0 && file.good())
			{
				metaData = object_3d::read_OFF_line < true > (file);
			}

			if (metaData.size() != 3)
			{
				throw std::runtime_error(""); // TODO: Create message.
			}

			const auto vertexCount = metaData[0];
			const auto faceCount = metaData[1];

			object_3d::read_OFF_vertices(file, vertexCount, vertices);
			object_3d::read_OFF_faces(file, faceCount, faces);

			size_t group = this->insert_group(groupName);

			this->insert_vertices < false > (group, vertices);

			for (const auto &face : faces)
			{
				this->insert_face(group, face);
			}

			return group;
		}

		template < bool WriteFaces >
		void write_OFF_format(const std::filesystem::path &directory = "./") const
		{
			io::fstream < char > objectFile(directory / (this->_name + ".off"), std::ios_base::out | std::ios_base::trunc);

			objectFile.write < false > ("OFF\n");
			objectFile.write < false > (std::format("{} {} 0\n", this->vertex_count(), this->face_count()));

			for (size_t i = 0; i < this->group_count(); i++)
			{
				if (this->vertex_count(i) > 0)
				{
					objectFile.write < false > (std::format("#{}\n", this->_group_names[i]));

					this->write_OFF_vertices(objectFile, i);

					objectFile.write('\n');
				}
			}

			if constexpr (WriteFaces)
			{
				this->write_OFF_faces(objectFile);
			}
		}

		template < bool WriteFaces, bool SeparateGroupsInObjects = false >
		void write_Wavefront_format(const std::filesystem::path &directory = "./") const
		{
			io::fstream < char > objectFile(directory / (this->_name + ".obj"), std::ios_base::out | std::ios_base::trunc);

			for (size_t i = 0; i < this->group_count(); i++)
			{
				if (this->vertex_count(i) > 0)
				{
					objectFile.write < false > (std::format("#{}\n", this->_group_names[i]));

					this->write_Wavefront_vertices(objectFile, i);

					objectFile.write('\n');
				}
			}

			if constexpr (WriteFaces)
			{
				this->write_Wavefront_faces < SeparateGroupsInObjects > (objectFile);
			}
		}

		std::vector < object_3d > split_groups_into_objects() const
		{
			std::vector < object_3d > result;

			for (size_t i = 0; i < this->group_count(); i++)
			{
				auto &currentObject = result.emplace_back(this->group_name(i));

				currentObject.insert_group(this->group_name(i));
				currentObject.insert_vertices < false > (0, this->group(i));

				for (size_t j = 0; j < this->face_count(i); j++)
				{
					currentObject.insert_face(0, this->_faces[i][j]);
				}
			}

			return result;
		}

		object_3d &operator=(const object_3d &object) = default;

		object_3d &operator=(object_3d &&) noexcept = default;

		object_3d() = default;

		explicit constexpr object_3d(const std::string &name) :
			_name(name)
		{
		}

		object_3d(const object_3d &object) = default;

		object_3d(object_3d &&) noexcept = default;
	};
}