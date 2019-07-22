#include "App.h"
#include <stdio.h>
#include <vector>
#include <string>
//#include <MyTools/MyTools.h>
#include "RasterizeGui.h"

#ifdef EMBED_LUA
#include "MinimalLuaInterpreter.h"
#endif

void App::Init(
    HWND                    consoleHwnd, 
    HWND                    nativeHwnd, 
    ID3D11Device*           pDevice, 
    ID3D11DeviceContext*    pDeviceContext)
{
    assert(consoleHwnd && nativeHwnd && pDevice && pDeviceContext);

    ConsoleHwnd         = consoleHwnd;
    NativeHwnd          = nativeHwnd;

    state = std::make_shared<InitState>(pDevice, pDeviceContext);

    guiGears.push_back(std::make_shared<RasterizeGui>());
    for (auto& guiGear : guiGears)
    {
        guiGear->Init(state);
    }
}

void App::Main()
{
    for (auto& guiGear : guiGears)
    {
        guiGear->Render();
    }
}

void App::CleanUp()
{
    for (auto& guiGear : guiGears)
    {
        guiGear->CleanUp();
    }
}

void App::HideConsoleWindow()
{
    ShowWindow(ConsoleHwnd, SW_HIDE);
}

void App::HideNativeWindow()
{
    ImGui::GetIO().InputCharacters[0] = 0;
    if (IsNativeWindowHiden)
    {
        return;
    }

    IsNativeWindowHiden = true;
    // hide current window
    ShowWindow(NativeHwnd, SW_HIDE);

    // focus on the last window.
    HWND nextWnd = GetWindow(NativeHwnd, GW_HWNDNEXT);

    while (true)
    {
        HWND temp = GetParent(nextWnd);
        if (temp == 0) break;
        nextWnd = temp;
    }

    //SetFocus(hwndc);
    SetForegroundWindow(nextWnd);
    SetCapture(nextWnd);
    SetFocus(nextWnd);
    SetActiveWindow(nextWnd);
    EnableWindow(nextWnd, TRUE);
}

void App::ShowNativeWindow()
{
    if (IsNativeWindowHiden == false)
    {
        return;
    }

    IsNativeWindowHiden = false;
    // show this window
    ShowWindow(NativeHwnd, SW_NORMAL);
    //SetFocus(hwnd);
    SetActiveWindow(NativeHwnd);
    SetForegroundWindow(NativeHwnd);
    SetCapture(NativeHwnd);
    SetFocus(NativeHwnd);
    SetActiveWindow(NativeHwnd);
    EnableWindow(NativeHwnd, TRUE);
}
