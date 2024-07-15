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

export module xablau.color:CIE_Lab;
export import :CIE_standard_illuminants;
export import :forward_declarations;
export import :utils;

export import std;

export namespace xablau::color
{
	template < std::floating_point Type >
	class CIELab final
	{
	private:
		static void check_boundaries_with_exception(const CIELab &color)
		{
			if (!color.valid_boundaries())
			{
				constexpr auto minValues = CIELab::min_values();
				constexpr auto maxValues = CIELab::max_values();

				throw std::domain_error(
					std::format(
						"CIELab colors must be {{ L:[{:.{}f}, {:.{}f}], a:[{:.{}f}, {:.{}f}], b:[{:.{}f}, {:.{}f}] }}.",
						minValues.L, std::numeric_limits < Type > ::max_digits10,
						maxValues.L, std::numeric_limits < Type > ::max_digits10,
						minValues.a, std::numeric_limits < Type > ::max_digits10,
						maxValues.a, std::numeric_limits < Type > ::max_digits10,
						minValues.b, std::numeric_limits < Type > ::max_digits10,
						maxValues.b, std::numeric_limits < Type > ::max_digits10));
			}
		}

		static void check_for_NaNs_with_exception(const CIELab &color)
		{
			if (std::isnan(color.L) || std::isnan(color.a) || std::isnan(color.b))
			{
				throw std::invalid_argument("CIELab cannot have NaN values.");
			}
		}

		[[nodiscard]] static constexpr Type degrees_to_radians(const Type angle) noexcept
		{
			return (angle * std::numbers::pi_v < Type >) / Type{180};
		}

