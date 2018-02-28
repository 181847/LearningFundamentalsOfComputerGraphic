#pragma once
#include "CommonTypes.h"

namespace CommonClass
{

// vector2 have two float component.
class vector2
{

public:
	union
	{
		struct 
		{
			Types::F32 m_x, m_y;
		};
		Types::F32 m_arr[2];
	};

private:
	const Types::F32 COMPARE_EPSILON = 1e-8f;

public:
	vector2();
	vector2(const Types::F32 & x, const Types::F32 & y);
	~vector2();

	bool operator == (const vector2 & cmp) const;
	bool operator != (const vector2 & cmp) const;
};

vector2    operator + (const vector2 & a, const vector2 & b);
vector2    operator - (const vector2 & a, const vector2 & b);
Types::F32 operator * (const vector2 & a, const vector2 & b);
Types::F32 dotProd    (const vector2 & a, const vector2 & b);	// same as operator *

}// namespace CommonClass
