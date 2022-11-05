////////////////////////////////////////////////////////////////////////////////
//         Distributed under the Boost Software License, Version 1.0.         //
//            (See accompanying file LICENSE or copy at                       //
//                 https://www.boost.org/LICENSE_1_0.txt)                     //
////////////////////////////////////////////////////////////////////////////////

#include "c_window.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "SDL.h"

#include "c_key_event.h"
#include "c_result.h"

typedef struct C_Window
{
    SDL_Window *window;
    SDL_Renderer *renderer;
} C_Window;

static C_Result map_sdl_key(C_Key *key, SDL_Keycode sdl_code)
{
    switch (sdl_code)
    {
        case SDLK_ESCAPE: *key = C_KEY_ESCAPE; return C_SUCCESS;
        case SDLK_LEFT: *key = C_KEY_LEFT; return C_SUCCESS;
        case SDLK_RIGHT: *key = C_KEY_RIGHT; return C_SUCCESS;
        default: return C_NO_MORE_EVENTS;
    }
}

C_Result c_window_create(C_Window **window)
{
    C_Result res = C_SUCCESS;

    // perform the initial SDL initialisation
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        res = C_SDL_INIT_FAILED;
        goto fail;
    }

    // allocate space for the window object
    C_Window *new_window = (C_Window *)calloc(1u, sizeof(C_Window));
    if (new_window == NULL)
    {
        res = C_FAILED_TO_ALLOCATE_WINDOW;
        goto fail;
    }

    // create an SDL window
    new_window->window =
        SDL_CreateWindow("c_game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN);
    if (new_window->window == NULL)
    {
        res = C_FAILED_TO_CREATE_WINDOW;
        goto fail;
    }

    // create an SDL renderer
    new_window->renderer = SDL_CreateRenderer(new_window->window, -1, 0);
    if (new_window->renderer == NULL)
    {
        res = C_FAILED_TO_CREATE_RENDERER;
        goto fail;
    }

    // assign the window to the user supplied pointer
    *window = new_window;
    return res;

fail:
    // clean up and return error code
    c_window_destroy(new_window);
    return res;
}

void c_window_destroy(C_Window *window)
{
    if (window == NULL)
    {
        return;
    }

    if (window->window != NULL)
    {
        SDL_DestroyWindow(window->window);
    }

    free(window);

    SDL_Quit();
}

C_Result c_window_get_event(const C_Window *window, C_KeyEvent *event)
{
    assert(window != NULL);
    assert(event != NULL);

    C_Result result = C_NO_MORE_EVENTS;

    // try and get an SDL event
    SDL_Event sdl_event;
    if (SDL_PollEvent(&sdl_event) != 0u)
    {
        bool try_map_key = false;

        if (sdl_event.type == SDL_KEYDOWN)
        {
            event->key_state = C_KEYSTATE_DOWN;
            try_map_key = true;
        }
        else if (sdl_event.type == SDL_KEYUP)
        {
            event->key_state = C_KEYSTATE_UP;
            try_map_key = true;
        }

        if (try_map_key)
        {
            // if we get here then we know we had a key event (either press or release), so convert the SDL key code
            // to our internal representation
            result = map_sdl_key(&event->key, sdl_event.key.keysym.sym);
        }
    }

    return result;
}

C_Result c_window_pre_render(const C_Window *window)
{
    C_Result result = C_SUCCESS;

    // clear the window to black

    if (SDL_SetRenderDrawColor(window->renderer, 0x0, 0x0, 0x0, 0x0) != 0)
    {
        result = C_FAILED_TO_SET_RENDER_COLOUR;
        goto end;
    }

    if (SDL_RenderClear(window->renderer) != 0)
    {
        result = C_FAILED_TO_CLEAR_RENDERER;
        goto end;
    }

end:
    return result;
}

void c_window_post_render(const C_Window *window)
{
    return SDL_RenderPresent(window->renderer);
}

C_Result c_window_draw_rectangle(const C_Window *window, const C_Rectangle *rectangle, uint8_t r, uint8_t g, uint8_t b)
{
    C_Result result = C_SUCCESS;

    // convert our internal rect to an SDL rect
    SDL_Rect sdl_rect = {
        .x = rectangle->position.x, .y = rectangle->position.y, .w = rectangle->width, .h = rectangle->height};

    // set the draw colour
    if (SDL_SetRenderDrawColor(window->renderer, r, g, b, 0xff) != 0)
    {
        result = C_FAILED_TO_SET_RENDER_COLOUR;
        goto end;
    }

    // draw!
    if (SDL_RenderFillRect(window->renderer, &sdl_rect) != 0)
    {
        result = C_FAILED_TO_DRAW_FILLED_RECT;
        goto end;
    }

end:
    return result;
}
