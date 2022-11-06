////////////////////////////////////////////////////////////////////////////////
//         Distributed under the Boost Software License, Version 1.0.         //
//            (See accompanying file LICENSE or copy at                       //
//                 https://www.boost.org/LICENSE_1_0.txt)                     //
////////////////////////////////////////////////////////////////////////////////

#include "vector2.h"

#include <iostream>

namespace cpp
{

Vector2::Vector2()
    : Vector2(0.0f, 0.0f)
{
}

Vector2::Vector2(float x, float y)
    : x(x)
    , y(y)
{
}

bool operator==(const Vector2 &v1, const Vector2 &v2)
{
    return (v1.x == v2.x) && (v1.y == v2.y);
}

bool operator!=(const Vector2 &v1, const Vector2 &v2)
{
    return !(v1 == v2);
}

Vector2 operator+(const Vector2 &v1, const Vector2 &v2)
{
    Vector2 new_vec{v1};
    new_vec += v2;
    return new_vec;
}

Vector2 &operator+=(Vector2 &v1, const Vector2 &v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    return v1;
}

std::ostream &operator<<(std::ostream &os, const Vector2 &v)
{
    os << "{ x: " << v.x << ", y: " << v.y << "}";
    return os;
}

}
