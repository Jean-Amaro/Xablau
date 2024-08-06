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

export module xablau.color:RGB;
export import :companding;
export import :forward_declarations;
export import :RGB_working_spaces;
export import :utils;

export import std;

export namespace xablau::color
{
	template < typename Type, size_t BitsPerChannel >
	requires (
		BitsPerChannel > 0 &&
		BitsPerChannel <= std::numeric_limits < Type > ::digits &&
		(std::floating_point < Type > || std::unsigned_integral < Type >))
	class RGB final
	{
	private:
		[[nodiscard]] static consteval auto closest_floating_point_by_size()
		{
			if constexpr (sizeof(Type) <= sizeof(float))
			{
				return float{};
			}

			else if constexpr (sizeof(Type) <= sizeof(double))
			{
				return double{};
			}

			else
			{
				constexpr long double value{};

				return value;
			}
		}

		using default_floating_point_type =
			std::conditional <
				std::floating_point < Type >,
				Type,
				decltype(RGB::closest_floating_point_by_size()) > ::type;

		static void check_boundaries_with_exception(const RGB &color)
		{
			if (!color.valid_boundaries())
			{
				constexpr auto minValues = RGB::min_values();
				constexpr auto maxValues = RGB::max_values();

				if constexpr (std::floating_point < Type >)
				{
					throw std::domain_error(
						std::format(
							"RGB {}-Bit colors must be {{ red:[{:.{}f}, {:.{}f}], green:[{:.{}f}, {:.{}f}], blue:[{:.{}f}, {:.{}f}] }}.",
							RGB::bits_per_channel,
							minValues.red, std::numeric_limits < Type > ::max_digits10,
							maxValues.red, std::numeric_limits < Type > ::max_digits10,
							minValues.green, std::numeric_limits < Type > ::max_digits10,
							maxValues.green, std::numeric_limits < Type > ::max_digits10,
							minValues.blue, std::numeric_limits < Type > ::max_digits10,
							maxValues.blue, std::numeric_limits < Type > ::max_digits10));
				}

				else
				{
					throw std::domain_error(
						std::format(
							"RGB {}-Bit colors must be {{ red:[{}, {}], green:[{}, {}], blue:[{}, {}] }}.",
							RGB::bits_per_channel,
							minValues.red,
							maxValues.red,
							minValues.green,
							maxValues.green,
							minValues.blue,
							maxValues.blue));

				}
			}
		}

		static void check_for_NaNs_with_exception(const RGB &color)
		{
			if constexpr (std::floating_point < Type >)
			{
				if (std::isnan(color.red) || std::isnan(color.green) || std::isnan(color.blue))
				{
					throw std::invalid_argument("RGB cannot have NaN values.");
				}
			}
		}

		template < std::floating_point DistanceType >
		[[nodiscard]] static DistanceType distance_to(const RGB &color1, const RGB &color2)
		{
			constexpr auto maxValue = std::rotl(std::uintmax_t{1}, BitsPerChannel) - 1;

			const auto maxDiagonalValue = static_cast < DistanceType > (maxValue) * std::sqrt(DistanceType{3});

			DistanceType redDifference{};
			DistanceType greenDifference{};
			DistanceType blueDifference{};

			if constexpr (std::is_floating_point < Type > ::value)
			{
				redDifference = static_cast < DistanceType > (color1.red - color2.red);
				greenDifference = static_cast < DistanceType > (color1.green - color2.green);
				blueDifference = static_cast < DistanceType > (color1.blue - color2.blue);
			}

			else
			{
				const auto &[minRed, maxRed] = std::minmax(color1.red, color2.red);
				const auto &[minGreen, maxGreen] = std::minmax(color1.green, color2.green);
				const auto &[minBlue, maxBlue] = std::minmax(color1.blue, color2.blue);

				redDifference = static_cast < DistanceType > (maxRed - minRed);
				greenDifference = static_cast < DistanceType > (maxGreen - minGreen);
				blueDifference = static_cast < DistanceType > (maxBlue - minBlue);
			}

			return std::sqrt(
				redDifference * redDifference +
				greenDifference * greenDifference +
				blueDifference * blueDifference) / maxDiagonalValue;
		}

