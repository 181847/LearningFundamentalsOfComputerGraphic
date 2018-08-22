#include "EFloat.h"
#include <assert.h>
#include <array>
#include "Helpers.h"

namespace CommonClass
{
EFloat::EFloat()
    :EFloat(0.0f)
{
}

EFloat::EFloat(Types::F32 v, Types::F32 err)
:m_v(v)
{
    if (err == 0.0f)
    {
        m_low = m_high = v;
    }
    else
    {
        m_low = NextFloatDown(m_v - err);
        m_high = NextFloatUp(m_v + err);
    }

#ifdef EFLOAT_DEBUG
    m_preciseV = m_v;
    Check();
#endif // EFLOAT_DEBUG
}

EFloat& EFloat::operator=(const EFloat & ef)
{
    m_v = ef.m_v;
    m_low = ef.m_low;
    m_high = ef.m_high;
    
#ifdef EFLOAT_DEBUG
    m_preciseV = ef.m_preciseV;
#endif // EFLOAT_DEBUG

    return *this;
}

EFloat::~EFloat()
{
}

void EFloat::Check() const
{
    // ensure m_low and m_high is comparable
    if (!std::isinf(m_low)
        && !std::isnan(m_low)
        && !std::isinf(m_high)
        && !std::isnan(m_high)) 
    {
        assert(m_low <= m_high);
    }

#ifdef EFLOAT_DEBUG
    if (!std::isinf(m_v) && !std::isnan(m_v))
    {
        assert(m_low <= m_preciseV);
        assert(m_preciseV <= m_high);
    }
#endif // EFLOAT_DEBUG
}

Types::F32 EFloat::LowerBound() const
{
    return m_low;
}

Types::F32 EFloat::UpperBound() const
{
    return m_high;
}

EFloat::operator Types::F32() const
{
    return m_v;
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

EFloat operator+(const EFloat & ef1, const EFloat & ef2)
{
    EFloat r;
    
    r.m_v = ef1.m_v + ef2.m_v;
    r.m_low  = NextFloatDown(ef1.m_low + ef2.m_low);
    r.m_high = NextFloatUp(ef1.m_high + ef2.m_high);

#ifdef EFLOAT_DEBUG
    r.m_preciseV = ef1.m_preciseV + ef2.m_preciseV;
    r.Check();
#endif // EFLOAT_DEBUG
    
    return r;
}

EFloat operator-(const EFloat & ef1, const EFloat & ef2)
{
    EFloat r;

    r.m_v = ef1.m_v - ef2.m_v;
    r.m_low = NextFloatDown(ef1.m_low - ef2.m_high);
    r.m_high = NextFloatUp(ef1.m_high - ef2.m_low);

#ifdef EFLOAT_DEBUG
    r.m_preciseV = ef1.m_preciseV - ef2.m_preciseV;
    r.Check();
#endif // EFLOAT_DEBUG

    return r;
}

EFloat operator*(const EFloat & ef1, const EFloat & ef2)
{
    EFloat r;

    r.m_v = ef1.m_v * ef2.m_v;
    
    std::array<Types::F32, 4> ferr = {
        ef1.m_low  * ef2.m_low, ef1.m_low  * ef2.m_high,
        ef1.m_high * ef2.m_low, ef1.m_high * ef2.m_high};

    r.m_low = r.m_high = ferr[0];
    // set r.m_low = min(ferr), r.m_high = max(ferr)
    // note: here skip the first element which is setted as initialization.
    for (unsigned int i = 1; i < ferr.size(); ++i)
    {
        if (ferr[i] < r.m_low)
            r.m_low = ferr[i]; // update min
        else if (ferr[i] > r.m_high)
            r.m_high = ferr[i]; // update max
    }

    // expand error bound a little
    r.m_low  = NextFloatDown(r.m_low);
    r.m_high = NextFloatUp  (r.m_high);

#ifdef EFLOAT_DEBUG
    r.m_preciseV = ef1.m_preciseV * ef2.m_preciseV;
    r.Check();
#endif // EFLOAT_DEBUG

    return r;
}

EFloat operator/(const EFloat & ef1, const EFloat & ef2)
{
    EFloat r;

    r.m_v = ef1.m_v / ef2.m_v;

    if (ef2.m_low < 0.0f && ef2.m_high > 0.0f)
    {
        // if the bound straddle zero, the ef2 may be near zero, so set the error bound to [-inf, +inf].
        r.m_low  = - std::numeric_limits<Types::F32>::infinity();
        r.m_high = + std::numeric_limits<Types::F32>::infinity();
    }
    else
    {
        std::array<Types::F32, 4> ferr = {
            ef1.m_low / ef2.m_low, ef1.m_low / ef2.m_high,
            ef1.m_high / ef2.m_low, ef1.m_high / ef2.m_high };

        r.m_low = r.m_high = ferr[0];
        // set r.m_low = min(ferr), r.m_high = max(ferr)
        // note: here skip the first element which is setted as initialization.
        for (unsigned int i = 1; i < ferr.size(); ++i)
        {
            if (ferr[i] < r.m_low)
                r.m_low = ferr[i]; // update min
            else if (ferr[i] > r.m_high)
                r.m_high = ferr[i]; // update max
        }

        // expand error bound a little
        r.m_low = NextFloatDown(r.m_low);
        r.m_high = NextFloatUp(r.m_high);
    }

#ifdef EFLOAT_DEBUG
    r.m_preciseV = ef1.m_preciseV / ef2.m_preciseV;
    r.Check();
#endif // EFLOAT_DEBUG

    return r;
}

EFloat operator-(const EFloat & ef)
{
    EFloat r;
    r.m_v        = -ef.m_v;
    r.m_low      = -ef.m_high;
    r.m_high     = -ef.m_low;
#ifdef EFLOAT_DEBUG
    r.m_preciseV = -ef.m_preciseV;
    r.Check();
#endif // EFLOAT_DEBUG
    return r;
}

EFloat abs(const EFloat & ef)
{
    EFloat r;

    if (ef.m_low > 0.0f)
    {
        // the number (include the error bound) is all positive,
        // just return itself.
        return ef;
    }
    else if (ef.m_high < 0.0f)
    {
        // flip all the sign
        r.m_v        = -ef.m_v;
        r.m_low      = -ef.m_high;
        r.m_high     = -ef.m_low;
#ifdef EFLOAT_DEBUG
        r.m_preciseV = -ef.m_preciseV;
#endif // EFLOAT_DEBUG
    }
    else
    {
        // error bound straddles zero, so the lower bound will be cut to zero,
        // upper bound will be the max magnitude in (low and high)
        r.m_v        = std::abs(ef.m_v);
        r.m_low      = 0.0f;
        r.m_high     = std::max(-ef.m_low, ef.m_high); // this equal to high = max(abs(ef.low), abs(ef.high))
#ifdef EFLOAT_DEBUG
        r.m_preciseV = std::abs(ef.m_preciseV);
#endif // EFLOAT_DEBUG
    }

#ifdef EFLOAT_DEBUG
    r.Check();
#endif // EFLOAT_DEBUG
    return r;
}

EFloat sqrt(const EFloat & ef)
{
    EFloat r;
    r.m_v = std::sqrt(ef.m_v);
    r.m_low = NextFloatDown(std::sqrt(ef.m_low));
    r.m_high = NextFloatUp(std::sqrt(ef.m_high));
#ifdef EFLOAT_DEBUG
    r.m_preciseV = std::sqrt(ef.m_preciseV);
#endif // EFLOAT_DEBUG
    return r;
}

std::ostream & operator<<(std::ostream & out, const EFloat & ef)
{
    out << string_format("v = %f (%a) - [%f, %f]", ef.m_v, ef.m_v, ef.m_low, ef.m_high);;

#ifdef EFLOAT_DEBUG
    out << string_format("   precise = %lf", ef.m_preciseV);
#endif // EFLOAT_DEBUG

    return out;
}

} // namespace CommonClass
