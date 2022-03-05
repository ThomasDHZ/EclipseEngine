#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include <array>
#include <glm/glm.hpp>
#include <json.hpp>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 uv;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
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
        PositionAttribute.offset = offsetof(Vertex, pos);
        AttributeDescriptionList.emplace_back(PositionAttribute);

        VkVertexInputAttributeDescription ColorAttribute;
        ColorAttribute.binding = 0;
        ColorAttribute.location = 1;
        ColorAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
        ColorAttribute.offset = offsetof(Vertex, color);
        AttributeDescriptionList.emplace_back(ColorAttribute);

        VkVertexInputAttributeDescription UVAttribute;
        UVAttribute.binding = 0;
        UVAttribute.location = 2;
        UVAttribute.format = VK_FORMAT_R32G32_SFLOAT;
        UVAttribute.offset = offsetof(Vertex, uv);
        AttributeDescriptionList.emplace_back(UVAttribute);

        return AttributeDescriptionList;
    }

    nlohmann::json ToJson()
    {
        nlohmann::json json;

        json["pos"] = { pos.x, pos.y, pos.z };
        json["color"] = { color.x, color.y, color.z };
        json["uv"] = { uv.x, uv.y };

        return json;
    }
};
