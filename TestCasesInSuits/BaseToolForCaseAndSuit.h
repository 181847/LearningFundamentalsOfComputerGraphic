#pragma once
/*!
    \brief this head file define may useful base class for Case and Suit.
*/

#include "CommonHeaders.h"
#include <filesystem>
#include <functional>
#undef RGB

using namespace CommonClass;

/*!
    \brief base case class that have common function for and useful Environment for pipline testing.
    for the Suit class, you should use SuitForPipline.
*/
class CaseForPipline : public TestSuit::Case
{
protected:
    using Super = CaseForPipline;

public:
    using PixelShaderSig = std::function<RGBA(const ScreenSpaceVertexTemplate*)>;

    // temp struct for generous case
    struct SimplePoint
    {
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
        explicit SimplePoint(const hvector& pos = hvector(), const hvector& normal = hvector(), const vector2& uv = vector2())
            :m_position(pos), m_rayIndex(normal), m_uv(uv)
        {
            // empty
        }
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
        RGB m_diffuse = RGB::WHITE;
        Types::F32 m_shiness = 1.0f;
        RGB m_fresnelR0;

        /*!
            \brief compute FresnelR0 efficiency by reflection index.
        */
        static RGB FresnelR0_byReflectionIndex(const Types::U32 reflectIndex)
        {
            if (reflectIndex == 0)
            {
                throw std::exception("FresnelR0 computation error, reflection index should greater than Zero.");
            }

            Types::F32 component = (reflectIndex - 1) * 1.0f / (reflectIndex + 1);
            component *= component; // take square
            return RGB(component, component, component);
        }

        static vector3 SchlickFresnel(const vector3& r0, const vector3& normal, const vector3& toLight)
        {
            using namespace Types;
            F32 cosIncidentAngle = MathTool::Saturate(dotProd(normal, toLight));
            F32 f0 = 1.0f - cosIncidentAngle;
            vector3 reflectPercent = r0 + (vector3::UNIT - r0) * (f0 * f0 * f0 * f0 * f0);
            return reflectPercent;
        }

        static vector3 BlinnPhone(const vector3& lightStrength, const vector3& diffuseAlbedo, const vector3& fresnelR0, const Types::F32 shiness, const vector3& toLight, const vector3& toEye, const vector3& normal)
        {
            using namespace Types;
            F32 m = shiness * 256.0f;
            vector3 halfVec = Normalize(toEye + toLight);
            F32 roughnessFactor = (m + 8.0f) * std::pow(std::max(dotProd(halfVec, normal), 0.0f), m);
            vector3 fresnelFactor = SchlickFresnel(vector3(fresnelR0.m_arr), normal, toLight);

            vector3 specAlbedo = fresnelFactor * roughnessFactor;
            specAlbedo = specAlbedo / (specAlbedo + vector3::UNIT);

            return (diffuseAlbedo + specAlbedo) * lightStrength;
        }

        static vector3 ComputePointLight(const LightBuffer& L, const vector3& diffuseAlbedo, const vector3& fresnelR0, const Types::F32 shiness, const vector3& posW, const vector3 normal, const vector3 toEye)
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

        static vector3 ComputeSpotLight(const LightBuffer& L, const vector3& diffuseAlbedo, const vector3& fresnelR0, const Types::F32 shiness, const vector3& posW, const vector3 normal, const vector3 toEye)
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
            std::unique_ptr<F32Buffer> vertexBuffer;
            std::vector<Types::U32> indices;
        };
        const Types::F32 
            LEFT_F      = -1.0f, RIGHT_F    = 1.0f, 
            BOTTOM_F    = -1.0f, TOP_F      = 1.0f, 
            NEAR_F      = -1.0f, FAR_F      = -10.0f;
        Transform perspect;
        std::array<ObjectInstance, 3> objInstances;
        std::array<ConstantBufferForInstance, 3> instanceBuffers;
        CameraFrame cameraFrame;
        ConstantBufferForCamera cameraBuffer;

        enum
        {
            M_CUBE = 0,
            M_CYLINDER,
            M_SPHERE,
            M_GEOSPHERE,
            NUM_SUPPORTED_MESH
        };
        std::array<SpecializedMeshData, NUM_SUPPORTED_MESH> prebuildMeshData;

