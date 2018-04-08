#include "Material.h"


namespace CommonClass
{

Material::Material()
    :m_kDiffuse(RGB::WHITE), m_rFresnel_0(RGB::BLACK)
{
}

Material::Material(const RGB & kDiffuse, const RGB & rFresnel_0)
    :m_kDiffuse(kDiffuse), m_rFresnel_0(rFresnel_0)
{
}

Material::Material(const RGB & kDiffuse, const unsigned int & reflectIndex)
    :m_kDiffuse(kDiffuse)
{
    Types::F32 fresenel0 = ( 1.0f * (reflectIndex - 1) ) / (reflectIndex + 1);
    fresenel0 *= fresenel0; // square it
    m_rFresnel_0 = RGB(fresenel0, fresenel0, fresenel0);
}

Material::~Material()
{
}

} // namespace CommonClass