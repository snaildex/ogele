#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele::Lua {

#define CheckLuaError(action) if( action != LUA_OK) PrintError()
#define LUA_STATE reinterpret_cast<lua_State*>(m_lua)

	void State::PrintError()
	{
		lua_Debug ar;
		lua_getstack(LUA_STATE, 1, &ar);
		lua_getinfo(LUA_STATE, "nSl", &ar);
		string err;
		Get(err);
		___Log(m_source.filename().string().data(), ar.currentline, "Lua error: %s", err.data());
	}

	State::State(const fs::path & source)
	{
		m_lua = luaL_newstate();
		m_source = fs::absolute(source);
		luaL_openlibs(LUA_STATE);
		SetMethod("print", this, &State::LuaLog);
	}
	void State::Load() {
		CheckLuaError(luaL_loadfile(LUA_STATE, m_source.string().c_str()));
		CheckLuaError(lua_pcall(LUA_STATE, 0, 0, 0));
	}
	State::~State()
	{
		lua_close(LUA_STATE);
	}

	int State::GetCount()
	{
		return lua_gettop(LUA_STATE);
	}

	void State::SetTable(int index)
	{
		lua_settable(LUA_STATE, index);
	}

#define DefPushPopNumber(type)\
	template <> void State::PushOne( type value) { lua_pushnumber(LUA_STATE, static_cast<lua_Number>(value)); }\
	template<> type State::Peek( int index) { return static_cast< type >(lua_tonumber(LUA_STATE, index));}

	DefPushPopNumber(int);
	DefPushPopNumber(unsigned int);
	DefPushPopNumber(short);
	DefPushPopNumber(unsigned short);
	DefPushPopNumber(float);
	DefPushPopNumber(double);

	template <> void State::PushOne(std::string value) { lua_pushlstring(LUA_STATE, value.data(), value.length()); }
	template<> string State::Peek(int index) { return string(lua_tostring(LUA_STATE, index)); }
	template <> void State::PushOne(bool value) { lua_pushboolean(LUA_STATE, value); }
	template<> bool State::Peek(int index) { return lua_toboolean(LUA_STATE, index); }
	template <> void State::PushOne(void* value) { lua_pushlightuserdata(LUA_STATE, value); }
	template<> void* State::Peek(int index) { return lua_touserdata(LUA_STATE, index); }

	void State::GetGlobal(const std::string& name)
	{
		lua_getglobal(LUA_STATE, name.data());
	}

	void State::SetGlobal(const std::string & name)
	{
		lua_setglobal(LUA_STATE, name.data());
	}

	void State::RawCall(int argNum, int retNum)
	{
		CheckLuaError(lua_pcall(LUA_STATE, argNum, retNum, 1));
	}

	int State::LuaLog(State * state)
	{
		lua_Debug ar;
		lua_getstack(LUA_STATE, 1, &ar);
		lua_getinfo(LUA_STATE, "nSl", &ar);
		int num = state->GetCount();
		stringstream str;
		for (int i = 1; i <= num; ++i) {
			int t = lua_type(LUA_STATE, i);
			switch (t) {
			case LUA_TSTRING: str << lua_tostring(LUA_STATE, i); break;
			case LUA_TBOOLEAN: str << (lua_toboolean(LUA_STATE, i) ? "true" : "false");
			case LUA_TNUMBER: str << lua_tonumber(LUA_STATE, i); break;
			case LUA_TNIL: str << "nil"; break;
			case LUA_TLIGHTUSERDATA: str << "ptr[" << lua_topointer(LUA_STATE, i) << "]"; break;
			case LUA_TFUNCTION: str << "func[" << lua_topointer(LUA_STATE, i) << "]"; break;
			default: str << lua_typename(LUA_STATE, t); break;
			}
			str << ' ';
		}
		Pop(num);
		___Log(m_source.filename().string().data(), ar.currentline, str.str().data());
		return 0;
	}

	void State::PeekTable(int index, std::function<void(const std::string&fieldName)> fieldProcessor)
	{
		lua_pushnil(LUA_STATE);
		while (lua_next(LUA_STATE, index) != 0) {
			if (nullptr != fieldProcessor) fieldProcessor(Peek<string>(-2));
			lua_pop(LUA_STATE, 1);
		}
	}

	void State::PeekArray(int index, std::function<void(int index)> arrayProcessor)
	{
		lua_pushnil(LUA_STATE);
		while (lua_next(LUA_STATE, index) != 0) {
			if (nullptr != arrayProcessor) arrayProcessor(Peek<int>(-2));
			lua_pop(LUA_STATE, 1);
		}
	}

	void State::CreateTable(int arrayCount, int recordCount)
	{
		lua_createtable(LUA_STATE, arrayCount, recordCount);
	}

	void State::LogStack()
	{
		int i;
		int top = lua_gettop(LUA_STATE);
		Log("Lua stack:");
		LogIndent;
		for (i = 1; i <= top; i++) {
			int t = lua_type(LUA_STATE, i);
			switch (t) {
			case LUA_TSTRING: Log("%2d. \"%s\"", i, lua_tostring(LUA_STATE, i)); break;
			case LUA_TBOOLEAN: Log("%2d. %s", i, lua_toboolean(LUA_STATE, i) ? "true" : "false"); break;
			case LUA_TNUMBER: Log("%2d. %g", i, lua_tonumber(LUA_STATE, i)); break;
			default: Log("%2d. %s", i, lua_typename(LUA_STATE, t)); break;
			}
		}
	}

	void State::Pop(int count)
	{
		lua_pop(LUA_STATE, count);
	}

	void State::RegisterClosure(const string& name, int(*function)(lua_State *), int argNum)
	{
		lua_pushcclosure(LUA_STATE, function, argNum);
		lua_setglobal(LUA_STATE, name.data());
	}

	void State::SetFunction(const string & name, int(*function)(State *))
	{
		lua_register(LUA_STATE, name.data(), (lua_CFunction)function);
	}

}