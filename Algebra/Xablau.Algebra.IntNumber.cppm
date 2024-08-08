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

export module xablau.algebra:int_number;
export import :fundamental_concepts;
export import :fundamental_configurations;

export import std;

export namespace xablau::algebra
{
	class int_number
	{
	private:
		static const int_number _one;

		std::vector < bool > _number = std::vector < bool > (1, false);

		static constexpr void convert_factors_to_positive(int_number &lhs, bool &bitSignLHS, int_number &rhs, bool &bitSignRHS)
		{
			bitSignLHS = lhs.bit_sign();
			bitSignRHS = rhs.bit_sign();

			if (bitSignLHS)
			{
				lhs.negate();
			}

			if (bitSignRHS)
			{
				rhs.negate();
			}
		}

		static constexpr void restore_sign(int_number &result, const bool bitSignLHS, const bool bitSignRHS)
		{
			if (bitSignLHS ^ bitSignRHS)
			{
				result.negate();
			}
		}

		template < typename IntegerType >
		constexpr void set(const IntegerType value)
		{
			constexpr std::size_t bitCount = sizeof(IntegerType) * 8;

			this->_number.resize(bitCount);

			for (std::size_t i = 0; i < bitCount; i++)
			{
				this->_number[i] = (value & (IntegerType{1} << i)) != 0;
			}

			this->lean();
		}

		constexpr void shift_insert(const typename std::vector < bool > ::size_type bitCount)
		{
			this->_number.insert(this->_number.begin(), bitCount, false);
		}

		constexpr void shift_erase(const typename std::vector < bool > ::size_type bitCount)
		{
			const auto limit = this->_number.size() - 1;
			const auto end = bitCount < limit ? this->_number.begin() + bitCount : this->_number.begin() + limit;

			this->_number.erase(this->_number.begin(), end);
		}

		constexpr bool equal_to(const int_number &other) const
		{
			std::vector < bool > ::size_type i = 0;
			const auto min = std::min(this->_number.size(), other._number.size());

			for (; i < min; i++)
			{
				if (this->_number[i] != other._number[i])
				{
					return false;
				}
			}

			for (; i < this->_number.size(); i++)
			{
				if (this->_number[i] != other._number.back())
				{
					return false;
				}
			}

			for (; i < other._number.size(); i++)
			{
				if (this->_number.back() != other._number[i])
				{
					return false;
				}
			}

			return true;
		}

		static constexpr sign_value inequality_comparison_operator(const int_number &lhs, const int_number &rhs)
		{
			const auto leftSize = lhs._number.size();
			const auto rightSize = rhs._number.size();
			const auto &[min, max] = std::minmax(leftSize, rightSize);
			const auto limit = max - min;
			const bool rhsWithGreaterSize = rhs._number.size() > lhs._number.size();
			auto iteratorLHS = lhs._number.crbegin();
			auto iteratorRHS = rhs._number.crbegin();

			for (std::vector < bool > ::size_type i = 0; i < limit; i++)
			{
				if (*iteratorLHS < *iteratorRHS)
				{
					return sign_value::positive;
				}

				if (*iteratorRHS < *iteratorLHS)
				{
					return sign_value::negative;
				}

				if (rhsWithGreaterSize)
				{
					++iteratorRHS;
				}

				else
				{
					++iteratorLHS;
				}
			}

			while (iteratorLHS != lhs._number.crend())
			{
				if (*iteratorLHS < *iteratorRHS)
				{
					return sign_value::positive;
				}

				if (*iteratorRHS < *iteratorLHS)
				{
					return sign_value::negative;
				}

				++iteratorLHS;
				++iteratorRHS;
			}

			return sign_value::zero;
		}

		constexpr bool lesser_than(const int_number &other) const
		{
			const bool negativeLHS = this->negative();
			const bool negativeRHS = other.negative();

			if (negativeLHS && !negativeRHS)
			{
				return true;
			}

			if (!negativeLHS && negativeRHS)
			{
				return false;
			}

			switch (int_number::inequality_comparison_operator(*this, other))
			{
			case sign_value::negative:
				return false;
			case sign_value::positive:
				return true;
			default:
				return false;
			}
		}

