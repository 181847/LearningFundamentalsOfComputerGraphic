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

public:
    ColorTemplate();

    /*!
        \brief this constructor is only avaliable for RGBA(or ColorTemplate<true>), accept four parameters for intializing RGBA channels.
        \param r red channel
        \param g green channel
        \param b blue channel
        \param a alpha channel, the type of this argument is complicated, 
                 it using the SFINAE to ensure only the ColorTemplate<true> instantiation version can use this 4 parameters constructor.
    */
    template<typename T = Types::F32>
    ColorTemplate(
        const T& r = MIN_CHANNEL_VALUE, 
        const T& g = MIN_CHANNEL_VALUE, 
        const T& b = MIN_CHANNEL_VALUE, 
        const typename std::enable_if<std::is_same<T, Types::F32>::value && HAVE_ALPHA, T>::type& // enable the type T only when T is Types::F32 and HAVE_ALPHA is TRUE.
                 a = ALPHA_CHANNEL_OPAQUE);

    template<typename T = Types::F32>
    ColorTemplate(
        const T& r = MIN_CHANNEL_VALUE, 
        const T& g = MIN_CHANNEL_VALUE, 
        const typename std::enable_if<std::is_same<T, Types::F32>::value && ! HAVE_ALPHA, T>::type& // enable the type T only when T is Types::F32 and HAVE_ALPHA is FALSE.
                 b = MIN_CHANNEL_VALUE);

    /*!
        \brief set one channel value
        \param value new value of one channel, this value will be clampped between [0.0f, 1.0f]
    */
    /*template<Types::U8 ch>
    void SetChannel(const Types::F32 & value);*/

    /*!
        \brief set channels for RGBA which enable four channel setting.
    */
    template<Types::U8 ch>
    void SetChannel(const typename std::enable_if<HAVE_ALPHA, Types::F32>::type& value);
    
    /*!
        \brief set channels for RGB which only allow three channels to set.
    */
    template<Types::U8 ch>
    void SetChannel(const typename std::enable_if< ( !HAVE_ALPHA && ch != ColorTemplate<HAVE_ALPHA>::A) , Types::F32>::type& value);

    
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
//template<bool HAVE_ALPHA>
//ColorTemplate<HAVE_ALPHA> operator * (const Types::F32& b, const ColorTemplate<HAVE_ALPHA>& a);

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
template<typename T>
inline ColorTemplate<HAVE_ALPHA>::ColorTemplate(const T& r, const T& g, const T& b, const typename std::enable_if<std::is_same<T, Types::F32>::value && HAVE_ALPHA, T>::type& a)
{
    m_arr[0] = r;
    m_arr[1] = g;
    m_arr[2] = b;
    m_arr[3] = a;
}

template<bool HAVE_ALPHA>
template<typename T>
inline ColorTemplate<HAVE_ALPHA>::ColorTemplate(const T& r, const T& g, const typename std::enable_if<std::is_same<T, Types::F32>::value && !HAVE_ALPHA, T>::type& b)
{
    m_arr[0] = r;
    m_arr[1] = g;
    m_arr[2] = b;
}

template<bool HAVE_ALPHA>
template<Types::U8 ch>
inline void ColorTemplate<HAVE_ALPHA>::SetChannel(const typename std::enable_if<HAVE_ALPHA, Types::F32>::type & value)
{
}

template<bool HAVE_ALPHA>
template<Types::U8 ch>
inline void ColorTemplate<HAVE_ALPHA>::SetChannel(const typename std::enable_if< (!HAVE_ALPHA && ch != ColorTemplate<HAVE_ALPHA>::A) , Types::F32>::type & value)
{
}

