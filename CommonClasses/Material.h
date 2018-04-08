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

    Types::F32 m_shinness = 2;

public:
    Material();
    Material(const RGB& kDiffuse, const RGB& rFresnel_0);
    Material(const RGB& kDiffuse, const unsigned int& reflectIndex);
    ~Material();

    /*!
        \brief set reflect fresnel coefficient by reflect index.
        \param reflectIndex the reflect index respect to air (whose reflect index is one).
    */
    void SetRFresnel0(const unsigned int& reflectIndex);

    /*!
        \brief get reflect Fresnel coefficient respect to cosTheta,
        \param cosThera cosin of the angle of normal and view ray.
          normal
        \   A   /eye
         \  |  /
          \ |_/
           \|/
        -------------
        
    */
    RGB RFresnel(const Types::F32& cosTheta);
};

} // namespace CommonClass
