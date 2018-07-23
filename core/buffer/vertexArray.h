//
// Created by ??????? on 21.07.2018.
//

#ifndef OGELE_VERTEXARRAY_H
#define OGELE_VERTEXARRAY_H

namespace ogele
{
    class VertexArray : HandleBase<GLuint>
    {
        Entity(VertexArray)
        Topology m_topology;
        bool m_indexed;

    public:

        VertexArray(Topology topology);

        inline Topology GetTopology() const noexcept { return m_topology; }

        inline void Bind() const { glBindVertexArray(m_handle); GLErr(); }
        inline void Unbind() const { glBindVertexArray(0); GLErr(); }
        inline bool IsIndexed() const noexcept { return m_indexed; }
        inline void bEnableVertexAttribArray(int index) { glEnableVertexArrayAttrib(m_handle, index); GLErr(); }
        inline void bDisableVertexAttribArray(int index) { glDisableVertexArrayAttrib(m_handle, index); GLErr(); }
        void bDraw(int first, int count) const;
        void bDraw(int first, int count, int instanceCount) const;

        void bAttachBuffer(int index, int vectorLength, const BufferBase* buffer, int offset = 0);
        void bAttachElementBuffer(const UnsignedIntBuffer<unsigned int>* buffer);

        ~VertexArray();
    };
}

#endif //OGELE_VERTEXARRAY_H
