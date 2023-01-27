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

export module xablau.color:XYZ;
export import :CIE_standard_illuminants;
export import :companding;
export import :forward_declarations;
export import :RGB_working_spaces;
export import :utils;

export import <algorithm>;
export import <format>;
export import <functional>;
export import <numeric>;
export import <optional>;
export import <stdexcept>;

export namespace xablau::color
{
	template < std::floating_point Type >
	class XYZ final
	{
	private:
		static void check_boundaries_with_exception(const XYZ &color)
		{
			if (!color.valid_boundaries())
			{
				constexpr auto minValues = XYZ::min_values();
				constexpr auto maxValues = XYZ::max_values();

				throw std::domain_error(
					std::format(
						"XYZ colors must be {{ X:[{:.{}f}, {:.{}f}], Y:[{:.{}f}, {:.{}f}], Z:[{:.{}f}, {:.{}f}] }}.",
						minValues.X, std::numeric_limits < Type > ::max_digits10,
						maxValues.X, std::numeric_limits < Type > ::max_digits10,
						minValues.Y, std::numeric_limits < Type > ::max_digits10,
						maxValues.Y, std::numeric_limits < Type > ::max_digits10,
						minValues.Z, std::numeric_limits < Type > ::max_digits10,
						maxValues.Z, std::numeric_limits < Type > ::max_digits10));
			}
		}

		static void check_for_NaNs_with_exception(const XYZ &color)
		{
			if (std::isnan(color.X) || std::isnan(color.Y) || std::isnan(color.Z))
			{
				throw std::invalid_argument("CIELab cannot have NaN values.");
			}
		}

		template < typename RGBColor, size_t BitsPerChannel >
		[[nodiscard]] static RGB < RGBColor, BitsPerChannel > convert_to_RGB(
			const XYZ &color,
			const RGB_working_spaces::RGB_working_space_inverse_matrix < Type > &RGBWorkingSpaceInverseMatrix,
			const std::optional < std::function < Type(const Type) > > &RGBCompanding)
		{
			const std::array < Type, 3 > _color =
				{
					color.X / Type{100},
					color.Y / Type{100},
					color.Z / Type{100}
				};

			std::array < Type, 3 > _xyz{};

			for (size_t i = 0; i < 3; i++)
			{
				_xyz[i] = std::inner_product(_color.cbegin(), _color.cend(), RGBWorkingSpaceInverseMatrix[i].cbegin(), Type{});

				if (RGBCompanding.has_value())
				{
					_xyz[i] = RGBCompanding.value()(_xyz[i]);
				}

				_xyz[i] = std::max(std::min(_xyz[i], Type{1}), Type{});
			}

			constexpr auto maxValuesRGB = RGB < RGBColor, BitsPerChannel > ::max_values();

			return RGB < RGBColor, BitsPerChannel >
				{
					static_cast < RGBColor > (_xyz[0] * maxValuesRGB.red),
					static_cast < RGBColor > (_xyz[1] * maxValuesRGB.green),
					static_cast < RGBColor > (_xyz[2] * maxValuesRGB.blue)
				};
		}

		template < std::floating_point CIELabColor >
		[[nodiscard]] static CIELab < CIELabColor > convert_to_CIELab(const XYZ &color, const XYZ < Type > &tristimulusXYZ)
		{
			constexpr auto f =
				[] (CIELabColor &value) -> void
				{
					constexpr auto k = CIELabColor{24389} / CIELabColor{27};
					constexpr auto e = CIELabColor{216} / CIELabColor{24389};

					if (value > e)
					{
						value = std::cbrt(value);
					}

					else
					{
						value = (k * value + CIELabColor{16}) / CIELabColor{116};
					}
				};

			std::array < CIELabColor, 3 > fXYZ =
				{
					static_cast < CIELabColor > (color.X) / static_cast < CIELabColor > (tristimulusXYZ.X),
					static_cast < CIELabColor > (color.Y) / static_cast < CIELabColor > (tristimulusXYZ.Y),
					static_cast < CIELabColor > (color.Z) / static_cast < CIELabColor > (tristimulusXYZ.Z)
				};

			std::for_each(fXYZ.begin(), fXYZ.end(), f);

			return CIELab < CIELabColor >
				{
					CIELabColor{116} * fXYZ[1] - CIELabColor{16},
					CIELabColor{500} * (fXYZ[0] - fXYZ[1]),
					CIELabColor{200} * (fXYZ[1] - fXYZ[2])
				};
		}

		template < std::floating_point OklabColor >
		[[nodiscard]] static Oklab < OklabColor > convert_to_Oklab(const XYZ &color)
		{
			constexpr std::array < std::array < OklabColor, 3 >, 3 > M1
				{ {
					{ OklabColor{0.8189330101}, OklabColor{0.3618667424}, OklabColor{-0.1288597137} },
					{ OklabColor{0.0329845436}, OklabColor{0.9293118715}, OklabColor{0.0361456387} },
					{ OklabColor{0.0482003018}, OklabColor{0.2643662691}, OklabColor{0.6338517070} }
				} };

			constexpr std::array < std::array < Type, 3 >, 3 > M2
				{ {
					{ OklabColor{0.2104542553}, OklabColor{0.7936177850}, OklabColor{-0.0040720468} },
					{ OklabColor{1.9779984951}, OklabColor{-2.4285922050}, OklabColor{0.4505937099} },
					{ OklabColor{0.0259040371}, OklabColor{0.7827717662}, OklabColor{-0.8086757660} }
				} };

			const std::array < OklabColor, 3 > _xyz =
				{
					static_cast < OklabColor > (color.X) / OklabColor{100},
					static_cast < OklabColor > (color.Y) / OklabColor{100},
					static_cast < OklabColor > (color.Z) / OklabColor{100}
				};

			const std::array < OklabColor, 3 > lmsLine
				{
					std::cbrt(std::inner_product(_xyz.cbegin(), _xyz.cend(), M1[0].cbegin(), OklabColor{})),
					std::cbrt(std::inner_product(_xyz.cbegin(), _xyz.cend(), M1[1].cbegin(), OklabColor{})),
					std::cbrt(std::inner_product(_xyz.cbegin(), _xyz.cend(), M1[2].cbegin(), OklabColor{}))
				};

			return Oklab < OklabColor >
				{
					std::inner_product(lmsLine.cbegin(), lmsLine.cend(), M2[0].cbegin(), OklabColor{}),
					std::inner_product(lmsLine.cbegin(), lmsLine.cend(), M2[1].cbegin(), OklabColor{}),
					std::inner_product(lmsLine.cbegin(), lmsLine.cend(), M2[2].cbegin(), OklabColor{})
				};
		}

