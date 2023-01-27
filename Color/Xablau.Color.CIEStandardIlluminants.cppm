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

export module xablau.color:CIE_standard_illuminants;
export import :forward_declarations;

export namespace xablau::color::CIE_standard_illuminants
{
	// Incandescent / Tungsten
	template < std::floating_point Type >
	constexpr XYZ < Type > A2{ Type{109.850}, Type{100.000}, Type{35.585} };

	// Incandescent / Tungsten
	template < std::floating_point Type >
	constexpr XYZ < Type > A10{ Type{111.144}, Type{100.000}, Type{35.200} };

	// Obsolete; Direct sunlight at noon
	template < std::floating_point Type >
	constexpr XYZ < Type > B2{ Type{99.0927}, Type{100.000}, Type{85.313} };

	// Obsolete; Direct sunlight at noon
	template < std::floating_point Type >
	constexpr XYZ < Type > B10{ Type{99.178}, Type{100.000}, Type{84.3493} };

	// Obsolete; Average / North sky daylight
	template < std::floating_point Type >
	constexpr XYZ < Type > C2{ Type{98.074}, Type{100.000}, Type{118.232} };

	// Obsolete; Average / North sky daylight
	template < std::floating_point Type >
	constexpr XYZ < Type > C10{ Type{97.285}, Type{100.000}, Type{116.145} };

	// Horizon light / ICC profile PCS
	template < std::floating_point Type >
	constexpr XYZ < Type > D502{ Type{96.422}, Type{100.000}, Type{82.521} };

	// Horizon light / ICC profile PCS
	template < std::floating_point Type >
	constexpr XYZ < Type > D5010{ Type{96.720}, Type{100.000}, Type{81.427} };

	// Mid-morning / Mid-afternoon daylight
	template < std::floating_point Type >
	constexpr XYZ < Type > D552{ Type{95.682}, Type{100.000}, Type{92.149} };

	// Mid-morning / Mid-afternoon daylight
	template < std::floating_point Type >
	constexpr XYZ < Type > D5510{ Type{95.799}, Type{100.000}, Type{90.926} };

	// Noon daylight / Television / sRGB color space
	template < std::floating_point Type >
	constexpr XYZ < Type > D652{ Type{95.047}, Type{100.000}, Type{108.883} };

	// Noon daylight / Television / sRGB color space
	template < std::floating_point Type >
	constexpr XYZ < Type > D6510{ Type{94.811}, Type{100.000}, Type{107.304} };

	// North sky daylight
	template < std::floating_point Type >
	constexpr XYZ < Type > D752{ Type{94.972}, Type{100.000}, Type{122.638} };

	// North sky daylight
	template < std::floating_point Type >
	constexpr XYZ < Type > D7510{ Type{94.416}, Type{100.000}, Type{120.641} };

	// Equal energy
	template < std::floating_point Type >
	constexpr XYZ < Type > E2{ Type{100.000}, Type{100.000}, Type{100.000} };

	// Equal energy
	template < std::floating_point Type >
	constexpr XYZ < Type > E10{ Type{100.000}, Type{100.000}, Type{100.000} };

	// Daylight fluorescent
	template < std::floating_point Type >
	constexpr XYZ < Type > F12{ Type{92.834}, Type{100.000}, Type{103.655} };

	// Daylight fluorescent
	template < std::floating_point Type >
	constexpr XYZ < Type > F110{ Type{94.791}, Type{100.000}, Type{103.191} };

	// Cool white fluorescent
	template < std::floating_point Type >
	constexpr XYZ < Type > F22{ Type{99.187}, Type{100.000}, Type{67.395} };

	// Cool white fluorescent
	template < std::floating_point Type >
	constexpr XYZ < Type > F210{ Type{103.280}, Type{100.000}, Type{69.026} };

	// White fluorescent
	template < std::floating_point Type >
	constexpr XYZ < Type > F32{ Type{103.754}, Type{100.000}, Type{49.861} };

	// White fluorescent
	template < std::floating_point Type >
	constexpr XYZ < Type > F310{ Type{108.968}, Type{100.000}, Type{51.965} };

	// Warm white fluorescent
	template < std::floating_point Type >
	constexpr XYZ < Type > F42{ Type{109.147}, Type{100.000}, Type{38.813} };

	// Warm white fluorescent
	template < std::floating_point Type >
	constexpr XYZ < Type > F410{ Type{114.961}, Type{100.000}, Type{40.963} };

	// Daylight fluorescent
	template < std::floating_point Type >
	constexpr XYZ < Type > F52{ Type{90.872}, Type{100.000}, Type{98.723} };

	// Daylight fluorescent
	template < std::floating_point Type >
	constexpr XYZ < Type > F510{ Type{93.369}, Type{100.000}, Type{98.636} };

	// Light white fluorescent
	template < std::floating_point Type >
	constexpr XYZ < Type > F62{ Type{97.309}, Type{100.000}, Type{60.191} };

	// Light white fluorescent
	template < std::floating_point Type >
	constexpr XYZ < Type > F610{ Type{102.148}, Type{100.000}, Type{62.074} };

	// D65 simulator / Daylight simulator
	template < std::floating_point Type >
	constexpr XYZ < Type > F72{ Type{95.044}, Type{100.000}, Type{108.755} };

	// D65 simulator / Daylight simulator
	template < std::floating_point Type >
	constexpr XYZ < Type > F710{ Type{95.792}, Type{100.000}, Type{107.687} };

	// D50 simulator / Sylvania F40 Design 50
	template < std::floating_point Type >
	constexpr XYZ < Type > F82{ Type{96.413}, Type{100.000}, Type{82.333} };

	// D50 simulator / Sylvania F40 Design 50
	template < std::floating_point Type >
	constexpr XYZ < Type > F810{ Type{97.115}, Type{100.000}, Type{81.135} };

	// Cool white deluxe fluorescent
	template < std::floating_point Type >
	constexpr XYZ < Type > F92{ Type{100.365}, Type{100.000}, Type{67.868} };

	// Cool white deluxe fluorescent
	template < std::floating_point Type >
	constexpr XYZ < Type > F910{ Type{102.116}, Type{100.000}, Type{67.826} };

	// Ultralume 50 / Philips TL85
	template < std::floating_point Type >
	constexpr XYZ < Type > F102{ Type{96.174}, Type{100.000}, Type{81.712} };

	// Ultralume 50 / Philips TL85
	template < std::floating_point Type >
	constexpr XYZ < Type > F1010{ Type{99.001}, Type{100.000}, Type{83.134} };

	// Ultralume 40 / Philips TL84
	template < std::floating_point Type >
	constexpr XYZ < Type > F112{ Type{100.966}, Type{100.000}, Type{64.370} };

	// Ultralume 40 / Philips TL84
	template < std::floating_point Type >
	constexpr XYZ < Type > F1110{ Type{103.866}, Type{100.000}, Type{65.627} };

	// Ultralume 30 / Philips TL83
	template < std::floating_point Type >
	constexpr XYZ < Type > F122{ Type{108.046}, Type{100.000}, Type{39.228} };

	// Ultralume 30 / Philips TL83
	template < std::floating_point Type >
	constexpr XYZ < Type > F1210{ Type{111.428}, Type{100.000}, Type{40.353} };
}