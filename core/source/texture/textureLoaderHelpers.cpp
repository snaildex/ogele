#include <stdafx.h>
#include <ogele.h>
using namespace std;
namespace ogele {
	const map<const string, TextureType> StrToTexType = {
		{"Array1D",					TextureType::Array1D},
		{"Array1DProxy",			TextureType::Array1DProxy},
		{"Array2D",					TextureType::Array2D},
		{"Array2DMultisample",		TextureType::Array2DMultisample},
		{"Array2DMultisampleProxy",	TextureType::Array2DMultisampleProxy},
		{"Array2DProxy",			TextureType::Array2DProxy},
		{"ArrayCubeMap",			TextureType::ArrayCubeMap},
		{"ArrayCubeMapProxy",		TextureType::ArrayCubeMapProxy},
		{"Single1D",				TextureType::Single1D},
		{"Single1DProxy",			TextureType::Single1DProxy},
		{"Single2D",				TextureType::Single2D},
		{"Single2DMultisample",		TextureType::Single2DMultisample},
		{"Single2DMultisampleProxy",TextureType::Single2DMultisampleProxy},
		{"SinSingle2DProxygle1D",	TextureType::Single2DProxy},
		{"Single3D",				TextureType::Single3D},
		{"Single3DProxy",			TextureType::Single3DProxy},
		{"SingleBuffer",			TextureType::SingleBuffer},
		{"SingleCubeMap",			TextureType::SingleCubeMap},
		{"SingleCubeMapProxy",		TextureType::SingleCubeMapProxy},
		{"SingleRectangle",			TextureType::SingleRectangle},
		{"SingleRectangleProxy",	TextureType::SingleRectangleProxy}
	};
	const map<const string, TextureWrapMode> StrToTexWrapMode = {
		{"ClampToEdge",       TextureWrapMode::ClampToEdge},
		{"ClampToBorder",     TextureWrapMode::ClampToBorder},
		{"MirroredRepeat",    TextureWrapMode::MirroredRepeat},
		{"Repeat",            TextureWrapMode::Repeat},
		{"MirrorClampToEdge", TextureWrapMode::MirrorClampToEdge}
	};
	const map<const string, TextureFilterMode> StrToTexFilterMode = {
		{"Linear",               TextureFilterMode::Linear},
		{"LinearMipMapLinear",   TextureFilterMode::LinearMipMapLinear},
		{"LinearMipMapNearest",  TextureFilterMode::LinearMipMapNearest},
		{"Nearest",              TextureFilterMode::Nearest},
		{"NearestMipMapLinear",  TextureFilterMode::NearestMipMapLinear},
		{"NearestMipMapNearest", TextureFilterMode::NearestMipMapNearest}
	};
	const map<const string, TextureFormat> StrToTexFormat = {
		{"DepthComponent", TextureFormat::DepthComponent},
		{"DepthStencil",TextureFormat::DepthStencil},

		{"R8",TextureFormat::R8},
		{"R8_SNorm",TextureFormat::R8_SNorm},
		{"R16",TextureFormat::R16},
		{"R16_SNorm",TextureFormat::R16_SNorm},
		{"RG8",TextureFormat::RG8},
		{"RG8_SNorm",TextureFormat::RG8_SNorm},
		{"RG16",TextureFormat::RG16},
		{"RG16_SNorm",TextureFormat::RG16_SNorm},

		{"R3G3B2",TextureFormat::R3G3B2},
		{"RGB4",TextureFormat::RGB4},
		{"RGB5",TextureFormat::RGB5 },
		{"RGB8",TextureFormat::RGB8},
		{"RGB8_SNorm",TextureFormat::RGB8_SNorm},
		{"RGB10",TextureFormat::RGB10},
		{"RGB12",TextureFormat::RGB12},
		{"RGB16_SNorm",TextureFormat::RGB16_SNorm},

		{"RGBA2",TextureFormat::RGBA2},
		{"RGBA4",TextureFormat::RGBA4},
		{"RGB5A1",TextureFormat::RGB5A1},
		{"RGBA8",TextureFormat::RGBA8},
		{"RGBA8_SNorm",TextureFormat::RGBA8_SNorm},
		{"RGB10A2",TextureFormat::RGB10A2},
		{"RGB10A2UI",TextureFormat::RGB10A2UI},
		{"RGBA12",TextureFormat::RGBA12},
		{"RGBA16",TextureFormat::RGBA16},

		{"SRGB8",TextureFormat::SRGB8},
		{"SRGB8A8",TextureFormat::SRGB8A8},

		{"R16F",TextureFormat::R16F},
		{"RG16F",TextureFormat::RG16F},
		{"RGB16F",TextureFormat::RGB16F},
		{"RGBA16F",TextureFormat::RGBA16F},
		{"R32F",TextureFormat::R32F},
		{"RG32F",TextureFormat::RG32F},
		{"RGB32F",TextureFormat::RGB32F},
		{"RGBA32F",TextureFormat::RGBA32F},
		{"R11FG11FB10F",TextureFormat::R11FG11FB10F},

		{"RGB9E5",TextureFormat::RGB9E5},

		{"R8I",TextureFormat::R8I},
		{"R8UI",TextureFormat::R8UI},
		{"R16I",TextureFormat::R16I},
		{"R16UI",TextureFormat::R16UI},
		{"R32I",TextureFormat::R32I},
		{"R32UI",TextureFormat::R32UI},
		{"RG8I",TextureFormat::RG8I},
		{"RG8UI",TextureFormat::RG8UI },
		{"RG16I",TextureFormat::RG16I},
		{"RG16UI",TextureFormat::RG16UI },
		{"RG32I",TextureFormat::RG32I},
		{"RG32UI",TextureFormat::RG32UI},
		{"RGB8I",TextureFormat::RGB8I},
		{"RGB8UI",TextureFormat::RGB8UI},
		{"RGB16I",TextureFormat::RGB16I},
		{"RGB16UI",TextureFormat::RGB16UI },
		{"RGB32I",TextureFormat::RGB32I},
		{"RGB32UI",TextureFormat::RGB32UI },
		{"RGBA8I",TextureFormat::RGBA8I },
		{"RGBA8UI",TextureFormat::RGBA8UI},
		{"RGBA16I",TextureFormat::RGBA16I},
		{"RGBA16UI",TextureFormat::RGBA16UI},
		{"RGBA32I",TextureFormat::RGBA32I},
		{"RGBA32UI",TextureFormat::RGBA32UI},

		{"Compressed_Red",TextureFormat::Compressed_Red},
		{"Compressed_RG",TextureFormat::Compressed_RG },
		{"Compressed_RGB",TextureFormat::Compressed_RGB},
		{"Compressed_RGBA",TextureFormat::Compressed_RGBA },
		{"Compressed_SRGB",TextureFormat::Compressed_SRGB },
		{"Compressed_SRGB_Alpha",TextureFormat::Compressed_SRGB_Alpha},
		{"Compressed_Red_RGTC1",TextureFormat::Compressed_Red_RGTC1 },
		{"Compressed_Signed_Red_RGTC1",TextureFormat::Compressed_Signed_Red_RGTC1 },
		{"Compressed_RG_RGTC2",TextureFormat::Compressed_RG_RGTC2 },
		{"Compressed_Signed_RG_RGTC2",TextureFormat::Compressed_Signed_RG_RGTC2 },
		{"Compressed_RGBA_BPTC_UNorm",TextureFormat::Compressed_RGBA_BPTC_UNorm },
		{"Compressed_SRGB_Alpha_BPTC_UNorm",TextureFormat::Compressed_SRGB_Alpha_BPTC_UNorm },
		{"Compressed_RGB_BPTC_Signed_Float",TextureFormat::Compressed_RGB_BPTC_Signed_Float },
		{"Compressed_RGB_BPTC_Unsigned_Float",TextureFormat::Compressed_RGB_BPTC_Unsigned_Float }
	};

	const set<string> extensions = {
			".jpeg",
			".jpg",
			".png",
			".tga",
			".bmp",
			".psd",
			".gif",
			".hdr",
			".pic",
			".pnm"
	};

	bool Is2DImage(const fs::path& file) {
		return extensions.count(file.extension().string()) > 0;
	}
}

