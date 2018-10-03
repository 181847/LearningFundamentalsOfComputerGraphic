#pragma once
#include "ColorTemplate.h"
#include "vector3.h"

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
    vector3     m_kDiffuse;

    /*!
        \brief fresnel coefficient when the degree is 0.
    */
    vector3     m_rFresnel_0;

    Types::F32 m_shinness = 2;

    Types::F32 m_reflectIndex = 1.0f;

    vector3 m_attenuation;

private:

    bool m_isDielectirc = false;

public:
    Material();
    Material(const vector3& kDiffuse, const vector3& rFresnel_0, const Types::F32 shinness = 2.0f);
    Material(const vector3& kDiffuse, const unsigned int& reflectIndex, const Types::F32 shinness = 2.0f);
    ~Material();

    /*!
        \brief set reflect fresnel coefficient by reflect index.
        \param reflectIndex the reflect index respect to air (whose reflect index is one).
    */
    void SetRFresnel0(const Types::F32& reflectIndex);

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
    vector3 RFresnel(const Types::F32& cosTheta) const;

    /*!
        \brief set if this material is dielectric (e.g. transparent)
        \param attenuationInside how much light indensity attenuated per 1 unit length.
    */
    void SetDielectric(bool isDielectric, const vector3& attenuationInside = vector3(0, 0, 0));

    /*!
        \brief if support dielectric, for example transparent material
    */
    bool IsDielectric();
};

} // namespace CommonClass
