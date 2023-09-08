#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include <array>
#include <glm/glm.hpp>
#include <GLTFVertex.h>

struct VoidVertex {

    VoidVertex()
    {
    }

    static std::vector<VkVertexInputBindingDescription> getBindingDescriptions()
    {
        std::vector<VkVertexInputBindingDescription>  bindingDescriptionList{};
        return bindingDescriptionList;
    }

    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions()
    {
        std::vector<VkVertexInputAttributeDescription> AttributeDescriptionList{};
        return AttributeDescriptionList;
    }
};

struct LineVertex2D {
    glm::vec2 pos = glm::vec2(0.0f);
    glm::vec3 color = glm::vec3(1.0f);

    LineVertex2D()
    {
    }

    LineVertex2D(glm::vec2 Pos)
    {
        pos = Pos;
        color = glm::vec3(1.0f);
    }

    LineVertex2D(glm::vec2 Pos, glm::vec3 Color)
    {
        pos = Pos;
        color = Color;
    }

    static std::vector<VkVertexInputBindingDescription> getBindingDescriptions()
    {
        std::vector<VkVertexInputBindingDescription>  bindingDescriptionList{};
        VkVertexInputBindingDescription bindingDescription{};

        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(LineVertex2D);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        bindingDescriptionList.emplace_back(bindingDescription);

        return bindingDescriptionList;
    }

    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions()
    {
        std::vector<VkVertexInputAttributeDescription> AttributeDescriptionList{};

        VkVertexInputAttributeDescription PositionAttribute;
        PositionAttribute.binding = 0;
        PositionAttribute.location = 0;
        PositionAttribute.format = VK_FORMAT_R32G32_SFLOAT;
        PositionAttribute.offset = offsetof(LineVertex2D, pos);
        AttributeDescriptionList.emplace_back(PositionAttribute);

        VkVertexInputAttributeDescription ColorAttribute;
        ColorAttribute.binding = 0;
        ColorAttribute.location = 1;
        ColorAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
        ColorAttribute.offset = offsetof(LineVertex2D, color);
        AttributeDescriptionList.emplace_back(ColorAttribute);

        return AttributeDescriptionList;
    }
};

struct LineVertex3D {
    glm::vec3 pos = glm::vec3(0.0f);
    glm::vec4 color = glm::vec4(1.0f);

    LineVertex3D()
    {

    }

    LineVertex3D(glm::vec3 Pos)
    {
        pos = Pos;
        color = glm::vec4(1.0f);
    }

    LineVertex3D(glm::vec3 Pos, glm::vec4 Color)
    {
        pos = Pos;
        color = Color;
    }

    static std::vector<VkVertexInputBindingDescription> getBindingDescriptions()
    {
        std::vector<VkVertexInputBindingDescription>  bindingDescriptionList{};
        VkVertexInputBindingDescription bindingDescription{};

        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(LineVertex3D);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        bindingDescriptionList.emplace_back(bindingDescription);

        return bindingDescriptionList;
    }

    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() 
    {
        std::vector<VkVertexInputAttributeDescription> AttributeDescriptionList{};

        VkVertexInputAttributeDescription PositionAttribute;
        PositionAttribute.binding = 0;
        PositionAttribute.location = 0;
        PositionAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
        PositionAttribute.offset = offsetof(LineVertex3D, pos);
        AttributeDescriptionList.emplace_back(PositionAttribute);

        VkVertexInputAttributeDescription ColorAttribute;
        ColorAttribute.binding = 0;
        ColorAttribute.location = 1;
        ColorAttribute.format = VK_FORMAT_R32G32B32A32_SFLOAT;
        ColorAttribute.offset = offsetof(LineVertex3D, color);
        AttributeDescriptionList.emplace_back(ColorAttribute);

        return AttributeDescriptionList;
    }
};

struct Vertex2D
{
    glm::vec2 Position = glm::vec2(0.0f);
    glm::vec2 UV = glm::vec2(0.0f);
    glm::vec4 Color = glm::vec4(1.0f);

    static std::vector<VkVertexInputBindingDescription> getBindingDescriptions()
    {
        std::vector<VkVertexInputBindingDescription>  bindingDescriptionList{};
        VkVertexInputBindingDescription bindingDescription{};

        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex2D);
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
        AttributeDescription.offset = offsetof(Vertex2D, Position);
        AttributeDescriptions.emplace_back(AttributeDescription);

