#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {
	Renderbuffer::Renderbuffer(RenderBufferFormat format, const ivec2 &size) {
		m_format = format;
		glGenRenderbuffers(1, &m_handle);
		GLErr();
		Bind();
		glRenderbufferStorage(GL_RENDERBUFFER, static_cast<GLenum>(format), size.x, size.y);
		GLErr();
		Unbind();
	}

	Renderbuffer::~Renderbuffer() {
		glDeleteRenderbuffers(1, &m_handle);
		GLErr();
	}

	void Renderbuffer::Resize(const glm::ivec2 &newSize) {
		glDeleteRenderbuffers(1, &m_handle);
		GLErr();
		glGenRenderbuffers(1, &m_handle);
		GLErr();
		Bind();
		glRenderbufferStorage(GL_RENDERBUFFER, static_cast<GLenum>(m_format), newSize.x, newSize.y);
		GLErr();
		Unbind();
	}

	Framebuffer::Framebuffer() {
		glGenFramebuffers(1, &m_handle);
		GLErr();
	}

	void Framebuffer::bDrawBuffers(const vector<int> &attachments) {
		GLenum *bufs = new GLenum[attachments.size()];
		for (int i = 0; i < attachments.size(); i++)
			bufs[i] = attachments[i] + GL_COLOR_ATTACHMENT0;
		glDrawBuffers(attachments.size(), bufs);
		delete[] bufs;
		GLErr();
	}

	void Framebuffer::bDrawBuffers(const initializer_list<int> &attachments) {
		GLenum *bufs = new GLenum[attachments.size()];
		int i = 0;
		for (const auto at : attachments)
			bufs[i++] = at + GL_COLOR_ATTACHMENT0;
		glDrawBuffers(attachments.size(), bufs);
		delete[] bufs;
		GLErr();
	}

	FrameBufferCompletion Framebuffer::bGetCompletionStatus() const {
		GLenum res = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		GLErr();
		return static_cast<FrameBufferCompletion>(res);
	}

	void Framebuffer::Copy(Framebuffer *destination, ivec2
		uv0,
		ivec2 uv1, BufferBit
		buffers,
		int sourceColorAttachment,
		int destColorAttachment
	) const {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_handle);
		GLErr();
		glReadBuffer(GL_COLOR_ATTACHMENT0 + sourceColorAttachment);
		GLErr();
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destination->m_handle);
		GLErr();
		glDrawBuffer(GL_COLOR_ATTACHMENT0 + destColorAttachment);
		GLErr();
		glBlitFramebuffer(uv0
			.x, uv0.y, uv1.x, uv1.y, uv0.x, uv0.y, uv1.x, uv1.y, static_cast
			<GLbitfield>(buffers),
			GL_NEAREST);
		GLErr();
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		GLErr();
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		GLErr();
	}

	void Framebuffer::Copy(ivec2 uv0, ivec2 uv1, BufferBit buffers, int sourceColorAttachment,
		DefaultColorBuffers destColorBuf) const {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_handle);
		GLErr();
		glReadBuffer(GL_COLOR_ATTACHMENT0 + sourceColorAttachment);
		GLErr();
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		GLErr();
		glDrawBuffer(static_cast<GLenum>(destColorBuf));
		GLErr();
		glBlitFramebuffer(uv0.x, uv0.y, uv1.x, uv1.y, uv0.x, uv0.y, uv1.x, uv1.y, static_cast<GLbitfield>(buffers),
			GL_NEAREST);
		GLErr();
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		GLErr();
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		GLErr();
	}

	Framebuffer::~Framebuffer() {
		glDeleteFramebuffers(1, &m_handle);
		GLErr();
	}

}
