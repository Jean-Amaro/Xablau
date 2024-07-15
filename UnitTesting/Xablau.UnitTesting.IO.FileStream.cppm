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

#include <gtest/gtest.h>

import std;

import xablau.io;
import xablau.testing;

namespace xablau::unit_testing
{
	template < typename Type >
	struct IOFileStreamTypeDefinitions
	{
		using value_type = Type;
	};

	template < typename Types >
	class IOFileStreamTest : public ::testing::Test
	{
	public:
		const std::filesystem::path directory = "./IO/";
		const std::filesystem::path fullPath = "./IO/file";

		void SetUp() override
		{
			this->TearDown();

			std::filesystem::create_directory(this->directory);
		}

		void TearDown() override
		{
			std::filesystem::remove_all(this->directory);
		}

		static auto createStringFull()
		{
			if constexpr (std::same_as < typename Types::value_type, char >)
			{
				return
					std::basic_string < typename Types::value_type > (
						"Lorem ipsum dolor sit amet, consectetur adipiscing elit, "
						"sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
						"Ut enim ad minim veniam, quis nostrud exercitation ullamco "
						"laboris nisi ut aliquip ex ea commodo consequat. "
						"Duis aute irure dolor in reprehenderit in voluptate velit esse "
						"cillum dolore eu fugiat nulla pariatur.Excepteur sint occaecat "
						"cupidatat non proident, sunt in culpa qui officia deserunt mollit "
						"anim id est laborum.");
			}

			else
			{
				return
					std::basic_string < typename Types::value_type > (
						L"Lorem ipsum dolor sit amet, consectetur adipiscing elit, "
						"sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
						"Ut enim ad minim veniam, quis nostrud exercitation ullamco "
						"laboris nisi ut aliquip ex ea commodo consequat. "
						"Duis aute irure dolor in reprehenderit in voluptate velit esse "
						"cillum dolore eu fugiat nulla pariatur.Excepteur sint occaecat "
						"cupidatat non proident, sunt in culpa qui officia deserunt mollit "
						"anim id est laborum.");
			}
		}

		static auto createStringDelimited()
		{
			if constexpr (std::same_as < typename Types::value_type, char >)
			{
				return
					std::basic_string < typename Types::value_type > (
						"Lorem ipsum dolor sit amet, consectetur adipiscing elit, "
						"sed do eiusmod tempor incididunt ut labore et dolore magna aliqua");
			}

			else
			{
				return
					std::basic_string < typename Types::value_type > (
						L"Lorem ipsum dolor sit amet, consectetur adipiscing elit, "
						"sed do eiusmod tempor incididunt ut labore et dolore magna aliqua");
			}
		}

		template < typename Type >
		void validateContainer(const Type &input) const
		{
			if constexpr (std::same_as < typename Types::value_type, char >)
			{
				xablau::io::fstream < typename Types::value_type > fileWrite(
					this->fullPath,
					std::ios_base::out | std::ios_base::binary);

				fileWrite.write(input);

				fileWrite.close();

				xablau::io::fstream < typename Types::value_type > fileRead(
					this->fullPath,
					std::ios_base::in | std::ios_base::binary);

				typename std::remove_cvref < Type > ::type output{};

				fileRead.read(output);

				EXPECT_EQ(input, output);
			}
		}

		template < typename Type >
		void validateContainerForwardList(Type &input) const
		{
			if constexpr (std::same_as < typename Types::value_type, char >)
			{
				xablau::io::fstream < typename Types::value_type > fileWrite(
					this->fullPath,
					std::ios_base::out | std::ios_base::binary);

				fileWrite.write(input);

				fileWrite.close();

				xablau::io::fstream < typename Types::value_type > fileRead(
					this->fullPath,
					std::ios_base::in | std::ios_base::binary);

				typename std::remove_cvref < Type > ::type output{};

				fileRead.read(output);

				EXPECT_EQ(input.size(), output.size());

				const auto size = input.size();

				for (size_t i = 0; i < size; i++)
				{
					EXPECT_EQ(input.top(), output.top());

					input.pop();
					output.pop();
				}
			}
		}
	};

	TYPED_TEST_SUITE_P(IOFileStreamTest);

