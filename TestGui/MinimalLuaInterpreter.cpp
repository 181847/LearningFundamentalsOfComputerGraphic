#include "MinimalLuaInterpreter.h"
#ifdef WIN32
#include <windows.h>
#else
static_assert(false, "lack of required API for getting execution path.");
#endif

MinimalLuaInpterpreter::MinimalLuaInpterpreter()
{
    if (MainLuaState = luaL_newstate())
    {
        luaL_openlibs(MainLuaState);
    }

    std::vector<std::string> readyInitScripts;
    printf("lua working directory is %s\n", ExePath().c_str());
    readyInitScripts.push_back("initialize.lua");
    readyInitScripts.push_back("..\\initialize.lua");
    bool isInitSuccess = false;
    // loop all init scripts until success one.
    for (const auto& initScript : readyInitScripts)
    {
        printf("try init with %s\n", initScript.c_str());
        std::string script = initScript;
        script = ExePath() + "\\" + script;
        isInitSuccess = !(luaL_dofile(MainLuaState, script.c_str()));
        if (isInitSuccess)
        {
            isInitSuccess = true;
            break;
        }
        else
        {
            const char * errorMessage = lua_tostring(MainLuaState, -1);
            sprintf_s(StatusText, "do file failed %s", errorMessage);
            printf("DoCommand failed: %s\n", errorMessage);
        }
    }

    if (isInitSuccess)
    {
        printf("lua initialize success\n");
    }
    else
    {
        sprintf_s(StatusText, "do file failed");
        printf("load init script failed.!");
    }
}

int MinimalLuaInpterpreter::DoCommand(const char * pCommandStr)
{
    if (!MainLuaState)
    {
        sprintf_s(StatusText, "lua state is nullptr, DoCommand failed.");
        return 1;
    }
    if (!lua_pushstring(MainLuaState, pCommandStr))
    {
        sprintf_s(StatusText, "push command to lua_state failed");
        return 1;
    }

    lua_setglobal(MainLuaState, "command");


    if (luaL_dostring(MainLuaState, "CallController();"))
    {
        const char * errorMessage = lua_tostring(MainLuaState, -1);
        sprintf_s(StatusText, "do file failed %s", errorMessage);
        printf("DoCommand failed: %s\n", errorMessage);
        return 1;
    }

    sprintf_s(StatusText, "");
    return 0;
}

std::string MinimalLuaInpterpreter::ExePath()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos);
}
