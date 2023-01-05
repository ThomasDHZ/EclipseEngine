#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include <array>
#include <glm/glm.hpp>

enum GLTFVertexEnum
{
    kGLTFVertexUndefined,
    kGLTFLineVertex2D,
    kGLTFLineVertex3D,
    kGLTFVertex2D,
    kGLTFVertex3D,
    kGLTFInstancedData3D
};

struct GLTFLineVertex2D {
    glm::vec2 pos;
    glm::vec4 color;
};

struct GLTFLineVertex3D {
    glm::vec3 pos;
    glm::vec4 color;
};

struct GLTFVertex2D
{
    glm::vec2 Position = glm::vec2(0.0f);
    glm::vec2 UV = glm::vec2(0.0f);
    glm::vec3 Color = glm::vec3(1.0f);
};

struct GLTFVertex3D
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
};

struct GLTFInstancedData3D
{
    glm::mat4 InstanceModel = glm::mat4(1.0f);
    uint32_t MaterialID = 0;
};