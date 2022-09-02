#pragma once
#include "GameObjectManager.h"
#include "GameObject3D.h"
#include "Mesh3D.h"

class MeshRenderer : public GameObject3D
{
private:
	std::shared_ptr<Mesh> mesh;

public:
	MeshRenderer();
	MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList);
	MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, glm::vec3 position);
	MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, glm::vec3 position, glm::vec3 rotation);
	MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, std::shared_ptr<Material> material);
	MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, std::shared_ptr<Material> material, glm::vec3 position);
	MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, std::shared_ptr<Material> material, glm::vec3 position, glm::vec3 rotation);
	MeshRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, std::shared_ptr<Material> material, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	MeshRenderer(const std::string Name, std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, InstancingDataStruct& instanceData);
	MeshRenderer(const std::string Name, std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, InstancingDataStruct& instanceData, glm::vec3 position);
	MeshRenderer(const std::string Name, std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, InstancingDataStruct& instanceData, glm::vec3 position, glm::vec3 rotation);
	MeshRenderer(const std::string Name, std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, InstancingDataStruct& instanceData, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	MeshRenderer(const std::string Name, std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, InstancingDataStruct& instanceData, std::shared_ptr<Material> materialPtr);
	MeshRenderer(const std::string Name, std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, InstancingDataStruct& instanceData, std::shared_ptr<Material> materialPtr, glm::vec3 position);
	MeshRenderer(const std::string Name, std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, InstancingDataStruct& instanceData, std::shared_ptr<Material> materialPtr, glm::vec3 position, glm::vec3 rotation);
	MeshRenderer(const std::string Name, std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, InstancingDataStruct& instanceData, std::shared_ptr<Material> materialPtr, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	~MeshRenderer();

	void Update(float DeltaTime) override;
	void Destroy() override;

	std::shared_ptr<Mesh> GetMesh() { return mesh; }
};

