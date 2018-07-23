//
// Created by ??????? on 19.07.2018.
//

#ifndef OGELE_GLSTATIC_H
#define OGELE_GLSTATIC_H

namespace ogele {

    void ClearColor(const glm::vec4& color);
    void Clear(BufferBit buffers);
    void Viewport(const glm::ivec2& offset, const glm::ivec2& size);
    void Enable(Feature feature);
    void Disable(Feature feature);
    void Barrier(MemoryBarriers barriers);

}
#endif //OGELE_GLSTATIC_H
