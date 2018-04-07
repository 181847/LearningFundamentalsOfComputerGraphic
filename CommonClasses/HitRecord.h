#pragma once
#include "vector3.h"

namespace CommonClass
{

class HitRecord
{
public:
	Types::F32	m_hitT;
	vector3		m_hitPoint;
	vector3		m_normal;

	/*!
		\brief is the hit position is the back of the surface?
	*/
	bool        m_isBackFace = false;

public:
};

} // namespace CommonClass
