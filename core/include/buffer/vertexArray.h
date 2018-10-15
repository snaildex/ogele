#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../buffer/vertexBuffer.h"
#include "../other/glerror.h"
#include "../helpers/helpers.h"
#include "../other/enums.h"

namespace ogele
{
    class VertexArray : HandleBase<GLuint>
    {
        Entity(VertexArray)
        Topology m_topology;
        bool m_indexed;
        int m_patchVerticies;
        glm::vec4 m_defOuterTess;
        glm::vec2 m_defInnerTess;

    public:

        VertexArray(Topology topology);

        inline Topology GetTopology() const noexcept { return m_topology; }
        inline int GetPatchVertCount() const noexcept { return m_patchVerticies; }
        inline void SetPatchVertCount(int value) noexcept { m_patchVerticies=value; }
        inline const glm::vec4& GetDefOuterTessFactor() const noexcept { return m_defOuterTess; }
        inline void SetDefOuterTessFactor(const glm::vec4& value) noexcept { m_defOuterTess=value; }
        inline const glm::vec2& GetDefInnerTessFactor() const noexcept { return m_defInnerTess; }
        inline void SetDefInnerTessFactor(const glm::vec2& value) noexcept { m_defInnerTess=value; }

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
}
