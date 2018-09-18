#include "App.h"
#include <stdio.h>
#include <vector>
#include <string>
#include <MyTools/MyTools.h>
#include "ImguiWrapImageDX11.h"
#include "../TestCasesInSuits/BaseToolForCaseAndSuit.h"
#include "MinimalLuaInterpreter.h"

namespace App
{

HWND            ConsoleHwnd         = 0;
HWND            NativeHwnd          = 0;
bool            IsNativeWindowHiden = false;

unsigned int    MainWindowWidth     = 720;
unsigned int    MainWindowHeight    = 720;

ID3D11Device*   pd3dDevice          = nullptr;
ID3D11DeviceContext* pd3dDeviceContex = nullptr;

char            StatusText[255];

MinimalLuaInpterpreter MainLuaInterpreter;

// type aliases
using SimplePoint               = CaseForPipline::SimplePoint;
using PSIn                      = CaseForPipline::PSIn;
using VSOut                     = CaseForPipline::VSOut;
using ObjectInstance            = CaseForPipline::ObjectInstance;
using ConstantBufferForInstance = CaseForPipline::ConstantBufferForInstance;
using ConstantBufferForCamera   = CaseForPipline::ConstantBufferForCamera;
using MaterialBuffer            = CaseForPipline::MaterialBuffer;

using CommonRenderingBuffer = CaseForPipline::CommonRenderingBuffer;
ImguiWrapImageDX11                                  MainImage;  // the render image for application display.
CaseForPipline                                      HelpPiplineCase("a help struct for pipline");   // preparations for pipline
std::unique_ptr<Pipline>                            MainPipline;
std::shared_ptr<CommonClass::PiplineStateObject>    PSO;
CommonRenderingBuffer                               renderingBuffer;
ConstantBufferForInstance                           instanceBufAgent;// agent buffer for setting instance data
std::shared_ptr<Texture>                            textureAgent;// texture agent for pixel shader.
CaseForPipline::PixelShaderSig                      pixelShaderNoTexture;
CaseForPipline::PixelShaderSig                      pixelShaderTexture;
CaseForPipline::PixelShaderSig                      pixelShaderNoiseNormalTexture;
std::shared_ptr<Texture>                            texture1 = std::make_shared<Texture>();
std::shared_ptr<Texture>                            texture2 = std::make_shared<Texture>();
std::shared_ptr<Texture>                            texture3 = std::make_shared<Texture>();

void Init(HWND consoleHwnd, HWND nativeHwnd, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
    assert(consoleHwnd && nativeHwnd && pDevice && pDeviceContext);

    ConsoleHwnd         = consoleHwnd;
    NativeHwnd          = nativeHwnd;
    pd3dDevice          = pDevice;
    pd3dDeviceContex    = pDeviceContext;

    renderingBuffer.Init();

    MainPipline = HelpPiplineCase.GetCommonPipline();
    PSO = MainPipline->GetPSO();

    // load textures
    texture1->LoadFile("d:\\GitHub\\C_C++\\LearningFundamentalsOfComputerGraphic\\build\\OutputTestImage\\StbImg\\noiseVector3_03.png");
    texture2->LoadFile("d:\\GitHub\\C_C++\\LearningFundamentalsOfComputerGraphic\\build\\OutputTestImage\\StbImg\\noiseVector3_06.png");
    texture3->LoadFile("d:\\picture\\Maria\\sideShowToy\\bloodborne-lady-maria-of-the-astral-clocktower-statue-prime1-studio-902974-04.jpg");
    textureAgent = texture3;

    // pre store pixel shaders.
    pixelShaderNoTexture            = HelpPiplineCase.GetPixelShaderWithPSIn(instanceBufAgent, renderingBuffer.cameraBuffer);
    pixelShaderTexture              = HelpPiplineCase.GetPixelShaderWithPSInAndTexture(instanceBufAgent, renderingBuffer.cameraBuffer, textureAgent);
    pixelShaderNoiseNormalTexture   = HelpPiplineCase.GetPixelShaderWithNoiseBumpMap(instanceBufAgent, renderingBuffer.cameraBuffer, textureAgent);

    // set VS and PS
    PSO->m_vertexShader = HelpPiplineCase.GetVertexShaderWithVSOut(instanceBufAgent, renderingBuffer.cameraBuffer);
    //PSO->m_pixelShader  = pixelShaderNoiseNormalTexture;
    PSO->m_pixelShader  = pixelShaderNoiseNormalTexture;
    PSO->m_vertexLayout.vertexShaderInputSize = sizeof(SimplePoint);
    PSO->m_vertexLayout.pixelShaderInputSize  = sizeof(PSIn);
}

std::string ExePath() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos);
}

