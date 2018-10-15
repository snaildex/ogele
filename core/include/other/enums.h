#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../helpers/helpers.h"

namespace ogele {
    /**
    *  Specifies the target to which the buffer object is bound.
    */
    enum class BufferTarget : GLenum {
        Array = GL_ARRAY_BUFFER, /**< Vertex attributes */
        AtomicCounter = GL_ATOMIC_COUNTER_BUFFER, /**< Atomic counter storage */
        CopyRead = GL_COPY_READ_BUFFER, /**< Buffer copy source */
        CopyWrite = GL_COPY_WRITE_BUFFER, /**< Buffer copy destination */
        DispatchIndirect = GL_DISPATCH_INDIRECT_BUFFER, /** Indirect compute dispatch commands */
        DrawIndirect = GL_DRAW_INDIRECT_BUFFER, /**< Indirect command arguments */
        ElementArray = GL_ELEMENT_ARRAY_BUFFER, /**< Vertex array indices */
        PixelPack = GL_PIXEL_PACK_BUFFER, /**< Pixel read target */
        PixelUnpack = GL_PIXEL_UNPACK_BUFFER, /**< Texture data source */
        Query = GL_QUERY_BUFFER, /**< Query result buffer */
        ShaderStorage = GL_SHADER_STORAGE_BUFFER, /**< Read-write storage for shaders */
        Texture = GL_TEXTURE_BUFFER, /**< Texture data buffer */
        TransformFeedback = GL_TRANSFORM_FEEDBACK_BUFFER, /**< Transform feedback buffer */
        Uniform = GL_UNIFORM_BUFFER /**< Uniform block storage */
    };
    /**
     *  Hint to the GL implementation as to how a buffer object's data store will be accessed.
     */
    enum class MutableBufferUsage : GLenum {
        StreamDraw = GL_STREAM_DRAW,/**< The data store contents will be modified once and used at most a few times.
 * The data store contents are modified by the application, and used as the source for GL drawing and image specification commands. */
        StreamRead = GL_STREAM_READ,/**< The data store contents will be modified once and used at most a few times.
 * The data store contents are modified by reading data from the GL, and used to return that data when queried by the application.*/
        StreamCopy = GL_STREAM_COPY,/**< The data store contents will be modified once and used at most a few times.
 * The data store contents are modified by reading data from the GL, and used as the source for GL drawing and image specification commands.*/
        StaticDraw = GL_STATIC_DRAW,/**< The data store contents will be modified once and used many times.
 * The data store contents are modified by the application, and used as the source for GL drawing and image specification commands.*/
        StaticRead = GL_STATIC_READ,/**< The data store contents will be modified once and used many times.
 * The data store contents are modified by reading data from the GL, and used to return that data when queried by the application.*/
        StaticCopy = GL_STATIC_COPY,/**< The data store contents will be modified once and used many times.
 * The data store contents are modified by reading data from the GL, and used as the source for GL drawing and image specification commands.*/
        DynamicDraw = GL_DYNAMIC_DRAW,/**< The data store contents will be modified repeatedly and used many times.
 * The data store contents are modified by the application, and used as the source for GL drawing and image specification commands.*/
        DynamicRead = GL_DYNAMIC_READ,/**< The data store contents will be modified repeatedly and used many times.
 * The data store contents are modified by reading data from the GL, and used to return that data when queried by the application.*/
        DynamicCopy = GL_DYNAMIC_COPY/**< The data store contents will be modified repeatedly and used many times.
 * The data store contents are modified by reading data from the GL, and used as the source for GL drawing and image specification commands.*/
    };
    /**
     * Intended usage of the buffer's data store.
     */
    enum class BufferFlags : GLenum {
        None = 0,/**< None */
        MapRead = GL_MAP_READ_BIT,/**<
 * The data store may be mapped by the client for read access and a pointer in the client's address space obtained that may be read from. */
        MapWrite = GL_MAP_WRITE_BIT,/**<
 * The data store may be mapped by the client for write access and a pointer in the client's address space obtained that may be written through. */
        MapPersistent = GL_MAP_PERSISTENT_BIT,/**<
 * The client may request that the server read from or write to the buffer while it is mapped.
 * The client's pointer to the data store remains valid so long as the data store is mapped, even during execution of drawing or dispatch commands.*/
        MapCoherent = GL_MAP_COHERENT_BIT,/**<
 * Shared access to buffers that are simultaneously mapped for client access and are used by the server will be coherent, so long as that mapping is performed using glMapBufferRange.
 * That is, data written to the store by either the client or server will be immediately visible to the other with no further action taken by the application. In particular,
* If MapCoherent is not set and the client performs a write followed by a call to the glMemoryBarrier command with the ClientMappedBuffer set,
* then in subsequent commands the server will see the writes.
* If MapCoherent is set and the client performs a write, then in subsequent commands the server will see the writes.
* If MapCoherent is not set and the server performs a write,
 * the application must call MemoryBarrier with the ClientMappedBuffer set and then call glFenceSync with GL_SYNC_GPU_COMMANDS_COMPLETE (or glFinish).
 * Then the CPU will see the writes after the sync is complete.
* If MapCoherent is set and the server does a write, the app must call glFenceSync with GL_SYNC_GPU_COMMANDS_COMPLETE (or glFinish). Then the CPU will see the writes after the sync is complete.*/
        DynamicStorage = GL_DYNAMIC_STORAGE_BIT,/**<
 * The contents of the data store may be updated after creation through calls to Buffer::bSetData.
 * If this bit is not set, the buffer content may not be directly updated by the client.
 * The data argument may be used to specify the initial content of the buffer's data store regardless of the presence of the DynamicStorage.
 * Regardless of the presence of this bit, buffers may always be updated with server-side copying and clearing. */
        ClientStorage = GL_CLIENT_STORAGE_BIT/**<
 * When all other criteria for the buffer storage allocation are met,
 * this bit may be used by an implementation to determine whether to use storage that is local to the server or to the client to serve as the backing store for the buffer.*/
    };