        CommonRenderingBuffer()
        {
            perspect = Transform::PerspectiveOG(LEFT_F, RIGHT_F, BOTTOM_F, TOP_F, NEAR_F, FAR_F);

            Types::F32 pitch(3.14f * 3.f / 4.f), yaw(3.14f / 4.f), roll(0.f * 3.14f / 3.f);
            // 1
            objInstances[0].m_position = vector3(0.0f, 0.0f, 0.0f);
            objInstances[0].m_rotation = vector3(0, 0, 0);
            objInstances[0].m_scale = vector3(2.0f, 2.0f, 2.0f);
            // 2
            objInstances[1].m_position = vector3(1.0f, 1.4f, 0.0f);
            objInstances[1].m_rotation = vector3(pitch, yaw + 3.14f / 3, roll);
            objInstances[1].m_scale = vector3(1.5f, 1.5f, 1.5f);
            // 3
            objInstances[2].m_position = vector3(-1.0f, 2.8f, 0.0f);
            objInstances[2].m_rotation = vector3(pitch, yaw + 3.14f / 2.f, roll + 3.14f / 8.f);
            objInstances[2].m_scale = vector3(0.8f, 0.8f, 0.8f);

            RebuildInstanceBuffer();

            cameraFrame.m_origin = vector3(0.0f, 0.0f, 1.0f) * 3.0f;
            cameraFrame.m_lookAt = vector3::ZERO;
            RebuildCameraBuffer();

            // pre build meshes.
            BuildMeshDatas();
        }

        /*!
            \brief rebuild instanceBuffers from objInstances.
        */
        void RebuildInstanceBuffer()
        {
            for (unsigned int i = 0; i < instanceBuffers.size(); ++i)
            {
                instanceBuffers[i].m_toWorld = Transform::TRS(objInstances[i].m_position, objInstances[i].m_rotation, objInstances[i].m_scale);
                instanceBuffers[i].m_toWorldInverse = Transform::InverseTRS(objInstances[i].m_position, objInstances[i].m_rotation, objInstances[i].m_scale);
                instanceBuffers[i].m_material.m_diffuse = RGB::WHITE;// *0.5f;
                instanceBuffers[i].m_material.m_shiness = 16.0f;
                instanceBuffers[i].m_material.m_fresnelR0 = MaterialBuffer::FresnelR0_byReflectionIndex(4);
            }// end for
        }

        /*!
            \brief rebuild camera buffer by camera frame.
        */
        void RebuildCameraBuffer()
        {
            cameraFrame.RebuildFrameDetail();
            cameraBuffer.m_toCamera = cameraFrame.WorldToLocal();
            cameraBuffer.m_toCameraInverse = cameraFrame.LocalToWorld();
            cameraBuffer.m_camPos = cameraFrame.m_origin;
            cameraBuffer.m_project = perspect;
            cameraBuffer.m_numLights = 1;
            cameraBuffer.m_ambientColor = RGB::WHITE * RGB(0.05f, 0.05f, 0.05f);
            cameraBuffer.m_lights[0] = { vector3(0.0f, 10.0f, 0.0f), RGB::WHITE };
        }

        /*1
            \brief build mesh data for drawing.
        */
        void BuildMeshDatas()
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
                    vertices.push_back(SimplePoint(vertex.m_pos.ToHvector(), vertex.m_normal.ToHvector(0.0f), vertex.m_uv * 3.0f));
                }
                prebuildMeshData[i].indices = rawData.m_indices;
                prebuildMeshData[i].vertexBuffer = std::make_unique<F32Buffer>(vertices.size() * sizeof(decltype(vertices)::value_type));
                memcpy(prebuildMeshData[i].vertexBuffer->GetBuffer(), vertices.data(), prebuildMeshData[i].vertexBuffer->GetSizeOfByte());
            }
        }
    };

protected:
    // membbers
    int COMMON_PIXEL_WIDTH      = 512;
    int COMMON_PIXEL_HEIGHT     = 512;

    // render boundary can be used in orthographic camera.
    float COMMON_RENDER_LEFT    = -3.0f;
    float COMMON_RENDER_RIGHT   = +3.0f;
    float COMMON_RENDER_BOTTOM  = -3.0f;
    float COMMON_RENDER_TOP     = +3.0f;

    /*!
        \brief the random number generator.
    */
    RandomTool::MTRandom mtr;

    /*!
        \brief a common path for output, which will have a back slash at the end of the string.
    */
    const std::wstring OUTPUT_PATH = L"../OutputTestImage/";

