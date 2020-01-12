#pragma once
namespace ogele {

	class Resource : public Object
	{
		Entity(Resource)
	public:
		Resource() = default;
		virtual ~Resource() = default;
	};

	class ResourceProxy : public Object
	{
		Entity(ResourceProxy)
	public:
		ResourceProxy() = default;
		virtual ~ResourceProxy() = default;
		virtual Resource* Build() const = 0;
	};

	class ResourcePtrBase {
		std::unique_ptr<ResourceProxy> m_proxy;
		std::unique_ptr<Resource> m_resource;
		std::future<Resource*> m_future;

	public:
		ResourceProxy* GetProxy() const noexcept { return m_proxy.get(); }
		Resource* Get();
		void Build() { m_resource.reset(m_proxy->Build()); }
		void BuildAsync();
		void Wait();
		void Destroy() { m_resource.reset(); }
		ResourcePtrBase(ResourceProxy* proxy) { m_proxy.reset(proxy); };
		ResourcePtrBase() = default;
	};

	template<typename T>
	class res_ptr {
		ResourcePtrBase* m_ptr;
	public:
		res_ptr() : m_ptr(nullptr) {}
		res_ptr(ResourcePtrBase* ptr) : m_ptr(ptr) {}
		T* get() const noexcept { return reinterpret_cast<T*>(m_ptr->Get()); }
		ResourceProxy* getProxy() const noexcept { return m_ptr->GetProxy(); }
		T* operator-> () const noexcept { return get(); }
		explicit operator bool() const noexcept { return m_ptr != nullptr; }
		void build() { m_ptr->Build(); }
		res_ptr& async() { m_ptr->BuildAsync(); return *this; }
		void destroy() { m_ptr->Destroy(); }
	};

	class ResourceLoader
	{
		Entity(ResourceLoader);
		std::vector<fs::path> m_files;
		std::vector<std::string> m_extensions;
	protected:
		ResourceLoader() = default;
		void LoadNameTags(ResourceProxy* res, const fs::path& path) const;
	public:
		const std::vector<fs::path>& GetFiles() const noexcept { return m_files; }
		void EnlistFile(const fs::path& file) noexcept { m_files.push_back(fs::absolute(file)); }
		virtual bool CanLoad(const fs::path& file) const = 0;
		virtual std::vector<ResourceProxy*> Load() const = 0;
		virtual ~ResourceLoader() {}
	};
}
