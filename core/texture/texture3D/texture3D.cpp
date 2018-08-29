//
// Created by ??????? on 21.07.2018.
//

#include "../../ogele.h"

using namespace std;
using namespace glm;
namespace fs = std::experimental::filesystem;

namespace ogele {

    void Texture3D::Allocate(const ivec3 &newSize) {
        m_size = newSize;
        Bind();
        if (HasMipmaps()) {
            int levels = std::log2(std::min(m_size.x, m_size.y));
            glTexStorage3D(static_cast<GLenum>(GetType()), levels, static_cast<GLenum>(GetFormat()), m_size.x,
                           m_size.y, m_size.z);
            GLErr();
            bGenerateMipMap();
        } else {
            glTexStorage3D(static_cast<GLenum>(GetType()), 1, static_cast<GLenum>(GetFormat()), m_size.x, m_size.y, m_size.z);
            GLErr();
        }
        Unbind();
    }

    Texture3D::Texture3D(const ivec3 &size, bool mipmaps, TextureFormat format) :
            TextureBase(TextureType::Single3D, format, mipmaps) {
        Allocate(size);
    }

#define Tex3DDef(dataType, format, pixelDataType) \
    template<> void Texture3D::bSetData< dataType >(int level, const ivec3& offset, const ivec3& size, const dataType * data)\
    {glTexSubImage3D(static_cast<GLenum>(GetType()), level, offset.x, offset.y, offset.z, size.x, size.y, size.z,\
    static_cast<GLenum>(IsInteger()? PixelDataFormat::format ## Integer : PixelDataFormat::format),\
    static_cast<GLenum>(PixelDataType::pixelDataType ), data);GLErr();\
    if(HasMipmaps()) bGenerateMipMap();}\
    template<> void Texture3D::bGetData< dataType >(int level, dataType * data) const\
    {glGetTexImage(static_cast<GLenum>(GetType()), level,\
    static_cast<GLenum>(IsInteger()? PixelDataFormat::format ## Integer : PixelDataFormat::format),\
    static_cast<GLenum>(PixelDataType::pixelDataType ), data);GLErr();}

    Tex3DDef(char, RGBA, Byte);

    Tex3DDef(cvec2, RG, Byte);

    Tex3DDef(cvec3, RGB, Byte);

    Tex3DDef(cvec4, RGBA, Byte);

    Tex3DDef(unsigned char, RGBA, UnsignedByte);

    Tex3DDef(ucvec2, RG, UnsignedByte);

    Tex3DDef(ucvec3, RGB, UnsignedByte);

    Tex3DDef(ucvec4, RGBA, UnsignedByte);

    Tex3DDef(short, RGBA, Short);

    Tex3DDef(svec2, RG, Short);

    Tex3DDef(svec3, RGB, Short);

    Tex3DDef(svec4, RGBA, Short);

    Tex3DDef(unsigned short, RGBA, UnsignedShort);

    Tex3DDef(usvec2, RG, UnsignedShort);

    Tex3DDef(usvec3, RGB, UnsignedShort);

    Tex3DDef(usvec4, RGBA, UnsignedShort);

    Tex3DDef(unsigned int, Red, UnsignedInt);

    Tex3DDef(uvec2, RG, UnsignedInt);

    Tex3DDef(uvec3, RGB, UnsignedInt);

    Tex3DDef(uvec4, RGBA, UnsignedInt);

    Tex3DDef(int, Red, Int);

    Tex3DDef(ivec2, RG, Int);

    Tex3DDef(ivec3, RGB, Int);

    Tex3DDef(ivec4, RGBA, Int);

    Tex3DDef(float, Red, Float);

    Tex3DDef(vec2, RG, Float);

    Tex3DDef(vec3, RGB, Float);

    Tex3DDef(vec4, RGBA, Float);
}
