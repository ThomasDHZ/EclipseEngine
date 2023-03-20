#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>
#include <glm/glm.hpp>
#include "VulkanBuffer.h"
#include "Camera.h"

struct SkyboxVertexLayout
{
    glm::vec4 Position = glm::vec4(0.0f);

    static std::vector<VkVertexInputBindingDescription> getBindingDescriptions()
    {
        std::vector<VkVertexInputBindingDescription>  bindingDescriptionList{};
        VkVertexInputBindingDescription bindingDescription{};

        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(SkyboxVertexLayout);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        bindingDescriptionList.emplace_back(bindingDescription);

        return bindingDescriptionList;
    }

    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions()
    {
        std::vector<VkVertexInputAttributeDescription> AttributeDescriptions = {};

        VkVertexInputAttributeDescription AttributeDescription;

        AttributeDescription.binding = 0;
        AttributeDescription.location = 0;
        AttributeDescription.format = VK_FORMAT_R32G32_SFLOAT;
        AttributeDescription.offset = offsetof(SkyboxVertexLayout, Position);
        AttributeDescriptions.emplace_back(AttributeDescription);

        return AttributeDescriptions;
    }
};

struct SkyBoxView
{
    alignas(16) glm::mat4 proj = glm::mat4(1.0f);
    alignas(16) glm::mat4 view = glm::mat4(1.0f);
};

class Skybox
{
private:

    VulkanBuffer VertexBuffer;
    VulkanBuffer IndexBuffer;
    SkyBoxView skyBoxView;

public:
	Skybox();
	~Skybox();

	void StartUp();
    void Update(std::shared_ptr<Camera> camera);
    void Draw(VkCommandBuffer& commandBuffer, VkPipelineLayout layout);
    void Destroy();
};