		constexpr bool lesser_or_equal_than(const int_number &other) const
		{
			const bool negativeLHS = this->negative();
			const bool negativeRHS = other.negative();

			if (negativeLHS && !negativeRHS)
			{
				return true;
			}

			if (!negativeLHS && negativeRHS)
			{
				return false;
			}

			switch (int_number::inequality_comparison_operator(*this, other))
			{
			case sign_value::negative:
				return false;
			case sign_value::positive:
				return true;
			default:
				return true;
			}
		}

		constexpr bool greater_than(const int_number &other) const
		{
			const bool negativeLHS = this->negative();
			const bool negativeRHS = other.negative();

			if (negativeLHS && !negativeRHS)
			{
				return false;
			}

			if (!negativeLHS && negativeRHS)
			{
				return true;
			}

			switch (int_number::inequality_comparison_operator(*this, other))
			{
			case sign_value::negative:
				return true;
			case sign_value::positive:
				return false;
			default:
				return false;
			}
		}

		constexpr bool greater_or_equal_than(const int_number &other) const
		{
			const bool negativeLHS = this->negative();
			const bool negativeRHS = other.negative();

			if (negativeLHS && !negativeRHS)
			{
				return false;
			}

			if (!negativeLHS && negativeRHS)
			{
				return true;
			}

			switch (int_number::inequality_comparison_operator(*this, other))
			{
			case sign_value::negative:
				return true;
			case sign_value::positive:
				return false;
			default:
				return true;
			}
		}

		constexpr std::vector < bool > ::difference_type most_significant_true_bit_position() const
		{
			const auto iterator = std::find(this->_number.crbegin(), this->_number.crend(), true);

			if (iterator == this->_number.crend())
			{
				return -1;
			}

			return std::distance(iterator, this->_number.crend());
		}

		constexpr static void format(std::string &formattedString, int i, auto &iterator)
		{
			unsigned char byte{};

			for (; i >= 0; i--)
			{
				byte |= (static_cast < unsigned char > (*iterator) << i);

				++iterator;
			}

			std::format_to(std::back_inserter(formattedString), "{:02X}", byte);
		}

		constexpr static void format(std::string &formattedString, int i, const bool signal, auto &iterator)
		{
			unsigned char byte{};

			for (int j = 7; j > i; j--)
			{
				byte |= (static_cast < unsigned char > (signal) << j);
			}

			for (; i >= 0; i--)
			{
				byte |= (static_cast < unsigned char > (*iterator) << i);

				++iterator;
			}

			std::format_to(std::back_inserter(formattedString), "{:02X}", byte);
		}

	public:
		constexpr bool negative() const
		{
			return this->_number.back();
		}

		constexpr bool null() const
		{
			return std::find(this->_number.cbegin(), this->_number.cend(), true) == this->_number.cend();
		}

		constexpr bool positive() const
		{
			return
				!this->_number.back() &&
				std::find(this->_number.crbegin() + 1, this->_number.crend(), true) != this->_number.crend();
		}

		constexpr bool bit_sign() const
		{
			return this->_number.back();
		}

		constexpr int_number &negate()
		{
			this->_number.flip();

			return this->add(int_number::_one);
		}

		constexpr int_number &shift_left(const typename std::vector < bool > ::difference_type bitCount)
		{
			if (bitCount > 0)
			{
				this->shift_insert(static_cast < typename std::vector < bool > ::size_type > (bitCount));

				this->lean();
			}

			else if (bitCount < 0)
			{
				this->shift_erase(static_cast < typename std::vector < bool > ::size_type > (-bitCount));
			}

			return *this;
		}

		constexpr int_number &shift_right(const typename std::vector < bool > ::difference_type bitCount)
		{
			return this->shift_left(-bitCount);
		}

		constexpr int_number &bitwise_not()
		{
			this->_number.flip();

			return *this;
		}

		constexpr int_number &bitwise_or(const int_number &other)
		{
			if (other._number.size() > this->_number.size())
			{
				this->_number.resize(other._number.size(), this->_number.back());
			}

			std::vector < bool > ::size_type i = 0;
			const bool signRHS = other._number.back();

			for (; i < other._number.size(); i++)
			{
				this->_number[i] = this->_number[i] | other._number[i];
			}

			for (; i < this->_number.size(); i++)
			{
				this->_number[i] = this->_number[i] | signRHS;
			}

			this->lean();

			return *this;
		}

