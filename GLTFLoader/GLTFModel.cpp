#include "GLTFModel.h"
#include "GLTFMaterial.h"
#include <glm/glm/gtc/type_ptr.inl>
#include <glm/gtc/quaternion.hpp>

GLTFModel::GLTFModel()
{
}

GLTFModel::GLTFModel(const char* filename, GLTFVertexEnum vertexType)
{
	VertexType = vertexType;

	auto file = "C:/Users/dotha/Desktop/Vulkan-master/Vulkan/data/models/FlightHelmet/glTF/FlightHelmet.gltf";
	model = GLTFFileLoader::Loader(file);

	ModelMatrix = glm::mat4(1.0f);

	const tinygltf::Scene& scene = model.scenes[model.defaultScene];
	const tinygltf::Node node = model.nodes[scene.nodes[0]];

	LoadMaterials(model);
	for (int x = 0; x < scene.nodes.size(); x++)
	{
		LoadModelNodes(model.nodes[scene.nodes[x]]);
	}
}

GLTFModel::~GLTFModel()
{
}

void GLTFModel::LoadTextureDetails(const tinygltf::Image tinygltfImage, TinyGltfTextureLoader& TextureLoader)
{
	TextureLoader.image = tinygltfImage.image;
	TextureLoader.uri = tinygltfImage.uri;
	TextureLoader.bits = tinygltfImage.bits;
	TextureLoader.component = tinygltfImage.component;
	TextureLoader.height = tinygltfImage.height;
	TextureLoader.name = tinygltfImage.name;
	TextureLoader.pixel_type = tinygltfImage.pixel_type;
	TextureLoader.width = tinygltfImage.width;
}

void GLTFModel::LoadSamplerDetails(const tinygltf::Sampler tinygltfSampler, TinyGltfTextureSamplerLoader SamplerLoader)
{
	SamplerLoader.magFilter = tinygltfSampler.magFilter;
	SamplerLoader.minFilter = tinygltfSampler.magFilter;
	SamplerLoader.name = tinygltfSampler.magFilter;
	SamplerLoader.wrapU = tinygltfSampler.magFilter;
	SamplerLoader.wrapV = tinygltfSampler.magFilter;
}

