////////////////////////////////////////////////////////////////////////////////
//         Distributed under the Boost Software License, Version 1.0.         //
//            (See accompanying file LICENSE or copy at                       //
//                 https://www.boost.org/LICENSE_1_0.txt)                     //
////////////////////////////////////////////////////////////////////////////////

#include "c_vector2.h"

#include <assert.h>
#include <stdio.h>

C_Vector2 c_vector2_create()
{
    return c_vector2_create_xy(0.0f, 0.0f);
}

C_Vector2 c_vector2_create_xy(float x, float y)
{
    C_Vector2 vec = {.x = x, .y = y};
    return vec;
}

void c_vector2_add(C_Vector2 *vec1, const C_Vector2 *vec2)
{
    assert(vec1 != NULL);
    assert(vec2 != NULL);

    vec1->x += vec2->x;
    vec1->y += vec2->y;
}

void c_vector2_add_xy(C_Vector2 *vec, float x, float y)
{
    assert(vec != NULL);

    vec->x += x;
    vec->y += y;
}

void c_vector2_print(const C_Vector2 *vec)
{
    assert(vec != NULL);

    printf("{ x: %f, y: %f }\n", vec->x, vec->y);
}
