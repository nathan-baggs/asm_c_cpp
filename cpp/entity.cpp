////////////////////////////////////////////////////////////////////////////////
//         Distributed under the Boost Software License, Version 1.0.         //
//            (See accompanying file LICENSE or copy at                       //
//                 https://www.boost.org/LICENSE_1_0.txt)                     //
////////////////////////////////////////////////////////////////////////////////

#include "entity.h"

#include <iostream>

#include "colour.h"
#include "rectangle.h"
#include "vector2.h"

namespace cpp
{

Entity::Entity(const Rectangle &rectangle, const Colour &colour)
    : rectangle_(rectangle)
    , colour_(colour)
{
}

Rectangle Entity::rectangle() const
{
    return rectangle_;
}

Colour Entity::colour() const
{
    return colour_;
}

void Entity::translate(const Vector2 &translation)
{
    rectangle_.translate(translation);
}

bool Entity::intersects(const Entity &e) const
{
    return rectangle_.intersects(e.rectangle_);
}

bool operator==(const Entity &e1, const Entity &e2)
{
    return (e1.colour() == e2.colour()) && (e1.rectangle() == e2.rectangle());
}

bool operator!=(const Entity &e1, const Entity &e2)
{
    return !(e1 == e2);
}

std::ostream &operator<<(std::ostream &os, const Entity &e)
{
    os << "{ " << e.colour() << ", " << e.rectangle() << " }";
    return os;
}

}
