//
// Created by ??????? on 22.07.2018.
//

#ifndef OGELE_RENDERTARGET_H
#define OGELE_RENDERTARGET_H
namespace ogele {

    class RenderTarget {
        unique_ptr<Framebuffer> m_frameBuf;
        vector<unique_ptr<Texture2D>> m_colorBufs;
        unique_ptr<Renderbuffer> m_depthBuffer;
        unique_ptr<Renderbuffer> m_stencilBuffer;
    public:
        RenderTarget(const ivec2 &size, int colorBufNum, TextureFormat format, bool mipmaps = false,
                     bool depthBuffer = false, bool stencilBuffer = false);

        inline Texture2D *operator[](int num) { return m_colorBufs[num].get(); }

        inline const Texture2D *operator[](int num) const { return m_colorBufs[num].get(); }

        inline const Framebuffer *GetFrameBuffer() const noexcept { return m_frameBuf.get(); }

        inline Framebuffer *GetFrameBuffer() noexcept { return m_frameBuf.get(); }

        inline void Bind() const { m_frameBuf->Bind(); }

        inline void Unbind() const { m_frameBuf->Unbind(); }
    };
}
#endif //OGELE_RENDERTARGET_H
