#pragma once
#include <imgui.h>
#include <windows.h>

namespace App
{
extern HWND ConsoleHwnd;
extern HWND NativeHwnd;
extern bool IsNativeWindowHiden;
extern bool ToBeHiden;
extern bool NextBeHiden;

extern unsigned int MainWindowHeight;
extern unsigned int MainWindowWidth;

void HideConsoleWindow();

void ToggleNativeWindow();

void HideNativeWindow();

void ShowNativeWindow();

/*!
    \brief the main function to run imgui logics
*/
int Main();

}// namespace App
