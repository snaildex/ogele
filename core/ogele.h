#ifndef OGELE_LIBRARY_H
#define OGELE_LIBRARY_H

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <tchar.h>
#include <string>
#include <sstream>
#include <memory>
#include <type_traits>
#include <stdexcept>
#include <limits>
#include <array>
#include <vector>
#include <deque>
#include <set>
#include <list>
#include <map>
#include <experimental/filesystem>
#include <math.h>
#include <functional>
#include <chrono>

#define GLEW_STATIC
#define GLM_FORCE_CXX03
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/projection.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "tinyxml2/tinyxml2.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#ifdef DOUBLE_PRECISION
#define trscalar double
#define trvec3 dvec3
#define trvec4 dvec4
#define trquat dquat
#define trmat4 dmat4
#else
#define trscalar float
#define trvec3 vec3
#define trvec4 vec4
#define trquat quat
#define trmat4 mat4
#endif

using namespace std;
using namespace glm;
namespace fs = std::experimental::filesystem;

namespace ogele
{
//region Helpers

#define Entity(Type)\
Type ( const Type & ) = delete;\
const Type & operator=( const Type & ) = delete;

#define EnumFlags(Type)\
inline constexpr Type operator| ( Type l, Type r) noexcept { return static_cast< Type >(static_cast<std::underlying_type_t< Type >>(l)|static_cast<std::underlying_type_t< Type >>(r));}\
inline constexpr Type operator& ( Type l, Type r) noexcept { return static_cast< Type >(static_cast<std::underlying_type_t< Type >>(l)&static_cast<std::underlying_type_t< Type >>(r));}

	template<typename HandleType> class HandleBase
	{
		Entity(HandleBase)
	protected:
		HandleType m_handle;
		HandleBase() = default;

	public:
		inline HandleType GetHandle() const noexcept { return m_handle; }
	};

	list<fs::path> ScanFiles(const string& extension);
//endregion

//region Vectors

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
	typedef highp_cvec2			cvec2;
	typedef highp_cvec3			cvec3;
	typedef highp_cvec4			cvec4;
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
	typedef highp_ucvec2			ucvec2;
	typedef highp_ucvec3			ucvec3;
	typedef highp_ucvec4			ucvec4;
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
	typedef highp_svec2			svec2;
	typedef highp_svec3			svec3;
	typedef highp_svec4			svec4;
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
	typedef highp_usvec2			usvec2;
	typedef highp_usvec3			usvec3;
	typedef highp_usvec4			usvec4;
#endif//GLM_PRECISION

//endregion

//region Enums

	enum class BufferTarget :GLenum
	{
		Array = GL_ARRAY_BUFFER,
		AtomicCounter = GL_ATOMIC_COUNTER_BUFFER,
		CopyRead = GL_COPY_READ_BUFFER,
		CopyWrite = GL_COPY_WRITE_BUFFER,
		DispatchIndirect = GL_DISPATCH_INDIRECT_BUFFER,
		DrawIndirect = GL_DRAW_INDIRECT_BUFFER,
		ElementArray = GL_ELEMENT_ARRAY_BUFFER,
		PixelPack = GL_PIXEL_PACK_BUFFER,
		PixelUnpack = GL_PIXEL_UNPACK_BUFFER,
		Query = GL_QUERY_BUFFER,
		ShaderStorage = GL_SHADER_STORAGE_BUFFER,
		Texture = GL_TEXTURE_BUFFER,
		TransformFeedback = GL_TRANSFORM_FEEDBACK_BUFFER,
		Uniform = GL_UNIFORM_BUFFER
	};
	enum class MutableBufferUsage :GLenum
	{
		StreamDraw = GL_STREAM_DRAW,
		StreamRead = GL_STREAM_READ,
		StreamCopy = GL_STREAM_COPY,
		StaticDraw = GL_STATIC_DRAW,
		StaticRead = GL_STATIC_READ,
		StaticCopy = GL_STATIC_COPY,
		DynamicDraw = GL_DYNAMIC_DRAW,
		DynamicRead = GL_DYNAMIC_READ,
		DynamicCopy = GL_DYNAMIC_COPY
	};
	enum class BufferFlags :GLenum
	{
		None = 0,
		MapRead = GL_MAP_READ_BIT,
		MapWrite = GL_MAP_WRITE_BIT,
		MapPersistent = GL_MAP_PERSISTENT_BIT,
		MapCoherent = GL_MAP_COHERENT_BIT,
		DynamicStorage = GL_DYNAMIC_STORAGE_BIT,
		ClientStorage = GL_CLIENT_STORAGE_BIT
	};
	EnumFlags(BufferFlags)
	enum class MappingOptionsFlags :GLenum
	{
		InvalidateRange = GL_MAP_INVALIDATE_RANGE_BIT,
		InvalidateBuffer = GL_MAP_INVALIDATE_BUFFER_BIT,
		FlushExplicit = GL_MAP_FLUSH_EXPLICIT_BIT,
		Unsynchronized = GL_MAP_UNSYNCHRONIZED_BIT
	};
	EnumFlags(MappingOptionsFlags)
	enum class BufferDataType :GLenum
	{
		Byte = GL_BYTE,
		UnsignedByte = GL_UNSIGNED_BYTE,
		Short = GL_SHORT,
		UnsignedShort = GL_UNSIGNED_SHORT,
		Int = GL_INT,
		UnsignedInt = GL_UNSIGNED_INT,
		HalfFLoat = GL_HALF_FLOAT,
		Float = GL_FLOAT,
		Fixed = GL_FIXED,
		Double = GL_DOUBLE,
	};
	enum class Topology :GLenum
	{
		Points = GL_POINTS,
		LineStrip = GL_LINE_STRIP,
		LineLoop = GL_LINE_LOOP,
		Lines = GL_LINES,
		LineStripAdjacency = GL_LINE_STRIP_ADJACENCY,
		LinesAdjacency = GL_LINES_ADJACENCY,
		TriangleStrip = GL_TRIANGLE_STRIP,
		TriangleFan = GL_TRIANGLE_FAN,
		Triangles = GL_TRIANGLES,
		TriangleStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY,
		TrianglesAdjacency = GL_TRIANGLES_ADJACENCY,
		Patches = GL_PATCHES
	};
	enum class Key : int
	{
		Unknown = GLFW_KEY_UNKNOWN,
		Space = GLFW_KEY_SPACE,
		Apostrophe = GLFW_KEY_APOSTROPHE,
		Comma = GLFW_KEY_COMMA,
		Minus = GLFW_KEY_MINUS,
		Period = GLFW_KEY_PERIOD,
		Slash = GLFW_KEY_SLASH,
		Digit0 = GLFW_KEY_0,
		Digit1 = GLFW_KEY_1,
		Digit2 = GLFW_KEY_2,
		Digit3 = GLFW_KEY_3,
		Digit4 = GLFW_KEY_4,
		Digit5 = GLFW_KEY_5,
		Digit6 = GLFW_KEY_6,
		Digit7 = GLFW_KEY_7,
		Digit8 = GLFW_KEY_8,
		Digit9 = GLFW_KEY_9,
		Semicolon = GLFW_KEY_SEMICOLON,
		Equal = GLFW_KEY_EQUAL,
		A = GLFW_KEY_A,
		B = GLFW_KEY_B,
		C = GLFW_KEY_C,
		D = GLFW_KEY_D,
		E = GLFW_KEY_E,
		F = GLFW_KEY_F,
		G = GLFW_KEY_G,
		H = GLFW_KEY_H,
		I = GLFW_KEY_I,
		J = GLFW_KEY_J,
		K = GLFW_KEY_K,
		L = GLFW_KEY_L,
		M = GLFW_KEY_M,
		N = GLFW_KEY_N,
		O = GLFW_KEY_O,
		P = GLFW_KEY_P,
		Q = GLFW_KEY_Q,
		R = GLFW_KEY_R,
		S = GLFW_KEY_S,
		T = GLFW_KEY_T,
		U = GLFW_KEY_U,
		V = GLFW_KEY_V,
		W = GLFW_KEY_W,
		X = GLFW_KEY_X,
		Y = GLFW_KEY_Y,
		Z = GLFW_KEY_Z,
		LBracket = GLFW_KEY_LEFT_BRACKET,
		Backslash = GLFW_KEY_BACKSLASH,
		RBracket = GLFW_KEY_RIGHT_BRACKET,
		GraveAccent = GLFW_KEY_GRAVE_ACCENT,
		World1 = GLFW_KEY_WORLD_1,
		World2 = GLFW_KEY_WORLD_2,