		template < std::floating_point HSVColor >
		[[nodiscard]] static HSV < HSVColor > convert_to_HSV(const RGB &color)
		{
			constexpr auto maxValuesRGB = RGB::max_values();

			const auto red = static_cast < HSVColor > (color.red) / static_cast < HSVColor > (maxValuesRGB.red);
			const auto green = static_cast < HSVColor > (color.green) / static_cast < HSVColor > (maxValuesRGB.green);
			const auto blue = static_cast < HSVColor > (color.blue) / static_cast < HSVColor > (maxValuesRGB.blue);

			const auto &max = std::max(std::max(red, green), blue);
			const auto &min = std::min(std::min(red, green), blue);

			const auto chroma = max - min;

			HSV < HSVColor > result{};

			if (chroma != HSVColor{})
			{
				HSVColor hue{};

				if (red >= green && red > blue)
				{
					hue = std::fmod((green - blue) / chroma, HSVColor{6});

					if (hue < HSVColor{})
					{
						hue += HSVColor{6};
					}
				}

				else if (green >= blue && green > red)
				{
					hue = ((blue - red) / chroma) + HSVColor{2};
				}

				else
				{
					hue = ((red - green) / chroma) + HSVColor{4};
				}

				result.hue = (hue / HSVColor{6}) * HSVColor{2} * std::numbers::pi_v < HSVColor >; 
				result.saturation = chroma / max;
			}

			result.value = max;

			return result;
		}

		template < std::floating_point HSLColor >
		[[nodiscard]] static HSL < HSLColor > convert_to_HSL(const RGB &color)
		{
			constexpr auto maxValuesRGB = RGB::max_values();

			const auto red = static_cast < HSLColor > (color.red) / static_cast < HSLColor > (maxValuesRGB.red);
			const auto green = static_cast < HSLColor > (color.green) / static_cast < HSLColor > (maxValuesRGB.green);
			const auto blue = static_cast < HSLColor > (color.blue) / static_cast < HSLColor > (maxValuesRGB.blue);

			const auto &max = std::max(std::max(red, green), blue);
			const auto &min = std::min(std::min(red, green), blue);

			const auto chroma = max - min;
			const auto lightness = HSLColor{0.5} * (max + min);

			HSL < HSLColor > result{};

			if (chroma != HSLColor{})
			{
				HSLColor hue{};

				if (red >= green && red > blue)
				{
					hue = std::fmod((green - blue) / chroma, HSLColor{6});

					if (hue < HSLColor{})
					{
						hue += HSLColor{6};
					}
				}

				else if (green >= blue && green > red)
				{
					hue = ((blue - red) / chroma) + HSLColor{2};
				}

				else
				{
					hue = ((red - green) / chroma) + HSLColor{4};
				}

				result.hue = (hue / HSLColor{6}) * HSLColor{2} * std::numbers::pi_v < HSLColor >; 
				result.saturation = chroma / (HSLColor{1} - std::abs(HSLColor{2} * lightness - HSLColor{1}));
			}

			result.lightness = lightness;

			return result;
		}

		template < std::floating_point XYZColor >
		[[nodiscard]] static XYZ < XYZColor > convert_to_XYZ(
			const RGB &color,
			const RGB_working_spaces::RGB_working_space_matrix < XYZColor > &RGBWorkingSpaceMatrix,
			const std::optional < std::function < XYZColor(const XYZColor) > > &RGBInverseCompanding)
		{
			constexpr auto maxValuesRGB = RGB::max_values();

			std::array < XYZColor, 3 > _rgb
				{
					static_cast < XYZColor > (color.red) / static_cast < XYZColor > (maxValuesRGB.red),
					static_cast < XYZColor > (color.green) / static_cast < XYZColor > (maxValuesRGB.green),
					static_cast < XYZColor > (color.blue) / static_cast < XYZColor > (maxValuesRGB.blue)
				};

			if (RGBInverseCompanding.has_value())
			{
				std::transform(_rgb.cbegin(), _rgb.cend(), _rgb.begin(), RGBInverseCompanding.value());
			}

			std::transform(_rgb.cbegin(), _rgb.cend(), _rgb.begin(),
				[] (const XYZColor value) -> XYZColor
				{
					return std::max(std::min(value, XYZColor{1}), XYZColor{}) * XYZColor{100};
				});

			return XYZ < XYZColor >
				{
					std::inner_product(_rgb.cbegin(), _rgb.cend(), RGBWorkingSpaceMatrix[0].cbegin(), XYZColor{}),
					std::inner_product(_rgb.cbegin(), _rgb.cend(), RGBWorkingSpaceMatrix[1].cbegin(), XYZColor{}),
					std::inner_product(_rgb.cbegin(), _rgb.cend(), RGBWorkingSpaceMatrix[2].cbegin(), XYZColor{})
				};
		}

