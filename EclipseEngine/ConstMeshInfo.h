#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "RenderedCubeMapTexture.h"
#include "RenderedDepthTexture.h"
#include "RenderedCubeMapDepthTexture.h"

struct ConstMeshInfo
{
	alignas(4) uint32_t MeshIndex;
	alignas(16) glm::mat4 proj = glm::mat4(1.0f);
	alignas(16) glm::mat4 view = glm::mat4(1.0f);
	alignas(16) glm::vec3 CameraPos = glm::vec3(0.0f);
};

struct PBRRenderPassTextureSubmitList
{
	std::shared_ptr<RenderedCubeMapTexture> CubeMapTexture;
	std::vector<std::shared_ptr<RenderedCubeMapTexture>> IrradianceTextureList;
	std::vector<std::shared_ptr<RenderedCubeMapTexture>> PrefilterTextureList;
	std::vector<std::shared_ptr<RenderedDepthTexture>> DirectionalLightTextureShadowMaps;
	std::vector<std::shared_ptr<RenderedCubeMapDepthTexture>> PointLightShadowMaps;
	std::vector<std::shared_ptr<RenderedDepthTexture>> SpotLightTextureShadowMaps;
};