		Escape = GLFW_KEY_ESCAPE,
		Enter = GLFW_KEY_ENTER,
		Tab = GLFW_KEY_TAB,
		Backspace = GLFW_KEY_BACKSPACE,
		Insert = GLFW_KEY_INSERT,
		Delete = GLFW_KEY_DELETE,
		Right = GLFW_KEY_RIGHT,
		Left = GLFW_KEY_LEFT,
		Down = GLFW_KEY_DOWN,
		Up = GLFW_KEY_UP,
		PageUp = GLFW_KEY_PAGE_UP,
		PageDown = GLFW_KEY_PAGE_DOWN,
		Home = GLFW_KEY_HOME,
		End = GLFW_KEY_END,
		CapsLock = GLFW_KEY_CAPS_LOCK,
		ScrollLock = GLFW_KEY_SCROLL_LOCK,
		NumLock = GLFW_KEY_NUM_LOCK,
		PrintScreen = GLFW_KEY_PRINT_SCREEN,
		Pause = GLFW_KEY_PAUSE,
		F1 = GLFW_KEY_F1,
		F2 = GLFW_KEY_F2,
		F3 = GLFW_KEY_F3,
		F4 = GLFW_KEY_F4,
		F5 = GLFW_KEY_F5,
		F6 = GLFW_KEY_F6,
		F7 = GLFW_KEY_F7,
		F8 = GLFW_KEY_F8,
		F9 = GLFW_KEY_F9,
		F10 = GLFW_KEY_F10,
		F11 = GLFW_KEY_F11,
		F12 = GLFW_KEY_F12,
		F13 = GLFW_KEY_F13,
		F14 = GLFW_KEY_F14,
		F15 = GLFW_KEY_F15,
		F16 = GLFW_KEY_F16,
		F17 = GLFW_KEY_F17,
		F18 = GLFW_KEY_F18,
		F19 = GLFW_KEY_F19,
		F20 = GLFW_KEY_F20,
		F21 = GLFW_KEY_F21,
		F22 = GLFW_KEY_F22,
		F23 = GLFW_KEY_F23,
		F24 = GLFW_KEY_F24,
		F25 = GLFW_KEY_F25,
		KP0 = GLFW_KEY_KP_0,
		KP1 = GLFW_KEY_KP_1,
		KP2 = GLFW_KEY_KP_2,
		KP3 = GLFW_KEY_KP_3,
		KP4 = GLFW_KEY_KP_4,
		KP5 = GLFW_KEY_KP_5,
		KP6 = GLFW_KEY_KP_6,
		KP7 = GLFW_KEY_KP_7,
		KP8 = GLFW_KEY_KP_8,
		KP9 = GLFW_KEY_KP_9,
		KPDecimal = GLFW_KEY_KP_DECIMAL,
		KPDivide = GLFW_KEY_KP_DIVIDE,
		KPMultiply = GLFW_KEY_KP_MULTIPLY,
		KPSubstract = GLFW_KEY_KP_SUBTRACT,
		KPAdd = GLFW_KEY_KP_ADD,
		KPEnter = GLFW_KEY_KP_ENTER,
		KPEqual = GLFW_KEY_KP_EQUAL,
		LShift = GLFW_KEY_LEFT_SHIFT,
		LControl = GLFW_KEY_LEFT_CONTROL,
		LAlt = GLFW_KEY_LEFT_ALT,
		LSuper = GLFW_KEY_LEFT_SUPER,
		RShift = GLFW_KEY_RIGHT_SHIFT,
		RControl = GLFW_KEY_RIGHT_CONTROL,
		RAlt = GLFW_KEY_RIGHT_ALT,
		RSuper = GLFW_KEY_RIGHT_SUPER,
		Menu = GLFW_KEY_MENU,

		Last = GLFW_KEY_LAST
	};
	enum class KeyState : int
	{
		Press = GLFW_PRESS,
		Repeat = GLFW_REPEAT,
		Release = GLFW_RELEASE
	};
	enum class KeyModifiers : int
	{
		Alt = GLFW_MOD_ALT,
		Control = GLFW_MOD_CONTROL,
		Shift = GLFW_MOD_SHIFT,
		Super = GLFW_MOD_SUPER
	};
	EnumFlags(KeyModifiers)
	enum class MouseButton :int
	{
		Button1 = GLFW_MOUSE_BUTTON_1,
		Button2 = GLFW_MOUSE_BUTTON_2,
		Button3 = GLFW_MOUSE_BUTTON_3,
		Button4 = GLFW_MOUSE_BUTTON_4,
		Button5 = GLFW_MOUSE_BUTTON_5,
		Button6 = GLFW_MOUSE_BUTTON_6,
		Button7 = GLFW_MOUSE_BUTTON_7,
		Button8 = GLFW_MOUSE_BUTTON_8,
		Last = GLFW_MOUSE_BUTTON_LAST,
		Left = GLFW_MOUSE_BUTTON_LEFT,
		Right = GLFW_MOUSE_BUTTON_RIGHT,
		Middle = GLFW_MOUSE_BUTTON_MIDDLE
	};
	enum class BufferBit : GLbitfield
	{
		Color = GL_COLOR_BUFFER_BIT,
		Depth = GL_DEPTH_BUFFER_BIT,
		Accumulation = GL_ACCUM_BUFFER_BIT,
		Stencil = GL_STENCIL_BUFFER_BIT
	};
	EnumFlags(BufferBit)
	enum class Feature : GLenum
	{
		CullFace = GL_CULL_FACE,
		DepthTest = GL_DEPTH_TEST,
		SeamlessCubemap = GL_TEXTURE_CUBE_MAP_SEAMLESS,
		FrameBufferSRGB = GL_FRAMEBUFFER_SRGB,
		PrimitiveRestart = GL_PRIMITIVE_RESTART,
		Blend = GL_BLEND
	};
	enum class DepthFuncMode : GLenum
	{
		Less = GL_LESS,
		Equal = GL_EQUAL,
		LEqual = GL_LEQUAL,
		Greater = GL_GREATER,
		NotEqual = GL_NOTEQUAL,
		GEqual = GL_GEQUAL,
		Always = GL_ALWAYS
	};
	enum class MemoryBarriers : GLbitfield
	{
		VertexAttribArray = GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT,
		ElementArray = GL_ELEMENT_ARRAY_BARRIER_BIT,
		Uniform = GL_UNIFORM_BARRIER_BIT,
		TextureFetch = GL_TEXTURE_FETCH_BARRIER_BIT,
		ShaderImageAccess = GL_SHADER_IMAGE_ACCESS_BARRIER_BIT,
		Command = GL_COMMAND_BARRIER_BIT,
		PixelBuffer = GL_PIXEL_BUFFER_BARRIER_BIT,
		TextureUpdate = GL_TEXTURE_UPDATE_BARRIER_BIT,
		BufferUpdate = GL_BUFFER_UPDATE_BARRIER_BIT,
		Framebuffer = GL_FRAMEBUFFER_BARRIER_BIT,
		TransformFeedback = GL_TRANSFORM_FEEDBACK_BARRIER_BIT,
		QueryBuffer = GL_QUERY_BUFFER_BARRIER_BIT,
		AtomicCounter = GL_ATOMIC_COUNTER_BARRIER_BIT,
		ClientMappedBuffer = GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT,
		ShaderStorage = GL_SHADER_STORAGE_BARRIER_BIT,
		All = GL_ALL_BARRIER_BITS
	};
	EnumFlags(MemoryBarriers)
	enum class BlendFactor : GLenum
	{
		Zero = GL_ZERO,
		One = GL_ONE,
		SrcColor = GL_SRC_COLOR,
		OneMinusSrcColor = GL_ONE_MINUS_SRC_COLOR,
		DstColor = GL_DST_COLOR,
		OneMinusDstColor = GL_ONE_MINUS_DST_COLOR,
		SrcAlpha = GL_SRC_ALPHA,
		OneMinusSrcAlpha = GL_ONE_MINUS_SRC_ALPHA,
		DstAlpha = GL_DST_ALPHA,
		OneMinusDstAlpha = GL_ONE_MINUS_DST_ALPHA,
		ConstantColor = GL_CONSTANT_COLOR,
		OneMinusConstantColor = GL_ONE_MINUS_CONSTANT_COLOR,
		ConstantAlpha = GL_CONSTANT_ALPHA,
		OnaMinusConstantAlpha = GL_ONE_MINUS_CONSTANT_ALPHA,
		SrcAlphaSaturate = GL_SRC_ALPHA_SATURATE,
		Src1Color = GL_SRC1_COLOR,
		OneMinusSrc1Color = GL_ONE_MINUS_SRC1_COLOR,
		Src1Alpha = GL_SRC1_ALPHA,
		OneMinusSrc1Alpha = GL_ONE_MINUS_SRC1_ALPHA
	};
	enum class ShaderType : GLenum
	{
		Vertex = GL_VERTEX_SHADER,
		Fragment = GL_FRAGMENT_SHADER,
		Geometry = GL_GEOMETRY_SHADER,
		Compute = GL_COMPUTE_SHADER,
		TessControl = GL_TESS_CONTROL_SHADER,
		TessEvaluation = GL_TESS_EVALUATION_SHADER
	};
	enum class UniformType : GLenum
	{
		Float = GL_FLOAT,
		FloatVec2 = GL_FLOAT_VEC2,
		FloatVec3 = GL_FLOAT_VEC3,
		FloatVec4 = GL_FLOAT_VEC4,
		Double = GL_DOUBLE,
		DoubleVec2 = GL_DOUBLE_VEC2,
		DoubleVec3 = GL_DOUBLE_VEC3,
		DoubleVec4 = GL_DOUBLE_VEC4,
		Int = GL_INT,
		IntVec2 = GL_INT_VEC2,
		IntVec3 = GL_INT_VEC3,
		IntVec4 = GL_INT_VEC4,
		UnsignedInt = GL_UNSIGNED_INT,
		UnsignedIntVec2 = GL_UNSIGNED_INT_VEC2,
		UnsignedIntVec3 = GL_UNSIGNED_INT_VEC3,
		UnsignedIntVec4 = GL_UNSIGNED_INT_VEC4,
		Bool = GL_BOOL,
		BoolVec2 = GL_BOOL_VEC2,
		BoolVec3 = GL_BOOL_VEC3,
		BoolVec4 = GL_BOOL_VEC4,
		FloatMat2 = GL_FLOAT_MAT2,
		FloatMat3 = GL_FLOAT_MAT3,
		FloatMat4 = GL_FLOAT_MAT4,
		FloatMat2x3 = GL_FLOAT_MAT2x3,
		FloatMat2x4 = GL_FLOAT_MAT2x4,
		FloatMat3x2 = GL_FLOAT_MAT3x2,
		FloatMat3x4 = GL_FLOAT_MAT3x4,
		FloatMat4x2 = GL_FLOAT_MAT4x2,
		FloatMat4x3 = GL_FLOAT_MAT4x3,
		DoubleMat2 = GL_DOUBLE_MAT2,
		DoubleMat3 = GL_DOUBLE_MAT3,
		DoubleMat4 = GL_DOUBLE_MAT4,
		DoubleMat2x3 = GL_DOUBLE_MAT2x3,
		DoubleMat2x4 = GL_DOUBLE_MAT2x4,
		DoubleMat3x2 = GL_DOUBLE_MAT3x2,
		DoubleMat3x4 = GL_DOUBLE_MAT3x4,
		DoubleMat4x2 = GL_DOUBLE_MAT4x2,
		DoubleMat4x3 = GL_DOUBLE_MAT4x3,
		Sampler1D = GL_SAMPLER_1D,
		Sampler2D = GL_SAMPLER_2D,
		Sampler3D = GL_SAMPLER_3D,
		SamplerCube = GL_SAMPLER_CUBE,
		SamplerCubeArray = GL_SAMPLER_CUBE_MAP_ARRAY,
		Sampler1DShadow = GL_SAMPLER_1D_SHADOW,
		Sampler2DShadow = GL_SAMPLER_2D_SHADOW,
		Sampler1DArray = GL_SAMPLER_1D_ARRAY,
		Sampler2DArray = GL_SAMPLER_2D_ARRAY,
		Sampler1DArrayShadow = GL_SAMPLER_1D_ARRAY_SHADOW,
		Sampler2DArrayShadow = GL_SAMPLER_2D_ARRAY_SHADOW,
		Sampler2DMultisample = GL_SAMPLER_2D_MULTISAMPLE,
		Sampler2DMultisampleArray = GL_SAMPLER_2D_MULTISAMPLE_ARRAY,
		SamplerCubeShadow = GL_SAMPLER_CUBE_SHADOW,
		SamplerCubeArrayShadow = GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW,
		SamplerBuffer = GL_SAMPLER_BUFFER,
		Sampler2DRect = GL_SAMPLER_2D_RECT,
		Sampler2DRectShadow = GL_SAMPLER_2D_RECT_SHADOW,
		IntSampler1D = GL_INT_SAMPLER_1D,
		IntSampler2D = GL_INT_SAMPLER_2D,
		IntSampler3D = GL_INT_SAMPLER_3D,
		IntSamplerCube = GL_INT_SAMPLER_CUBE,
		IntSamplerCubeArray = GL_INT_SAMPLER_CUBE_MAP_ARRAY,
		IntSampler1DArray = GL_INT_SAMPLER_1D_ARRAY,
		IntSampler2DArray = GL_INT_SAMPLER_2D_ARRAY,
		IntSampler2DMultisample = GL_INT_SAMPLER_2D_MULTISAMPLE,
		IntSampler2DMultisampleArray = GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY,
		IntSamplerBuffer = GL_INT_SAMPLER_BUFFER,
		IntSampler2DRect = GL_INT_SAMPLER_2D_RECT,
		UnsignedIntSampler1D = GL_UNSIGNED_INT_SAMPLER_1D,
		UnsignedIntSampler2D = GL_UNSIGNED_INT_SAMPLER_2D,
		UnsignedIntSampler3D = GL_UNSIGNED_INT_SAMPLER_3D,
		UnsignedIntSamplerCube = GL_UNSIGNED_INT_SAMPLER_CUBE,
		UnsignedIntSamplerCubeArray = GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY,
		UnsignedIntSampler1DArray = GL_UNSIGNED_INT_SAMPLER_1D_ARRAY,
		UnsignedIntSampler2DArray = GL_UNSIGNED_INT_SAMPLER_2D_ARRAY,
		UnsignedIntSampler2DMultisample = GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE,
		UnsignedIntSampler2DMultisampleArray = GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY,
		UnsignedIntSamplerBuffer = GL_UNSIGNED_INT_SAMPLER_BUFFER,
		UnsignedIntSampler2DRect = GL_UNSIGNED_INT_SAMPLER_2D_RECT,
		Image1D = GL_IMAGE_1D,
		Image2D = GL_IMAGE_2D,
		Image3D = GL_IMAGE_3D,
		Image2DRect = GL_IMAGE_2D_RECT,
		ImageCube = GL_IMAGE_CUBE,
		ImageCubeArray = GL_IMAGE_CUBE_MAP_ARRAY,
		ImageBuffer = GL_IMAGE_BUFFER,
		Image1DArray = GL_IMAGE_1D_ARRAY,
		Image2DArray = GL_IMAGE_2D_ARRAY,
		Image2DMultisample = GL_IMAGE_2D_MULTISAMPLE,
		Image2DMultisampleArray = GL_IMAGE_2D_MULTISAMPLE_ARRAY,
		IntImage1D = GL_INT_IMAGE_1D,
		IntImage2D = GL_INT_IMAGE_2D,
		IntImage3D = GL_INT_IMAGE_3D,
		IntImage2DRect = GL_INT_IMAGE_2D_RECT,
		IntImageCube = GL_INT_IMAGE_CUBE,
		IntImageCubeArray = GL_INT_IMAGE_CUBE_MAP_ARRAY,
		IntImageBuffer = GL_INT_IMAGE_BUFFER,
		IntImage1DArray = GL_INT_IMAGE_1D_ARRAY,
		IntImage2DArray = GL_INT_IMAGE_2D_ARRAY,
		IntImage2DMultisample = GL_INT_IMAGE_2D_MULTISAMPLE,
		IntImage2DMultisampleArray = GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY,
		UnsignedIntImage1D = GL_UNSIGNED_INT_IMAGE_1D,
		UnsignedIntImage2D = GL_UNSIGNED_INT_IMAGE_2D,
		UnsignedIntImage3D = GL_UNSIGNED_INT_IMAGE_3D,
		UnsignedIntImage2DRect = GL_UNSIGNED_INT_IMAGE_2D_RECT,
		UnsignedIntImageCube = GL_UNSIGNED_INT_IMAGE_CUBE,
		UnsignedIntImageCubeArray = GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY,
		UnsignedIntImageBuffer = GL_UNSIGNED_INT_IMAGE_BUFFER,
		UnsignedIntImage1DArray = GL_UNSIGNED_INT_IMAGE_1D_ARRAY,
		UnsignedIntImage2DArray = GL_UNSIGNED_INT_IMAGE_2D_ARRAY,
		UnsignedIntImage2DMultisample = GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE,
		UnsignedIntImage2DMultisampleArray = GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY,
		UnsignedIntAtomicCounter = GL_UNSIGNED_INT_ATOMIC_COUNTER
	};
	enum class TextureType : GLenum
	{
		Single1D = GL_TEXTURE_1D,
		Single2D = GL_TEXTURE_2D,
		Single3D = GL_TEXTURE_3D,
		Array1D = GL_TEXTURE_1D_ARRAY,
		Array2D = GL_TEXTURE_2D_ARRAY,
		SingleRectangle = GL_TEXTURE_RECTANGLE,
		SingleCubeMap = GL_TEXTURE_CUBE_MAP,
		ArrayCubeMap = GL_TEXTURE_CUBE_MAP_ARRAY,
		SingleBuffer = GL_TEXTURE_BUFFER,
		Single2DMultisample = GL_TEXTURE_2D_MULTISAMPLE,
		Array2DMultisample = GL_TEXTURE_2D_MULTISAMPLE_ARRAY,

