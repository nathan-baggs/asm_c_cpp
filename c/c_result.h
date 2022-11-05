////////////////////////////////////////////////////////////////////////////////
//         Distributed under the Boost Software License, Version 1.0.         //
//            (See accompanying file LICENSE or copy at                       //
//                 https://www.boost.org/LICENSE_1_0.txt)                     //
////////////////////////////////////////////////////////////////////////////////

#pragma once

/**
 * Enumeration of all possible return codes.
 */
typedef enum C_Result
{
    C_SUCCESS,

    C_NO_MORE_EVENTS,

    C_SDL_INIT_FAILED,

    C_FAILED_TO_ALLOCATE_WINDOW,
    C_FAILED_TO_CREATE_WINDOW,
    C_FAILED_TO_GET_SURFACE,
    C_FAILED_TO_FILL_RECT,
    C_FAILED_TO_UPDATE_SURFACE,
    C_FAILED_TO_CREATE_RENDERER,

    C_FAILED_TO_MAP_KEY,

    C_FAILED_TO_CLEAR_RENDERER,
    C_FAILED_TO_SET_RENDER_COLOUR,
    C_FAILED_TO_DRAW_FILLED_RECT,

    C_FAILED_TO_ALLOCATE_LIST,
    C_FAILED_TO_ALLOCATE_NODE,
    C_FAILED_TO_ALLOCATE_ITERATOR,
} C_Result;
