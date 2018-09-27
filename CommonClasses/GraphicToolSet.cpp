#include "GraphicToolSet.h"
#include "GeomentryBuilder.h"
#include "DebugHelpers.h"
#include "DebugConfigs.h"


namespace CommonClass
{

GraphicToolSet::CommonRenderingBuffer::CommonRenderingBuffer()
{
    Init();
}

void GraphicToolSet::CommonRenderingBuffer::Init()
{
    // pre build meshes.
    BuildMeshDatas();

    //perspect = Transform::PerspectiveOG(LEFT_F, RIGHT_F, BOTTOM_F, TOP_F, NEAR_F, FAR_F);
    perspect = Transform::PerspectiveFOV(Types::Constant::PI_F * 0.5f, 1.0f, NEAR_F, FAR_F);

    Types::F32 pitch(3.14f * 3.f / 4.f), yaw(3.14f / 4.f), roll(0.f * 3.14f / 3.f);
    // 1
    objInstances[0].m_position = vector3(0.0f, 0.0f, 0.0f);
    objInstances[0].m_rotation = vector3(0, 0, 0);
    objInstances[0].m_scale = vector3(2.0f, 2.0f, 2.0f);
    objInstances[0].m_material.m_diffuse = RGB::WHITE;
    objInstances[0].m_material.m_fresnelR0 = FresnelR0_byReflectionIndex(6);
    objInstances[0].m_material.m_shiness = 64.0f;
    // 2
    objInstances[1].m_position = vector3(1.0f, 1.4f, 0.0f);
    objInstances[1].m_rotation = vector3(pitch, yaw + 3.14f / 3, roll);
    objInstances[1].m_scale = vector3(1.5f, 1.5f, 1.5f);
    objInstances[1].m_material.m_diffuse = RGB::WHITE;
    objInstances[1].m_material.m_fresnelR0 = FresnelR0_byReflectionIndex(6);
    objInstances[1].m_material.m_shiness = 64.0f;
    // 3
    objInstances[2].m_position = vector3(-1.0f, 2.8f, 0.0f);
    objInstances[2].m_rotation = vector3(pitch, yaw + 3.14f / 2.f, roll + 3.14f / 8.f);
    objInstances[2].m_scale = vector3(0.8f, 0.8f, 0.8f);
    objInstances[2].m_material.m_diffuse = RGB::WHITE;
    objInstances[2].m_material.m_fresnelR0 = FresnelR0_byReflectionIndex(6);
    objInstances[2].m_material.m_shiness = 64.0f;

    LightBuffer pointLight;
    cameraBuffer.m_numLights = 2;
    cameraBuffer.m_ambientColor = RGB::WHITE * RGB(0.05f, 0.05f, 0.05f);
    cameraBuffer.m_lights[0] = { vector3(0.0f, 5.0f, 0.0f), RGB::WHITE };
    LightBuffer spotLight;
    spotLight.m_color = RGB::RED;
    spotLight.m_direction = Normalize(vector3(-1.0f, 1.0f, -1.0f));
    spotLight.m_fadeoffStart = 1.0f;
    spotLight.m_fadeoffEnd = 10.0f;
    spotLight.m_position = 2.0f * vector3(1.0f, -1.0f, 1.0f);
    spotLight.m_spotPower = 1.0f;
    cameraBuffer.m_lights[1] = spotLight;

    cameraFrame.m_origin = vector3(0.0f, 0.0f, 1.0f) * 4.0f;
    cameraFrame.m_lookAt = vector3::ZERO;

    // camera frame for shadow map
    lightCameraFrame.m_origin = cameraBuffer.m_lights[1].m_position;
    lightCameraFrame.m_lookAt = lightCameraFrame.m_origin + cameraBuffer.m_lights[1].m_direction;

    // first set the constant buffer.
    UpdateConstantBuffer();
}

void GraphicToolSet::CommonRenderingBuffer::UpdateConstantBuffer()
{
    RebuildInstanceBuffer();
    RebuildCameraBuffer();
}

void GraphicToolSet::CommonRenderingBuffer::RebuildInstanceBuffer()
{
    for (unsigned int i = 0; i < instanceBuffers.size(); ++i)
    {
        instanceBuffers[i].m_toWorld = Transform::TRS(objInstances[i].m_position, objInstances[i].m_rotation, objInstances[i].m_scale);
        instanceBuffers[i].m_toWorldInverse = Transform::InverseTRS(objInstances[i].m_position, objInstances[i].m_rotation, objInstances[i].m_scale);
        instanceBuffers[i].m_material = objInstances[i].m_material;
    }// end for
}

void GraphicToolSet::CommonRenderingBuffer::RebuildCameraBuffer()
{
    cameraFrame.RebuildFrameDetail();
    lightCameraFrame.RebuildFrameDetail();

    cameraBuffer.SetCameraMatrix(cameraFrame);
    cameraBuffer.m_project = perspect;

    lightCameraBuffer.SetCameraMatrix(lightCameraFrame);
    lightCameraBuffer.m_project = perspect;
}

void GraphicToolSet::CommonRenderingBuffer::BuildMeshDatas()
{
    for (unsigned int i = 0; i < NUM_SUPPORTED_MESH; ++i)
    {
        MeshData rawData;
        switch (i)
        {
        case M_CUBE:
            rawData = GeometryBuilder::BuildCube(2.0f, 2.0f, 2.0f);
            break;

        case M_CYLINDER:
            rawData = GeometryBuilder::BuildCylinder(0.6f, 0.8f, 0.8f, 8, 3, true);
            break;

        case M_SPHERE:
            rawData = GeometryBuilder::BuildSphere(0.8f, 16, 16);
            break;

        case M_GEOSPHERE:
            rawData = GeometryBuilder::BuildGeoSphere(0.8f, 2);
            break;

        default:
            throw std::exception("unsupported prebuild mesh type");
            break;
        }

        // transfer raw data to specialized mesh data.
        std::vector<SimplePoint> vertices;
        for (const auto& vertex : rawData.m_vertices)
        {
            vertices.push_back(SimplePoint(vertex.m_pos.Tovector4(), vertex.m_normal.Tovector4(0.0f), vertex.m_uv * 3.0f));
        }
        prebuildMeshData[i].indices = rawData.m_indices;
        prebuildMeshData[i].vertexBuffer = std::make_unique<F32Buffer>(vertices.size() * sizeof(decltype(vertices)::value_type));
        memcpy(prebuildMeshData[i].vertexBuffer->GetBuffer(), vertices.data(), prebuildMeshData[i].vertexBuffer->GetSizeOfByte());
    }
}


std::shared_ptr<CommonClass::PiplineStateObject> GraphicToolSet::GetCommonPSO()
{
    auto pso = std::make_shared<PiplineStateObject>();
    pso->m_primitiveType = PrimitiveType::LINE_LIST;
    pso->m_vertexLayout.vertexShaderInputSize = sizeof(SimplePoint);
    pso->m_vertexLayout.pixelShaderInputSize = sizeof(SimplePoint);
    pso->SetViewport(GetCommonViewport());

    return pso;
}

CommonClass::Viewport GraphicToolSet::GetCommonViewport()
{
    Viewport viewport;
    viewport.left = 0.0f;
    viewport.right = static_cast<Types::F32>(COMMON_PIXEL_WIDTH - 1);
    viewport.bottom = 0.0f;
    viewport.top = static_cast<Types::F32>(COMMON_PIXEL_HEIGHT - 1);
    return viewport;
}

std::unique_ptr<CommonClass::Pipline> GraphicToolSet::GetCommonPipline()
{
    // create and set a pipline.
    auto pipline = std::make_unique<Pipline>();
    pipline->SetPSO(GetCommonPSO());

    // set a back buffer
    pipline->SetBackBuffer(std::make_unique<RasterizeImage>(
        COMMON_PIXEL_WIDTH,
        COMMON_PIXEL_HEIGHT,
        RGBA::WHITE));

    return std::move(pipline);
}

GraphicToolSet::VertexShaderSig GraphicToolSet::GetVertexShaderWithNormal(Transform& trs, Transform& perspect, Transform& normalTrs)
{
    return [&trs, &perspect, &normalTrs](const unsigned char * pSrcVertex, ScreenSpaceVertexTemplate * pDestV)->void {
        const SimplePoint* pSrcH = reinterpret_cast<const SimplePoint*>(pSrcVertex);
        SimplePoint* pDestH = reinterpret_cast<SimplePoint*>(pDestV);

        vector4 inViewPos = trs * pSrcH->m_position;

        pDestH->m_position = perspect * inViewPos;
        //pDestH->m_position = pSrcH->m_position;
        vector4 normal = pSrcH->m_rayIndex;
        normal.m_w = 0.0f;
        pDestH->m_rayIndex = normalTrs * normal;
    };
}

GraphicToolSet::VertexShaderSig GraphicToolSet::GetVertexShaderWithNormalAndConstantBuffer(ConstantBufferForInstance& constBufInstance, ConstantBufferForCamera& constBufCamera)
{
    return [&constBufInstance, &constBufCamera](const unsigned char * pSrcVertex, ScreenSpaceVertexTemplate * pDestV)->void {
        const SimplePoint* pSrcH = reinterpret_cast<const SimplePoint*>(pSrcVertex);
        SimplePoint* pDestH = reinterpret_cast<SimplePoint*>(pDestV);

        vector4 world = constBufInstance.m_toWorld * pSrcH->m_position;
        vector4 camera = constBufCamera.m_toCamera * world;

        pDestH->m_position = constBufCamera.m_project * camera;
        //pDestH->m_position = pSrcH->m_position;

        vector4 normal = pSrcH->m_rayIndex;
        normal.m_w = 0.0f;// ensure translation will not affect calculations.
        Transform transformNormalToCamera = (constBufInstance.m_toWorldInverse * constBufCamera.m_toCameraInverse).T();// take transposes
        pDestH->m_rayIndex = transformNormalToCamera * normal;
    };
}

GraphicToolSet::VertexShaderSig GraphicToolSet::GetVertexShaderWithVSOut(ConstantBufferForInstance& constBufInstance, ConstantBufferForCamera& constBufCamera)
{
    return [&constBufInstance, &constBufCamera](const unsigned char * pSrcVertex, ScreenSpaceVertexTemplate * pDestV)->void {
        const SimplePoint* pSrcH = reinterpret_cast<const SimplePoint*>(pSrcVertex);
        VSOut* pDest = reinterpret_cast<VSOut*>(pDestV);

        pDest->m_uv = pSrcH->m_uv;

        pDest->m_posW = constBufInstance.m_toWorld * pSrcH->m_position;
        vector4 camera = constBufCamera.m_toCamera * pDest->m_posW;

        pDest->m_posH = constBufCamera.m_project * camera;
        //pDestH->m_position = pSrcH->m_position;

        vector4 normal = pSrcH->m_rayIndex;
        normal.m_w = 0.0f;// ensure translation will not affect calculations.
        Transform transformNormalToCamera = constBufInstance.m_toWorldInverse.T();// take transposes
        pDest->m_normalW = Normalize((transformNormalToCamera * normal).ToVector3()).Tovector4();
    };
}

CommonClass::RGBA GraphicToolSet::ColdToWarm(const vector3& normal, const vector3& WarmDirection /*= Normalize(vector3::UNIT)*/)
{
    Types::F32 kw = 0.5f * (1 + dotProd(normal, WarmDirection));
    RGB color = kw * RGB::BLUE + (1 - kw) * RGB::RED;
    return Cast(color);
}

GraphicToolSet::PixelShaderSig GraphicToolSet::GetPixelShaderWithNormal()
{
    return [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
        const SimplePoint* pPoint = reinterpret_cast<const SimplePoint*>(pVertex);
        vector3 WarmDirection = Normalize(vector3(1.0f, 1.0f, 0.0f));
        vector3 normal = pPoint->m_rayIndex.ToVector3();
        return ColdToWarm(normal);
    };
}

GraphicToolSet::PixelShaderSig GraphicToolSet::GetPixelShaderWithPSIn(ConstantBufferForInstance& constBufInstance, ConstantBufferForCamera& constBufCamera)
{
    return [&constBufInstance, &constBufCamera](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
        const PSIn* pPoint = reinterpret_cast<const PSIn*>(pVertex);

        vector3 normal = Normalize(pPoint->m_normalW.ToVector3());
        vector3 pixelPosW = pPoint->m_posW.ToVector3();
        vector3 toEye = Normalize(constBufCamera.m_camPos - pixelPosW);

        vector3 blinn = ComputePointLight(
            constBufCamera.m_lights[0],
            vector3(constBufInstance.m_material.m_diffuse.m_arr),
            vector3(constBufInstance.m_material.m_fresnelR0.m_arr),
            constBufInstance.m_material.m_shiness,
            pixelPosW,
            normal,
            toEye);

        if (constBufCamera.m_numLights == 2)
        {
            vector3 blinnSpot = ComputeSpotLight(
                constBufCamera.m_lights[1],
                vector3(constBufInstance.m_material.m_diffuse.m_arr),
                vector3(constBufInstance.m_material.m_fresnelR0.m_arr),
                constBufInstance.m_material.m_shiness,
                pixelPosW,
                normal,
                toEye);
            blinn = blinn + blinnSpot;
        }

        RGBA color(blinn.m_arr);
        return color;
    };
}

GraphicToolSet::PixelShaderSig GraphicToolSet::GetPixelShaderWithPSInAndTexture(ConstantBufferForInstance& constBufInstance, ConstantBufferForCamera& constBufCamera, std::shared_ptr<Texture>& texture)
{
    return [&constBufInstance, &constBufCamera, &texture](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
        assert(texture->IsValid());
        const PSIn* pPoint = reinterpret_cast<const PSIn*>(pVertex);

        vector3 normal = Normalize(pPoint->m_normalW.ToVector3());
        vector3 pixelPosW = pPoint->m_posW.ToVector3();
        vector3 toEye = Normalize(constBufCamera.m_camPos - pixelPosW);

        // sample texture to get diffuse color.
        vector2 uv = pPoint->m_uv;
        RGBA sampleColor = texture->Sample(uv.m_x, uv.m_y);
        RGB diffuse = Cast(sampleColor);

        vector3 resultColor = ComputePointLight(
            constBufCamera.m_lights[0],
            vector3(diffuse.m_arr),
            vector3(constBufInstance.m_material.m_fresnelR0.m_arr),
            constBufInstance.m_material.m_shiness,
            pixelPosW,
            normal,
            toEye);

        if (constBufCamera.m_numLights == 2)
        {
            vector3 blinnSpot = ComputeSpotLight(
                constBufCamera.m_lights[1],
                vector3(diffuse.m_arr),
                vector3(constBufInstance.m_material.m_fresnelR0.m_arr),
                constBufInstance.m_material.m_shiness,
                pixelPosW,
                normal,
                toEye);
            resultColor = resultColor + blinnSpot;
        }

        RGBA color(resultColor.m_arr);
        return color;
    };
}

GraphicToolSet::PixelShaderSig GraphicToolSet::GetPixelShaderWithNoiseBumpMap(ConstantBufferForInstance& constBufInstance, ConstantBufferForCamera& constBufCamera, std::shared_ptr<Texture>& texture)
{
    return [&constBufInstance, &constBufCamera, &texture](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
        assert(texture->IsValid());
        const PSIn* pPoint = reinterpret_cast<const PSIn*>(pVertex);

        DEBUG_CLIENT(DEBUG_CLIENT_CONF_TRIANGL);
        // get noise normal from texture.
        vector2 uv = pPoint->m_uv;
        RGBA sampleColor = texture->Sample(uv.m_x, uv.m_y);
        vector3 noiseNormal(sampleColor.m_chas.m_r, sampleColor.m_chas.m_g, sampleColor.m_chas.m_b);
        // channels of noise color all lay in [0,1], map them to [-1, 1]
        noiseNormal = noiseNormal * 2.0f - vector3::UNIT;
        noiseNormal = Normalize(noiseNormal);
        //noiseNormal = noiseNormal * 2.0f;// enhance the noise power.
        vector3 normal = Normalize(noiseNormal + pPoint->m_normalW.ToVector3());

        // ignore noise normal
        normal = Normalize(pPoint->m_normalW.ToVector3());

        vector3 pixelPosW = pPoint->m_posW.ToVector3();
        vector3 toEye = Normalize(constBufCamera.m_camPos - pixelPosW);

        vector3 blinn = ComputePointLight(
            constBufCamera.m_lights[0],
            vector3(constBufInstance.m_material.m_diffuse.m_arr),
            vector3(constBufInstance.m_material.m_fresnelR0.m_arr),
            constBufInstance.m_material.m_shiness,
            pixelPosW,
            normal,
            toEye);

        if (constBufCamera.m_numLights == 2)
        {
            vector3 blinnSpot = ComputeSpotLight(
                constBufCamera.m_lights[1],
                vector3(constBufInstance.m_material.m_diffuse.m_arr),
                vector3(constBufInstance.m_material.m_fresnelR0.m_arr),
                constBufInstance.m_material.m_shiness,
                pixelPosW,
                normal,
                toEye);
            blinn = blinn + blinnSpot;
        }

        RGBA color(blinn.m_arr);
        return color;
    };
}

GraphicToolSet::PixelShaderSig GraphicToolSet::GetPixelShaderForShadowMap(ConstantBufferForInstance& constBufInstance, ConstantBufferForCamera& lightCamera)
{
    static float minNdcZ = 1.0f, maxNdcZ = 0.0f;
    return [&constBufInstance, &lightCamera](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
        const PSIn* pPoint = reinterpret_cast<const PSIn*>(pVertex);
        
        Types::F32 ndcZ = pPoint->m_posH.m_z;

        if (ndcZ < minNdcZ)
        {
            minNdcZ = ndcZ;
        }

        if (ndcZ > maxNdcZ)
        {
            maxNdcZ = ndcZ;
        }

        printf("minNdcZ = %.7f, maxNdcZ = %.7f\r", minNdcZ, maxNdcZ);

        assert(0.0f <= ndcZ && ndcZ <= 1.0f);

        return RGBA(ndcZ, ndcZ, ndcZ);
    };
}

CommonClass::GraphicToolSet::PixelShaderSig GraphicToolSet::GetPixelShaderForShadowEffect(ConstantBufferForInstance& constBufInstance, ConstantBufferForCamera& constBufCamera, ConstantBufferForCamera& lightCamera, std::shared_ptr<Texture>& shadowMap)
{
    return [&constBufInstance, &constBufCamera, &lightCamera, &shadowMap](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
        assert(shadowMap->IsValid());
        const PSIn* pPoint = reinterpret_cast<const PSIn*>(pVertex);

        vector3 normal = Normalize(pPoint->m_normalW.ToVector3());
        vector3 pixelPosW = pPoint->m_posW.ToVector3();
        vector3 toEye = Normalize(constBufCamera.m_camPos - pixelPosW);

        vector3 resultColor = ComputePointLight(
            constBufCamera.m_lights[0],
            vector3(constBufInstance.m_material.m_diffuse.m_arr),
            vector3(constBufInstance.m_material.m_fresnelR0.m_arr),
            constBufInstance.m_material.m_shiness,
            pixelPosW,
            normal,
            toEye);

        if (constBufCamera.m_numLights == 2)
        {
            // consider shadow effect only of spot light.
            vector4 inLightCameraH = lightCamera.m_project * lightCamera.m_toCamera * pPoint->m_posW;
            vector3 inLightCamera = inLightCameraH.ToVector3();
            inLightCamera = inLightCamera * (1.0f / inLightCameraH.m_w);
            auto sampledDepth = shadowMap->Sample(0.5f + 0.5f * inLightCamera.m_x, 0.5f + 0.5f * inLightCamera.m_y);

            if ((sampledDepth.m_chas.m_r + 0.01) > inLightCamera.m_z)
            {
                vector3 blinnSpot = ComputeSpotLight(
                    constBufCamera.m_lights[1],
                    vector3(constBufInstance.m_material.m_diffuse.m_arr),
                    vector3(constBufInstance.m_material.m_fresnelR0.m_arr),
                    constBufInstance.m_material.m_shiness,
                    pixelPosW,
                    normal,
                    toEye);
                resultColor = resultColor + blinnSpot;
            }
        }

        RGBA color(resultColor.m_arr);
        return color;
    };
}

GraphicToolSet::SimplePoint::SimplePoint(const vector4& pos /*= vector4()*/, const vector4& normal /*= vector4()*/, const vector2& uv /*= vector2()*/)
    :m_position(pos), m_rayIndex(normal), m_uv(uv) 
{
    // empty
}

RGB GraphicToolSet::FresnelR0_byReflectionIndex(const Types::U32 reflectIndex)
{
    if (reflectIndex == 0)
    {
        throw std::exception("FresnelR0 computation error, reflection index should greater than Zero.");
    }

    Types::F32 component = (reflectIndex - 1) * 1.0f / (reflectIndex + 1);
    component *= component; // take square
    return RGB(component, component, component);
}

CommonClass::vector3 GraphicToolSet::SchlickFresnel(const vector3& r0, const vector3& normal, const vector3& toLight)
{
    using namespace Types;
    F32 cosIncidentAngle = MathTool::Saturate(dotProd(normal, toLight));
    F32 f0 = 1.0f - cosIncidentAngle;
    vector3 reflectPercent = r0 + (vector3::UNIT - r0) * (f0 * f0 * f0 * f0 * f0);
    return reflectPercent;
}

CommonClass::vector3 GraphicToolSet::BlinnPhone(const vector3& lightStrength, const vector3& diffuseAlbedo, const vector3& fresnelR0, const Types::F32 shiness, const vector3& toLight, const vector3& toEye, const vector3& normal)
{
    using namespace Types;
    F32 m = shiness * 256.0f;
    vector3 halfVec = Normalize(toEye + toLight);
    F32 roughnessFactor = (m + 8.0f) * 0.125f * std::pow(std::max(dotProd(halfVec, normal), 0.0f), m);
    vector3 fresnelFactor = SchlickFresnel(vector3(fresnelR0.m_arr), normal, toLight);

    vector3 specAlbedo = fresnelFactor * roughnessFactor;
    specAlbedo = specAlbedo / (specAlbedo + vector3::UNIT);

    return (diffuseAlbedo + specAlbedo) * lightStrength;
}

CommonClass::vector3 GraphicToolSet::ComputePointLight(const LightBuffer& L, const vector3& diffuseAlbedo, const vector3& fresnelR0, const Types::F32 shiness, const vector3& posW, const vector3 normal, const vector3 toEye)
{
    using namespace Types;
    vector3 toLight = L.m_position - posW;
    F32 toLightLength = Length(toLight);

    if (toLightLength > L.m_fadeoffEnd)
    {
        return vector3::ZERO;
    }
    toLight = toLight / toLightLength;

    // cosine law
    F32 ndotl = std::max(dotProd(toLight, normal), 0.0f);
    vector3 lightStrength = vector3(L.m_color.m_arr) * ndotl;

    // fade effect
    F32 att = MathTool::Attenuation(toLightLength, L.m_fadeoffStart, L.m_fadeoffEnd);
    lightStrength = lightStrength * att;

    return BlinnPhone(lightStrength, diffuseAlbedo, fresnelR0, shiness, toLight, toEye, normal);
}

CommonClass::vector3 GraphicToolSet::ComputeSpotLight(const LightBuffer& L, const vector3& diffuseAlbedo, const vector3& fresnelR0, const Types::F32 shiness, const vector3& posW, const vector3 normal, const vector3 toEye)
{
    using namespace Types;
    vector3 toLight = L.m_position - posW;
    F32 toLightLength = Length(toLight);

    if (toLightLength > L.m_fadeoffEnd)
    {
        return vector3::ZERO;
    }
    toLight = toLight / toLightLength;

    // cosine law
    F32 ndotl = std::max(dotProd(toLight, normal), 0.0f);
    vector3 lightStrength = vector3(L.m_color.m_arr) * ndotl;

    // fade effect
    F32 att = MathTool::Attenuation(toLightLength, L.m_fadeoffStart, L.m_fadeoffEnd);
    lightStrength = lightStrength * att;

    // spot power
    F32 spotFactor = std::pow(std::max(dotProd(-toLight, L.m_direction), 0.0f), L.m_spotPower);
    lightStrength = lightStrength * spotFactor;

    return BlinnPhone(lightStrength, diffuseAlbedo, fresnelR0, shiness, toLight, toEye, normal);
}

void GraphicToolSet::ConstantBufferForCamera::SetCameraMatrix(const CameraFrame& cameraFrame)
{
    m_toCamera          = cameraFrame.WorldToLocal();
    m_toCameraInverse   = cameraFrame.LocalToWorld();
    m_camPos            = cameraFrame.m_origin;
}

}// namespace CommonClass