    EnumFlags(BufferFlags)

    enum class MappingOptionsFlags : GLenum {
        InvalidateRange = GL_MAP_INVALIDATE_RANGE_BIT,
        InvalidateBuffer = GL_MAP_INVALIDATE_BUFFER_BIT,
        FlushExplicit = GL_MAP_FLUSH_EXPLICIT_BIT,
        Unsynchronized = GL_MAP_UNSYNCHRONIZED_BIT
    };

    EnumFlags(MappingOptionsFlags)

    enum class BufferDataType : GLenum {
        Byte = GL_BYTE,
        UnsignedByte = GL_UNSIGNED_BYTE,
        Short = GL_SHORT,
        UnsignedShort = GL_UNSIGNED_SHORT,
        Int = GL_INT,
        UnsignedInt = GL_UNSIGNED_INT,
        HalfFloat = GL_HALF_FLOAT,
        Float = GL_FLOAT,
        Fixed = GL_FIXED,
        Double = GL_DOUBLE,
    };
    enum class Topology : GLenum {
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
    enum class Key : int {
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
    enum class KeyState : int {
        Press = GLFW_PRESS,
        Repeat = GLFW_REPEAT,
        Release = GLFW_RELEASE
    };
    enum class KeyModifiers : int {
        Alt = GLFW_MOD_ALT,
        Control = GLFW_MOD_CONTROL,
        Shift = GLFW_MOD_SHIFT,
        Super = GLFW_MOD_SUPER
    };

    EnumFlags(KeyModifiers)

    enum class MouseButton : int {
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
    enum class BufferBit : GLbitfield {
        Color = GL_COLOR_BUFFER_BIT,
        Depth = GL_DEPTH_BUFFER_BIT,
        Accumulation = GL_ACCUM_BUFFER_BIT,
        Stencil = GL_STENCIL_BUFFER_BIT
    };

    EnumFlags(BufferBit)

