#pragma once
#include <imgui/imgui.h>
#include <windows.h>
#include <string>
#include <d3d11.h>

namespace App
{
extern HWND ConsoleHwnd;
extern HWND NativeHwnd;
extern bool IsNativeWindowHiden;
extern bool ToBeHiden;
extern bool NextBeHiden;

extern unsigned int MainWindowHeight;
extern unsigned int MainWindowWidth;

extern ID3D11Device* pd3dDevice;

/*!
    \brief hide the console window
*/
void HideConsoleWindow();

void ToggleNativeWindow();

void HideNativeWindow();

void ShowNativeWindow();

/*!
    \brief get the path of current working place.
*/
std::string ExePath();

/*!
    \brief the main function to run imgui logics
*/
int Main();

}// namespace App
