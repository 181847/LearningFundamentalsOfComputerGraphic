/*!
    \brief a minimal lua interpreter, very poor functionalities.
    \time 2018/9/8
    \author liuyang
*/
#pragma once
#include <vector>

extern "C"
{
#include <Lua/Lua-5.3.4/src/lua.h>
#include <Lua/Lua-5.3.4/src/lauxlib.h>
#include <Lua/Lua-5.3.4/src/lualib.h>
}

/*!
    \brief a minimal lua interpreter.
*/
struct MinimalLuaInpterpreter
{
protected:
    /*!
        \brief main state
    */
    lua_State* MainLuaState = nullptr;

public:
    /*!
        \brief store lastest error message.
    */
    char StatusText[256];

    MinimalLuaInpterpreter();
    MinimalLuaInpterpreter(const MinimalLuaInpterpreter&) = delete;
    MinimalLuaInpterpreter(const MinimalLuaInpterpreter&&) = delete;
    MinimalLuaInpterpreter& operator = (const MinimalLuaInpterpreter&) = delete;

    /*!
        \brief push one string and execute the corresponding command
        \return 1 for error
    */
    int DoCommand(const char * pCommandStr);

    /*!
        \brief get the application executtion path, this path is for loading files.
    */
    std::string ExePath();
};