		constexpr int_number &bitwise_and(const int_number &other)
		{
			if (other._number.size() > this->_number.size())
			{
				this->_number.resize(other._number.size(), this->_number.back());
			}

			std::vector < bool > ::size_type i = 0;
			const bool signRHS = other._number.back();

			for (; i < other._number.size(); i++)
			{
				this->_number[i] = this->_number[i] & other._number[i];
			}

			for (; i < this->_number.size(); i++)
			{
				this->_number[i] = this->_number[i] & signRHS;
			}

			this->lean();

			return *this;
		}

		constexpr int_number &add(const int_number &other)
		{
			const bool signLHS = this->_number.back();
			const bool signRHS = other._number.back();
			const bool nonNegativeAddends = !signLHS && !signRHS;
			const bool negativeAddends = signLHS && signRHS;

			if (other._number.size() > this->_number.size())
			{
				this->_number.resize(other._number.size(), signLHS);
			}

			std::vector < bool > ::size_type i = 0;
			bool carry = false;

			for (; i < other._number.size(); i++)
			{
				const bool lhs = this->_number[i];
				const bool rhs = other._number[i];
				const bool result = lhs ^ rhs ^ carry;

				carry = (lhs && rhs) || (carry && (lhs || rhs));
				this->_number[i] = result;
			}

			for (; i < this->_number.size(); i++)
			{
				const bool lhs = this->_number[i];
				const bool result = lhs ^ signRHS ^ carry;

				carry = (lhs && signRHS) || (carry && (lhs || signRHS));
				this->_number[i] = result;
			}

			if (nonNegativeAddends)
			{
				if (carry)
				{
					this->_number.push_back(true);
					this->_number.push_back(false);
				}

				else if (this->bit_sign())
				{
					this->_number.push_back(false);
				}
			}

			else if (negativeAddends && !this->bit_sign())
			{
				this->_number.push_back(true);
			}

			this->lean();

			return *this;
		}

		constexpr int_number &subtract(const int_number &other)
		{
			int_number negativeOther(other);

			negativeOther.negate();

			this->add(negativeOther);

			return *this;
		}

		constexpr int_number &multiply(const int_number &factor)
		{
			int_number result{};

			for (std::vector < bool > ::size_type i = 0; i < factor._number.size() - 1; i++)
			{
				if (!factor[i])
				{
					continue;
				}

				int_number addend = *this;

				addend._number.insert(addend._number.cbegin(), i, false);

				result.add(addend);
			}

			if (factor.bit_sign())
			{
				int_number addend = *this;

				addend.negate();

				addend._number.insert(addend._number.cbegin(), factor._number.size() - 1, false);

				result.add(addend);
			}

			result.lean();

			*this = std::move(result);

			return *this;
		}

		constexpr int_number &divide(int_number divisor)
		{
			if (divisor == int_number{})
			{
				throw std::domain_error("\"divisor\" is zero.");
			}

			bool dividendBitSign;
			bool divisorBitSign;
			int_number dividend(*this);

			int_number::convert_factors_to_positive(dividend, dividendBitSign, divisor, divisorBitSign);

			if (divisor > dividend)
			{
				*this = int_number{};

				return *this;
			}

			if (divisor == dividend)
			{
				*this = int_number::_one;

				int_number::restore_sign(*this, dividendBitSign, divisorBitSign);

				return *this;
			}

			int_number current(int_number::_one);
			int_number quotient{};

			auto shiftCount = dividend.most_significant_true_bit_position() - divisor.most_significant_true_bit_position();

			if (shiftCount > 0)
			{
				divisor.shift_left(shiftCount);
			}

			while (divisor <= dividend) 
			{
				divisor.shift_left(1);

				shiftCount++;
			}

			divisor.shift_right(1);

			shiftCount--;

			if (shiftCount)
			{
				current.shift_left(shiftCount);
			}

			shiftCount = 0;

			const std::vector < bool > ::size_type loopCount = current.most_significant_true_bit_position() + 1;

			for (std::vector < bool > ::size_type i = 0; i < loopCount; i++)
			{
				if (dividend >= divisor) 
				{
					if (shiftCount != 0)
					{
						current.shift_right(shiftCount);
						shiftCount = 0;
					}

					dividend -= divisor;
					quotient |= current;
				}

				divisor.shift_right(1);
				shiftCount++;
			}

			int_number::restore_sign(quotient, dividendBitSign, divisorBitSign);

			*this = std::move(quotient);

			this->lean();

			return *this;
		}

