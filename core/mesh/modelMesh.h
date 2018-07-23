//
// Created by ??????? on 22.07.2018.
//

#ifndef OGELE_MODELMESH_H
#define OGELE_MODELMESH_H
namespace ogele {
    class ModelMesh : public MeshBase {
        unique_ptr <FloatBuffer<vec3>> m_position;
        unique_ptr <FloatBuffer<vec3>> m_normals;
        unique_ptr <FloatBuffer<vec3>> m_tangents;
        unique_ptr <FloatBuffer<vec4>> m_color;
        array <unique_ptr<FloatBuffer < vec2>>, 2>
        m_uvs;
        unique_ptr <UnsignedIntBuffer<unsigned int>> m_indexes;
    public:
        ModelMesh() : MeshBase(Topology::Triangles) {}

        inline FloatBuffer <vec3> *GetPositionBuffer() const noexcept { return m_position.get(); }

        inline FloatBuffer <vec3> *GetNormalsBuffer() const noexcept { return m_normals.get(); }

        inline FloatBuffer <vec3> *GetTangentsBuffer() const noexcept { return m_tangents.get(); }

        inline FloatBuffer <vec4> *GetColorBuffer() const noexcept { return m_color.get(); }

        inline FloatBuffer <vec2> *GetUVBuffer(int num) const { return m_uvs[num].get(); }

        inline constexpr int GetUVBufferCount() const noexcept { return m_uvs.size(); }

        inline void SetPositionBuffer(FloatBuffer <vec3> *buffer) {
            SetCount(buffer->GetSize() * 3);
            m_position.reset(buffer);
            SetBuffer(0, 3, buffer);
        }

        inline void SetNormalsBuffer(FloatBuffer <vec3> *buffer) {
            m_normals.reset(buffer);
            SetBuffer(1, 3, buffer);
        }

        inline void SetTangentsBuffer(FloatBuffer <vec3> *buffer) {
            m_tangents.reset(buffer);
            SetBuffer(2, 3, buffer);
        }

        inline void SetColorBuffer(FloatBuffer <vec4> *buffer) {
            m_color.reset(buffer);
            SetBuffer(3, 4, buffer);
        }

        inline void SetUVBuffer(int num, FloatBuffer <vec2> *buffer) {
            m_uvs[num].reset(buffer);
            SetBuffer(4 + num, 2, buffer);
        }

        void SetIndexBuffer(UnsignedIntBuffer<unsigned int> *buffer);
    };
}
#endif //OGELE_MODELMESH_H
