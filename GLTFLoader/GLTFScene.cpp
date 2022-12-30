#include "GLTFScene.h"

GLTFModel::GLTFModel(const char* filename, uint32_t vertexSize)
{
	VertexSize = vertexSize;

	auto file = "C:/Users/dotha/Desktop/Vulkan-master/Vulkan/data/models/FlightHelmet/glTF/FlightHelmet.gltf";
	tinygltf::TinyGLTF loader;
	std::string error;
	std::string warning;

	loader.LoadASCIIFromFile(&model, &error, &warning, file);

	if (!error.empty())
	{
		std::cout << "Error: " << error << std::endl;
	}

	if (!warning.empty())
	{
		std::cout << "Warning: " << warning << std::endl;
	}

	const tinygltf::Scene& scene = model.scenes[model.defaultScene];
	const tinygltf::Node node = model.nodes[scene.nodes[0]];
	for (int x = 0; x < scene.nodes.size(); x++)
	{
		ModelNodes(model.nodes[scene.nodes[x]]);
	}
}

GLTFModel::~GLTFModel()
{
}

void GLTFModel::ModelNodes(tinygltf::Node& node)
{
	if (node.mesh >= 0 &&
		node.mesh < model.meshes.size())
	{
		ModelMesh(model.meshes[node.mesh]);
	}

	for (uint32_t x = 0; x < node.children.size(); x++)
	{
		ModelNodes(model.nodes[node.children[x]]);
	}
}

void GLTFModel::ModelMesh(tinygltf::Mesh& mesh)
{
	for (size_t x = 0; x < mesh.primitives.size(); x++) 
	{
		const tinygltf::BufferView& bufferView = model.bufferViews[x];
		const tinygltf::Primitive& glTFPrimitive = mesh.primitives[x];
		if (bufferView.target == 0)
		{
			continue;
		}
		
		const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
	}
}