public:
    CaseForPipline(const std::string& caseName) : Case(caseName) {}

    void Run() {}

    /*!
        \brief fill in the mesh data with sphere ray line list.
        \param outVetexData return vertex data
        \param outIndices return indices in line list.
    */
    void BuildSphereRayLineData(std::vector<SimplePoint>& outVertexData, std::vector<unsigned int>& outIndices)
    {
        std::vector<SimplePoint> points;
        std::vector<unsigned int> indices;
        unsigned int numIndices = 0;

        const float CENTER_X = 0.0f;
        const float CENTER_Y = 0.0f;
        const float SEGMENT_LENGTH = 0.3f;
        const float START_RADIUS = 0.3f;
        const Types::U32 NUM_ROUNDS = 12;
        const float RADIO_OFFSET = 0.0f; // Add this offset to every angle inside the sphereRay generating process.

                                         // create line segments in sphere ray.
        SphereRay([&numIndices, &outVertexData, &outIndices](HELP_SPHERE_RAY_LAMBDA_PARAMETERS)->void {

            const unsigned int theIndexOfOneLine = 64;
            // only get one line for convenience of debugging
            //if (lineIndex == theIndexOfOneLine)
            //{
            // add start vertex and its index
            SimplePoint start(hvector(x0, y0, 0.0f));
            start.m_rayIndex.m_x = static_cast<Types::F32>(roundIndex);
            start.m_rayIndex.m_y = static_cast<Types::F32>(lineIndex);
            start.m_rayIndex.m_z = 0.0f;
            outVertexData.push_back(start);
            outIndices.push_back(numIndices++);

            // add end vertex and its index
            SimplePoint end(hvector(x1, y1, 0.0f));
            end.m_rayIndex.m_x = static_cast<Types::F32>(roundIndex);
            end.m_rayIndex.m_y = static_cast<Types::F32>(lineIndex);
            end.m_rayIndex.m_z = 1.0f;
            outVertexData.push_back(end);
            outIndices.push_back(numIndices++);
            //}
        },
            CENTER_X, CENTER_Y, // center location
            SEGMENT_LENGTH,     // segment length
            START_RADIUS,       // start radius
            NUM_ROUNDS,         // num rounds
            RADIO_OFFSET        // radio offset
            );// end calling SphereRay()
    }// end BuildSphereRayTriangleMeshData()

    
    /*!
        \brief clamp the channel to [0.0f, 1.0f]
        \param ch the channel value to be clamped.
    */
    Types::F32 ClampChannel(Types::F32 ch)
    {
        if (ch <= 0.0f)
        {
            return 0.0f;
        }
        else if (ch >= 1.0f)
        {
            return 1.0f;
        }
        else
        {
            return ch;
        }
    }

    /*!
        \brief get random rgb color.
    */
    CommonClass::RGB GetRandomRGB()
    {
        return CommonClass::RGB(mtr.Random(), mtr.Random(), mtr.Random());
    }

    /*!
        \brief get random vector3.
        if you want to set the seed, try mtr.SetRandomSeed(...);
    */
    CommonClass::vector3 GetRandomVector3(bool allowZeroVector = true, const int MAX_INT = 200)
    {
        CommonClass::vector3 randVec;
        do
        {
            randVec = CommonClass::vector3(
                (mtr.Random() - 0.5f) * (mtr.Random(MAX_INT) + 1),
                (mtr.Random() - 0.5f) * (mtr.Random(MAX_INT) + 1),
                (mtr.Random() - 0.5f) * (mtr.Random(MAX_INT) + 1));

            // if allowZeroVector is false, loop until a none zero vector
        } while (!allowZeroVector && randVec.m_x == 0.0f && randVec.m_y == 0.0f && randVec.m_z == 0.0f);


        return randVec;
    }

    /*!
        \brief create a random float number between [-MAX_RADIUS, +MAX_RADIUS]
        \param MAX_RADIUS max absolute value of the float.
    */
    Types::F32 RandomFloat(const Types::F32 MAX_RADIUS = 200.0f)
    {
        assert(MAX_RADIUS > 0.0f);
        return (mtr.Random() - 0.5f) * 2.f * MAX_RADIUS;
    }
    
    /*!
        \brief create a float number between [-MAX_RADIUS, 0) ^ (0, +MAX_RADIUS]
        \param MAX_RADIUS max absolute value of the float.
    */
    Types::F32 RandomFloatNotZero(const Types::F32 MAX_RADIUS = 200.0f)
    {
        assert(MAX_RADIUS > 0.0f);
        float ret = 0.0f;
        do 
        {
            ret = RandomFloat(MAX_RADIUS);
        } while (ret == 0.0f);
        return ret;
    }

    /*!
        \brief create an array contain random float  [-200.0f, +200.0f]
        \param MAX_RADIUS max absolute value of the float.
    */
    template<unsigned int COUNT>
    std::array<Types::F32, COUNT> GetRandomFloatArray(const Types::F32 MAX_RADIUS = 200.0f)
    {
        assert(MAX_RADIUS > 0.0f);
        std::array<Types::F32, COUNT> retArr;
        for (auto& number : retArr)
        {
            number = RandomFloat(MAX_RADIUS);
        }
        return retArr;
    }
    
    /*!
        \brief create a float number between [-200.0f, 0) ^ (0, +200.0f]
        \param MAX_RADIUS max absolute value of the float.
    */
    template<unsigned int COUNT>
    std::array<Types::F32, COUNT> GetRandomFloatNotZeroArray(const Types::F32 MAX_RADIUS = 200.0f)
    {
        assert(MAX_RADIUS > 0.0f);
        std::array<Types::F32, COUNT> retArr;
        for (auto& number : retArr)
        {
            number = RandomFloatNotZero(MAX_RADIUS);
        }
        return retArr;
    }

    /*!
        \brief show the image and block the thread until user close the window.
        \param pImg the image to be shown
        \param title the title of the window
    */
    void BlockShowImg(Image * pImg, const std::wstring title) const
    {
        assert(pImg != nullptr);
        ImageWindow imgWnd(pImg, title);
        imgWnd.BlockShow();
    }

    /*!
        \brief create a quad poly.
        \param p1-4 four point position
        remember the clockwise is front face.
        A-------->
        |
        |   front order
        |
        <--------
    */
    auto CreatQuadPoly(const vector3 &p1, const vector3 &p2, const vector3 &p3, const vector3 &p4)
    {

        auto poly = std::make_unique<CommonClass::Polygon>(
            p1,
            p2,
            p3
            );

        poly->AddPoint(p4);

        return poly;
    }

    /*!
        \brief help to get point position in an array whose size is 8.
    */
    enum PointsOrder
    {
        TOP_LEFT_FRONT = 0,
        TOP_LEFT_BACK,
        TOP_RIGHT_BACK,
        TOP_RIGHT_FRONT,
        BOTTOM_LEFT_FRONT,
        BOTTOM_LEFT_BACK,
        BOTTOM_RIGHT_BACK,
        BOTTOM_RIGHT_FRONT
    };


    /*!
        \brief build six poly for a box
        \param points eight points to consist a box,
        \return 6 polygons which is visible from the outside of the box, polygons are in the order of {top, bottom, left, right, front, back}
        first four points is on the top, ranged in clockwise
        last four points is on the bottom, ranged ALSO in clockwise
        --------->
        A        |  \
        |  top   |   \
        |start   V    \
        *<--------     V
        \       ---------->
         \      A         |
          \     |   bottom|
           \    |start    V
            V   *<---------

    */
    std::array<std::unique_ptr<CommonClass::Polygon>, 6> CreatBox(const std::array<vector3, 8>& points)
    {
        std::array<std::unique_ptr<CommonClass::Polygon>, 6> boxPolys;

        // top
        boxPolys[0] = std::move(CreatQuadPoly(
            points[PointsOrder::TOP_LEFT_FRONT],
            points[PointsOrder::TOP_LEFT_BACK], 
            points[PointsOrder::TOP_RIGHT_BACK], 
            points[PointsOrder::TOP_RIGHT_FRONT]));

        // bottom
        boxPolys[1] = std::move(CreatQuadPoly(
            points[PointsOrder::BOTTOM_LEFT_BACK],
            points[PointsOrder::BOTTOM_LEFT_FRONT],
            points[PointsOrder::BOTTOM_RIGHT_FRONT],
            points[PointsOrder::BOTTOM_RIGHT_BACK]));

        // left
        boxPolys[2] = std::move(CreatQuadPoly(
            points[PointsOrder::TOP_LEFT_BACK],
            points[PointsOrder::TOP_LEFT_FRONT],
            points[PointsOrder::BOTTOM_LEFT_FRONT],
            points[PointsOrder::BOTTOM_LEFT_BACK]));

        // right
        boxPolys[3] = std::move(CreatQuadPoly(
            points[PointsOrder::TOP_RIGHT_FRONT],
            points[PointsOrder::TOP_RIGHT_BACK],
            points[PointsOrder::BOTTOM_RIGHT_BACK],
            points[PointsOrder::BOTTOM_RIGHT_FRONT]));

        // front
        boxPolys[4] = std::move(CreatQuadPoly(
            points[PointsOrder::TOP_LEFT_FRONT],
            points[PointsOrder::TOP_RIGHT_FRONT],
            points[PointsOrder::BOTTOM_RIGHT_FRONT],
            points[PointsOrder::BOTTOM_LEFT_FRONT]));

        // back
        boxPolys[5] = std::move(CreatQuadPoly(
            points[PointsOrder::TOP_RIGHT_BACK],
            points[PointsOrder::TOP_LEFT_BACK],
            points[PointsOrder::BOTTOM_LEFT_BACK],
            points[PointsOrder::BOTTOM_RIGHT_BACK]));

        return boxPolys;
    }

    /*!
        \brief get the path for output files.
    */
    virtual std::wstring GetStoragePath() const
    {
        return OUTPUT_PATH;
    }

    /*!
        \brief get safe storage path, if the path not exist, create it right now.
    */
    virtual std::wstring GetSafeStoragePath() const
    {
        auto path = GetStoragePath();
        if ( ! std::experimental::filesystem::exists(path))
        {
            std::experimental::filesystem::create_directories(path);
        }
        return path;
    }

    /*!
        \brief save the image to the sub folder with specific name, and show the image in a block thread with the title as same as the image.
        \param pipline the pipline to show
        \param nameOfImgAndWnd image name without .png and the window title at the same time
    */
    void SaveAndShowPiplineBackbuffer(const Pipline & pipline, const std::wstring& nameOfImgAndWindow) const
    {
        pipline.m_backBuffer->SaveTo(GetSafeStoragePath() + nameOfImgAndWindow + L".png");
        BlockShowImg(pipline.m_backBuffer.get(), nameOfImgAndWindow);
    }/*!
        \brief Build common PiplineStateObject for using.
    */
    std::unique_ptr<PiplineStateObject> GetCommonPSO()
    {
        auto pso = std::make_unique<PiplineStateObject>();
        pso->m_primitiveType = PrimitiveType::LINE_LIST;
        pso->m_vertexLayout.vertexShaderInputSize = sizeof(SimplePoint);
        pso->m_vertexLayout.pixelShaderInputSize = sizeof(SimplePoint);
        pso->SetViewport(GetCommonViewport());

        return std::move(pso);
    }

    /*!
        \brief Build common viewport by the default view size defined in CommonEnvironment.
    */
    Viewport GetCommonViewport()
    {
        Viewport viewport;
        viewport.left = 0.0f;
        viewport.right = static_cast<Types::F32>(COMMON_PIXEL_WIDTH - 1);
        viewport.bottom = 0.0f;
        viewport.top = static_cast<Types::F32>(COMMON_PIXEL_HEIGHT - 1);
        return viewport;
    }

    /*!
        \brief Build common Pipline for drawing, 
        all you need do is to set PixelShader/VertexShader, 
        and call drawing function with mesh data.
    */
    std::unique_ptr<Pipline> GetCommonPipline()
     {
        // create and set a pipline.
        auto pipline = std::make_unique<Pipline>();
        pipline->SetPSO(std::move(GetCommonPSO()));

        // set a back buffer
        pipline->SetBackBuffer(std::make_unique<RasterizeImage>(
            COMMON_PIXEL_WIDTH,
            COMMON_PIXEL_HEIGHT,
            RGBA::WHITE));

        return std::move(pipline);
    }

    /*!
        \brief get a vertex shader with TRS/perspectCamera/normalTransformation
    */
    static auto GetVertexShaderWithNormal(Transform& trs, Transform& perspect, Transform& normalTrs)
    {
        return [&trs, &perspect, &normalTrs](const unsigned char * pSrcVertex, ScreenSpaceVertexTemplate * pDestV)->void {
            const SimplePoint* pSrcH = reinterpret_cast<const SimplePoint*>(pSrcVertex);
            SimplePoint* pDestH = reinterpret_cast<SimplePoint*>(pDestV);

            hvector inViewPos = trs * pSrcH->m_position;

            pDestH->m_position = perspect * inViewPos;
            //pDestH->m_position = pSrcH->m_position;
            hvector normal = pSrcH->m_rayIndex;
            normal.m_w = 0.0f;
            pDestH->m_rayIndex = normalTrs * normal;
        };
    }

    /*!
        \brief get a useful vertex shader that require a ConstantBuffer for transformations.
    */
    static auto GetVertexShaderWithNormalAndConstantBuffer(ConstantBufferForInstance& constBufInstance, ConstantBufferForCamera& constBufCamera)
    {
        return [&constBufInstance, &constBufCamera](const unsigned char * pSrcVertex, ScreenSpaceVertexTemplate * pDestV)->void {
            const SimplePoint* pSrcH = reinterpret_cast<const SimplePoint*>(pSrcVertex);
            SimplePoint* pDestH = reinterpret_cast<SimplePoint*>(pDestV);

            hvector world  = constBufInstance.m_toWorld * pSrcH->m_position;
            hvector camera = constBufCamera.m_toCamera * world;

            pDestH->m_position = constBufCamera.m_project * camera;
            //pDestH->m_position = pSrcH->m_position;

            hvector normal = pSrcH->m_rayIndex;
            normal.m_w = 0.0f;// ensure translation will not affect calculations.
            Transform transformNormalToCamera = (constBufInstance.m_toWorldInverse * constBufCamera.m_toCameraInverse).T();// take transposes
            pDestH->m_rayIndex = transformNormalToCamera * normal;
        };
    }

    /*!
        \brief a vertex shader output a struct of VSOut
    */
    static auto GetVertexShaderWithVSOut(ConstantBufferForInstance& constBufInstance, ConstantBufferForCamera& constBufCamera)
    {
        return [&constBufInstance, &constBufCamera](const unsigned char * pSrcVertex, ScreenSpaceVertexTemplate * pDestV)->void {
            const SimplePoint* pSrcH = reinterpret_cast<const SimplePoint*>(pSrcVertex);
            VSOut* pDest = reinterpret_cast<VSOut*>(pDestV);

            pDest->m_uv = pSrcH->m_uv;
            
            pDest->m_posW = constBufInstance.m_toWorld * pSrcH->m_position;
            hvector camera = constBufCamera.m_toCamera * pDest->m_posW;

            pDest->m_posH = constBufCamera.m_project * camera;
            //pDestH->m_position = pSrcH->m_position;

            hvector normal = pSrcH->m_rayIndex;
            normal.m_w = 0.0f;// ensure translation will not affect calculations.
            Transform transformNormalToCamera = constBufInstance.m_toWorldInverse.T();// take transposes
            pDest->m_normalW = Normalize((transformNormalToCamera * normal).ToVector3()).ToHvector();
        };
    }

    /*!
        \brief cold to warm color shading.
        \param normal the normal of the pixel
        \param WarmDirection the warmest normal direction
    */
    static RGBA ColdToWarm(const vector3& normal, const vector3& WarmDirection = Normalize(vector3::UNIT))
    {
        Types::F32 kw = 0.5f * (1 + dotProd(normal, WarmDirection));
        RGB color = kw * RGB::BLUE + (1 - kw) * RGB::RED;
        return Cast(color);
    }

    /*!
        \brief pixel shader for vertex that have normal.
    */
    static auto GetPixelShaderWithNormal()
    {
        return [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
            const SimplePoint* pPoint = reinterpret_cast<const SimplePoint*>(pVertex);
            vector3 WarmDirection = Normalize(vector3(1.0f, 1.0f, 0.0f));
            vector3 normal = pPoint->m_rayIndex.ToVector3();
            return ColdToWarm(normal);
        };
    }

    /*!
        \brief a pixel shader require VSOut as input
    */
    static auto GetPixelShaderWithPSIn(ConstantBufferForInstance& constBufInstance, ConstantBufferForCamera& constBufCamera)
    {
        return [&constBufInstance, &constBufCamera](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
            const PSIn* pPoint = reinterpret_cast<const PSIn*>(pVertex);

            

            vector3 normal      = Normalize(pPoint->m_normalW.ToVector3());
            vector3 pixelPosW   = pPoint->m_posW.ToVector3();
            vector3 toEye       = Normalize(constBufCamera.m_camPos - pixelPosW);

            vector3 blinn = MaterialBuffer::ComputePointLight(
                constBufCamera.m_lights[0],
                vector3(constBufInstance.m_material.m_diffuse.m_arr),
                vector3(constBufInstance.m_material.m_fresnelR0.m_arr),
                constBufInstance.m_material.m_shiness,
                pixelPosW, 
                normal, 
                toEye);

            RGBA color(blinn.m_arr);
            return color;
        };
    }

    /*!
        \brief a pixel shader require one texture.
    */
    static auto GetPixelShaderWithPSInAndTexture(ConstantBufferForInstance& constBufInstance, ConstantBufferForCamera& constBufCamera, std::shared_ptr<Texture>& texture)
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

            vector3 resultColor = MaterialBuffer::ComputePointLight(
                constBufCamera.m_lights[0],
                vector3(diffuse.m_arr),
                vector3(constBufInstance.m_material.m_fresnelR0.m_arr),
                constBufInstance.m_material.m_shiness,
                pixelPosW,
                normal,
                toEye);

            if (constBufCamera.m_numLights == 2)
            {
                vector3 blinnSpot = MaterialBuffer::ComputeSpotLight(
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

    /*!
        \brief a pixel shader require one texture, whose sampling color will be used to disturb the normal of original geometry.
    */
    static auto GetPixelShaderWithNoiseBumpMap(ConstantBufferForInstance& constBufInstance, ConstantBufferForCamera& constBufCamera, std::shared_ptr<Texture>& texture)
    {
        return [&constBufInstance, &constBufCamera, &texture](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
            assert(texture->IsValid());
            const PSIn* pPoint = reinterpret_cast<const PSIn*>(pVertex);

            // get noise normal from texture.
            vector2 uv = pPoint->m_uv;
            RGBA sampleColor = texture->Sample(uv.m_x, uv.m_y);
            vector3 noiseNormal(sampleColor.m_chas.m_r, sampleColor.m_chas.m_g, sampleColor.m_chas.m_b);
            // channels of noise color all lay in [0,1], map them to [-1, 1]
            noiseNormal = noiseNormal * 2.0f - vector3::UNIT;
            noiseNormal = Normalize(noiseNormal);
            //noiseNormal = noiseNormal * 2.0f;// enhance the noise power.
            vector3 normal = Normalize(noiseNormal + pPoint->m_normalW.ToVector3());

            vector3 pixelPosW = pPoint->m_posW.ToVector3();
            vector3 toEye = Normalize(constBufCamera.m_camPos - pixelPosW);

            vector3 blinn = MaterialBuffer::ComputePointLight(
                constBufCamera.m_lights[0],
                vector3(constBufInstance.m_material.m_diffuse.m_arr),
                vector3(constBufInstance.m_material.m_fresnelR0.m_arr),
                constBufInstance.m_material.m_shiness,
                pixelPosW,
                normal,
                toEye);

            RGBA color(blinn.m_arr);
            return color;
        };
    }
};

/*!
    \Brief base suit for pipline test, this class will have an special environment, 
    to fully take use of this suit, please inherit CaseForPipline to write you own case.
*/
template<typename ... CASE_TYPE_LIST>
class SuitForPipline: public TestSuit::Suit<CASE_TYPE_LIST...>
{
public:
};

