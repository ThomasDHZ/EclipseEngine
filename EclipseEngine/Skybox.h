#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>
#include <glm/glm.hpp>
#include "VulkanBuffer.h"
#include "Camera.h"
#include "UniformBuffer.h"

struct SkyboxVertexLayout
{
    glm::vec3 Position = glm::vec3(0.0f);

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
        AttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
        AttributeDescription.offset = offsetof(SkyboxVertexLayout, Position);
        AttributeDescriptions.emplace_back(AttributeDescription);

        return AttributeDescriptions;
    }
};


class Skybox
{
private:

    VulkanBuffer VertexBuffer;
    VulkanBuffer IndexBuffer;

public:
	Skybox();
	~Skybox();

    SkyBoxView skyBoxView;
	void StartUp();
    void Update(std::shared_ptr<Camera> camera);
    void Draw(VkCommandBuffer& commandBuffer, VkPipelineLayout layout, VkDescriptorSet descriptorSet);
    void Destroy();
};
