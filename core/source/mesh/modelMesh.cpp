#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {
    void ModelMesh::SetIndexBuffer(UnsignedIntBuffer<unsigned int> *buffer) {
        SetCount(buffer->GetSize());
        m_indexes.reset(buffer);
        GetVAO()->Bind();
        GetVAO()->bAttachElementBuffer(buffer);
        GetVAO()->Unbind();
    }
}
