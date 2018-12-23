#pragma once
namespace ogele {

    class RenderTarget {
        std::unique_ptr<Framebuffer> m_frameBuf;
        std::vector<std::unique_ptr<Texture2D>> m_colorBufs;
        std::unique_ptr<Renderbuffer> m_depthBuffer;
        std::unique_ptr<Renderbuffer> m_stencilBuffer;
    public:
        RenderTarget(const glm::ivec2 &size, 
			int colorBufNum, 
			TextureFormat format, 
			TextureFilterMode minFilter = TextureFilterMode::Nearest,
			TextureFilterMode magFilter = TextureFilterMode::Nearest,
			bool mipmaps = false,
                     bool depthBuffer = false, bool stencilBuffer = false);

         Texture2D *operator[](int num) { return m_colorBufs[num].get(); }

         const Texture2D *operator[](int num) const { return m_colorBufs[num].get(); }

         const Framebuffer *GetFrameBuffer() const noexcept { return m_frameBuf.get(); }

         Framebuffer *GetFrameBuffer() noexcept { return m_frameBuf.get(); }

         void Bind() const { m_frameBuf->Bind(); }

         void Unbind() const { m_frameBuf->Unbind(); }
    };
}