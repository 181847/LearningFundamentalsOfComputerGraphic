#pragma once
#include "vector2.h"
#include "vector3.h"
#include "hvector.h"
#include "ColorTemplate.h"
#include "Transform.h"
#include "ScreenSpaceVertexTemplate.h"
#include "F32Buffer.h"
#include "Pipline.h"
#include "Texture.h"
#include "CameraFrame.h"
#include <functional>
#include <memory>
#include <array>
#include <vector>

namespace CommonClass
{


/*!
    \brief a set of useful tools for rendering.
*/
class GraphicToolSet
{
public:
    using PixelShaderSig  = std::function<RGBA(const ScreenSpaceVertexTemplate*)>;
    using VertexShaderSig = std::function<void(const unsigned char * , ScreenSpaceVertexTemplate * pDestV)>;

    // temp struct for generous case
    struct SimplePoint
    {
    public:
        explicit SimplePoint(const hvector& pos = hvector(), const hvector& normal = hvector(), const vector2& uv = vector2());

    public:
        hvector m_position;
        /*!
            \brief used to store the sphere ray location information
            x - round index
            y - line index in the round
            z = 0 means it's start point
            z = 1 means it's end point
        */
        hvector m_rayIndex;
        vector2 m_uv;
    };
    static_assert(sizeof(SimplePoint) == 2 * sizeof(hvector) + 2 * sizeof(Types::F32), "The size of SimplePoint is not matched for this case.");

    /*!
        \brief output of the vertex shader. 
    */
    struct VSOut
    {
    public:
        hvector m_posH;
        hvector m_posW;
        hvector m_normalW;
        vector2 m_uv;
    };
    using PSIn = VSOut;
    static_assert(sizeof(VSOut) == 3 * sizeof(hvector) + 2 * sizeof(Types::F32), "structure VSOut has wrong size.");
    
    /*!
        \brief the buffer of light
    */
    struct LightBuffer
    {
    public:
        vector3     m_position      = vector3::ZERO;
        RGB         m_color         = RGB::WHITE;
        vector3     m_direction     = vector3::AXIS_Y;
        Types::F32  m_fadeoffStart  = 0.0f;
        Types::F32  m_fadeoffEnd    = 10.0f;
        Types::F32  m_spotPower     = 1.0f;
    };

    
    /*!
        \brief buffer of simple material
    */
    struct MaterialBuffer
    {
    public:
        RGB         m_diffuse = RGB::WHITE;
        Types::F32  m_shiness = 1.0f;
        RGB         m_fresnelR0;
    };
    
    /*!
        \brief a simple wrapper for object instance data.
    */
    struct ObjectInstance
    {
    public:
        vector3         m_position;
        vector3         m_rotation;
        vector3         m_scale;
        MaterialBuffer  m_material;
    };

    /*!
        \brief constant buffer for instances
    */
    struct ConstantBufferForInstance
    {
    public:
        Transform       m_toWorld;         // matrix for objects to world
        Transform       m_toWorldInverse;  // inverse matrix to world
        MaterialBuffer  m_material;   // material of the object
    };

    /*!
        \brief constant buffer for camera settings
    */
    struct ConstantBufferForCamera
    {
    public:
        Transform                   m_toCamera;         // matrix to transform vertex to camera space
        Transform                   m_toCameraInverse;  // inverse matrix from world to camera
        Transform                   m_project;          // projection matrix
        vector3                     m_camPos;           // camera position
        RGB                         m_ambientColor;     // ambientColor
        int                         m_numLights;        // the number of light in the scene, less or equal m_lights.size().
        std::array<LightBuffer, 3>  m_lights;           // the lights in scene

        /*!
            \brief set transform matrix about camera.
        */
        void SetCameraMatrix(const CameraFrame& cameraFrame);
    };

    /*!
        \brief this kind of structure will have pre build data for rendering, for example: TRS of prebuilded instance, camera buffer...
    */
    struct CommonRenderingBuffer
    {
    public:
        /*!
            \brief the mesh data can be directly drawn.
        */
        struct SpecializedMeshData
        {
            std::unique_ptr<F32Buffer>  vertexBuffer;
            std::vector<Types::U32>     indices;
        };
        enum
        {
            M_CUBE = 0,
            M_CYLINDER,
            M_SPHERE,
            M_GEOSPHERE,
            NUM_SUPPORTED_MESH
        };
        CommonRenderingBuffer();

        void Init();

        /*!
            \brief apply adjustment to the constant buffer, which is the data that will be read from shaders.
        */
        void UpdateConstantBuffer();

        /*!
            \brief rebuild instanceBuffers from objInstances.
        */
        void RebuildInstanceBuffer();

        /*!
            \brief rebuild camera buffer by camera frame.
        */
        void RebuildCameraBuffer();

        /*1
            \brief build mesh data for drawing.
        */
        void BuildMeshDatas();

