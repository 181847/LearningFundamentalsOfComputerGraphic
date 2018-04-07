#pragma once
#include "CommonTypes.h"
#include <type_traits>
#include <assert.h>
#include <MyTools\MathTool.h>

/*!
    \brief a helper struct to assist the ColorTemplate to access individual channels of a color.
    this struct support RGB three channels to read.
*/
struct Channel3
{
public:
    Types::F32 m_r, m_g, m_b;
};
static_assert(sizeof(Channel3) == sizeof(Types::F32) * 3, "size of Channel3 is wrong");


/*!
    \brief a helper struct to assist the ColorTemplate to access individual channels of a color.
    this struct support RGBA four channels to read.
*/
struct Channel4
    :public Channel3
{

public:

    Types::F32 m_a;
};
static_assert(sizeof(Channel4) == sizeof(Types::F32) * 4, "size of Channel4 is wrong");


/*!
    \brief a ColorTemplate is the template of the RGB and RGBA.
    the difference between them is whether they have alpha channel.
    the RGB type can be used to compute color,
    the RGBA type can be used to out put image file or do the blending works.
*/
template<bool HAVE_ALPHA>
class ColorTemplate
{
public:

    using ChannelType = typename std::conditional<HAVE_ALPHA, Channel4, Channel3>::type;

    /*!
        \brief use union to access data in seperate channels or array.
    */
    union
    {
        ChannelType m_chas;
        Types::F32 m_arr[HAVE_ALPHA ? 4 : 3];
    };
    
public:
    /*!
        \brief give the specialization class a alias of the CURRENT type.
    */
    using AliasOfThisType = ColorTemplate<HAVE_ALPHA>;

    enum
    {
        R = 0,
        G,
        B,
        A
    };

    /*!
        \brief store whether the ColorTemplate have alpha.
    */
    enum 
    {
        IS_ALPHA = HAVE_ALPHA
    };

    /*!
        \brief some useful constant value
    */
    const static Types::F32 MIN_CHANNEL_VALUE;
    const static Types::F32 MAX_CHANNEL_VALUE;
    const static Types::F32 HALF_CHANNEL_VALUE;
    const static Types::F32 ALPHA_CHANNEL_OPAQUE;
    const static Types::F32 ALPHA_CHANNEL_TRANSPARENT;

    /*!
        \brief useful constant colors, if the color have alpha, default to be opaque.
    */
    const static ColorTemplate<HAVE_ALPHA> RED;
    const static ColorTemplate<HAVE_ALPHA> GREEN;
    const static ColorTemplate<HAVE_ALPHA> BLUE;
    const static ColorTemplate<HAVE_ALPHA> YELLOW;
    const static ColorTemplate<HAVE_ALPHA> CYAN;
    const static ColorTemplate<HAVE_ALPHA> MAGENTA;
    const static ColorTemplate<HAVE_ALPHA> WHITE;
    const static ColorTemplate<HAVE_ALPHA> BLACK;

public:
    /*!
        \brief a constructor only for ColorTemplate<true> or RGBA
        \param r red channel, default to be zero
        \param g green channel, default to be zero
        \param b blue channel, default to be zero
        \param a alpha channel, default to be Opaque
    */
    template<typename T = Types::F32, typename std::enable_if<HAVE_ALPHA && std::is_same<T, Types::F32>::value>::type* = nullptr>
    ColorTemplate(
        const T& r = MIN_CHANNEL_VALUE,
        const T& g = MIN_CHANNEL_VALUE,
        const T& b = MIN_CHANNEL_VALUE,
        const T& a = ALPHA_CHANNEL_OPAQUE);

    
    /*!
        \brief a constructor only for ColorTemplate<false> or RGB
        \param r red channel, default to be zero
        \param g green channel, default to be zero
        \param b blue channel, default to be zero
    */
    template<typename T = Types::F32, typename std::enable_if< ! HAVE_ALPHA && std::is_same<T, Types::F32>::value>::type* = nullptr>
    ColorTemplate(
        const T& r = MIN_CHANNEL_VALUE,
        const T& g = MIN_CHANNEL_VALUE,
        const T& b = MIN_CHANNEL_VALUE);


    /*!
        \brief set individual channels, the template parameter ch can be ColorTemplate<...>::R/G/B/A or 0/1/2/3, but setting alpha channel on a RGB will cause a compile error.
        \param value the value to set the channel, it will be clampped into the avaliable interval.
    */
    template<Types::U8 ch>
    typename std::enable_if<(ch <= (2 + HAVE_ALPHA))>::type
        SetChannel(const Types::F32& value);
    
    /*!
        \brief clamp the value to [MIN_CHANNEL_VALUE, MAX_CHANNEL_VALUE].
        \param value the value will be clampped.
    */
    static Types::F32 ClampChannel(const Types::F32 & value);
};

