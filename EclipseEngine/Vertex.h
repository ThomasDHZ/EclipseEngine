#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include <array>
#include <glm/glm.hpp>
#include <json.hpp>

struct LineVertex {
    glm::vec3 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(LineVertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() 
    {
        std::vector<VkVertexInputAttributeDescription> AttributeDescriptionList{};

        VkVertexInputAttributeDescription PositionAttribute;
        PositionAttribute.binding = 0;
        PositionAttribute.location = 0;
        PositionAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
        PositionAttribute.offset = offsetof(LineVertex, pos);
        AttributeDescriptionList.emplace_back(PositionAttribute);

        VkVertexInputAttributeDescription ColorAttribute;
        ColorAttribute.binding = 0;
        ColorAttribute.location = 1;
        ColorAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
        ColorAttribute.offset = offsetof(LineVertex, color);
        AttributeDescriptionList.emplace_back(ColorAttribute);

        return AttributeDescriptionList;
    }

    nlohmann::json ToJson()
    {
        nlohmann::json json;

        json["pos"] = { pos.x, pos.y, pos.z };
        json["color"] = { color.x, color.y, color.z };

        return json;
    }
};

struct MeshVertex 
{
    glm::vec4 Position = glm::vec4(0.0f);
    glm::vec4 Normal = glm::vec4(0.0f);
    glm::vec4 TexureCoord = glm::vec4(0.0f);
    glm::vec4 Padding3 = glm::vec4(0.0f);
    glm::vec4 Tangant = glm::vec4(0.0f);
    glm::vec4 BiTangant = glm::vec4(0.0f);

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(MeshVertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions()
    {
        std::vector<VkVertexInputAttributeDescription> AttributeDescriptions = {};

        VkVertexInputAttributeDescription AttributeDescription;

        AttributeDescription.binding = 0;
        AttributeDescription.location = 0;
        AttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
        AttributeDescription.offset = offsetof(MeshVertex, Position);
        AttributeDescriptions.emplace_back(AttributeDescription);

        AttributeDescription.binding = 0;
        AttributeDescription.location = 1;
        AttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
        AttributeDescription.offset = offsetof(MeshVertex, Normal);
        AttributeDescriptions.emplace_back(AttributeDescription);

        AttributeDescription.binding = 0;
        AttributeDescription.location = 2;
        AttributeDescription.format = VK_FORMAT_R32G32_SFLOAT;
        AttributeDescription.offset = offsetof(MeshVertex, TexureCoord);
        AttributeDescriptions.emplace_back(AttributeDescription);

        AttributeDescription.binding = 0;
        AttributeDescription.location = 3;
        AttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
        AttributeDescription.offset = offsetof(MeshVertex, Tangant);
        AttributeDescriptions.emplace_back(AttributeDescription);

        AttributeDescription.binding = 0;
        AttributeDescription.location = 4;
        AttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
        AttributeDescription.offset = offsetof(MeshVertex, BiTangant);
        AttributeDescriptions.emplace_back(AttributeDescription);

        return AttributeDescriptions;
    }

    nlohmann::json ToJson()
    {
        nlohmann::json json;


        return json;
    }
};