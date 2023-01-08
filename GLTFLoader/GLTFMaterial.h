#pragma once
#include <VulkanBuffer.h>

struct GLTFMaterial
{
	std::string MaterialName;

	glm::vec4 AlbedoFactor;
	glm::vec3 MetallicFactor;
	glm::vec3 roughnessFactor;
	glm::vec3 EmissiveFactor;

	uint32_t AlbedoTextureIndex;
	uint32_t MetallicRoughnessIndex;
	uint32_t NormalTextureIndex;
	uint32_t OcclusionTextureIndex;
	uint32_t EmissiveTextureIndex;
};

