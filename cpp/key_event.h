////////////////////////////////////////////////////////////////////////////////
//         Distributed under the Boost Software License, Version 1.0.         //
//            (See accompanying file LICENSE or copy at                       //
//                 https://www.boost.org/LICENSE_1_0.txt)                     //
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace cpp
{

/**
 * Enumeration of possible key states.
 */
enum class KeyState
{
    DOWN,
    UP
};

/**
 * Enumeration of possible input keys (maybe incomplete).
 */
enum class Key
{
    ESCAPE,
    LEFT,
    RIGHT,
};

/**
 * Struct encapsulating the data for a key press event.
 */
struct KeyEvent
{
    KeyState key_state;
    Key key;
};

}
