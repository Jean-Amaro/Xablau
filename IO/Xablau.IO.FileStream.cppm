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

export module xablau.io:fstream;

export import std;

export namespace xablau::io
{
	template < typename CharType, typename Traits = std::char_traits < CharType > >
	requires (std::same_as < CharType, typename Traits::char_type >)
	class fstream :
		public std::basic_fstream < CharType, Traits >
	{
	private:
		template < typename Type, typename Container >
		class derived_stack :
			public std::stack < Type, Container >
		{
		public:
			const Container &container()
			{
				return this->c;
			}

			explicit derived_stack(const std::stack < Type, Container > &stack) :
				std::stack < Type, Container > (stack)
			{
			}
		};

		template < typename Type, typename Container >
		class derived_queue :
			public std::queue < Type, Container >
		{
		public:
			const Container &container()
			{
				return this->c;
			}

			explicit derived_queue(const std::queue < Type, Container > &queue) :
				std::queue < Type, Container > (queue)
			{
			}
		};

		template < typename Type, typename Container, typename Compare >
		class derived_priority_queue :
			public std::priority_queue < Type, Container, Compare >
		{
		public:
			const Container &container()
			{
				return this->c;
			}

			explicit derived_priority_queue(const std::priority_queue < Type, Container, Compare > &priorityQueue) :
				std::priority_queue < Type, Container, Compare > (priorityQueue)
			{
			}
		};

	protected:
		std::filesystem::path _xablau_filename;

	public:
		using std::basic_fstream < CharType, Traits > ::char_type;
		using std::basic_fstream < CharType, Traits > ::traits_type;
		using std::basic_fstream < CharType, Traits > ::int_type;
		using std::basic_fstream < CharType, Traits > ::pos_type;
		using std::basic_fstream < CharType, Traits > ::off_type;

		using std::basic_fstream < CharType, Traits > ::read;
		using std::basic_fstream < CharType, Traits > ::write;

		template < typename Type >
		requires (
			!std::is_pointer < Type > ::value && (
				std::is_array < Type > ::value ||
				std::same_as < CharType, char >))
		fstream &read(Type &data)
		{
			this->read(reinterpret_cast < CharType * > (&data), sizeof(Type));

			return *this;
		}

		template < typename Allocator >
		fstream &read(std::basic_string < CharType, Traits, Allocator > &string)
		{
			while (this->good())
			{
				CharType character{};

				this->get(character);

				if (character == CharType{'\0'})
				{
					break;
				}

				string.push_back(character);
			}

			return *this;
		}

		template < typename Allocator >
		fstream &read(
			std::basic_string < CharType, Traits, Allocator > &string,
			const std::function < bool(const CharType) > &delimiter)
		{
			while (this->good())
			{
				CharType character{};

				this->get(character);

				if (delimiter(character))
				{
					break;
				}

				string.push_back(character);
			}

			return *this;
		}

		template < typename Type, std::size_t Size >
		fstream &read(std::array < Type, Size > &array)
		{
			for (std::size_t i = 0; i < Size; i++)
			{
				this->read(array[i]);
			}

			return *this;
		}

		template < std::default_initializable Type, typename Allocator >
		fstream &read(std::vector < Type, Allocator > &vector)
		{
			using localSizeType =
				typename std::vector < Type, Allocator > ::size_type;

			localSizeType elementCount{};

			this->read(elementCount);

			vector.reserve(elementCount + vector.size());

			for (localSizeType i = 0; i < elementCount; i++)
			{
				Type data{};

				this->read(data);

				vector.emplace_back(std::move(data));
			}

			return *this;
		}

		template < std::default_initializable Type, typename Allocator >
		fstream &read(std::deque < Type, Allocator > &deque)
		{
			using localSizeType =
				typename std::deque < Type, Allocator > ::size_type;

			localSizeType elementCount{};

			this->read(elementCount);

			for (localSizeType i = 0; i < elementCount; i++)
			{
				Type data{};

				this->read(data);

				deque.emplace_back(std::move(data));
			}

			return *this;
		}

		template < std::default_initializable Type, typename Allocator >
		fstream &read(std::forward_list < Type, Allocator > &forwardList)
		{
			size_t elementCount{};

			this->read(elementCount);

			std::vector < Type, Allocator > stackContainer;

			stackContainer.reserve(elementCount);

			std::stack < Type, std::vector < Type, Allocator > > stack(std::move(stackContainer));

			for (size_t i = 0; i < elementCount; i++)
			{
				Type data{};

				this->read(data);

				stack.push(std::move(data));
			}

			while (!stack.empty())
			{
				forwardList.push_front(std::move(stack.top()));

				stack.pop();
			}

			return *this;
		}

		template < std::default_initializable Type, typename Allocator >
		fstream &read(std::list < Type, Allocator > &list)
		{
			using localSizeType =
				typename std::list < Type, Allocator > ::size_type;

			localSizeType elementCount{};

			this->read(elementCount);

			for (localSizeType i = 0; i < elementCount; i++)
			{
				Type data{};

				this->read(data);

				list.emplace_back(std::move(data));
			}

			return *this;
		}

		template < typename Type1, typename Type2 >
		fstream &read(std::pair < Type1, Type2 > &pair)
		{
			this->read(pair.first);
			this->read(pair.second);

			return *this;
		}

		template < std::default_initializable Type, typename LessThan, typename Allocator >
		fstream &read(std::set < Type, LessThan, Allocator > &set)
		{
			using localSizeType =
				typename std::set < Type, LessThan, Allocator > ::size_type;

			localSizeType elementCount{};

			this->read(elementCount);

			for (localSizeType i = 0; i < elementCount; i++)
			{
				Type data{};

				this->read(data);

				set.emplace(std::move(data));
			}

			return *this;
		}

		template < std::default_initializable Type, typename LessThan, typename Allocator >
		fstream &read(std::multiset < Type, LessThan, Allocator > &multiset)
		{
			using localSizeType =
				typename std::multiset < Type, LessThan, Allocator > ::size_type;

			localSizeType elementCount{};

			this->read(elementCount);

			for (localSizeType i = 0; i < elementCount; i++)
			{
				Type data{};

				this->read(data);

				multiset.emplace(std::move(data));
			}

			return *this;
		}

		template < typename Type1, typename Type2, typename LessThan, typename Allocator >
		fstream &read(std::map < Type1, Type2, LessThan, Allocator > &map)
		{
			using localSizeType =
				typename std::map < Type1, Type2, LessThan, Allocator > ::size_type;

			localSizeType pairCount{};

			this->read(pairCount);

			for (localSizeType i = 0; i < pairCount; i++)
			{
				std::pair < Type1, Type2 > pair{};

				this->read(pair);

				map.emplace(std::move(pair));
			}

			return *this;
		}

		template < typename Type1, typename Type2, typename LessThan, typename Allocator >
		fstream &read(std::multimap < Type1, Type2, LessThan, Allocator > &multimap)
		{
			using localSizeType =
				typename std::multimap < Type1, Type2, LessThan, Allocator > ::size_type;

			localSizeType pairCount{};

			this->read(pairCount);

			for (localSizeType i = 0; i < pairCount; i++)
			{
				std::pair < Type1, Type2 > pair{};

				this->read(pair);

				multimap.emplace(std::move(pair));
			}

			return *this;
		}

		template < std::default_initializable Type, typename Hasher, typename EqualTo, typename Allocator >
		fstream &read(std::unordered_set < Type, Hasher, EqualTo, Allocator > &unorderedSet)
		{
			using localSizeType =
				typename std::unordered_set < Type, Hasher, EqualTo, Allocator > ::size_type;

			localSizeType elementCount{};

			this->read(elementCount);

			unorderedSet.reserve(unorderedSet.size() + elementCount);

			for (localSizeType i = 0; i < elementCount; i++)
			{
				Type data{};

				this->read(data);

				unorderedSet.emplace(std::move(data));
			}

			return *this;
		}

		template < std::default_initializable Type, typename Hasher, typename EqualTo, typename Allocator >
		fstream &read(std::unordered_multiset < Type, Hasher, EqualTo, Allocator > &unorderedMultiset)
		{
			using localSizeType =
				typename std::unordered_multiset < Type, Hasher, EqualTo, Allocator > ::size_type;

			localSizeType elementCount{};

			this->read(elementCount);

			unorderedMultiset.reserve(unorderedMultiset.size() + elementCount);

			for (localSizeType i = 0; i < elementCount; i++)
			{
				Type data{};

				this->read(data);

				unorderedMultiset.emplace(std::move(data));
			}

			return *this;
		}

		template < typename Type1, typename Type2, typename Hasher, typename EqualTo, typename Allocator >
		fstream &read(std::unordered_map < Type1, Type2, Hasher, EqualTo, Allocator > &unorderedMap)
		{
			using localSizeType =
				typename std::unordered_map < Type1, Type2, Hasher, EqualTo, Allocator > ::size_type;

			localSizeType pairCount{};

			this->read(pairCount);

			unorderedMap.reserve(unorderedMap.size() + pairCount);

			for (localSizeType i = 0; i < pairCount; i++)
			{
				std::pair < Type1, Type2 > pair{};

				this->read(pair);

				unorderedMap.emplace(std::move(pair));
			}

			return *this;
		}

		template < typename Type1, typename Type2, typename Hasher, typename EqualTo, typename Allocator >
		fstream &read(std::unordered_multimap < Type1, Type2, Hasher, EqualTo, Allocator > &unorderedMultimap)
		{
			using localSizeType =
				typename std::unordered_multimap < Type1, Type2, Hasher, EqualTo, Allocator > ::size_type;

			localSizeType pairCount{};

			this->read(pairCount);

			for (localSizeType i = 0; i < pairCount; i++)
			{
				std::pair < Type1, Type2 > pair;

				this->read(pair);

				unorderedMultimap.emplace(pair);
			}

			return *this;
		}

		template < std::default_initializable Type, typename Container >
		fstream &read(std::stack < Type, Container > &stack)
		{
			using localSizeType =
				typename std::stack < Type, Container > ::size_type;

			localSizeType elementCount{};

			this->read(elementCount);

			for (localSizeType i = 0; i < elementCount; i++)
			{
				Type data{};

				this->read(data);

				stack.push(std::move(data));
			}

			return *this;
		}

		template < std::default_initializable Type, typename Container >
		fstream &read(std::queue < Type, Container > &queue)
		{
			using localSizeType =
				typename std::queue < Type, Container > ::size_type;

			localSizeType elementCount{};

			this->read(elementCount);

			for (localSizeType i = 0; i < elementCount; i++)
			{
				Type data{};

				this->read(data);

				queue.push(std::move(data));
			}

			return *this;
		}

		template < std::default_initializable Type, typename Container, typename Compare >
		fstream &read(std::priority_queue < Type, Container, Compare > &priorityQueue)
		{
			using localSizeType =
				typename std::priority_queue < Type, Container, Compare > ::size_type;

			localSizeType elementCount{};

			this->read(elementCount);

			for (localSizeType i = 0; i < elementCount; i++)
			{
				Type data{};

				this->read(data);

				priorityQueue.push(std::move(data));
			}

			return *this;
		}

		template < typename Type >
		requires (
			!std::is_pointer < Type > ::value && (
				std::is_array < Type > ::value ||
				std::same_as < CharType, char >))
		fstream &write(const Type &data)
		{
			this->write(reinterpret_cast < const CharType * > (&data), sizeof(Type));

			return *this;
		}

		template <
			bool WriteNullCharacter = true,
			typename Allocator >
		fstream &write(const std::basic_string < CharType, Traits, Allocator > &string)
		{
			this->write(string.c_str(), string.size());

			if constexpr (WriteNullCharacter)
			{
				this->put(CharType{});
			}

			return *this;
		}

		template < bool WriteNullCharacter = true >
		fstream &write(const CharType *string)
		{
			size_t length{};

			if constexpr (std::same_as < CharType, char >)
			{
				length = std::strlen(string);
			}

			else if constexpr (std::same_as < CharType, wchar_t >)
			{
				length = std::wcslen(string);
			}

			else
			{
				throw std::logic_error(""); // TODO: Create message.
			}

			this->write(string, length);

			if constexpr (WriteNullCharacter)
			{
				this->put(CharType{});
			}

			return *this;
		}

		template <
			bool WriteNullCharacter = true,
			typename Allocator >
		fstream &write(const std::basic_string_view < CharType, Traits > &string)
		{
			this->write(string.data(), string.size());

			if constexpr (WriteNullCharacter)
			{
				this->put(CharType{});
			}

			return *this;
		}

		template < typename Type, std::size_t Size >
		fstream &write(const std::array < Type, Size > &array)
		{
			for (const auto &data : array)
			{
				this->write(data);
			}

			return *this;
		}

		template < typename Type, typename Allocator >
		fstream &write(const std::vector < Type, Allocator > &vector)
		{
			this->write(vector.size());

			for (const auto &data : vector)
			{
				this->write(data);
			}

			return *this;
		}

		template < typename Type, typename Allocator >
		fstream &write(const std::deque < Type, Allocator > &deque)
		{
			this->write(deque.size());

			for (const auto &data : deque)
			{
				this->write(data);
			}

			return *this;
		}

		template < typename Type, typename Allocator >
		fstream &write(const std::forward_list < Type, Allocator > &forwardList)
		{
			this->write(std::ranges::distance(forwardList));

			for (const auto &data : forwardList)
			{
				this->write(data);
			}

			return *this;
		}

		template < typename Type, typename Allocator >
		fstream &write(const std::list < Type, Allocator > &list)
		{
			this->write(list.size());

			for (const auto &data : list)
			{
				this->write(data);
			}

			return *this;
		}

		template < typename Type1, typename Type2 >
		fstream &write(const std::pair < Type1, Type2 > &pair)
		{
			this->write(pair.first);
			this->write(pair.second);

			return *this;
		}

		template < typename Type, typename LessThan, typename Allocator >
		fstream &write(const std::set < Type, LessThan, Allocator > &set)
		{
			this->write(set.size());

			for (const auto &data : set)
			{
				this->write(data);
			}

			return *this;
		}

		template < typename Type, typename LessThan, typename Allocator >
		fstream &write(const std::multiset < Type, LessThan, Allocator > &multiset)
		{
			this->write(multiset.size());

			for (const auto &data : multiset)
			{
				this->write(data);
			}

			return *this;
		}

		template < typename Type1, typename Type2, typename LessThan, typename Allocator >
		fstream &write(const std::map < Type1, Type2, LessThan, Allocator > &map)
		{
			this->write(map.size());

			for (const auto &pair : map)
			{
				this->write(pair);
			}

			return *this;
		}

		template < typename Type1, typename Type2, typename LessThan, typename Allocator >
		fstream &write(const std::multimap < Type1, Type2, LessThan, Allocator > &multimap)
		{
			this->write(multimap.size());

			for (const auto &pair : multimap)
			{
				this->write(pair);
			}

			return *this;
		}

		template < typename Type, typename Hasher, typename EqualTo, typename Allocator >
		fstream &write(const std::unordered_set < Type, Hasher, EqualTo, Allocator > &unorderedSet)
		{
			this->write(unorderedSet.size());

			for (const auto &data : unorderedSet)
			{
				this->write(data);
			}

			return *this;
		}

		template < typename Type, typename Hasher, typename EqualTo, typename Allocator >
		fstream &write(const std::unordered_multiset < Type, Hasher, EqualTo, Allocator > &unorderedMultiset)
		{
			this->write(unorderedMultiset.size());

			for (const auto &data : unorderedMultiset)
			{
				this->write(data);
			}

			return *this;
		}

		template < typename Type1, typename Type2, typename Hasher, typename EqualTo, typename Allocator >
		fstream &write(const std::unordered_map < Type1, Type2, Hasher, EqualTo, Allocator > &unorderedMap)
		{
			this->write(unorderedMap.size());

			for (const auto &pair : unorderedMap)
			{
				this->write(pair);
			}

			return *this;
		}

		template < typename Type1, typename Type2, typename Hasher, typename EqualTo, typename Allocator >
		fstream &write(const std::unordered_multimap < Type1, Type2, Hasher, EqualTo, Allocator > &unorderedMultimap)
		{
			this->write(unorderedMultimap.size());

			for (const auto &pair : unorderedMultimap)
			{
				this->write(pair);
			}

			return *this;
		}

		template < std::copy_constructible Type, std::ranges::forward_range Container >
		fstream &write(const std::stack < Type, Container > &stack)
		{
			derived_stack < Type, Container > _stack(stack);
			const auto &container = _stack.container();

			this->write(stack.size());

			for (const auto &data : container)
			{
				this->write(data);
			}

			return *this;
		}

		template < std::copy_constructible Type, typename Container >
		fstream &write(const std::queue < Type, Container > &queue)
		{
			derived_queue < Type, Container > _queue(queue);
			const auto &container = _queue.container();

			this->write(queue.size());

			for (const auto &data : container)
			{
				this->write(data);
			}

			return *this;
		}

		template < std::copy_constructible Type, typename Container, typename Compare >
		fstream &write(const std::priority_queue < Type, Container, Compare > &priorityQueue)
		{
			derived_priority_queue < Type, Container, Compare > _priorityQueue(priorityQueue);
			const auto &container = _priorityQueue.container();

			this->write(priorityQueue.size());

			for (const auto &data : container)
			{
				this->write(data);
			}

			return *this;
		}

		const std::filesystem::path &filename() const noexcept
		{
			return this->_xablau_filename;
		}

		fstream() = default;

		explicit fstream(
			const char *filename,
			std::ios_base::openmode openMode = std::ios_base::in | std::ios_base::out) :

			std::basic_fstream < CharType, Traits > (filename, openMode),

			_xablau_filename(filename)
		{
		}

		explicit fstream(
			const wchar_t *filename,
			std::ios_base::openmode openMode = std::ios_base::in | std::ios_base::out) :

			std::basic_fstream < CharType, Traits > (filename, openMode),

			_xablau_filename(filename)
		{
		}

		explicit fstream(
			const std::string &filename,
			std::ios_base::openmode openMode = std::ios_base::in | std::ios_base::out) :

			std::basic_fstream < CharType, Traits > (filename, openMode),

			_xablau_filename(filename)
		{
		}

		template < class FsPath >
		explicit fstream(
			const FsPath &filename,
			std::ios_base::openmode openMode = std::ios_base::in | std::ios_base::out) :

			std::basic_fstream < CharType, Traits > (filename, openMode),

			_xablau_filename(filename)
		{
		}

		fstream(const fstream &) = delete;

		fstream(fstream &&) noexcept = default;

		fstream &operator=(const fstream &) = delete;

		fstream &operator=(fstream &&) noexcept = default;

		~fstream() noexcept override = default;
	};
}