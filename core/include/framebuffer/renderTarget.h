#pragma once

#include <memory>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../other/enums.h"
#include "../texture/texture2D/texture2D.h"
#include "framebuffer.h"

namespace ogele {

    class RenderTarget {
        std::unique_ptr<Framebuffer> m_frameBuf;
        std::vector<std::unique_ptr<Texture2D>> m_colorBufs;
        std::unique_ptr<Renderbuffer> m_depthBuffer;
        std::unique_ptr<Renderbuffer> m_stencilBuffer;
    public:
        RenderTarget(const glm::ivec2 &size, int colorBufNum, TextureFormat format, bool mipmaps = false,
                     bool depthBuffer = false, bool stencilBuffer = false);

        inline Texture2D *operator[](int num) { return m_colorBufs[num].get(); }

        inline const Texture2D *operator[](int num) const { return m_colorBufs[num].get(); }

        inline const Framebuffer *GetFrameBuffer() const noexcept { return m_frameBuf.get(); }

        inline Framebuffer *GetFrameBuffer() noexcept { return m_frameBuf.get(); }

        inline void Bind() const { m_frameBuf->Bind(); }

        inline void Unbind() const { m_frameBuf->Unbind(); }
    };
}