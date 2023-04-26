//#pragma once
//#include "GameObjectManager.h"
//#include "GameObject3D.h"
//#include "Mesh3D.h"
//
//class MeshRenderer : public GameObject3D
//{
//private:
//	std::shared_ptr<Mesh> mesh;
//
//public:
//	MeshRenderer();
//	MeshRenderer(const std::string Name, MeshLoader3D& meshLoader);
//	MeshRenderer(const std::string Name, MeshLoader3D& meshLoader, const glm::vec3& position);
//	MeshRenderer(const std::string Name, MeshLoader3D& meshLoader, const glm::vec3& position, const glm::vec3& rotation);
//	MeshRenderer(const std::string Name, MeshLoader3D& meshLoader, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
//	~MeshRenderer();
//
//	void Update(float DeltaTime) override;
//	void Destroy() override;
//
//	std::shared_ptr<Mesh> GetMesh() { return mesh; }
//};
//
