#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace fs = std::filesystem;
namespace ogele {
	void ClearColor(const glm::vec4 &color) {
		glClearColor(color.r, color.g, color.b, color.a);
		GLErr();
	}

	void Clear(BufferBit buffers) {
		glClear(static_cast<GLenum>(buffers));
		GLErr();
	}

	void Viewport(const glm::ivec2 &offset, const glm::ivec2 &size) {
		glViewport(offset.x, offset.y, size.x, size.y);
		GLErr();
	}

	void Enable(Feature feature) {
		glEnable(static_cast<GLenum>(feature));
		GLErr();
	}

	void Disable(Feature feature) {
		glDisable(static_cast<GLenum>(feature));
		GLErr();
	}

	void BlendFunc(BlendFactor src, BlendFactor dst)
	{
		glBlendFunc(static_cast<GLenum>(src), static_cast<GLenum>(dst));
		GLErr();
	}

	void Barrier(MemoryBarriers barriers) {
		glMemoryBarrier(static_cast<GLenum>(barriers));
		GLErr();
	}

	void SetPolygonMode(PolygonFace face, PolygonMode mode) {
		glPolygonMode(static_cast<GLenum>(face), static_cast<GLenum>(mode));
		GLErr();
	}

	void Finish() {
		glFinish();
		GLErr();
	}
}