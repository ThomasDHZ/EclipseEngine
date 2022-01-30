#include <iostream>
#include "VulkanRenderer.h"
#include "Scene.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <json.hpp>

int main()
{
    Window::CreateWindow(1280, 720, "Eclipse Engine");
    VulkanRenderer::StartUp();

    Scene scene;


    while (!glfwWindowShouldClose(Window::GetWindowPtr()))
    {
        glfwPollEvents();
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }
        ImGui::Render();
        scene.Update();
        scene.Draw();
        //GameController::IsButtonPressed(GLFW_GAMEPAD_BUTTON_CROSS);
        //GameController::IsJoyStickMoved(GLFW_GAMEPAD_BUTTON_CROSS);
        //GameController::IsJoyStickMoved(GLFW_GAMEPAD_AXIS_LEFT_TRIGGER);
    }
    vkDeviceWaitIdle(VulkanRenderer::GetDevice());

    VulkanRenderer::Destroy();
}

