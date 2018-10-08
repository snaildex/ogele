//
// Created by ??????? on 22.07.2018.
//

#include <ogele.h>

using namespace std;
using namespace glm;
namespace fs = std::experimental::filesystem;

namespace ogele {

	RenderTarget::RenderTarget(const ivec2 &size, int colorBufNum, TextureFormat format, bool mipmaps, bool depthBuffer,
		bool stencilBuffer) {
		m_frameBuf = make_unique<Framebuffer>();
		m_frameBuf->Bind();
		vector<int> indecies;
		for (int i = 0; i < colorBufNum; i++) {
			indecies.push_back(i);
			m_colorBufs.emplace_back(new Texture2D(size, mipmaps, format));
			m_frameBuf->bAttachColorBuffer(i, m_colorBufs[i].get(), 0);
		}
		if (depthBuffer) {
			m_depthBuffer = make_unique<Renderbuffer>(RenderBufferFormat::DepthComponent32, size);
			m_frameBuf->bAttachDepthBuffer(m_depthBuffer.get());
		}
		if (stencilBuffer) {
			m_stencilBuffer = make_unique<Renderbuffer>(RenderBufferFormat::StencilIndex16, size);
			m_frameBuf->bAttachStencilBuffer(m_stencilBuffer.get());
		}
		m_frameBuf->bDrawBuffers(indecies);
		if (!m_frameBuf->bIsComplete())
			throw runtime_error("Framebuffer is not complete");
		m_frameBuf->Unbind();

	}
}
