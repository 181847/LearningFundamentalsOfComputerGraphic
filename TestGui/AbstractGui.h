#pragma once
#include <d3d11.h>
#include <memory>
#include <string>

class InitState
{
public:
    ID3D11Device*           pDevice = nullptr;
    ID3D11DeviceContext*    pDeviceContext = nullptr;

    InitState(ID3D11Device* pd, ID3D11DeviceContext* pdc);
};

std::string ExePath();

/*!
    \brief each AbstractGui can have render() to draw imgui staff, 
    can be stack into the global App instance to render each of them.
*/
class AbstractGui
{
public:
    virtual void Init(std::shared_ptr<InitState> state);
    virtual void Render() = 0;
    virtual void CleanUp() = 0;

protected:
    std::shared_ptr<InitState> state;
};
