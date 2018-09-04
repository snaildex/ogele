//
// Created by ??????? on 19.07.2018.
//

#ifndef OGELE_RESOURCE_H
#define OGELE_RESOURCE_H
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

        list<unique_ptr<Resource>> m_data;
        map <string, unique_ptr<ResourceLoader>> m_loaders;

    public:

        ResourceContainer();

        void LoadFromFilesystem();

        void AddResource(Resource* resource) noexcept
        {
            m_data.emplace_back(unique_ptr<Resource>(resource));
        }

        void GUI();

        template<typename T> T* GetResourceByName(const string& name) const noexcept
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

        template<typename T> T* GetResourceByTag(const string& tag) const noexcept
        {
            for (const auto& t : m_data)
                if (t->HasTag(tag))
                {
                    T* res = dynamic_cast<T*>(t.get());
                    if (res != nullptr)
                        return res;
                }
        }

        template<typename T> vector<T*> GetResources() const noexcept
        {
            vector<T*> res;
            for (const auto& t : m_data)
            {
                T* r = dynamic_cast<T*>(t.get());
                if (r != nullptr)
                    res.push_back(r);
            }
            return res;
        }

        template<typename T> vector<T*> GetResourcesByTag(const string& tag) const noexcept
        {
            vector<T*> res;
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
#endif //OGELE_RESOURCE_H
