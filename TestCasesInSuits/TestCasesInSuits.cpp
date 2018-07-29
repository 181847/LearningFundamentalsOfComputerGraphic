#include <MyTools/TestTool/Suit.h>  // include test framework
#include "CommonHeaders.h"
// include required suits here
#include "TestBasicTypesSuit.h"

#pragma comment(lib, "MyTools/MyTool.lib")
#pragma comment(lib, "CommonClasses.lib")


class RaySphereLineCase : public CaseForPipline
{
public:
    RaySphereLineCase() : CaseForPipline("draw ray sphere line") {}

    virtual void Run()
    {
        using namespace CommonClass;

        assert(pEnvironment);
        auto pipline = pEnvironment->GetCommonPipline();
        auto pso = pipline->GetPSO();

        const Types::F32 LEFT(-1.0f), RIGHT(1.0f), BOTTOM(-1.0f), TOP(1.0f), NEAR(-0.5f), FAR(-4.0f);

        // world to camera space
        Transform pushInto = Transform::Translation(0.0f, 0.0f, -2.0f);
        Transform rotateY = Transform::RotationY(Types::Constant::PI_F / 3.0f);
        Transform rotateZ = Transform::RotationZ(Types::Constant::PI_F / 3.0f);
        Transform toView = pushInto * rotateZ * rotateY;

        // perspective tranformation
        Transform perTrans = Transform::PerspectiveOG(LEFT, RIGHT, BOTTOM, TOP, NEAR, FAR);
        Transform perspect = perTrans;

        // single matrix from world space to NDC space. 
        // This matrix may not be used currently for debuging.
        Transform toNDC = perTrans * toView;

        pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
            const Types::F32   depth = (pVertex->m_posH.m_z + 1.0f) * 0.5f;
            const SimplePoint* pPoint = reinterpret_cast<const SimplePoint*>(pVertex);
            const Types::F32   red = pPoint->m_rayIndex.m_z;

            const Types::U32   roundIndex = static_cast<const Types::U32>(pPoint->m_rayIndex.m_x);
            const Types::U32   lineIndex = static_cast<const Types::U32>(pPoint->m_rayIndex.m_y);
            //const Types::F32   isTheOne   = 22 < lineIndex && lineIndex < 34 ? 1.0f : 0.0f;
            const Types::F32   isTheOne = roundIndex == 7 && 34 <= lineIndex && lineIndex <= 34 ? 1.0f : 0.0f;

            RGBA               color(isTheOne, 0.0f, 0.0f, 1.0f);
            return color;
        };

        pso->m_vertexShader = [&toView, &perspect](const unsigned char * pSrcVertex, ScreenSpaceVertexTemplate * pDestV)->void {
            const SimplePoint* pSrcH = reinterpret_cast<const SimplePoint*>(pSrcVertex);
            SimplePoint* pDestH = reinterpret_cast<SimplePoint*>(pDestV);

            //DebugClient<DEBUG_CLIENT_CONF_LINE_CLIP_ERROR_ANALYSIS>(pSrcH->m_rayIndex.m_x == 7.0f && pSrcH->m_rayIndex.m_y == 34.0f);

            // to camera space
            hvector inViewPos = toView * pSrcH->m_position;

            // perspective transform
            pDestH->m_position = perspect * inViewPos;

            // additional information about the ray index for Debuging.
            pDestH->m_rayIndex = pSrcH->m_rayIndex;
        };

        std::vector<SimplePoint> points;
        std::vector<unsigned int> indices;
        BuildSphereRayLineData(points, indices);

        auto vertexBuffer = std::make_unique<F32Buffer>(points.size() * sizeof(SimplePoint));
        memcpy(vertexBuffer->GetBuffer(), points.data(), vertexBuffer->GetSizeOfByte());

        {
            COUNT_DETAIL_TIME;
            DebugGuard<DEBUG_CLIENT_CONF_LINE_CLIP_ERROR_ANALYSIS> guard;
            pipline->DrawInstance(indices, vertexBuffer.get());
        }

        std::wstring pictureIndex = L"final";
        std::wstring pictureName = L"lineClippingErrAnalysis_fixed_" + pictureIndex + L".png";
        ImageWindow imgWnd(pipline->m_backBuffer.get(), pictureName);
        imgWnd.BlockShow();

        pipline->m_backBuffer->SaveTo(L"..\\OutputTestImage\\PiplineTest\\" + pictureName);
    }
};


int main()
{
    SuitForPipline<RaySphereLineCase> firstSuit;

    firstSuit.Start();

    BasicTypesTestSuit_Runable bttsRunable;
    bttsRunable.Start();

    getchar();
    return 0;
}

