#pragma once
namespace ogele {
	class MeshBase : public Resource {
		std::unique_ptr<VertexArray> m_vao;
		int m_count;
	protected:
		void SetBuffer(int index, int vectorSize, const BufferBase *buffer, int offset = 0);

		void SetCount(int count) noexcept { m_count = count; }

		MeshBase(Topology topology);

	public:
		VertexArray *GetVAO() const noexcept { return m_vao.get(); }

		void Draw() const {
			m_vao->Bind();
			m_vao->bDraw(0, m_count);
		}

		void Draw(int offset, int count) const {
			m_vao->Bind();
			m_vao->bDraw(offset, count);
		}

		void Draw(int instanceCount) const {
			m_vao->Bind();
			m_vao->bDraw(0, m_count, instanceCount);
		}

		void Draw(int offset, int count, int instanceCount) const {
			m_vao->Bind();
			m_vao->bDraw(offset, count, instanceCount);
		}
	};
}