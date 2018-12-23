#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {
	void Texture2DArray::Allocate(const ivec3 &newSize) {
		m_size = newSize;
		Bind();
		if (HasMipmaps()) {
			int levels = std::log2(std::min(m_size.x, m_size.y));
			glTexStorage3D(static_cast<GLenum>(GetType()), levels, static_cast<GLenum>(GetFormat()), m_size.x, m_size.y, m_size.z);
			GLErr();
			bGenerateMipMap();
		}
		else {
			glTexStorage3D(static_cast<GLenum>(GetType()), 1, static_cast<GLenum>(GetFormat()), m_size.x, m_size.y, m_size.z);
			GLErr();
		}
		Unbind();
	}

	Texture2DArray::Texture2DArray(const ivec3 &size, bool mipmaps, TextureFormat format) :
		TextureBase(TextureType::Array2D, format, mipmaps) {
		Allocate(size);
	}

#define Tex2DArrayDef(dataType, format, pixelDataType) \
    template<> void Texture2DArray::bSetData< dataType >(int level, const ivec3& offset, const ivec3& size, const dataType * data)\
    {glTexSubImage3D(static_cast<GLenum>(GetType()), level, offset.x, offset.y, offset.z, size.x, size.y, size.z,\
    static_cast<GLenum>(IsInteger()? PixelDataFormat::format ## Integer : PixelDataFormat::format),\
    static_cast<GLenum>(PixelDataType::pixelDataType ), data);GLErr();\
    if(HasMipmaps()) bGenerateMipMap();}\
    template<> void Texture2DArray::bGetData< dataType >(int level, dataType * data) const\
    {glGetTexImage(static_cast<GLenum>(GetType()), level,\
    static_cast<GLenum>(IsInteger()? PixelDataFormat::format ## Integer : PixelDataFormat::format),\
    static_cast<GLenum>(PixelDataType::pixelDataType ), data);GLErr();}

	Tex2DArrayDef(char, RGBA, Byte);

	Tex2DArrayDef(cvec2, RG, Byte);

	Tex2DArrayDef(cvec3, RGB, Byte);

	Tex2DArrayDef(cvec4, RGBA, Byte);

	Tex2DArrayDef(unsigned char, RGBA, UnsignedByte);

	Tex2DArrayDef(ucvec2, RG, UnsignedByte);

	Tex2DArrayDef(ucvec3, RGB, UnsignedByte);

	Tex2DArrayDef(ucvec4, RGBA, UnsignedByte);

	Tex2DArrayDef(short, RGBA, Short);

	Tex2DArrayDef(svec2, RG, Short);

	Tex2DArrayDef(svec3, RGB, Short);

	Tex2DArrayDef(svec4, RGBA, Short);

	Tex2DArrayDef(unsigned short, RGBA, UnsignedShort);

	Tex2DArrayDef(usvec2, RG, UnsignedShort);

	Tex2DArrayDef(usvec3, RGB, UnsignedShort);

	Tex2DArrayDef(usvec4, RGBA, UnsignedShort);

	Tex2DArrayDef(unsigned int, Red, UnsignedInt);

	Tex2DArrayDef(uvec2, RG, UnsignedInt);

	Tex2DArrayDef(uvec3, RGB, UnsignedInt);

	Tex2DArrayDef(uvec4, RGBA, UnsignedInt);

	Tex2DArrayDef(int, Red, Int);

	Tex2DArrayDef(ivec2, RG, Int);

	Tex2DArrayDef(ivec3, RGB, Int);

	Tex2DArrayDef(ivec4, RGBA, Int);

	Tex2DArrayDef(float, Red, Float);

	Tex2DArrayDef(vec2, RG, Float);

	Tex2DArrayDef(vec3, RGB, Float);

	Tex2DArrayDef(vec4, RGBA, Float);
}
