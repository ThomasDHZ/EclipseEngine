#pragma once
#include "GLTFFileLoader.h"
#include "GLTFMesh.h"

class GLTFModel
{
private:
	GLTFVertexEnum VertexType;
	tinygltf::Model model;

	std::vector<GLTFMesh> MeshList;
	glm::mat4 ModelMatrix;
public: 
	GLTFModel()
	{
	}

	GLTFModel(const char* filename, GLTFVertexEnum vertexType)
	{
		VertexType = vertexType;

		auto file = "C:/Users/dotha/Desktop/Vulkan-master/Vulkan/data/models/FlightHelmet/glTF/FlightHelmet.gltf";
		model = GLTFFileLoader::Loader(file);

		const tinygltf::Scene& scene = model.scenes[model.defaultScene];
		const tinygltf::Node node = model.nodes[scene.nodes[0]];
		for (int x = 0; x < scene.nodes.size(); x++)
		{
			LoadModelNodes(model.nodes[scene.nodes[x]]);
		}
	}

	~GLTFModel()
	{
	}

	void LoadModelNodes(tinygltf::Node& node)
	{
		if (node.mesh >= 0 &&
			node.mesh < model.meshes.size())
		{
			GLTFMesh mesh = GLTFMesh(VertexType);
			mesh.LoadMesh(model, model.meshes[node.mesh]);
			MeshList.emplace_back(mesh);
		}

		for (uint32_t x = 0; x < node.children.size(); x++)
		{
			LoadModelNodes(model.nodes[node.children[x]]);
		}
	}
};