//template<>
//template<Types::U8 ch>
//void ColorTemplate<true>::SetChannel(const Types::F32 & value)
//{
//    const Types::F32 clamppedValue = ClampChannel(value);
//
//    switch (ch)
//    {
//    case AliasOfThisType::R:
//        m_chas.m_r = clamppedValue;
//        break;
//
//    case AliasOfThisType::G:
//        m_chas.m_g = clamppedValue;
//        break;
//
//    case AliasOfThisType::B:
//        m_chas.m_b = clamppedValue;
//        break;
//
//    case AliasOfThisType::A:
//        m_chas.m_a = clamppedValue;
//        break;
//
//    default:
//        assert(false && "unexpected channel to set");
//        break;
//    }
//}
//
//template<>
//template<Types::U8 ch>
//void ColorTemplate<false>::SetChannel(const Types::F32 & value)
//{
//    const Types::F32 clamppedValue = ClampChannel(value);
//
//    switch (ch)
//    {
//    case AliasOfThisType::R:
//        m_chas.m_r = clamppedValue;
//        break;
//
//    case AliasOfThisType::G:
//        m_chas.m_g = clamppedValue;
//        break;
//
//    case AliasOfThisType::B:
//        m_chas.m_b = clamppedValue;
//        break;
//
//    default:
//        assert(false && "unexpected channel to set");
//        break;
//    }
//}

//template<>
//template<>
//void ColorTemplate<false>::SetChannel<ColorTemplate<false>::A>(const Types::F32& value)
//{
//    static_assert(false, "ColorTemplate<false> means RGB, it has no alpha channel, please don't set alpha channel on it!");
//}

template<bool HAVE_ALPHA>
inline ColorTemplate<HAVE_ALPHA>::ColorTemplate()
{
    // set RGB channels to 1.0f.
    for (unsigned char i = 0; i < (HAVE_ALPHA ? 4 : 3); ++i)
    {
        m_arr[i] = MAX_CHANNEL_VALUE;
    }
}

template<bool HAVE_ALPHA>
Types::F32 ColorTemplate<HAVE_ALPHA>::ClampChannel(const Types::F32 & value)
{
    if (value < MIN_CHANNEL_VALUE)
    {
        return 0.0f;
    }
    else if (value > MAX_CHANNEL_VALUE)
    {
        return 1.0f;
    }
    else
    {
        return value;
    }
}

template<bool HAVE_ALPHA>
inline bool operator==(const ColorTemplate<HAVE_ALPHA>& a, const ColorTemplate<HAVE_ALPHA>& b)
{
    for (unsigned char i = 0; i < HAVE_ALPHA ? 4 : 3; ++i)
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
    for (unsigned char i = 0; i < HAVE_ALPHA ? 4 : 3; ++i)
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
    for (unsigned char i = 0; i < HAVE_ALPHA ? 4 : 3; ++i)
    {
        ret.m_arr[i] = ClampChannel(a.m_arr[i] * b.m_arr[i]);
    }
    return ret;
}

template<bool HAVE_ALPHA>
inline ColorTemplate<HAVE_ALPHA> operator+(const ColorTemplate<HAVE_ALPHA>& a, const ColorTemplate<HAVE_ALPHA>& b)
{
    ColorTemplate<HAVE_ALPHA> ret;
    for (unsigned char i = 0; i < HAVE_ALPHA ? 4 : 3; ++i)
    {
        ret.m_arr[i] = ClampChannel(a.m_arr[i] + b.m_arr[i]);
    }
    return ret;
}

template<bool HAVE_ALPHA>
inline ColorTemplate<HAVE_ALPHA> operator-(const ColorTemplate<HAVE_ALPHA>& a, const ColorTemplate<HAVE_ALPHA>& b)
{
    ColorTemplate<HAVE_ALPHA> ret;
    for (unsigned char i = 0; i < HAVE_ALPHA ? 4 : 3; ++i)
    {
        ret.m_arr[i] = ClampChannel(a.m_arr[i] - b.m_arr[i]);
    }
    return ret;
}

template<bool HAVE_ALPHA>
inline ColorTemplate<HAVE_ALPHA> operator*(const ColorTemplate<HAVE_ALPHA>& a, const Types::F32 & b)
{
    return ColorTemplate<HAVE_ALPHA>();
}

template<bool HAVE_ALPHA>
inline bool AlmostEqual(const ColorTemplate<HAVE_ALPHA>& a, const ColorTemplate<HAVE_ALPHA>& b, int ulp)
{
    for (unsigned char i = 0; i < HAVE_ALPHA ? 4 : 3; ++i)
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
    \brief give the ColorTemplate names.
*/
using TRGB = ColorTemplate<false>;
using TRGBA = ColorTemplate<true>;