void HideConsoleWindow()
{
    ShowWindow(ConsoleHwnd, SW_HIDE);
}

void HideNativeWindow()
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

void ShowNativeWindow()
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

/*!
    \brief draw Imgui widgets and setting new data from the widget
    \return true for some data has been updated.
*/
bool ImguiUpdateRenderData()
{
    bool isDirtyData = false;

    // widget for camera location
    if (ImGui::SliderFloat3("camera location", renderingBuffer.cameraFrame.m_origin.m_arr, -10.0f, 10.0f))
    {
        renderingBuffer.RebuildCameraBuffer();
        isDirtyData = true;
    }

    if (ImGui::SliderFloat3("light position", renderingBuffer.cameraBuffer.m_lights[0].m_position.m_arr, -10.0f, 10.0f))
    {
        isDirtyData = true;
    }

    if (ImGui::SliderFloat("first object shiness", &renderingBuffer.objInstances[0].m_material.m_shiness, 0.0f, 20.0f))
    {
        renderingBuffer.RebuildInstanceBuffer();
        isDirtyData = true;
    }

    return isDirtyData;
}

/*!
    \brief render the main image.
*/
void RenderMainImage()
{
    MainPipline->ClearBackBuffer(RGBA::WHITE);

    PSO->m_primitiveType = PrimitiveType::TRIANGLE_LIST;
    PSO->m_cullFace = CullFace::CLOCK_WISE;
    PSO->m_fillMode = FillMode::SOLIDE;
    const auto& mesh = renderingBuffer.prebuildMeshData[CommonRenderingBuffer::M_GEOSPHERE];

    instanceBufAgent = renderingBuffer.instanceBuffers[0];
    {
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        MainPipline->DrawInstance(mesh.indices, mesh.vertexBuffer.get());
    }

    PSO->m_fillMode = FillMode::SOLIDE;
    instanceBufAgent = renderingBuffer.instanceBuffers[1];
    {
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        MainPipline->DrawInstance(mesh.indices, mesh.vertexBuffer.get());
    }

    PSO->m_fillMode = FillMode::WIREFRAME;
    instanceBufAgent = renderingBuffer.instanceBuffers[2];
    {
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        MainPipline->DrawInstance(mesh.indices, mesh.vertexBuffer.get());
    }

    // set image data.
    if (MainImage.m_isValide)
    {
        // only update resource when image is valide
        MainImage.UpdateImageRowData(pd3dDeviceContex, MainPipline->m_backBuffer->GetRawData(), MainPipline->m_backBuffer->GetWidth(), MainPipline->m_backBuffer->GetHeight());
    }
    else
    {
        // else set a new image data, in the mean time, create all ID3D11* resources.
        MainImage.SetImageRawData(pd3dDevice, MainPipline->m_backBuffer->GetRawData(), MainPipline->m_backBuffer->GetWidth(), MainPipline->m_backBuffer->GetHeight());
    }// end else
}// end Render

int Main()
{
    static bool show_demo_window = true;
    static bool show_another_window = false;
    static bool show_app_main_window = true;
    static float color[3];

    static float f = 0.0f;
    static int counter = 0;

    // force the main imgui window fill the native window.
    ImGuiWindowFlags mainAppWndFlags = /*ImGuiWindowFlags_NoResize | */ImGuiWindowFlags_NoTitleBar;
    //ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f)); 
    //ImGui::SetNextWindowSize(ImVec2(static_cast<float>(MainWindowWidth), static_cast<float>(MainWindowHeight)));
    ImGui::Begin("Hello, world!", &show_app_main_window/* indicating no close */, mainAppWndFlags);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    static char InputBuf[255];
    if (ImGui::InputText("input your text", InputBuf, sizeof(InputBuf)))
    {
        // deal with command input here.

    }

    if (ImguiUpdateRenderData())
    {
        // render the main image each frame.
        RenderMainImage();
    }


    static char InputImagePath[256];
    // if rendering has complete at least once, draw a button that can save this image,
    // save process is done by the back buffer of MainPipline.
    if (MainImage.m_isValide)
    {
        ImGui::InputText("save path", InputImagePath, sizeof(InputImagePath));
        if (ImGui::Button("save current image"))
        {
            MainPipline->m_backBuffer->SaveTo("");
        }
    }


    // put other widget here.
    ImGui::Image(App::MainImage.GetSRV(), ImVec2(static_cast<float>(MainImage.m_width), static_cast<float>(MainImage.m_height)));

    // status bar
    ImGui::Text("app status: %s", StatusText);
    ImGui::Text("lua status: %s", MainLuaInterpreter.StatusText);
    ImGui::End();
    return 0;
}


}// namespace App