	TYPED_TEST_P(IOFileStreamTest, FundamentalTypes)
	{
		if constexpr (std::same_as < typename TypeParam::value_type, char >)
		{
			constexpr signed char _writeSignedChar{-2};
			constexpr unsigned char _writeUnsignedChar{3};
			constexpr short int _writeShortInt{-5};
			constexpr unsigned short int _writeUnsignedShortInt{7};
			constexpr int _writeInt{-11};
			constexpr unsigned int _writeUnsignedInt{13};
			constexpr long int _writeLongInt{-17};
			constexpr unsigned long int _writeUnsignedLongInt{19};
			constexpr long long int _writeLongLongInt{-23};
			constexpr unsigned long long int _writeUnsignedLongLongInt{29};
			constexpr bool _writeBool{true};
			constexpr char8_t _writeChar8_t{31};
			constexpr char16_t _writeChar16_t{37};
			constexpr char32_t _writeChar32_t{41};
			constexpr float _writeFloat{43};
			constexpr double _writeDouble{-47};
			constexpr long double _writeLongDouble{53};

			xablau::io::fstream < typename TypeParam::value_type > fileWrite(
				this->fullPath,
				std::ios_base::out | std::ios_base::binary);

			fileWrite.write(_writeSignedChar);
			fileWrite.write(_writeUnsignedChar);
			fileWrite.write(_writeShortInt);
			fileWrite.write(_writeUnsignedShortInt);
			fileWrite.write(_writeInt);
			fileWrite.write(_writeUnsignedInt);
			fileWrite.write(_writeLongInt);
			fileWrite.write(_writeUnsignedLongInt);
			fileWrite.write(_writeLongLongInt);
			fileWrite.write(_writeUnsignedLongLongInt);
			fileWrite.write(_writeBool);
			fileWrite.write(_writeChar8_t);
			fileWrite.write(_writeChar16_t);
			fileWrite.write(_writeChar32_t);
			fileWrite.write(_writeFloat);
			fileWrite.write(_writeDouble);
			fileWrite.write(_writeLongDouble);

			fileWrite.close();

			xablau::io::fstream < typename TypeParam::value_type > fileRead(
				this->fullPath,
				std::ios_base::in | std::ios_base::binary);

			signed char _readSignedChar{};
			unsigned char _readUnsignedChar{};
			short int _readShortInt{};
			unsigned short int _readUnsignedShortInt{};
			int _readInt{};
			unsigned int _readUnsignedInt{};
			long int _readLongInt{};
			unsigned long int _readUnsignedLongInt{};
			long long int _readLongLongInt{};
			unsigned long long int _readUnsignedLongLongInt{};
			bool _readBool{};
			char8_t _readChar8_t{};
			char16_t _readChar16_t{};
			char32_t _readChar32_t{};
			float _readFloat{};
			double _readDouble{};
			long double _readLongDouble{};

			fileRead.read(_readSignedChar);
			fileRead.read(_readUnsignedChar);
			fileRead.read(_readShortInt);
			fileRead.read(_readUnsignedShortInt);
			fileRead.read(_readInt);
			fileRead.read(_readUnsignedInt);
			fileRead.read(_readLongInt);
			fileRead.read(_readUnsignedLongInt);
			fileRead.read(_readLongLongInt);
			fileRead.read(_readUnsignedLongLongInt);
			fileRead.read(_readBool);
			fileRead.read(_readChar8_t);
			fileRead.read(_readChar16_t);
			fileRead.read(_readChar32_t);
			fileRead.read(_readFloat);
			fileRead.read(_readDouble);
			fileRead.read(_readLongDouble);

			EXPECT_EQ(_readSignedChar, _writeSignedChar);
			EXPECT_EQ(_readUnsignedChar, _writeUnsignedChar);
			EXPECT_EQ(_readShortInt, _writeShortInt);
			EXPECT_EQ(_readUnsignedShortInt, _writeUnsignedShortInt);
			EXPECT_EQ(_readInt, _writeInt);
			EXPECT_EQ(_readUnsignedInt, _writeUnsignedInt);
			EXPECT_EQ(_readLongInt, _writeLongInt);
			EXPECT_EQ(_readUnsignedLongInt, _writeUnsignedLongInt);
			EXPECT_EQ(_readLongLongInt, _writeLongLongInt);
			EXPECT_EQ(_readUnsignedLongLongInt, _writeUnsignedLongLongInt);
			EXPECT_EQ(_readBool, _writeBool);
			EXPECT_EQ(_readChar8_t, _writeChar8_t);
			EXPECT_EQ(_readChar16_t, _writeChar16_t);
			EXPECT_EQ(_readChar32_t, _writeChar32_t);
			EXPECT_EQ(_readFloat, _writeFloat);
			EXPECT_EQ(_readDouble, _writeDouble);
			EXPECT_EQ(_readLongDouble, _writeLongDouble);
		}
	}

