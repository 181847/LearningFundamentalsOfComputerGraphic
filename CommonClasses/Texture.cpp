
#include <array>
#include "Utils/MathTool.h"
#include "Utils/MTRandom.h"
#include "Texture.h"
#include "vector3.h"

#define STB_IMAGE_IMPLEMENTATION
extern "C"
{
#include "Utils/stb_image.h"
}

namespace CommonClass
{

void SampleState::FixUV(const Types::F32 u, const Types::F32 v, Types::F32& outU, Types::F32& outV) const
{
    if (m_wrapMode == SampleState::Loop)
    {
        outU = u - std::floor(u);
        outV = v - std::floor(v);
    }
    else
    {
        throw std::logic_error("unsupported wrap mode");
    }
}

bool Texture::LoadFile(const std::string& file)
{
    assert(!file.empty());

    int x, y, n = 0;
    unsigned char *data = stbi_load(file.c_str(), &x, &y, &n, 4);
    if (!data)
    {
        throw std::exception("texture load failed.");
        printf("texture load file failed: %s\n", file.c_str());
        return false;
    }

    Init(x, y); // resize image memory, discard previous data.
    unsigned char * restoreAddr = GetRawData(); // get destination ptr
    memcpy(restoreAddr, data, x * y * 4);
    stbi_image_free(data);

    // update byte pixels to float pixels.
    this->BytePixelToFloatPixel();
    return true;
}

CommonClass::vector4 Texture::Sample(const Types::F32 u, const Types::F32 v, const SampleState& sampleState /*= SampleState()*/)
{
    using namespace Types;
    F32 fu, fv;
    sampleState.FixUV(u, v, fu, fv);

    U32 sx, sy;
    sx = static_cast<U32>((m_width - 2)  * fu);
    sy = static_cast<U32>((m_height - 2) * fv);

    std::array<vector4, 4> colors;

    colors[0] = this->GetPixel(sx,      sy      );
    colors[1] = this->GetPixel(sx + 1,  sy      );
    colors[2] = this->GetPixel(sx,      sy + 1  );
    colors[3] = this->GetPixel(sx + 1,  sy + 1  );

    F32 uInp, vInp;
    uInp = m_width  * u - std::floor(m_width  * u);
    vInp = m_height * v - std::floor(m_height * v);

    vector4 sampledResult;
    for (unsigned int i = 0; i < 4; ++i)
    {
        sampledResult = 
                (1.0f - uInp)   * (1.0f - vInp) * colors[0] 
            +   uInp            * (1.0f - vInp) * colors[1]
            +   (1.0f - uInp)   * vInp          * colors[2]
            +   uInp            * vInp          * colors[3];
    }// end for
    
    return sampledResult;
}

/*!
    \brief this is a function only for PerlinNoise.
*/
vector3 GammaVector(const Types::U32 i, const Types::U32 j, const Types::U32 k)
{
    using namespace Types;
    const U32 NUM_PRECOMPUTED_VEC = 256;
    static std::vector<vector3> precomputeVecList;
    static std::vector<U32> randomIndices;

    if (precomputeVecList.empty())
    {// pre compute random vectors and indices.
        precomputeVecList.resize(NUM_PRECOMPUTED_VEC);
        randomIndices    .resize(NUM_PRECOMPUTED_VEC);

        RandomTool::MTRandom mtr;
        for (auto& vec : precomputeVecList)
        {
            vector3 randomVecInSphere;
            std::array<F32, 3> sigma;

            do {
                // three sigma valus
                for (auto& sig : sigma) { sig = mtr.Random(); }

                // set vector3
                for (U32 i = 0; i < sigma.size(); ++i)
                {
                    randomVecInSphere.m_arr[i] = 2.0f * sigma[i] - 1.0f;
                }
            } while (Length(randomVecInSphere) >= 1.0f);
            // loop until get a random vector inside the unit sphere

            // store the normalized random vector.
            vec = Normalize(randomVecInSphere);
        }

        // index
        for (U32 index = 0; index < randomIndices.size(); ++index)
        {
            randomIndices[index] = index;
        }
        std::random_shuffle(randomIndices.begin(), randomIndices.end());
    }

    assert(precomputeVecList.size() == NUM_PRECOMPUTED_VEC && randomIndices.size() == NUM_PRECOMPUTED_VEC);

    U32 vecIndex;
    vecIndex = randomIndices[k              % NUM_PRECOMPUTED_VEC];
    vecIndex = randomIndices[(vecIndex + j) % NUM_PRECOMPUTED_VEC];
    vecIndex = randomIndices[(vecIndex + i) % NUM_PRECOMPUTED_VEC];

    assert(vecIndex < NUM_PRECOMPUTED_VEC);

    return precomputeVecList[vecIndex];
}

Types::F32 Texture::PerlinNoise(const Types::F32 x, const Types::F32 y /*= 0.0f*/, const Types::F32 z /*= 0.0f*/)
{
    using namespace Types;
    U32 lx = static_cast<U32>(x);
    U32 ly = static_cast<U32>(y);
    U32 lz = static_cast<U32>(z);
    vector3 sampleLoc(x, y, z);
    vector3 tVec(x - lx, y - ly, z - lz);

    for (U32 i = 0; i < 3; ++i)
    {
        F32 t = tVec.m_arr[i];
        tVec.m_arr[i] = t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
    }

    std::array<F32, 8> cornerIntensities;
    // help to get value of corner by three component(0/1)
#define CORNER_VALUE(cix, ciy, ciz) cornerIntensities[((cix << 2) + (ciy << 1) + ciz)]

    for (U32 dx = 0; dx < 2; ++dx)
    {
        for (U32 dy = 0; dy < 2; ++dy)
        {
            for (U32 dz = 0; dz < 2; ++dz)
            {
                vector3 cornerLoc = vector3(static_cast<F32>(lx + dx), static_cast<F32>(ly + dy), static_cast<F32>(lz + dz));
                vector3 randomVec = GammaVector(lx + dx, ly + dy, lz + dz);
                vector3 deltaLoc = sampleLoc - cornerLoc;
                CORNER_VALUE(dx, dy, dz) = dotProd(randomVec, deltaLoc);
            }// end for
        }// end for
    }// end for

    //  |\ Y+   /| Z+
    //  |      /                              
    //  |     /                               
    //  |    /                                
    //  |   /                                 
    //  |  /                                  
    //  | /                                   
    //  O------------->  X+ 
    return 
    MathTool::Lerp(// lerp z
        MathTool::Lerp(// lerp y
            MathTool::Lerp(CORNER_VALUE(0, 0, 0), CORNER_VALUE(1, 0, 0), tVec.m_x),
            MathTool::Lerp(CORNER_VALUE(0, 1, 0), CORNER_VALUE(1, 1, 0), tVec.m_x),
            tVec.m_y),
        MathTool::Lerp(// lerp y
            MathTool::Lerp(CORNER_VALUE(0, 0, 1), CORNER_VALUE(1, 0, 1), tVec.m_x),
            MathTool::Lerp(CORNER_VALUE(0, 1, 1), CORNER_VALUE(1, 1, 1), tVec.m_x),
            tVec.m_y),
        tVec.m_z);

#undef CORNER_VALUE
}

Types::F32 Texture::PerlinNoise(const CommonClass::vector2& xy, const Types::F32 z /*= 0.0f*/)
{
    return PerlinNoise(xy.m_x, xy.m_y, z);
}

Types::F32 Texture::PerlinNoise(const CommonClass::vector3& xyz)
{
    return PerlinNoise(xyz.m_x, xyz.m_y, xyz.m_z);
}

CommonClass::vector3 Texture::NoiseVector3(const Types::F32 x, const Types::F32 y /*= 0.0f*/, const Types::F32 z /*= 0.0f*/)
{
    using namespace Types;
    U32 lx = static_cast<U32>(x);
    U32 ly = static_cast<U32>(y);
    U32 lz = static_cast<U32>(z);
    vector3 sampleLoc(x, y, z);
    vector3 tVec(x - lx, y - ly, z - lz);

    for (U32 i = 0; i < 3; ++i)
    {
        F32 t = tVec.m_arr[i];
        tVec.m_arr[i] = t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
    }

    std::array<vector3, 8> cornerVecs;
    // help to get value of corner by three component(0/1)
#define CORNER_VEC(cix, ciy, ciz) cornerVecs[((cix << 2) + (ciy << 1) + ciz)]

    for (U32 dx = 0; dx < 2; ++dx)
    {
        for (U32 dy = 0; dy < 2; ++dy)
        {
            for (U32 dz = 0; dz < 2; ++dz)
            {
                vector3 cornerLoc = vector3(static_cast<F32>(lx + dx), static_cast<F32>(ly + dy), static_cast<F32>(lz + dz));
                vector3 randomVec = GammaVector(lx + dx, ly + dy, lz + dz);
                vector3 deltaLoc = sampleLoc - cornerLoc;
                CORNER_VEC(dx, dy, dz) = dotProd(randomVec, deltaLoc) * randomVec;
            }// end for
        }// end for
    }// end for

     //  |\ Y+   /| Z+
     //  |      /                              
     //  |     /                               
     //  |    /                                
     //  |   /                                 
     //  |  /                                  
     //  | /                                   
     //  O------------->  X+ 
    return
        MathTool::Lerp(// lerp z
            MathTool::Lerp(// lerp y
                MathTool::Lerp(CORNER_VEC(0, 0, 0), CORNER_VEC(1, 0, 0), tVec.m_x),
                MathTool::Lerp(CORNER_VEC(0, 1, 0), CORNER_VEC(1, 1, 0), tVec.m_x),
                tVec.m_y),
            MathTool::Lerp(// lerp y
                MathTool::Lerp(CORNER_VEC(0, 0, 1), CORNER_VEC(1, 0, 1), tVec.m_x),
                MathTool::Lerp(CORNER_VEC(0, 1, 1), CORNER_VEC(1, 1, 1), tVec.m_x),
                tVec.m_y),
            tVec.m_z);

#undef CORNER_VEC
}

vector3 Texture::NoiseVector3(const CommonClass::vector2& xy, const Types::F32 z /*= 0.0f*/)
{
    return NoiseVector3(xy.m_x, xy.m_y, z);
}

CommonClass::vector3 Texture::NoiseVector3(const CommonClass::vector3& xyz)
{
    return NoiseVector3(xyz.m_x, xyz.m_y, xyz.m_z);
}

}// namespace CommonClass
