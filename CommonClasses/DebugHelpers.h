#pragma once

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


#endif
