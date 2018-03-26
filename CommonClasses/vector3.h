#pragma once
#include "CommonTypes.h"

namespace CommonClass
{

/*!
	\brief a vector in 2D space
*/
class vector3
{

public:
	union
	{
		struct
		{
			Types::F32 m_x, m_y, m_z;
		};
		Types::F32 m_arr[3];
	};

	/*!
		\brief decide the precision of the almost equal floats
	*/
	static const Types::I32 COMPARE_ULP = 8;

public:
	vector3();
	vector3(const Types::F32 & x, const Types::F32 & y, const Types::F32 & z);
	~vector3();
};

vector3    operator + (const vector3 & a, const vector3 & b);
vector3    operator - (const vector3 & a, const vector3 & b);
bool       operator ==(const vector3 & a, const vector3 & b);
bool       operator !=(const vector3 & a, const vector3 & b);
Types::F32 operator * (const vector3 & a, const vector3 & b);

/*!
	\brief compute the dot product of two vector
*/
Types::F32 dotProd    (const vector3 & a, const vector3 & b);	// same as operator *

/*!
	\brief compute the cross product of two 3D vector
*/
vector3    crossProd  (const vector3 & a, const vector3 & b);

/*!
	\brief normalize the vector3
*/
vector3	   Normalize(const vector3 & a);

/*!
	\brief get length of the vector.
*/
Types::F32 Length(const vector3 & a);

/*!
	\brief two vector are almost equal
	\param a compared vector
	\param b compared vector
	\param ulp the precision of compare, lower and more accurate
*/
bool AlmostEqual(const vector3 & a, const vector3 & b, int ulp = vector3::COMPARE_ULP);

}// namespace CommonClass
