////////////////////////////////////////////////////////////////////////////////
//         Distributed under the Boost Software License, Version 1.0.         //
//            (See accompanying file LICENSE or copy at                       //
//                 https://www.boost.org/LICENSE_1_0.txt)                     //
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <ranges>

#include "colour.h"
#include "entity.h"
#include "key_event.h"
#include "rectangle.h"
#include "vector2.h"
#include "window.h"

namespace
{

/**
 * Helper function to create a row of 10 bricks.
 *
 * @param entities
 *   Collection to add new entities to.
 *
 * @param y
 *   Y coordinate of row.
 *
 * @param colour
 *   Colour of bricks.
 */
void create_brick_row(std::vector<cpp::Entity> &entities, float y, const cpp::Colour &colour)
{
    auto x = 20.0f;

    for (auto i = 0u; i < 10u; ++i)
    {
        entities.push_back({{{x, y}, 58.0f, 20.0f}, colour});
        x += 78.0f;
    }
}

/**
 * Helper function to check for and resolve collisions between the ball and other entities.
 *
 * @param ball
 *   Ball to check for collisions.
 *
 * @param ball_velocity
 *   Velocity of ball, might get mutated during collision response.
 *
 * @param paddle
 *   Paddle to check for collisions with.
 *
 * @param entities
 *   Collection of all entities, brick entities will be removed if a collision is detected.
 */
void check_collisions(
    const cpp::Entity &ball,
    cpp::Vector2 &ball_velocity,
    const cpp::Entity &paddle,
    std::vector<cpp::Entity> &entities)
{
    // check and handle ball and paddle collision
    if (paddle.intersects(ball))
    {
        const auto ball_pos = ball.rectangle().position;
        const auto paddle_pos = paddle.rectangle().position;

        if (ball_pos.x < paddle_pos.x + 100.0f)
        {
            ball_velocity.x = -0.7f;
            ball_velocity.y = -0.7f;
        }
        else if (ball_pos.x < paddle_pos.x + 200.0f)
        {
            ball_velocity.x = 0.0f;
            ball_velocity.y = -1.0f;
        }
        else
        {
            ball_velocity.x = 0.7f;
            ball_velocity.y = -0.7f;
        }
    }
    else
    {
        // only check brick intersections if we didn't intersect the paddle, unlikely these will both happen in the same
        // frame due to the layout of the game

        // iterate over all entities, skipping the first two as these are the paddle and ball
        auto bricks_view = entities | std::views::drop(2u);
        auto hit_brick =
            std::ranges::find_if(bricks_view, [&ball](const auto &brick) { return ball.intersects(brick); });

        if (hit_brick != std::ranges::end(bricks_view))
        {
            // we hit a brick so update ball velocity and remove brick entity
            ball_velocity.y *= -1.0f;
            entities.erase(hit_brick);
        }
    }
}

/**
 * Helper function to update the ball. Will check if the ball leaves the window and adjust the velocity so it "bounces"
 * off walls.
 *
 * @param ball
 *   Ball entity to update.
 *
 * @param velocity
 *   Ball velocity, will be mutated if ball goes off screen.
 */
void update_ball(cpp::Entity &ball, cpp::Vector2 &velocity)
{
    ball.translate(velocity);

    const auto ball_pos = ball.rectangle().position;

    if ((ball_pos.y > 800.0f) || (ball_pos.y < 0.0f))
    {
        velocity.y *= -1.0f;
    }

    if ((ball_pos.x > 800.0f) || (ball_pos.x < 0.0f))
    {
        velocity.x *= -1.0f;
    }
}

/**
 * Helper function to update the paddle.
 *
 * @param paddle
 *   Paddle entity to update.
 *
 * @param velocity
 *   Paddle velocity.
 */
void update_paddle(cpp::Entity &paddle, const cpp::Vector2 &velocity)
{
    paddle.translate(velocity);
}

}

int main()
{
    std::cout << "hello world\n";

    const cpp::Window window{};
    auto running = true;

    std::vector<cpp::Entity> entities{
        {{{300.0f, 780.0f}, 300.0f, 20.0f}, 0xFFFFFF}, {{{420.0f, 400.0f}, 10.0f, 10.0f}, 0xFFFFFF}};

    create_brick_row(entities, 50.0f, 0xff0000);
    create_brick_row(entities, 80.0f, 0xff0000);
    create_brick_row(entities, 110.0f, 0xffa500);
    create_brick_row(entities, 140.0f, 0xffa500);
    create_brick_row(entities, 170.0f, 0x00ff00);
    create_brick_row(entities, 200.0f, 0x00ff00);

    cpp::Vector2 ball_velocity{0.0f, 1.0f};
    cpp::Vector2 paddle_velocity{0.0f, 0.0f};
    const float paddle_speed = 1.0f;
    auto left_press = false;
    auto right_press = false;

    while (running)
    {
        for (;;)
        {
            if (const auto event = window.get_event(); event)
            {
                using enum cpp::Key;
                using enum cpp::KeyState;

                if ((event->key_state == DOWN) && (event->key == ESCAPE))
                {
                    running = false;
                }
                else if (event->key == LEFT)
                {
                    left_press = (event->key_state == DOWN) ? true : false;
                }
                else if (event->key == RIGHT)
                {
                    right_press = (event->key_state == DOWN) ? true : false;
                }
            }
            else
            {
                break;
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

        // scope the references to the paddle and ball, if check_collisions results in an entity being removed then
        // that will invalidate our references, so prevent them from being accidentally used later
        {
            auto &paddle = entities[0];
            auto &ball = entities[1];

            update_paddle(paddle, paddle_velocity);
            update_ball(ball, ball_velocity);
            check_collisions(ball, ball_velocity, paddle, entities);
        }

        window.render(entities);
    }

    std::cout << "goodbye\n";

    return 0;
}
