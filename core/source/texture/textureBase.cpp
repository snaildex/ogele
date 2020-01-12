#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {
	bool IsTexFormatInteger(TextureFormat format) {
		switch (format) {
		case TextureFormat::R8I:
			return true;
		case TextureFormat::R8UI:
			return true;
		case TextureFormat::R16I:
			return true;
		case TextureFormat::R16UI:
			return true;
		case TextureFormat::R32I:
			return true;
		case TextureFormat::R32UI:
			return true;
		case TextureFormat::RG8I:
			return true;
		case TextureFormat::RG8UI:
			return true;
		case TextureFormat::RG16I:
			return true;
		case TextureFormat::RG16UI:
			return true;
		case TextureFormat::RG32I:
			return true;
		case TextureFormat::RG32UI:
			return true;
		case TextureFormat::RGB8I:
			return true;
		case TextureFormat::RGB8UI:
			return true;
		case TextureFormat::RGB16I:
			return true;
		case TextureFormat::RGB16UI:
			return true;
		case TextureFormat::RGB32I:
			return true;
		case TextureFormat::RGB32UI:
			return true;
		case TextureFormat::RGBA8I:
			return true;
		case TextureFormat::RGBA8UI:
			return true;
		case TextureFormat::RGBA16I:
			return true;
		case TextureFormat::RGBA16UI:
			return true;
		case TextureFormat::RGBA32I:
			return true;
		case TextureFormat::RGBA32UI:
			return true;
		default:
			return false;
		}
	}

	void TextureBase::RecreateTexture()
	{
		glDeleteTextures(1, &m_handle);
		GLErr();
		glGenTextures(1, &m_handle);
		GLErr();
	}

	void TextureBase::ApplyProperties()
	{
		Bind();
		bSetBaseLevel(m_props.BaseLevel);
		bSetBorderColor(m_props.BorderColor);
		bSetCompareFunc(m_props.CompareFunc);
		bSetCompareMode(m_props.CompareMode);
		bSetDepthStencilMode(m_props.DepthStencilMode);
		bSetMagFilter(m_props.MagFilter);
		bSetMinFilter(m_props.MinFilter);
		bSetWrapR(m_props.WrapR);
		bSetWrapS(m_props.WrapS);
		bSetWrapT(m_props.WrapT);
		bSetBorderColor(m_props.BorderColor);
		bSetBaseLevel(m_props.BaseLevel);
		bSetMaxLevel(m_props.MaxLevel);
		bSetLODBias(m_props.LodBias);
		bSetMinLOD(m_props.MinLod);
		bSetMaxLOD(m_props.MaxLod);
		bSetSwizzleR(m_props.SwizzleR);
		bSetSwizzleG(m_props.SwizzleG);
		bSetSwizzleB(m_props.SwizzleB);
		bSetSwizzleA(m_props.SwizzleA);
		Unbind();
	}

	TextureBase::TextureBase(TextureType type, TextureFormat format, bool mipmaps) {
		m_props.Type = type;
		m_props.Format = format;
		m_mipmaps = mipmaps;
		m_bindAccess = AccessMode::ReadWrite;
		m_bindLevel = 0;
		m_bindLayered = true;
		m_bindLayer = 0;
		glGenTextures(1, &m_handle);
		GLErr();
	}

	bool TextureBase::IsInteger() const noexcept { return IsTexFormatInteger(GetFormat()); }

	void TextureBase::BindImage(int unitNum) const {
		glBindImageTexture(unitNum, m_handle, m_bindLevel, m_bindLayered, m_bindLayer,
			static_cast<GLenum>(m_bindAccess), static_cast<GLenum>(m_props.Format));
		GLErr();
	}

	ogele::TextureDepthStencilMode TextureBase::bGetDepthStencilMode() const {
		glGetTexParameteriv(static_cast<GLenum>(m_props.Type), GL_DEPTH_STENCIL_TEXTURE_MODE, reinterpret_cast<int*>(&m_props.DepthStencilMode));
		GLErr();
		return m_props.DepthStencilMode;
	}

	void TextureBase::bSetDepthStencilMode(TextureDepthStencilMode value) {
		glTexParameteri(static_cast<GLenum>(m_props.Type), GL_DEPTH_STENCIL_TEXTURE_MODE, static_cast<int>(value));
		GLErr();
		m_props.DepthStencilMode = value;
	}

	glm::vec4 TextureBase::bGetBorderColor() const {
		glGetTexParameterfv(static_cast<GLenum>(m_props.Type), GL_TEXTURE_BORDER_COLOR, glm::value_ptr(m_props.BorderColor));
		GLErr();
		return m_props.BorderColor;
	}

	void TextureBase::bSetBorderColor(vec4 value) {
		glTexParameterfv(static_cast<GLenum>(m_props.Type), GL_TEXTURE_BORDER_COLOR, &value.x);
		GLErr();
		m_props.BorderColor = value;
	}

	ogele::DepthFuncMode TextureBase::bGetCompareFunc() const {
		glGetTexParameteriv(static_cast<GLenum>(m_props.Type), GL_TEXTURE_COMPARE_FUNC, reinterpret_cast<int*>(&m_props.CompareFunc));
		GLErr();
		return m_props.CompareFunc;
	}

	void TextureBase::bSetCompareFunc(DepthFuncMode value) {
		glTexParameteri(static_cast<GLenum>(m_props.Type), GL_TEXTURE_COMPARE_FUNC, static_cast<int>(value));
		GLErr();
		m_props.CompareFunc = value;
	}

	ogele::TextureCompareMode TextureBase::bGetCompareMode() const {
		glGetTexParameteriv(static_cast<GLenum>(m_props.Type), GL_TEXTURE_COMPARE_MODE, reinterpret_cast<int*>(&m_props.CompareMode));
		GLErr();
		return m_props.CompareMode;
	}

	void TextureBase::bSetCompareMode(TextureCompareMode value) {
		glTexParameteri(static_cast<GLenum>(m_props.Type), GL_TEXTURE_COMPARE_MODE, static_cast<int>(m_props.CompareMode));
		GLErr();
		m_props.CompareMode = value;
	}

	ogele::TextureFilterMode TextureBase::bGetMinFilter() const {
		glGetTexParameteriv(static_cast<GLenum>(m_props.Type), GL_TEXTURE_MIN_FILTER, reinterpret_cast<int*>(&m_props.MinFilter));
		GLErr();
		return static_cast<TextureFilterMode>(m_props.MinFilter);
	}

	void TextureBase::bSetMinFilter(TextureFilterMode value) {
		glTexParameteri(static_cast<GLenum>(m_props.Type), GL_TEXTURE_MIN_FILTER, static_cast<int>(value));
		GLErr();
		m_props.MinFilter = value;
	}

	ogele::TextureFilterMode TextureBase::bGetMagFilter() const {
		glGetTexParameteriv(static_cast<GLenum>(m_props.Type), GL_TEXTURE_MAG_FILTER, reinterpret_cast<int*>(&m_props.MagFilter));
		GLErr();
		return m_props.MagFilter;
	}

	void TextureBase::bSetMagFilter(TextureFilterMode value) {
		glTexParameteri(static_cast<GLenum>(m_props.Type), GL_TEXTURE_MAG_FILTER, static_cast<int>(value));
		GLErr();
		m_props.MagFilter = value;
	}

	int TextureBase::bGetBaseLevel() const {
		glGetTexParameteriv(static_cast<GLenum>(m_props.Type), GL_TEXTURE_BASE_LEVEL, &m_props.BaseLevel);
		GLErr();
		return m_props.BaseLevel;
	}

	void TextureBase::bSetBaseLevel(int value) {
		glTexParameteri(static_cast<GLenum>(m_props.Type), GL_TEXTURE_BASE_LEVEL, value);
		GLErr();
		m_props.BaseLevel = value;
	}

	int TextureBase::bGetMaxLevel() const {
		glGetTexParameteriv(static_cast<GLenum>(m_props.Type), GL_TEXTURE_MAX_LEVEL, &m_props.MaxLevel);
		GLErr();
		return m_props.MaxLevel;
	}

	void TextureBase::bSetMaxLevel(int value) {
		glTexParameteri(static_cast<GLenum>(m_props.Type), GL_TEXTURE_MAX_LEVEL, value);
		GLErr();
		m_props.MaxLevel = value;
	}

	float TextureBase::bGetLODBias() const {
		glGetTexParameterfv(static_cast<GLenum>(m_props.Type), GL_TEXTURE_LOD_BIAS, &m_props.LodBias);
		GLErr();
		return m_props.LodBias;
	}

	void TextureBase::bSetLODBias(float value) {
		glTexParameterf(static_cast<GLenum>(m_props.Type), GL_TEXTURE_LOD_BIAS, value);
		GLErr();
		m_props.LodBias = value;
	}

	float TextureBase::bGetMinLOD() const {
		glGetTexParameterfv(static_cast<GLenum>(m_props.Type), GL_TEXTURE_MIN_LOD, &m_props.MinLod);
		GLErr();
		return m_props.MinLod;
	}

	void TextureBase::bSetMinLOD(float value) {
		glTexParameterf(static_cast<GLenum>(m_props.Type), GL_TEXTURE_MIN_LOD, value);
		GLErr();
		m_props.MinLod = value;
	}

	float TextureBase::bGetMaxLOD() const {
		glGetTexParameterfv(static_cast<GLenum>(m_props.Type), GL_TEXTURE_MAX_LOD, &m_props.MaxLod);
		GLErr();
		return m_props.MaxLod;
	}

	void TextureBase::bSetMaxLOD(float value) {
		glTexParameterf(static_cast<GLenum>(m_props.Type), GL_TEXTURE_MAX_LOD, value);
		GLErr();
		m_props.MaxLod = value;
	}

	ogele::ChannelSwizzleValue TextureBase::bGetSwizzleR() const {
		glGetTexParameteriv(static_cast<GLenum>(m_props.Type), GL_TEXTURE_SWIZZLE_R, reinterpret_cast<int*>(&m_props.SwizzleR));
		GLErr();
		return m_props.SwizzleR;
	}

	void TextureBase::bSetSwizzleR(ChannelSwizzleValue value) {
		glTexParameteri(static_cast<GLenum>(m_props.Type), GL_TEXTURE_SWIZZLE_R, static_cast<int>(value));
		GLErr();
		m_props.SwizzleR = value;
	}

	ogele::ChannelSwizzleValue TextureBase::bGetSwizzleG() const {
		glGetTexParameteriv(static_cast<GLenum>(m_props.Type), GL_TEXTURE_SWIZZLE_G, reinterpret_cast<int*>(&m_props.SwizzleG));
		GLErr();
		return m_props.SwizzleG;
	}

	void TextureBase::bSetSwizzleG(ChannelSwizzleValue value) {
		glTexParameteri(static_cast<GLenum>(m_props.Type), GL_TEXTURE_SWIZZLE_G, static_cast<int>(value));
		GLErr();
		m_props.SwizzleG = value;
	}

	ogele::ChannelSwizzleValue TextureBase::bGetSwizzleB() const {
		glGetTexParameteriv(static_cast<GLenum>(m_props.Type), GL_TEXTURE_SWIZZLE_B, reinterpret_cast<int*>(&m_props.SwizzleB));
		GLErr();
		return m_props.SwizzleB;
	}

	void TextureBase::bSetSwizzleB(ChannelSwizzleValue value) {
		glTexParameteri(static_cast<GLenum>(m_props.Type), GL_TEXTURE_SWIZZLE_B, static_cast<int>(value));
		GLErr();
		m_props.SwizzleB = value;
	}

	ogele::ChannelSwizzleValue TextureBase::bGetSwizzleA() const {
		glGetTexParameteriv(static_cast<GLenum>(m_props.Type), GL_TEXTURE_SWIZZLE_A, reinterpret_cast<int*>(&m_props.SwizzleA));
		GLErr();
		return m_props.SwizzleA;
	}

	void TextureBase::bSetSwizzleA(ChannelSwizzleValue value) {
		glTexParameteri(static_cast<GLenum>(m_props.Type), GL_TEXTURE_SWIZZLE_A, static_cast<int>(value));
		GLErr();
		m_props.SwizzleA = value;
	}

	ogele::TextureWrapMode TextureBase::bGetWrapS() const {
		glGetTexParameteriv(static_cast<GLenum>(m_props.Type), GL_TEXTURE_WRAP_S, reinterpret_cast<int*>(&m_props.WrapS));
		GLErr();
		return m_props.WrapS;
	}

	void TextureBase::bSetWrapS(TextureWrapMode value) {
		glTexParameteri(static_cast<GLenum>(m_props.Type), GL_TEXTURE_WRAP_S, static_cast<int>(value));
		GLErr();
		m_props.WrapS = value;
	}

	ogele::TextureWrapMode TextureBase::bGetWrapT() const {
		glGetTexParameteriv(static_cast<GLenum>(m_props.Type), GL_TEXTURE_WRAP_T, reinterpret_cast<int*>(&m_props.WrapT));
		GLErr();
		return m_props.WrapT;
	}

	void TextureBase::bSetWrapT(TextureWrapMode value) {
		glTexParameteri(static_cast<GLenum>(m_props.Type), GL_TEXTURE_WRAP_T, static_cast<int>(value));
		GLErr();
		m_props.WrapT = value;
	}

	ogele::TextureWrapMode TextureBase::bGetWrapR() const {
		glGetTexParameteriv(static_cast<GLenum>(m_props.Type), GL_TEXTURE_WRAP_R, reinterpret_cast<int*>(&m_props.WrapR));
		GLErr();
		return m_props.WrapR;
	}

	void TextureBase::bSetWrapR(TextureWrapMode value) {
		glTexParameteri(static_cast<GLenum>(m_props.Type), GL_TEXTURE_WRAP_R, static_cast<int>(value));
		GLErr();
		m_props.WrapR = value;
	}

	void TextureBase::bSetWrap(TextureWrapMode value) {
		glTexParameteri(static_cast<GLenum>(m_props.Type), GL_TEXTURE_WRAP_R, static_cast<int>(value));
		GLErr();
		glTexParameteri(static_cast<GLenum>(m_props.Type), GL_TEXTURE_WRAP_S, static_cast<int>(value));
		GLErr();
		glTexParameteri(static_cast<GLenum>(m_props.Type), GL_TEXTURE_WRAP_T, static_cast<int>(value));
		GLErr();
		m_props.WrapS = value;
		m_props.WrapT = value;
		m_props.WrapR = value;
	}

	void TextureBase::bGenerateMipMap() {
		glGenerateMipmap(static_cast<GLenum>(m_props.Type));
		GLErr();
	}

	void TextureBase::SetActive(int num) {
		glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + num));
		Bind(); GLErr();
	}

	TextureBase::~TextureBase() {
		glDeleteTextures(1, &m_handle);
		GLErr();
	}
}
