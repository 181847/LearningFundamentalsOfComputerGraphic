#include "App.h"
#include <stdio.h>
#include <vector>
#include <string>
#include <MyTools/MyTools.h>
#include "ImguiWrapImageDX11.h"
#include "../TestCasesInSuits/BaseToolForCaseAndSuit.h"

extern "C"
{
#include <Lua/Lua-5.3.4/src/lua.h>
#include <Lua/Lua-5.3.4/src/lauxlib.h>
#include <Lua/Lua-5.3.4/src/lualib.h>
}

namespace App
{

HWND ConsoleHwnd = 0;
HWND NativeHwnd = 0;
bool IsNativeWindowHiden = false;
bool ToBeHiden = false;
bool NextBeHiden = false;

unsigned int MainWindowWidth = 720;
unsigned int MainWindowHeight = 720;

ID3D11Device* pd3dDevice = nullptr;

char StatusText[255];

// the render image for application display.
ImguiWrapImageDX11 MainImage;

// preparations for pipline
CaseForPipline HelpPiplineCase("a help struct for pipline");
auto MainPipline = HelpPiplineCase.GetCommonPipline();

struct MinimalLuaInpterpreter
{
public:
    lua_State* MainLuaState = nullptr;

    MinimalLuaInpterpreter()
    {
        if (MainLuaState = luaL_newstate())
        {
            luaL_openlibs(MainLuaState);
        }

        std::vector<std::string> readyInitScripts;
        printf("lua workding directory is %s\n", ExePath().c_str());
        readyInitScripts.push_back("initialize.lua");
        readyInitScripts.push_back("..\\initialize.lua");
        bool isInitSuccess = false;
        // loop all init scripts until success one.
        for (const auto& initScript : readyInitScripts)
        {
            printf("try init with %s\n", initScript.c_str());
            std::string script = initScript;
            script = ExePath() + "\\" +  script;
            isInitSuccess = ! (luaL_dofile(MainLuaState, script.c_str()));
            if (isInitSuccess)
            {
                isInitSuccess = true;
                break;
            }
            else
            {
                const char * errorMessage = lua_tostring(MainLuaState, -1);
                sprintf_s(StatusText, "do file failed %s", errorMessage);
                printf("DoCommand failed: %s\n", errorMessage);
            }
        }
        
        if (isInitSuccess)
        {
            printf("lua initialize success\n");
        }
        else
        {
            sprintf_s(StatusText, "do file failed");
            printf("load init script failed.!");
        }
    }
    MinimalLuaInpterpreter(const MinimalLuaInpterpreter&) = delete;
    MinimalLuaInpterpreter(const MinimalLuaInpterpreter&&) = delete;
    MinimalLuaInpterpreter& operator = (const MinimalLuaInpterpreter&) = delete;

