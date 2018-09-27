#include "Material.h"


namespace CommonClass
{

Material::Material()
    :m_kDiffuse(RGB::WHITE), m_rFresnel_0(RGB::BLACK)
{
}

Material::Material(const vector3 & kDiffuse, const vector3 & rFresnel_0, const Types::F32 shinness)
    :m_kDiffuse(kDiffuse), m_rFresnel_0(rFresnel_0), m_shinness(shinness)
{
}

Material::Material(const vector3 & kDiffuse, const unsigned int & reflectIndex, const Types::F32 shinness)
    :m_kDiffuse(kDiffuse), m_shinness(shinness)
{
    SetRFresnel0(reflectIndex);
}

Material::~Material()
{
}

void Material::SetRFresnel0(const unsigned int & reflectIndex)
{
    Types::F32 fresenel0 = (1.0f * (reflectIndex - 1)) / (reflectIndex + 1);
    fresenel0 *= fresenel0; // square it
    m_rFresnel_0 = vector3(fresenel0, fresenel0, fresenel0);
}

CommonClass::vector3 Material::RFresnel(const Types::F32& cosTheta) const
{
    Types::F32 cosThetaPow5 = (1 - cosTheta);
    cosThetaPow5 = cosThetaPow5 * cosThetaPow5 * cosThetaPow5 * cosThetaPow5 * cosThetaPow5;
    return m_rFresnel_0 + (vector3::WHITE - m_rFresnel_0) * cosThetaPow5;
}

} // namespace CommonClass