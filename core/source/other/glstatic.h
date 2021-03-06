#pragma once
namespace ogele {

    void ClearColor(const glm::vec4 &color);
    void Clear(BufferBit buffers);
    void Viewport(const glm::ivec2 &offset, const glm::ivec2 &size);
    void Enable(Feature feature);
    void Disable(Feature feature);
	void BlendFunc(BlendFactor src, BlendFactor dst);
    void Barrier(MemoryBarriers barriers);
    void SetPolygonMode(PolygonFace face, PolygonMode mode);
    void Finish();
}