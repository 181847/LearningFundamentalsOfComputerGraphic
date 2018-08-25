// ImGui - standalone example application for DirectX 11
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
#include "ImguiWrapImageDX11.h"

#include "App.h"
#include "../TestCasesInSuits/CommonHeaders.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")

// Data
static ID3D11Device*            g_pd3dDevice = NULL;
static ID3D11DeviceContext*     g_pd3dDeviceContext = NULL;
static IDXGISwapChain*          g_pSwapChain = NULL;
static ID3D11RenderTargetView*  g_mainRenderTargetView = NULL;

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

HRESULT CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
        return E_FAIL;

    CreateRenderTarget();

    return S_OK;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            App::MainWindowWidth = (UINT)LOWORD(lParam);
            App::MainWindowHeight = (UINT)HIWORD(lParam);
            ImGui_ImplDX11_InvalidateDeviceObjects();
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
            ImGui_ImplDX11_CreateDeviceObjects();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

ImguiWrapImageDX11 BuildATempImage()
{
    using SimplePoint = CommonEnvironment::SimplePoint;
    static_assert(sizeof(SimplePoint) == 2 * sizeof(hvector), "SimplePoint size is wrong");

    CommonEnvironment environment;
    auto pipline = environment.GetCommonPipline();
    auto pso = pipline->GetPSO();

    pso->m_cullFace = CullFace::COUNTER_CLOCK_WISE;

    // the pixel shader will not work
    // due to the imcompletation of the triangle pipeline.
    pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
        const SimplePoint* pPoint = reinterpret_cast<const SimplePoint*>(pVertex);

        return RGBA::BLUE;
        return RGBA(pPoint->m_rayIndex.m_x, pPoint->m_rayIndex.m_y, pPoint->m_rayIndex.m_z);
    };

    const Types::F32 LEFT_F(-1.0f), RIGHT_F(1.0f), BOTTOM_F(-1.0f), TOP_F(1.0f), NEAR_F(-1.0f), FAR_F(-10.0f);

    // rotate the line a little.
    Types::F32 pitch(3.14f * 3.f / 4.f), yaw(3.14f / 4.f), roll(0 * 3.14f / 3.f);

    // perspective transformation
    Transform perspect = Transform::PerspectiveOG(LEFT_F, RIGHT_F, BOTTOM_F, TOP_F, NEAR_F, FAR_F);

    Transform trs = Transform::TRS(vector3(-1.0f, 0.0f, -3.0f), vector3(pitch, yaw, roll), vector3(1.5f, 2.1f, 1.0f));

    Transform mat = perspect * trs;

    pso->m_vertexShader = [&mat, &trs, &perspect](const unsigned char * pSrcVertex, ScreenSpaceVertexTemplate * pDestV)->void {
        const SimplePoint* pSrcH = reinterpret_cast<const SimplePoint*>(pSrcVertex);
        SimplePoint* pDestH = reinterpret_cast<SimplePoint*>(pDestV);

        hvector inViewPos = trs * pSrcH->m_position;

        pDestH->m_position = perspect * inViewPos;
        //pDestH->m_position = pSrcH->m_position;
        pDestH->m_rayIndex = pSrcH->m_rayIndex;
    };

    std::vector<SimplePoint> points;
    std::vector<unsigned int> indices;
    auto meshData = GeometryBuilder::BuildGeoSphere(0.8f, 2);
    indices = meshData.m_indices;
    points.clear();
    for (const auto& vertex : meshData.m_vertices)
    {
        SimplePoint sp(vertex.m_pos.ToHvector(), vertex.m_normal.ToHvector());
        points.push_back(sp);
    }

    auto vertexBuffer = std::make_unique<F32Buffer>(points.size() * sizeof(SimplePoint));
    memcpy(vertexBuffer->GetBuffer(), points.data(), vertexBuffer->GetSizeOfByte());

    pso->m_primitiveType = PrimitiveType::TRIANGLE_LIST;
    pso->m_cullFace = CullFace::CLOCK_WISE;
    {
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(indices, vertexBuffer.get());
    }

    pso->m_fillMode = FillMode::WIREFRAME;
    // change the trs matrix and draw same cube with different instance
    trs = Transform::TRS(vector3(-0.1f, 1.4f, -3.2f), vector3(pitch, yaw + 3.14f / 3, roll), vector3(1.5f, 1.5f, 1.5f));
    {
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(indices, vertexBuffer.get());
    }

    pso->m_fillMode = FillMode::WIREFRAME;
    // change the trs matrix and draw same cube with different instance
    trs = Transform::TRS(vector3(0.4f, 0.6f, -4.0f), vector3(pitch, yaw + 3.14f / 2, roll + 3.14 / 8), vector3(0.8f, 0.8f, 0.8f));
    {
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(indices, vertexBuffer.get());
    }

    Image depthImg = ToImage(*(pipline->m_depthBuffer.get()), -1 / NEAR_F);

    ImguiWrapImageDX11 retData;
    retData.SetImageRawData(
        g_pd3dDevice, 
        pipline->m_backBuffer->GetRawData(), 
        pipline->m_backBuffer->GetWidth(), 
        pipline->m_backBuffer->GetHeight());
    
    return retData;
}


static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


int main(int, char**)
{
    // Create application window
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
    RegisterClassEx(&wc);
    HWND hwnd = CreateWindow(_T("ImGui Example"), _T("Dear ImGui DirectX11 Example"), WS_OVERLAPPEDWINDOW, 200, 100, App::MainWindowWidth, App::MainWindowHeight, NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (CreateDeviceD3D(hwnd) < 0)
    {
        CleanupDeviceD3D();
        UnregisterClass(_T("ImGui Example"), wc.hInstance);
        return 1;
    }


    // register hot key
    RegisterHotKey(NULL, 1, MOD_CONTROL, VK_BACK);
    App::ConsoleHwnd = GetConsoleWindow();
    App::NativeHwnd = hwnd;
    App::HideConsoleWindow();

    // Show the window
    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);


    ImguiWrapImageDX11 image = BuildATempImage();

    // Setup style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them. 
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple. 
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'misc/fonts/README.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state

    // Main loop
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.

        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        if (msg.message == WM_HOTKEY)
            App::ShowNativeWindow();

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        App::Main();
        ImGui::Image(image.GetSRV(), ImVec2(image.GetWidth(), image.GetHeight()));
        ImGui::End();

        // Rendering
        ImGui::Render();
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, (float*)&clear_color);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0); // Present with vsync
                                     //g_pSwapChain->Present(0, 0); // Present without vsync
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    UnregisterHotKey(hwnd, 1);

    CleanupDeviceD3D();
    DestroyWindow(hwnd);
    UnregisterClass(_T("ImGui Example"), wc.hInstance);

    return 0;
}
