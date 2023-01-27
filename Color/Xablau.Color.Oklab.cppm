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

export module xablau.color:Oklab;
export import :forward_declarations;
export import :utils;

export import <algorithm>;
export import <format>;
export import <numeric>;
export import <stdexcept>;

export namespace xablau::color
{
	template < std::floating_point Type >
	class Oklab final
	{
	private:
		static void check_bounds_with_exception(const Oklab &color)
		{
			if (!color.valid_boundaries())
			{
				constexpr auto minValues = Oklab::min_values();
				constexpr auto maxValues = Oklab::max_values();

				throw std::domain_error(
					std::format(
						"Oklab colors must be {{ L:[{:.{}f}, {:.{}f}], a:[{:.{}f}, {:.{}f}], b:[{:.{}f}, {:.{}f}] }}.",
						minValues.L, std::numeric_limits < Type > ::max_digits10,
						maxValues.L, std::numeric_limits < Type > ::max_digits10,
						minValues.a, std::numeric_limits < Type > ::max_digits10,
						maxValues.a, std::numeric_limits < Type > ::max_digits10,
						minValues.b, std::numeric_limits < Type > ::max_digits10,
						maxValues.b, std::numeric_limits < Type > ::max_digits10));
			}
		}

		static void check_for_NaNs_with_exception(const Oklab &color)
		{
			if (std::isnan(color.L) || std::isnan(color.a) || std::isnan(color.b))
			{
				throw std::invalid_argument("Oklab cannot have NaN values.");
			}
		}

		template < typename RGBColor, size_t BitsPerChannel >
		[[nodiscard]] static RGB < RGBColor, BitsPerChannel > convert_to_RGB(const Oklab &color)
		{
			constexpr auto maxValuesRGB = RGB < RGBColor, BitsPerChannel > ::max_values();

			constexpr std::array < std::array < Type, 3 >, 3 > matrix
				{ {
					{ Type{4.0767416621}, Type{-3.3077115913}, Type{0.2309699292} },
					{ Type{-1.2684380046}, Type{2.6097574011}, Type{-0.3413193965} },
					{ Type{-0.0041960863}, Type{-0.7034186147}, Type{1.7076147010} }
				} };

			const std::array < Type, 3 > lms
				{
					std::pow(color.L + Type{0.3963377774} * color.a + Type{0.2158037573} * color.b, Type{3}),
					std::pow(color.L - Type{0.1055613458} * color.a - Type{0.0638541728} * color.b, Type{3}),
					std::pow(color.L - Type{0.0894841775} * color.a - Type{1.2914855480} * color.b, Type{3})
				};

			return RGB < RGBColor, BitsPerChannel >
				{
					static_cast < RGBColor > (std::inner_product(lms.cbegin(), lms.cend(), matrix[0].cbegin(), Type{})) * maxValuesRGB.red,
					static_cast < RGBColor > (std::inner_product(lms.cbegin(), lms.cend(), matrix[1].cbegin(), Type{})) * maxValuesRGB.green,
					static_cast < RGBColor > (std::inner_product(lms.cbegin(), lms.cend(), matrix[2].cbegin(), Type{})) * maxValuesRGB.blue
				};
		}

		template < std::floating_point XYZColor >
		[[nodiscard]] static XYZ < XYZColor > convert_to_XYZ(const Oklab &color)
		{
			constexpr std::array < std::array < Type, 3 >, 3 > inverseM1
				{ {
					{ Type{1.2270138511}, Type{-0.5577999806}, Type{0.2812561489} },
					{ Type{-0.0405801784}, Type{1.1122568696}, Type{-0.0716766786} },
					{ Type{-0.0763812845}, Type{-0.4214819784}, Type{1.5861632204} }
				} };

			constexpr std::array < std::array < Type, 3 >, 3 > inverseM2
				{ {
					{ Type{0.9999999984}, Type{0.3963377921}, Type{0.2158037580} },
					{ Type{1.0000000088}, Type{-0.1055613423}, Type{-0.0638541747} },
					{ Type{1.0000000546}, Type{-0.0894841820}, Type{-1.2914855378} }
				} };

			const std::array < XYZColor, 3 > Lab
				{
					static_cast < XYZColor > (color.L),
					static_cast < XYZColor > (color.a),
					static_cast < XYZColor > (color.b)
				};

			const std::array < XYZColor, 3 > lms
				{
					std::pow(std::inner_product(Lab.cbegin(), Lab.cend(), inverseM2[0].cbegin(), XYZColor{}), XYZColor{3}),
					std::pow(std::inner_product(Lab.cbegin(), Lab.cend(), inverseM2[1].cbegin(), XYZColor{}), XYZColor{3}),
					std::pow(std::inner_product(Lab.cbegin(), Lab.cend(), inverseM2[2].cbegin(), XYZColor{}), XYZColor{3})
				};

			return XYZ < XYZColor >
				{
					std::inner_product(lms.cbegin(), lms.cend(), inverseM1[0].cbegin(), XYZColor{}) * XYZColor{100},
					std::inner_product(lms.cbegin(), lms.cend(), inverseM1[1].cbegin(), XYZColor{}) * XYZColor{100},
					std::inner_product(lms.cbegin(), lms.cend(), inverseM1[2].cbegin(), XYZColor{}) * XYZColor{100}
				};
		}

