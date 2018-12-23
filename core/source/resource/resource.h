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
		T* operator-> () const noexcept { return get(); }
		explicit operator bool() const noexcept { return m_ptr != nullptr; }
		void build() { m_ptr->Build(); }
		res_ptr& async() { m_ptr->BuildAsync(); return *this; }
		void destroy() { m_ptr->Destroy(); }
	};

	class ResourceLoader
	{
		Entity(ResourceLoader)
	protected:
		ResourceLoader() = default;
		void LoadNameTags(const Jzon::Node* reader, ResourceProxy* res) const;
		std::string ReadStringProperty(const Jzon::Node *reader, const std::string &propName, const std::string &defaultValue) const;
		std::vector<std::string> ReadStringArrayProperty(const Jzon::Node *reader, const std::string &propName) const;
		int ReadIntProperty(const Jzon::Node *reader, const std::string &propName, int defaultValue) const;
		float ReadFloatProperty(const Jzon::Node *reader, const std::string &propName, float defaultValue) const;
		double ReadDoubleProperty(const Jzon::Node *reader, const std::string &propName, double defaultValue) const;
		bool ReadBoolProperty(const Jzon::Node *reader, const std::string &propName, bool defaultValue) const;

	public:
		virtual ~ResourceLoader() {}
		virtual ResourceProxy* Load(const Jzon::Node* reader) const = 0;
	};
}
