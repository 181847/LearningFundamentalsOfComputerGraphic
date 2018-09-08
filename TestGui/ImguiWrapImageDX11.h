#pragma once

#include <d3d11.h>
#include <stdio.h>
#include <exception>

// Helper class for COM exceptions
// reference from: https://github.com/Microsoft/DirectXTK/wiki/ThrowIfFailed
class com_exception : public std::exception
{
public:
    com_exception(HRESULT hr) : result(hr) {}

    virtual const char* what() const override
    {
        static char s_str[64] = {};
        sprintf_s(s_str, "Failure with HRESULT of %08X",
            static_cast<unsigned int>(result));
        return s_str;
    }

private:
    HRESULT result;
};

// Helper utility converts D3D API failures into exceptions.
inline void ThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        throw com_exception(hr);
    }
}

/*!
    \brief a wrapper class help to create texture from pixel datas.
*/
class ImguiWrapImageDX11
{
public:
    /*!
        \brief the texture in dx11
    */
    ID3D11Texture2D* m_pTexture = nullptr;

    /*!
        \brief SRV of the texture
    */
    ID3D11ShaderResourceView* m_pSRV = nullptr;

    /*!
        \brief image raw size.
    */
    unsigned int m_width;
    unsigned int m_height;

    /*!
        \brief bytes of each pixel.
    */
    const int m_sizePerPixel = 4;

    bool m_isValide = false;

public:
    ImguiWrapImageDX11();
    ImguiWrapImageDX11(ImguiWrapImageDX11&& other);
    ImguiWrapImageDX11(const ImguiWrapImageDX11&) = delete;
    ImguiWrapImageDX11& operator=(const ImguiWrapImageDX11&) = delete;
    ~ImguiWrapImageDX11();

    /*!
        \brief create texture data in dx11,
        release previous data if have.
    */
    bool SetImageRawData(ID3D11Device* pDevice, const unsigned char * pRawData, unsigned int WIDTH, unsigned int HEIGHT);

    /*!
        \brief only update the image data, 
    */
    void UpdateImageRowData(ID3D11DeviceContext* pDeviceContext, const unsigned char * pRawData, unsigned int WIDTH, unsigned int HEIGHT);
    
    /*!
        \brief get the shader resource view of the texture.
    */
    ID3D11ShaderResourceView* GetSRV();

    /*!
        \brief release all the data, and set pointer to nullptr.
    */
    void Release();
};