		template < std::floating_point OklabColor >
		[[nodiscard]] static Oklab < OklabColor > convert_to_Oklab(const RGB &color)
		{
			constexpr std::array < std::array < OklabColor, 3 >, 3 > matrix1
				{ {
					{ OklabColor{0.4122214708}, OklabColor{0.5363325363}, OklabColor{0.0514459929} },
					{ OklabColor{0.2119034982}, OklabColor{0.6806995451}, OklabColor{0.1073969566} },
					{ OklabColor{0.0883024619}, OklabColor{0.2817188376}, OklabColor{0.6299787005} }
				} };

			constexpr std::array < std::array < OklabColor, 3 >, 3 > matrix2
				{ {
					{ OklabColor{0.2104542553}, OklabColor{0.7936177850}, OklabColor{-0.0040720468} },
					{ OklabColor{1.9779984951}, OklabColor{-2.4285922050}, OklabColor{0.4505937099} },
					{ OklabColor{0.0259040371}, OklabColor{0.7827717662}, OklabColor{-0.8086757660} }
				} };

			constexpr auto maxValuesRGB = RGB::max_values();

			const std::array < OklabColor, 3 > _rgb
				{
					static_cast < OklabColor > (color.red) / static_cast < OklabColor > (maxValuesRGB.red),
					static_cast < OklabColor > (color.green) / static_cast < OklabColor > (maxValuesRGB.green),
					static_cast < OklabColor > (color.blue) / static_cast < OklabColor > (maxValuesRGB.blue)
				};

			const std::array < OklabColor, 3 > lmsLine
				{
					std::cbrt(std::inner_product(_rgb.cbegin(), _rgb.cend(), matrix1[0].cbegin(), OklabColor{})),
					std::cbrt(std::inner_product(_rgb.cbegin(), _rgb.cend(), matrix1[1].cbegin(), OklabColor{})),
					std::cbrt(std::inner_product(_rgb.cbegin(), _rgb.cend(), matrix1[2].cbegin(), OklabColor{}))
				};

			return Oklab < OklabColor >
				{
					std::inner_product(lmsLine.cbegin(), lmsLine.cend(), matrix2[0].cbegin(), OklabColor{}),
					std::inner_product(lmsLine.cbegin(), lmsLine.cend(), matrix2[1].cbegin(), OklabColor{}),
					std::inner_product(lmsLine.cbegin(), lmsLine.cend(), matrix2[2].cbegin(), OklabColor{})
				};
		}

	public:
		using value_type = Type;

		Type red{};
		Type green{};
		Type blue{};

		static constexpr size_t bits_per_channel = BitsPerChannel;

		[[nodiscard]] static consteval RGB min_values()
		{
			return RGB{ Type{}, Type{}, Type{} };
		}

		[[nodiscard]] static consteval RGB max_values()
		{
			constexpr auto maxValue = std::rotl(uintmax_t{1}, BitsPerChannel) - 1;

			return RGB
				{
					static_cast < Type > (maxValue),
					static_cast < Type > (maxValue),
					static_cast < Type > (maxValue)
				};
		}

		[[nodiscard]] constexpr bool valid_boundaries() const
		{
			constexpr auto maxValue = std::rotl(uintmax_t{1}, BitsPerChannel) - 1;

			if constexpr (std::numeric_limits < Type > ::max() != maxValue)
			{
				constexpr auto maxValues = RGB::max_values();

				if (this->red > maxValues.red || this->green > maxValues.green || this->blue > maxValues.blue)
				{
					return false;
				}
			}

			if constexpr (std::is_signed < Type > ::value)
			{
				constexpr auto minValues = RGB::min_values();

				if (this->red < minValues.red || this->green < minValues.green || this->blue < minValues.blue)
				{
					return false;
				}
			}

			return true;
		}

		constexpr RGB &clamp_out_of_boundaries_values()
		{
			constexpr auto maxValue = std::rotl(uintmax_t{1}, BitsPerChannel) - 1;

			if constexpr (std::numeric_limits < Type > ::max() != maxValue)
			{
				constexpr auto maxValues = RGB::max_values();

				this->red = std::min(this->red, maxValues.red);
				this->green = std::min(this->green, maxValues.green);
				this->blue = std::min(this->blue, maxValues.blue);
			}

			if constexpr (std::is_signed < Type > ::value)
			{
				constexpr auto minValues = RGB::min_values();

				this->red = std::max(this->red, minValues.red);
				this->green = std::max(this->green, minValues.green);
				this->blue = std::max(this->blue, minValues.blue);
			}

			return *this;
		}

