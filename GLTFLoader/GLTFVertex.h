#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include <array>
#include <glm/glm.hpp>

struct GLTFVertex
{
    glm::vec3 Position = glm::vec3(0.0f);
    glm::vec3 Normal = glm::vec3(0.0f);
    glm::vec2 UV = glm::vec2(0.0f);
    glm::vec4 Tangant = glm::vec4(0.0f);
    glm::vec3 BiTangant = glm::vec3(0.0f);
    glm::vec3 Color = glm::vec3(1.0f);
};