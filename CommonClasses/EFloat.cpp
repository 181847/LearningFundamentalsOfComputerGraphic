#include "EFloat.h"
#include <assert.h>

namespace CommonClass
{


EFloat::EFloat(Types::F32 v, Types::F32 err)
    :m_v(v)
{
    if (err == 0.0f)
    {
        m_low = m_high = 0.0f;
    }
    else
    {
        m_low = NextFloatDown(v - err);
        m_high = NextFloatUp(v + err);
    }

#ifdef EFLOAT_DEBUG
    m_preciseV = m_v;
    Check();
#endif // EFLOAT_DEBUG
}

EFloat::~EFloat()
{
}

void EFloat::Check()
{
    // ensure m_low and m_high is comparable
    if (!std::isinf(m_low)
        && !std::isnan(m_low)
        && !std::isinf(m_high)
        && !std::isnan(m_high)) 
    {
        assert(m_low < m_high);
    }

#ifdef EFLOAT_DEBUG
    if (!std::isinf(m_v) && !std::isnan(m_v))
    {
        assert(m_low < m_preciseV);
        assert(m_preciseV < m_high);
    }
#endif // EFLOAT_DEBUG
}

Types::U32 FloatToBits(const Types::F32 & f)
{
    Types::U32 u;
    memcpy(&u, &f, sizeof(Types::F32));
    return u;
}

Types::F32 BitsToFloat(const Types::U32 & u)
{
    Types::F32 f;
    memcpy(&f, &u, sizeof(Types::U32));
    return f;
}

Types::F32 NextFloatUp(Types::F32 f)
{
    if (std::isinf(f) && f > 0.0f) return f; // just return the positive infinity
    if (f == -0.0f) f = 0.0f;
    Types::U32 u = FloatToBits(f);
    if (f >= 0.0f)
    {
        ++u;
    }
    else
    {
        --u;
    }

    return BitsToFloat(u);
}

Types::F32 NextFloatDown(Types::F32 f)
{
    if (std::isinf(f) && f < 0.0f) return f; // just return the negative infinity
    if (f == 0.0f) f = -0.0f;
    Types::U32 u = FloatToBits(f);
    if (f <= 0.0f)
    {
        ++u;
    }
    else
    {
        --u;
    }

    return BitsToFloat(u);
}

} // namespace CommonClass