		constexpr typename std::vector < bool > ::const_reference operator[](const typename std::vector < bool > ::size_type index) const
		{
			if (index >= this->_number.size())
			{
				return this->_number.back();
			}

			return this->_number[index];
		}

		constexpr typename std::vector < bool > ::reference operator[](const typename std::vector < bool > ::size_type index)
		{
			if (index >= this->_number.size())
			{
				return this->_number.back();
			}

			return this->_number[index];
		}

		constexpr int_number &operator=(const std::int8_t value)
		{
			this->set(value);

			return *this;
		}

		constexpr int_number &operator=(const std::int16_t value)
		{
			this->set(value);

			return *this;
		}

		constexpr int_number &operator=(const std::int32_t value)
		{
			this->set(value);

			return *this;
		}

		constexpr int_number &operator=(const std::int64_t value)
		{
			this->set(value);

			return *this;
		}

		constexpr int_number &operator=(const int_number &) = default;

		constexpr int_number &operator=(int_number &&) noexcept = default;

		constexpr int_number operator>>(const typename std::vector < bool > ::difference_type bitCount) const
		{
			int_number result = *this;

			result.shift_right(bitCount);

			return result;
		}

		constexpr int_number &operator>>=(const typename std::vector < bool > ::difference_type bitCount)
		{
			this->shift_right(bitCount);

			return *this;
		}

		constexpr int_number operator<<(const typename std::vector < bool > ::difference_type bitCount) const
		{
			int_number result = *this;

			result.shift_left(bitCount);

			return result;
		}

		constexpr int_number &operator<<=(const typename std::vector < bool > ::difference_type bitCount)
		{
			this->shift_left(bitCount);

			return *this;
		}

		constexpr int_number operator+() const
		{
			return *this;
		}

		constexpr int_number operator-() const
		{
			int_number result = *this;

			result.negate();

			return result;
		}

		constexpr int_number operator+(const int_number &other) const
		{
			int_number result(*this);

			result.add(other);

			return result;
		}

		constexpr int_number &operator++()
		{
			this->add(int_number::_one);

			return *this;
		}

		constexpr int_number operator++(int)
		{
			int_number result(*this);

			operator++();

			return result;
		}

		constexpr int_number &operator+=(const int_number &other)
		{
			this->add(other);

			return *this;
		}

		constexpr int_number operator-(const int_number &other) const
		{
			int_number result(*this);

			result.subtract(other);

			return result;
		}

		constexpr int_number &operator--()
		{
			this->subtract(int_number::_one);

			return *this;
		}

		constexpr int_number operator--(int)
		{
			int_number result(*this);

			operator--();

			return result;
		}

		constexpr int_number &operator-=(const int_number &other)
		{
			this->subtract(other);

			return *this;
		}

		constexpr int_number operator*(const int_number &other) const
		{
			int_number result(*this);

			result.multiply(other);

			return result;
		}

		constexpr int_number &operator*=(const int_number &other)
		{
			this->multiply(other);

			return *this;
		}

		constexpr int_number operator/(const int_number &divisor) const
		{
			int_number result(*this);

			result.divide(divisor);

			return result;
		}

		constexpr int_number &operator/=(const int_number &divisor)
		{
			this->divide(divisor);

			return *this;
		}

		constexpr bool operator<(const int_number &other) const
		{
			return this->lesser_than(other);
		}

		constexpr bool operator<=(const int_number &other) const
		{
			return this->lesser_or_equal_than(other);
		}

		constexpr bool operator>(const int_number &other) const
		{
			return this->greater_than(other);
		}

		constexpr bool operator>=(const int_number &other) const
		{
			return this->greater_or_equal_than(other);
		}

		constexpr bool operator==(const int_number &other) const
		{
			return this->equal_to(other);
		}

		constexpr bool operator!=(const int_number &other) const
		{
			return !this->equal_to(other);
		}

		constexpr int_number operator&(const int_number &other) const
		{
			int_number result(*this);

			result.bitwise_and(other);

			return result;
		}

		constexpr int_number &operator&=(const int_number &other)
		{
			this->bitwise_and(other);

			return *this;
		}

