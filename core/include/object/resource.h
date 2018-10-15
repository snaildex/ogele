#pragma once

#include <list>
#include <vector>
#include <map>
#include <memory>

#include <jzon/Jzon.h>

#include "object.h"
#include "../helpers/helpers.h"

namespace ogele {

    class Resource : public Object
    {
        Entity(Resource)
    public:
        Resource() = default;
        virtual ~Resource() = default;
    };

    class ResourceLoader
    {
        Entity(ResourceLoader)
    protected:
        ResourceLoader() = default;
        void LoadNameTags(const Jzon::Node* reader, Resource* res) const;
    public:
        virtual ~ResourceLoader() {}
        virtual Resource* Load(const Jzon::Node* reader) const=0;
    };

    class ResourceContainer
    {
        Entity(ResourceContainer)

    private:

        std::list<std::unique_ptr<Resource>> m_data;
		std::map <std::string, std::unique_ptr<ResourceLoader>> m_loaders;

    public:

        ResourceContainer();

        void LoadFromFilesystem();

        void AddResource(Resource* resource) noexcept
        {
            m_data.emplace_back(std::unique_ptr<Resource>(resource));
        }

        void GUI();

        template<typename T> T* GetResourceByName(const std::string& name) const noexcept
        {
            for (const auto& t : m_data)
                if (t->GetName() == name)
                {
                    T* res = dynamic_cast<T*>(t.get());
                    if (res != nullptr)
                        return res;
                }
            return nullptr;
        }

        template<typename T> T* GetResourceByTag(const std::string& tag) const noexcept
        {
            for (const auto& t : m_data)
                if (t->HasTag(tag))
                {
                    T* res = dynamic_cast<T*>(t.get());
                    if (res != nullptr)
                        return res;
                }
        }

        template<typename T> std::vector<T*> GetResources() const noexcept
        {
            std::vector<T*> res;
            for (const auto& t : m_data)
            {
                T* r = dynamic_cast<T*>(t.get());
                if (r != nullptr)
                    res.push_back(r);
            }
            return res;
        }

        template<typename T> std::vector<T*> GetResourcesByTag(const std::string& tag) const noexcept
        {
            std::vector<T*> res;
            for (const auto& t : m_data)
                if (t->HasTag(tag))
                {
                    T* r = dynamic_cast<T*>(t.get());
                    if (r != nullptr)
                        res.push_back(r);
                }
            return res;
        }

    };

}
