#pragma once
#include <vulkan/vulkan_core.h>
#include <glm/glm.hpp>
#include <cmath>

class EngineMath
{
public:
    static float Distance3D(const glm::vec3& pos1, const glm::vec3& pos2)
    {
        return std::sqrtf(pow(pos2.x - pos1.x, 2) + pow(pos2.y - pos1.y, 2) + pow(pos2.z - pos1.z, 2));
    }

    static VkTransformMatrixKHR GLMToVkTransformMatrix(glm::mat4& matrix)
    {
        return VkTransformMatrixKHR
        {
            matrix[0][0], matrix[0][1], matrix[0][2], matrix[3][0],
            matrix[1][0], matrix[1][1], matrix[1][2], matrix[3][1],
            matrix[2][0], matrix[2][1], matrix[2][2], matrix[3][2]
        };
    }
};