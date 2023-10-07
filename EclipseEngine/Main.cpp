#include <iostream>
#include "VulkanRenderer.h"
#include "Scene.h"
#include <json.hpp>
#include "GameObject.h"
#include "InterfaceRenderPass.h"
#include "SoundDevice.h"
#include "SoundEffectManager.h"
#include "Timer.h"



VkRenderPass InterfaceRenderPass::RenderPass = VK_NULL_HANDLE;
VkDescriptorPool InterfaceRenderPass::ImGuiDescriptorPool = VK_NULL_HANDLE;
VkCommandPool InterfaceRenderPass::ImGuiCommandPool = VK_NULL_HANDLE;

std::vector<VkFramebuffer> InterfaceRenderPass::SwapChainFramebuffers;
std::vector<VkCommandBuffer> InterfaceRenderPass::ImGuiCommandBuffers;

//std::vector<uint32_t> HLSLToSpriV(size_t size, const char* src, VkShaderStageFlagBits stage)
//{
//    Microsoft::WRL::ComPtr<IDxcUtils> dxc_utils{};
//    Microsoft::WRL::ComPtr<IDxcCompiler3> dxc_compiler{};
//    Microsoft::WRL::ComPtr<IDxcLibrary> library{};
//    auto a = DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&library));
//    auto b = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(dxc_utils.ReleaseAndGetAddressOf()));
//    auto c = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxc_compiler));
//
//    DxcBuffer src_buffer = {
//        .Ptr = src,
//        .Size = static_cast<uint32_t>(size),
//        .Encoding = 0
//    };
//
//    std::vector<LPCWSTR> args;
//    args.emplace_back(L"Fragment Shader");
//    args.emplace_back(L"-Zpc");
//    args.emplace_back(L"-HV");
//    args.emplace_back(L"2021");
//    args.emplace_back(L"-T");
//    args.emplace_back(L"ps_6_0");
//    args.emplace_back(L"-E");
//    args.emplace_back(L"main");
//    args.emplace_back(L"-spirv");
//    args.emplace_back(L"-fspv-target-env=vulkan1.3");
//
//    Microsoft::WRL::ComPtr<IDxcResult> result;
//    auto d = dxc_compiler->Compile(&src_buffer, args.data(), static_cast<uint32_t>(args.size()), nullptr, IID_PPV_ARGS(&result));
//
//    Microsoft::WRL::ComPtr<IDxcBlob> shader_obj;
//    auto e = result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shader_obj), nullptr);
//
//    Microsoft::WRL::ComPtr<IDxcBlobUtf8> error_message;
//    result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&error_message), nullptr);
//    if (error_message && error_message->GetStringLength() > 0)
//    {
//        auto string = std::string();
//        string.resize(error_message->GetBufferSize());
//        for (size_t x = 0; x < string.size(); x++)
//        {
//            string[x] = static_cast<const char*>(error_message->GetBufferPointer())[x];
//        }
//
//        MessageBoxA(nullptr, string.c_str(), "DXC Compile Error", MB_OK);
//    }
//
//    std::vector<uint32_t> spriv_buffer;
//    spriv_buffer.resize(shader_obj->GetBufferSize() / sizeof(uint32_t));
//
//    for (int x = 0; x < spriv_buffer.size(); x++)
//    {
//        uint32_t spv_uint = static_cast<std::uint32_t*>(shader_obj->GetBufferPointer())[x];
//        spriv_buffer[x] = spv_uint;
//    }
//
//    return spriv_buffer;
//}

int main()
{
    //std::ifstream ifs("C:/Users/dotha/source/repos/EclipseEngine/EclipseEngine/WireFrameShader.hlsl");
    //std::string str;
    //ifs.seekg(0, std::ios::end);
    //str.reserve(ifs.tellg());
    //ifs.seekg(0, std::ios::beg);
    //str.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());

    //auto spirv_buffer = HLSLToSpriV(str.size(), str.c_str(), VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT);

    //VkShaderModule shaderModule{};
    //VkShaderModuleCreateInfo ShaderModuleCreateInfo{};
    //ShaderModuleCreateInfo.codeSize = spirv_buffer.size() *sizeof(spirv_buffer[0]);
    //ShaderModuleCreateInfo.pCode = (uint32_t*)spirv_buffer.data();

    //auto f = vkCreateShaderModule(VulkanRenderer::GetDevice(), &ShaderModuleCreateInfo, nullptr, &shaderModule);

    Window::CreateGraphicsWindow(1280, 720, "Eclipse Engine");
    SoundDevice::StartUp();
    SoundEffectManager::StartUp();
    VulkanRenderer::StartUp();
    InterfaceRenderPass::StartUp();
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
            GameController::IsButtonPressed(GLFW_GAMEPAD_BUTTON_CROSS);
            GameController::IsJoyStickMoved(GLFW_GAMEPAD_BUTTON_CROSS);
            GameController::IsJoyStickMoved(GLFW_GAMEPAD_AXIS_LEFT_TRIGGER);
    }
    vkDeviceWaitIdle(VulkanRenderer::GetDevice());


    scene.Destroy();

    SceneManager::Destory();
    InterfaceRenderPass::Destroy();
    VulkanRenderer::Destroy();
    SoundEffectManager::Destroy();
    SoundDevice::Destroy();
    Window::Destroy();
}

