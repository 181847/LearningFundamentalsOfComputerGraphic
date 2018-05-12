#pragma once
#include "CommonTypes.h"
#include <ostream>

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
    /*!
        \brief default construct 0.0f +- 0
    */
    EFloat();

    /*!
        \brief construct a EFloat with an absolute error
        \param v value of the float
        \param error absolute error, should greater than 0
        low <= v - err
        v + err <= high
        Warning!! Here we don't handle the situation when error bound get overflow or underflow.
    */
    EFloat(Types::F32 v, Types::F32 err = 0.0f);

    /*!
        \brief copy assignment.
    */
    EFloat& operator = (const EFloat& ef);
    ~EFloat();

    /*!
        \brief check m_low < m_high (neighter should be infinity or nan) 
        In the EFLOAT_DEBUG mode, will check the (low < m_preciseV && m_prciseV < high) 
    */
    void Check() const;

    Types::F32 LowerBound() const;
    Types::F32 UpperBound() const;

    explicit operator Types::F32() const;

    friend EFloat operator + (const EFloat& ef1, const EFloat& ef2);
    friend EFloat operator - (const EFloat& ef1, const EFloat& ef2);
    friend EFloat operator * (const EFloat& ef1, const EFloat& ef2);
    friend EFloat operator / (const EFloat& ef1, const EFloat& ef2);
    friend EFloat operator - (const EFloat& ef);
    friend EFloat abs        (const EFloat& ef1);
    friend EFloat sqrt       (const EFloat& ef);
    friend std::ostream& operator << (std::ostream& out, const EFloat& ef);
};

/*!
    \brief plus operation test
    \param ef1, ef2 operate numbers
*/
EFloat operator + (const EFloat& ef1, const EFloat& ef2);

/*!
    \brief minus operation test
    \param ef1, ef2 operate numbers
*/
EFloat operator - (const EFloat& ef1, const EFloat& ef2);

/*!
    \brief multiply operation test
    \param ef1, ef2 operate numbers
*/
EFloat operator * (const EFloat& ef1, const EFloat& ef2);

/*!
    \brief divide operation test
    \param ef1, ef2 operate numbers
*/
EFloat operator / (const EFloat& ef1, const EFloat& ef2);

/*!
    \brief negative of the error float
*/
EFloat operator - (const EFloat& ef);

/*!
    \brief absolute operation
    \param ef the error float
    note: if ef = { 0.004 -> [ -0.005, 0.0043] }
          will return {0.004 -> [0, 0.005]}
*/
EFloat abs(const EFloat& ef);

/*!
    \brief square root of the error float
*/
EFloat sqrt(const EFloat& ef);

/*!
    \brief output the EFloat number to console.
*/
std::ostream& operator << (std::ostream& out, const EFloat& ef);

}// namespace CommonClass
