////////////////////////////////////////////////////////////////////////////////
//         Distributed under the Boost Software License, Version 1.0.         //
//            (See accompanying file LICENSE or copy at                       //
//                 https://www.boost.org/LICENSE_1_0.txt)                     //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "c_vector2.h"

/**
 * A Rectangle represents an axis aligned box defined by the coordinates of its upper left corner as well as width and
 * height.
 */

/**
 * Struct for rectangle data. Deliberately public.
 */
typedef struct C_Rectangle
{
    C_Vector2 position;
    float width;
    float height;
} C_Rectangle;

/**
 * Create a new Rectangle.
 *
 * @param position
 *   Position of rectangle (upper left corner).
 *
 * @param width
 *   Width of rectangle.
 *
 * @param height
 *   Height of rectangle.
 *
 * @returns
 *   Rectangle constructed with supplied values.
 */
C_Rectangle c_rectangle_create(const C_Vector2 *position, float width, float height);

/**
 * Create a new Rectangle.
 *
 * @param x
 *   X coordinate of position (upper left corner).
 *
 * @param y
 *   Y coordinate of position (upper left corner).
 *
 * @param width
 *   Width of rectangle.
 *
 * @param height
 *   Height of rectangle.
 *
 * @returns
 *   Rectangle constructed with supplied values.
 */
C_Rectangle c_rectangle_create_xy(float x, float y, float width, float height);

/**
 * Translate the position of a rectangle.
 *
 * @param rectangle
 *   Rectangle to translate.
 *
 * @param translation
 *   Translation amount.
 */
void c_rectangle_translate(C_Rectangle *rectangle, const C_Vector2 *translation);

/**
 * Translate the position of a rectangle.
 *
 * @param rectangle
 *   Rectangle to translate.
 *
 * @param x
 *   Amount to move along x axis.
 *
 * @param y
 *   Amount to move along y axis.
 */
void c_rectangle_translate_xy(C_Rectangle *rectangle, float x, float y);

/**
 * Set the position of a rectangle.
 *
 * @param rectangle
 *   Rectangle to set position of.
 *
 * @param position
 *   New rectangle position.
 */
void c_rectangle_set_position(C_Rectangle *rectangle, const C_Vector2 *position);

/**
 * Set the position of a rectangle.
 *
 * @param rectangle
 *   Rectangle to set position of.
 *
 * @param x
 *   X coordinate of new position.
 *
 * @param y
 *   Y coordinate of new position.
 */
void c_rectangle_set_position_xy(C_Rectangle *rectangle, float x, float y);

/**
 * Print rectangle to stdout.
 *
 * @param rectangle
 *   Rectangle to print.
 */
void c_rectangle_print(const C_Rectangle *rectangle);
