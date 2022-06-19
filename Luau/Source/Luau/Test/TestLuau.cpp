#include "luacode.h"
#include "lua.h"
#include "lualib.h"

#include <cstdlib>
#include <cstring>
#include <string>


std::string TestLuau()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	size_t bytecodeSize = 0;
	const char* source = "print(123);";
	char* bytecode = luau_compile(source, strlen(source), NULL, &bytecodeSize);
	if(luau_load(L, "=test", bytecode, bytecodeSize, 0) != 0)
	{
		size_t len;
		const char* msg = lua_tolstring(L, -1, &len);

		std::string error(msg, len);
		lua_pop(L, 1);

		free(bytecode);
		return error;
	}
	else
	{

		auto status = lua_resume(L, NULL, 0);

		if (status != 0)
		{
			std::string error;

			if (status == LUA_YIELD)
			{
				error = "thread yielded unexpectedly";
			}
			else if (status == LUA_ERRRUN)
			{
				size_t len;
				const char* msg = lua_tolstring(L, 1, &len);

				error = std::string(msg, len);
			}

			error += "\nstacktrace:\n";
			error += lua_debugtrace(L);

			UE_LOG(LogTemp, Log, TEXT("%s"), *FString(error.c_str()));
		}

		free(bytecode);
		return status == 0 ? "succ" : "failed";
	}
}

FAutoConsoleCommand GTestLuauCommand(TEXT("luau.test"),
	TEXT("test luau"),
	FConsoleCommandDelegate::CreateLambda([]()
	{
		auto Result = TestLuau();
		UE_LOG(LogTemp, Log, TEXT("%s"), *FString(Result.c_str()))
	}));