		SingleCubeMapPositiveX = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		SingleCubeMapPositiveY = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		SingleCubeMapPositiveZ = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		SingleCubeMapNegativeX = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		SingleCubeMapNegativeY = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		SingleCubeMapNegativeZ = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,

		Single1DProxy = GL_PROXY_TEXTURE_1D,
		Single2DProxy = GL_PROXY_TEXTURE_2D,
		Single3DProxy = GL_PROXY_TEXTURE_3D,
		Array1DProxy = GL_PROXY_TEXTURE_1D_ARRAY,
		Array2DProxy = GL_PROXY_TEXTURE_2D_ARRAY,
		SingleRectangleProxy = GL_PROXY_TEXTURE_RECTANGLE,
		SingleCubeMapProxy = GL_PROXY_TEXTURE_CUBE_MAP,
		ArrayCubeMapProxy = GL_PROXY_TEXTURE_CUBE_MAP_ARRAY,
		Single2DMultisampleProxy = GL_PROXY_TEXTURE_2D_MULTISAMPLE,
		Array2DMultisampleProxy = GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY
	};
	enum class TextureParameter : GLenum
	{
		DepthStencilMode = GL_DEPTH_STENCIL_TEXTURE_MODE,
		BaseLevel = GL_TEXTURE_BASE_LEVEL,
		CompareFunc = GL_TEXTURE_COMPARE_FUNC,
		CompareMode = GL_TEXTURE_COMPARE_MODE,
		LodBias = GL_TEXTURE_LOD_BIAS,
		MinFilter = GL_TEXTURE_MIN_FILTER,
		MagFilter = GL_TEXTURE_MAG_FILTER,
		MinLod = GL_TEXTURE_MIN_LOD,
		MaxLod = GL_TEXTURE_MAX_LOD,
		MaxLevel = GL_TEXTURE_MAX_LEVEL,
		SwizzleR = GL_TEXTURE_SWIZZLE_R,
		SwizzleG = GL_TEXTURE_SWIZZLE_G,
		SwizzleB = GL_TEXTURE_SWIZZLE_B,
		SwizzleA = GL_TEXTURE_SWIZZLE_A,
		WrapS = GL_TEXTURE_WRAP_S,
		WrapT = GL_TEXTURE_WRAP_T,
		WrapR = GL_TEXTURE_WRAP_R,
		BorderColor = GL_TEXTURE_BORDER_COLOR,
		SwizzleRGBA = GL_TEXTURE_SWIZZLE_RGBA
	};
	enum class AccessMode : GLenum
	{
		ReadOnly = GL_READ_ONLY,
		WriteOnly = GL_WRITE_ONLY,
		ReadWrite = GL_READ_WRITE
	};
	enum class TextureDepthStencilMode : GLenum
	{
		DepthComponent = GL_DEPTH_COMPONENT
	};
	enum class TextureCompareMode : GLenum
	{
		CompareRefToTexture = GL_COMPARE_REF_TO_TEXTURE,
		None = GL_NONE
	};
	enum class TextureFilterMode : GLenum
	{
		Nearest = GL_NEAREST,
		Linear = GL_LINEAR,
		NearestMipMapNearest = GL_NEAREST_MIPMAP_NEAREST,
		NearestMipMapLinear = GL_NEAREST_MIPMAP_LINEAR,
		LinearMipMapNearest = GL_LINEAR_MIPMAP_NEAREST,
		LinearMipMapLinear = GL_LINEAR_MIPMAP_LINEAR
	};
	enum class ChannelSwizzleValue :GLenum
	{
		Red = GL_RED,
		Green = GL_GREEN,
		Blue = GL_BLUE,
		Alpha = GL_ALPHA,
		Zero = GL_ZERO,
		One = GL_ONE
	};
	enum class TextureWrapMode : GLenum
	{
		ClampToEdge = GL_CLAMP_TO_EDGE,
		ClampToBorder = GL_CLAMP_TO_BORDER,
		MirroredRepeat = GL_MIRRORED_REPEAT,
		Repeat = GL_REPEAT,
		MirrorClampToEdge = GL_MIRROR_CLAMP_TO_EDGE
	};
	enum class TextureFormat :GLenum
	{
		DepthComponent = GL_DEPTH_COMPONENT,
		DepthStencil = GL_DEPTH_STENCIL,

