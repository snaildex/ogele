#pragma once
namespace ogele {

    class Query : public HandleBase<GLenum> {
        Entity(Query)

    private:
        QueryType m_type;
    public:
        Query(QueryType type);

        void Begin() { glBeginQuery(static_cast<GLenum>(m_type), m_handle); GLErr(); }

        void End() { glEndQuery(static_cast<GLenum>(m_type));  GLErr(); }

         int GetResultI() const {
            int res;
            glGetQueryObjectiv(m_handle, GL_QUERY_RESULT, &res); GLErr();
            return res;
        }

         unsigned int GetResultU() const {
            unsigned int res;
            glGetQueryObjectuiv(m_handle, GL_QUERY_RESULT, &res); GLErr();
            return res;
        }

         long long GetResultI64() const {
            long long res;
            glGetQueryObjecti64v(m_handle, GL_QUERY_RESULT, &res); GLErr();
            return res;
        }

         unsigned long long GetResultU64() const {
            unsigned long long res;
            glGetQueryObjectui64v(m_handle, GL_QUERY_RESULT, &res); GLErr();
            return res;
        }

        ~Query();
    };
}
