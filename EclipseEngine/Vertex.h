#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include <array>
#include <glm/glm.hpp>
#include "JsonConverter.h"

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

    void to_json(nlohmann::json& json)
    {
        JsonConverter::to_json(json["pos"], pos);
        JsonConverter::to_json(json["color"], color);
    }
};

struct MeshVertex 
{
    glm::vec3 Position = glm::vec3(0.0f);
    float PositionPadding = 0.0f;
    glm::vec3 Normal = glm::vec3(0.0f);
    float NormalPadding = 0.0f;
    glm::vec2 UV = glm::vec2(0.0f);
    glm::vec2 UVPadding = glm::vec2(0.0f);
    glm::vec3 Tangant = glm::vec3(0.0f);
    float TangantPadding = 0.0f;
    glm::vec3 BiTangant = glm::vec3(0.0f);
    float BiTangentPadding = 0.0f;
    glm::vec3 Color = glm::vec3(1.0f);
    float ColorPadding = 0.0f;

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
        AttributeDescription.offset = offsetof(MeshVertex, UV);
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

        AttributeDescription.binding = 0;
        AttributeDescription.location = 5;
        AttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
        AttributeDescription.offset = offsetof(MeshVertex, Color);
        AttributeDescriptions.emplace_back(AttributeDescription);

        return AttributeDescriptions;
    }

    void to_json(nlohmann::json& json)
    {
        JsonConverter::to_json(json["Position"], Position);
        JsonConverter::to_json(json["PositionPadding"], PositionPadding);
        JsonConverter::to_json(json["Normal"], Normal);
        JsonConverter::to_json(json["NormalPadding"], NormalPadding);
        JsonConverter::to_json(json["UV"], UV);
        JsonConverter::to_json(json["UVPadding"], UVPadding);
        JsonConverter::to_json(json["Tangant"], Tangant);
        JsonConverter::to_json(json["TangantPadding"], TangantPadding);
        JsonConverter::to_json(json["BiTangant"], BiTangant);
        JsonConverter::to_json(json["BiTangentPadding"], BiTangentPadding);
        JsonConverter::to_json(json["Color"], Color);
        JsonConverter::to_json(json["ColorPadding"], ColorPadding);
    }
};