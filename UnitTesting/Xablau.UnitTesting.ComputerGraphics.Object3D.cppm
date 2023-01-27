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

#include <gtest/gtest.h>

import <algorithm>;
import <array>;
import <concepts>;
import <filesystem>;
import <numbers>;

import xablau.computer_graphics;
import xablau.testing;

namespace xablau::unit_testing
{
	template < typename Type >
	struct ComputerGraphicsObject3DTypeDefinitions
	{
		using value_type = Type;
	};

	template < typename Types >
	class ComputerGraphicsObject3DTest : public ::testing::Test
	{
	public:
		const std::filesystem::path directory = "./ComputerGraphics/";
		const std::filesystem::path objFile = "./ComputerGraphics/object_3d.obj";

		void SetUp() override
		{
			this->TearDown();

			std::filesystem::create_directory(this->directory);
		}

		void TearDown() override
		{
			std::filesystem::remove_all(this->directory);
		}

		template < bool WrittenWithFaces >
		void validateWavefrontFile()
		{
			std::string savedWavefront;
			std::string expectedWavefront =
				"#group1\n"
				"v  0.000000000 0.000000000 1.000000000\n"
				"v  1.000000000 0.000000000 1.000000000\n"
				"v  0.000000000 1.000000000 1.000000000\n"
				"v  1.000000000 1.000000000 1.000000000\n"
				"v  0.000000000 1.000000000 0.000000000\n"
				"v  1.000000000 1.000000000 0.000000000\n"
				"v  0.000000000 0.000000000 0.000000000\n"
				"v  1.000000000 0.000000000 0.000000000\n"
				"\n\n"
				"#group2\nv  1.000000000 0.000000000 1.000000000\n"
				"v  2.000000000 0.000000000 1.000000000\n"
				"v  1.000000000 1.000000000 1.000000000\n"
				"v  2.000000000 1.000000000 1.000000000\n"
				"v  1.000000000 1.000000000 0.000000000\n"
				"v  2.000000000 1.000000000 0.000000000\n"
				"v  1.000000000 0.000000000 0.000000000\n"
				"v  2.000000000 0.000000000 0.000000000\n"
				"\n\n";

			if constexpr (WrittenWithFaces)
			{
				expectedWavefront.append(
					"o object_3d\n"
					"\tg group1\n"
					"\tf  1 2 3 \n"
					"\tf  3 2 4 \n"
					"\tf  3 4 5 \n"
					"\tf  5 4 6 \n"
					"\tf  5 6 7 \n"
					"\tf  7 6 8 \n"
					"\tf  7 8 1 \n"
					"\tf  1 8 2 \n"
					"\tf  2 8 4 \n"
					"\tf  4 8 6 \n"
					"\tf  7 1 5 \n"
					"\tf  5 1 3 \n"
					"\n\n"
					"\tg group2\n"
					"\tf  9 10 11 \n"
					"\tf  11 10 12 \n"
					"\tf  11 12 13 \n"
					"\tf  13 12 14 \n"
					"\tf  13 14 15 \n"
					"\tf  15 14 16 \n"
					"\tf  15 16 9 \n"
					"\tf  9 16 10 \n"
					"\tf  10 16 12 \n"
					"\tf  12 16 14 \n"
					"\tf  15 9 13 \n"
					"\tf  13 9 11 \n\n\n");
			}

			io::fstream < char > objectFile(this->objFile, std::ios_base::in);

			objectFile.read(
				savedWavefront,
				[] (const char character) -> bool
				{
					if (character == std::char_traits < char > ::eof() ||
						character == '\0')
					{
						return true;
					}

					return false;
				});

			objectFile.close();

			savedWavefront.erase(
				std::remove(
					savedWavefront.begin(),
					savedWavefront.end(),
					'\r'),
				savedWavefront.end());

			EXPECT_EQ(savedWavefront, expectedWavefront);
		}
	};

	TYPED_TEST_SUITE_P(ComputerGraphicsObject3DTest);

	TYPED_TEST_P(ComputerGraphicsObject3DTest, ObjectNaming)
	{
		xablau::computer_graphics::object_3d < typename TypeParam::value_type > object;

		EXPECT_EQ(object.name(), "object_3d");

		object.name("Lorem Ipsum");

		EXPECT_EQ(object.name(), "Lorem Ipsum");
	}

