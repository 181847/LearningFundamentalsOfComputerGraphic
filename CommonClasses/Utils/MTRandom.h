#pragma once

//========================================================================
//
//  "Mersenne Twister pseudorandom number generator" 
//  Original Code written by Takuji Nishimura and Makoto Matsumoto
//
//========================================================================
// Original Code from the book <<Game Code Complete: 4th>> by Mike McShaffry. 

/* Period parameters */
#undef MAXINT

namespace
{

const unsigned int CMATH_N = 624;
const unsigned int CMATH_M = 397;
const unsigned int CMATH_MATRIX_A = 0x9908b0df;   /* constant vector a */
const unsigned int CMATH_UPPER_MASK = 0x80000000; /* most significant w-r bits */
const unsigned int CMATH_LOWER_MASK = 0x7fffffff; /* least significant r bits */

const unsigned int CMATH_TEMPERING_MASK_B = 0x9d2c5680;
const unsigned int CMATH_TEMPERING_MASK_C = 0xefc60000;

const int CMATH_MAXINT = int(~unsigned int(0) >> 1);

}

#define CMATH_TEMPERING_SHIFT_U(y)  (y >> 11)
#define CMATH_TEMPERING_SHIFT_S(y)  (y << 7)
#define CMATH_TEMPERING_SHIFT_T(y)  (y << 15)
#define CMATH_TEMPERING_SHIFT_L(y)  (y >> 18)

namespace RandomTool
{

class MTRandom
{
private:
	// DATA
	unsigned int		m_rseed;
	unsigned int		m_rseed_sp;
	unsigned long		m_mt[CMATH_N]; /* the array for the state vector  */
	int					m_mti; /* m_mti==N+1 means m_mt[N] is not initialized */

			 // FUNCTIONS
public:
	MTRandom();

	unsigned int	Random(unsigned int n);
	float			Random();
	void			SetRandomSeed(unsigned int n);
	unsigned int	GetRandomSeed(void);
	void			Randomize(void);
};

}// namespace RandomTool