    /*!
        \brief push one string and execute the corresponding command
        \return 1 for error
    */
    int DoCommand(const char * pCommandStr)
    {
        if (!MainLuaState)
        {
            sprintf_s(StatusText, "lua state is nullptr, DoCommand failed.");
            return 1;
        }
        if (!lua_pushstring(MainLuaState, pCommandStr))
        {
            sprintf_s(StatusText, "push command to lua_state failed");
            return 1;
        }

        lua_setglobal(MainLuaState, "command");

        
        if (luaL_dostring(MainLuaState, "CallController();"))
        {
            const char * errorMessage = lua_tostring(MainLuaState, -1);
            sprintf_s(StatusText, "do file failed %s", errorMessage);
            printf("DoCommand failed: %s\n", errorMessage);
            return 1;
        }

        sprintf_s(StatusText, "");
        return 0;
    }
};

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

void ToggleNativeWindow()
{
    if (IsNativeWindowHiden)
    {
        ShowNativeWindow();
    }
    else
    {
        HideNativeWindow();
    }
}


/*!
    \brief render the main image.
*/
void RenderMainImage()
{
    // type aliases
    using SimplePoint               = CaseForPipline::SimplePoint;
    using PSIn                      = CaseForPipline::PSIn;
    using VSOut                     = CaseForPipline::VSOut;
    using ObjectInstance            = CaseForPipline::ObjectInstance;
    using ConstantBufferForInstance = CaseForPipline::ConstantBufferForInstance;
    using ConstantBufferForCamera   = CaseForPipline::ConstantBufferForCamera;
    using MaterialBuffer            = CaseForPipline::MaterialBuffer;
    static_assert(sizeof(SimplePoint) == 2 * sizeof(hvector), "SimplePoint size is wrong");

    std::shared_ptr<CommonClass::PiplineStateObject> PSO = MainPipline->GetPSO();
    PSO->m_vertexLayout.vertexShaderInputSize = sizeof(SimplePoint);
    PSO->m_vertexLayout.pixelShaderInputSize = sizeof(PSIn);

    // perspective transformation
    const Types::F32 V_LEFT(-1.0f), V_RIGHT(1.0f), V_BOTTOM(-1.0f), V_TOP(1.0f), V_NEAR(-1.0f), V_FAR(-10.0f);
    Transform perspect = Transform::PerspectiveOG(V_LEFT, V_RIGHT, V_BOTTOM, V_TOP, V_NEAR, V_FAR);

    Types::F32 pitch(3.14f * 3.f / 4.f), yaw(3.14f / 4.f), roll(0.f * 3.14f / 3.f);
    std::array<ObjectInstance, 3> objInstances;
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
    objInstances[2].m_scale = vector3(0.8f, 0.8f, 0.8f);

    std::array<ConstantBufferForInstance, 3> instanceBuffers;
    for (unsigned int i = 0; i < instanceBuffers.size(); ++i)
    {
        instanceBuffers[i].m_toWorld = Transform::TRS(objInstances[i].m_position, objInstances[i].m_rotation, objInstances[i].m_scale);
        instanceBuffers[i].m_toWorldInverse = Transform::InverseTRS(objInstances[i].m_position, objInstances[i].m_rotation, objInstances[i].m_scale);
        instanceBuffers[i].m_material.m_diffuse = RGB::RED * 0.5f;
        instanceBuffers[i].m_material.m_shiness = 1024.0f;
        instanceBuffers[i].m_material.m_fresnelR0 = MaterialBuffer::FresnelR0_byReflectionIndex(4);
    }// end for

    std::wstring pictureIndex = L"023";
    CameraFrame cameraFrames(vector3(0.0f, 0.0f, 1.0f) * 3.0f /* location */, vector3(0.0f, 0.0f, 0.0f) /* target */);
    ConstantBufferForCamera cameraBuffer;
    cameraBuffer.m_toCamera = cameraFrames.WorldToLocal();
    cameraBuffer.m_toCameraInverse = cameraFrames.LocalToWorld();
    cameraBuffer.m_camPos = cameraFrames.GetOrigin();
    cameraBuffer.m_project = perspect;
    cameraBuffer.m_numLights = 1;
    cameraBuffer.m_ambientColor = RGB::RED * RGB(0.05f, 0.05f, 0.05f);
    cameraBuffer.m_lights[0] = { vector3(5.0f, -1.0f, 2.0f), RGB(1.0f, 1.0f, 0.5f) };

    ConstantBufferForInstance instanceBufAgent;// agent buffer for setting instance data
                                               // set VS and PS
    PSO->m_vertexShader = HelpPiplineCase.GetVertexShaderWithVSOut(instanceBufAgent, cameraBuffer);
    PSO->m_pixelShader = HelpPiplineCase.GetPixelShaderWithPSIn(instanceBufAgent, cameraBuffer);

    // build mesh data
    std::vector<SimplePoint>  vertices;
    std::vector<unsigned int> indices;
    //auto meshData = GeometryBuilder::BuildCylinder(0.6f, 0.8f, 0.8f, 32, 3, true);
    auto meshData = GeometryBuilder::BuildGeoSphere(0.8f, 2);
    indices = meshData.m_indices;
    vertices.clear();
    for (const auto& vertex : meshData.m_vertices)
    {
        vertices.push_back(SimplePoint(vertex.m_pos.ToHvector(), vertex.m_normal.ToHvector(0.0f)));
    }
    auto vertexBuffer = std::make_unique<F32Buffer>(vertices.size() * sizeof(decltype(vertices)::value_type));
    memcpy(vertexBuffer->GetBuffer(), vertices.data(), vertexBuffer->GetSizeOfByte());


    PSO->m_primitiveType = PrimitiveType::TRIANGLE_LIST;
    PSO->m_cullFace = CullFace::CLOCK_WISE;
    PSO->m_fillMode = FillMode::SOLIDE;
    instanceBufAgent = instanceBuffers[0];
    {
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        MainPipline->DrawInstance(indices, vertexBuffer.get());
    }

    PSO->m_fillMode = FillMode::SOLIDE;
    instanceBufAgent = instanceBuffers[1];
    {
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        MainPipline->DrawInstance(indices, vertexBuffer.get());
    }

    PSO->m_fillMode = FillMode::WIREFRAME;
    instanceBufAgent = instanceBuffers[2];
    {
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        MainPipline->DrawInstance(indices, vertexBuffer.get());
    }

    MainImage.SetImageRawData(pd3dDevice, MainPipline->m_backBuffer->GetRawData(), MainPipline->m_backBuffer->GetWidth(), MainPipline->m_backBuffer->GetHeight());
}

int Main()
{
    static bool show_demo_window = true;
    static bool show_another_window = false;
    static bool show_app_main_window = true;
    static float color[3];

    static float f = 0.0f;
    static int counter = 0;

    static MinimalLuaInpterpreter LuaInterpreter;

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

    // render the main image each frame.
    RenderMainImage();

    // put other widget here.
    ImGui::Image(App::MainImage.GetSRV(), ImVec2(static_cast<float>(MainImage.m_width), static_cast<float>(MainImage.m_height)));

    // status bar
    ImGui::Text(StatusText);
    ImGui::End();
    return 0;
}

}// namespace App