/*!
    \brief is all component is excatly the same?
    float == float for all channels.
*/
template<bool HAVE_ALPHA>
bool operator == (const ColorTemplate<HAVE_ALPHA>& a, const ColorTemplate<HAVE_ALPHA>& b);

/*!
    \brief is there any channel different?
    if and return (float != float)
*/
template<bool HAVE_ALPHA>
bool operator != (const ColorTemplate<HAVE_ALPHA>& a, const ColorTemplate<HAVE_ALPHA>& b);

/*!
    \brief multiply on each channel.
*/
template<bool HAVE_ALPHA>
ColorTemplate<HAVE_ALPHA> operator * (const ColorTemplate<HAVE_ALPHA>& a, const ColorTemplate<HAVE_ALPHA>& b);

/*!
    \brief add on each channel.
*/
template<bool HAVE_ALPHA>
ColorTemplate<HAVE_ALPHA> operator + (const ColorTemplate<HAVE_ALPHA>& a, const ColorTemplate<HAVE_ALPHA>& b);

/*!
    \brief substract on each channel.
*/
template<bool HAVE_ALPHA>
ColorTemplate<HAVE_ALPHA> operator - (const ColorTemplate<HAVE_ALPHA>& a, const ColorTemplate<HAVE_ALPHA>& b);

/*!
    \brief multiply a scalar on each channel.
*/
template<bool HAVE_ALPHA>
ColorTemplate<HAVE_ALPHA> operator * (const ColorTemplate<HAVE_ALPHA>& a, const Types::F32& b);

/*!
    \brief multiply a scalar on each channel.
*/
template<bool HAVE_ALPHA>
ColorTemplate<HAVE_ALPHA> operator * (const Types::F32& b, const ColorTemplate<HAVE_ALPHA>& a);

/*!
    \brief divided a scalar on each channel.
*/
template<bool HAVE_ALPHA>
ColorTemplate<HAVE_ALPHA> operator / (const ColorTemplate<HAVE_ALPHA>& a, const Types::F32& by);

/*!
    \brief AlmostEqual can check whether two color is likely.
    \param a the color to be checked
    \param b the color to be checked
    \param ulp the presicion level, lower it is, more accurate result will be returned. Default to be 8.
*/
template<bool HAVE_ALPHA>
bool AlmostEqual(const ColorTemplate<HAVE_ALPHA>& a, const ColorTemplate<HAVE_ALPHA>& b, int ulp = 8);

#pragma region implement parts

template<bool HAVE_ALPHA>
const Types::F32    ColorTemplate<HAVE_ALPHA>::MIN_CHANNEL_VALUE = 0.0f;

template<bool HAVE_ALPHA>
const Types::F32    ColorTemplate<HAVE_ALPHA>::MAX_CHANNEL_VALUE = 1.0f;

template<bool HAVE_ALPHA>
const Types::F32    ColorTemplate<HAVE_ALPHA>::HALF_CHANNEL_VALUE = 0.5f;

template<bool HAVE_ALPHA>
const Types::F32    ColorTemplate<HAVE_ALPHA>::ALPHA_CHANNEL_OPAQUE = 1.0f;

template<bool HAVE_ALPHA>
const Types::F32    ColorTemplate<HAVE_ALPHA>::ALPHA_CHANNEL_TRANSPARENT = 0.0f;

template<bool HAVE_ALPHA>
const ColorTemplate<HAVE_ALPHA> ColorTemplate<HAVE_ALPHA>::RED      = ColorTemplate<HAVE_ALPHA>(ColorTemplate<HAVE_ALPHA>::MAX_CHANNEL_VALUE, ColorTemplate<HAVE_ALPHA>::MIN_CHANNEL_VALUE, ColorTemplate<HAVE_ALPHA>::MIN_CHANNEL_VALUE);

template<bool HAVE_ALPHA>
const ColorTemplate<HAVE_ALPHA> ColorTemplate<HAVE_ALPHA>::GREEN    = ColorTemplate<HAVE_ALPHA>(ColorTemplate<HAVE_ALPHA>::MIN_CHANNEL_VALUE, ColorTemplate<HAVE_ALPHA>::MAX_CHANNEL_VALUE, ColorTemplate<HAVE_ALPHA>::MIN_CHANNEL_VALUE);

template<bool HAVE_ALPHA>
const ColorTemplate<HAVE_ALPHA> ColorTemplate<HAVE_ALPHA>::BLUE     = ColorTemplate<HAVE_ALPHA>(ColorTemplate<HAVE_ALPHA>::MIN_CHANNEL_VALUE, ColorTemplate<HAVE_ALPHA>::MIN_CHANNEL_VALUE, ColorTemplate<HAVE_ALPHA>::MAX_CHANNEL_VALUE);

