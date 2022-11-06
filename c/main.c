////////////////////////////////////////////////////////////////////////////////
//         Distributed under the Boost Software License, Version 1.0.         //
//            (See accompanying file LICENSE or copy at                       //
//                 https://www.boost.org/LICENSE_1_0.txt)                     //
////////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "c_key_event.h"
#include "c_list.h"
#include "c_rectangle.h"
#include "c_window.h"

/**
 * Struct encapsulating the data for a renderable entity.
 */
typedef struct Entity
{
    C_Rectangle rectangle;
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Entity;

/**
 * Helper macro for checking if a value is C_SUCCESS. If not it prints a supplied messaged and exits.
 */
#define CHECK_SUCCESS(X, MSG)                                                                                          \
    do                                                                                                                 \
    {                                                                                                                  \
        C_Result r = X;                                                                                                \
        if (r != C_SUCCESS)                                                                                            \
        {                                                                                                              \
            printf("%s [error: %i]\n", MSG, r);                                                                        \
            exit(1);                                                                                                   \
        }                                                                                                              \
    } while (false)

/**
 * Helper function to create a row of ten bricks.
 *
 * @param entities
 *   List to store entities in.
 *
 * @param y
 *   Y coordinate of row.
 *
 * @param r
 *   Red component of brick colour.
 *
 * @param g
 *   Green component of brick colour.
 *
 * @param b
 *  Blue component of brick colour.
 */
static void create_brick_row(C_List *entities, float y, uint8_t r, uint8_t g, uint8_t b)
{
    float x = 20.0f;

    for (int i = 0; i < 10; ++i)
    {
        Entity *e = (Entity *)calloc(sizeof(Entity), 1u);
        e->rectangle.position.x = x;
        e->rectangle.position.y = y;
        e->rectangle.width = 58.0f;
        e->rectangle.height = 20.0f;
        e->r = r;
        e->g = g;
        e->b = b;

        CHECK_SUCCESS(c_list_push_back_dtor(entities, e, &free), "failed to add brick");

        x += 78.0f;
    }
}

/**
 * Helper function to update the ball.
 *
 * @param ball
 *   Entity for ball.
 *
 * @param ball_velocity
 *   Velocity of ball.
 */
static void update_ball(Entity *ball, C_Vector2 *ball_velocity)
{
    c_vector2_add(&ball->rectangle.position, ball_velocity);

    // if ball does out of the screen then invert the y velocity
    if ((ball->rectangle.position.y < 0.0f) || (ball->rectangle.position.y > 800.0f))
    {
        ball_velocity->y *= -1.0f;
    }

    if ((ball->rectangle.position.x < 0.0f) || (ball->rectangle.position.x > 800.0f))
    {
        ball_velocity->x *= -1.0f;
    }
}

/**
 * Helper function to check if two entities are colliding.
 *
 * @param entity1
 *   First entity to check.
 *
 * @param entity2
 *   Second entity to check.
 *
 * @returns
 *   True if entities collide, false otherwise.
 */
bool static check_collision(const Entity *entity1, const Entity *entity2)
{
    return (
        (entity1->rectangle.position.x < entity2->rectangle.position.x + entity2->rectangle.width) &&
        (entity1->rectangle.position.x + entity1->rectangle.width > entity2->rectangle.position.x) &&
        (entity1->rectangle.position.y < entity2->rectangle.position.y + entity2->rectangle.height) &&
        (entity1->rectangle.height + entity1->rectangle.position.y > entity2->rectangle.position.y));
}

/**
 * Helper function to handle collisions between the ball and other entities.
 *
 * @param entities
 *   List of all entities.
 *
 * @param ball
 *   Ball entity.
 *
 * @param ball_velocity
 *   The velocity of the ball.
 *
 * @param paddle
 *   Paddle entity.
 */
static void handle_collisions(C_List *entities, Entity *ball, C_Vector2 *ball_velocity, const Entity *paddle)
{
    // keep iterator scoped so we can't use it after it's been destroyed
    {
        C_ListIter *iter;
        CHECK_SUCCESS(c_list_iterator_create(entities, &iter), "could not create iterator");

        // move past ball and paddle
        c_list_iterator_advance(&iter);
        c_list_iterator_advance(&iter);

        // see if the ball intersects with any bricks
        while (!c_list_iterator_at_end(iter))
        {
            Entity *block = (Entity *)c_list_iterator_value(iter);

            if (check_collision(ball, block))
            {
                c_list_remove(entities, iter);
                ball_velocity->y *= -1.0f;

                // if we modify the list this will invalidate the iterator, so stop
                break;
            }

            c_list_iterator_advance(&iter);
        }

        c_list_iterator_destroy(iter);
    }

    // handle ball - paddle collision
    if (check_collision(ball, paddle))
    {
        if (ball->rectangle.position.x < paddle->rectangle.position.x + 6.0f)
        {
            ball_velocity->x = -0.7f;
            ball_velocity->y = -0.7f;
        }
        else if (ball->rectangle.position.x < paddle->rectangle.position.x + 14.0f)
        {
            ball_velocity->x = 0.0f;
            ball_velocity->y = -1.0f;
        }
        else
        {
            ball_velocity->x = 0.7f;
            ball_velocity->y = -0.7f;
        }
    }
}

int main()
{
    printf("hello world\n");

    Entity paddle = {
        .rectangle = c_rectangle_create_xy(300.0f, 780.0f, 300.0f, 20.0f), .r = 0xff, .g = 0xff, .b = 0xff};
    Entity ball = {.rectangle = c_rectangle_create_xy(420.0f, 400.0f, 10.0f, 10.0f), .r = 0xff, .g = 0xff, .b = 0xff};

    C_Vector2 paddle_velocity = c_vector2_create();
    C_Vector2 ball_velocity = c_vector2_create_xy(0.0f, 0.5f);

    C_List *entities = NULL;
    CHECK_SUCCESS(c_list_create(&entities), "failed to create entity list");

    CHECK_SUCCESS(c_list_push_back(entities, &paddle), "failed to add paddle to list");
    CHECK_SUCCESS(c_list_push_back(entities, &ball), "failed to add ball to list");

    create_brick_row(entities, 50.0f, 0xff, 0x00, 0x00);
    create_brick_row(entities, 80.0f, 0xff, 0x00, 0x00);
    create_brick_row(entities, 110.0f, 0xff, 0xa5, 0x00);
    create_brick_row(entities, 140.0f, 0xff, 0xa5, 0x00);
    create_brick_row(entities, 170.0f, 0x00, 0xff, 0x00);
    create_brick_row(entities, 200.0f, 0x00, 0xff, 0x00);

    C_ListIter *iter = NULL;
    CHECK_SUCCESS(c_list_iterator_create(entities, &iter), "failed to get entity iterator");

    // create window
    C_Window *window;
    CHECK_SUCCESS(c_window_create(&window), "failed to create window");

    C_KeyEvent event;
    bool running = true;

    const float paddle_speed = 1.0f;
    bool left_press = false;
    bool right_press = false;

    while (running)
    {
        // process all events
        for (;;)
        {
            C_Result event_result = c_window_get_event(window, &event);
            if (event_result == C_SUCCESS)
            {
                if ((event.key_state == C_KEYSTATE_DOWN) && (event.key == C_KEY_ESCAPE))
                {
                    running = false;
                }
                else if (event.key == C_KEY_LEFT)
                {
                    left_press = (event.key_state == C_KEYSTATE_DOWN) ? true : false;
                }
                else if (event.key == C_KEY_RIGHT)
                {
                    right_press = (event.key_state == C_KEYSTATE_DOWN) ? true : false;
                }
            }
            else if (event_result == C_NO_MORE_EVENTS)
            {
                break;
            }
            else
            {
                printf("error getting event\n");
                exit(1);
            }
        }

        if ((left_press && right_press) || (!left_press && !right_press))
        {
            paddle_velocity.x = 0.0f;
        }
        else if (left_press)
        {
            paddle_velocity.x = -paddle_speed;
        }
        else if (right_press)
        {
            paddle_velocity.x = paddle_speed;
        }

        c_vector2_add(&paddle.rectangle.position, &paddle_velocity);
        update_ball(&ball, &ball_velocity);
        handle_collisions(entities, &ball, &ball_velocity, &paddle);

        // reset iterator as we may have modified the list and we will want to start from the beginning anyway
        c_list_iterator_reset(entities, &iter);

        // render our scene

        CHECK_SUCCESS(c_window_pre_render(window), "pre render failed");

        while (!c_list_iterator_at_end(iter))
        {
            Entity *entity = (Entity *)c_list_iterator_value(iter);
            CHECK_SUCCESS(
                c_window_draw_rectangle(window, &entity->rectangle, entity->r, entity->g, entity->b),
                "failed to render entity");

            c_list_iterator_advance(&iter);
        }

        c_window_post_render(window);
    }

    c_list_iterator_destroy(iter);
    c_window_destroy(window);

    printf("goodbye\n");

    return 0;
}
