#include <object/object.h>
#include <sstream>

using namespace std;

namespace ogele {
    Object::Object() {
        m_name = "New object";
    }

    void Object::CopyNameTagsFrom(const Object *source) noexcept {
        m_name = source->m_name;
        m_tags = source->m_tags;
    }

    string Object::PrintTags() {
        stringstream s;
        s << '(';
        for (const auto &tag : m_tags) {
            s << tag;
            s << ", ";
        }
        s.seekp(-2, std::ios_base::end);
        s << ')';
        return s.str();
    }
}