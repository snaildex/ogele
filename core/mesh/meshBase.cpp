//
// Created by ??????? on 22.07.2018.
//

#include "../ogele.h"

using namespace std;
using namespace glm;
namespace fs = std::experimental::filesystem;

namespace ogele {
    MeshBase::MeshBase(Topology topology) {
        m_vao = make_unique<VertexArray>(topology);
    }

    void MeshBase::SetBuffer(int index, int vectorSize, const BufferBase *buffer, int offset) {
        m_vao->Bind();
        if (buffer == nullptr)
            m_vao->bDisableVertexAttribArray(index);
        else {
            m_vao->bEnableVertexAttribArray(index);
            m_vao->bAttachBuffer(index, vectorSize, buffer, offset);
        }
        m_vao->Unbind();
    }
}
