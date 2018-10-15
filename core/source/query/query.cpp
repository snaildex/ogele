#include <query/query.h>

using namespace std;

namespace ogele {

    Query::Query(QueryType type) {
        m_type = type;
        glGenQueries(1, &m_handle);
        GLErr();
    }

    Query::~Query() {
        glDeleteQueries(1, &m_handle);
        GLErr();
    }
}
