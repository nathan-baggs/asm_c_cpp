////////////////////////////////////////////////////////////////////////////////
//         Distributed under the Boost Software License, Version 1.0.         //
//            (See accompanying file LICENSE or copy at                       //
//                 https://www.boost.org/LICENSE_1_0.txt)                     //
////////////////////////////////////////////////////////////////////////////////

#include "window.h"

#include <memory>
#include <optional>
#include <stdexcept>
#include <vector>

#include "SDL.h"

#include "entity.h"
#include "key_event.h"

namespace
{

/**
 * Helper function to map an SDL key code to an internal type.
 *
 * @param sql_code
 *   SDL key code.
 *
 * @returns
 *   Internal representation of the supplied key code, or empty optional if no mapping exists.
 */
std::optional<cpp::Key> map_sdl_key(SDL_Keycode sdl_code)
{
    switch (sdl_code)
    {
        using enum cpp::Key;

        case SDLK_ESCAPE: return ESCAPE;
        case SDLK_LEFT: return LEFT;
        case SDLK_RIGHT: return RIGHT;
        default: return std::nullopt;
    }
}

}

namespace cpp
{

Window::Window()
    : window_(nullptr, &SDL_DestroyWindow)
    , renderer_(nullptr, &SDL_DestroyRenderer)
{
    if (::SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        throw std::runtime_error("failed to init SDL");
    }

    window_.reset(
        ::SDL_CreateWindow("cpp_game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN));
    if (!window_)
    {
        throw std::runtime_error("failed to create window");
    }

    renderer_.reset(::SDL_CreateRenderer(window_.get(), -1, 0u));
    if (!renderer_)
    {
        throw std::runtime_error("failed to create renderer");
    }
}

std::optional<KeyEvent> Window::get_event() const
{
    std::optional<KeyEvent> event{};

    SDL_Event sdl_event = {0};
    if (::SDL_PollEvent(&sdl_event) != 0u)
    {
        std::optional<KeyState> key_state;

        if (sdl_event.type == SDL_KEYDOWN)
        {
            key_state = KeyState::DOWN;
        }
        else if (sdl_event.type == SDL_KEYUP)
        {
            key_state = KeyState::UP;
        }

        // if we got a key state then it's safe to inspect the key code
        if (key_state)
        {
            if (const auto key = map_sdl_key(sdl_event.key.keysym.sym); key)
            {
                // got state and key - so create the KeyEvent
                event = KeyEvent{.key_state = *key_state, .key = *key};
            }
        }
    }

    return event;
}

void Window::render(const std::vector<Entity> &entities) const
{
    if (::SDL_SetRenderDrawColor(renderer_.get(), 0x0, 0x0, 0x0, 0xff) != 0)
    {
        throw std::runtime_error("failed to set render draw colour");
    }

    if (::SDL_RenderClear(renderer_.get()) != 0)
    {
        throw std::runtime_error("failed to clear renderer");
    }

    for (const auto &entity : entities)
    {
        const auto entity_rect = entity.rectangle();
        const auto entity_colour = entity.colour();

        const SDL_Rect sdl_rect = {
            .x = static_cast<int>(entity_rect.position.x),
            .y = static_cast<int>(entity_rect.position.y),
            .w = static_cast<int>(entity_rect.width),
            .h = static_cast<int>(entity_rect.height),
        };

        if (::SDL_SetRenderDrawColor(renderer_.get(), entity_colour.r, entity_colour.g, entity_colour.b, 0xff) != 0)
        {
            throw std::runtime_error("failed to draw entity");
        }

        if (::SDL_RenderFillRect(renderer_.get(), &sdl_rect) != 0)
        {
            throw std::runtime_error("failed to draw filled rect");
        }
    }

    ::SDL_RenderPresent(renderer_.get());
}

}
