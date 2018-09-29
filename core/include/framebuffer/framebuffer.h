//
// Created by ??????? on 22.07.2018.
//

#ifndef OGELE_FRAMEBUFFER_H
#define OGELE_FRAMEBUFFER_H
namespace ogele {

    class Renderbuffer : public HandleBase<GLenum> {
        Entity(Renderbuffer)

    private:
        RenderBufferFormat m_format;
    public:
        Renderbuffer(RenderBufferFormat format, const ivec2 &size);

        inline void Bind() const { glBindRenderbuffer(GL_RENDERBUFFER, m_handle); GLErr(); }

        inline void Unbind() const { glBindRenderbuffer(GL_RENDERBUFFER, 0); GLErr(); }

        inline RenderBufferFormat GetFormat() const noexcept { return m_format; }

        ~Renderbuffer();
    };

    class Framebuffer : public HandleBase<GLenum> {
        Entity(Framebuffer)

    public:
        Framebuffer();

        void bAttachColorBuffer(int num, Texture2D *texture, int level) {
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + num, texture->GetHandle(), level); GLErr();
        }

        void bAttachColorBuffer(int num, Renderbuffer *buffer) {
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + num, GL_RENDERBUFFER,
                                      buffer->GetHandle()); GLErr();
        }

        void bAttachDepthBuffer(Texture2D *texture, int level) {
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture->GetHandle(), level); GLErr();
        }

        void bAttachDepthBuffer(Renderbuffer *buffer) {
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
                                      buffer->GetHandle()); GLErr();
        }

        void bAttachStencilBuffer(Texture2D *texture, int level) {
            glFramebufferTexture(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, texture->GetHandle(), level); GLErr();
        }

        void bAttachStencilBuffer(Renderbuffer *buffer) {
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                                      buffer->GetHandle()); GLErr();
        }

        void bDrawBuffers(const vector<int> &attachments);

        void bDrawBuffers(const initializer_list<int> &attachments);

        FrameBufferCompletion bGetCompletionStatus() const;

        inline bool bIsComplete() const { return bGetCompletionStatus() == FrameBufferCompletion::Complete; }

        void Copy(Framebuffer *destination, ivec2 uv0, ivec2 uv1, BufferBit buffers, int sourceColorAttachment,
                  int destColorAttachment) const;

        void Copy(ivec2 uv0, ivec2 uv1, BufferBit buffers, int sourceColorAttachment,
                  DefaultColorBuffers destColorBuf) const;

        inline void Bind() const { glBindFramebuffer(GL_FRAMEBUFFER, m_handle); GLErr(); }

        inline void Unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); GLErr(); }

        ~Framebuffer();
    };
}
#endif //OGELE_FRAMEBUFFER_H
