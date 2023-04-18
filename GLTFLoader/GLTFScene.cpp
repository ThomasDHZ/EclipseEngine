//#include "GLTFScene.h"
//#include <glm/glm/gtc/type_ptr.inl>
//#include <glm/gtc/quaternion.hpp>
//
//GLTFScene::GLTFScene(const char* filename)
//{
//	auto model = GLTFFileLoader::Loader(filename);
//	LoadModel(model);
//}
//
//GLTFScene::~GLTFScene()
//{
//}
//
//void GLTFScene::LoadModel(tinygltf::Model model)
//{
//	const tinygltf::Scene& scene = model.scenes[model.defaultScene];
//
//	LoadMaterial(model);
//	for (int x = 0; x < scene.nodes.size(); x++)
//	{
//		tinygltf::Node node = model.nodes[scene.nodes[x]];
//		LoadLights(model, node);
//		LoadMesh(model, node, nullptr, x);
//	}
//
//	data.VertexList = VertexList;
//	data.IndexList = IndexList;
//	data.NodeList = NodeList;
//}