		R8 = GL_R8,
		R8_SNorm = GL_R8_SNORM,
		R16 = GL_R16,
		R16_SNorm = GL_R16_SNORM,
		RG8 = GL_RG8,
		RG8_SNorm = GL_RG8_SNORM,
		RG16 = GL_RG16,
		RG16_SNorm = GL_RG16_SNORM,

		R3G3B2 = GL_R3_G3_B2,
		RGB4 = GL_RGB4,
		RGB5 = GL_RGB5,
		RGB8 = GL_RGB8,
		RGB8_SNorm = GL_RGB8_SNORM,
		RGB10 = GL_RGB10,
		RGB12 = GL_RGB12,
		RGB16_SNorm = GL_RGB16_SNORM,

		RGBA2 = GL_RGBA2,
		RGBA4 = GL_RGBA4,
		RGB5A1 = GL_RGB5_A1,
		RGBA8 = GL_RGBA8,
		RGBA8_SNorm = GL_RGBA8_SNORM,
		RGB10A2 = GL_RGB10_A2,
		RGB10A2UI = GL_RGB10_A2UI,
		RGBA12 = GL_RGBA12,
		RGBA16 = GL_RGBA16,

		SRGB8 = GL_SRGB8,
		SRGB8A8 = GL_SRGB8_ALPHA8,

		R16F = GL_R16F,
		RG16F = GL_RG16F,
		RGB16F = GL_RGB16F,
		RGBA16F = GL_RGBA16F,
		R32F = GL_R32F,
		RG32F = GL_RG32F,
		RGB32F = GL_RGB32F,
		RGBA32F = GL_RGBA32F,
		R11FG11FB10F = GL_R11F_G11F_B10F,

		RGB9E5 = GL_RGB9_E5,

		R8I = GL_R8I,
		R8UI = GL_R8UI,
		R16I = GL_R16I,
		R16UI = GL_R16UI,
		R32I = GL_R32I,
		R32UI = GL_R32UI,
		RG8I = GL_RG8I,
		RG8UI = GL_RG8UI,
		RG16I = GL_RG16I,
		RG16UI = GL_RG16UI,
		RG32I = GL_RG32I,
		RG32UI = GL_RG32UI,
		RGB8I = GL_RGB8I,
		RGB8UI = GL_RGB8UI,
		RGB16I = GL_RGB16I,
		RGB16UI = GL_RGB16UI,
		RGB32I = GL_RGB32I,
		RGB32UI = GL_RGB32UI,
		RGBA8I = GL_RGBA8I,
		RGBA8UI = GL_RGBA8UI,
		RGBA16I = GL_RGBA16I,
		RGBA16UI = GL_RGBA16UI,
		RGBA32I = GL_RGBA32I,
		RGBA32UI = GL_RGBA32UI,

		Compressed_Red = GL_COMPRESSED_RED,
		Compressed_RG = GL_COMPRESSED_RG,
		Compressed_RGB = GL_COMPRESSED_RGB,
		Compressed_RGBA = GL_COMPRESSED_RGBA,
		Compressed_SRGB = GL_COMPRESSED_SRGB,
		Compressed_SRGB_Alpha = GL_COMPRESSED_SRGB_ALPHA,
		Compressed_Red_RGTC1 = GL_COMPRESSED_RED_RGTC1,
		Compressed_Signed_Red_RGTC1 = GL_COMPRESSED_SIGNED_RED_RGTC1,
		Compressed_RG_RGTC2 = GL_COMPRESSED_RG_RGTC2,
		Compressed_Signed_RG_RGTC2 = GL_COMPRESSED_SIGNED_RG_RGTC2,
		Compressed_RGBA_BPTC_UNorm = GL_COMPRESSED_RGBA_BPTC_UNORM,
		Compressed_SRGB_Alpha_BPTC_UNorm = GL_COMPRESSED_RGBA_BPTC_UNORM,
		Compressed_RGB_BPTC_Signed_Float = GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT,
		Compressed_RGB_BPTC_Unsigned_Float = GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT
	};
	enum class PixelDataFormat :GLenum
	{
		DepthComponent = GL_DEPTH_COMPONENT,
		DepthStencil = GL_DEPTH_STENCIL,
		Red = GL_RED,
		RG = GL_RG,
		RGB = GL_RGB,
		BGR = GL_BGR,
		RGBA = GL_RGBA,
		BGRA = GL_BGRA,
		RedInteger = GL_RED_INTEGER,
		RGInteger = GL_RG_INTEGER,
		RGBInteger = GL_RGB_INTEGER,
		BGRInteger = GL_BGR_INTEGER,
		RGBAInteger = GL_RGBA_INTEGER,
		BGRAInteger = GL_BGRA_INTEGER
	};
	enum class PixelDataType : GLenum
	{
		UnsignedByte = GL_UNSIGNED_BYTE,
		Byte = GL_BYTE,
		UnsignedShort = GL_UNSIGNED_SHORT,
		Short = GL_SHORT,
		UnsignedInt = GL_UNSIGNED_INT,
		Int = GL_INT,
		Float = GL_FLOAT,
		UnsignedByte_3_3_2 = GL_UNSIGNED_BYTE_3_3_2,
		UnsignedByte_2_3_3_Rev = GL_UNSIGNED_BYTE_2_3_3_REV,
		UnsignedShort_5_6_5 = GL_UNSIGNED_SHORT_5_6_5,
		UnsignedShort_5_6_5_Rev = GL_UNSIGNED_SHORT_5_6_5_REV,
		UnsignedShort_4_4_4_4 = GL_UNSIGNED_SHORT_4_4_4_4,
		UnsignedShort_4_4_4_4_Rev = GL_UNSIGNED_SHORT_4_4_4_4_REV,
		UnsignedShort_5_5_5_1 = GL_UNSIGNED_SHORT_5_5_5_1,
		UnsignedShort_1_5_5_5_Rev = GL_UNSIGNED_SHORT_1_5_5_5_REV,
		UnsignedInt_8_8_8_8 = GL_UNSIGNED_INT_8_8_8_8,
		UnsignedInt_8_8_8_8_Rev = GL_UNSIGNED_INT_8_8_8_8_REV,
		UnsignedInt_10_10_10_2 = GL_UNSIGNED_INT_10_10_10_2,
		UnsignedInt_2_10_10_10_Rev = GL_UNSIGNED_INT_2_10_10_10_REV
	};
	enum class RenderBufferFormat :GLenum
	{
		RGBA4 = GL_RGBA4,
		RGBA8 = GL_RGBA8,
		RGB565 = GL_RGB565,
		RGB5A1 = GL_RGB5_A1,
		DepthComponent = GL_DEPTH_COMPONENT,
		DepthComponent16 = GL_DEPTH_COMPONENT16,
		DepthComponent24 = GL_DEPTH_COMPONENT24,
		DepthComponent32 = GL_DEPTH_COMPONENT32,
		StencilIndex8 = GL_STENCIL_INDEX8,
		StencilIndex16 = GL_STENCIL_INDEX16
	};
	enum class FrameBufferCompletion
	{
		Complete = GL_FRAMEBUFFER_COMPLETE,
		Undefined = GL_FRAMEBUFFER_UNDEFINED,
		IncompleteAttachment = GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT,
		IncompleteMissingAttachment = GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT,
		IncompleteDrawBuffer = GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER,
		IncompleteReadBuffer = GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER,
		Unsupported = GL_FRAMEBUFFER_UNSUPPORTED,
		IncompleteMultisample = GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE,
		IncompleteLayerTargets = GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS
	};
	enum class DefaultColorBuffers :GLenum
	{
		FrontLeft = GL_FRONT_LEFT,
		FrontRight = GL_FRONT_RIGHT,
		BackLeft = GL_BACK_LEFT,
		BackRight = GL_BACK_RIGHT,
		Front = GL_FRONT,
		Back = GL_BACK,
		Left = GL_LEFT,
		Right = GL_RIGHT
	};
	enum class QueryType : GLenum
	{
		TimeStamp = GL_TIMESTAMP,
		TimeElapsed = GL_TIME_ELAPSED,
		SamplesPassed = GL_SAMPLES_PASSED,
		AnySamplesPassed = GL_ANY_SAMPLES_PASSED,
		PrimitivesGenerated = GL_PRIMITIVES_GENERATED,
		TransformFeedbackPrimitivesWritten = GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN
	};
//endregion

//region Window

