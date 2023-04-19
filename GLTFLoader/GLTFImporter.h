#pragma once
#include "GLTFFileLoader.h"
#include <vector>
#include <memory>
#include <iostream>
#include <vulkan/vulkan_core.h>
#include "GLTFVertex.h"
#include "GLTFLights.h"

enum TextureTypeEnum
{
	kUndefinedTexture,
	kTextureAtlus,
	kRenderedColorTexture,
	kRenderedDepthTexture,
	kReadableTexture,
	kDiffuseTextureMap,
	kSpecularTextureMap,
	kAlbedoTextureMap,
	kMetallicTextureMap,
	kRoughnessTextureMap,
	kAmbientOcclusionTextureMap,
	kNormalTextureMap,
	kDepthTextureMap,
	kAlphaTextureMap,
	kEmissionTextureMap,
	kCubeMapTexture,
	kCubeMapDepthTexture,
	kEnvironmentTexture,
	kRenderedCubeMap,
	kBakedTexture
};

struct GLTFTextureDetails
{
	std::string name;
	std::string uri;
	int width;
	int height;
	int component;
	int bits;
	int pixel_type;
	std::vector<unsigned char> image;
};

struct GLTFSamplerDetails
{
	std::string name;
	int minFilter = VK_FILTER_NEAREST;
	int magFilter = VK_FILTER_NEAREST;
	int wrapU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	int wrapV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
};

struct GLTFTextureLoader
{
	GLTFTextureDetails TextureLoader;
	GLTFSamplerDetails SamplerLoader;
	VkFormat Format;
	TextureTypeEnum TextureType;
};

struct GLTFMaterialLoader
{
	glm::vec3 Albedo = glm::vec3(0.0f, 0.35f, 0.45);
	float Metallic = 0.0f;
	float Roughness = 0.0f;
	float AmbientOcclusion = 1.0f;
	glm::vec3 Emission = glm::vec3(0.0f);
	float Alpha = 1.0f;

	std::string MaterialName;
	GLTFTextureLoader AlbedoMap;
	GLTFTextureLoader MetallicRoughnessMap;
	GLTFTextureLoader AmbientOcclusionMap;
	GLTFTextureLoader NormalMap;
	GLTFTextureLoader DepthMap;
	GLTFTextureLoader AlphaMap;
	GLTFTextureLoader EmissionMap;
};

struct GLTFPrimitive
{
	uint32_t FirstIndex = 0;
	uint32_t IndexCount = 0;
	uint32_t material = 0;
};

struct GLTFNode
{
public:
	std::string NodeName;

	uint64_t ParentGameObjectID = 0;
	uint64_t ParentModelID = 0;
	int NodeID = -1;

	std::shared_ptr<GLTFNode> ParentMesh;
	std::vector<std::shared_ptr<GLTFNode>> ChildMeshList;
	GLTFMaterialLoader Material;

	std::vector<GLTFPrimitive> PrimitiveList;

	glm::mat4 ModelTransformMatrix = glm::mat4(1.0f);
	glm::mat4 NodeTransformMatrix = glm::mat4(1.0f);
	bool Visible = true;

	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 Rotation = glm::vec3(0.0f);
	glm::vec3 Scale = glm::vec3(1.0f);

	GLTFNode() {}
	~GLTFNode() {}
};

struct GLTFModelData
{
	std::vector<GLTFVertex> VertexList = std::vector<GLTFVertex>();
	std::vector<uint32_t> IndexList = std::vector<uint32_t>();
	std::vector<std::shared_ptr<GLTFNode>> NodeList = std::vector<std::shared_ptr<GLTFNode>>();
	std::vector<GLTFMaterialLoader> MaterialList = std::vector<GLTFMaterialLoader>();
	std::vector<GLTFSunLightLoader> SunLightList = std::vector<GLTFSunLightLoader>();
	std::vector<GLTFDirectionalLightLoader> DirectionalLightList = std::vector<GLTFDirectionalLightLoader>();
	std::vector<GLTFPointLightLoader> PointLightList = std::vector<GLTFPointLightLoader>();
	std::vector<GLTFSpotLightLoader> SpotLightList = std::vector<GLTFSpotLightLoader>();
};

class GLTFImporter
{
private: 
	uint32_t VertexCount;
	uint32_t IndexCount;
	uint32_t VertexStartIndex;
	uint32_t FirstIndex;

protected:
	std::vector<GLTFVertex> VertexList;
	std::vector<uint32_t> IndexList;
	std::vector<std::shared_ptr<GLTFNode>> NodeList;
	tinygltf::Model model;

	GLTFModelData data;

	void LoadLights(tinygltf::Model& model, tinygltf::Node& node);
	void LoadTextureDetails(const tinygltf::Image tinygltfImage, GLTFTextureDetails& TextureLoader);
	void LoadSamplerDetails(const tinygltf::Sampler tinygltfSampler, GLTFSamplerDetails SamplerLoader);
	void LoadMaterial(tinygltf::Model& model);
	void LoadMesh(tinygltf::Model& model, tinygltf::Node& node, std::shared_ptr<GLTFNode> parentNode, int index);
};

