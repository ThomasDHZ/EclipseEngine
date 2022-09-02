#pragma once
#include "Mesh.h"
#include "MeshRendererManager.h"

class InstanceMesh3D : public Mesh
{
private:
	std::vector<Vertex3D> VertexList;
	std::vector<uint32_t> IndexList;
	std::vector<InstancingDataStruct> InstanceDataVectors;
	std::vector<InstancedData3D> InstancedDataList;

	VulkanBuffer TransformBuffer;
	VulkanBuffer TransformInverseBuffer;

protected:
	VkAccelerationStructureGeometryKHR AccelerationStructureGeometry{};
	VkAccelerationStructureBuildRangeInfoKHR AccelerationStructureBuildRangeInfo{};

	void MeshStartUp(std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, InstancingDataStruct& instanceData, uint64_t parentGameObjectID);
	void MeshStartUp(std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, InstancingDataStruct& instanceData, std::shared_ptr<Material> materialPtr, uint64_t parentGameObjectID);

	void InstancingStartUp(InstancingDataStruct& instanceData);
	void RTXMeshStartUp();
	void UpdateMeshBottomLevelAccelerationStructure();

public:
	InstanceMesh3D();
	InstanceMesh3D(std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, InstancingDataStruct& instanceData, MeshSubTypeEnum meshSubType, uint64_t parentGameObjectID);
	InstanceMesh3D(std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, InstancingDataStruct& instanceData, std::shared_ptr<Material> materialPtr, MeshSubTypeEnum meshSubType, uint64_t parentGameObjectID);
	~InstanceMesh3D();

	virtual void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix) override;
	virtual void Destroy() override;
};