	public:
		using value_type = Type;

		Type L{};
		Type a{};
		Type b{};

		[[nodiscard]] static consteval Oklab min_values() noexcept
		{
			return Oklab
				{
					Type{},
					Type{-std::numeric_limits < Type > ::max()},
					Type{-std::numeric_limits < Type > ::max()}
				};
		}

		[[nodiscard]] static consteval Oklab max_values() noexcept
		{
			return Oklab
				{
					Type{100},
					Type{std::numeric_limits < Type > ::max()},
					Type{std::numeric_limits < Type > ::max()}
				};
		}

		[[nodiscard]] constexpr bool valid_boundaries() const noexcept
		{
			constexpr auto minValues = Oklab::min_values();
			constexpr auto maxValues = Oklab::max_values();

			return
				this->L >= minValues.L && this->a >= minValues.a && this->b >= minValues.b &&
				this->L <= maxValues.L && this->a <= maxValues.a && this->b <= maxValues.b;
		}

		constexpr Oklab &clamp_out_of_boundaries_values()
		{
			constexpr auto minValues = Oklab::min_values();
			constexpr auto maxValues = Oklab::max_values();

			this->L = std::clamp(this->L, minValues.L, maxValues.L);
			this->a = std::clamp(this->a, minValues.a, maxValues.a);
			this->b = std::clamp(this->b, minValues.b, maxValues.b);

			return *this;
		}

		// Thanks, http://www2.ece.rochester.edu/~gsharma/ciede2000/ciede2000noteCRNA.pdf!
		template < boundaries_check CheckBoundaries = boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs >
		[[nodiscard]] Type distance_to(const Oklab &color) const
		{
			if constexpr (CheckBoundaries == boundaries_check::check_with_exception)
			{
				Oklab::check_boundaries_with_exception(*this);
				Oklab::check_boundaries_with_exception(color);
			}

			else if constexpr (CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs)
			{
				Oklab::check_for_NaNs_with_exception(*this);
				Oklab::check_for_NaNs_with_exception(color);
			}

			if constexpr (
				CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values ||
				CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs)
			{
				auto _this = *this;
				auto _color = color;

				_this.clamp_out_of_boundaries_values();
				_color.clamp_out_of_boundaries_values();

				const CIELab < Type > color1{ _this.L, _this.a, _this.b };
				const CIELab < Type > color2{ _color.L, _color.a, _color.b };

				return color1.distance_to < boundaries_check::do_not_check > (color2);
			}

			else
			{
				const CIELab < Type > color1{ this->L, this->a, this->b };
				const CIELab < Type > color2{ this->L, this->a, this->b };

				return color1.distance_to < boundaries_check::do_not_check > (color2);
			}
		}

		// Thanks, https://bottosson.github.io/posts/oklab/!
		template <
			typename RGBColor,
			size_t BitsPerChannel,
			boundaries_check CheckBoundaries = boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs >
		[[nodiscard]] RGB < RGBColor, BitsPerChannel > convert_to_RGB() const
		{
			if constexpr (CheckBoundaries == boundaries_check::check_with_exception)
			{
				Oklab::check_boundaries_with_exception(*this);
			}

			else if constexpr (CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs)
			{
				Oklab::check_for_NaNs_with_exception(*this);
			}

			if constexpr (
				CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values ||
				CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs)
			{
				auto _this = *this;

				return Oklab::template convert_to_RGB < RGBColor, BitsPerChannel > (_this.clamp_out_of_boundaries_values());
			}

			else
			{
				return Oklab::template convert_to_RGB < RGBColor, BitsPerChannel > (*this);
			}
		}

		// Thanks, https://bottosson.github.io/posts/oklab/!
		template <
			boundaries_check CheckBoundaries = boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs,
			std::floating_point XYZColor = Type >
		[[nodiscard]] XYZ < XYZColor > convert_to_XYZ() const
		{
			if constexpr (CheckBoundaries == boundaries_check::check_with_exception)
			{
				Oklab::check_boundaries_with_exception(*this);
			}

			else if constexpr (CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs)
			{
				Oklab::check_for_NaNs_with_exception(*this);
			}

			if constexpr (
				CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values ||
				CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs)
			{
				auto _this = *this;

				return Oklab::template convert_to_XYZ < XYZColor > (_this.clamp_out_of_boundaries_values());
			}

			else
			{
				return Oklab::template convert_to_XYZ < XYZColor > (*this);
			}
		}
	};
}