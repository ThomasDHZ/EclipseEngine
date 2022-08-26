#include "Mesh.h"
#include "Math.h"

uint64_t Mesh::MeshIDCounter = 0;

Mesh::Mesh()
{
}

Mesh::Mesh(MeshTypeEnum meshType, MeshSubTypeEnum meshSubType, uint64_t parentGameObjectID)
{
	MeshType = meshType;
	MeshSubType = meshSubType;
	ParentGameObjectID = parentGameObjectID;
}

Mesh::~Mesh()
{
}

void Mesh::GenerateID()
{
	MeshIDCounter++;
	MeshID = MeshIDCounter;
}

void Mesh::GenerateColorID()
{
	byte red = (255.0f - (float)MeshID);
	byte green = (255.0f - (float)MeshID);
	byte blue = (255.0f - (float)MeshID);

	MeshColorID = Pixel(red, green, blue, 0xFF);
}

void Mesh::Draw(VkCommandBuffer& commandBuffer)
{
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, VertexBuffer.GetBufferPtr(), offsets);
	if (IndexCount == 0)
	{
		vkCmdDraw(commandBuffer, VertexCount, 1, 0, 0);
	}
	else
	{
		vkCmdBindIndexBuffer(commandBuffer, IndexBuffer.GetBuffer(), 0, VK_INDEX_TYPE_UINT32);
		vkCmdDrawIndexed(commandBuffer, IndexCount, 1, 0, 0, 0);
	}
}

void Mesh::Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix)
{
}

void Mesh::Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix, const std::vector<std::shared_ptr<Bone>>& BoneList)
{
}

void Mesh::Destroy()
{
	VertexBuffer.DestoryBuffer();
	MeshPropertiesBuffer.Destroy();

	if (IndexBuffer.Buffer != nullptr)
	{
		IndexBuffer.DestoryBuffer();
	}
	if (BottomLevelAccelerationBuffer.GetAccelerationStructureHandle() != VK_NULL_HANDLE)
	{
		BottomLevelAccelerationBuffer.Destroy();
	}
}

void Mesh::SetParentModel(uint64_t ModelID)
{
	ParentModelID = ModelID;
}
void Mesh::SetSelectedMesh(bool selected)
{
	SelectedMesh = selected;
}
void Mesh::SetParentGameObjectID(uint64_t GameObjectID)
{
	ParentGameObjectID = GameObjectID;
}

void Mesh::SetBufferIndex(int bufferIndex)
{

	if (VulkanRenderer::UpdateRendererFlag)
	{
		BufferIndex = bufferIndex;
	}
	else
	{
		std::cout << "Can't update BufferIndex unless pipelines in the process of being rebuilt." << std::endl;
	}
}

void Mesh::SetMaterial(std::shared_ptr<Material> materialPtr)
{
	material = materialPtr;
}

void Mesh::SetMaterial(const std::string& MaterialPath)
{
	material = MaterialManager::LoadMaterial(MaterialPath);
}

void Mesh::SetReflectionPoint(glm::vec3 reflectionPoint)
{
	ReflectionPoint = reflectionPoint;
}

void  Mesh::GetMeshPropertiesBuffer(std::vector<VkDescriptorBufferInfo>& MeshPropertiesBufferList)
{
	VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
	MeshPropertiesmBufferBufferInfo.buffer = MeshPropertiesBuffer.GetVulkanBufferData().Buffer;
	MeshPropertiesmBufferBufferInfo.offset = 0;
	MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
	MeshPropertiesBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);

	BufferIndex = MeshPropertiesBufferList.size() - 1;
}

void Mesh::GetMeshVertexBuffer(std::vector<VkDescriptorBufferInfo>& VertexBufferList)
{
	VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
	MeshPropertiesmBufferBufferInfo.buffer = VertexBuffer.Buffer;
	MeshPropertiesmBufferBufferInfo.offset = 0;
	MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
	VertexBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);

	BufferIndex = VertexBufferList.size() - 1;
}

void Mesh::GetMeshIndexBuffer(std::vector<VkDescriptorBufferInfo>& IndexBufferList)
{
	VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
	MeshPropertiesmBufferBufferInfo.buffer = IndexBuffer.Buffer;
	MeshPropertiesmBufferBufferInfo.offset = 0;
	MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
	IndexBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);

	BufferIndex = IndexBufferList.size() - 1;
}