	class Window : public HandleBase<GLFWwindow*>
	{
		Entity(Window)

	private:
		dvec2 m_oldCursorPos;
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void CharCallback(GLFWwindow* window, unsigned int character);
		static void MouseBtnCallback(GLFWwindow* window, int button, int action, int mods);
		static void CursorPosCallback(GLFWwindow* window, double x, double y);
		static void CursorEnterCallback(GLFWwindow* window, int entered);
		static void ScrollCallback(GLFWwindow* window, double x, double y);
		static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

	protected:
		Window(const glm::ivec2& size, const std::string& caption, bool firstInit);
		virtual void OnKey(Key key, int scancode, KeyState state, KeyModifiers mods) {};
		virtual void OnChar(wchar_t character) {};
		virtual void OnMouseBtn(MouseButton button, KeyState state, KeyModifiers mods) {};
		virtual void OnCursorPos(const glm::dvec2& pos, const glm::dvec2& delta) {};
		virtual void OnCursorEnter(bool entered) {};
		virtual void OnScroll(const glm::dvec2& offset) {};
		virtual void OnResize(const glm::ivec2& size) {};
		inline void MakeContextCurrent() const { glfwMakeContextCurrent(m_handle); }
		inline void SwapBuffers() { glfwSwapBuffers(m_handle); }
		inline void PollEvents() { glfwPollEvents(); }
		inline bool ShouldClose() const { return glfwWindowShouldClose(m_handle); }
		inline void SetShouldClose(bool value) { glfwSetWindowShouldClose(m_handle, value); }
	public:
		inline bool GetKey(Key key) const { return glfwGetKey(m_handle, static_cast<int>(key)) == GLFW_PRESS; }
		inline KeyState GetMouseButton(MouseButton btn) const { return static_cast<KeyState>(glfwGetMouseButton(m_handle, static_cast<int>(btn))); }
		inline ivec2 GetResolution() const { ivec2 res; glfwGetFramebufferSize(m_handle, &res.x, &res.y); return res; }
		inline dvec2 GetCursorPos() const { dvec2 res; glfwGetCursorPos(m_handle, &res.x, &res.y); return res; }
		inline void SetCursorPos(const dvec2& value) { glfwSetCursorPos(m_handle, value.x, value.y); }
		inline void LockCursor(bool value) const { glfwSetInputMode(m_handle, GLFW_CURSOR, value ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL); }
		~Window();
	};
//endregion

//region Application

	class ResourceContainer;
	class ScreenQuadMesh;
	class ShaderProgram;
	class TextureBase;

	class Application :public Window
	{
		Entity(Application)

	private:
		static Application* m_instance;
		unique_ptr<ResourceContainer> m_res;
		ivec2 m_screenSize;
		chrono::high_resolution_clock::time_point m_lastFrameTimePoint;
		double m_timeDelta, m_time;
		void CreateInternal();
		ScreenQuadMesh* m_screenQuad;
		ShaderProgram* m_drawTex;
	protected:
		virtual void Start() {};
		virtual void Update() {};
		virtual void Shutdown() {};

		void DrawTex(TextureBase* tex);
	public:
		Application();

		inline ScreenQuadMesh* GetScreenQuadMesh() const noexcept { return m_screenQuad; }
		inline static Application* GetInstance() noexcept { return m_instance; }
		inline double GetTime() const noexcept { return m_time; }
		inline double GetTimeDelta() const noexcept { return m_timeDelta; }
		inline ResourceContainer* GetResources() noexcept { return m_res.get(); }
		inline const ivec2& GetScreenResolution() const noexcept { return m_screenSize; }
		void Run();

		~Application();
	};
//endregion

//region Exception

	class GLError : public std::runtime_error
	{
		GLenum m_code;
	public:
		GLError(GLenum code) noexcept;

		const char* what() const noexcept override;
		const wchar_t* whatW() const;

	};

	void CheckGLError();

#ifdef _DEBUG
#define GLErr() CheckGLError()
#else
#define GLErr()
#endif //_DEBUG
//endregion

//region Object

	class Object
	{
		string m_name;
		set<string> m_tags;

	public:
		Object() { m_name = "New object"; }

		inline const string& GetName() const noexcept { return m_name; }
		inline void SetName(const string& name) noexcept { m_name = name; }

		inline bool HasTag(const string& tag) const noexcept { return m_tags.find(tag) != m_tags.cend(); }
		inline void AddTag(const string& tag) noexcept { m_tags.insert(tag); }
		inline void RemoveTag(const string& tag) noexcept { m_tags.erase(tag); }

		string PrintTags();

		void CopyNameTagsFrom(const Object* source) noexcept
		{
			m_name = source->m_name;
			m_tags = source->m_tags;
		}

		virtual ~Object() = default;
	};
//endregion

//region Resource

	class Resource : public Object
	{
		Entity(Resource)
	public:
		Resource() = default;
		virtual ~Resource() = default;
	};

	class ResourceLoader
	{
		Entity(ResourceLoader)
	protected:
		ResourceLoader() = default;
		void LoadNameTags(const tinyxml2::XMLElement* reader, Resource* res) const;
	public:
		virtual ~ResourceLoader() {}
		virtual Resource* Load(const tinyxml2::XMLElement* reader) const = 0;
	};

	class ResourceContainer
	{
		Entity(ResourceContainer)

	private:

		list<unique_ptr<Resource>> m_data;
		map <string, unique_ptr<ResourceLoader>> m_loaders;

	public:

		ResourceContainer();

		void LoadFromFilesystem();

		void AddResource(Resource* resource) noexcept
		{
			m_data.emplace_back(unique_ptr<Resource>(resource));
		}

		void GUI();

		template<typename T> T* GetResourceByName(const string& name) const noexcept
		{
			for (const auto& t : m_data)
				if (t->GetName() == name)
				{
					T* res = dynamic_cast<T*>(t.get());
					if (res != nullptr)
						return res;
				}
			return nullptr;
		}

		template<typename T> T* GetResourceByTag(const string& tag) const noexcept
		{
			for (const auto& t : m_data)
				if (t->HasTag(tag))
				{
					T* res = dynamic_cast<T*>(t.get());
					if (res != nullptr)
						return res;
				}
		}

		template<typename T> vector<T*> GetResources() const noexcept
		{
			vector<T*> res;
			for (const auto& t : m_data)
			{
				T* r = dynamic_cast<T*>(t.get());
				if (r != nullptr)
					res.push_back(r);
			}
			return res;
		}

		template<typename T> vector<T*> GetResourcesByTag(const string& tag) const noexcept
		{
			vector<T*> res;
			for (const auto& t : m_data)
				if (t->HasTag(tag))
				{
					T* r = dynamic_cast<T*>(t.get());
					if (r != nullptr)
						res.push_back(r);
				}
			return res;
		}

	};
//endregion

//region Static

	void ClearColor(const glm::vec4& color);
	void Clear(BufferBit buffers);
	void Viewport(const glm::ivec2& offset, const glm::ivec2& size);
	void Enable(Feature feature);
	void Disable(Feature feature);
	void Barrier(MemoryBarriers barriers);
//endregion

//region VertexBuffer

	class BufferBase :public HandleBase<GLenum>
	{
		Entity(BufferBase)

		BufferDataType m_dataType;
		int m_size;
		size_t m_elemSize;
		size_t m_structSize;

	protected:
		BufferBase(BufferDataType dataType, size_t structSize, int size);

	public:

		inline BufferDataType GetDataType() const noexcept { return m_dataType; }

		inline void Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_handle); GLErr(); }
		inline void Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); GLErr(); }

		inline void Bind(BufferTarget usage) const { glBindBuffer(static_cast<GLenum>(usage), m_handle); GLErr(); }
		inline void Unbind(BufferTarget usage) const { glBindBuffer(static_cast<GLenum>(usage), 0); GLErr(); }
		inline void BindBase(BufferTarget usage, int bindingIndex) { glBindBufferBase(static_cast<GLenum>(usage), bindingIndex, m_handle); GLErr(); }

		constexpr inline size_t GetElementSize() const noexcept { return m_elemSize; }
		constexpr inline size_t GetStructSize() const noexcept { return m_structSize; }
		inline int GetSize() const noexcept { return m_size; }

		~BufferBase();

	};

	template <typename T> class Buffer : public BufferBase
	{
		BufferFlags m_flags;

	protected:
		Buffer(BufferDataType dataType, BufferFlags flags, int size, const T* data = nullptr) : BufferBase(dataType, sizeof(T), size)
		{
			m_flags = flags;
			Bind();
			glBufferStorage(GL_ARRAY_BUFFER, sizeof(T)*size, data, static_cast<GLbitfield>(flags));
			GLErr();
			Unbind();
		}

	public:

		inline T* bMap(AccessMode accessMode) { T* r = (T*)glMapBuffer(GL_ARRAY_BUFFER, static_cast<GLbitfield>(accessMode));  GLErr(); return r; }
		inline void bUnmap() { glUnmapBuffer(GL_ARRAY_BUFFER); GLErr(); }

		inline T* bMapRange(int first, int count, MappingOptionsFlags options) { T* r = (T*)glMapBufferRange(GL_ARRAY_BUFFER, sizeof(T)*first, sizeof(T)*count, static_cast<GLbitfield>(m_flags) | static_cast<GLbitfield>(options));  GLErr(); return r; }
		inline void bFlushRange(int first, int count) { glFlushMappedBufferRange(GL_ARRAY_BUFFER, sizeof(T)*first, sizeof(T)*count); GLErr(); }

		inline void bSetData(const T* dataPtr, int first, int count) { glBufferSubData(GL_ARRAY_BUFFER, sizeof(T)*first, sizeof(T)*count, dataPtr); GLErr(); }
		inline void bGetData(T* dataPtr, int first, int count) const { glGetBufferSubData(GL_ARRAY_BUFFER, sizeof(T)*first, sizeof(T)*count, dataPtr); GLErr(); }

		inline void bClear(TextureFormat internalFormat, PixelDataFormat format, T value)
		{
			glClearBufferData(GL_ARRAY_BUFFER, static_cast<GLenum>(internalFormat), static_cast<GLenum>(format), static_cast<GLenum>(GetDataType()),&value);
			GLErr();
		}
		inline void bClearRange(TextureFormat internalFormat, int offset, int size, PixelDataFormat format, T value)
		{
			glClearBufferSubData(GL_ARRAY_BUFFER, static_cast<GLenum>(internalFormat), offset * sizeof(T), size * sizeof(T), static_cast<GLenum>(format),static_cast<GLenum>(GetDataType()),&value);
			GLErr();
		}

	};

