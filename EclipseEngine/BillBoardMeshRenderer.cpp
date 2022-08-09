//#include "BillBoardMeshRenderer.h"
//
//BillBoardMeshRenderer::BillBoardMeshRenderer()
//{
//}
//
//BillBoardMeshRenderer::BillBoardMeshRenderer(const std::string Name, glm::vec2 BillBoardSize) : GameObject3D(Name)
//{
//	std::vector<Vertex3D> VertexList =
//	{
//		{{0.0f,            0.0f, 0.0f},            { 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}},
//		{{BillBoardSize.x, 0.0f, 0.0f},            { 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f}, {0.0f, 0.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f}},
//		{{BillBoardSize.x, BillBoardSize.y, 0.0f}, { 0.0f}, {0.0f, 0.0f, 1.0f},{ 0.0f}, {0.0f, 1.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {0.0f, 0.0f, 1.0f},{ 0.0f}},
//		{{0.0f,            BillBoardSize.y, 0.0f}, { 0.0f}, {1.0f, 1.0f, 1.0f},{ 0.0f}, {1.0f, 1.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 1.0f, 0.0f},{ 0.0f}}
//	};
//
//	std::vector<uint32_t> IndexList = {
//	   0, 1, 2, 2, 3, 0
//	};
//
//	mesh = std::make_shared<Mesh3D>(Mesh3D(VertexList, IndexList, MeshSubTypeEnum::kBillboard, GameObjectID));
//	GameObjectManager::AddGameObject(std::make_shared<BillBoardMeshRenderer>(*this));
//}
//
//BillBoardMeshRenderer::BillBoardMeshRenderer(const std::string Name, glm::vec2 BillBoardSize, glm::vec3 position) : GameObject3D(Name, position)
//{
//	std::vector<Vertex3D> VertexList =
//	{
//		{{0.0f,            0.0f, 0.0f},            { 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}},
//		{{BillBoardSize.x, 0.0f, 0.0f},            { 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f}, {0.0f, 0.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f}},
//		{{BillBoardSize.x, BillBoardSize.y, 0.0f}, { 0.0f}, {0.0f, 0.0f, 1.0f},{ 0.0f}, {0.0f, 1.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {0.0f, 0.0f, 1.0f},{ 0.0f}},
//		{{0.0f,            BillBoardSize.y, 0.0f}, { 0.0f}, {1.0f, 1.0f, 1.0f},{ 0.0f}, {1.0f, 1.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 1.0f, 0.0f},{ 0.0f}}
//	};
//
//	std::vector<uint32_t> IndexList = {
//	   0, 1, 2, 2, 3, 0
//	};
//
//	mesh = std::make_shared<Mesh3D>(Mesh3D(VertexList, IndexList, MeshSubTypeEnum::kBillboard, GameObjectID));
//	GameObjectManager::AddGameObject(std::make_shared<BillBoardMeshRenderer>(*this));
//}
//
//BillBoardMeshRenderer::BillBoardMeshRenderer(const std::string Name, glm::vec2 BillBoardSize, glm::vec3 position, glm::vec3 scale) : GameObject3D(Name, position, glm::vec3(0.0f), scale)
//{
//	std::vector<Vertex3D> VertexList =
//	{
//		{{0.0f,            0.0f, 0.0f},            { 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}},
//		{{BillBoardSize.x, 0.0f, 0.0f},            { 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f}, {0.0f, 0.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f}},
//		{{BillBoardSize.x, BillBoardSize.y, 0.0f}, { 0.0f}, {0.0f, 0.0f, 1.0f},{ 0.0f}, {0.0f, 1.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {0.0f, 0.0f, 1.0f},{ 0.0f}},
//		{{0.0f,            BillBoardSize.y, 0.0f}, { 0.0f}, {1.0f, 1.0f, 1.0f},{ 0.0f}, {1.0f, 1.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 1.0f, 0.0f},{ 0.0f}}
//	};
//
//	std::vector<uint32_t> IndexList = {
//	   0, 1, 2, 2, 3, 0
//	};
//
//	mesh = std::make_shared<Mesh3D>(Mesh3D(VertexList, IndexList, MeshSubTypeEnum::kBillboard, GameObjectID));
//	GameObjectManager::AddGameObject(std::make_shared<BillBoardMeshRenderer>(*this));
//}
//
//BillBoardMeshRenderer::BillBoardMeshRenderer(const std::string Name, glm::vec2 BillBoardSize, std::shared_ptr<Material> material) : GameObject3D(Name)
//{
//	std::vector<Vertex3D> VertexList =
//	{
//		{{0.0f,            0.0f, 0.0f},            { 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}},
//		{{BillBoardSize.x, 0.0f, 0.0f},            { 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f}, {0.0f, 0.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f}},
//		{{BillBoardSize.x, BillBoardSize.y, 0.0f}, { 0.0f}, {0.0f, 0.0f, 1.0f},{ 0.0f}, {0.0f, 1.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {0.0f, 0.0f, 1.0f},{ 0.0f}},
//		{{0.0f,            BillBoardSize.y, 0.0f}, { 0.0f}, {1.0f, 1.0f, 1.0f},{ 0.0f}, {1.0f, 1.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 1.0f, 0.0f},{ 0.0f}}
//	};
//
//	std::vector<uint32_t> IndexList = {
//	   0, 1, 2, 2, 3, 0
//	};
//
//	mesh = std::make_shared<Mesh3D>(Mesh3D(VertexList, IndexList, material, MeshSubTypeEnum::kBillboard, GameObjectID));
//	GameObjectManager::AddGameObject(std::make_shared<BillBoardMeshRenderer>(*this));
//}
//
//BillBoardMeshRenderer::BillBoardMeshRenderer(const std::string Name, glm::vec2 BillBoardSize, std::shared_ptr<Material> material, glm::vec3 position) : GameObject3D(Name, position)
//{
//	std::vector<Vertex3D> VertexList =
//	{
//		{{0.0f,            0.0f, 0.0f},            { 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}},
//		{{BillBoardSize.x, 0.0f, 0.0f},            { 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f}, {0.0f, 0.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f}},
//		{{BillBoardSize.x, BillBoardSize.y, 0.0f}, { 0.0f}, {0.0f, 0.0f, 1.0f},{ 0.0f}, {0.0f, 1.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {0.0f, 0.0f, 1.0f},{ 0.0f}},
//		{{0.0f,            BillBoardSize.y, 0.0f}, { 0.0f}, {1.0f, 1.0f, 1.0f},{ 0.0f}, {1.0f, 1.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 1.0f, 0.0f},{ 0.0f}}
//	};
//
//	std::vector<uint32_t> IndexList = {
//	   0, 1, 2, 2, 3, 0
//	};
//
//	mesh = std::make_shared<Mesh3D>(Mesh3D(VertexList, IndexList, material, MeshSubTypeEnum::kBillboard, GameObjectID));
//	GameObjectManager::AddGameObject(std::make_shared<BillBoardMeshRenderer>(*this));
//}
//
//BillBoardMeshRenderer::BillBoardMeshRenderer(const std::string Name, glm::vec2 BillBoardSize, std::shared_ptr<Material> material, glm::vec3 position, glm::vec3 scale) : GameObject3D(Name, position, glm::vec3(0.0f), scale)
//{
//	std::vector<Vertex3D> VertexList =
//	{
//		{{0.0f,            0.0f, 0.0f},            { 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}},
//		{{BillBoardSize.x, 0.0f, 0.0f},            { 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f}, {0.0f, 0.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f}},
//		{{BillBoardSize.x, BillBoardSize.y, 0.0f}, { 0.0f}, {0.0f, 0.0f, 1.0f},{ 0.0f}, {0.0f, 1.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {0.0f, 0.0f, 1.0f},{ 0.0f}},
//		{{0.0f,            BillBoardSize.y, 0.0f}, { 0.0f}, {1.0f, 1.0f, 1.0f},{ 0.0f}, {1.0f, 1.0f},{  0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 0.0f, 0.0f},{ 0.0f}, {1.0f, 1.0f, 0.0f},{ 0.0f}}
//	};
//
//	std::vector<uint32_t> IndexList = {
//	   0, 1, 2, 2, 3, 0
//	};
//
//	mesh = std::make_shared<Mesh3D>(Mesh3D(VertexList, IndexList, material, MeshSubTypeEnum::kBillboard, GameObjectID));
//	GameObjectManager::AddGameObject(std::make_shared<BillBoardMeshRenderer>(*this));
//}
//
//BillBoardMeshRenderer::~BillBoardMeshRenderer()
//{
//}
//
//void BillBoardMeshRenderer::Update(float DeltaTime)
//{
//	GameObject::Update(DeltaTime);
//	mesh->Update(GameObjectTransform, glm::mat4(1.0f));
//}
//
//void BillBoardMeshRenderer::Destroy()
//{
//	mesh->Destroy();
//}