#pragma once
namespace ogele::Lua {
	class State : public Object {
		void* m_lua;
		fs::path m_source;

		void PrintError();

		template <typename T> static int LuaMethodClosureWrapper(lua_State* state) {
			union {
				int (T::*methodPtr)(State* s);
				void* ptr;
			} hack;
			hack.ptr = lua_touserdata(state, lua_upvalueindex(3));
			T* optr = static_cast<T*>(lua_touserdata(state, lua_upvalueindex(2)));
			State* sptr = static_cast<State*>(lua_touserdata(state, lua_upvalueindex(1)));
			return (optr->*hack.methodPtr)(sptr);
		}

		template <typename T> void PushOne(T value);
		template <typename T> void GetInternal(int index, T& arg) { arg = Peek<T>(index); }
		template <typename T, typename... Args> void GetInternal(int index, T& arg, Args&... args) { arg = Peek<T>(index); GetInternal(++index, args...); }
		void RegisterClosure(const std::string& name, int(*function)(lua_State*), int argNum);
		int LuaLog(State* state);
		void SetTable(int index);

	public:
		State(const fs::path& source);
		~State();

		const fs::path& GetSourcePath() const noexcept { return m_source; }
		int GetCount();

		void RawCall(int argNum, int retNum);
		void GetGlobal(const std::string& name);
		void SetGlobal(const std::string& name);

		template <typename T> void Push(T arg) { PushOne(arg); }
		template <typename T, typename... Args> void Push(T arg, Args... args) { Push(arg);  Push(args...); }
		void Pop(int count);
		template <typename T> T Peek(int index);
		template <typename... Args> void Get(Args&... args) { GetInternal(-sizeof...(args), args...); Pop(sizeof...(args)); }
		void SetFunction(const std::string& name, int(*function)(State*));
		template <typename T> void SetMethod(const std::string& name, T* object, int(__thiscall T::*callback)(State* s)) {
			static_assert(sizeof(callback) <= sizeof(void*));
			union {
				int(__thiscall T::*method)(State* s);
				void* ptr;
			} hack;
			hack.method = callback;
			Push((void*)this, (void*)object, hack.ptr);
			RegisterClosure(name, LuaMethodClosureWrapper<T>, 3);
		}

		template<typename T> void SetGlobalVar(const std::string& name, T var) { Push(var); SetGlobal(name); };
		template<typename T> T GetGlobalVar(const std::string& name) { T res;  GetGlobal(name); Get(res); return res; };

		void Load();

		template <typename... Args> void Call(const std::string& name, Args... args) {
			Push(args...);
			GetGlobal(name);
			RawCall(sizeof...(args), 0);
		}
		void Call(const std::string& name) {
			GetGlobal(name);
			RawCall(0, 0);
		}
		template <typename T, typename... Args> T CallRet(const std::string& name, Args... args) {
			T res;
			Push(args...);
			GetGlobal(name);
			RawCall(sizeof...(args), 1);
			Get(res);
			return res;
		}
		template <typename T> T CallRet(const std::string& name) {
			T res;
			GetGlobal(name);
			RawCall(0, 1);
			Get(res);
			return res;
		}
		void PeekTable(int index, std::function<void(const std::string& fieldName)> fieldProcessor);
		void PeekArray(int index, std::function<void(int index)> arrayProcessor);

		void CreateTable(int arrayCount, int recordCount);
		template <typename T> void TableField(const std::string& name, T value, int index = -3) {
			Push(name, value);
			SetTable(index);
		}

		void LogStack();
	};
}