	TYPED_TEST_P(ComputerGraphicsObject3DTest, Grouping)
	{
		xablau::computer_graphics::object_3d < typename TypeParam::value_type > object;

		EXPECT_EQ(object.insert_group("group1"), 0);
		EXPECT_EQ(object.insert_group("group2"), 1);

		EXPECT_EQ(object.group_name(0), "group1");
		EXPECT_EQ(object.group_name(1), "group2");

		EXPECT_EQ(object.group_count(), 2);

		object.erase_group(0);

		EXPECT_EQ(object.group_count(), 1);
		EXPECT_EQ(object.group_name(0), "group2");
	}

	TYPED_TEST_P(ComputerGraphicsObject3DTest, Vertex)
	{
		xablau::computer_graphics::object_3d < typename TypeParam::value_type > object;
		const std::vector < xablau::geometry::vertex < typename TypeParam::value_type, geometry::spatial_dimension < 3 > > > vectorOfVertices(
			5,
			xablau::geometry::vertex < typename TypeParam::value_type, geometry::spatial_dimension < 3 > > {});

		EXPECT_EQ(object.insert_group("group1"), 0);
		EXPECT_EQ(object.insert_group("group2"), 1);
		EXPECT_EQ(object.insert_group("group3"), 2);

		for (size_t i = 0; i < 10; i++)
		{
			const auto constVertex = xablau::geometry::make_vertex(
				std::to_array({static_cast < typename TypeParam::value_type > (i)}));

			EXPECT_EQ(object.vertex_count(0), i);
			EXPECT_EQ(object.insert_vertex(0, constVertex), i);
			EXPECT_EQ(object.vertex_count(0), i + 1);
		}

		for (size_t i = 0; i < 10; i++)
		{
			EXPECT_EQ(object.vertex_count(1), i);
			EXPECT_EQ(object.insert_vertex(1, xablau::geometry::vertex < typename TypeParam::value_type, geometry::spatial_dimension < 3 > > {}), i);
			EXPECT_EQ(object.vertex_count(1), i + 1);
		}

		EXPECT_EQ(object.vertex_count(2), 0);
		EXPECT_EQ(object.insert_vertices < false > (2, vectorOfVertices), 0);
		EXPECT_EQ(object.vertex_count(2), 5);
		EXPECT_EQ(object.insert_vertices < true > (2, vectorOfVertices), std::make_pair(size_t{5}, size_t{0}));
		EXPECT_EQ(object.vertex_count(2), 10);

		for (size_t i = 0; i < 5; i++)
		{
			object.erase_vertex(0, i);
			object.erase_vertex(1, i);
		}

		EXPECT_EQ(object.vertex_count(0), 5);
		EXPECT_EQ(object.vertex_count(1), 5);
		EXPECT_EQ(object.vertex_count(2), 10);
		EXPECT_EQ(object.vertex_count(), 20);

		for (size_t i = 0; i < 5; i++)
		{
			EXPECT_EQ(object.vertex(0, i)[0], static_cast < typename TypeParam::value_type > ((2 * i) + 1));
		}
	}

