
#include <array>
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
extern "C"
{
#include <stb/stb_image.h>
}

namespace CommonClass
{

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
    return true;
}

RGBA Texture::Sample(const Types::F32 u, const Types::F32 v, const SampleState& sampleState /*= SampleState()*/)
{
    using namespace Types;
    F32 fu, fv;
    sampleState.FixUV(u, v, fu, fv);

    U32 sx, sy;
    sx = static_cast<U32>(m_width  * fu);
    sy = static_cast<U32>(m_height * fv);

    std::array<RGBA, 4> colors = {RGBA(), RGBA() ,RGBA(), RGBA()};
    std::array<F32,  4> comps;

    colors[0] = this->GetPixel(sx,      sy      );
    colors[1] = this->GetPixel(sx + 1,  sy      );
    colors[2] = this->GetPixel(sx,      sy + 1  );
    colors[3] = this->GetPixel(sx + 1,  sy + 1  );

    F32 uInp, vInp;
    uInp = m_width  * u - std::floor(m_width  * u);
    vInp = m_height * v - std::floor(m_height * v);

    for (unsigned int i = 0; i < 4; ++i)
    {
        comps[i] = 
                (1.0f - uInp)   * (1.0f - vInp) * colors[0].m_arr[i] 
            +   uInp            * (1.0f - vInp) * colors[1].m_arr[i] 
            +   (1.0f - uInp)   * vInp          * colors[2].m_arr[i] 
            +   uInp            * vInp          * colors[3].m_arr[i];
    }// end for
    
    RGBA retRgba;
    retRgba.SetChannel<0>(comps[0]);
    retRgba.SetChannel<1>(comps[1]);
    retRgba.SetChannel<2>(comps[2]);
    retRgba.SetChannel<3>(comps[3]);
    return retRgba;
}

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

}// namespace CommonClass
