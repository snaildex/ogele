//
// Created by ??????? on 18.07.2018.
//

#ifndef OGELE_HELPERS_H
#define OGELE_HELPERS_H

using namespace std;
using namespace glm;
namespace fs = std::experimental::filesystem;

namespace ogele {

#define Entity(Type)\
Type ( const Type & ) = delete;\
const Type & operator=( const Type & ) = delete;

#define EnumFlags(Type)\
inline constexpr Type operator| ( Type l, Type r) noexcept { return static_cast< Type >(static_cast<std::underlying_type_t< Type >>(l)|static_cast<std::underlying_type_t< Type >>(r));}\
inline constexpr Type operator& ( Type l, Type r) noexcept { return static_cast< Type >(static_cast<std::underlying_type_t< Type >>(l)&static_cast<std::underlying_type_t< Type >>(r));}

	template<typename HandleType>
	class HandleBase {
		Entity(HandleBase)

	protected:
		HandleType m_handle;

		HandleBase() = default;

	public:
		inline HandleType GetHandle() const noexcept { return m_handle; }
	};

	list<fs::path> ScanFiles(const string &extension);

	template <typename T>
	class swapchain {
		vector<unique_ptr<T>> m_ptrs;
		int m_current;
	public:
		swapchain() { m_current = 0; }
		inline void add(T* ptr) { m_ptrs.push_back(unique_ptr<T>(ptr)); }
		inline void swap() noexcept { m_current = (m_current + 1) % m_ptrs.size(); }
		inline T* get() const noexcept { return m_ptrs[m_current].get(); }
		inline T* operator->() const noexcept { return m_ptrs[m_current].get(); }
		inline T* operator[](int index) const { return m_ptrs[(m_current + m_ptrs.size() - index) % m_ptrs.size()].get(); }
		inline size_t size() const noexcept { return m_ptrs.size(); }
		inline void reset(T* newVal, int index) { m_ptrs[index].reset(newVal); }
	};

}

#endif //OGELE_HELPERS_H
