#pragma once
namespace ogele
{
    class VertexArray : HandleBase<GLuint>
    {
        Entity(VertexArray)
        Topology m_topology;
        bool m_indexed;
        int m_patchVerticies;
        glm::vec4 m_defOuterTess;
        glm::vec2 m_defInnerTess;

    public:

        VertexArray(Topology topology);

         Topology GetTopology() const noexcept { return m_topology; }
         int GetPatchVertCount() const noexcept { return m_patchVerticies; }
         void SetPatchVertCount(int value) noexcept { m_patchVerticies=value; }
         const glm::vec4& GetDefOuterTessFactor() const noexcept { return m_defOuterTess; }
         void SetDefOuterTessFactor(const glm::vec4& value) noexcept { m_defOuterTess=value; }
         const glm::vec2& GetDefInnerTessFactor() const noexcept { return m_defInnerTess; }
         void SetDefInnerTessFactor(const glm::vec2& value) noexcept { m_defInnerTess=value; }

         void Bind() const { glBindVertexArray(m_handle); GLErr(); }
         void Unbind() const { glBindVertexArray(0); GLErr(); }
         bool IsIndexed() const noexcept { return m_indexed; }
         void bEnableVertexAttribArray(int index) { glEnableVertexArrayAttrib(m_handle, index); GLErr(); }
         void bDisableVertexAttribArray(int index) { glDisableVertexArrayAttrib(m_handle, index); GLErr(); }
        void bDraw(int first, int count) const;
        void bDraw(int first, int count, int instanceCount) const;

        void bAttachBuffer(int index, int vectorLength, const BufferBase* buffer, int offset = 0);
        void bAttachElementBuffer(const UnsignedIntBuffer<unsigned int>* buffer);

        ~VertexArray();
    };
}
