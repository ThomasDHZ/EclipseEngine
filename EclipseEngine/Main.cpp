#include <iostream>
#include "VulkanRenderer.h"
#include "Scene.h"
#include <json.hpp>
#include "GameObject.h"
#include "TextureManager.h"
#include "InterfaceRenderPass.h"
#include "MaterialManager.h"
#include "LightManager.h"
#include "SoundDevice.h"
#include "SoundEffectManager.h"
#include <Timer.h>
#include <VRAMManager.h>

VkRenderPass InterfaceRenderPass::RenderPass = VK_NULL_HANDLE;
VkDescriptorPool InterfaceRenderPass::ImGuiDescriptorPool = VK_NULL_HANDLE;
VkCommandPool InterfaceRenderPass::ImGuiCommandPool = VK_NULL_HANDLE;

std::vector<VkFramebuffer> InterfaceRenderPass::SwapChainFramebuffers;
std::vector<VkCommandBuffer> InterfaceRenderPass::ImGuiCommandBuffers;

int main()
{
    Window::CreateGraphicsWindow(1280, 720, "Eclipse Engine");
    SoundDevice::StartUp();
    SoundEffectManager::StartUp();
    VulkanRenderer::StartUp();
    InterfaceRenderPass::StartUp();
    VRAMManager::StartUp();
    TextureManager::StartUp();
    MaterialManager::StartUp();
    //TopLevelAccelerationStructureManager::StartUp();
    LightManager::StartUp();
    SceneManager::StartUp();
    
    Scene scene;

    while (!glfwWindowShouldClose(Window::GetWindowPtr()))
    {
        glfwPollEvents();
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            scene.ImGuiUpdate();
        }
        ImGui::Render();
        scene.Update();
        scene.Draw();
    //    //GameController::IsButtonPressed(GLFW_GAMEPAD_BUTTON_CROSS);
    //    //GameController::IsJoyStickMoved(GLFW_GAMEPAD_BUTTON_CROSS);
    //    //GameController::IsJoyStickMoved(GLFW_GAMEPAD_AXIS_LEFT_TRIGGER);
    }
    vkDeviceWaitIdle(VulkanRenderer::GetDevice());


    scene.Destroy();

    SceneManager::Destory();
    TopLevelAccelerationStructureManager::Destroy();
    LightManager::Destory();
    MaterialManager::Destroy();
    TextureManager::Destroy();
    InterfaceRenderPass::Destroy();
    VulkanRenderer::Destroy();
    SoundEffectManager::Destroy();
    SoundDevice::Destroy();
    Window::Destroy();
}

