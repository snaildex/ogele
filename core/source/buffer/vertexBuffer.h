#pragma once
namespace ogele {
    class BufferBase : public HandleBase<GLenum> {
        Entity(BufferBase)

        BufferDataType m_dataType;
        int m_size;
        size_t m_elemSize;
        size_t m_structSize;

    protected:
        BufferBase(BufferDataType dataType, size_t structSize, int size);

    public:

         BufferDataType GetDataType() const noexcept { return m_dataType; }

         void Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_handle); GLErr(); }

         void Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); GLErr(); }

         void Bind(BufferTarget usage) const { glBindBuffer(static_cast<GLenum>(usage), m_handle); GLErr(); }

         void Unbind(BufferTarget usage) const { glBindBuffer(static_cast<GLenum>(usage), 0); GLErr(); }

         void BindBase(BufferTarget usage, int bindingIndex) {
            glBindBufferBase(static_cast<GLenum>(usage), bindingIndex, m_handle); GLErr();
        }

        constexpr  size_t GetElementSize() const noexcept { return m_elemSize; }

        constexpr  size_t GetStructSize() const noexcept { return m_structSize; }

         int GetSize() const noexcept { return m_size; }

        ~BufferBase();

    };

    template<typename T>
    class Buffer : public BufferBase {
        BufferFlags m_flags;

    protected:
        Buffer(BufferDataType dataType, BufferFlags flags, int size, const T *data = nullptr) : BufferBase(dataType,
                                                                                                           sizeof(T),
                                                                                                           size) {
            m_flags = flags;
            Bind();
            glBufferStorage(GL_ARRAY_BUFFER, sizeof(T) * size, data, static_cast<GLbitfield>(flags));
            GLErr();
            Unbind();
        }

    public:

         T *bMap(AccessMode accessMode) {
            T *r = (T *) glMapBuffer(GL_ARRAY_BUFFER, static_cast<GLbitfield>(accessMode));  GLErr();
            return r;
        }

         void bUnmap() { glUnmapBuffer(GL_ARRAY_BUFFER); GLErr(); }

         T *bMapRange(int first, int count, MappingOptionsFlags options) {
            T *r = (T *) glMapBufferRange(GL_ARRAY_BUFFER, sizeof(T) * first, sizeof(T) * count,
                                          static_cast<GLbitfield>(m_flags) |
                                          static_cast<GLbitfield>(options));  GLErr();
            return r;
        }

         void bFlushRange(int first, int count) {
            glFlushMappedBufferRange(GL_ARRAY_BUFFER, sizeof(T) * first, sizeof(T) * count); GLErr();
        }

         void bSetData(const T *dataPtr, int first, int count) {
            glBufferSubData(GL_ARRAY_BUFFER, sizeof(T) * first, sizeof(T) * count, dataPtr); GLErr();
        }

         void bGetData(T *dataPtr, int first, int count) const {
            glGetBufferSubData(GL_ARRAY_BUFFER, sizeof(T) * first, sizeof(T) * count, dataPtr); GLErr();
        }

         void bClear(TextureFormat internalFormat, PixelDataFormat format, T value) {
            glClearBufferData(GL_ARRAY_BUFFER, static_cast<GLenum>(internalFormat), static_cast<GLenum>(format),
                              static_cast<GLenum>(GetDataType()), &value);
            GLErr();
        }

         void bClearRange(TextureFormat internalFormat, int offset, int size, PixelDataFormat format, T value) {
            glClearBufferSubData(GL_ARRAY_BUFFER, static_cast<GLenum>(internalFormat), offset * sizeof(T),
                                 size * sizeof(T), static_cast<GLenum>(format), static_cast<GLenum>(GetDataType()),
                                 &value);
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

    DefBuffer(HalfFloat)

    DefBuffer(Float)

    DefBuffer(Fixed)

    DefBuffer(Double)
}