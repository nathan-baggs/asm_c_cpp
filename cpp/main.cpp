////////////////////////////////////////////////////////////////////////////////
//         Distributed under the Boost Software License, Version 1.0.         //
//            (See accompanying file LICENSE or copy at                       //
//                 https://www.boost.org/LICENSE_1_0.txt)                     //
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "key_event.h"
#include "window.h"

int main()
{
    std::cout << "hello world\n";

    cpp::Window window{};
    auto running = true;

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
            }
            else
            {
                break;
            }
        }
    }

    return 0;
}
