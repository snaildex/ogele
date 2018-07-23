//
// Created by ??????? on 22.07.2018.
//

#include "../ogele.h"

using namespace std;
using namespace glm;
namespace fs = std::experimental::filesystem;

namespace ogele {
    void ModelMesh::SetIndexBuffer(UnsignedIntBuffer<unsigned int> *buffer) {
        SetCount(buffer->GetSize());
        m_indexes.reset(buffer);
        GetVAO()->Bind();
        GetVAO()->bAttachElementBuffer(buffer);
        GetVAO()->Unbind();
    }
}
