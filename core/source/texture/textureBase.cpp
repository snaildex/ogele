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

    TextureBase::TextureBase(TextureType type, TextureFormat format, bool mipmaps) {
        m_type = type;
        m_format = format;
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
                           static_cast<GLenum>(m_bindAccess), static_cast<GLenum>(m_format));
        GLErr();
    }

    ogele::TextureDepthStencilMode TextureBase::bGetDepthStencilMode() const {
        int res;
        glGetTexParameteriv(static_cast<GLenum>(m_type), GL_DEPTH_STENCIL_TEXTURE_MODE, &res);
        GLErr();
        return static_cast<TextureDepthStencilMode>(res);
    }

    void TextureBase::bSetDepthStencilMode(TextureDepthStencilMode value) {
        glTexParameteri(static_cast<GLenum>(m_type), GL_DEPTH_STENCIL_TEXTURE_MODE, static_cast<int>(value));
        GLErr();
    }

    glm::vec4 TextureBase::bGetBorderColor() const {
        vec4 res;
        glGetTexParameterfv(static_cast<GLenum>(m_type), GL_TEXTURE_BORDER_COLOR, &res.x);
        GLErr();
        return res;
    }

    void TextureBase::bSetBorderColor(vec4 value) {
        glTexParameterfv(static_cast<GLenum>(m_type), GL_TEXTURE_BORDER_COLOR, &value.x);
        GLErr();
    }

    ogele::DepthFuncMode TextureBase::bGetCompareFunc() const {
        int res;
        glGetTexParameteriv(static_cast<GLenum>(m_type), GL_TEXTURE_COMPARE_FUNC, &res);
        GLErr();
        return static_cast<DepthFuncMode>(res);
    }

    void TextureBase::bSetCompareFunc(DepthFuncMode value) {
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_COMPARE_FUNC, static_cast<int>(value));
        GLErr();
    }

    ogele::TextureCompareMode TextureBase::bGetCompareMode() const {
        int res;
        glGetTexParameteriv(static_cast<GLenum>(m_type), GL_TEXTURE_COMPARE_MODE, &res);
        GLErr();
        return static_cast<TextureCompareMode>(res);
    }

    void TextureBase::bSetCompareMode(TextureCompareMode value) {
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_COMPARE_MODE, static_cast<int>(value));
        GLErr();
    }

    ogele::TextureFilterMode TextureBase::bGetMinFilter() const {
        int res;
        glGetTexParameteriv(static_cast<GLenum>(m_type), GL_TEXTURE_MIN_FILTER, &res);
        GLErr();
        return static_cast<TextureFilterMode>(res);
    }

    void TextureBase::bSetMinFilter(TextureFilterMode value) {
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_MIN_FILTER, static_cast<int>(value));
        GLErr();
    }

    ogele::TextureFilterMode TextureBase::bGetMagFilter() const {
        int res;
        glGetTexParameteriv(static_cast<GLenum>(m_type), GL_TEXTURE_MAG_FILTER, &res);
        GLErr();
        return static_cast<TextureFilterMode>(res);
    }

    void TextureBase::bSetMagFilter(TextureFilterMode value) {
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_MAG_FILTER, static_cast<int>(value));
        GLErr();
    }

    int TextureBase::bGetBaseLevel() const {
        int res;
        glGetTexParameteriv(static_cast<GLenum>(m_type), GL_TEXTURE_BASE_LEVEL, &res);
        GLErr();
        return res;
    }

    void TextureBase::bSetBaseLevel(int value) {
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_BASE_LEVEL, value);
        GLErr();
    }

    int TextureBase::bGetMaxLevel() const {
        int res;
        glGetTexParameteriv(static_cast<GLenum>(m_type), GL_TEXTURE_MAX_LEVEL, &res);
        GLErr();
        return res;
    }

    void TextureBase::bSetMaxLevel(int value) {
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_MAX_LEVEL, value);
        GLErr();
    }

    float TextureBase::bGetLODBias() const {
        float res;
        glGetTexParameterfv(static_cast<GLenum>(m_type), GL_TEXTURE_LOD_BIAS, &res);
        GLErr();
        return res;
    }

    void TextureBase::bSetLODBias(float value) {
        glTexParameterf(static_cast<GLenum>(m_type), GL_TEXTURE_LOD_BIAS, value);
        GLErr();
    }

    float TextureBase::bGetMinLOD() const {
        float res;
        glGetTexParameterfv(static_cast<GLenum>(m_type), GL_TEXTURE_MIN_LOD, &res);
        GLErr();
        return res;
    }

    void TextureBase::bSetMinLOD(float value) {
        glTexParameterf(static_cast<GLenum>(m_type), GL_TEXTURE_MIN_LOD, value);
        GLErr();
    }

    float TextureBase::bGetMaxLOD() const {
        float res;
        glGetTexParameterfv(static_cast<GLenum>(m_type), GL_TEXTURE_MAX_LOD, &res);
        GLErr();
        return res;
    }

    void TextureBase::bSetMaxLOD(float value) {
        glTexParameterf(static_cast<GLenum>(m_type), GL_TEXTURE_MAX_LOD, value);
        GLErr();
    }

    ogele::ChannelSwizzleValue TextureBase::bGetSwizzleR() const {
        int res;
        glGetTexParameteriv(static_cast<GLenum>(m_type), GL_TEXTURE_SWIZZLE_R, &res);
        GLErr();
        return static_cast<ChannelSwizzleValue>(res);
    }

    void TextureBase::bSetSwizzleR(ChannelSwizzleValue value) {
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_SWIZZLE_R, static_cast<int>(value));
        GLErr();
    }

    ogele::ChannelSwizzleValue TextureBase::bGetSwizzleG() const {
        int res;
        glGetTexParameteriv(static_cast<GLenum>(m_type), GL_TEXTURE_SWIZZLE_G, &res);
        GLErr();
        return static_cast<ChannelSwizzleValue>(res);
    }

    void TextureBase::bSetSwizzleG(ChannelSwizzleValue value) {
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_SWIZZLE_G, static_cast<int>(value));
        GLErr();
    }

    ogele::ChannelSwizzleValue TextureBase::bGetSwizzleB() const {
        int res;
        glGetTexParameteriv(static_cast<GLenum>(m_type), GL_TEXTURE_SWIZZLE_B, &res);
        GLErr();
        return static_cast<ChannelSwizzleValue>(res);
    }

    void TextureBase::bSetSwizzleB(ChannelSwizzleValue value) {
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_SWIZZLE_B, static_cast<int>(value));
        GLErr();
    }

    ogele::ChannelSwizzleValue TextureBase::bGetSwizzleA() const {
        int res;
        glGetTexParameteriv(static_cast<GLenum>(m_type), GL_TEXTURE_SWIZZLE_A, &res);
        GLErr();
        return static_cast<ChannelSwizzleValue>(res);
    }

    void TextureBase::bSetSwizzleA(ChannelSwizzleValue value) {
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_SWIZZLE_A, static_cast<int>(value));
        GLErr();
    }

    ogele::TextureWrapMode TextureBase::bGetWrapS() const {
        int res;
        glGetTexParameteriv(static_cast<GLenum>(m_type), GL_TEXTURE_WRAP_S, &res);
        GLErr();
        return static_cast<TextureWrapMode>(res);
    }

    void TextureBase::bSetWrapS(TextureWrapMode value) {
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_WRAP_S, static_cast<int>(value));
        GLErr();
    }

    ogele::TextureWrapMode TextureBase::bGetWrapT() const {
        int res;
        glGetTexParameteriv(static_cast<GLenum>(m_type), GL_TEXTURE_WRAP_T, &res);
        GLErr();
        return static_cast<TextureWrapMode>(res);
    }

    void TextureBase::bSetWrapT(TextureWrapMode value) {
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_WRAP_T, static_cast<int>(value));
        GLErr();
    }

    ogele::TextureWrapMode TextureBase::bGetWrapR() const {
        int res;
        glGetTexParameteriv(static_cast<GLenum>(m_type), GL_TEXTURE_WRAP_R, &res);
        GLErr();
        return static_cast<TextureWrapMode>(res);
    }

    void TextureBase::bSetWrapR(TextureWrapMode value) {
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_WRAP_R, static_cast<int>(value));
        GLErr();
    }

    void TextureBase::bSetWrap(TextureWrapMode value) {
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_WRAP_R, static_cast<int>(value));
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_WRAP_S, static_cast<int>(value));
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_WRAP_T, static_cast<int>(value));
        GLErr();
    }

    void TextureBase::bGenerateMipMap() {
        glGenerateMipmap(static_cast<GLenum>(m_type));
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
