#pragma once
#include "vector3.h"
#include "RGBA.h"

namespace CommonClass
{

class HitRecord
{
public:
	Types::F32	m_hitT;
	vector3		m_normal;
	RGBA		m_kAmbient;
	RGBA		m_kDiffuse;

	/*!
		\brief is the hit position is the back of the surface?
	*/
	bool        m_isBackFace = false;

public:
};

} // namespace CommonClass
