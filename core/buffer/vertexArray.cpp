//
// Created by ??????? on 21.07.2018.
//

#include "../ogele.h"

using namespace std;
using namespace glm;
namespace fs = std::experimental::filesystem;

namespace ogele {

    VertexArray::VertexArray(Topology topology) {
        m_topology = topology;
        m_indexed = false;
        glGenVertexArrays(1, &m_handle);
        GLErr();
    }

    void VertexArray::bAttachBuffer(int index, int vectorLength, const BufferBase *buffer, int offset) {
        bEnableVertexAttribArray(index);
        auto type = buffer->GetDataType();
        buffer->Bind(BufferTarget::Array);
        if (
                type == BufferDataType::Byte ||
                type == BufferDataType::Short ||
                type == BufferDataType::Int ||
                type == BufferDataType::UnsignedByte ||
                type == BufferDataType::UnsignedShort ||
                type == BufferDataType::UnsignedInt
                )
            glVertexAttribIPointer(index, vectorLength, static_cast<GLenum>(type), buffer->GetStructSize(),
                                   (const void *) (buffer->GetElementSize() * offset));
        else if (type == BufferDataType::Double)
            glVertexAttribLPointer(index, vectorLength, static_cast<GLenum>(type), buffer->GetStructSize(),
                                   (const void *) (buffer->GetElementSize() * offset));
        else
            glVertexAttribPointer(index, vectorLength, static_cast<GLenum>(type), false, buffer->GetStructSize(),
                                  (const void *) (buffer->GetElementSize() * offset));
        GLErr();
        buffer->Unbind(BufferTarget::Array);
    }

    void VertexArray::bAttachElementBuffer(const UnsignedIntBuffer<unsigned int> *buffer) {
        if (buffer == nullptr) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            GLErr();
            m_indexed = false;
        } else {
            buffer->Bind(BufferTarget::ElementArray);
            m_indexed = true;
        }
    }

    void VertexArray::bDraw(int first, int count) const {
        if (m_indexed) {
            glDrawElements(static_cast<GLenum>(m_topology), count, GL_UNSIGNED_INT,
                           (void *) (sizeof(unsigned int) * first));
            GLErr();
        } else {
            glDrawArrays(static_cast<GLenum>(m_topology), first, count);
            GLErr();

        }
    }

    void VertexArray::bDraw(int first, int count, int instanceCount) const {
        if (m_indexed) {
            glDrawElementsInstanced(static_cast<GLenum>(m_topology), count, GL_UNSIGNED_INT,
                                    (void *) (sizeof(unsigned int) * first), instanceCount);
            GLErr();
        } else {
            glDrawArraysInstanced(static_cast<GLenum>(m_topology), first, count, instanceCount);
            GLErr();
        }
    }

    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &m_handle);
        GLErr();
    }

}