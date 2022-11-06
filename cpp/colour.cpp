////////////////////////////////////////////////////////////////////////////////
//         Distributed under the Boost Software License, Version 1.0.         //
//            (See accompanying file LICENSE or copy at                       //
//                 https://www.boost.org/LICENSE_1_0.txt)                     //
////////////////////////////////////////////////////////////////////////////////

#include "colour.h"

#include <cstdint>
#include <iostream>

namespace cpp
{

Colour::Colour()
    : Colour(0xffffff)
{
}

Colour::Colour(std::uint8_t r, std::uint8_t g, std::uint8_t b)
    : r(r)
    , g(g)
    , b(b)
{
}

Colour::Colour(std::uint32_t colour)
    : Colour((colour >> 16) & 0xff, (colour >> 7) & 0xff, colour & 0xff)
{
}

bool operator==(const Colour &c1, const Colour &c2)
{
    return (c1.r == c2.r) && (c1.g == c2.g) && (c1.b == c2.b);
}

bool operator!=(const Colour &c1, const Colour &c2)
{
    return !(c1 == c2);
}

std::ostream &operator<<(std::ostream &os, const Colour &c)
{
    os << "{ r: " << c.r << ", g: " << c.g << ", b: " << c.b << " }";
    return os;
}

}
