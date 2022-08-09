//#pragma once
//#include "GameObjectManager.h"
//#include "GameObject3D.h"
//#include "Mesh3D.h"
//
//class BillBoardMeshRenderer : public GameObject3D
//{
//private:
//	std::shared_ptr<Mesh> mesh;
//
//public:
//	BillBoardMeshRenderer();
//	BillBoardMeshRenderer(const std::string Name, glm::vec2 BillBoardSize);
//	BillBoardMeshRenderer(const std::string Name, glm::vec2 BillBoardSize, glm::vec3 position);
//	BillBoardMeshRenderer(const std::string Name, glm::vec2 BillBoardSize, glm::vec3 position, glm::vec3 scale);
//
//	BillBoardMeshRenderer(const std::string Name, glm::vec2 BillBoardSize, std::shared_ptr<Material> material);
//	BillBoardMeshRenderer(const std::string Name, glm::vec2 BillBoardSize, std::shared_ptr<Material> material, glm::vec3 position);
//	BillBoardMeshRenderer(const std::string Name, glm::vec2 BillBoardSize, std::shared_ptr<Material> material, glm::vec3 position, glm::vec3 scale);
//	~BillBoardMeshRenderer();
//
//	void Update(float DeltaTime) override;
//	void Destroy() override;
//
//	std::shared_ptr<Mesh> GetMesh() { return mesh; }
//};
//