void GLTFModel::LoadMaterials(tinygltf::Model& model)
{
	std::vector<tinygltf::Image> tinygltfImage;
	std::vector<tinygltf::Texture> tinygltfTexture;
	std::vector<tinygltf::Sampler> tinygltfSampler;

	for (uint32_t x = 0; x < model.images.size(); x++)
	{
		tinygltfImage.emplace_back(model.images[x]);
	}

	for (uint32_t x = 0; x < model.textures.size(); x++)
	{
		tinygltfTexture.emplace_back(model.textures[x]);
	}

	for (uint32_t x = 0; x < model.samplers.size(); x++)
	{
		tinygltfSampler.emplace_back(model.samplers[x]);
	}

	for (uint32_t x = 0; x < model.materials.size(); x++)
	{
		std::shared_ptr<Material> material = std::make_shared<Material>(model.materials[x].name, MaterialTypeEnum::kMaterialPBR);

		TinyGltfTextureLoader TextureLoader{};
		TinyGltfTextureSamplerLoader SamplerLoader{};
		tinygltf::Material glTFMaterial = model.materials[x];

		const int imageIndex = tinygltfTexture[x].source;
		const int samplerIndex = tinygltfTexture[x].sampler;

		if (glTFMaterial.values.find("baseColorFactor") != glTFMaterial.values.end()) 
		{
			material->SetAlbedoMap(glm::make_vec4(glTFMaterial.values["baseColorFactor"].ColorFactor().data()));
		}

		if (glTFMaterial.values.find("metallicFactor") != glTFMaterial.values.end())
		{
			material->SetMetallicMap(*reinterpret_cast<float*>(glTFMaterial.values["metallicFactor"].ColorFactor().data()));
		}

		if (glTFMaterial.values.find("roughnessFactor") != glTFMaterial.values.end())
		{
			material->SetRoughnessMap(*reinterpret_cast<float*>(glTFMaterial.values["roughnessFactor"].ColorFactor().data()));
		}

		if (glTFMaterial.values.find("baseColorTexture") != glTFMaterial.values.end()) 
		{
			const uint32_t TextureIndex = glTFMaterial.pbrMetallicRoughness.baseColorTexture.index;

			LoadTextureDetails(tinygltfImage[TextureIndex], TextureLoader);
			TextureList.emplace_back(std::make_shared<Texture2D>(Texture2D(TextureLoader, SamplerLoader, VK_FORMAT_R8G8B8A8_SRGB, TextureTypeEnum::kAlbedoTextureMap)));
			material->SetAlbedoMap(TextureList.back());
			VRAMManager::LoadTexture2D(TextureList.back());
		}

		if (glTFMaterial.additionalValues.find("normalTexture") != glTFMaterial.additionalValues.end()) 
		{
			const uint32_t TextureIndex = glTFMaterial.normalTexture.index;

			LoadTextureDetails(tinygltfImage[TextureIndex], TextureLoader);
			if (tinygltfSampler.size() > 0)
			{
				LoadSamplerDetails(tinygltfSampler[TextureIndex], SamplerLoader);
			}
			TextureList.emplace_back(std::make_shared<Texture2D>(Texture2D(TextureLoader, SamplerLoader, VK_FORMAT_R8G8B8A8_UNORM, TextureTypeEnum::kNormalTextureMap)));
			material->SetNormalMap(TextureList.back());
			VRAMManager::LoadTexture2D(TextureList.back());
		}

		if (glTFMaterial.additionalValues.find("metallicRoughnessTexture") != glTFMaterial.additionalValues.end()) 
		{
			const uint32_t TextureIndex = glTFMaterial.pbrMetallicRoughness.metallicRoughnessTexture.index;

			LoadTextureDetails(tinygltfImage[TextureIndex], TextureLoader);
			if (tinygltfSampler.size() > 0)
			{
				LoadSamplerDetails(tinygltfSampler[TextureIndex], SamplerLoader);
			}
			TextureList.emplace_back(std::make_shared<Texture2D>(Texture2D(TextureLoader, SamplerLoader, VK_FORMAT_R8G8B8A8_UNORM, TextureTypeEnum::kMetallicTextureMap)));
			material->SetMetallicMap(TextureList.back());
			VRAMManager::LoadTexture2D(TextureList.back());
		}

		if (glTFMaterial.additionalValues.find("occlusionTexture") != glTFMaterial.additionalValues.end())
		{
			const uint32_t TextureIndex = glTFMaterial.occlusionTexture.index;
;
			LoadTextureDetails(tinygltfImage[TextureIndex], TextureLoader);
			if (tinygltfSampler.size() > 0)
			{
				LoadSamplerDetails(tinygltfSampler[TextureIndex], SamplerLoader);
			}
			TextureList.emplace_back(std::make_shared<Texture2D>(Texture2D(TextureLoader, SamplerLoader, VK_FORMAT_R8G8B8A8_UNORM, TextureTypeEnum::kAmbientOcclusionTextureMap)));
			material->SetAmbientOcclusionMap(TextureList.back());
			VRAMManager::LoadTexture2D(TextureList.back());
		}

		MaterialList.emplace_back(material);
		VRAMManager::AddMaterial(MaterialList.back());
	}
}

void GLTFModel::LoadModelNodes(tinygltf::Node& node)
{
	if (node.mesh >= 0 &&
		node.mesh < model.meshes.size())
	{
		GLTFMesh mesh = GLTFMesh(VertexType);
		mesh.LoadMesh(model, model.meshes[node.mesh], node);
		MeshList.emplace_back(mesh);
	}

	for (uint32_t x = 0; x < node.children.size(); x++)
	{
		LoadModelNodes(model.nodes[node.children[x]]);
	}
}