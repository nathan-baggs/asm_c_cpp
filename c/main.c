////////////////////////////////////////////////////////////////////////////////
//         Distributed under the Boost Software License, Version 1.0.         //
//            (See accompanying file LICENSE or copy at                       //
//                 https://www.boost.org/LICENSE_1_0.txt)                     //
////////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "c_key_event.h"
#include "c_list.h"
#include "c_rectangle.h"
#include "c_window.h"

int main()
{
    printf("hello world\n");

    // create window
    C_Window *window;
    if (c_window_create(&window) != C_SUCCESS)
    {
        printf("failed to create window\n");
        exit(1);
    }

    C_KeyEvent event;
    bool running = true;

    while (running)
    {
        // process all events
        for (;;)
        {
            C_Result event_result = c_window_get_event(window, &event);
            if (event_result == C_SUCCESS)
            {
                if ((event.key_state == C_KEYSTATE_DOWN) && (event.key == C_KEY_Q))
                {
                    running = false;
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

        // render our scene

        if (c_window_pre_render(window) != C_SUCCESS)
        {
            printf("pre render failed\n");
            exit(1);
        }

        C_Rectangle paddle = c_rectangle_create_xy(300.0f, 780.0f, 300.0f, 20.0f);
        if (c_window_draw_rectangle(window, &paddle, 0xff, 0xff, 0xff) != C_SUCCESS)
        {
            printf("failed to render\n");
            exit(1);
        }

        c_window_post_render(window);
    }

    c_window_destroy(window);

    printf("goodbye\n");

    return 0;
}
