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

extern unsigned int MainWindowHeight;
extern unsigned int MainWindowWidth;

extern ID3D11Device* pd3dDevice;

/*!
    \brief initialize the applications.
*/
void Init(HWND consoleHwnd, HWND nativeHwnd, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);

/*!
    \brief hide the console window
*/
void HideConsoleWindow();

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
