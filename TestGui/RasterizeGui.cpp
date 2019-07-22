#include "RasterizeGui.h"
#include "imgui/imgui.h"
#include "ImguiWrapImageDX11.h"

void RasterizeGui::Init(std::shared_ptr<InitState> state)
{
    AbstractGui::Init(state);

    renderingBuffer.Init();

    MainPipline = HelpPiplineCase.GetCommonPipline();
    PSO = MainPipline->GetPSO();

    // load textures
    //texture1->LoadFile("d:\\GitHub\\C_C++\\LearningFundamentalsOfComputerGraphic\\build\\OutputTestImage\\StbImg\\noiseVector3_03.png");
    //texture2->LoadFile("d:\\GitHub\\C_C++\\LearningFundamentalsOfComputerGraphic\\build\\OutputTestImage\\StbImg\\noiseVector3_06.png");
    texture3->LoadFile("d:\\picture\\Maria\\sideShowToy\\bloodborne-lady-maria-of-the-astral-clocktower-statue-prime1-studio-902974-04.jpg");
    textureAgent = texture3;

    // pre store pixel shaders.
    pixelShaderNoTexture = HelpPiplineCase.GetPixelShaderWithPSIn(instanceBufAgent, renderingBuffer.cameraBuffer);
    pixelShaderTexture = HelpPiplineCase.GetPixelShaderWithPSInAndTexture(instanceBufAgent, renderingBuffer.cameraBuffer, textureAgent);
    pixelShaderNoiseNormalTexture = HelpPiplineCase.GetPixelShaderWithNoiseBumpMap(instanceBufAgent, renderingBuffer.cameraBuffer, textureAgent);

    // set VS and PS
    PSO->m_vertexShader = HelpPiplineCase.GetVertexShaderWithVSOut(instanceBufAgent, renderingBuffer.cameraBuffer);
    //PSO->m_pixelShader  = pixelShaderNoiseNormalTexture;
    PSO->m_pixelShader = pixelShaderNoTexture;
    PSO->m_vertexLayout.vertexShaderInputSize = sizeof(SimplePoint);
    PSO->m_vertexLayout.pixelShaderInputSize = sizeof(PSIn);
}

void RasterizeGui::Render()
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
    static char InputImageName[256] = "savePicture";
    // if rendering has complete at least once, draw a button that can save this image,
    // save process is done by the back buffer of MainPipline.
    if (MainImage.m_isValide)
    {
        // init saved path
        if (InputImagePath[0] == 0)
        {
            strcpy_s(InputImagePath, ExePath().c_str());
        }

        ImGui::InputText("save path", InputImagePath, sizeof(InputImagePath));

        if (ImGui::InputText(".png, save file name", InputImageName, sizeof(InputImageName), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            std::string fullPath = std::string(InputImagePath) + "\\" + InputImageName + ".png";

            try
            {
                MainPipline->m_backBuffer->SaveTo(fullPath);
                std::string resultMsg = "save success: ";
                resultMsg += InputImageName;
                strcpy_s(StatusText, resultMsg.c_str());
            }
            catch (std::exception e)
            {
                memset(StatusText, 0, sizeof(StatusText));
                strcpy_s(StatusText, e.what());
            }
        }
    }


    // put other widget here.
    ImGui::Image(MainImage.GetSRV(), ImVec2(static_cast<float>(MainImage.m_width), static_cast<float>(MainImage.m_height)));

    // status bar
    ImGui::Text("app status: %s", StatusText);

#ifdef EMBED_LUA
    ImGui::Text("lua status: %s", MainLuaInterpreter.StatusText);
#endif

    ImGui::End();
}

void RasterizeGui::CleanUp()
{
}

bool RasterizeGui::ImguiUpdateRenderData()
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

    if (ImGui::SliderFloat("first object shiness", &renderingBuffer.objInstances[0].m_material.m_shiness, 0.0f, 1.0f))
    {
        renderingBuffer.RebuildInstanceBuffer();
        isDirtyData = true;
    }

    return isDirtyData;
}

void RasterizeGui::RenderMainImage()
{
    MainPipline->ClearBackBuffer(vector4::WHITE);

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
        // update float pixel to byte pixel
        MainPipline->m_backBuffer->FloatPixelToBytePixel();
        // only update resource when image is valide
        MainImage.UpdateImageRowData(state->pDeviceContext, MainPipline->m_backBuffer->GetRawData(), MainPipline->m_backBuffer->GetWidth(), MainPipline->m_backBuffer->GetHeight());
    }
    else
    {
        // update float pixel to byte pixel
        MainPipline->m_backBuffer->FloatPixelToBytePixel();
        // else set a new image data, in the mean time, create all ID3D11* resources.
        MainImage.SetImageRawData(state->pDevice, MainPipline->m_backBuffer->GetRawData(), MainPipline->m_backBuffer->GetWidth(), MainPipline->m_backBuffer->GetHeight());
    }// end else
}