		[[nodiscard]] static Type distance_to(const CIELab &color1, const CIELab &color2)
		{
			const auto L1Star = color1.L;
			const auto L2Star = color2.L;
			const auto a1Star = color1.a;
			const auto a2Star = color2.a;
			const auto b1Star = color1.b;
			const auto b2Star = color2.b;

			constexpr auto one = Type{1};
			constexpr auto two = Type{2};
			constexpr auto three = Type{3};
			constexpr auto four = Type{4};
			constexpr auto seven = Type{7};
			constexpr auto twenty = Type{20};
			constexpr auto fifty = Type{50};
			constexpr auto twentyToThePowerOfSeven = Type{6103515625};
			constexpr auto twoTimesPi = Type{two * std::numbers::pi_v < Type >};
			constexpr auto sixtyDegrees = CIELab::degrees_to_radians(Type{6});
			constexpr auto twentyFiveDegrees = CIELab::degrees_to_radians(Type{25});
			constexpr auto thirtyDegrees = CIELab::degrees_to_radians(Type{30});
			constexpr auto sixtyThreeDegrees = CIELab::degrees_to_radians(Type{63});
			constexpr auto twoHundredsAndSeventyFiveDegrees = CIELab::degrees_to_radians(Type{275});

			const auto C1Star = std::sqrt((a1Star * a1Star) + (b1Star * b1Star));
			const auto C2Star = std::sqrt((a2Star * a2Star) + (b2Star * b2Star));
			const auto meanCStar = (C1Star + C2Star) / two;
			const auto meanCStarToTheSeven = std::pow(meanCStar, seven);
			const auto G = (one - std::sqrt(meanCStarToTheSeven / (meanCStarToTheSeven + twentyToThePowerOfSeven))) / two;
			const auto a1Line = a1Star * (one + G);
			const auto a2Line = a2Star * (one + G);
			const auto C1Line = std::sqrt((a1Line * a1Line) + (b1Star * b1Star));
			const auto C2Line = std::sqrt((a2Line * a2Line) + (b2Star * b2Star));

			auto h1Line = std::atan2(b1Star, a1Line);
			auto h2Line = std::atan2(b2Star, a2Line);

			if (h1Line < Type{})
			{
				h1Line += twoTimesPi;
			}

			if (h2Line < Type{})
			{
				h2Line += twoTimesPi;
			}

			const auto deltaLLine = L2Star - L1Star;
			const auto deltaCLine = C2Line - C1Line;

			Type deltahLine{};
			auto meanhLine = h1Line + h2Line;

			if (C1Line != Type{} && C2Line != Type{})
			{
				const auto differenceHLine = h2Line - h1Line;
				const auto absHLine = std::abs(differenceHLine);

				deltahLine = differenceHLine;

				if (deltahLine < -std::numbers::pi_v < Type >)
				{
					deltahLine += twoTimesPi;
				}

				else if (deltahLine > std::numbers::pi_v < Type >)
				{
					deltahLine -= twoTimesPi;
				}

				meanhLine /= two;

				if (absHLine > std::numbers::pi_v < Type >)
				{
					if (h1Line + h2Line < twoTimesPi)
					{
						meanhLine += std::numbers::pi_v < Type >;
					}

					else
					{
						meanhLine -= std::numbers::pi_v < Type >;
					}
				}
			}

			const auto deltaHLine = two * std::sqrt(C1Line * C2Line) * std::sin(deltahLine / two);
			const auto meanL = (L1Star + L2Star) / two;
			const auto meanCLine = (C1Line + C2Line) / two;
			const auto T = one -
				Type{0.17} * std::cos(meanhLine - thirtyDegrees) +
				Type{0.24} * std::cos(two * meanhLine) +
				Type{0.32} * std::cos(three * meanhLine + sixtyDegrees) -
				Type{0.2} * std::cos(four * meanhLine - sixtyThreeDegrees);
			const auto deltaTheta =
				thirtyDegrees * std::exp(-std::pow((meanhLine - twoHundredsAndSeventyFiveDegrees) / twentyFiveDegrees, two));
			const auto meanCLineToTheSeven = std::pow(meanCLine, seven);
			const auto RC = two * std::sqrt(meanCLineToTheSeven / (meanCLineToTheSeven + twentyToThePowerOfSeven));
			const auto auxiliaryTermSL = (meanL - fifty) * (meanL - fifty);
			const auto SL = one + ((Type{0.015} * auxiliaryTermSL) / std::sqrt(twenty + auxiliaryTermSL));
			const auto SC = one + Type{0.045} * meanCLine;
			const auto SH = one + Type{0.015} * meanCLine * T;
			const auto RT = -std::sin(two * deltaTheta) * RC;
			const auto term2 = deltaCLine / SC;
			const auto term3 = deltaHLine / SH;

			return std::sqrt(
				std::pow(deltaLLine / SL, two) +
				std::pow(term2, two) +
				std::pow(term3, two) +
				RT * term2 * term3);
		}

	public:
		using value_type = Type;

		Type L{};
		Type a{};
		Type b{};

		[[nodiscard]] static consteval CIELab min_values()
		{
			return CIELab
				{
					Type{},
					Type{-std::numeric_limits < Type > ::max()},
					Type{-std::numeric_limits < Type > ::max()}
				};
		}

		[[nodiscard]] static consteval CIELab max_values()
		{
			return CIELab
				{
					Type{100},
					Type{std::numeric_limits < Type > ::max()},
					Type{std::numeric_limits < Type > ::max()}
				};
		}

		[[nodiscard]] constexpr bool valid_boundaries() const noexcept
		{
			constexpr auto minValues = CIELab::min_values();
			constexpr auto maxValues = CIELab::max_values();

			return
				this->L >= minValues.L && this->a >= minValues.a && this->b >= minValues.b &&
				this->L <= maxValues.L && this->a <= maxValues.a && this->b <= maxValues.b;
		}

		constexpr CIELab &clamp_out_of_boundaries_values()
		{
			constexpr auto minValues = CIELab::min_values();
			constexpr auto maxValues = CIELab::max_values();

			this->L = std::clamp(this->L, minValues.L, maxValues.L);
			this->a = std::clamp(this->a, minValues.a, maxValues.a);
			this->b = std::clamp(this->b, minValues.b, maxValues.b);

			return *this;
		}