#define DefBuffer(typeName) template <typename T> class typeName ## Buffer : public Buffer<T> { public: typeName ## Buffer(BufferFlags flags, int count, const T* data = nullptr) : Buffer<T>(BufferDataType::typeName , flags, count, data) {} };
	DefBuffer(Byte)
	DefBuffer(UnsignedByte)
	DefBuffer(Short)
	DefBuffer(UnsignedShort)
	DefBuffer(Int)
	DefBuffer(UnsignedInt)
	DefBuffer(HalfFLoat)
	DefBuffer(Float)
	DefBuffer(Fixed)
	DefBuffer(Double)
//endregion

//region VertexArray

	class VertexArray : HandleBase<GLuint>
	{
		Entity(VertexArray)
		Topology m_topology;
		bool m_indexed;

	public:

		VertexArray(Topology topology);

		inline Topology GetTopology() const noexcept { return m_topology; }

		inline void Bind() const { glBindVertexArray(m_handle); GLErr(); }
		inline void Unbind() const { glBindVertexArray(0); GLErr(); }
		inline bool IsIndexed() const noexcept { return m_indexed; }
		inline void bEnableVertexAttribArray(int index) { glEnableVertexArrayAttrib(m_handle, index); GLErr(); }
		inline void bDisableVertexAttribArray(int index) { glDisableVertexArrayAttrib(m_handle, index); GLErr(); }
		void bDraw(int first, int count) const;
		void bDraw(int first, int count, int instanceCount) const;

		void bAttachBuffer(int index, int vectorLength, const BufferBase* buffer, int offset = 0);
		void bAttachElementBuffer(const UnsignedIntBuffer<unsigned int>* buffer);

		~VertexArray();
	};
//endregion

//region Shader

	class ShaderCompileException : public std::runtime_error
	{
		std::string m_message;
	public:
		ShaderCompileException(const std::string& message);

		const char* what() const noexcept override;

		~ShaderCompileException();
	};

	class ShaderStage : public HandleBase<GLenum>
	{
	public:
		ShaderStage(ShaderType type, const std::string& source);
		~ShaderStage();
	};

	class TextureBase;
	class Material;
	class ShaderProgram : public HandleBase<GLenum>, public Resource
	{
	public:

		class BufferInfo
		{
			int m_pos;
			std::string m_name;
			BufferTarget m_bufferTarget;

		public:
			inline int GetPos() const noexcept { return m_pos; }
			inline const std::string& GetName() const noexcept { return m_name; }
			inline BufferTarget GetBufferTarget() const noexcept { return m_bufferTarget; }

			BufferInfo(int pos, const std::string& name, BufferTarget bufferTarget)
			{
				m_pos = pos;
				m_name = name;
				m_bufferTarget = bufferTarget;
			}
		};

		static bool IsSampler(UniformType type);
		static bool IsImage(UniformType type);

		class UniformInfo
		{
			int m_pos;
			std::string m_name;
			UniformType m_type;
			BufferInfo* m_buffer;

		public:
			inline int GetPos() const noexcept { return m_pos; }
			inline const std::string& GetName() const noexcept { return m_name; }
			inline UniformType GetType() const noexcept { return m_type; }
			inline const BufferInfo* GetBuffer() const noexcept { return m_buffer; }

			bool IsSampler() const noexcept { return ShaderProgram::IsSampler(m_type); }
			bool IsImage() const noexcept { return ShaderProgram::IsImage(m_type); }

			UniformInfo(int pos, const std::string& name, UniformType type, BufferInfo* buffer)
			{
				m_pos = pos;
				m_name = name;
				m_type = type;
				m_buffer = buffer;
			}
		};

	private:
		std::vector<BufferInfo> m_buffers;
		std::vector<UniformInfo> m_uniforms;

		int GetUniformLocation(const string& name) const noexcept;

	public:
		ShaderProgram();

		inline void AttachStage(ShaderStage* stage) { glAttachShader(m_handle, stage->GetHandle()); GLErr(); }
		inline void DetachStage(ShaderStage* stage) { glDetachShader(m_handle, stage->GetHandle()); GLErr(); }

		inline void Bind() { glUseProgram(m_handle); GLErr(); }
		inline void Unbind() { glUseProgram(0); GLErr(); }

		void Link();

		template<typename T> void Set(const string& name, const T& value);
		void SetTexture(const string& name, TextureBase* value);
		void SetBuffer(const string& name, BufferBase* value);

		inline void bDispatchCompute(const ivec3& groupNum) const { glDispatchCompute(groupNum.x, groupNum.y, groupNum.z); GLErr(); }

		Material* CreateMaterial() const;

		~ShaderProgram();
	};

	ShaderProgram* CreateShaderProgram(const string& vertex, const string& fragment);
	ShaderProgram* CreateShaderProgram(const string& compute);

	class ShaderLoader : public ResourceLoader
	{
		static map<string, ShaderType> m_shaderTypeMap;

	public:

		Resource * Load(const tinyxml2::XMLElement* reader) const override;

	};
//endregion

//region Query

	class Query :public HandleBase<GLenum>
	{
		Entity(Query)
	private:
		QueryType m_type;
	public:
		Query(QueryType type);

		void Begin() { glBeginQuery(static_cast<GLenum>(m_type), m_handle); GLErr(); }
		void End() { glEndQuery(static_cast<GLenum>(m_type));  GLErr(); }

		inline int GetResultI() const { int res; glGetQueryObjectiv(m_handle, GL_QUERY_RESULT, &res); GLErr(); return res; }
		inline unsigned int GetResultU() const { unsigned int res; glGetQueryObjectuiv(m_handle, GL_QUERY_RESULT, &res); GLErr(); return res; }
		inline long long GetResultI64() const { long long res; glGetQueryObjecti64v(m_handle, GL_QUERY_RESULT, &res); GLErr(); return res; }
		inline unsigned long long GetResultU64() const { unsigned long long res; glGetQueryObjectui64v(m_handle, GL_QUERY_RESULT, &res); GLErr(); return res; }

		~Query();
	};
//endregion

//region GPUStopwatch

	template<int SampleCount> class GPUStopwatch
	{
		Entity(GPUStopwatch)
	private:
		unique_ptr<Query> m_query;
		deque<double> m_samples;
		double m_time;
	public:
		GPUStopwatch() { m_query = make_unique<Query>(QueryType::TimeElapsed); m_time = 0; }
		inline double GetTime() const noexcept { return m_time; }
		inline void Start() { m_query->Begin(); }
		void Stop()
		{
			m_query->End();
			m_samples.push_back(m_query->GetResultU64() / 1000000000.0);
			if (m_samples.size() > SampleCount)
				m_samples.pop_front();
			m_time = 0;
			for (double t : m_samples)
				m_time += t;
			m_time /= m_samples.size();
		}
	};
//endregion

//region GUIHelper

	template<typename T> void PropertyGUI(const string& name, T* value)
	{
		ImGui::TextDisabled(name.c_str());
	}
//endregion

//region Texture

	class TextureBase :public HandleBase<GLenum>, public Resource
	{
		TextureType m_type;
		TextureFormat m_format;
		bool m_mipmaps;
		int m_bindLevel;
		bool m_bindLayered;
		int m_bindLayer;
		AccessMode m_bindAccess;

	protected:
		TextureBase(TextureType type, TextureFormat format, bool mipmaps);
	public:
		inline void Bind() const { glBindTexture(static_cast<GLenum>(m_type), m_handle); GLErr(); }
		inline void Unbind() const { glBindTexture(static_cast<GLenum>(m_type), 0); GLErr(); }

		inline TextureType GetType() const noexcept { return m_type; }
		inline TextureFormat GetFormat() const noexcept { return m_format; }
		bool IsInteger() const noexcept;
		inline bool HasMipmaps() const noexcept { return m_mipmaps; }
		inline int GetBindLevel() const noexcept { return m_bindLevel; }
		inline bool IsBindLayered() const noexcept { return m_bindLayered; }
		inline int GetBindLayer() const noexcept { return m_bindLayer; }
		inline AccessMode GetBindAccessMode() const noexcept { return m_bindAccess; }
		inline void SetBindLevel(int value) noexcept { m_bindLevel = value; }
		inline void SetBindLayered(bool value) noexcept { m_bindLayered = value; }
		inline void SetBindLayer(int value) noexcept { m_bindLayer = value; }
		inline void SetBindAccessMode(AccessMode value) noexcept { m_bindAccess = value; }

		void BindImage(int unitNum) const;

		TextureDepthStencilMode bGetDepthStencilMode()const;
		void bSetDepthStencilMode(TextureDepthStencilMode value);
		vec4 bGetBorderColor()const;
		void bSetBorderColor(vec4 value);
		DepthFuncMode bGetCompareFunc()const;
		void bSetCompareFunc(DepthFuncMode value);
		TextureCompareMode bGetCompareMode()const;
		void bSetCompareMode(TextureCompareMode value);
		TextureFilterMode bGetMinFilter()const;
		void bSetMinFilter(TextureFilterMode value);
		TextureFilterMode bGetMagFilter()const;
		void bSetMagFilter(TextureFilterMode value);
		int bGetBaseLevel()const;
		void bSetBaseLevel(int value);
		int bGetMaxLevel()const;
		void bSetMaxLevel(int value);
		float bGetLODBias()const;
		void bSetLODBias(float value);
		float bGetMinLOD()const;
		void bSetMinLOD(float value);
		float bGetMaxLOD()const;
		void bSetMaxLOD(float value);
		ChannelSwizzleValue bGetSwizzleR()const;
		void bSetSwizzleR(ChannelSwizzleValue value);
		ChannelSwizzleValue bGetSwizzleG()const;
		void bSetSwizzleG(ChannelSwizzleValue value);
		ChannelSwizzleValue bGetSwizzleB()const;
		void bSetSwizzleB(ChannelSwizzleValue value);
		ChannelSwizzleValue bGetSwizzleA()const;
		void bSetSwizzleA(ChannelSwizzleValue value);
		TextureWrapMode bGetWrapS()const;
		void bSetWrapS(TextureWrapMode value);
		TextureWrapMode bGetWrapT()const;
		void bSetWrapT(TextureWrapMode value);
		TextureWrapMode bGetWrapR()const;
		void bSetWrapR(TextureWrapMode value);
		void bSetWrap(TextureWrapMode value);

		void bGenerateMipMap();

		void SetActive(int num);
		~TextureBase();
	};

	class Texture2D : public TextureBase
	{
		ivec2 m_size;

		void Allocate(const ivec2& newSize);

	public:
		Texture2D(const ivec2& size, bool mipmaps, TextureFormat format);

		inline const ivec2& GetSize() const noexcept { return m_size; }

		template<typename T> void bSetData(int level, const ivec2& offset, const ivec2& size, const T* data);
		template<typename T> void bGetData(int level, T* data) const;

	};

	class Texture2DLoader : public ResourceLoader
	{

	public:

		Resource * Load(const tinyxml2::XMLElement* reader) const override;

	};