		template <
			boundaries_check CheckBoundaries = boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs,
			std::floating_point DistanceType = RGB::default_floating_point_type >
		[[nodiscard]] DistanceType distance_to(const RGB &color) const
		{
			if constexpr (CheckBoundaries == boundaries_check::check_with_exception)
			{
				RGB::check_boundaries_with_exception(*this);
				RGB::check_boundaries_with_exception(color);
			}

			else if constexpr (CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs)
			{
				RGB::check_for_NaNs_with_exception(*this);
				RGB::check_for_NaNs_with_exception(color);
			}

			if constexpr (
				CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values ||
				CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs)
			{
				auto _this = *this;
				auto _color = color;

				return
					RGB::distance_to < DistanceType > (
						_this.clamp_out_of_boundaries_values(),
						_color.clamp_out_of_boundaries_values());
			}

			else
			{
				return RGB::distance_to < DistanceType > (*this, color);
			}
		}

		template <
			boundaries_check CheckBoundaries = boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs,
			std::floating_point HSVColor = RGB::default_floating_point_type >
		[[nodiscard]] HSV < HSVColor > convert_to_HSV() const
		{
			if constexpr (CheckBoundaries == boundaries_check::check_with_exception)
			{
				RGB::check_boundaries_with_exception(*this);
			}

			else if constexpr (CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs)
			{
				RGB::check_for_NaNs_with_exception(*this);
			}

			if constexpr (
				CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values ||
				CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs)
			{
				auto _this = *this;

				return RGB::template convert_to_HSV < HSVColor > (_this.clamp_out_of_boundaries_values());
			}

			else
			{
				return RGB::template convert_to_HSV < HSVColor > (*this);
			}
		}

		template <
			boundaries_check CheckBoundaries = boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs,
			std::floating_point HSLColor = RGB::default_floating_point_type >
		[[nodiscard]] HSL < HSLColor > convert_to_HSL() const
		{
			if constexpr (CheckBoundaries == boundaries_check::check_with_exception)
			{
				RGB::check_boundaries_with_exception(*this);
			}

			else if constexpr (CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs)
			{
				RGB::check_for_NaNs_with_exception(*this);
			}

			if constexpr (
				CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values ||
				CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs)
			{
				auto _this = *this;

				return RGB::template convert_to_HSL < HSLColor > (_this.clamp_out_of_boundaries_values());
			}

			else
			{
				return RGB::template convert_to_HSL < HSLColor > (*this);
			}
		}

		// Thanks, http://www.brucelindbloom.com/ and https://www.easyrgb.com/en/math.php!
		template <
			boundaries_check CheckBoundaries = boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs,
			std::floating_point XYZColor = RGB::default_floating_point_type >
		[[nodiscard]] XYZ < XYZColor > convert_to_XYZ(
			const RGB_working_spaces::RGB_working_space_matrix < XYZColor > &RGBWorkingSpaceMatrix =
				RGB_working_spaces::sRGB_D65 < XYZColor >,
			const std::optional < std::function < XYZColor(const XYZColor) > > RGBInverseCompanding =
				companding::inverse_sRGB < XYZColor >) const
		{
			if constexpr (CheckBoundaries == boundaries_check::check_with_exception)
			{
				RGB::check_boundaries_with_exception(*this);
			}

			else if constexpr (CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs)
			{
				RGB::check_for_NaNs_with_exception(*this);
			}

			if constexpr (
				CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values ||
				CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs)
			{
				auto _this = *this;

				return
					RGB::template convert_to_XYZ < XYZColor > (
						_this.clamp_out_of_boundaries_values(),
						RGBWorkingSpaceMatrix,
						RGBInverseCompanding);
			}

			else
			{
				return RGB::template convert_to_XYZ < XYZColor > (*this, RGBWorkingSpaceMatrix, RGBInverseCompanding);
			}
		}

		// Thanks, https://bottosson.github.io/posts/oklab/!
		template <
			boundaries_check CheckBoundaries = boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs,
			std::floating_point OklabColor = RGB::default_floating_point_type >
		[[nodiscard]] Oklab < OklabColor > convert_to_Oklab() const
		{
			if constexpr (CheckBoundaries == boundaries_check::check_with_exception)
			{
				RGB::check_boundaries_with_exception(*this);
			}

			else if constexpr (CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs)
			{
				RGB::check_for_NaNs_with_exception(*this);
			}

			if constexpr (
				CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values ||
				CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs)
			{
				auto _this = *this;

				return RGB::template convert_to_Oklab < OklabColor > (_this.clamp_out_of_boundaries_values());
			}

			else
			{
				return RGB::template convert_to_Oklab < OklabColor > (*this);
			}
		}
	};
}