		constexpr int_number operator|(const int_number &other) const
		{
			int_number result(*this);

			result.bitwise_or(other);

			return result;
		}

		constexpr int_number &operator|=(const int_number &other)
		{
			this->bitwise_or(other);

			return *this;
		}

		constexpr int_number operator~() const
		{
			int_number result(*this);

			result.bitwise_not();

			return result;
		}

		constexpr std::string format() const
		{
			auto iterator = this->_number.crbegin();
			std::string formattedString;

			if (const auto i = static_cast < int > (this->_number.size() % 8) - 1;
				i >= 0)
			{
				int_number::format(formattedString, i, *iterator, iterator);
			}

			while (iterator != this->_number.crend())
			{
				int_number::format(formattedString, 7, iterator);
			}

			return formattedString;
		}

		friend std::ostream &operator<<(std::ostream &stream, const int_number &number)
		{
			stream << number.format();

			return stream;
		}

		constexpr std::intmax_t convert_to_int() const
		{
			std::vector < bool > ::size_type i = 0;
			std::intmax_t result{};

			for (; i < this->_number.size() && i < sizeof(std::intmax_t) * 8; i++)
			{
				result |= (std::intmax_t{this->_number[i]} << i);
			}

			for (; i < sizeof(std::intmax_t) * 8; i++)
			{
				result |= (std::intmax_t{this->_number.back()} << i);
			}

			return result;
		}

		constexpr const auto &container() const noexcept
		{
			return this->_number;
		}

		constexpr void lean()
		{
			auto iterator =
				std::find_if(this->_number.crbegin() + 1, this->_number.crend(),
					[this] (const bool value) -> bool
					{
						return value != this->bit_sign();
					});

			if (iterator != this->_number.crend())
			{
				--iterator;

				if (iterator == this->_number.crbegin())
				{
					return;
				}

				this->_number.resize(static_cast < std::vector < bool > ::size_type > (std::distance(iterator, this->_number.crend())));
			}

			else
			{
				this->_number.resize(1);
			}
		}

		constexpr int_number() noexcept = default;

		constexpr int_number(const std::int8_t value)
		{
			this->set(value);
		}

		constexpr int_number(const std::int16_t value)
		{
			this->set(value);
		}

		constexpr int_number(const std::int32_t value)
		{
			this->set(value);
		}

		constexpr int_number(const std::int64_t value)
		{
			this->set(value);
		}

		explicit constexpr int_number(const std::vector < bool > &value)
		{
			if (!value.empty())
			{
				this->_number = value;
			}
		}

		constexpr int_number(const int_number &) = default;

		constexpr int_number(int_number &&) noexcept = default;
	};

	const int_number int_number::_one = int_number(std::vector < bool > ({ true, false }));

	template < typename StandardIntType >
	requires (concepts::standard_integral < StandardIntType > && concepts::standard_signed < StandardIntType >)
	constexpr int_number operator+(const StandardIntType addend, const int_number &number)
	{
		return int_number(addend) + number;
	}

	template < typename StandardIntType >
	requires (concepts::standard_integral < StandardIntType > && concepts::standard_signed < StandardIntType >)
	constexpr int_number operator-(const StandardIntType subtrahend, const int_number &number)
	{
		return int_number(subtrahend) - number;
	}

	template < typename StandardIntType >
	requires (concepts::standard_integral < StandardIntType > && concepts::standard_signed < StandardIntType >)
	constexpr int_number operator*(const StandardIntType multiplier, const int_number &number)
	{
		return int_number(multiplier) * number;
	}

	template < typename StandardIntType >
	requires (concepts::standard_integral < StandardIntType > && concepts::standard_signed < StandardIntType >)
	constexpr int_number operator/(const StandardIntType dividend, const int_number &number)
	{
		return int_number(dividend) / number;
	}

	template < typename StandardIntType >
	requires (concepts::standard_integral < StandardIntType > && concepts::standard_signed < StandardIntType >)
	constexpr int_number operator&(const StandardIntType lhs, const int_number &rhs)
	{
		return int_number(lhs) & rhs;
	}

	template < typename StandardIntType >
	requires (concepts::standard_integral < StandardIntType > && concepts::standard_signed < StandardIntType >)
	constexpr int_number operator|(const StandardIntType lhs, const int_number &rhs)
	{
		return int_number(lhs) | rhs;
	}
}