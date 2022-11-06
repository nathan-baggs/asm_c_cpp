////////////////////////////////////////////////////////////////////////////////
//         Distributed under the Boost Software License, Version 1.0.         //
//            (See accompanying file LICENSE or copy at                       //
//                 https://www.boost.org/LICENSE_1_0.txt)                     //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "entity.h"
#include "key_event.h"

struct SDL_Window;
struct SDL_Renderer;

using SDLWindowDelete = void (*)(SDL_Window *);
using SDLRendererDelete = void (*)(SDL_Renderer *);

namespace cpp
{

/**
 * Window is responsible for creating and destroying a platform window as well as rendering to it and getting events.
 */
class Window
{
  public:
    /**
     * Construct a new Window.
     */
    Window();

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    Window(Window &&) = default;
    Window &operator=(Window &&) = default;

    /**
     * Get an event if one is available.
     *
     * @returns
     *   A KeyEvent if an event was available, otherwise an empty optional.
     */
    std::optional<KeyEvent> get_event() const;

    /**
     * Render a collection of entities.
     *
     * @param entities
     *   Entities to render.
     */
    void render(const std::vector<Entity> &entities) const;

  private:
    /** SDL window object. */
    std::unique_ptr<SDL_Window, SDLWindowDelete> window_;

    /** SDL renderer object. */
    std::unique_ptr<SDL_Renderer, SDLRendererDelete> renderer_;
};

}
