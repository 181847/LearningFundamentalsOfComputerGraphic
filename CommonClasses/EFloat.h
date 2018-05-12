#pragma once
#include "CommonTypes.h"

#define EFLOAT_DEBUG


/*!
    \brief this file define many accurancy analysis tool
    main of the codes is copy from the
    ¡¶Physical Based Rendering, 3rd¡·
*/

namespace CommonClass
{
   
/*!
    \brief copy bits data from float to bits
    \param f the 4 bytes floating point number
    \return the unsigned int type of the float.
*/
Types::U32 FloatToBits(const Types::F32 & f);

/*!
    \brief copy bits data from uint32 to construct a float
    \param u the uint32 number, maybe the return value of FloatToBits
    \return the float number of the same data bits
*/
Types::F32 BitsToFloat(const Types::U32 & u);

/*!
    \brief find the nearest float greater than the parameter
    \param the f the float of base
    \return if f is +inf ,return +inf
            the -0.0 is same as 0.0, and will return a positive number
*/
Types::F32 NextFloatUp(Types::F32 f);

/*!
    \brief find the nearest float less than the parameter
    \param the f the float of base
    \return if f is -inf ,return -inf
            the -0.0 is same as 0.0, and will return a negative number
*/
Types::F32 NextFloatDown(Types::F32 f);

/*!
    \brief EFloat is for error float,
    this is a float type which have a error bound abount the value,
    and the error bound will be expended by the operations:
    -plus, minus, multiply, divided, square root
    Reference from the EFloat in the book
    ¡¶Physical Based Rendering 3rd¡·
*/
class EFloat
{
public:
    
    Types::F32
        m_v;    // main value of the float 

private:
    Types::F32
        m_low,    // lower bound
        m_high;   // higher bound

#ifdef EFLOAT_DEBUG
    /*!
        \brief the precise value store in long double type
    */
    long double m_preciseV;
#endif
    
public:
    EFloat(Types::F32 v, Types::F32 err = 0.0f);
    ~EFloat();

    /*!
        \brief check m_low < m_high (neighter should be infinity or nan) 
        In the EFLOAT_DEBUG mode, will check the (low < m_preciseV && m_prciseV < high) 
    */
    void Check();
};

}// namespace CommonClass
