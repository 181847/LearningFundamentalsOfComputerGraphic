#include "MTRandom.h"
#include <time.h>
#include <basetsd.h>



/////////////////////////////////////////////////////////////////////////////
// DEBUG info
/////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------
namespace RandomTool
{

MTRandom::MTRandom(void)
	:m_rseed(1), m_rseed_sp(0), m_mti(CMATH_N + 1)
{
}

// Returns a number from 0 to n (excluding n)
unsigned int MTRandom::Random(unsigned int n)
{
	unsigned long y;
	static unsigned long mag01[2] = { 0x0, CMATH_MATRIX_A };

	if (n == 0)
		return(0);

	/* mag01[x] = x * MATRIX_A  for x=0,1 */

	if (m_mti >= CMATH_N) { /* generate N words at one time */
		int kk;

		if (m_mti == CMATH_N + 1)   /* if sgenrand() has not been called, */
			SetRandomSeed(4357); /* a default initial seed is used   */

		for (kk = 0; kk<CMATH_N - CMATH_M; kk++) {
			y = (m_mt[kk] & CMATH_UPPER_MASK) | (m_mt[kk + 1] & CMATH_LOWER_MASK);
			m_mt[kk] = m_mt[kk + CMATH_M] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		for (; kk<CMATH_N - 1; kk++) {
			y = (m_mt[kk] & CMATH_UPPER_MASK) | (m_mt[kk + 1] & CMATH_LOWER_MASK);
			m_mt[kk] = m_mt[kk + (CMATH_M - CMATH_N)] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		y = (m_mt[CMATH_N - 1] & CMATH_UPPER_MASK) | (m_mt[0] & CMATH_LOWER_MASK);
		m_mt[CMATH_N - 1] = m_mt[CMATH_M - 1] ^ (y >> 1) ^ mag01[y & 0x1];

		m_mti = 0;
	}

	y = m_mt[m_mti++];
	y ^= CMATH_TEMPERING_SHIFT_U(y);
	y ^= CMATH_TEMPERING_SHIFT_S(y) & CMATH_TEMPERING_MASK_B;
	y ^= CMATH_TEMPERING_SHIFT_T(y) & CMATH_TEMPERING_MASK_C;
	y ^= CMATH_TEMPERING_SHIFT_L(y);

	// ET - old engine added one to the result.
	// We almost NEVER wanted to use this function
	// like this.  So, removed the +1 to return a 
	// range from 0 to n (not including n).
	return (y%n);
}


float MTRandom::Random()
{
	float r = (float)Random(CMATH_MAXINT);
	float divisor = (float)CMATH_MAXINT;
	return (r / divisor);
}



void MTRandom::SetRandomSeed(unsigned int n)
{
	/* setting initial seeds to mt[N] using         */
	/* the generator Line 25 of Table 1 in          */
	/* [KNUTH 1981, The Art of Computer Programming */
	/*    Vol. 2 (2nd Ed.), pp102]                  */
	m_mt[0] = n & 0xffffffff;
	for (m_mti = 1; m_mti<CMATH_N; m_mti++)
		m_mt[m_mti] = (69069 * m_mt[m_mti - 1]) & 0xffffffff;

	m_rseed = n;
}

unsigned int MTRandom::GetRandomSeed(void)
{
	return(m_rseed);
}

void MTRandom::Randomize(void)
{
	SetRandomSeed((unsigned int)time(NULL));
}

}// namespace RandomTool

//--------------------------------------------------------------------------------
// EOF