	TYPED_TEST_P(IOFileStreamTest, StringWithoutDelimiter)
	{
		const auto writeString = this->createStringFull();

		xablau::io::fstream < typename TypeParam::value_type > fileWrite(
			this->fullPath,
			std::ios_base::out | std::ios_base::binary);

		fileWrite.write(writeString);

		fileWrite.close();

		xablau::io::fstream < typename TypeParam::value_type > fileRead(
			this->fullPath,
			std::ios_base::in | std::ios_base::binary);

		std::basic_string < typename TypeParam::value_type > readString;

		fileRead.read(readString);

		EXPECT_EQ(writeString, readString);
	}

	TYPED_TEST_P(IOFileStreamTest, StringWithDelimiter)
	{
		constexpr auto delimiter =
			[] (const typename TypeParam::value_type data) -> bool
			{
				if constexpr (std::same_as < typename TypeParam::value_type, char >)
				{
					if (data == '.')
					{
						return true;
					}
				}

				else
				{
					if (data == L'.')
					{
						return true;
					}
				}

				return false;
			};

		const auto writeString = this->createStringFull();

		xablau::io::fstream < typename TypeParam::value_type > fileWrite(
			this->fullPath,
			std::ios_base::out | std::ios_base::binary);

		fileWrite.write(writeString);

		fileWrite.close();

		xablau::io::fstream < typename TypeParam::value_type > fileRead(
			this->fullPath,
			std::ios_base::in | std::ios_base::binary);

		std::basic_string < typename TypeParam::value_type > readString;

		fileRead.read(readString, delimiter);

		EXPECT_EQ(this->createStringDelimited(), readString);
	}

	TYPED_TEST_P(IOFileStreamTest, Array)
	{
		constexpr std::array < int, 10 > array
			{
				2, 3, 5, 7, 11,
				13, 17, 19, 23, 29
			};

		this->validateContainer(array);
	}

	TYPED_TEST_P(IOFileStreamTest, Vector)
	{
		const std::vector < int > vector
			{
				2, 3, 5, 7, 11,
				13, 17, 19, 23, 29
			};

		this->validateContainer(vector);
	}

	TYPED_TEST_P(IOFileStreamTest, Deque)
	{
		const std::deque < int > deque
			{
				2, 3, 5, 7, 11,
				13, 17, 19, 23, 29
			};

		this->validateContainer(deque);
	}

	TYPED_TEST_P(IOFileStreamTest, ForwardList)
	{
		const std::forward_list < int > forwardList
			{
				2, 3, 5, 7, 11,
				13, 17, 19, 23, 29
			};

		this->validateContainer(forwardList);
	}

	TYPED_TEST_P(IOFileStreamTest, List)
	{
		const std::list < int > list
			{
				2, 3, 5, 7, 11,
				13, 17, 19, 23, 29
			};

		this->validateContainer(list);
	}

	TYPED_TEST_P(IOFileStreamTest, Pair)
	{
		this->validateContainer(std::make_pair(2, 3));
	}

	TYPED_TEST_P(IOFileStreamTest, Set)
	{
		const std::set < int > set
			{
				2, 3, 5, 7, 11,
				13, 17, 19, 23, 29
			};

		this->validateContainer(set);
	}

	TYPED_TEST_P(IOFileStreamTest, Multiset)
	{
		const std::multiset < int > multiset
			{
				2, 3, 5, 7, 11,
				13, 17, 19, 23, 29,
				2, 3, 5, 7, 11,
				13, 17, 19, 23, 29
			};

		this->validateContainer(multiset);
	}

	TYPED_TEST_P(IOFileStreamTest, Map)
	{
		const std::map < int, int > map
			{
				std::make_pair(2, 3),
				std::make_pair(5, 7),
				std::make_pair(11, 13),
				std::make_pair(17, 19),
				std::make_pair(23, 29)
			};

		this->validateContainer(map);
	}

