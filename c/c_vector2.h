////////////////////////////////////////////////////////////////////////////////
//         Distributed under the Boost Software License, Version 1.0.         //
//            (See accompanying file LICENSE or copy at                       //
//                 https://www.boost.org/LICENSE_1_0.txt)                     //
////////////////////////////////////////////////////////////////////////////////

#pragma once

/**
 * A Vector2 represents a two component (x and y) vector.
 */

/**
 * Struct for vector data. Deliberately public.
 */
typedef struct C_Vector2
{
    float x;
    float y;
} C_Vector2;

/**
 * Create a new Vector2 with both components 0.0.
 *
 * @returns
 *   Vector2 with x and y set to 0.0.
 */
C_Vector2 c_vector2_create();

/**
 * Create a new Vector2 with supplied component values.
 *
 * @param x
 *   X component.
 *
 * @param y
 *   Y component.
 *
 * @returns
 *   Vector2 with x and y set to supplied values.
 */
C_Vector2 c_vector2_create_xy(float x, float y);

/**
 * Add one vector to another.
 *
 * @param vec1
 *   The source vector, this will be modified.
 *
 * @param vec2
 *   The vector to add to vec1.
 */
void c_vector2_add(C_Vector2 *vec1, const C_Vector2 *vec2);

/**
 * Add values to a vector.
 *
 * @param vec
 *   The source vector, this will be modified.
 *
 * @param x
 *   Value to add to x component.
 *
 * @param y
 *   Value to add to y component.
 */
void c_vector2_add_xy(C_Vector2 *vec, float x, float y);

/**
 * Print vector to stdout.
 *
 * @param vec
 *   The vector to print.
 */
void c_vector2_print(const C_Vector2 *vec);
