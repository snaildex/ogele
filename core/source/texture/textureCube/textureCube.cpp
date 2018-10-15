#include <texture/textureCube/textureCube.h>
#include <buffer/vectors.h>

using namespace std;
using namespace glm;

namespace ogele {

    void TextureCube::Allocate(const ivec2 &newSize) {
        m_size = newSize;
        Bind();
        if (HasMipmaps()) {
            int levels = std::log2(std::min(m_size.x, m_size.y));
            glTexStorage2D(
                    static_cast<GLenum>(GetType()), levels, static_cast<GLenum>(GetFormat()), m_size.x, m_size.y);
            GLErr();
            bGenerateMipMap();
        } else {
            glTexStorage2D(static_cast<GLenum>(GetType()), 1, static_cast<GLenum>(GetFormat()), m_size.x, m_size.y);
            GLErr();
        }
        Unbind();
    }

    TextureCube::TextureCube(const ivec2 &size, bool mipmaps, TextureFormat format) :
            TextureBase(TextureType::SingleCubeMap, format, mipmaps) {
        Allocate(size);
    }

#define TexCubeDef(dataType, format, pixelDataType) \
    template<> void TextureCube::bSetData< dataType >(int face, int level, const ivec2& offset, const ivec2& size, const dataType * data)\
    {glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+face, level, offset.x, offset.y, size.x, size.y,\
    static_cast<GLenum>(IsInteger()? PixelDataFormat::format ## Integer : PixelDataFormat::format),\
    static_cast<GLenum>(PixelDataType::pixelDataType ), data);GLErr();\
    if(HasMipmaps()) bGenerateMipMap();}\
    template<> void TextureCube::bGetData< dataType >(int face, int level, dataType * data) const\
    {glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X+face, level,\
    static_cast<GLenum>(IsInteger()? PixelDataFormat::format ## Integer : PixelDataFormat::format),\
    static_cast<GLenum>(PixelDataType::pixelDataType ), data);GLErr();};

     TexCubeDef(char, RGBA, Byte);

     TexCubeDef(cvec2, RG, Byte);

     TexCubeDef(cvec3, RGB, Byte);

     TexCubeDef(cvec4, RGBA, Byte);

     TexCubeDef(unsigned char, RGBA, UnsignedByte);

     TexCubeDef(ucvec2, RG, UnsignedByte);

     TexCubeDef(ucvec3, RGB, UnsignedByte);

     TexCubeDef(ucvec4, RGBA, UnsignedByte);

     TexCubeDef(short, RGBA, Short);

     TexCubeDef(svec2, RG, Short);

     TexCubeDef(svec3, RGB, Short);

     TexCubeDef(svec4, RGBA, Short);

     TexCubeDef(unsigned short, RGBA, UnsignedShort);

     TexCubeDef(usvec2, RG, UnsignedShort);

     TexCubeDef(usvec3, RGB, UnsignedShort);

     TexCubeDef(usvec4, RGBA, UnsignedShort);

     TexCubeDef(unsigned int, Red, UnsignedInt);

     TexCubeDef(uvec2, RG, UnsignedInt);

     TexCubeDef(uvec3, RGB, UnsignedInt);

     TexCubeDef(uvec4, RGBA, UnsignedInt);

     TexCubeDef(int, Red, Int);

     TexCubeDef(ivec2, RG, Int);

     TexCubeDef(ivec3, RGB, Int);

     TexCubeDef(ivec4, RGBA, Int);

     TexCubeDef(float, Red, Float);

     TexCubeDef(vec2, RG, Float);

    TexCubeDef(vec3, RGB, Float);

    TexCubeDef(vec4, RGBA, Float);
}
