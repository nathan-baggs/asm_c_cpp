////////////////////////////////////////////////////////////////////////////////
//         Distributed under the Boost Software License, Version 1.0.         //
//            (See accompanying file LICENSE or copy at                       //
//                 https://www.boost.org/LICENSE_1_0.txt)                     //
////////////////////////////////////////////////////////////////////////////////

#include "rectangle.h"

#include <iostream>

#include "vector2.h"

namespace cpp
{

Rectangle::Rectangle(const Vector2 &position, float width, float height)
    : position(position)
    , width(width)
    , height(height)
{
}

void Rectangle::translate(const Vector2 &translation)
{
    position += translation;
}

bool Rectangle::intersects(const Rectangle &rect) const
{
    return (
        (position.x < rect.position.x + rect.width) && (position.x + width > rect.position.x) &&
        (position.y < rect.position.y + rect.height) && (height + position.y > rect.position.y));
}

bool operator==(const Rectangle &r1, const Rectangle &r2)
{
    return (r1.position == r2.position) && (r1.width == r2.width) && (r1.height == r2.height);
}

bool operator!=(const Rectangle &r1, const Rectangle &r2)
{
    return !(r1 == r2);
}

std::ostream &operator<<(std::ostream &os, const Rectangle &rect)
{
    os << "{ position: " << rect.position << ", w: " << rect.width << ", h: " << rect.height << " }";
    return os;
}

}
