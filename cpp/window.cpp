////////////////////////////////////////////////////////////////////////////////
//         Distributed under the Boost Software License, Version 1.0.         //
//            (See accompanying file LICENSE or copy at                       //
//                 https://www.boost.org/LICENSE_1_0.txt)                     //
////////////////////////////////////////////////////////////////////////////////

#include "window.h"

#include <memory>
#include <optional>
#include <stdexcept>

#include "key_event.h"

#include "SDL.h"

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
        ::SDL_CreateWindow("c_game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN));
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

}
