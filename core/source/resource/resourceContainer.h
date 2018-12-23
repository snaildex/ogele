#pragma once
namespace ogele {
	class ResourceContainer {
		Entity(ResourceContainer)
	private:
		std::list<std::unique_ptr<ResourcePtrBase>> m_data;
		std::map <std::string, std::unique_ptr<ResourceLoader>> m_loaders;

	public:
		ResourceContainer();

		void LoadFromFilesystem();

		void AddResource(ResourceProxy* resource) noexcept
		{
			m_data.emplace_back(new ResourcePtrBase(resource));
		}

		void GUI();

		template<typename T> 
		res_ptr<T> GetResourceByName(const std::string& name) const noexcept
		{
			for (const auto& t : m_data)
				if (t->GetProxy()->GetName() == name)
				{
					typename T::Proxy* res = dynamic_cast<typename T::Proxy*>(t->GetProxy());
					if (res != nullptr)
						return res_ptr<T>(t.get());
				}
			return res_ptr<T>();
		}

		template<typename T> 
		res_ptr<T> GetResourceByTag(const std::string& tag) const noexcept
		{
			for (const auto& t : m_data)
				if (t->GetProxy()->HasTag(tag))
				{
					typename T::Proxy* res = dynamic_cast<typename T::Proxy*>(t->GetProxy());
					if (res != nullptr)
						return res_ptr<T>(t.get());
				}
			return res_ptr<T>();
		}

		template<typename T> 
		res_ptr<T> GetResourceByTags(const std::vector<std::string>& tags) const noexcept
		{
			for (const auto& t : m_data)
				if (t->GetProxy()->HasTags(tags))
				{
					typename T::Proxy* res = dynamic_cast<typename T::Proxy*>(t->GetProxy());
					if (res != nullptr)
						return res_ptr<T>(t.get());
				}
			return res_ptr<T>();
		}

		template<typename T> 
		std::vector<res_ptr<T>> GetResources() const noexcept
		{
			std::vector<res_ptr<T>> res;
			for (const auto& t : m_data)
			{
				typename T::Proxy* r = dynamic_cast<typename T::Proxy*>(t->GetProxy());
				if (r != nullptr)
					res.emplace_back(t.get());
			}
			return res;
		}

		template<typename T> 
		std::vector<res_ptr<T>> GetResourcesByTag(const std::string& tag) const noexcept
		{
			std::vector<res_ptr<T>> res;
			for (const auto& t : m_data)
				if (t->GetProxy()->HasTag(tag))
				{
					typename T::Proxy* r = dynamic_cast<typename T::Proxy*>(t->GetProxy());
					if (r != nullptr)
						res.emplace_back(t.get());
				}
			return res;
		}

		template<typename T> 
		std::vector<res_ptr<T>> GetResourcesByTags(const std::vector<std::string>& tags) const noexcept
		{
			std::vector<res_ptr<T>> res;
			for (const auto& t : m_data)
				if (t->GetProxy()->HasTags(tags))
				{
					typename T::Proxy* r = dynamic_cast<typename T::Proxy*>(t->GetProxy());
					if (r != nullptr)
						res.emplace_back(t.get());
				}
			return res;
		}
	};

}
