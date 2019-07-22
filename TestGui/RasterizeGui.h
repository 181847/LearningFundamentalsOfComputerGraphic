#pragma once
#include "AbstractGui.h"
#include <d3d11.h>
#include "ImguiWrapImageDX11.h"
#include "../TestCasesInSuits/BaseToolForCaseAndSuit.h"

class RasterizeGui : public AbstractGui 
{
public:
    virtual void Init(std::shared_ptr<InitState> state) override;
    virtual void Render() override;
    virtual void CleanUp() override;

    /*!
        \brief imgui show render data
        \return true if data change, which indicate to fresh corresponding render img.
    */
    bool ImguiUpdateRenderData();

    /*!
        \brief render custom image(not imgui).
    */
    void RenderMainImage();

protected:
    char            StatusText[255];

#ifdef EMBED_LUA
    MinimalLuaInpterpreter MainLuaInterpreter;
#endif

    // type aliases
    using SimplePoint = GraphicToolSet::SimplePoint;
    using PSIn = GraphicToolSet::PSIn;
    using VSOut = GraphicToolSet::VSOut;
    using ObjectInstance = GraphicToolSet::ObjectInstance;
    using ConstantBufferForInstance = GraphicToolSet::ConstantBufferForInstance;
    using ConstantBufferForCamera = GraphicToolSet::ConstantBufferForCamera;
    using MaterialBuffer = GraphicToolSet::MaterialBuffer;

    using CommonRenderingBuffer = GraphicToolSet::CommonRenderingBuffer;
    ImguiWrapImageDX11                                  MainImage;  // the render image for application display.
    GraphicToolSet                                      HelpPiplineCase;   // preparations for pipline
    std::unique_ptr<Pipline>                            MainPipline;
    std::shared_ptr<CommonClass::PiplineStateObject>    PSO;
    CommonRenderingBuffer                               renderingBuffer;
    ConstantBufferForInstance                           instanceBufAgent;// agent buffer for setting instance data
    std::shared_ptr<Texture>                            textureAgent;// texture agent for pixel shader.
    GraphicToolSet::PixelShaderSig                      pixelShaderNoTexture;
    GraphicToolSet::PixelShaderSig                      pixelShaderTexture;
    GraphicToolSet::PixelShaderSig                      pixelShaderNoiseNormalTexture;
    std::shared_ptr<Texture>                            texture1 = std::make_shared<Texture>();
    std::shared_ptr<Texture>                            texture2 = std::make_shared<Texture>();
    std::shared_ptr<Texture>                            texture3 = std::make_shared<Texture>();

};