	TYPED_TEST_P(ComputerGraphicsObject3DTest, Face)
	{
		xablau::computer_graphics::object_3d < typename TypeParam::value_type > object;
		const std::vector < size_t > faces{ 0, 1, 2 };

		EXPECT_EQ(object.insert_group("group"), 0);

		for (size_t i = 0; i < 10; i++)
		{
			const auto vertex = xablau::geometry::make_vertex(
				std::to_array({static_cast < typename TypeParam::value_type > (i)}));

			EXPECT_EQ(object.vertex_count(0), i);
			EXPECT_EQ(object.insert_vertex(0, vertex), i);
			EXPECT_EQ(object.vertex_count(0), i + 1);
		}

		EXPECT_EQ(object.face_count(), 0);
		EXPECT_EQ(object.insert_face(0, faces), 0);
		EXPECT_EQ(object.face_count(), 1);
		EXPECT_EQ(object.insert_face(0, { 3, 4, 5, 6 }), 1);
		EXPECT_EQ(object.face_count(), 2);
		EXPECT_EQ(object.insert_face(0, 7, 8, 9), 2);
		EXPECT_EQ(object.face_count(), 3);

		EXPECT_EQ(object.face(0, 0), std::vector < size_t > ({ 0, 1, 2 }));
		EXPECT_EQ(object.face(0, 1), std::vector < size_t > ({ 3, 4, 5, 6 }));
		EXPECT_EQ(object.face(0, 2), std::vector < size_t > ({ 7, 8, 9 }));

		EXPECT_EQ(
			object.triangular_face(0, 0),
			xablau::geometry::make_euclidean_triangle(
				xablau::geometry::make_vertex(std::to_array({static_cast < typename TypeParam::value_type > (0)})),
				xablau::geometry::make_vertex(std::to_array({static_cast < typename TypeParam::value_type > (1)})),
				xablau::geometry::make_vertex(std::to_array({static_cast < typename TypeParam::value_type > (2)}))));

		EXPECT_EQ(
			object.triangular_face(0, 2),
			xablau::geometry::make_euclidean_triangle(
				xablau::geometry::make_vertex(std::to_array({static_cast < typename TypeParam::value_type > (7)})),
				xablau::geometry::make_vertex(std::to_array({static_cast < typename TypeParam::value_type > (8)})),
				xablau::geometry::make_vertex(std::to_array({static_cast < typename TypeParam::value_type > (9)}))));

		object.erase_face(0, 0);
		EXPECT_EQ(object.face_count(), 2);
		object.erase_face(0, 1);
		EXPECT_EQ(object.face_count(), 1);

		EXPECT_EQ(object.face(0, 0), std::vector < size_t > ({ 3, 4, 5, 6 }));

		object.erase_vertex(0, 9);
		EXPECT_EQ(object.face_count(), 1);
		EXPECT_EQ(object.face(0, 0), std::vector < size_t > ({ 3, 4, 5, 6 }));

		object.erase_vertex(0, 0);
		EXPECT_EQ(object.face_count(), 1);
		EXPECT_EQ(object.face(0, 0), std::vector < size_t > ({ 2, 3, 4, 5 }));

		object.erase_vertex(0, 2);
		EXPECT_EQ(object.face_count(), 0);
	}