template<bool HAVE_ALPHA>
const ColorTemplate<HAVE_ALPHA> ColorTemplate<HAVE_ALPHA>::YELLOW   = ColorTemplate<HAVE_ALPHA>(ColorTemplate<HAVE_ALPHA>::MAX_CHANNEL_VALUE, ColorTemplate<HAVE_ALPHA>::MAX_CHANNEL_VALUE, ColorTemplate<HAVE_ALPHA>::MIN_CHANNEL_VALUE);

template<bool HAVE_ALPHA>
const ColorTemplate<HAVE_ALPHA> ColorTemplate<HAVE_ALPHA>::CYAN     = ColorTemplate<HAVE_ALPHA>(ColorTemplate<HAVE_ALPHA>::MIN_CHANNEL_VALUE, ColorTemplate<HAVE_ALPHA>::MAX_CHANNEL_VALUE, ColorTemplate<HAVE_ALPHA>::MAX_CHANNEL_VALUE);

template<bool HAVE_ALPHA>
const ColorTemplate<HAVE_ALPHA> ColorTemplate<HAVE_ALPHA>::MAGENTA  = ColorTemplate<HAVE_ALPHA>(ColorTemplate<HAVE_ALPHA>::MAX_CHANNEL_VALUE, ColorTemplate<HAVE_ALPHA>::MIN_CHANNEL_VALUE, ColorTemplate<HAVE_ALPHA>::MAX_CHANNEL_VALUE);

template<bool HAVE_ALPHA>
const ColorTemplate<HAVE_ALPHA> ColorTemplate<HAVE_ALPHA>::WHITE    = ColorTemplate<HAVE_ALPHA>(ColorTemplate<HAVE_ALPHA>::MAX_CHANNEL_VALUE, ColorTemplate<HAVE_ALPHA>::MAX_CHANNEL_VALUE, ColorTemplate<HAVE_ALPHA>::MAX_CHANNEL_VALUE);

template<bool HAVE_ALPHA>
const ColorTemplate<HAVE_ALPHA> ColorTemplate<HAVE_ALPHA>::BLACK    = ColorTemplate<HAVE_ALPHA>(ColorTemplate<HAVE_ALPHA>::MIN_CHANNEL_VALUE, ColorTemplate<HAVE_ALPHA>::MIN_CHANNEL_VALUE, ColorTemplate<HAVE_ALPHA>::MIN_CHANNEL_VALUE);

template<bool HAVE_ALPHA>
inline Types::F32 ColorTemplate<HAVE_ALPHA>::ClampChannel(const Types::F32 & value)
{
    if (value < MIN_CHANNEL_VALUE)
    {
        return MIN_CHANNEL_VALUE;
    }
    else if (value > MAX_CHANNEL_VALUE)
    {
        return MAX_CHANNEL_VALUE;
    }
    else
    {
        return value;
    }
}

template<bool HAVE_ALPHA>
template<typename T, typename std::enable_if<HAVE_ALPHA && std::is_same<T, Types::F32>::value>::type* >
inline ColorTemplate<HAVE_ALPHA>::ColorTemplate(const T & r, const T & g, const T & b, const T & a)
{
    m_arr[0] = r;
    m_arr[1] = g;
    m_arr[2] = b;
    m_arr[3] = a;
}

template<bool HAVE_ALPHA>
template<typename T, typename std::enable_if< !HAVE_ALPHA && std::is_same<T, Types::F32>::value>::type*>
inline ColorTemplate<HAVE_ALPHA>::ColorTemplate(const T & r, const T & g, const T & b)
{
    m_arr[0] = r;
    m_arr[1] = g;
    m_arr[2] = b;
}

template<bool HAVE_ALPHA>
template<Types::U8 ch>
typename std::enable_if<(ch <= (2 + HAVE_ALPHA))>::type ColorTemplate<HAVE_ALPHA>::SetChannel(const Types::F32 & value)
{
    m_arr[ch] = ColorTemplate<HAVE_ALPHA>::ClampChannel(value);
}

template<bool HAVE_ALPHA>
inline bool operator==(const ColorTemplate<HAVE_ALPHA>& a, const ColorTemplate<HAVE_ALPHA>& b)
{
    for (unsigned char i = 0; i < ( HAVE_ALPHA ? 4 : 3 ); ++i)
    {
        if (a.m_arr[i] != b.m_arr[i])
        {
            // break if any one channel is NOT the same.
            return false;
        }
    }
    return true;
}

