namespace ogele {
	class VertexAccumulator {
		Entity(VertexAccumulator)
		unique_ptr<DoubleBuffer<dvec3>> m_coordsBuf;
		unique_ptr<FloatBuffer<vec3>> m_colorsBuf;
		unique_ptr<VertexArray> m_vao;
		int m_count;
		int m_currentCount;
		dvec3* m_coordsData;
		vec3* m_colorsData;
	public:
		VertexAccumulator(Topology topology);
		void StartFrame();
		void PushVertex(const glm::dvec3& coord, const glm::vec3& color);
		void EndFrame();
		void Draw();
	};
}
