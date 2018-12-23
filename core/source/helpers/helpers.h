#pragma once
namespace fs = std::filesystem;
namespace ogele {

#define Entity(Type)\
Type ( const Type & ) = delete;\
const Type & operator=( const Type & ) = delete;

#define EnumFlags(Type)\
 constexpr Type operator| ( Type l, Type r) noexcept { return static_cast< Type >(static_cast<std::underlying_type_t< Type >>(l)|static_cast<std::underlying_type_t< Type >>(r));}\
 constexpr Type operator& ( Type l, Type r) noexcept { return static_cast< Type >(static_cast<std::underlying_type_t< Type >>(l)&static_cast<std::underlying_type_t< Type >>(r));}

	template<typename HandleType>
	class HandleBase {
		Entity(HandleBase)

	protected:
		HandleType m_handle;

		HandleBase() = default;

	public:
		 HandleType GetHandle() const noexcept { return m_handle; }
	};

	std::list<fs::path> ScanFiles(const std::string &extension);

	template <typename T>
	class swapchain {
		std::vector<std::unique_ptr<T>> m_ptrs;
		int m_current;
	public:
		swapchain() { m_current = 0; }
		 void add(T* ptr) { m_ptrs.push_back(std::unique_ptr<T>(ptr)); }
		 void swap() noexcept { m_current = (m_current + 1) % m_ptrs.size(); }
		 T* get() const noexcept { return m_ptrs[m_current].get(); }
		 T* operator->() const noexcept { return m_ptrs[m_current].get(); }
		 T* operator[](int index) const { return m_ptrs[(m_current + m_ptrs.size() - index) % m_ptrs.size()].get(); }
		 size_t size() const noexcept { return m_ptrs.size(); }
		 void reset(T* newVal, int index) { m_ptrs[index].reset(newVal); }
	};

}