template<bool HAVE_ALPHA>
inline bool operator!=(const ColorTemplate<HAVE_ALPHA>& a, const ColorTemplate<HAVE_ALPHA>& b)
{
    for (unsigned char i = 0; i < ( HAVE_ALPHA ? 4 : 3 ); ++i)
    {
        if (a.m_arr[i] != b.m_arr[i])
        {
            // break if any one channel is NOT the same.
            // return true because even a little different exist, then we can sure that this two color is different.
            return true;
        }
    }
    return false;
}

template<bool HAVE_ALPHA>
inline ColorTemplate<HAVE_ALPHA> operator*(const ColorTemplate<HAVE_ALPHA>& a, const ColorTemplate<HAVE_ALPHA>& b)
{
    ColorTemplate<HAVE_ALPHA> ret;
    for (unsigned char i = 0; i < ( HAVE_ALPHA ? 4 : 3 ); ++i)
    {
        ret.m_arr[i] = ColorTemplate<HAVE_ALPHA>::ClampChannel(a.m_arr[i] * b.m_arr[i]);
    }
    return ret;
}

template<bool HAVE_ALPHA>
inline ColorTemplate<HAVE_ALPHA> operator+(const ColorTemplate<HAVE_ALPHA>& a, const ColorTemplate<HAVE_ALPHA>& b)
{
    ColorTemplate<HAVE_ALPHA> ret;
    for (unsigned char i = 0; i < (HAVE_ALPHA ? 4 : 3); ++i)
    {
        ret.m_arr[i] = ColorTemplate<HAVE_ALPHA>::ClampChannel(a.m_arr[i] + b.m_arr[i]);
    }
    return ret;
}

template<bool HAVE_ALPHA>
inline ColorTemplate<HAVE_ALPHA> operator-(const ColorTemplate<HAVE_ALPHA>& a, const ColorTemplate<HAVE_ALPHA>& b)
{
    ColorTemplate<HAVE_ALPHA> ret;
    for (unsigned char i = 0; i < ( HAVE_ALPHA ? 4 : 3 ); ++i)
    {
        ret.m_arr[i] = ColorTemplate<HAVE_ALPHA>::ClampChannel(a.m_arr[i] - b.m_arr[i]);
    }
    return ret;
}

template<bool HAVE_ALPHA>
inline ColorTemplate<HAVE_ALPHA> operator*(const ColorTemplate<HAVE_ALPHA>& a, const Types::F32 & b)
{
    ColorTemplate<HAVE_ALPHA> ret;
    for (unsigned char i = 0; i < ( HAVE_ALPHA ? 4 : 3 ); ++i)
    {
        ret.m_arr[i] = ColorTemplate<HAVE_ALPHA>::ClampChannel(a.m_arr[i] * b);
    }
    return ret;
}

template<bool HAVE_ALPHA>
inline ColorTemplate<HAVE_ALPHA> operator*(const Types::F32 & b, const ColorTemplate<HAVE_ALPHA>& a)
{
    ColorTemplate<HAVE_ALPHA> ret;
    for (unsigned char i = 0; i < ( HAVE_ALPHA ? 4 : 3 ); ++i)
    {
        ret.m_arr[i] = ColorTemplate<HAVE_ALPHA>::ClampChannel(a.m_arr[i] * b);
    }
    return ret;
}

template<bool HAVE_ALPHA>
inline ColorTemplate<HAVE_ALPHA> operator/(const ColorTemplate<HAVE_ALPHA>& a, const Types::F32 & by)
{
    ColorTemplate<HAVE_ALPHA> ret;
    for (unsigned char i = 0; i < ( HAVE_ALPHA ? 4 : 3 ); ++i)
    {
        ret.m_arr[i] = ColorTemplate<HAVE_ALPHA>::ClampChannel(a.m_arr[i] / by);
    }
    return ret;
}

template<bool HAVE_ALPHA>
inline bool AlmostEqual(const ColorTemplate<HAVE_ALPHA>& a, const ColorTemplate<HAVE_ALPHA>& b, int ulp)
{
    for (unsigned char i = 0; i < ( HAVE_ALPHA ? 4 : 3 ); ++i)
    {
        if ( ! MathTool::almost_equal(a.m_arr[i], b.m_arr[i], ulp))
        {
            // NOT almost_equal, then return false.
            return false;
        }
    }
    return true;
}

#pragma endregion

/*!
    \brief cast function from RGBA to RGB.
    \param color the rgba color
*/
ColorTemplate<false> Cast(const ColorTemplate<true>& color);

/*!
    \brief cast function from RGB to RGBA.
    \param color rgb color
    \param alpha additional alpha channel default to be opaque
*/
ColorTemplate<true> Cast(const ColorTemplate<false>& color, const Types::F32 alpha = ColorTemplate<true>::ALPHA_CHANNEL_OPAQUE);

/*!
    \brief give the ColorTemplate names.
*/
using TRGB = ColorTemplate<false>;
using TRGBA = ColorTemplate<true>;
