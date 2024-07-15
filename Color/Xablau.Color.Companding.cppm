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

export module xablau.color:companding;

export import std;

export namespace xablau::color::companding
{
	template < std::floating_point XYZColor >
	XYZColor sRGB(const XYZColor value)
	{
		if (value > XYZColor{0.0031308})
		{
			return XYZColor{1.055} * std::pow(value, XYZColor{1} / XYZColor{2.4}) - XYZColor{0.055};
		}

		return value * XYZColor{12.92};
	}

	template < std::floating_point XYZColor >
	XYZColor inverse_sRGB(const XYZColor value)
	{
		if (value > XYZColor{0.04045})
		{
			return std::pow((value + XYZColor{0.055}) / XYZColor{1.055}, XYZColor{2.4});
		}

		return value / XYZColor{12.92};
	}

	template < std::floating_point Type >
	Type gamma(const Type value, const Type gamma)
	{
		return std::pow(value, Type{1} / gamma);
	}

	template < std::floating_point Type >
	Type inverse_gamma(const Type value, const Type gamma)
	{
		return std::pow(value, gamma);
	};

	template < std::floating_point CIELabColor >
	CIELabColor L_star(const CIELabColor value)
	{
		constexpr auto e = CIELabColor{216} / CIELabColor{24389};
		constexpr auto k = CIELabColor{24389} / CIELabColor{27};

		if (value > e)
		{
			return CIELabColor{1.16} * std::cbrt(value) - CIELabColor{0.16};
		}

		return (value * k) / CIELabColor{100};
	}

	template < std::floating_point CIELabColor >
	CIELabColor inverse_L_star(const CIELabColor value)
	{
		constexpr auto k = CIELabColor{24389} / CIELabColor{27};

		if (value > CIELabColor{0.08})
		{
			return std::pow((value + CIELabColor{0.16}) / CIELabColor{1.16}, CIELabColor{3});
		}

		return (value * CIELabColor{100}) / k;
	}
}