	TYPED_TEST_P(ComputerGraphicsObject3DTest, Wavefront)
	{
		if constexpr (std::same_as < typename TypeParam::value_type, float >)
		{
			xablau::computer_graphics::object_3d < typename TypeParam::value_type > object;

			object.insert_group("group1");
			object.insert_group("group2");

			object.insert_vertex(0, xablau::geometry::make_vertex(std::to_array({ typename TypeParam::value_type{}, typename TypeParam::value_type{}, typename TypeParam::value_type{1} })));
			object.insert_vertex(0, xablau::geometry::make_vertex(std::to_array({ typename TypeParam::value_type{1}, typename TypeParam::value_type{}, typename TypeParam::value_type{1} })));
			object.insert_vertex(0, xablau::geometry::make_vertex(std::to_array({ typename TypeParam::value_type{}, typename TypeParam::value_type{1}, typename TypeParam::value_type{1} })));
			object.insert_vertex(0, xablau::geometry::make_vertex(std::to_array({ typename TypeParam::value_type{1}, typename TypeParam::value_type{1}, typename TypeParam::value_type{1} })));

			object.insert_vertex(0, xablau::geometry::make_vertex(std::to_array({ typename TypeParam::value_type{}, typename TypeParam::value_type{1}, typename TypeParam::value_type{} })));
			object.insert_vertex(0, xablau::geometry::make_vertex(std::to_array({ typename TypeParam::value_type{1}, typename TypeParam::value_type{1}, typename TypeParam::value_type{} })));
			object.insert_vertex(0, xablau::geometry::make_vertex(std::to_array({ typename TypeParam::value_type{}, typename TypeParam::value_type{}, typename TypeParam::value_type{} })));
			object.insert_vertex(0, xablau::geometry::make_vertex(std::to_array({ typename TypeParam::value_type{1}, typename TypeParam::value_type{}, typename TypeParam::value_type{} })));

			object.insert_vertex(1, xablau::geometry::make_vertex(std::to_array({ typename TypeParam::value_type{1}, typename TypeParam::value_type{}, typename TypeParam::value_type{1} })));
			object.insert_vertex(1, xablau::geometry::make_vertex(std::to_array({ typename TypeParam::value_type{2}, typename TypeParam::value_type{}, typename TypeParam::value_type{1} })));
			object.insert_vertex(1, xablau::geometry::make_vertex(std::to_array({ typename TypeParam::value_type{1}, typename TypeParam::value_type{1}, typename TypeParam::value_type{1} })));
			object.insert_vertex(1, xablau::geometry::make_vertex(std::to_array({ typename TypeParam::value_type{2}, typename TypeParam::value_type{1}, typename TypeParam::value_type{1} })));

			object.insert_vertex(1, xablau::geometry::make_vertex(std::to_array({ typename TypeParam::value_type{1}, typename TypeParam::value_type{1}, typename TypeParam::value_type{} })));
			object.insert_vertex(1, xablau::geometry::make_vertex(std::to_array({ typename TypeParam::value_type{2}, typename TypeParam::value_type{1}, typename TypeParam::value_type{} })));
			object.insert_vertex(1, xablau::geometry::make_vertex(std::to_array({ typename TypeParam::value_type{1}, typename TypeParam::value_type{}, typename TypeParam::value_type{} })));
			object.insert_vertex(1, xablau::geometry::make_vertex(std::to_array({ typename TypeParam::value_type{2}, typename TypeParam::value_type{}, typename TypeParam::value_type{} })));

			object.insert_face(0, std::vector < size_t > ({ 0, 1, 2 }));
			object.insert_face(0, std::vector < size_t > ({ 2, 1, 3 }));
			object.insert_face(0, std::vector < size_t > ({ 2, 3, 4 }));
			object.insert_face(0, std::vector < size_t > ({ 4, 3, 5 }));
			object.insert_face(0, std::vector < size_t > ({ 4, 5, 6 }));
			object.insert_face(0, std::vector < size_t > ({ 6, 5, 7 }));
			object.insert_face(0, std::vector < size_t > ({ 6, 7, 0 }));
			object.insert_face(0, std::vector < size_t > ({ 0, 7, 1 }));
			object.insert_face(0, std::vector < size_t > ({ 1, 7, 3 }));
			object.insert_face(0, std::vector < size_t > ({ 3, 7, 5 }));
			object.insert_face(0, std::vector < size_t > ({ 6, 0, 4 }));
			object.insert_face(0, std::vector < size_t > ({ 4, 0, 2 }));

			object.insert_face(1, std::vector < size_t > ({ 0, 1, 2 }));
			object.insert_face(1, std::vector < size_t > ({ 2, 1, 3 }));
			object.insert_face(1, std::vector < size_t > ({ 2, 3, 4 }));
			object.insert_face(1, std::vector < size_t > ({ 4, 3, 5 }));
			object.insert_face(1, std::vector < size_t > ({ 4, 5, 6 }));
			object.insert_face(1, std::vector < size_t > ({ 6, 5, 7 }));
			object.insert_face(1, std::vector < size_t > ({ 6, 7, 0 }));
			object.insert_face(1, std::vector < size_t > ({ 0, 7, 1 }));
			object.insert_face(1, std::vector < size_t > ({ 1, 7, 3 }));
			object.insert_face(1, std::vector < size_t > ({ 3, 7, 5 }));
			object.insert_face(1, std::vector < size_t > ({ 6, 0, 4 }));
			object.insert_face(1, std::vector < size_t > ({ 4, 0, 2 }));

			object.write_Wavefront_format < true > (this->directory);

			this->validateWavefrontFile < true > ();

			this->SetUp();

			object.write_Wavefront_format < false > (this->directory);

			this->validateWavefrontFile < false > ();
		}
	}

	REGISTER_TYPED_TEST_SUITE_P(
		ComputerGraphicsObject3DTest,
		ObjectNaming,
		Grouping,
		Vertex,
		Face,
		Wavefront);

	using ComputerGraphicsObject3DTestDataTypes = std::tuple < float, double, long double >;

	using ComputerGraphicsObject3DTestGroup =
		xablau::testing::test_bundler < ComputerGraphicsObject3DTestDataTypes > ::template test_group < ComputerGraphicsObject3DTypeDefinitions >;

	using ComputerGraphicsObject3DTypes = xablau::testing::tuple_transformation < ::testing::Types, ComputerGraphicsObject3DTestGroup > ::types;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		Xablau,
		ComputerGraphicsObject3DTest,
		ComputerGraphicsObject3DTypes);
}
