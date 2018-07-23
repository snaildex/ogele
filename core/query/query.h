//
// Created by ??????? on 21.07.2018.
//

#ifndef OGELE_QUERY_H
#define OGELE_QUERY_H
namespace ogele {

    class Query : public HandleBase<GLenum> {
        Entity(Query)

    private:
        QueryType m_type;
    public:
        Query(QueryType type);

        void Begin() { glBeginQuery(static_cast<GLenum>(m_type), m_handle); GLErr(); }

        void End() { glEndQuery(static_cast<GLenum>(m_type));  GLErr(); }

        inline int GetResultI() const {
            int res;
            glGetQueryObjectiv(m_handle, GL_QUERY_RESULT, &res); GLErr();
            return res;
        }

        inline unsigned int GetResultU() const {
            unsigned int res;
            glGetQueryObjectuiv(m_handle, GL_QUERY_RESULT, &res); GLErr();
            return res;
        }

        inline long long GetResultI64() const {
            long long res;
            glGetQueryObjecti64v(m_handle, GL_QUERY_RESULT, &res); GLErr();
            return res;
        }

        inline unsigned long long GetResultU64() const {
            unsigned long long res;
            glGetQueryObjectui64v(m_handle, GL_QUERY_RESULT, &res); GLErr();
            return res;
        }

        ~Query();
    };
}
#endif //OGELE_QUERY_H
