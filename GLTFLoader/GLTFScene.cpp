#include "GLTFScene.h"

GLTFScene::GLTFScene(const char* filename, uint32_t vertexSize)
{
	//VertexSize = vertexSize;

	//auto file = "C:/Users/dotha/Desktop/Vulkan-master/Vulkan/data/models/FlightHelmet/glTF/FlightHelmet.gltf";
	//model = GLTFFileLoader::Loader(file);

	//const tinygltf::Scene& scene = model.scenes[model.defaultScene];
	//const tinygltf::Node node = model.nodes[scene.nodes[0]];
	//for (int x = 0; x < scene.nodes.size(); x++)
	//{
	//	ModelNodes(model.nodes[scene.nodes[x]]);
	//}
}

GLTFScene::~GLTFScene()
{
}

void GLTFScene::ModelNodes(tinygltf::Node& node)
{
	//if (node.mesh >= 0 &&
	//	node.mesh < model.meshes.size())
	//{
	//	LoadMesh(model.meshes[node.mesh]);
	//}

	//for (uint32_t x = 0; x < node.children.size(); x++)
	//{
	//	ModelNodes(model.nodes[node.children[x]]);
	//}
}

void GLTFScene::LoadMesh(tinygltf::Mesh& mesh)
{
	//for (size_t x = 0; x < mesh.primitives.size(); x++) 
	//{
	//	const tinygltf::BufferView& bufferView = model.bufferViews[x];
	//	const tinygltf::Primitive& glTFPrimitive = mesh.primitives[x];
	//	if (bufferView.target == 0)
	//	{
	//		continue;
	//	}
	//	
	//	const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
	//}
}
