#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Converter
{
public:
	//static glm::mat4 AssimpToGLMMatrixConverter(aiMatrix4x4 AssMatrix)
	//{
	//	glm::mat4 GLMMatrix;
	//	for (int y = 0; y < 4; y++)
	//	{
	//		for (int x = 0; x < 4; x++)
	//		{
	//			GLMMatrix[x][y] = AssMatrix[y][x];
	//		}
	//	}
	//	return GLMMatrix;
	//}

	//static VkTransformMatrixKHR GLMToVkTransformMatrix(glm::mat4 matrix)
	//{
	//	return VkTransformMatrixKHR
	//	{
	//		matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3],
	//		matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3],
	//		matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3],
	//	};
	//}
};