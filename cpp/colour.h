////////////////////////////////////////////////////////////////////////////////
//         Distributed under the Boost Software License, Version 1.0.         //
//            (See accompanying file LICENSE or copy at                       //
//                 https://www.boost.org/LICENSE_1_0.txt)                     //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <iosfwd>

namespace cpp
{

/**
 * A Colour represents an RGB component (where each component is 8 bits).
 */
class Colour
{
  public:
    /**
     * Construct a new white colour.
     */
    Colour();

    /**
     * Construct a new colour from r, g and b components.
     *
     * @param r
     *   Red component.
     *
     * @param g
     *   Green component.
     *
     * @param b
     *   Blue component.
     */
    Colour(std::uint8_t r, std::uint8_t g, std::uint8_t b);

    /**
     * Construct a new colour from a hex colour number. The expected format of the number is 0xRRGGBB.
     *
     * @param colour
     *   Colour value (0xRRGGBB).
     */
    Colour(std::uint32_t colour);

    /**
     * Stream operator.
     *
     * @param os
     *   Stream to write to.
     *
     * @param c
     *   Colour to write to stream.
     *
     * @returns
     *   Reference to supplied stream.
     */
    friend std::ostream &operator<<(std::ostream &os, const Colour &c);

    /** Red component. */
    std::uint8_t r;

    /** Green component. */
    std::uint8_t g;

    /** Blue component. */
    std::uint8_t b;
};

/**
 * Check if two colours are equal.
 *
 * @param c1
 *   First colour to check.
 *
 * @param c2
 *   Second colour to check.
 *
 * @returns
 *   True if both colours are equal, otherwise false.
 */
bool operator==(const Colour &c1, const Colour &c2);

/**
 * Check if two colours are not equal.
 *
 * @param c1
 *   First colour to check.
 *
 * @param c2
 *   Second colour to check.
 *
 * @returns
 *   True if both colours are not equal, otherwise false.
 */
bool operator!=(const Colour &c1, const Colour &c2);

// see docs on class definition
std::ostream &operator<<(std::ostream &os, const Colour &c);

}