	public:
		using value_type = Type;

		Type X{};
		Type Y{};
		Type Z{};

		[[nodiscard]] static consteval XYZ min_values()
		{
			return XYZ{ Type{}, Type{}, Type{} };
		}

		[[nodiscard]] static consteval XYZ max_values()
		{
			return XYZ
				{
					std::numeric_limits < Type > ::max(),
					Type{100},
					std::numeric_limits < Type > ::max()
				};
		}

		[[nodiscard]] constexpr bool valid_boundaries() const
		{
			constexpr auto minValues = XYZ::min_values();
			constexpr auto maxValues = XYZ::max_values();

			return
				this->X >= minValues.X && this->Y >= minValues.Y && this->Z >= minValues.Z &&
				this->X <= maxValues.X && this->Y <= maxValues.Y && this->Z <= maxValues.Z;
		}

		constexpr XYZ &clamp_out_of_boundaries_values()
		{
			constexpr auto minValues = XYZ::min_values();
			constexpr auto maxValues = XYZ::max_values();

			this->X = std::clamp(this->X, minValues.X, maxValues.X);
			this->Y = std::clamp(this->Y, minValues.Y, maxValues.Y);
			this->Z = std::clamp(this->Z, minValues.Z, maxValues.Z);

			return *this;
		}

		// Thanks, http://www.brucelindbloom.com/ and https://www.easyrgb.com/en/math.php!
		template <
			typename RGBColor,
			size_t BitsPerChannel,
			boundaries_check CheckBoundaries = boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs >
		[[nodiscard]] RGB < RGBColor, BitsPerChannel > convert_to_RGB(
			const RGB_working_spaces::RGB_working_space_inverse_matrix < Type > &RGBWorkingSpaceInverseMatrix =
				RGB_working_spaces::inverse_sRGB_D65 < Type >,
			const std::optional < std::function < Type(const Type) > > RGBCompanding =
				companding::template sRGB < Type >) const
		{
			if constexpr (CheckBoundaries == boundaries_check::check_with_exception)
			{
				XYZ::check_boundaries_with_exception(*this);
			}

			else if constexpr (CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs)
			{
				XYZ::check_for_NaNs_with_exception(*this);
			}

			if constexpr (
				CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values ||
				CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs)
			{
				auto _this = *this;

				return
					XYZ::template convert_to_RGB < RGBColor, BitsPerChannel > (
						_this.clamp_out_of_boundaries_values(),
						RGBWorkingSpaceInverseMatrix,
						RGBCompanding);
			}

			else
			{
				return XYZ::template convert_to_RGB < RGBColor, BitsPerChannel > (*this, RGBWorkingSpaceInverseMatrix, RGBCompanding);
			}
		}

		// Thanks, http://www.brucelindbloom.com/ and https://www.easyrgb.com/en/math.php!
		template <
			boundaries_check CheckBoundaries = boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs,
			std::floating_point CIELabColor = Type >
		[[nodiscard]] CIELab < CIELabColor > convert_to_CIELab(
			const XYZ < Type > &tristimulusXYZ = CIE_standard_illuminants::D652 < Type >) const
		{
			if constexpr (CheckBoundaries == boundaries_check::check_with_exception)
			{
				XYZ::check_boundaries_with_exception(*this);
			}

			else if constexpr (CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs)
			{
				XYZ::check_for_NaNs_with_exception(*this);
			}

			if constexpr (
				CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values ||
				CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs)
			{
				auto _this = *this;

				return XYZ::template convert_to_CIELab < CIELabColor > (_this.clamp_out_of_boundaries_values(), tristimulusXYZ);
			}

			else
			{
				return XYZ::template convert_to_CIELab < CIELabColor > (*this, tristimulusXYZ);
			}
		}

		// Thanks, https://bottosson.github.io/posts/oklab/!
		template <
			boundaries_check CheckBoundaries = boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs,
			std::floating_point OklabColor = Type >
		[[nodiscard]] Oklab < OklabColor > convert_to_Oklab() const
		{
			if constexpr (CheckBoundaries == boundaries_check::check_with_exception)
			{
				XYZ::check_boundaries_with_exception(*this);
			}

			else if constexpr (CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs)
			{
				XYZ::check_for_NaNs_with_exception(*this);
			}

			if constexpr (
				CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values ||
				CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs)
			{
				auto _this = *this;

				return XYZ::template convert_to_Oklab < OklabColor > (_this.clamp_out_of_boundaries_values());
			}

			else
			{
				return XYZ::template convert_to_Oklab < OklabColor > (*this);
			}
		}
	};
}