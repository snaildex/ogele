//
// Created by ??????? on 21.07.2018.
//

#include "../../ogele.h"

using namespace std;
using namespace glm;
namespace fs = std::experimental::filesystem;

namespace ogele {

    void Texture2D::Allocate(const ivec2 &newSize) {
        m_size = newSize;
        Bind();
        if (HasMipmaps()) {
            int levels = std::log2(std::min(m_size.x, m_size.y));
            glTexStorage2D(static_cast<GLenum>(GetType()), levels, static_cast<GLenum>(GetFormat()), m_size.x,
                           m_size.y);
            GLErr();
            bGenerateMipMap();
        } else {
            glTexStorage2D(static_cast<GLenum>(GetType()), 1, static_cast<GLenum>(GetFormat()), m_size.x, m_size.y);
            GLErr();
        }
        Unbind();
    }

    Texture2D::Texture2D(const ivec2 &size, bool mipmaps, TextureFormat format) :
            TextureBase(TextureType::Single2D, format, mipmaps) {
        Allocate(size);
    }

#define Tex2DDef(dataType, format, pixelDataType) \
    template<> void Texture2D::bSetData< dataType >(int level, const ivec2& offset, const ivec2& size, const dataType * data)\
    {glTexSubImage2D(static_cast<GLenum>(GetType()), level, offset.x, offset.y, size.x, size.y,\
    static_cast<GLenum>(IsInteger()? PixelDataFormat::format ## Integer : PixelDataFormat::format),\
    static_cast<GLenum>(PixelDataType::pixelDataType ), data);GLErr();\
    if(HasMipmaps()) bGenerateMipMap();}\
    template<> void Texture2D::bGetData< dataType >(int level, dataType * data) const\
    {glGetTexImage(static_cast<GLenum>(GetType()), level,\
    static_cast<GLenum>(IsInteger()? PixelDataFormat::format ## Integer : PixelDataFormat::format),\
    static_cast<GLenum>(PixelDataType::pixelDataType ), data);GLErr();}

    Tex2DDef(char, RGBA, Byte);

    Tex2DDef(cvec2, RG, Byte);

    Tex2DDef(cvec3, RGB, Byte);

    Tex2DDef(cvec4, RGBA, Byte);

    Tex2DDef(unsigned char, RGBA, UnsignedByte);

    Tex2DDef(ucvec2, RG, UnsignedByte);

    Tex2DDef(ucvec3, RGB, UnsignedByte);

    Tex2DDef(ucvec4, RGBA, UnsignedByte);

    Tex2DDef(short, RGBA, Short);

    Tex2DDef(svec2, RG, Short);

    Tex2DDef(svec3, RGB, Short);

    Tex2DDef(svec4, RGBA, Short);

    Tex2DDef(unsigned short, RGBA, UnsignedShort);

    Tex2DDef(usvec2, RG, UnsignedShort);

    Tex2DDef(usvec3, RGB, UnsignedShort);

    Tex2DDef(usvec4, RGBA, UnsignedShort);

    Tex2DDef(unsigned int, Red, UnsignedInt);

    Tex2DDef(uvec2, RG, UnsignedInt);

    Tex2DDef(uvec3, RGB, UnsignedInt);

    Tex2DDef(uvec4, RGBA, UnsignedInt);

    Tex2DDef(int, Red, Int);

    Tex2DDef(ivec2, RG, Int);

    Tex2DDef(ivec3, RGB, Int);

    Tex2DDef(ivec4, RGBA, Int);

    Tex2DDef(float, Red, Float);

    Tex2DDef(vec2, RG, Float);

    Tex2DDef(vec3, RGB, Float);

    Tex2DDef(vec4, RGBA, Float);
}
