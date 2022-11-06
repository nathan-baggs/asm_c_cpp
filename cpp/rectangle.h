////////////////////////////////////////////////////////////////////////////////
//         Distributed under the Boost Software License, Version 1.0.         //
//            (See accompanying file LICENSE or copy at                       //
//                 https://www.boost.org/LICENSE_1_0.txt)                     //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <iosfwd>

#include "vector2.h"

namespace cpp
{

/**
 * A Rectangle represents an axis aligned box defined by the coordinates of its upper left corner as well as width and
 * height.
 */
class Rectangle
{
  public:
    /**
     * Construct a new Rectangle.
     *
     * @param position
     *   Position of rectangle (upper left corner).
     *
     * @param width
     *   Width of rectangle.
     *
     * @param height
     *   Height of rectangle.
     */
    Rectangle(const Vector2 &position, float width, float height);

    /**
     * Stream operator.
     *
     * @param os
     *   Stream to write to.
     *
     * @param rect
     *   Rectangle to write to stream.
     *
     * @returns
     *   Reference to supplied stream.
     */
    friend std::ostream &operator<<(std::ostream &os, const Rectangle &rect);

    /**
     * Translate the position of a rectangle.
     *
     * @param translation
     *   Translation amount.
     */
    void translate(const Vector2 &translation);

    /**
     * Check if another rectangle intersects this.
     *
     * @param rect
     *   Rectangle to check for intersection.
     *
     * @returns
     *   True if supplied rectangle intersects this, otherwise false.
     */
    bool intersects(const Rectangle &rect) const;

    /** Position of upper left corner of rectangle. */
    Vector2 position;

    /** Rectangle width. */
    float width;

    /** Rectangle height. */
    float height;
};

/**
 * Check if two rectangles are equal.
 *
 * @param r1
 *   First rectangle to check.
 *
 * @param r2
 *   Second rectangle to check.
 *
 * @returns
 *   True if both rectangles are equal, otherwise false.
 */
bool operator==(const Rectangle &r1, const Rectangle &r2);

/**
 * Check if two rectangles are not equal.
 *
 * @param r1
 *   First rectangle to check.
 *
 * @param r2
 *   Second rectangle to check.
 *
 * @returns
 *   True if both rectangles are not equal, otherwise false.
 */
bool operator!=(const Rectangle &r1, const Rectangle &r2);

// see docs on class definition
std::ostream &operator<<(std::ostream &os, const Rectangle &rect);

}
