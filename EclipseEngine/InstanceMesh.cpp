#include "InstanceMesh.h"
#include <glm/ext/matrix_transform.hpp>

InstanceMesh::InstanceMesh()
{
	MeshID = 0;
	MeshBufferIndex = 0;
	InstanceMeshTransform = glm::mat4(1.0f);

	InstancePosition = glm::vec3(0.0f);
	InstanceRotation = glm::vec3(0.0f);
	InstanceScale = glm::vec3(1.0f);

	meshProperties.VertexBufferIndex = 0;
	meshProperties.IndiceBufferIndex = 0;
	meshProperties.MaterialBufferIndex = 0;
}

InstanceMesh::InstanceMesh(uint64_t InstMeshID)
{
	MeshID = 0;
	MeshBufferIndex = 0;
	InstanceMeshTransform = glm::mat4(1.0f);

	InstancePosition = glm::vec3(0.0f);
	InstanceRotation = glm::vec3(0.0f);
	InstanceScale = glm::vec3(1.0f);

	meshProperties.VertexBufferIndex = 0;
	meshProperties.IndiceBufferIndex = 0;
	meshProperties.MaterialBufferIndex = 0;
}

InstanceMesh::~InstanceMesh()
{
}

//void InstanceMesh::UpdateMeshTopLevelAccelerationStructure(const glm::mat4& GameObjectMatrix, std::vector<VkAccelerationStructureInstanceKHR>& AccelerationStructureInstanceList)
//{
//
//	for (auto& mesh : GetMeshList())
//	{
//
//		if (mesh->GetMeshType() != MeshTypeEnum::kPolygonInstanced)
//		{
//			glm::mat4 GLMTransformMatrix2 = glm::mat4(1.0f);
//			VkTransformMatrixKHR transformMatrix = EngineMath::GLMToVkTransformMatrix(GLMTransformMatrix2);
//
//			VkAccelerationStructureInstanceKHR AccelerationStructureInstance{};
//			AccelerationStructureInstance.transform = transformMatrix;
//			AccelerationStructureInstance.instanceCustomIndex = mesh->GetMeshBufferIndex();
//			AccelerationStructureInstance.mask = 0xFF;
//			AccelerationStructureInstance.instanceShaderBindingTableRecordOffset = 0;
//			AccelerationStructureInstance.flags = VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR;
//			AccelerationStructureInstance.accelerationStructureReference = mesh->GetBLASBufferDeviceAddress();
//			AccelerationStructureInstanceList.emplace_back(AccelerationStructureInstance);
//		}
//		else
//		{
//			const Mesh3D* instanceMeshList = static_cast<Mesh3D*>(mesh.get());
//			for (auto& instance : instanceMeshList->RTXInstanceMeshList)
//			{
//				auto instanceTransform = instance.InstanceMeshTransform;
//				VkTransformMatrixKHR transformMatrix = EngineMath::GLMToVkTransformMatrix(instanceTransform);
//
//				VkAccelerationStructureInstanceKHR AccelerationStructureInstance{};
//				AccelerationStructureInstance.transform = transformMatrix;
//				AccelerationStructureInstance.instanceCustomIndex = mesh->GetMeshBufferIndex();
//				AccelerationStructureInstance.mask = 0xFF;
//				AccelerationStructureInstance.instanceShaderBindingTableRecordOffset = 0;
//				AccelerationStructureInstance.flags = VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR;
//				AccelerationStructureInstance.accelerationStructureReference = mesh->GetBLASBufferDeviceAddress();
//				AccelerationStructureInstanceList.emplace_back(AccelerationStructureInstance);
//			}
//		}
//	}
//}

void InstanceMesh::Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix)
{
	glm::mat4 TransformMatrix = glm::mat4(1.0f);
	TransformMatrix = glm::translate(TransformMatrix, InstancePosition);
	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(InstanceRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(InstanceRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(InstanceRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	TransformMatrix = glm::scale(TransformMatrix, InstanceScale);

	meshProperties.MeshTransform = GameObjectMatrix * ModelMatrix * TransformMatrix;
	meshProperties.MaterialBufferIndex = material->GetMaterialBufferIndex();
}

void InstanceMesh::GetMeshPropertiesBuffer(std::vector<VkDescriptorBufferInfo>& MeshPropertiesBufferList)
{
	VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
	MeshPropertiesmBufferBufferInfo.buffer = MeshPropertiesBuffer.GetVulkanBufferData().Buffer;
	MeshPropertiesmBufferBufferInfo.offset = 0;
	MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
	MeshPropertiesBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);

	MeshBufferIndex = MeshPropertiesBufferList.size() - 1;
}

void InstanceMesh::GetMeshVertexBuffer(std::vector<VkDescriptorBufferInfo>& VertexBufferList, VkBuffer VertexBuffer)
{
	VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
	MeshPropertiesmBufferBufferInfo.buffer = VertexBuffer;
	MeshPropertiesmBufferBufferInfo.offset = 0;
	MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
	VertexBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);

	MeshBufferIndex = VertexBufferList.size() - 1;
}

void InstanceMesh::GetMeshIndexBuffer(std::vector<VkDescriptorBufferInfo>& IndexBufferList, VkBuffer IndexBuffer)
{
	VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
	MeshPropertiesmBufferBufferInfo.buffer = IndexBuffer;
	MeshPropertiesmBufferBufferInfo.offset = 0;
	MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
	IndexBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);

	MeshBufferIndex = IndexBufferList.size() - 1;
}

void InstanceMesh::Destroy()
{
	MeshPropertiesBuffer.Destroy();
}
