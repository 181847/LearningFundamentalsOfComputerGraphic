#pragma once
#include <type_traits>

/*
    \brief define some useful marcos for Debug
*/

#ifdef WIN32

#include <intrin.h>

/*!
    \brief put an explicit break point to debug program
*/
#define PUT_BREAK_POINT do { __debugbreak(); } while(0)

#define BREAK_POINT_IF(expr) do { if ((expr)) {__debugbreak();} }while(0)

/*!
    \brief put a break point and enable it on some where else.
    \param condition additional parameter that user can use, if this is set to false, the break point will not be triggered.
            defaul to be true.
    \return whether the break point is been hit.
    the break point only be triggered when DEBUG_CONF::ENABLE_CLIENT > 0 && DEBUG_CONF::Active == true && condition == true
    and DEBUG_CONF::Active(can be static boolean).

    Usage:
    1. define a struct for configuration, you will need *.h and *.cpp file because the DEBUG_CONF::Active should be a static member
      which should be implemented in the *.cpp file.
    ||||  a sample of a struct ||||
struct DE_CLIENT_SAMPLE
{
public:
    static bool Active;
    enum {ENABLE_CLIENT = 1}; // enable the ability of this configuration.
};
bool DE_CLIENT_SAMPLE::Active = true; // break on the client location.
    ||||  a sample of a struct ||||

    2. in the code that you want to put a break point
    ...other codes...
    DebugClient<DEBUG_CONF>();
    ...other codes...
    
    3. enable the break point when you want to
    DEBUG_CONF::Active = true;
    OR use it with DebugGuard, more detail you can reference to the comment in the head of DebugGuard
    {
        DebugGuard<DEBUG_CONF> guard;
        ...
        ... the codes that may trigger the DebugClient ...
        ...
    }
    
*/
template<typename DEBUG_CONF>
typename std::enable_if_t<DEBUG_CONF::ENABLE_CLIENT, bool>
DebugClient(bool condition = true)
{
    if (DEBUG_CONF::Active && condition)
    {
        PUT_BREAK_POINT;
        return true;
    }
    return false;
}

#pragma region macros about DebugClient, here replace template function DebugClient<>() with macro.

#define DEBUG_CLIENT_WIDTH_CONFIG(DEBUG_CONFIG) DEBUG_CLIENT_WIDTH_CONFIG_CONDITION(DEBUG_CONFIG, true)

#define DEBUG_CLIENT_WIDTH_CONFIG_CONDITION(DEBUG_CONFIG, condition) do {\
    if (DEBUG_CONFIG::Active && (condition))\
    {\
        PUT_BREAK_POINT;\
    }\
}while(0)

#define ID_OF_DEBUG_CLIENT(x) x

#define GET_DEBUG_CLIENT_MACRO(_1, _2, NAME, ...) NAME

/*!
    \brief how to used these macros:
    DEBUG_CLIENT(DebugClientStruct);
    DEBUG_CLIENT(DebugConfStruct, true/false);

1. define a struct for configuration, you will need *.h and *.cpp file because the DEBUG_CONF::Active should be a static member
    which should be implemented in the *.cpp file.
    ||||  a sample of a struct ||||
    struct DE_CLIENT_SAMPLE
    {
    public:
    static bool Active;
    enum {ENABLE_CLIENT = 1}; // enable the ability of this configuration.
    };
    bool DE_CLIENT_SAMPLE::Active = true; // break on the client location.
    ||||  a sample of a struct ||||
2. use macros to reference to those struct like
    DEBUG_CLIENT(DE_CLIENT_SAMPLE);                // always hit the break point if DE_CLIENT_SAMPLE::Active is true.
    DEBUG_CLIENT(DE_CLIENT_SAMPLE, condition); // hit the break point if DE_CLIENT_SAMPLE::Active and condition is both true.
PS.
    you can use DebugGuard<> to help set DE_CLIENT_SAMPLE::Active.
*/
#define DEBUG_CLIENT(...) ID_OF_DEBUG_CLIENT(GET_DEBUG_CLIENT_MACRO(__VA_ARGS__, DEBUG_CLIENT_WIDTH_CONFIG_CONDITION, DEBUG_CLIENT_WIDTH_CONFIG)(__VA_ARGS__))
#pragma endregion
// end macros about DebugClient,


/*!
    \brief this struct is for assiting DebugClient.
    which will set DEBUG_CONF::Active to true in the DebugGuard's scope,
    and set DEBUG_CONF::Active to false the DebugGuard is destoried.

    Usage(we using the debug configuration struct form DebugClient sample in it's comments: DE_CLIENT_SAMPLE)
    {
        DebugGuard<DE_CLIENT_SAMPLE> guard;

        // do all the stuff here that may trigger the break point.
    }
*/
template<typename DEBUG_CONF>
class DebugGuard
{
public:
    DebugGuard()
    {
        DEBUG_CONF::Active = true;
    }
    DebugGuard(const DebugGuard&) = delete;
    DebugGuard& operator = (const DebugGuard&) = delete;
    ~DebugGuard()
    {
        DEBUG_CONF::Active = false;
    }
};


#endif
