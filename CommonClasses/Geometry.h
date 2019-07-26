#pragma once
#include <cmath>
#include <ostream>
#include "Assert.h"

namespace rt
{
    template <typename T> class Vector2
    {
        Vector2()
        {
            x = y = 0;
        }
        Vector2(T xx, T yy)
            :x(xx), y(yy)
        {
            Assert(!HasNaNs());
        }
        bool HasNaNs() const
        {
            return std::isnan(x) || std::isnan(y);
        }

        /*explicit Vector2(const Point2<T> &p);
        explicit Vector2(const Point3<T> &p);*/

#ifndef NDEBUG
        Vector2(const Vector2<T> &v)
        {
            Assert(!v.HasNaNs());
            x = v.x; y = v.y;
        }
        Vector2<T> &operator=(const Vector2<T> &v)
        {
            Assert(!v.HasNaNs());
            x = v.x; y = v.y;
            return *this;
        }
#endif // !NDEBUG
        friend std::ostream& operator<<(std::ostream& os, const Vector2<T> &v)
        {
            os << "[" << v.x << ", " < v.y << "]";
            return os;
        }

        Vector2<T> operator+(const Vector2<T> &v) const
        {
            Assert(!v.HasNaNs());
            return Vector2(x + v.x, y + v.y);
        }
        Vector2<T>& operator+=(const Vector2<T> &v) const
        {
            Assert(!v.HasNaNs());
            x += v.x; y += v.y;
            return *this;
        }

        Vector2<T> operator-(const Vector2<T> &v) const
        {
            Assert(!v.HasNaNs());
            return Vector2(x - v.x, y - v.y);
        }
        Vector2<T>& operator-=(const Vector2<T> &v) const
        {
            Assert(!v.HasNaNs());
            x -= v.x; y -= v.y;
            return *this;
        }

        bool operator==(const Vector2<T> &v) const
        {
            return x == v.x && y == v.y;
        }
        bool operator!=(const Vector2<T> &v) const
        {
            return x != v.x || y != v.y;
        }

        Vector2<T> operator*(T f) const { return Vector2<T>(f * x, f * y); }
        Vector2<T> &operator*=(T f)
        {
            Assert(!std::isnan(f));
            x *= f; y *= f;
            return *this;
        }

        Vector2<T> operator/(T f) const { return Vector2<T>(f / x, f / y); }
        Vector2<T> &operator/=(T f)
        {
            Assert(!std::isnan(f));
            x /= f; y /= f;
            return *this;
        }

        Vector2<T> operator-() const { return Vector2<T>(-x, -y); }
        T operator[](int i) const
        {
            Assert(i >= 0 && i <= 1);
            if (i == 0) return x;
            return y;
        }

        T x, y;
    };
}
