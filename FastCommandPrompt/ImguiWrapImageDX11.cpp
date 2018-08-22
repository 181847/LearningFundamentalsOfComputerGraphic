#include "ImguiWrapImageDX11.h"
#include <assert.h>
#include <exception>


ImguiWrapImageDX11::ImguiWrapImageDX11()
{
}


ImguiWrapImageDX11::ImguiWrapImageDX11(ImguiWrapImageDX11&& other)
{
    this->m_pTexture = other.m_pTexture;
    this->m_pSRV = other.m_pSRV;

    this->m_width = other.m_width;
    this->m_height = other.m_height;

    other.m_pTexture = nullptr;
    other.m_pSRV = nullptr;
}

ImguiWrapImageDX11& ImguiWrapImageDX11::operator=(ImguiWrapImageDX11&& other)
{
    this->m_pTexture = other.m_pTexture;
    this->m_pSRV = other.m_pSRV;

    this->m_width = other.m_width;
    this->m_height = other.m_height;

    other.m_pTexture = nullptr;
    other.m_pSRV = nullptr;

    return *this;
}

ImguiWrapImageDX11::~ImguiWrapImageDX11()
{
    Release();
}

bool ImguiWrapImageDX11::SetImageRawData(ID3D11Device* pDevice, const unsigned char * pRawData, unsigned int WIDTH, unsigned int HEIGHT)
{
    assert(pDevice 
        && pRawData 
        && WIDTH
        && HEIGHT
        && "argument error");

    m_width = WIDTH;
    m_height = HEIGHT;

    const unsigned int PIXEL_SIZE = 4;// bytes

    Release();// release data.

    // create texture
    D3D11_TEXTURE2D_DESC Desc;
    Desc.Width = WIDTH;
    Desc.Height = HEIGHT;
    Desc.MipLevels = 1;
    Desc.ArraySize = 1;
    Desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    Desc.SampleDesc.Count = 1;
    Desc.SampleDesc.Quality = 0;
    Desc.Usage = D3D11_USAGE_DEFAULT;
    Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    Desc.CPUAccessFlags = 0;
    Desc.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA InitializeData;
    InitializeData.pSysMem = pRawData;
    InitializeData.SysMemPitch = Desc.Width * PIXEL_SIZE;
    InitializeData.SysMemSlicePitch = Desc.Width * Desc.Height * PIXEL_SIZE;
    if (FAILED(pDevice->CreateTexture2D(&Desc, &InitializeData, &m_pTexture)))
    {
        throw std::exception("create texture failed");
        return false;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
    viewDesc.Format = Desc.Format;
    viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    viewDesc.Texture2D.MipLevels = 1;
    viewDesc.Texture2D.MostDetailedMip = 0;
    if (FAILED(pDevice->CreateShaderResourceView(m_pTexture, &viewDesc, &m_pSRV)))
    {
        m_pTexture->Release();
        throw std::exception("create shader resource view of texture failed");
        return false;
    }

    return true;
}

ID3D11ShaderResourceView * ImguiWrapImageDX11::GetSRV()
{
    return m_pSRV;
}

void ImguiWrapImageDX11::Release()
{
    // reset texture data.
    if (m_pSRV)
    {
        m_pSRV->Release();
        m_pSRV = nullptr;
    }
    if (m_pTexture)
    {
        m_pTexture->Release();
        m_pTexture = nullptr;
    }
}
