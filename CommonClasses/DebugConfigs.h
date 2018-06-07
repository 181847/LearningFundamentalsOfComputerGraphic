#pragma once
#include "DebugHelpers.h"

/*!
    \brief DebugConfigs.h is used to construct debug configurations for DebugClient
    who is a template function and need a static member to indicate whether to break or not.
*/

/*!
    \brief this debug client configuration is for error analysis in line clipping function.
    should be used in "Pipline.cpp" about line clipping in homogenous clipping space
*/
class DEBUG_CLIENT_CONF_LINE_CLIP_ERROR_ANALYSIS
{
public:
    /*!
        \brief whether put break point in the run time.
    */
    static bool Active;

    enum 
    {
        /*!
           \brief this enum number is used to active the debug client
           if this is set to NOT zero, it means disable it,
           and all the places that use this debug client configuration will cause
           an compile error.
        */
        ENABLE_CLIENT = 1
    };
};

/*!
    \brief the debug configurations of triangle rasterization.
*/
class DEBUG_CLIENT_CONF_TRIANGL
{
public:
    /*!
    \brief whether put break point in the run time.
    */
    static bool Active;

    enum
    {
        /*!
        \brief this enum number is used to active the debug client
        if this is set to NOT zero, it means disable it,
        and all the places that use this debug client configuration will cause
        an compile error.
        */
        ENABLE_CLIENT = 1
    };
};