    public:
        const Types::F32                                        LEFT_F      = -1.0f;
        const Types::F32                                        RIGHT_F     =  1.0f;
        const Types::F32                                        BOTTOM_F    = -1.0f;
        const Types::F32                                        TOP_F       =  1.0f;
        const Types::F32                                        NEAR_F      = 1.0f;
        const Types::F32                                        FAR_F       = 30.0f;
        Transform                                               perspect;
        std::array<ObjectInstance, 3>                           objInstances;
        std::array<ConstantBufferForInstance, 3>                instanceBuffers;
        CameraFrame                                             cameraFrame;
        CameraFrame                                             lightCameraFrame;
        ConstantBufferForCamera                                 cameraBuffer;
        ConstantBufferForCamera                                 lightCameraBuffer;  // a rendering constant buffer for spot light.
        std::array<SpecializedMeshData, NUM_SUPPORTED_MESH>     prebuildMeshData;
    };

    /*!
        \brief Build common PiplineStateObject for using.
    */
    std::shared_ptr<PiplineStateObject> GetCommonPSO();

    /*!
        \brief Build common viewport by the default view size defined in CommonEnvironment.
    */
    Viewport GetCommonViewport();

    /*!
        \brief Build common Pipline for drawing, 
        all you need do is to set PixelShader/VertexShader, 
        and call drawing function with mesh data.
    */
    std::unique_ptr<Pipline> GetCommonPipline();

    /*!
        \brief get a vertex shader with TRS/perspectCamera/normalTransformation
    */
    static VertexShaderSig GetVertexShaderWithNormal(Transform& trs, Transform& perspect, Transform& normalTrs);

    /*!
        \brief get a useful vertex shader that require a ConstantBuffer for transformations.
    */
    static VertexShaderSig GetVertexShaderWithNormalAndConstantBuffer(ConstantBufferForInstance& constBufInstance, ConstantBufferForCamera& constBufCamera);

    /*!
        \brief a vertex shader output a struct of VSOut
    */
    static VertexShaderSig GetVertexShaderWithVSOut(ConstantBufferForInstance& constBufInstance, ConstantBufferForCamera& constBufCamera);

    /*!
        \brief pixel shader for vertex that have normal.
    */
    static PixelShaderSig GetPixelShaderWithNormal();

    /*!
        \brief a pixel shader require VSOut as input
    */
    static PixelShaderSig GetPixelShaderWithPSIn(ConstantBufferForInstance& constBufInstance, ConstantBufferForCamera& constBufCamera);

    /*!
        \brief a pixel shader require one texture.
    */
    static PixelShaderSig GetPixelShaderWithPSInAndTexture(ConstantBufferForInstance& constBufInstance, ConstantBufferForCamera& constBufCamera, std::shared_ptr<Texture>& texture);

    /*!
        \brief a pixel shader require one texture, whose sampling color will be used to disturb the normal of original geometry.
    */
    static PixelShaderSig GetPixelShaderWithNoiseBumpMap(ConstantBufferForInstance& constBufInstance, ConstantBufferForCamera& constBufCamera, std::shared_ptr<Texture>& texture);

    /*!
        \brief a pixel shader return pixel depth value, this is for shadowing map.
        \param lightCamera store the light position/direction...
    */
    static PixelShaderSig GetPixelShaderForShadowMap(ConstantBufferForInstance& constBufInstance, ConstantBufferForCamera& lightCamera);

    /*!
        \brief a pixel shader return pixel depth value, this is for shadowing map.
        \param lightCamera store the light position/direction...
    */
    static PixelShaderSig GetPixelShaderForShadowEffect(ConstantBufferForInstance& constBufInstance, ConstantBufferForCamera& constBufCamera, ConstantBufferForCamera& lightCamera, std::shared_ptr<Texture>& shadowMap);

    /*!
        \brief compute FresnelR0 efficiency by reflection index.
    */
    static RGB FresnelR0_byReflectionIndex(const Types::U32 reflectIndex);

    /*!
        \brief cold to warm color shading.
        \param normal the normal of the pixel
        \param WarmDirection the warmest normal direction
    */
    static RGBA ColdToWarm(const vector3& normal, const vector3& WarmDirection = Normalize(vector3::UNIT));

    static vector3 SchlickFresnel(const vector3& r0, const vector3& normal, const vector3& toLight);

    static vector3 BlinnPhone(const vector3& lightStrength, const vector3& diffuseAlbedo, const vector3& fresnelR0, const Types::F32 shiness, const vector3& toLight, const vector3& toEye, const vector3& normal);

    static vector3 ComputePointLight(const LightBuffer& L, const vector3& diffuseAlbedo, const vector3& fresnelR0, const Types::F32 shiness, const vector3& posW, const vector3 normal, const vector3 toEye);

    static vector3 ComputeSpotLight(const LightBuffer& L, const vector3& diffuseAlbedo, const vector3& fresnelR0, const Types::F32 shiness, const vector3& posW, const vector3 normal, const vector3 toEye);

public:
    // membbers
    int COMMON_PIXEL_WIDTH      = 512;
    int COMMON_PIXEL_HEIGHT     = 512;

    // render boundary can be used in orthographic camera.
    float COMMON_RENDER_LEFT    = -3.0f;
    float COMMON_RENDER_RIGHT   = +3.0f;
    float COMMON_RENDER_BOTTOM  = -3.0f;
    float COMMON_RENDER_TOP     = +3.0f;

};

}// namespace CommonClass
