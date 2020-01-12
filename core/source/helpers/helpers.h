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

	constexpr const char* ___StripFileName(const char* file)
	{
		int i = 0;
		while (file[i] != '\0') i++;
		while (file[i] != '/' && file[i] != '\\') i--;
		return &file[i + 1];
	}

	void ___LogIndent();
	void ___LogUnindent();
	void ___Log(const char* file, int line, const char* fmt, ...);
#define Log(fmt, ...) ___Log(___StripFileName(__FILE__), __LINE__, fmt, __VA_ARGS__)
	void LogSpace();

	class ___LogIndenter {
	public:
		___LogIndenter() { ___LogIndent(); }
		~___LogIndenter() { ___LogUnindent(); }
	};
#define LogIndent ___LogIndenter ___logIndent

	constexpr void ___StripArgName(const char* input, std::string_view& name, const char*& leftover)
	{
		int i = 0;
		while (input[i] != ',' && input[i] != '\0' && input[i] != ' ' && input[i] != '\0') i++;
		name = std::string_view(input, i);
		while (input[i] == ' ' || input[i] == ',') i++;
		leftover = &input[i];
	}

	template <typename T>
	void ___DumpRecursive(std::stringstream& str, const char *argsNames, T first) {
		const char* leftover;
		std::string_view name;
		___StripArgName(argsNames, name, leftover);
		str << name << '=' << first;
	}

	template <typename T, typename ...Args>
	void ___DumpRecursive(std::stringstream& str, const char *argsNames, T first, Args... args) {
		const char* leftover;
		std::string_view name;
		___StripArgName(argsNames, name, leftover);
		str << name << '=' << first << ", ";
		___DumpRecursive(str, leftover, args...);
	}

	template <typename ...Args>
	void ___Dump(const char* file, int line, const char *argsNames, Args... args) {
		std::stringstream str;
		___DumpRecursive(str, argsNames, args...);
		___Log(file, line, str.str().data());
	}

#define Dump(...) ___Dump(___StripFileName(__FILE__), __LINE__, #__VA_ARGS__, __VA_ARGS__)

	bool CheckTail(const std::string& str, const std::string& tail);
	std::string StripTail(const std::string& str, size_t tailLength);
	int StripTailNum(const std::string& str);
	int StripTailNum(const std::string& str, std::string& leftover);

}
