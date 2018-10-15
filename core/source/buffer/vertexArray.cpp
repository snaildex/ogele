#include <buffer/vertexArray.h>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

namespace ogele {

    VertexArray::VertexArray(Topology topology) {
        m_topology = topology;
        m_indexed = false;
        m_patchVerticies=1;
        m_defOuterTess=vec4(1);
        m_defInnerTess=vec2(1);
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
        if (m_topology == Topology::Patches) {
            glPatchParameteri(GL_PATCH_VERTICES, m_patchVerticies);
            GLErr();
            glPatchParameterfv(GL_PATCH_DEFAULT_OUTER_LEVEL, value_ptr(m_defOuterTess));
            GLErr();
            glPatchParameterfv(GL_PATCH_DEFAULT_INNER_LEVEL, value_ptr(m_defInnerTess));
            GLErr();
        }
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
        if (m_topology == Topology::Patches) {
            glPatchParameteri(GL_PATCH_VERTICES, m_patchVerticies);
            GLErr();
            glPatchParameterfv(GL_PATCH_DEFAULT_OUTER_LEVEL, value_ptr(m_defOuterTess));
            GLErr();
            glPatchParameterfv(GL_PATCH_DEFAULT_INNER_LEVEL, value_ptr(m_defInnerTess));
            GLErr();
        }
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