#ifndef Keyboard_H
#define Keyboard_H
#pragma once
#include <GLFW/glfw3.h>
#include <memory>

class Keyboard
{
private:
    static std::unique_ptr<Keyboard> keyboard;
    static bool KeyPressed[350];

public:
    static void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (action == GLFW_PRESS)
        {
            keyboard->KeyPressed[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keyboard->KeyPressed[key] = false;
        }
    }

    static bool IsKeyPressed(int key) { return KeyPressed[key] == GLFW_PRESS; }
};

#endif