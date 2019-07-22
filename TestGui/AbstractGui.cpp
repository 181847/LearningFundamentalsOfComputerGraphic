#include "AbstractGui.h"

InitState::InitState(ID3D11Device * pd, ID3D11DeviceContext * pdc)
    :pDevice(pd), pDeviceContext(pdc)
{
}

std::string ExePath() 
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos);
}

void AbstractGui::Init(std::shared_ptr<InitState> state)
{
    this->state = state;
}
