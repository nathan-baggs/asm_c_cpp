////////////////////////////////////////////////////////////////////////////////
//         Distributed under the Boost Software License, Version 1.0.         //
//            (See accompanying file LICENSE or copy at                       //
//                 https://www.boost.org/LICENSE_1_0.txt)                     //
////////////////////////////////////////////////////////////////////////////////

#pragma once

/**
 * Enumeration of possible key states.
 */
typedef enum C_KeyState
{
    C_KEYSTATE_DOWN,
    C_KEYSTATE_UP
} C_KeyState;

/**
 * Enumeration of possible input keys (maybe incomplete).
 */
typedef enum C_Key
{
    C_KEY_ESCAPE,
    C_KEY_LEFT,
    C_KEY_RIGHT,
} C_Key;

/**
 * Struct encapsulating the data for a key press event.
 */
typedef struct C_KeyEvent
{
    C_KeyState key_state;
    C_Key key;
} C_KeyEvent;
