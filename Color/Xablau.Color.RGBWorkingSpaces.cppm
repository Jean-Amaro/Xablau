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

export module xablau.color:RGB_working_spaces;

export import <array>;
export import <concepts>;

export namespace xablau::color::RGB_working_spaces
{
	template < std::floating_point Type >
	using RGB_working_space_matrix = std::array < std::array < Type, 3 >, 3 >;

	template < std::floating_point Type >
	using RGB_working_space_inverse_matrix = std::array < std::array < Type, 3 >, 3 >;

	template < std::floating_point Type = float >
	constexpr RGB_working_space_matrix < Type > Adobe_RGB_1998_D50
		{ {
			{ Type{0.6097559}, Type{0.2052401}, Type{0.1492240} },
			{ Type{0.3111242}, Type{0.6256560}, Type{0.0632197} },
			{ Type{0.0194811}, Type{0.0608902}, Type{0.7448387} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_inverse_matrix < Type > inverse_Adobe_RGB_1998_D50
		{ {
			{ Type{1.9624274}, Type{-0.6105343}, Type{-0.3413404} },
			{ Type{-0.9787684}, Type{1.9161415}, Type{0.0334540} },
			{ Type{0.0286869}, Type{-0.1406752}, Type{1.3487655} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_matrix < Type > Adobe_RGB_1998_D65
		{ {
			{ Type{0.5767309}, Type{0.1855540}, Type{0.1881852} },
			{ Type{0.2973769}, Type{0.6273491}, Type{0.0752741} },
			{ Type{0.0270343}, Type{0.0706872}, Type{0.9911085} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_inverse_matrix < Type > inverse_Adobe_RGB_1998_D65
		{ {
			{ Type{2.0413690}, Type{-0.5649464}, Type{-0.3446944} },
			{ Type{-0.9692660}, Type{1.8760108}, Type{0.0415560} },
			{ Type{0.0134474}, Type{-0.1183897}, Type{1.0154096} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_matrix < Type > Apple_RGB_D50
		{ {
			{ Type{0.4755678}, Type{0.3396722}, Type{0.1489800} },
			{ Type{0.2551812}, Type{0.6725693}, Type{0.0722496} },
			{ Type{0.0184697}, Type{0.1133771}, Type{0.6933632} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_inverse_matrix < Type > inverse_Apple_RGB_D50
		{ {
			{ Type{2.8510695}, Type{-1.3605261}, Type{-0.4708281} },
			{ Type{-1.0927680}, Type{2.0348871}, Type{0.0227598} },
			{ Type{0.1027403}, Type{-0.2964984}, Type{1.4510659} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_matrix < Type > Apple_RGB_D65
		{ {
			{ Type{0.4497288}, Type{0.3162486}, Type{0.1844926} },
			{ Type{0.2446525}, Type{0.6720283}, Type{0.0833192} },
			{ Type{0.0251848}, Type{0.1411824}, Type{0.9224628} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_inverse_matrix < Type > inverse_Apple_RGB_D65
		{ {
			{ Type{2.9515373}, Type{-1.2894116}, Type{-0.4738445} },
			{ Type{-1.0851093}, Type{1.9908566}, Type{0.0372026} },
			{ Type{0.0854934}, Type{-0.2694964}, Type{1.0912975} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_matrix < Type > Best_RGB_D50
		{ {
			{ Type{0.6326696}, Type{0.2045558}, Type{0.1269946} },
			{ Type{0.2284569}, Type{0.7373523}, Type{0.0341908} },
			{ Type{0.0000000}, Type{0.0095142}, Type{0.8156958} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_inverse_matrix < Type > inverse_Best_RGB_D50
		{ {
			{ Type{1.7552599}, Type{-0.4836786}, Type{-0.2530000} },
			{ Type{-0.5441336}, Type{1.5068789}, Type{0.0215528} },
			{ Type{0.0063467}, Type{-0.0175761}, Type{1.2256959} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_matrix < Type > Beta_RGB_D50
		{ {
			{ Type{0.6712537}, Type{0.1745834}, Type{0.1183829} },
			{ Type{0.3032726}, Type{0.6637861}, Type{0.0329413} },
			{ Type{0.0000000}, Type{0.0407010}, Type{0.7845090} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_inverse_matrix < Type > inverse_Beta_RGB_D50
		{ {
			{ Type{1.6832270}, Type{-0.4282363}, Type{-0.2360185} },
			{ Type{-0.7710229}, Type{1.7065571}, Type{0.0446900} },
			{ Type{0.0400013}, Type{-0.0885376}, Type{1.2723640} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_matrix < Type > Bruce_RGB_D50
		{ {
			{ Type{0.4941816}, Type{0.3204834}, Type{0.1495550} },
			{ Type{0.2521531}, Type{0.6844869}, Type{0.0633600} },
			{ Type{0.0157886}, Type{0.0629304}, Type{0.7464909} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_inverse_matrix < Type > inverse_Bruce_RGB_D50
		{ {
			{ Type{2.6502856}, Type{-1.2014485}, Type{-0.4289936} },
			{ Type{-0.9787684}, Type{1.9161415}, Type{0.0334540} },
			{ Type{0.0264570}, Type{-0.1361227}, Type{1.3458542} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_matrix < Type > Bruce_RGB_D65
		{ {
			{ Type{0.4674162}, Type{0.2944512}, Type{0.1886026} },
			{ Type{0.2410115}, Type{0.6835475}, Type{0.0754410} },
			{ Type{0.0219101}, Type{0.0736128}, Type{0.9933071} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_inverse_matrix < Type > inverse_Bruce_RGB_D65
		{ {
			{ Type{2.7454669}, Type{-1.1358136}, Type{-0.4350269} },
			{ Type{-0.9692660}, Type{1.8760108}, Type{0.0415560} },
			{ Type{0.0112723}, Type{-0.1139754}, Type{1.0132541} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_matrix < Type > CIE_RGB_D50
		{ {
			{ Type{0.4868870}, Type{0.3062984}, Type{0.1710347} },
			{ Type{0.1746583}, Type{0.8247541}, Type{0.0005877} },
			{ Type{-0.0012563}, Type{0.0169832}, Type{0.8094831} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_inverse_matrix < Type > inverse_CIE_RGB_D50
		{ {
			{ Type{2.3638081}, Type{-0.8676030}, Type{-0.4988161} },
			{ Type{-0.5005940}, Type{1.3962369}, Type{0.1047562} },
			{ Type{0.0141712}, Type{-0.0306400}, Type{1.2323842} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_matrix < Type > CIE_RGB_E
		{ {
			{ Type{0.4887180}, Type{0.3106803}, Type{0.2006017} },
			{ Type{0.1762044}, Type{0.8129847}, Type{0.0108109} },
			{ Type{0.0000000}, Type{0.0102048}, Type{0.9897952} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_inverse_matrix < Type > inverse_CIE_RGB_E
		{ {
			{ Type{2.3706743}, Type{-0.9000405}, Type{-0.4706338} },
			{ Type{-0.5138850}, Type{1.4253036}, Type{0.0885814} },
			{ Type{0.0052982}, Type{-0.0146949}, Type{1.0093968} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_matrix < Type > Color_Match_RGB_D50
		{ {
			{ Type{0.5093439}, Type{0.3209071}, Type{0.1339691} },
			{ Type{0.2748840}, Type{0.6581315}, Type{0.0669845} },
			{ Type{0.0242545}, Type{0.1087821}, Type{0.6921735} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_inverse_matrix < Type > inverse_Color_Match_RGB_D50
		{ {
			{ Type{2.6422874}, Type{-1.2234270}, Type{-0.3930143} },
			{ Type{-1.1119763}, Type{2.0590183}, Type{0.0159614} },
			{ Type{0.0821699}, Type{-0.2807254}, Type{1.4559877} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_matrix < Type > Don_RGB_4_D50
		{ {
			{ Type{0.6457711}, Type{0.1933511}, Type{0.1250978} },
			{ Type{0.2783496}, Type{0.6879702}, Type{0.0336802} },
			{ Type{0.0037113}, Type{0.0179861}, Type{0.8035125} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_inverse_matrix < Type > inverse_Don_RGB_4_D50
		{ {
			{ Type{1.7603902}, Type{-0.4881198}, Type{-0.2536126} },
			{ Type{-0.7126288}, Type{1.6527432}, Type{0.0416715} },
			{ Type{0.0078207}, Type{-0.0347411}, Type{1.2447743} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_matrix < Type > eciRGB_D50
		{ {
			{ Type{0.6502043}, Type{0.1780774}, Type{0.1359384} },
			{ Type{0.3202499}, Type{0.6020711}, Type{0.0776791} },
			{ Type{0.0000000}, Type{0.0678390}, Type{0.7573710} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_inverse_matrix < Type > inverse_eciRGB_D50
		{ {
			{ Type{1.7827618}, Type{-0.4969847}, Type{-0.2690101} },
			{ Type{-0.9593623}, Type{1.9477962}, Type{-0.0275807} },
			{ Type{0.0859317}, Type{-0.1744674}, Type{1.3228273} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_matrix < Type > Ekta_Space_PS5_D50
		{ {
			{ Type{0.5938914}, Type{0.2729801}, Type{0.0973485} },
			{ Type{0.2606286}, Type{0.7349465}, Type{0.0044249} },
			{ Type{0.0000000}, Type{0.0419969}, Type{0.7832131} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_inverse_matrix < Type > inverse_Ekta_Space_PS5_D50
		{ {
			{ Type{2.0043819}, Type{-0.7304844}, Type{-0.2450052} },
			{ Type{-0.7110285}, Type{1.6202126}, Type{0.0792227} },
			{ Type{0.0381263}, Type{-0.0868780}, Type{1.2725438} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_matrix < Type > NTSC_RGB_C
		{ {
			{ Type{0.6068909}, Type{0.1735011}, Type{0.2003480} },
			{ Type{0.2989164}, Type{0.5865990}, Type{0.1144845} },
			{ Type{0.0000000}, Type{0.0660957}, Type{1.1162243} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_inverse_matrix < Type > inverse_NTSC_RGB_C
		{ {
			{ Type{1.9099961}, Type{-0.5324542}, Type{-0.2882091} },
			{ Type{-0.9846663}, Type{1.9991710}, Type{-0.0283082} },
			{ Type{0.0583056}, Type{-0.1183781}, Type{0.8975535} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_matrix < Type > NTSC_RGB_D50
		{ {
			{ Type{0.6343706}, Type{0.1852204}, Type{0.1446290} },
			{ Type{0.3109496}, Type{0.5915984}, Type{0.0974520} },
			{ Type{-0.0011817}, Type{0.0555518}, Type{0.7708399} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_inverse_matrix < Type > inverse_NTSC_RGB_D50
		{ {
			{ Type{1.8464881}, Type{-0.5521299}, Type{-0.2766458} },
			{ Type{-0.9826630}, Type{2.0044755}, Type{-0.0690396} },
			{ Type{0.0736477}, Type{-0.1453020}, Type{1.3018376} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_matrix < Type > PAL_SECAM_RGB_D50
		{ {
			{ Type{0.4552773}, Type{0.3675500}, Type{0.1413926} },
			{ Type{0.2323025}, Type{0.7077956}, Type{0.0599019} },
			{ Type{0.0145457}, Type{0.1049154}, Type{0.7057489} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_inverse_matrix < Type > inverse_PAL_SECAM_RGB_D50
		{ {
			{ Type{2.9603944}, Type{-1.4678519}, Type{-0.4685105} },
			{ Type{-0.9787684}, Type{1.9161415}, Type{0.0334540} },
			{ Type{0.0844874}, Type{-0.2545973}, Type{1.4216174} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_matrix < Type > PAL_SECAM_RGB_D65
		{ {
			{ Type{0.4306190}, Type{0.3415419}, Type{0.1783091} },
			{ Type{0.2220379}, Type{0.7066384}, Type{0.0713236} },
			{ Type{0.0201853}, Type{0.1295504}, Type{0.9390944} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_inverse_matrix < Type > inverse_PAL_SECAM_RGB_D65
		{ {
			{ Type{3.0628971}, Type{-1.3931791}, Type{-0.4757517} },
			{ Type{-0.9692660}, Type{1.8760108}, Type{0.0415560} },
			{ Type{0.0678775}, Type{-0.2288548}, Type{1.0693490} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_matrix < Type > ProPhoto_RGB_D50
		{ {
			{ Type{0.7976749}, Type{0.1351917}, Type{0.0313534} },
			{ Type{0.2880402}, Type{0.7118741}, Type{0.0000857} },
			{ Type{0.0000000}, Type{0.0000000}, Type{0.8252100} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_inverse_matrix < Type > inverse_ProPhoto_RGB_D50
		{ {
			{ Type{1.3459433}, Type{-0.2556075}, Type{-0.0511118} },
			{ Type{-0.5445989}, Type{1.5081673}, Type{0.0205351} },
			{ Type{0.0000000}, Type{0.0000000}, Type{1.2118128} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_matrix < Type > SMPTE_C_RGB_D50
		{ {
			{ Type{0.4163290}, Type{0.3931464}, Type{0.1547446} },
			{ Type{0.2216999}, Type{0.7032549}, Type{0.0750452} },
			{ Type{0.0136576}, Type{0.0913604}, Type{0.7201920} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_inverse_matrix < Type > inverse_SMPTE_C_RGB_D50
		{ {
			{ Type{3.3921940}, Type{-1.8264027}, Type{-0.5385522} },
			{ Type{-1.0770996}, Type{2.0213975}, Type{0.0207989} },
			{ Type{0.0723073}, Type{-0.2217902}, Type{1.3960932} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_matrix < Type > SMPTE_C_RGB_D65
		{ {
			{ Type{0.3935891}, Type{0.3652497}, Type{0.1916313} },
			{ Type{0.2124132}, Type{0.7010437}, Type{0.0865432} },
			{ Type{0.0187423}, Type{0.1119313}, Type{0.9581563} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_inverse_matrix < Type > inverse_SMPTE_C_RGB_D65
		{ {
			{ Type{3.5053960}, Type{-1.7394894}, Type{-0.5439640} },
			{ Type{-1.0690722}, Type{1.9778245}, Type{0.0351722} },
			{ Type{0.0563200}, Type{-0.1970226}, Type{1.0502026} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_matrix < Type > sRGB_D50
		{ {
			{ Type{0.4360747}, Type{0.3850649}, Type{0.1430804} },
			{ Type{0.2225045}, Type{0.7168786}, Type{0.0606169} },
			{ Type{0.0139322}, Type{0.0971045}, Type{0.7141733} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_inverse_matrix < Type > inverse_sRGB_D50
		{ {
			{ Type{3.1338561}, Type{-1.6168667}, Type{-0.4906146} },
			{ Type{-0.9787684}, Type{1.9161415}, Type{0.0334540} },
			{ Type{0.0719453}, Type{-0.2289914}, Type{1.4052427} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_matrix < Type > sRGB_D65
		{ {
			{ Type{0.4124564}, Type{0.3575761}, Type{0.1804375} },
			{ Type{0.2126729}, Type{0.7151522}, Type{0.0721750} },
			{ Type{0.0193339}, Type{0.1191920}, Type{0.9503041} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_inverse_matrix < Type > inverse_sRGB_D65
		{ {
			{ Type{3.2404542}, Type{-1.5371385}, Type{-0.4985314} },
			{ Type{-0.9692660}, Type{1.8760108}, Type{0.0415560} },
			{ Type{0.0556434}, Type{-0.2040259}, Type{1.0572252} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_matrix < Type > Wide_Gamut_RGB_D50
		{ {
			{ Type{0.7161046}, Type{0.1009296}, Type{0.1471858} },
			{ Type{0.2581874}, Type{0.7249378}, Type{0.0168748} },
			{ Type{0.0000000}, Type{0.0517813}, Type{0.7734287} }
		} };

	template < std::floating_point Type = float >
	constexpr RGB_working_space_inverse_matrix < Type > inverse_Wide_Gamut_RGB_D50
		{ {
			{ Type{1.4628067}, Type{-0.1840623}, Type{-0.2743606} },
			{ Type{-0.5217933}, Type{1.4472381}, Type{0.0677227} },
			{ Type{0.0349342}, Type{-0.0968930}, Type{1.2884099} }
		} };
}