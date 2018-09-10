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

ImguiWrapImageDX11                                  MainImage;  // the render image for application display.
CaseForPipline                                      HelpPiplineCase("a help struct for pipline");   // preparations for pipline
auto                                                MainPipline = HelpPiplineCase.GetCommonPipline();
std::shared_ptr<CommonClass::PiplineStateObject>    PSO = MainPipline->GetPSO();
std::vector<SimplePoint>                            MainVertices;
std::unique_ptr<F32Buffer>                          MainVertexBuffer;
std::vector<unsigned int>                           MainIndices;
std::array<ObjectInstance, 3>                       objInstances;
std::array<ConstantBufferForInstance, 3>            instanceBuffers;
ConstantBufferForInstance                           instanceBufAgent;// agent buffer for setting instance data
CameraFrame                                         cameraFrames(vector3(0.0f, 0.0f, 1.0f) * 3.0f /* location */, vector3(0.0f, 0.0f, 0.0f) /* target */);
ConstantBufferForCamera                             cameraBuffer;

void UpdateCameraBuffer()
{
    // perspective transformation
    const Types::F32 V_LEFT(-1.0f), V_RIGHT(1.0f), V_BOTTOM(-1.0f), V_TOP(1.0f), V_NEAR(-1.0f), V_FAR(-10.0f);
    Transform perspect = Transform::PerspectiveOG(V_LEFT, V_RIGHT, V_BOTTOM, V_TOP, V_NEAR, V_FAR);
    cameraBuffer.m_toCamera = cameraFrames.WorldToLocal();
    cameraBuffer.m_toCameraInverse = cameraFrames.LocalToWorld();
    cameraBuffer.m_camPos = cameraFrames.m_origin;
    cameraBuffer.m_project = perspect;
    cameraBuffer.m_ambientColor = RGB::RED * RGB(0.05f, 0.05f, 0.05f);
    cameraBuffer.m_numLights = 1;
    cameraBuffer.m_lights[0] = { vector3(5.0f, -1.0f, 2.0f), RGB(1.0f, 1.0f, 0.5f) };
}

void Init(HWND consoleHwnd, HWND nativeHwnd, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
    assert(consoleHwnd && nativeHwnd && pDevice && pDeviceContext);

    ConsoleHwnd = consoleHwnd;
    NativeHwnd = nativeHwnd;
    pd3dDevice = pDevice;
    pd3dDeviceContex = pDeviceContext;

    //auto meshData = GeometryBuilder::BuildCylinder(0.6f, 0.8f, 0.8f, 32, 3, true);
    auto meshData = GeometryBuilder::BuildGeoSphere(0.8f, 2);
    MainIndices = meshData.m_indices;
    MainVertices.clear();
    for (const auto& vertex : meshData.m_vertices)
    {
        MainVertices.push_back(CaseForPipline::SimplePoint(vertex.m_pos.ToHvector(), vertex.m_normal.ToHvector(0.0f)));
    }
    MainVertexBuffer = std::make_unique<F32Buffer>(MainVertices.size() * sizeof(decltype(MainVertices)::value_type));
    memcpy(MainVertexBuffer->GetBuffer(), MainVertices.data(), MainVertexBuffer->GetSizeOfByte());

    // initialize instances data
    Types::F32 pitch(3.14f * 3.f / 4.f), yaw(3.14f / 4.f), roll(0.f * 3.14f / 3.f);
    // 1
    objInstances[0].m_position = vector3(0.0f, 0.0f, 0.0f);
    objInstances[0].m_rotation = vector3(0, 0, 0);
    objInstances[0].m_scale = vector3(1.5f, 2.1f, 1.0f);
    // 2
    objInstances[1].m_position = vector3(1.0f, 1.4f, 0.0f);
    objInstances[1].m_rotation = vector3(pitch, yaw + 3.14f / 3, roll);
    objInstances[1].m_scale = vector3(1.5f, 1.5f, 1.5f);
    // 3
    objInstances[2].m_position = vector3(-1.0f, 2.8f, 0.0f);
    objInstances[2].m_rotation = vector3(pitch, yaw + 3.14f / 2.f, roll + 3.14f / 8.f);
    objInstances[2].m_scale = vector3(1.8f, 1.8f, 1.8f);
    // initialized instances buffer
    for (unsigned int i = 0; i < instanceBuffers.size(); ++i)
    {
        instanceBuffers[i].m_toWorld = Transform::TRS(objInstances[i].m_position, objInstances[i].m_rotation, objInstances[i].m_scale);
        instanceBuffers[i].m_toWorldInverse = Transform::InverseTRS(objInstances[i].m_position, objInstances[i].m_rotation, objInstances[i].m_scale);
        instanceBuffers[i].m_material.m_diffuse = RGB::RED * 0.5f;
        instanceBuffers[i].m_material.m_shiness = 1024.0f;
        instanceBuffers[i].m_material.m_fresnelR0 = MaterialBuffer::FresnelR0_byReflectionIndex(4);
    }// end for

    // camera settings.
    UpdateCameraBuffer();

    // set VS and PS
    PSO->m_vertexShader = HelpPiplineCase.GetVertexShaderWithVSOut(instanceBufAgent, cameraBuffer);
    PSO->m_pixelShader  = HelpPiplineCase.GetPixelShaderWithPSIn(instanceBufAgent, cameraBuffer);
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
    if (ImGui::DragFloat3("camera location", cameraFrames.m_origin.m_arr, 0.001f, -10.0f, 10.0f))
    {
        cameraFrames.RebuildFrameDetail();
        UpdateCameraBuffer();
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
    instanceBufAgent = instanceBuffers[0];
    {
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        MainPipline->DrawInstance(MainIndices, MainVertexBuffer.get());
    }

    PSO->m_fillMode = FillMode::SOLIDE;
    instanceBufAgent = instanceBuffers[1];
    {
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        MainPipline->DrawInstance(MainIndices, MainVertexBuffer.get());
    }

    PSO->m_fillMode = FillMode::WIREFRAME;
    instanceBufAgent = instanceBuffers[2];
    {
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        MainPipline->DrawInstance(MainIndices, MainVertexBuffer.get());
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


    // put other widget here.
    ImGui::Image(App::MainImage.GetSRV(), ImVec2(static_cast<float>(MainImage.m_width), static_cast<float>(MainImage.m_height)));

    // status bar
    ImGui::Text("app status: %s", StatusText);
    ImGui::Text("lua status: %s", MainLuaInterpreter.StatusText);
    ImGui::End();
    return 0;
}


}// namespace App
