#pragma once
#include "CommonTypes.h"

namespace CommonClass
{

/*!
    \brief a Fix Point Number is basically using a signed integal to represent a real number
*/

class FixPointNumber
{
private:
    /*!
        \brief the real number stored by an integer.
    */
    Types::I32 m_iNumber;

    static const unsigned short SHIFT_POINT = 16;
    static const unsigned int MAG = 1 << SHIFT_POINT;

public:
    /*!
        \brief default to be zero.
    */
    FixPointNumber();
    FixPointNumber(const Types::I32 iNumber);
    FixPointNumber(const Types::F32 fNumber);

    /*!
        \brief cast to a normal float pointing number.
    */
    Types::F32 ToFloat();
    
    ~FixPointNumber();

    friend FixPointNumber operator * (const FixPointNumber& a, const FixPointNumber& b);
};

FixPointNumber operator * (const FixPointNumber& a, const FixPointNumber& b);
//FixPointNumber operator / (const FixPointNumber& a, const FixPointNumber& b);
//FixPointNumber operator + (const FixPointNumber& a, const FixPointNumber& b);
//FixPointNumber operator - (const FixPointNumber& a, const FixPointNumber& b);

} // namespace CommonClass
