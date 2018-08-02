#pragma once
#include "CommonHeaders.h"

class CaseForDepthBufferToImg : public CaseForPipline
{
public:
    CaseForDepthBufferToImg() :CaseForPipline("convert depth buffer to image") {}

    virtual void Run() override
    {
        
        DepthBuffer dpbuffer(pEnvironment->COMMON_PIXEL_WIDTH, pEnvironment->COMMON_PIXEL_HEIGHT);
        dpbuffer.SetAll(0.75f);

        auto img = ToImage(dpbuffer, 1.0f);
        
        BlockShowImg(&img, L"a image converted from a depth buffer");
    }
};

using SuitForDepthBuffer = SuitForPipline
<
    CaseForDepthBufferToImg
>;