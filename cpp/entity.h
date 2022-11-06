////////////////////////////////////////////////////////////////////////////////
//         Distributed under the Boost Software License, Version 1.0.         //
//            (See accompanying file LICENSE or copy at                       //
//                 https://www.boost.org/LICENSE_1_0.txt)                     //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "colour.h"
#include "rectangle.h"
#include "vector2.h"

namespace cpp
{

/**
 * An Entity represents a renderable rectangle.
 */
class Entity
{
  public:
    /**
     * Construct a new entity.
     *
     * @param rectangle
     *   Rectangle representing the area to draw.
     *
     * @param colour
     *   The colour to render the entity.
     */
    Entity(const Rectangle &rectangle, const Colour &colour);

    /**
     * Get the entities rectangle.
     *
     * @returns
     *   Entity rectangle.
     */
    Rectangle rectangle() const;

    /**
     * Get the entities colour.
     *
     * @returns
     *   Entity colour.
     */
    Colour colour() const;

    /**
     * Translate the entity.
     *
     * @param translation
     *   Translation amount.
     */
    void translate(const Vector2 &translation);

    /**
     * Check if another entity intersects this.
     *
     * @param e
     *   Entity to check for intersection.
     *
     * @returns
     *   True if supplied entity intersects this, otherwise false.
     */
    bool intersects(const Entity &e) const;

    /**
     * Stream operator.
     *
     * @param os
     *   Stream to write to.
     *
     * @param e
     *   Entity to write to stream.
     *
     * @returns
     *   Reference to supplied stream.
     */
    friend std::ostream &operator<<(std::ostream &os, const Entity &e);

  private:
    /** Drawable area. */
    Rectangle rectangle_;

    /** Draw colour. */
    Colour colour_;
};

/**
 * Check if two entities are equal.
 *
 * @param e1
 *   First entity to check.
 *
 * @param e2
 *   Second entity to check.
 *
 * @returns
 *   True if both entities are equal, otherwise false.
 */
bool operator==(const Entity &e1, const Entity &e2);

/**
 * Check if two entities are equal.
 *
 * @param e1
 *   First entity to check.
 *
 * @param e2
 *   Second entity to check.
 *
 * @returns
 *   True if both entities are equal, otherwise false.
 */
bool operator!=(const Entity &e1, const Entity &e2);

// see docs on class definition
std::ostream &operator<<(std::ostream &os, const Entity &e);

}
