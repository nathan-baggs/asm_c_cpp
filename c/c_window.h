////////////////////////////////////////////////////////////////////////////////
//         Distributed under the Boost Software License, Version 1.0.         //
//            (See accompanying file LICENSE or copy at                       //
//                 https://www.boost.org/LICENSE_1_0.txt)                     //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <stdint.h>

#include "c_key_event.h"
#include "c_rectangle.h"
#include "c_result.h"

/**
 * Window is responsible fro creating and destroying a platform window as well as rendering to it and getting event.
 */

/**
 * Handle to internal window data.
 */
typedef struct C_Window C_Window;

/**
 * Create a new platform window.
 *
 * Note this library only supports creating one window, any calls to this function after the first successful call are
 * undefined.
 *
 * @param window
 *   Out paramater for created window object.
 *
 * @returns
 *   C_SUCCESS on success
 *   Another Result type on error
 */
C_Result c_window_create(C_Window **window);

/**
 * Destroy a window.
 *
 * @param window
 *   The window to destroy.
 */
void c_window_destroy(C_Window *window);

/**
 * Get an event if one is available.
 *
 * @param window
 *   Window to get events for.
 *
 * @param event
 *   Out paramater to write event data to.
 *
 * @returns
 *   C_SUCCESS on success
 *   C_NO_MORE_EVENTS if there was no event to get
 *   Another Result type on error
 */
C_Result c_window_get_event(const C_Window *window, C_KeyEvent *event);

/**
 * Perform an pre-render tasks.
 *
 * @param window
 *   Window to render to.
 *
 * @returns
 *   C_SUCCESS on success
 *   Another Result type on error
 */
C_Result c_window_pre_render(const C_Window *window);

/**
 * Perform an post-render tasks.
 *
 * @param window
 *   Window to render to.
 */
void c_window_post_render(const C_Window *window);

/**
 * Draw a rectangle to the screen.
 *
 * This *must* be called after c_window_pre_render and before c_window_post_render for any given frame.
 *
 * @param window
 *   The window to render to.
 *
 * @param rectangle
 *   The rectangle to draw.
 *
 * @param r
 *   The red component of the rectangle colour.
 *
 * @param g
 *   The green component of the rectangle colour.
 *
 * @param b
 *   The blue component of the rectangle colour.
 *
 * @returns
 *   C_SUCCESS on success
 *   Another Result type on error
 */
C_Result c_window_draw_rectangle(const C_Window *window, const C_Rectangle *rectangle, uint8_t r, uint8_t g, uint8_t b);
