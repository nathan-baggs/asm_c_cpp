////////////////////////////////////////////////////////////////////////////////
//         Distributed under the Boost Software License, Version 1.0.         //
//            (See accompanying file LICENSE or copy at                       //
//                 https://www.boost.org/LICENSE_1_0.txt)                     //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <iosfwd>

namespace cpp
{

/**
 * A Vector2 represents a two component (x and y) vector.
 */
class Vector2
{
  public:
    /**
     * Construct a new Vector2, with both components 0.0.
     */
    Vector2();

    /**
     * Construct a new Vector2, with supplied component values.
     *
     * @param x
     *   X component.
     *
     * @param y
     *   Y component.
     */
    Vector2(float x, float y);

    /**
     * Stream operator.
     *
     * @param os
     *   Stream to write to.
     *
     * @param v
     *   Vector to write to stream.
     *
     * @returns
     *   Reference to supplied stream.
     */
    friend std::ostream &operator<<(std::ostream &os, const Vector2 &v);

    /** X component. */
    float x;

    /** Y component. */
    float y;
};

/**
 * Check if two vectors are equal.
 *
 * @param v1
 *   First vector to check.
 *
 * @param v2
 *   Second vector to check.
 *
 * @returns
 *   True if both vectors are equal, otherwise false.
 */
bool operator==(const Vector2 &v1, const Vector2 &v2);

/**
 * Check if two vectors are not equal.
 *
 * @param v1
 *   First vector to check.
 *
 * @param v2
 *   Second vector to check.
 *
 * @returns
 *   True if both vectors are not equal, otherwise false.
 */
bool operator!=(const Vector2 &v1, const Vector2 &v2);

/**
 * Construct a new vector by adding two supplied vectors.
 *
 * @param v1
 *   First vector to add.
 *
 * @param v2
 *   Second vector to add.
 *
 * @returns
 *   v1 + v2.
 */
Vector2 operator+(const Vector2 &v1, const Vector2 &v2);

/**
 * Add assign one vector to another.
 *
 * @param v1
 *   Vector to add to, will be mutated.
 *
 * @param v2
 *   Second vector to add.
 *
 * @returns
 *   v1 += v2.
 */
Vector2 &operator+=(Vector2 &v1, const Vector2 &v2);

// see docs on class definition
std::ostream &operator<<(std::ostream &os, const Vector2 &v);

}
