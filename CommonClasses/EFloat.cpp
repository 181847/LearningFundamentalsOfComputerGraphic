#include "EFloat.h"

namespace CommonClass
{


EFloat::EFloat()
{
}


EFloat::~EFloat()
{
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
