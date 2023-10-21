#include "GLTFImporter.h"
#include <glm/glm/gtc/type_ptr.inl>
#include <glm/gtc/quaternion.hpp>

void GLTFImporter::LoadLights(tinygltf::Model& model, tinygltf::Node& node)
{
	if (node.mesh == -1)
	{
		auto color = glm::vec3(1.0f);
		auto intensity = 0.0f;

		for (auto& light : model.lights)
		{
			std::string baseName = node.name;
			if (node.name.find("_Orientation") != std::string::npos)
			{
				int stringPos = node.name.find("_Orientation");
				baseName = baseName.substr(0, stringPos);
			}
			if (light.name == baseName)
			{
				if (light.type == "point")
				{
					GLTFPointLightLoader pointLight{};
					pointLight.name = light.name;
					pointLight.position = glm::make_vec3(&node.translation[0]);
					pointLight.diffuse = glm::vec3(glm::make_vec3(light.color.data()));
					pointLight.intensity = light.intensity;
					pointLight.radius = 1.0f;
					data.PointLightList.emplace_back(pointLight);
				}
				if (light.type == "directional")
				{
					GLTFSunLightLoader sunLight{};
					sunLight.name = light.name;
					sunLight.position = glm::make_vec3(&node.translation[0]);
					sunLight.diffuse = glm::vec3(glm::make_vec3(light.color.data()));
					sunLight.intensity = light.intensity;
					data.SunLightList.emplace_back(sunLight);

					GLTFDirectionalLightLoader directionalLight{};
					directionalLight.name = light.name;
					directionalLight.direction = glm::make_vec3(&node.translation[0]);
					directionalLight.diffuse = glm::vec3(glm::make_vec3(light.color.data()));
					directionalLight.intensity = light.intensity;
					data.DirectionalLightList.emplace_back(directionalLight);
				}
				if (light.type == "spot")
				{
					GLTFSpotLightLoader spotLight{};
					spotLight.name = light.name;
					spotLight.direction = glm::make_vec3(&node.translation[0]);
					spotLight.diffuse = glm::vec3(glm::make_vec3(light.color.data()));
					spotLight.intensity = light.intensity;
					data.SpotLightList.emplace_back(spotLight);
				}
			}
		}
	}
}

void GLTFImporter::LoadTextureDetails(const tinygltf::Image tinygltfImage, GLTFTextureDetails& TextureLoader)
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

void GLTFImporter::LoadSamplerDetails(const tinygltf::Sampler tinygltfSampler, GLTFSamplerDetails SamplerLoader)
{
	SamplerLoader.magFilter = tinygltfSampler.magFilter;
	SamplerLoader.minFilter = tinygltfSampler.magFilter;
	SamplerLoader.name = tinygltfSampler.magFilter;
	SamplerLoader.wrapU = tinygltfSampler.magFilter;
	SamplerLoader.wrapV = tinygltfSampler.magFilter;
}

