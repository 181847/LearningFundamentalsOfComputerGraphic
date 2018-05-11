#include "FixPointNumber.h"


namespace CommonClass
{
FixPointNumber::FixPointNumber()
    :m_iNumber(0)
{
}

    CommonClass::FixPointNumber::FixPointNumber(const Types::I32 iNumber)
{
    m_iNumber = iNumber << SHIFT_POINT;
}

FixPointNumber::FixPointNumber(const Types::F32 fNumber)
{
    float scaleResult = fNumber * MAG;
    m_iNumber = static_cast<Types::I32>(scaleResult);
}

Types::F32 FixPointNumber::ToFloat()
{
    const Types::F32 ret = static_cast<Types::F32>(m_iNumber) / MAG;
    return ret;
}

FixPointNumber::~FixPointNumber()
{
}

FixPointNumber operator*(const FixPointNumber & a, const FixPointNumber & b)
{
    const long long 
        al = static_cast<long long>(a.m_iNumber), 
        bl = static_cast<long long>(b.m_iNumber);
    
    long long scaleResult = al * bl;
    scaleResult >>= FixPointNumber::SHIFT_POINT;

    FixPointNumber ret;
    ret.m_iNumber = static_cast<Types::I32>(scaleResult);
    return ret;
}

} // namespace CommonClass
