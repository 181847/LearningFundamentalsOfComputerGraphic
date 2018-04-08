#pragma once
#include "ColorTemplate.h"

namespace CommonClass
{

/*!
    \brief common material class
*/
class Material
{
public:
    /*!
        \brief diffuse coefficient
    */
    RGB     m_kDiffuse;

    /*!
        \brief fresnel coefficient when the degree is 0.
    */
    RGB     m_rFresnel_0;

public:
    Material();
    Material(const RGB& kDiffuse, const RGB& rFresnel_0);
    Material(const RGB& kDiffuse, const unsigned int& reflectIndex);
    ~Material();
};

} // namespace CommonClass