void GLTFImporter::LoadMaterial(tinygltf::Model& model)
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
		GLTFMaterialLoader material{};
		material.MaterialName = model.materials[x].name;

		GLTFTextureDetails TextureLoader{};
		GLTFSamplerDetails SamplerLoader{};
		tinygltf::Material glTFMaterial = model.materials[x];

		//if (glTFMaterial.values.find("baseColorFactor") != glTFMaterial.values.end())
		//{
		//	material->Albedo = glm::make_vec4(glTFMaterial.values["baseColorFactor"].ColorFactor().data());
		//}

		//if (glTFMaterial.values.find("metallicFactor") != glTFMaterial.values.end())
		//{
		//	material->Metallic = *reinterpret_cast<float*>(glTFMaterial.values["metallicFactor"].ColorFactor().data());
		//}

		//if (glTFMaterial.values.find("roughnessFactor") != glTFMaterial.values.end())
		//{
		//	material->Roughness = *reinterpret_cast<float*>(glTFMaterial.values["roughnessFactor"].ColorFactor().data());
		//}

		if (glTFMaterial.values.find("baseColorTexture") != glTFMaterial.values.end())
		{
			const uint32_t TextureIndex = tinygltfTexture[glTFMaterial.values["baseColorTexture"].TextureIndex()].source;
			const uint32_t SamplerIndex = tinygltfTexture[glTFMaterial.values["baseColorTexture"].TextureIndex()].sampler;

			LoadTextureDetails(tinygltfImage[TextureIndex], TextureLoader);
			if (tinygltfSampler.size() > 0)
			{
				LoadSamplerDetails(tinygltfSampler[SamplerIndex], SamplerLoader);
			}

			GLTFTextureLoader loader{};
			loader.TextureLoader = TextureLoader;
			loader.SamplerLoader = SamplerLoader;
			loader.Format = VK_FORMAT_R8G8B8A8_SRGB;
			loader.TextureType = TextureTypeEnum::kAlbedoTextureMap;

			material.AlbedoMap = loader;
		}

		if (glTFMaterial.additionalValues.find("occlusionTexture") != glTFMaterial.additionalValues.end())
		{
			const uint32_t TextureIndex = tinygltfTexture[glTFMaterial.additionalValues["occlusionTexture"].TextureIndex()].source;
			const uint32_t SamplerIndex = tinygltfTexture[glTFMaterial.additionalValues["occlusionTexture"].TextureIndex()].sampler;

			LoadTextureDetails(tinygltfImage[TextureIndex], TextureLoader);
			if (tinygltfSampler.size() > 0)
			{
				LoadSamplerDetails(tinygltfSampler[SamplerIndex], SamplerLoader);
			}

			GLTFTextureLoader loader{};
			loader.TextureLoader = TextureLoader;
			loader.SamplerLoader = SamplerLoader;
			loader.Format = VK_FORMAT_R8G8B8A8_UNORM;
			loader.TextureType = TextureTypeEnum::kAmbientOcclusionTextureMap;

			material.AmbientOcclusionMap = loader;
		}

		if (glTFMaterial.additionalValues.find("normalTexture") != glTFMaterial.additionalValues.end())
		{
			const uint32_t TextureIndex = tinygltfTexture[glTFMaterial.additionalValues["normalTexture"].TextureIndex()].source;
			const uint32_t SamplerIndex = tinygltfTexture[glTFMaterial.additionalValues["normalTexture"].TextureIndex()].sampler;

			LoadTextureDetails(tinygltfImage[TextureIndex], TextureLoader);
			if (tinygltfSampler.size() > 0)
			{
				LoadSamplerDetails(tinygltfSampler[SamplerIndex], SamplerLoader);
			}

			GLTFTextureLoader loader{};
			loader.TextureLoader = TextureLoader;
			loader.SamplerLoader = SamplerLoader;
			loader.Format = VK_FORMAT_R8G8B8A8_UNORM;
			loader.TextureType = TextureTypeEnum::kNormalTextureMap;

			material.NormalMap = loader;
		}

		if (glTFMaterial.values.find("metallicRoughnessTexture") != glTFMaterial.values.end())
		{
			const uint32_t TextureIndex = tinygltfTexture[glTFMaterial.values["metallicRoughnessTexture"].TextureIndex()].source;
			const uint32_t SamplerIndex = tinygltfTexture[glTFMaterial.values["metallicRoughnessTexture"].TextureIndex()].sampler;

			LoadTextureDetails(tinygltfImage[TextureIndex], TextureLoader);
			if (tinygltfSampler.size() > 0)
			{
				LoadSamplerDetails(tinygltfSampler[SamplerIndex], SamplerLoader);
			}

			GLTFTextureLoader loader{};
			loader.TextureLoader = TextureLoader;
			loader.SamplerLoader = SamplerLoader;
			loader.Format = VK_FORMAT_R8G8B8A8_UNORM;
			loader.TextureType = TextureTypeEnum::kNormalTextureMap;

			material.MetallicRoughnessMap = loader;
		}

		data.MaterialList.emplace_back(material);
	}
}