		// Thanks, http://www2.ece.rochester.edu/~gsharma/ciede2000/ciede2000noteCRNA.pdf!
		template < boundaries_check CheckBoundaries = boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs >
		[[nodiscard]] Type distance_to(const CIELab &color) const
		{
			if constexpr (CheckBoundaries == boundaries_check::check_with_exception)
			{
				CIELab::check_boundaries_with_exception(*this);
				CIELab::check_boundaries_with_exception(color);
			}

			else if constexpr (CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs)
			{
				CIELab::check_for_NaNs_with_exception(*this);
				CIELab::check_for_NaNs_with_exception(color);
			}

			if constexpr (
				CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values ||
				CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs)
			{
				auto _this = *this;
				auto _color = color;

				return CIELab::distance_to(_this.clamp_out_of_boundaries_values(), _color.clamp_out_of_boundaries_values());
			}

			else
			{
				return CIELab::distance_to(*this, color);
			}
		}

		// http://www.brucelindbloom.com/ and https://www.easyrgb.com/en/math.php
		template <
			boundaries_check CheckBoundaries = boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs,
			std::floating_point XYZColor = Type >
		[[nodiscard]] XYZ < XYZColor > convert_to_XYZ(
			const XYZ < XYZColor > &tristimulusXYZ = CIE_standard_illuminants::D652 < XYZColor >) const
		{
			if constexpr (CheckBoundaries == boundaries_check::check_with_exception)
			{
				CIELab::check_boundaries_with_exception(*this);
			}

			else if constexpr (CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs)
			{
				CIELab::check_for_NaNs_with_exception(*this);
			}

			constexpr auto e = XYZColor{216} / XYZColor{24389};
			constexpr auto k = XYZColor{24389} / XYZColor{27};

			constexpr auto fy =
				[] (const CIELab &_color) -> XYZColor
				{
					return (_color.L + XYZColor{16}) / XYZColor{116};
				};

			constexpr auto fx =
				[fy] (const CIELab &_color) -> XYZColor
				{
					return (_color.a / XYZColor{500}) + fy(_color);
				};

			constexpr auto fz =
				[fy] (const CIELab &_color) -> XYZColor
				{
					return fy(_color) - (_color.b / XYZColor{200});
				};

			constexpr auto xr =
				[e, k, fx] (const CIELab &_color) -> XYZColor
				{
					const auto _fx = fx(_color);
					const auto _fxToThePowerOfThree = static_cast < XYZColor > (std::pow(_fx, XYZColor{3}));

					if (_fxToThePowerOfThree > e)
					{
						return _fxToThePowerOfThree;
					}

					return (XYZColor{116} * _fx - XYZColor{16}) / k;
				};

			constexpr auto yr =
				[e, k] (const CIELab &_color) -> XYZColor
				{
					if (_color.L > k * e)
					{
						return static_cast < XYZColor > (std::pow((_color.L + XYZColor{16}) / XYZColor{116}, XYZColor{3}));
					}

					return _color.L / k;
				};

			constexpr auto zr =
				[e, k, fz] (const CIELab &_color) -> XYZColor
				{
					const auto _fz = fz(_color);
					const auto _fzToThePowerOfThree = static_cast < XYZColor > (std::pow(_fz, XYZColor{3}));

					if (_fzToThePowerOfThree > e)
					{
						return _fzToThePowerOfThree;
					}

					return (XYZColor{116} * _fz - XYZColor{16}) / k;
				};

			if constexpr (
				CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values ||
				CheckBoundaries == boundaries_check::clamp_out_of_boundaries_values_and_check_for_NaNs)
			{
				auto _this = *this;

				_this.clamp_out_of_boundaries_values();

				return XYZ < XYZColor >
					{
						xr(_this) * tristimulusXYZ.X,
						yr(_this) * tristimulusXYZ.Y,
						zr(_this) * tristimulusXYZ.Z
					};
			}

			else
			{
				return XYZ < XYZColor >
					{
						xr(*this) * tristimulusXYZ.X,
						yr(*this) * tristimulusXYZ.Y,
						zr(*this) * tristimulusXYZ.Z
					};
			}
		}
	};
}