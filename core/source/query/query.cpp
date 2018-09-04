//
// Created by ??????? on 21.07.2018.
//

#include <ogele.h>

using namespace std;
using namespace glm;
namespace fs = std::experimental::filesystem;

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