void GLTFImporter::LoadMesh(tinygltf::Model& model, tinygltf::Node& node, std::shared_ptr<GLTFNode> parentNode, int index)
{

	std::shared_ptr<GLTFNode> gltfNode = std::make_shared<GLTFNode>();
	gltfNode->NodeName = node.name;
	gltfNode->NodeID = index;
	if (parentNode)
	{
		gltfNode->ParentMesh = parentNode;
	}
	{
		if (node.translation.size() == 3)
		{
			gltfNode->Position = glm::vec3(glm::make_vec3(node.translation.data()));
			gltfNode->NodeTransformMatrix = glm::translate(gltfNode->NodeTransformMatrix, glm::vec3(glm::make_vec3(node.translation.data())));
		}
		if (node.rotation.size() == 4)
		{
			glm::quat quat = glm::make_quat(node.rotation.data());
			gltfNode->NodeTransformMatrix *= glm::mat4(quat);
			gltfNode->Rotation = glm::eulerAngles(quat);
		}
		if (node.scale.size() == 3)
		{
			gltfNode->NodeTransformMatrix = glm::scale(gltfNode->NodeTransformMatrix, glm::vec3(glm::make_vec3(node.scale.data())));
			gltfNode->Scale = glm::vec3(glm::make_vec3(node.scale.data()));
		}
		if (node.matrix.size() == 16)
		{
			gltfNode->NodeTransformMatrix = glm::make_mat4x4(node.matrix.data());
		};
	}

	if (node.children.size() > 0)
	{
		for (size_t x = 0; x < node.children.size(); x++)
		{
			LoadMesh(model, model.nodes[node.children[x]], gltfNode, x);
		}
	}

	if (node.mesh > -1)
	{
		const tinygltf::Mesh mesh = model.meshes[node.mesh];
		for (size_t x = 0; x < mesh.primitives.size(); x++)
		{
			FirstIndex = static_cast<uint32_t>(IndexList.size());
			VertexStartIndex = static_cast<uint32_t>(VertexList.size());
			IndexCount = 0;

			{
				const tinygltf::Primitive& glTFPrimitive = mesh.primitives[x];

				const float* PositionBuffer = nullptr;
				const float* NormalsBuffer = nullptr;
				const float* UVBuffer = nullptr;
				const float* ColorBuffer = nullptr;
				const float* TangantBuffer = nullptr;

				if (glTFPrimitive.attributes.find("POSITION") != glTFPrimitive.attributes.end()) {
					const tinygltf::Accessor& accessor = model.accessors[glTFPrimitive.attributes.find("POSITION")->second];
					const tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
					PositionBuffer = reinterpret_cast<const float*>(&(model.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
					VertexCount = accessor.count;
				}

				if (glTFPrimitive.attributes.find("NORMAL") != glTFPrimitive.attributes.end()) {
					const tinygltf::Accessor& accessor = model.accessors[glTFPrimitive.attributes.find("NORMAL")->second];
					const tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
					NormalsBuffer = reinterpret_cast<const float*>(&(model.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
				}

				if (glTFPrimitive.attributes.find("TEXCOORD_0") != glTFPrimitive.attributes.end()) {
					const tinygltf::Accessor& accessor = model.accessors[glTFPrimitive.attributes.find("TEXCOORD_0")->second];
					const tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
					UVBuffer = reinterpret_cast<const float*>(&(model.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
				}

				if (glTFPrimitive.attributes.find("COLOR_0") != glTFPrimitive.attributes.end()) {
					const tinygltf::Accessor& accessor = model.accessors[glTFPrimitive.attributes.find("COLOR_0")->second];
					const tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
					ColorBuffer = reinterpret_cast<const float*>(&(model.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
				}

				if (glTFPrimitive.attributes.find("TANGENT") != glTFPrimitive.attributes.end()) {
					const tinygltf::Accessor& accessor = model.accessors[glTFPrimitive.attributes.find("TANGENT")->second];
					const tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
					TangantBuffer = reinterpret_cast<const float*>(&(model.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
				}

				for (uint32_t x = 0; x < VertexCount; x++)
				{
					GLTFVertex vertex;
					vertex.Position = glm::vec4(glm::make_vec3(&PositionBuffer[x * 3]), 1.0f);
					vertex.Normal = glm::normalize(glm::vec3(NormalsBuffer ? glm::make_vec3(&NormalsBuffer[x * 3]) : glm::vec3(0.0f)));
					vertex.UV = UVBuffer ? glm::make_vec2(&UVBuffer[x * 2]) : glm::vec3(0.0f);
					vertex.Color = ColorBuffer ? glm::make_vec4(&ColorBuffer[x * 4]) : glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
					vertex.Tangant = TangantBuffer ? glm::make_vec4(&TangantBuffer[x * 4]) : glm::vec4(0.0f);
					vertex.BiTangant = cross(vertex.Normal, glm::vec3(vertex.Tangant.x, vertex.Tangant.y, vertex.Tangant.z)) * vertex.Tangant.w;

					VertexList.emplace_back(vertex);
				}
			}

			{
				const tinygltf::Primitive& glTFPrimitive = mesh.primitives[x];
				const tinygltf::Accessor& accessor = model.accessors[glTFPrimitive.indices];
				const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
				const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

				IndexCount += static_cast<uint32_t>(accessor.count);

				switch (accessor.componentType)
				{
				case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT:
				{
					const uint32_t* indexBuffer = reinterpret_cast<const uint32_t*>(&buffer.data[accessor.byteOffset + bufferView.byteOffset]);
					for (uint32_t x = 0; x < accessor.count; x++)
					{
						IndexList.emplace_back(indexBuffer[x] + VertexStartIndex);
					}
					break;
				}
				case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT:
				{
					const uint16_t* indexBuffer = reinterpret_cast<const uint16_t*>(&buffer.data[accessor.byteOffset + bufferView.byteOffset]);
					for (uint32_t x = 0; x < accessor.count; x++)
					{
						IndexList.emplace_back(indexBuffer[x] + VertexStartIndex);
					}
					break;
				}
				case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE:
				{
					const uint8_t* indexBuffer = reinterpret_cast<const uint8_t*>(&buffer.data[accessor.byteOffset + bufferView.byteOffset]);
					for (uint32_t x = 0; x < accessor.count; x++)
					{
						IndexList.emplace_back(indexBuffer[x] + VertexStartIndex);
					}
					break;
				}
				default:
				{
					std::cout << "Index component type " << accessor.componentType << " not supported!" << std::endl;
					return;
				}
				}
			}
			{
				if (data.MaterialList.size() > 1)
				{
					const tinygltf::Mesh mesh = model.meshes[node.mesh];
					const tinygltf::Primitive& glTFPrimitive = mesh.primitives[x];
					gltfNode->Material = data.MaterialList[glTFPrimitive.material];
				}
			}
			{
				const tinygltf::Primitive& glTFPrimitive = mesh.primitives[x];

				GLTFPrimitive primitive;
				primitive.FirstIndex = FirstIndex;
				primitive.IndexCount = IndexCount;
				primitive.material = glTFPrimitive.material;
				gltfNode->PrimitiveList.emplace_back(primitive);
			}
		}
	}

	if (parentNode)
	{
		parentNode->ChildMeshList.push_back(gltfNode);
	}
	else
	{
		NodeList.push_back(gltfNode);
	}
}