        AttributeDescription.binding = 0;
        AttributeDescription.location = 1;
        AttributeDescription.format = VK_FORMAT_R32G32_SFLOAT;
        AttributeDescription.offset = offsetof(Vertex2D, UV);
        AttributeDescriptions.emplace_back(AttributeDescription);

        AttributeDescription.binding = 0;
        AttributeDescription.location = 2;
        AttributeDescription.format = VK_FORMAT_R32G32B32A32_SFLOAT;
        AttributeDescription.offset = offsetof(Vertex2D, Color);
        AttributeDescriptions.emplace_back(AttributeDescription);

        return AttributeDescriptions;
    }
};

struct InstancedVertexData2D
{
    InstancedVertexData2D() {};

    glm::mat4 InstanceModel = glm::mat4(1.0f);
    glm::vec2 UVOffset = glm::vec2(0.0f);
    uint32_t MaterialID = 0;

    static std::vector<VkVertexInputBindingDescription> getBindingDescriptions()
    {
        std::vector<VkVertexInputBindingDescription>  bindingDescriptionList{};
        VkVertexInputBindingDescription bindingDescription{};

        bindingDescription.binding = 1;
        bindingDescription.stride = sizeof(InstancedVertexData2D);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
        bindingDescriptionList.emplace_back(bindingDescription);

        return bindingDescriptionList;
    }

    static std::vector<VkVertexInputAttributeDescription> AddInstnacingAttributeDescription(std::vector<VkVertexInputAttributeDescription>& AttributeDescriptions)
    {
        VkVertexInputAttributeDescription AttributeDescription;

        AttributeDescription.binding = 1;
        AttributeDescription.location = 3;
        AttributeDescription.format = VK_FORMAT_R32G32B32A32_SFLOAT;
        AttributeDescription.offset = sizeof(glm::vec4) * 0;
        AttributeDescriptions.emplace_back(AttributeDescription);

        AttributeDescription.binding = 1;
        AttributeDescription.location = 4;
        AttributeDescription.format = VK_FORMAT_R32G32B32A32_SFLOAT;
        AttributeDescription.offset = sizeof(glm::vec4) * 1;
        AttributeDescriptions.emplace_back(AttributeDescription);

        AttributeDescription.binding = 1;
        AttributeDescription.location = 5;
        AttributeDescription.format = VK_FORMAT_R32G32B32A32_SFLOAT;
        AttributeDescription.offset = sizeof(glm::vec4) * 2;
        AttributeDescriptions.emplace_back(AttributeDescription);

        AttributeDescription.binding = 1;
        AttributeDescription.location = 6;
        AttributeDescription.format = VK_FORMAT_R32G32B32A32_SFLOAT;
        AttributeDescription.offset = sizeof(glm::vec4) * 3;
        AttributeDescriptions.emplace_back(AttributeDescription);

        AttributeDescription.binding = 1;
        AttributeDescription.location = 7;
        AttributeDescription.format = VK_FORMAT_R32G32_SFLOAT;
        AttributeDescription.offset = offsetof(InstancedVertexData2D, UVOffset);
        AttributeDescriptions.emplace_back(AttributeDescription);

        AttributeDescription.binding = 1;
        AttributeDescription.location = 8;
        AttributeDescription.format = VK_FORMAT_R32_SINT;
        AttributeDescription.offset = offsetof(InstancedVertexData2D, MaterialID);
        AttributeDescriptions.emplace_back(AttributeDescription);

        //AttributeDescription.binding = 1;
        //AttributeDescription.location = 9;
        //AttributeDescription.format = VK_FORMAT_R8_;
        //AttributeDescription.offset = offsetof(InstancedVertexData2D, MaterialID);
        //AttributeDescriptions.emplace_back(AttributeDescription);

        return AttributeDescriptions;
    }
};

