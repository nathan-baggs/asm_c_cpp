////////////////////////////////////////////////////////////////////////////////
//         Distributed under the Boost Software License, Version 1.0.         //
//            (See accompanying file LICENSE or copy at                       //
//                 https://www.boost.org/LICENSE_1_0.txt)                     //
////////////////////////////////////////////////////////////////////////////////

#include "c_rectangle.h"

#include <assert.h>
#include <stdio.h>

#include "c_vector2.h"

C_Rectangle c_rectangle_create(const C_Vector2 *position, float width, float height)
{
    return c_rectangle_create_xy(position->x, position->y, width, height);
}

C_Rectangle c_rectangle_create_xy(float x, float y, float width, float height)
{
    C_Rectangle rect = {.position = {.x = x, .y = y}, .width = width, .height = height};
    return rect;
}

void c_rectangle_translate(C_Rectangle *rectangle, const C_Vector2 *translation)
{
    c_vector2_add(&rectangle->position, translation);
}

void c_rectangle_translate_xy(C_Rectangle *rectangle, float x, float y)
{
    c_vector2_add_xy(&rectangle->position, x, y);
}

void c_rectangle_set_position(C_Rectangle *rectangle, const C_Vector2 *position)
{
    rectangle->position = *position;
}

void c_rectangle_set_position_xy(C_Rectangle *rectangle, float x, float y)
{
    rectangle->position.x = x;
    rectangle->position.y = y;
}

void c_rectangle_print(const C_Rectangle *rectangle)
{
    printf(
        "{ x: %f, y: %f, w: %f, h: %f }\n",
        rectangle->position.x,
        rectangle->position.y,
        rectangle->width,
        rectangle->height);
}
