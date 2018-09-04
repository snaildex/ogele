//
// Created by ??????? on 18.07.2018.
//

#ifndef OGELE_OBJECT_H
#define OGELE_OBJECT_H
namespace ogele {
    class Object {
        string m_name;
        set <string> m_tags;

    public:
        Object();

        inline const string &GetName() const noexcept { return m_name; }

        inline void SetName(const string &name) noexcept { m_name = name; }

        inline bool HasTag(const string &tag) const noexcept { return m_tags.find(tag) != m_tags.cend(); }

        inline void AddTag(const string &tag) noexcept { m_tags.insert(tag); }

        inline void RemoveTag(const string &tag) noexcept { m_tags.erase(tag); }

        string PrintTags();

        void CopyNameTagsFrom(const Object *source) noexcept;

        virtual ~Object() = default;
    };
}
#endif //OGELE_OBJECT_H