struct Vertex3D
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

    Vertex3D(){}

    static std::vector<VkVertexInputBindingDescription> getBindingDescriptions()
    {
        std::vector<VkVertexInputBindingDescription>  bindingDescriptionList{};
        VkVertexInputBindingDescription bindingDescription{};

        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex3D);
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
        AttributeDescription.offset = offsetof(Vertex3D, Position);
        AttributeDescriptions.emplace_back(AttributeDescription);

        AttributeDescription.binding = 0;
        AttributeDescription.location = 1;
        AttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
        AttributeDescription.offset = offsetof(Vertex3D, Normal);
        AttributeDescriptions.emplace_back(AttributeDescription);

        AttributeDescription.binding = 0;
        AttributeDescription.location = 2;
        AttributeDescription.format = VK_FORMAT_R32G32_SFLOAT;
        AttributeDescription.offset = offsetof(Vertex3D, UV);
        AttributeDescriptions.emplace_back(AttributeDescription);

        AttributeDescription.binding = 0;
        AttributeDescription.location = 3;
        AttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
        AttributeDescription.offset = offsetof(Vertex3D, Tangant);
        AttributeDescriptions.emplace_back(AttributeDescription);

        AttributeDescription.binding = 0;
        AttributeDescription.location = 4;
        AttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
        AttributeDescription.offset = offsetof(Vertex3D, BiTangant);
        AttributeDescriptions.emplace_back(AttributeDescription);

        AttributeDescription.binding = 0;
        AttributeDescription.location = 5;
        AttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
        AttributeDescription.offset = offsetof(Vertex3D, Color);
        AttributeDescriptions.emplace_back(AttributeDescription);

        return AttributeDescriptions;
    }
    
    Vertex3D& operator= (const GLTFVertex& gltfVertex)
    {
        Vertex3D vertex;
        vertex.Position = gltfVertex.Position;
        vertex.Normal = gltfVertex.Normal;
        vertex.UV = gltfVertex.UV;
        vertex.Tangant = gltfVertex.Tangant;
        vertex.BiTangant = gltfVertex.BiTangant;
        vertex.Color = gltfVertex.Color;
        return vertex;
    }

    std::vector<Vertex3D>& operator= (const std::vector<GLTFVertex>& gltfVertexList)
    {
        std::vector<Vertex3D> VertexList;
        for (GLTFVertex gltfVertex : gltfVertexList)
        {
            Vertex3D vertex;
            vertex.Position = gltfVertex.Position;
            vertex.Normal = gltfVertex.Normal;
            vertex.UV = gltfVertex.UV;
            vertex.Tangant = gltfVertex.Tangant;
            vertex.BiTangant = gltfVertex.BiTangant;
            vertex.Color = gltfVertex.Color;
            VertexList.emplace_back(vertex);
        }
        return VertexList;
    }
};

struct InstancedVertexData3D
{
    InstancedVertexData3D() {};

    glm::mat4 InstanceModel = glm::mat4(1.0f);
    uint32_t MaterialID = 0;

    static std::vector<VkVertexInputBindingDescription> getBindingDescriptions() 
    {
        std::vector<VkVertexInputBindingDescription>  bindingDescriptionList{};
        VkVertexInputBindingDescription bindingDescription{};

        bindingDescription.binding = 1;
        bindingDescription.stride = sizeof(InstancedVertexData3D);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
        bindingDescriptionList.emplace_back(bindingDescription);

        return bindingDescriptionList;
    }

    static std::vector<VkVertexInputAttributeDescription> AddInstnacingAttributeDescription(std::vector<VkVertexInputAttributeDescription>& AttributeDescriptions)
    {
        VkVertexInputAttributeDescription AttributeDescription;

        AttributeDescription.binding = 1;
        AttributeDescription.location = 6;
        AttributeDescription.format = VK_FORMAT_R32G32B32A32_SFLOAT;
        AttributeDescription.offset = sizeof(glm::vec4) * 0;
        AttributeDescriptions.emplace_back(AttributeDescription);

        AttributeDescription.binding = 1;
        AttributeDescription.location = 7;
        AttributeDescription.format = VK_FORMAT_R32G32B32A32_SFLOAT;
        AttributeDescription.offset = sizeof(glm::vec4) * 1;
        AttributeDescriptions.emplace_back(AttributeDescription);

        AttributeDescription.binding = 1;
        AttributeDescription.location = 8;
        AttributeDescription.format = VK_FORMAT_R32G32B32A32_SFLOAT;
        AttributeDescription.offset = sizeof(glm::vec4) * 2;
        AttributeDescriptions.emplace_back(AttributeDescription);

        AttributeDescription.binding = 1;
        AttributeDescription.location = 9;
        AttributeDescription.format = VK_FORMAT_R32G32B32A32_SFLOAT;
        AttributeDescription.offset = sizeof(glm::vec4) * 3;
        AttributeDescriptions.emplace_back(AttributeDescription);

        AttributeDescription.binding = 1;
        AttributeDescription.location = 10;
        AttributeDescription.format = VK_FORMAT_R32_SINT;
        AttributeDescription.offset = sizeof(InstancedVertexData3D) - sizeof(uint32_t);
        AttributeDescriptions.emplace_back(AttributeDescription);

        return AttributeDescriptions;
    }
};