    enum class Feature : GLenum {
        CullFace = GL_CULL_FACE,
        DepthTest = GL_DEPTH_TEST,
        SeamlessCubemap = GL_TEXTURE_CUBE_MAP_SEAMLESS,
        FrameBufferSRGB = GL_FRAMEBUFFER_SRGB,
        PrimitiveRestart = GL_PRIMITIVE_RESTART,
        Blend = GL_BLEND
    };
    enum class PolygonMode : GLenum{
        Point=GL_POINT,
        Line=GL_LINE,
        Fill=GL_FILL
    };
    enum class PolygonFace : GLenum{
        Front=GL_FRONT ,
        Back=GL_BACK ,
        FrontAndBack=GL_FRONT_AND_BACK
    };
    enum class DepthFuncMode : GLenum {
        Less = GL_LESS,
        Equal = GL_EQUAL,
        LEqual = GL_LEQUAL,
        Greater = GL_GREATER,
        NotEqual = GL_NOTEQUAL,
        GEqual = GL_GEQUAL,
        Always = GL_ALWAYS
    };
    enum class MemoryBarriers : GLbitfield {
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

    enum class BlendFactor : GLenum {
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
    enum class ShaderType : GLenum {
        Vertex = GL_VERTEX_SHADER,
        Fragment = GL_FRAGMENT_SHADER,
        Geometry = GL_GEOMETRY_SHADER,
        Compute = GL_COMPUTE_SHADER,
        TessControl = GL_TESS_CONTROL_SHADER,
        TessEvaluation = GL_TESS_EVALUATION_SHADER
    };
    enum class UniformType : GLenum {
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
    enum class TextureType : GLenum {
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
    enum class TextureParameter : GLenum {
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
    enum class AccessMode : GLenum {
        ReadOnly = GL_READ_ONLY,
        WriteOnly = GL_WRITE_ONLY,
        ReadWrite = GL_READ_WRITE
    };
    enum class TextureDepthStencilMode : GLenum {
        DepthComponent = GL_DEPTH_COMPONENT
    };
    enum class TextureCompareMode : GLenum {
        CompareRefToTexture = GL_COMPARE_REF_TO_TEXTURE,
        None = GL_NONE
    };
    enum class TextureFilterMode : GLenum {
        Nearest = GL_NEAREST,
        Linear = GL_LINEAR,
        NearestMipMapNearest = GL_NEAREST_MIPMAP_NEAREST,
        NearestMipMapLinear = GL_NEAREST_MIPMAP_LINEAR,
        LinearMipMapNearest = GL_LINEAR_MIPMAP_NEAREST,
        LinearMipMapLinear = GL_LINEAR_MIPMAP_LINEAR
    };
    enum class ChannelSwizzleValue : GLenum {
        Red = GL_RED,
        Green = GL_GREEN,
        Blue = GL_BLUE,
        Alpha = GL_ALPHA,
        Zero = GL_ZERO,
        One = GL_ONE
    };
    enum class TextureWrapMode : GLenum {
        ClampToEdge = GL_CLAMP_TO_EDGE,
        ClampToBorder = GL_CLAMP_TO_BORDER,
        MirroredRepeat = GL_MIRRORED_REPEAT,
        Repeat = GL_REPEAT,
        MirrorClampToEdge = GL_MIRROR_CLAMP_TO_EDGE
    };
    enum class TextureFormat : GLenum {
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
    enum class PixelDataFormat : GLenum {
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
    enum class PixelDataType : GLenum {
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
    enum class RenderBufferFormat : GLenum {
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
    enum class FrameBufferCompletion {
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
    enum class DefaultColorBuffers : GLenum {
        FrontLeft = GL_FRONT_LEFT,
        FrontRight = GL_FRONT_RIGHT,
        BackLeft = GL_BACK_LEFT,
        BackRight = GL_BACK_RIGHT,
        Front = GL_FRONT,
        Back = GL_BACK,
        Left = GL_LEFT,
        Right = GL_RIGHT
    };
    enum class QueryType : GLenum {
        TimeStamp = GL_TIMESTAMP,
        TimeElapsed = GL_TIME_ELAPSED,
        SamplesPassed = GL_SAMPLES_PASSED,
        AnySamplesPassed = GL_ANY_SAMPLES_PASSED,
        PrimitivesGenerated = GL_PRIMITIVES_GENERATED,
        TransformFeedbackPrimitivesWritten = GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN
    };

}