
#pragma once

// Note: Only RPI defines TARGET_GLFW_WINDOW, it's default on desktop platforms
#if defined( TARGET_RASPBERRY_PI ) && defined( TARGET_GLFW_WINDOW )

#ifndef GLFW_GAMEPAD_BUTTON_A
#pragma message "Warning, you are using an older GLFW version. Trying to load some fallbacks."
#endif

// Dirty: Add support for older GLFW releases (needed for RPi Stretch at least, Glfw 3.2)
// These defines were hardcoded in older releases, but they're easier to read with these nice macros
#ifndef GLFW_GAMEPAD_BUTTON_A
#define GLFW_GAMEPAD_BUTTON_A               0
#endif
#ifndef GLFW_GAMEPAD_BUTTON_B
#define GLFW_GAMEPAD_BUTTON_B               1
#endif
#ifndef GLFW_GAMEPAD_BUTTON_X
#define GLFW_GAMEPAD_BUTTON_X               2
#endif
#ifndef GLFW_GAMEPAD_BUTTON_Y
#define GLFW_GAMEPAD_BUTTON_Y               3
#endif
#ifndef GLFW_GAMEPAD_BUTTON_LEFT_BUMPER
#define GLFW_GAMEPAD_BUTTON_LEFT_BUMPER     4
#endif
#ifndef GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER
#define GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER    5
#endif
#ifndef GLFW_GAMEPAD_BUTTON_BACK
#define GLFW_GAMEPAD_BUTTON_BACK            6
#endif
#ifndef GLFW_GAMEPAD_BUTTON_START
#define GLFW_GAMEPAD_BUTTON_START           7
#endif
#ifndef GLFW_GAMEPAD_BUTTON_GUIDE
#define GLFW_GAMEPAD_BUTTON_GUIDE           8
#endif
#ifndef GLFW_GAMEPAD_BUTTON_LEFT_THUMB
#define GLFW_GAMEPAD_BUTTON_LEFT_THUMB      9
#endif
#ifndef GLFW_GAMEPAD_BUTTON_RIGHT_THUMB
#define GLFW_GAMEPAD_BUTTON_RIGHT_THUMB     10
#endif
#ifndef GLFW_GAMEPAD_BUTTON_DPAD_UP
#define GLFW_GAMEPAD_BUTTON_DPAD_UP         11
#endif
#ifndef GLFW_GAMEPAD_BUTTON_DPAD_RIGHT
#define GLFW_GAMEPAD_BUTTON_DPAD_RIGHT      12
#endif
#ifndef GLFW_GAMEPAD_BUTTON_DPAD_DOWN
#define GLFW_GAMEPAD_BUTTON_DPAD_DOWN       13
#endif
#ifndef GLFW_GAMEPAD_BUTTON_DPAD_LEFT
#define GLFW_GAMEPAD_BUTTON_DPAD_LEFT       14
#endif
#ifndef GLFW_JOYSTICK_1
#define GLFW_JOYSTICK_1                     0
#endif
#ifndef GLFW_GAMEPAD_AXIS_LEFT_X
#define GLFW_GAMEPAD_AXIS_LEFT_X            0
#endif
#ifndef GLFW_GAMEPAD_AXIS_LEFT_Y
#define GLFW_GAMEPAD_AXIS_LEFT_Y            1
#endif
#ifndef GLFW_GAMEPAD_AXIS_RIGHT_X
#define GLFW_GAMEPAD_AXIS_RIGHT_X           2
#endif
#ifndef GLFW_GAMEPAD_AXIS_RIGHT_Y
#define GLFW_GAMEPAD_AXIS_RIGHT_Y           3
#endif
#ifndef GLFW_GAMEPAD_AXIS_LEFT_TRIGGER
#define GLFW_GAMEPAD_AXIS_LEFT_TRIGGER      4
#endif
#ifndef GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER
#define GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER     5
#endif
#ifndef GLFW_GAMEPAD_AXIS_LAST
#define GLFW_GAMEPAD_AXIS_LAST              GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER
#endif

#endif