//endregion

//region Material

	class Material :Resource
	{
		class UniformBase
		{
		protected:
			UniformBase() = default;
		public:
			virtual void Apply(const string& name, ShaderProgram* sh) const = 0;
			virtual string GetTypeName() const noexcept = 0;
			virtual string PrintValue() const noexcept = 0;
			virtual void GUI(const string& name) = 0;
			virtual ~UniformBase() = default;
			virtual void TryCopyFrom(UniformBase* other) = 0;
		};
		template<typename T> class UniformValue : public UniformBase
		{
			T m_value;
		public:
			inline const T& GetValue() const noexcept { return m_value; }
			inline void SetValue(const T& value) noexcept { m_value = value; }

			void Apply(const string& name, ShaderProgram* sh) const override { sh->Set(name, m_value); }
			string GetTypeName() const noexcept override { return typeid(T).name(); };
			string PrintValue() const noexcept override { return to_string(m_value); };
			UniformValue() = default;
			UniformValue(const T& val) { m_value = val; }
			void GUI(const string& name) override { PropertyGUI<T>(name, &m_value); }
			void TryCopyFrom(UniformBase* other) override
			{
				UniformValue<T>* cother = dynamic_cast<UniformValue<T>*>(other);
				if (cother)
					m_value = cother->m_value;
			}
		};
		class UniformTexture : public UniformBase
		{
			TextureBase* m_value;
		public:
			inline TextureBase* GetValue() const noexcept { return m_value; }
			inline void SetValue(TextureBase* value) noexcept { m_value = value; }
			void Apply(const string& name, ShaderProgram* sh) const override { sh->SetTexture(name, m_value); }
			string GetTypeName() const noexcept override { return typeid(m_value).name(); };
			string PrintValue() const noexcept override { return m_value->GetName(); };
			UniformTexture() = default;
			UniformTexture(TextureBase* val) { m_value = val; }
			void GUI(const string& name) override { PropertyGUI<TextureBase*>(name, &m_value); }
			void TryCopyFrom(UniformBase* other) override
			{
				UniformTexture* cother = dynamic_cast<UniformTexture*>(other);
				if (cother)
					m_value = cother->m_value;
			}
		};
		class UniformBuffer : public UniformBase
		{
			BufferBase* m_value;
		public:
			inline BufferBase* GetValue() const noexcept { return m_value; }
			inline void SetValue(BufferBase* value) noexcept { m_value = value; }
			void Apply(const string& name, ShaderProgram* sh) const override { sh->SetBuffer(name, m_value); }
			string GetTypeName() const noexcept override { return typeid(m_value).name(); };
			string PrintValue() const noexcept override { return "Buffer"; };
			UniformBuffer() = default;
			UniformBuffer(BufferBase* val) { m_value = val; }
			void GUI(const string& name) override { PropertyGUI<BufferBase*>(name, &m_value); }
			void TryCopyFrom(UniformBase* other) override
			{
				UniformBuffer* cother = dynamic_cast<UniformBuffer*>(other);
				if (cother)
					m_value = cother->m_value;
			}
		};
		mutable map<string, unique_ptr<UniformBase>> m_data;
	public:
		void AddUniform(const string& name, UniformType type);

		template <typename T> inline void Set(const string& name, const T& value) noexcept { m_data[name] = unique_ptr<UniformBase>(new UniformValue<T>(value)); }
		template <typename T> inline T Get(const string& name) const { return dynamic_cast<UniformValue<T>*>(m_data[name].get())->GetValue(); }
		inline void SetTexture(const string& name, TextureBase* value) noexcept { m_data[name] = unique_ptr<UniformBase>(new UniformTexture(value)); }
		inline TextureBase* GetTexture(const string& name) const { return dynamic_cast<UniformTexture*>(m_data[name].get())->GetValue(); }
		inline void SetBuffer(const string& name, BufferBase* value) noexcept { m_data[name] = unique_ptr<UniformBase>(new UniformBuffer(value)); }
		inline BufferBase* GetBuffer(const string& name) const { return dynamic_cast<UniformBuffer*>(m_data[name].get())->GetValue(); }

		void Apply(ShaderProgram* sh);
		void GUI();
		void FetchValues(Material* mat);
	};

//endregion

//region FrameBuffer

	class RenderBuffer : public HandleBase<GLenum>
	{
		Entity(RenderBuffer)
	private:
		RenderBufferFormat m_format;
	public:
		RenderBuffer(RenderBufferFormat format, const ivec2& size);

		inline void Bind() const { glBindRenderbuffer(GL_RENDERBUFFER, m_handle); GLErr(); }
		inline void Unbind() const { glBindRenderbuffer(GL_RENDERBUFFER, 0); GLErr(); }

		inline RenderBufferFormat GetFormat() const noexcept { return m_format; }

		~RenderBuffer();
	};

	class Framebuffer : public HandleBase<GLenum>
	{
		Entity(Framebuffer)
	public:
		Framebuffer();

		void bAttachColorBuffer(int num, Texture2D* texture, int level) { glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + num, texture->GetHandle(), level); GLErr(); }
		void bAttachColorBuffer(int num, RenderBuffer* buffer) { glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + num, GL_RENDERBUFFER, buffer->GetHandle()); GLErr(); }
		void bAttachDepthBuffer(Texture2D* texture, int level) { glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture->GetHandle(), level); GLErr(); }
		void bAttachDepthBuffer(RenderBuffer* buffer) { glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, buffer->GetHandle()); GLErr(); }
		void bAttachStencilBuffer(Texture2D* texture, int level) { glFramebufferTexture(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, texture->GetHandle(), level); GLErr(); }
		void bAttachStencilBuffer(RenderBuffer* buffer) { glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, buffer->GetHandle()); GLErr(); }

		void bDrawBuffers(const vector<int>& attachments);
		void bDrawBuffers(const initializer_list<int>& attachments);

		FrameBufferCompletion bGetCompletionStatus() const;
		inline bool bIsComplete() const { return bGetCompletionStatus() == FrameBufferCompletion::Complete; }

		void Copy(Framebuffer* destination, ivec2 uv0, ivec2 uv1, BufferBit buffers, int sourceColorAttachment, int destColorAttachment) const;
		void Copy(ivec2 uv0, ivec2 uv1, BufferBit buffers, int sourceColorAttachment, DefaultColorBuffers destColorBuf) const;

		inline void Bind() const { glBindFramebuffer(GL_FRAMEBUFFER, m_handle); GLErr(); }
		inline void Unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); GLErr(); }

		~Framebuffer();
	};
//endregion

//region RenderTarget

	class RenderTarget
	{
		unique_ptr<Framebuffer> m_frameBuf;
		vector<unique_ptr<Texture2D>> m_colorBufs;
		unique_ptr<RenderBuffer> m_depthBuffer;
		unique_ptr<RenderBuffer> m_stencilBuffer;
	public:
		RenderTarget(const ivec2& size, int colorBufNum, TextureFormat format, bool mipmaps = false, bool depthBuffer = false, bool stencilBuffer = false);
		inline Texture2D* operator[] (int num) { return m_colorBufs[num].get(); }
		inline const Texture2D* operator[] (int num) const { return m_colorBufs[num].get(); }
		inline const Framebuffer* GetFrameBuffer() const noexcept { return m_frameBuf.get(); }
		inline Framebuffer* GetFrameBuffer() noexcept { return m_frameBuf.get(); }
		inline void Bind() const { m_frameBuf->Bind(); }
		inline void Unbind() const { m_frameBuf->Unbind(); }
	};
//endregion

//region Math3D

	trquat RotationBetweenVectors(const trvec3& from, const trvec3& to);
	trscalar AngleBetween(const trvec3& a, const trvec3& b, const trvec3& n);
	trquat LookAtRotation(const trvec3& dir, const trvec3& up);
	trvec3 ProjectOnPlane(const trvec3& vec, const trvec3& up);
//endregion