	TYPED_TEST_P(IOFileStreamTest, Multimap)
	{
		const std::multimap < int, int > multimap
			{
				std::make_pair(2, 3),
				std::make_pair(5, 7),
				std::make_pair(11, 13),
				std::make_pair(17, 19),
				std::make_pair(23, 29),
				std::make_pair(2, 3),
				std::make_pair(5, 7),
				std::make_pair(11, 13),
				std::make_pair(17, 19),
				std::make_pair(23, 29)
			};

		this->validateContainer(multimap);
	}

	TYPED_TEST_P(IOFileStreamTest, UnorderedSet)
	{
		const std::unordered_set < int > unorderedSet
			{
				2, 3, 5, 7, 11,
				13, 17, 19, 23, 29
			};

		this->validateContainer(unorderedSet);
	}

	TYPED_TEST_P(IOFileStreamTest, UnorderedMultiset)
	{
		const std::unordered_multiset < int > unorderedMultiset
			{
				2, 3, 5, 7, 11,
				13, 17, 19, 23, 29,
				2, 3, 5, 7, 11,
				13, 17, 19, 23, 29
			};

		this->validateContainer(unorderedMultiset);
	}

	TYPED_TEST_P(IOFileStreamTest, UnorderedMap)
	{
		const std::unordered_map < int, int > unorderedMap
			{
				std::make_pair(2, 3),
				std::make_pair(5, 7),
				std::make_pair(11, 13),
				std::make_pair(17, 19),
				std::make_pair(23, 29)
			};

		this->validateContainer(unorderedMap);
	}

	TYPED_TEST_P(IOFileStreamTest, UnorderedMultimap)
	{
		const std::unordered_multimap < int, int > unorderedMultimap
			{
				std::make_pair(2, 3),
				std::make_pair(5, 7),
				std::make_pair(11, 13),
				std::make_pair(17, 19),
				std::make_pair(23, 29),
				std::make_pair(2, 3),
				std::make_pair(5, 7),
				std::make_pair(11, 13),
				std::make_pair(17, 19),
				std::make_pair(23, 29)
			};

		this->validateContainer(unorderedMultimap);
	}

	TYPED_TEST_P(IOFileStreamTest, Stack)
	{
		std::stack < int > stack;

		stack.push(2);
		stack.push(3);
		stack.push(5);
		stack.push(7);
		stack.push(11);
		stack.push(13);
		stack.push(17);
		stack.push(19);
		stack.push(23);
		stack.push(29);

		this->validateContainer(stack);
	}

	TYPED_TEST_P(IOFileStreamTest, Queue)
	{
		std::queue < int > queue;

		queue.push(2);
		queue.push(3);
		queue.push(5);
		queue.push(7);
		queue.push(11);
		queue.push(13);
		queue.push(17);
		queue.push(19);
		queue.push(23);
		queue.push(29);

		this->validateContainer(queue);
	}

	TYPED_TEST_P(IOFileStreamTest, PriorityQueue)
	{
		std::priority_queue < int > priorityQueue;

		priorityQueue.push(2);
		priorityQueue.push(3);
		priorityQueue.push(5);
		priorityQueue.push(7);
		priorityQueue.push(11);
		priorityQueue.push(13);
		priorityQueue.push(17);
		priorityQueue.push(19);
		priorityQueue.push(23);
		priorityQueue.push(29);

		this->validateContainerForwardList(priorityQueue);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		IOFileStreamTest,
		FundamentalTypes,
		StringWithoutDelimiter,
		StringWithDelimiter,
		Array,
		Vector,
		Deque,
		ForwardList,
		List,
		Pair,
		Set,
		Multiset,
		Map,
		Multimap,
		UnorderedSet,
		UnorderedMultiset,
		UnorderedMap,
		UnorderedMultimap,
		Stack,
		Queue,
		PriorityQueue);

	using IOFileStreamTestDataTypes = std::tuple < char, wchar_t >;

	using IOFileStreamTestGroup =
		xablau::testing::test_bundler < IOFileStreamTestDataTypes > ::template test_group < IOFileStreamTypeDefinitions >;

	using IOFileStreamTypes = xablau::testing::tuple_transformation < ::testing::Types, IOFileStreamTestGroup > ::types;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		Xablau,
		IOFileStreamTest,
		IOFileStreamTypes);
}
