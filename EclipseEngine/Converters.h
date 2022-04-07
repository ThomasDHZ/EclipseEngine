#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Converter
{
public:

	static glm::vec1 PixelToVec1(Pixel pixel)
	{
		const float red = (float)pixel.Red / 255.0f;
		return glm::vec1(red);
	}

	static glm::vec2 PixelToVec2(Pixel pixel)
	{
		const float red = (float)pixel.Red / 255.0f;
		const float green = (float)pixel.Green / 255.0f;

		return glm::vec2(red, green);
	}

	static glm::vec3 PixelToVec3(Pixel pixel)
	{
		const float red = (float)pixel.Red / 255.0f;
		const float green = (float)pixel.Green / 255.0f;
		const float blue = (float)pixel.Blue / 255.0f;

		return glm::vec3(red, green, blue);
	}

	static glm::vec4 PixelToVec4(Pixel pixel)
	{
		const float red = (float)pixel.Red / 255.0f;
		const float green = (float)pixel.Green / 255.0f;
		const float blue = (float)pixel.Blue / 255.0f;
		const float alpha = (float)pixel.Alpha / 255.0f;

		return glm::vec4(red, green, blue, alpha);
	}

	static glm::mat4 AssimpToGLMMatrixConverter(aiMatrix4x4 AssMatrix)
	{
		glm::mat4 GLMMatrix;
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				GLMMatrix[x][y] = AssMatrix[y][x];
			}
		}
		return GLMMatrix;
	}

	static VkTransformMatrixKHR GLMToVkTransformMatrix(glm::mat4 matrix)
	{
		return VkTransformMatrixKHR
		{
			matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3],
			matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3],
			matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3],
		};
	}
};