//region Transform

	class Transform : public Object
	{
		trvec3 m_localPos;
		trquat m_localRot;
		trmat4 m_matrix;
		trmat4 m_matrixInv;
		trmat4 m_localMatrix;
		void UpdateMatrix();
		void UpdateMatrixRecursive();

		Transform* m_parent;
		list<unique_ptr<Transform>> m_childs;
	protected:
		virtual void OnTransformUpdate() {};
	public:
		Transform();

		inline void SetLocalPos(const trvec3& value) noexcept { m_localPos = value; UpdateMatrix(); }
		inline void SetLocalRot(const trquat& value) noexcept { m_localRot = value; UpdateMatrix(); }
		inline void SetLocalEulerAngles(const trvec3& value) noexcept { m_localRot = trquat(value); m_localRot = normalize(m_localRot); UpdateMatrix(); }

		inline const trvec3& GetLocalPos() const noexcept { return m_localPos; }
		inline const trquat& GetLocalRot() const noexcept { return m_localRot; }
		inline const trvec3 GetLocalEulerAngles() const noexcept { return eulerAngles(m_localRot); }
		inline const trmat4& GetMatrix() const noexcept { return m_matrix; }

		inline trvec3 Forward() const { return m_matrix * vec4(0, 0, -1, 0); }
		inline trvec3 Right() const { return m_matrix * vec4(1, 0, 0, 0); }
		inline trvec3 Up() const { return m_matrix * vec4(0, 1, 0, 0); }

		inline trvec3 ToWorld(const trvec3& pos) const noexcept { return m_matrix * trvec4(pos, 1); }
		inline trvec3 ToLocal(const trvec3& pos) const noexcept { return m_matrixInv * trvec4(pos, 1); }

		inline void LookAt(const trvec3& pos, const trvec3& up = { 0,1,0 }) noexcept { SetLocalRot(LookAtRotation(pos - m_localPos, up)); }

		Transform* CreateChild();
		void SetParent(Transform* parent);
	};
//endregion

//region Camera

	class Camera : public Transform
	{
		vec3 m_eulerAngles;
		ivec2 m_frameSize;
	protected:
		trmat4 m_viewMatrix;
		trmat4 m_projMatrix;
		trmat4 m_viewProjMatrix;
		virtual void OnFrameSizeChanged() {};
	protected:
		Camera(const ivec2& frameSize);
	public:
		void LookAround(const dvec2& delta, trscalar speed, trscalar xMin, trscalar xMax);
		inline void SetFrameSize(const ivec2& value) noexcept { m_frameSize = value; OnFrameSizeChanged(); }
		inline const ivec2& GetFrameSize() const noexcept { return m_frameSize; }
		inline const float GetAspectRatio() const noexcept { return m_frameSize.x / (float)m_frameSize.y; }
		inline const trmat4& GetViewMatrix() const noexcept { return m_viewMatrix; }
		inline const trmat4& GetProjMatrix() const noexcept { return m_projMatrix; }
		inline const trmat4& GetViewProjMatrix() const noexcept { return m_viewProjMatrix; }
	};
	class PerspectiveCamera : public Camera
	{
		trscalar m_fov;
		trscalar m_zNear;
		trscalar m_zFar;

		void OnTransformUpdate() override;
		void OnFrameSizeChanged() override;
		void UpdateProjection();
		void UpdateView();
	public:
		void SetFOV(trscalar value) noexcept { m_fov = value; UpdateProjection(); }
		inline trscalar GetFOV() const noexcept { return m_fov; }
		void SetZNear(trscalar value) noexcept { m_zNear = value; UpdateProjection(); }
		inline trscalar GetZNear() const noexcept { return m_zNear; }
		void SetZFar(trscalar value) noexcept { m_zFar = value; UpdateProjection(); }
		inline trscalar GetZFar() const noexcept { return m_zFar; }

		PerspectiveCamera(const ivec2& frameSize, trscalar fov, trscalar zNear, trscalar zFar);
	};
//endregion

//region Mesh

	class MeshBase : public Resource
	{
		unique_ptr<VertexArray> m_vao;
		int m_count;
	protected:
		void SetBuffer(int index, int vectorSize, const BufferBase* buffer, int offset = 0);
		inline void SetCount(int count) noexcept { m_count = count; }
		MeshBase(Topology topology);
	public:
		inline  VertexArray* GetVAO() const noexcept { return m_vao.get(); }
		inline void Draw() const { m_vao->Bind();  m_vao->bDraw(0, m_count); }
		inline void Draw(int instanceCount) const { m_vao->Bind();  m_vao->bDraw(0, m_count, instanceCount); }
	};

	class ModelMesh : public MeshBase
	{
		unique_ptr<FloatBuffer<vec3>> m_position;
		unique_ptr<FloatBuffer<vec3>> m_normals;
		unique_ptr<FloatBuffer<vec3>> m_tangents;
		unique_ptr<FloatBuffer<vec4>> m_color;
		array<unique_ptr<FloatBuffer<vec2>>, 2> m_uvs;
		unique_ptr<UnsignedIntBuffer<unsigned int>> m_indexes;
	public:
		ModelMesh() : MeshBase(Topology::Triangles) {}

		inline  FloatBuffer<vec3>* GetPositionBuffer() const noexcept { return m_position.get(); }
		inline  FloatBuffer<vec3>* GetNormalsBuffer() const noexcept { return m_normals.get(); }
		inline  FloatBuffer<vec3>* GetTangentsBuffer() const noexcept { return m_tangents.get(); }
		inline  FloatBuffer<vec4>* GetColorBuffer() const noexcept { return m_color.get(); }
		inline  FloatBuffer<vec2>* GetUVBuffer(int num) const { return m_uvs[num].get(); }
		inline constexpr int GetUVBufferCount() const noexcept { return m_uvs.size(); }

		inline void SetPositionBuffer(FloatBuffer<vec3>* buffer) { SetCount(buffer->GetSize() * 3); m_position.reset(buffer); SetBuffer(0, 3, buffer); }
		inline void SetNormalsBuffer(FloatBuffer<vec3>* buffer) { m_normals.reset(buffer); SetBuffer(1, 3, buffer); }
		inline void SetTangentsBuffer(FloatBuffer<vec3>* buffer) { m_tangents.reset(buffer); SetBuffer(2, 3, buffer); }
		inline void SetColorBuffer(FloatBuffer<vec4>* buffer) { m_color.reset(buffer); SetBuffer(3, 4, buffer); }
		inline void SetUVBuffer(int num, FloatBuffer<vec2>* buffer) { m_uvs[num].reset(buffer); SetBuffer(4 + num, 2, buffer); }
		void SetIndexBuffer(UnsignedIntBuffer<unsigned int>* buffer);
	};

	class ScreenQuadMesh : public MeshBase
	{
	public:
		struct Vertex
		{
			vec2 UV;
			vec3 NearPosition;
			vec3 FarPosition;
		};
	private:
		unique_ptr<FloatBuffer<Vertex>> m_vertexBuffer;
	public:
		ScreenQuadMesh();
	};

	class CoordBasisMesh : public MeshBase
	{
	public:
		struct Vertex
		{
			vec3 Position;
			vec3 Color;
		};
	private:
		unique_ptr<FloatBuffer<Vertex>> m_vertexBuffer;
	public:
		CoordBasisMesh();
	};

	class TerrainMesh : public MeshBase
	{
		unique_ptr<IntBuffer<ivec2>> m_coord;
		unique_ptr<FloatBuffer<vec2>> m_uv;
		unique_ptr<UnsignedIntBuffer<unsigned int>> m_indexes;
		int m_size;
	public:
		TerrainMesh(int size);
		inline int GetSize() const noexcept { return m_size; }
	};
//endregion

//region Terrain

	class Terrain : public Object
	{
		unique_ptr<TerrainMesh> m_plane;
		unique_ptr<Texture2D> m_heightmap;
		unique_ptr<Texture2D> m_normalmap;
		unique_ptr<IntBuffer<ivec2>> m_offsets;
		mutable vector<ivec2> m_currentChunks;
		ivec2 m_size;
		trvec3 m_offset;
		int m_chunkSize;
		int m_drawRange;
		ShaderProgram* m_heightGen;
		ShaderProgram* m_normals;
		ShaderProgram* m_terrainDraw;
	public:
		inline TerrainMesh* GetPlaneMesh() const noexcept { return m_plane.get(); }
		inline const ivec2& GetSize() const noexcept { return m_size; }
		inline int GetChunkSize() const noexcept { return m_chunkSize; }
		inline const trvec3& GetOffset() const noexcept { return m_offset; }

		void Generate();
		void Draw(Camera* cam) const;

		Terrain(const ivec2& size, int chunkSize);
	};
//endregion

//region DeferredPBRPipeline

	class DeferredPBRPipeline
	{
		unique_ptr<RenderTarget> m_GBufFBO;
		unique_ptr<RenderTarget> m_LightsFBO;
		unique_ptr<RenderTarget> m_FinalFBO;
		Camera* m_cam;
		ShaderProgram* m_lightCompute;
		ShaderProgram* m_lightCompose;
		ScreenQuadMesh* m_screenQuad;
		Texture2D* m_brdflut;
	public:
		DeferredPBRPipeline(const ivec2& frameSize);

		inline RenderTarget* GetGBuffer() const noexcept { return m_GBufFBO.get(); }
		inline RenderTarget* GetLightsBuffer() const noexcept { return m_LightsFBO.get(); }
		inline RenderTarget* GetFinalBuffer() const noexcept { return m_FinalFBO.get(); }
		inline Camera* GetFrameCamera() const noexcept { return m_cam; }
		inline void SetFrameCamera(Camera* cam) noexcept { m_cam = cam; }

		void Resize(const ivec2& size);
		inline void Bind() const { m_GBufFBO->Bind(); }
		void Unbind();
	};
//endregion

}

#endif