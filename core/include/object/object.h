#pragma once

#include <string>
#include <set>

namespace ogele {
    class Object {
        std::string m_name;
        std::set <std::string> m_tags;

    public:
        Object();

        inline const std::string &GetName() const noexcept { return m_name; }

        inline void SetName(const std::string &name) noexcept { m_name = name; }

        inline bool HasTag(const std::string &tag) const noexcept { return m_tags.find(tag) != m_tags.cend(); }

        inline void AddTag(const std::string &tag) noexcept { m_tags.insert(tag); }

        inline void RemoveTag(const std::string &tag) noexcept { m_tags.erase(tag); }

		std::string PrintTags();

        void CopyNameTagsFrom(const Object *source) noexcept;

        virtual ~Object() = default;
    };
}