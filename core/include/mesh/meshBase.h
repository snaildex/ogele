#pragma once

#include <memory>

#include "../buffer/vertexBuffer.h"
#include "../buffer/vertexArray.h"
#include "../object/resource.h"
#include "../other/enums.h"

namespace ogele {
    class MeshBase : public Resource {
        std::unique_ptr<VertexArray> m_vao;
        int m_count;
    protected:
        void SetBuffer(int index, int vectorSize, const BufferBase *buffer, int offset = 0);

        inline void SetCount(int count) noexcept { m_count = count; }

        MeshBase(Topology topology);

    public:
        inline VertexArray *GetVAO() const noexcept { return m_vao.get(); }

        inline void Draw() const {
            m_vao->Bind();
            m_vao->bDraw(0, m_count);
        }

        inline void Draw(int instanceCount) const {
            m_vao->Bind();
            m_vao->bDraw(0, m_count, instanceCount);
        }
    };
}