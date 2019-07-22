#pragma once
#include <windows.h>
#include <string>
#include <d3d11.h>
#include "imgui/imgui.h"
#include "ImguiWrapImageDX11.h"
#include "../TestCasesInSuits/BaseToolForCaseAndSuit.h"
#include "AbstractGui.h"

class App
{
public:
    HWND            ConsoleHwnd = 0;
    HWND            NativeHwnd = 0;
    bool            IsNativeWindowHiden = false;

    unsigned int    MainWindowWidth = 720;
    unsigned int    MainWindowHeight = 720;

    std::shared_ptr<InitState> state;

    std::vector<std::shared_ptr<AbstractGui>> guiGears;

public:
    /*!
        \brief initialize the applications.
    */
    void Init(HWND consoleHwnd, HWND nativeHwnd, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
    void Main();
    void CleanUp();

    void HideConsoleWindow();
    void HideNativeWindow();
    void ShowNativeWindow();

};// class App
