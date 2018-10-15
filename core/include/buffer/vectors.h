#pragma once
#include <glm/glm.hpp>

namespace ogele {

	using namespace glm;

    typedef tvec2<char, lowp> lowp_cvec2;
    typedef tvec3<char, lowp> lowp_cvec3;
    typedef tvec4<char, lowp> lowp_cvec4;
    typedef tvec2<char, mediump> mediump_cvec2;
    typedef tvec3<char, mediump> mediump_cvec3;
    typedef tvec4<char, mediump> mediump_cvec4;
    typedef tvec2<char, highp> highp_cvec2;
    typedef tvec3<char, highp> highp_cvec3;
    typedef tvec4<char, highp> highp_cvec4;
#if(defined(GLM_PRECISION_LOWP_CHAR))
    typedef lowp_cvec2			cvec2;
    typedef lowp_cvec3			cvec3;
    typedef lowp_cvec4			cvec4;
#elif(defined(GLM_PRECISION_MEDIUMP_CHAR))
    typedef mediump_cvec2		cvec2;
    typedef mediump_cvec3		cvec3;
    typedef mediump_cvec4		cvec4;
#else //defined(GLM_PRECISION_HIGHP_CHAR)
    typedef highp_cvec2 cvec2;
    typedef highp_cvec3 cvec3;
    typedef highp_cvec4 cvec4;
#endif//GLM_PRECISION

    typedef tvec2<unsigned char, lowp> lowp_ucvec2;
    typedef tvec3<unsigned char, lowp> lowp_ucvec3;
    typedef tvec4<unsigned char, lowp> lowp_ucvec4;
    typedef tvec2<unsigned char, mediump> mediump_ucvec2;
    typedef tvec3<unsigned char, mediump> mediump_ucvec3;
    typedef tvec4<unsigned char, mediump> mediump_ucvec4;
    typedef tvec2<unsigned char, highp> highp_ucvec2;
    typedef tvec3<unsigned char, highp> highp_ucvec3;
    typedef tvec4<unsigned char, highp> highp_ucvec4;
#if(defined(GLM_PRECISION_LOWP_UCHAR))
    typedef lowp_ucvec2			ucvec2;
    typedef lowp_ucvec3			ucvec3;
    typedef lowp_ucvec4			ucvec4;
#elif(defined(GLM_PRECISION_MEDIUMP_UCHAR))
    typedef mediump_ucvec2		ucvec2;
    typedef mediump_ucvec3		ucvec3;
    typedef mediump_ucvec4		ucvec4;
#else //defined(GLM_PRECISION_HIGHP_UCHAR)
    typedef highp_ucvec2 ucvec2;
    typedef highp_ucvec3 ucvec3;
    typedef highp_ucvec4 ucvec4;
#endif//GLM_PRECISION

    typedef tvec2<short, lowp> lowp_svec2;
    typedef tvec3<short, lowp> lowp_svec3;
    typedef tvec4<short, lowp> lowp_svec4;
    typedef tvec2<short, mediump> mediump_svec2;
    typedef tvec3<short, mediump> mediump_svec3;
    typedef tvec4<short, mediump> mediump_svec4;
    typedef tvec2<short, highp> highp_svec2;
    typedef tvec3<short, highp> highp_svec3;
    typedef tvec4<short, highp> highp_svec4;
#if(defined(GLM_PRECISION_LOWP_SHORT))
    typedef lowp_svec2			svec2;
    typedef lowp_svec3			svec3;
    typedef lowp_svec4			svec4;
#elif(defined(GLM_PRECISION_MEDIUMP_SHORT))
    typedef mediump_svec2		svec2;
    typedef mediump_svec3		svec3;
    typedef mediump_svec4		svec4;
#else //defined(GLM_PRECISION_HIGHP_SHORT)
    typedef highp_svec2 svec2;
    typedef highp_svec3 svec3;
    typedef highp_svec4 svec4;
#endif//GLM_PRECISION

    typedef tvec2<unsigned short, lowp> lowp_usvec2;
    typedef tvec3<unsigned short, lowp> lowp_usvec3;
    typedef tvec4<unsigned short, lowp> lowp_usvec4;
    typedef tvec2<unsigned short, mediump> mediump_usvec2;
    typedef tvec3<unsigned short, mediump> mediump_usvec3;
    typedef tvec4<unsigned short, mediump> mediump_usvec4;
    typedef tvec2<unsigned short, highp> highp_usvec2;
    typedef tvec3<unsigned short, highp> highp_usvec3;
    typedef tvec4<unsigned short, highp> highp_usvec4;
#if(defined(GLM_PRECISION_LOWP_USHORT))
    typedef lowp_usvec2			usvec2;
    typedef lowp_usvec3			usvec3;
    typedef lowp_usvec4			usvec4;
#elif(defined(GLM_PRECISION_MEDIUMP_USHORT))
    typedef mediump_usvec2		usvec2;
    typedef mediump_usvec3		usvec3;
    typedef mediump_usvec4		usvec4;
#else //defined(GLM_PRECISION_HIGHP_USHORT)
    typedef highp_usvec2 usvec2;
    typedef highp_usvec3 usvec3;
    typedef highp_usvec4 usvec4;
#endif//GLM_PRECISION

}
