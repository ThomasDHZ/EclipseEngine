#pragma once
#include "GameObject.h"
class MeshRendererManager
{
private:
	std::vector<std::shared_ptr<Model>> ModelList;
	std::vector<std::shared_ptr<Mesh>> MeshList;

public:
	static void Update(float DeltaTime)
	{
		//for (auto& comp : ComponentList)
		//{
		//	comp->Update(DeltaTime);
		//}

		//auto objRenderer = GetComponentBySubType(ComponentSubType::kRenderedObject);
		//if (objRenderer != nullptr)
		//{
		//	ComponentRenderer* objRendererPtr = static_cast<ComponentRenderer*>(objRenderer.get());
		//	for (auto& mesh : objRendererPtr->GetModel()->GetMeshList())
		//	{
		//		objRendererPtr->UpdateMeshProperties();
		//	}
		//}
	}

	//void GetGameObjectPropertiesBuffer(std::vector<VkDescriptorBufferInfo>& MeshPropertiesBufferList)
	//{

	//	auto objRenderer = GetComponentBySubType(ComponentSubType::kRenderedObject);
	//	if (objRenderer != nullptr)
	//	{
	//		ComponentRenderer* objRendererPtr = static_cast<ComponentRenderer*>(objRenderer.get());
	//		for (auto& mesh : objRendererPtr->GetModel()->GetMeshList())
	//		{
	//			MeshProperties meshProps = {};
	//			objRendererPtr->UpdateMeshProperties();
	//			VkBuffer buffer = mesh->GetMeshPropertiesBuffer();

	//			VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
	//			MeshPropertiesmBufferBufferInfo.buffer = buffer;
	//			MeshPropertiesmBufferBufferInfo.offset = 0;
	//			MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
	//			MeshPropertiesBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);

	//			mesh->SetBufferIndex(MeshPropertiesBufferList.size() - 1);
	//		}
	//	}
	//}

	//void GetMeshIndexBuffer(std::vector<VkDescriptorBufferInfo>& IndexBufferList)
	//{
	//	auto objRenderer = GetComponentBySubType(ComponentSubType::kRenderedObject);
	//	if (objRenderer != nullptr)
	//	{
	//		ComponentRenderer* objRendererPtr = static_cast<ComponentRenderer*>(objRenderer.get());
	//		for (auto& mesh : objRendererPtr->GetModel()->GetMeshList())
	//		{
	//			MeshProperties meshProps = {};
	//			objRendererPtr->UpdateMeshProperties();
	//			VkBuffer buffer = mesh->GetMeshPropertiesBuffer();

	//			VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
	//			MeshPropertiesmBufferBufferInfo.buffer = buffer;
	//			MeshPropertiesmBufferBufferInfo.offset = 0;
	//			MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
	//			IndexBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);

	//			mesh->SetBufferIndex(IndexBufferList.size() - 1);
	//		}
	//	}
	//}

	//void GetMeshVertexBuffer(std::vector<VkDescriptorBufferInfo>& VertexBufferList)
	//{
	//	auto objRenderer = GetComponentBySubType(ComponentSubType::kRenderedObject);
	//	if (objRenderer != nullptr)
	//	{
	//		ComponentRenderer* objRendererPtr = static_cast<ComponentRenderer*>(objRenderer.get());
	//		for (auto& mesh : objRendererPtr->GetModel()->GetMeshList())
	//		{
	//			MeshProperties meshProps = {};
	//			objRendererPtr->UpdateMeshProperties();
	//			VkBuffer buffer = mesh->GetMeshPropertiesBuffer();

	//			VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
	//			MeshPropertiesmBufferBufferInfo.buffer = buffer;
	//			MeshPropertiesmBufferBufferInfo.offset = 0;
	//			MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
	//			VertexBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);

	//			mesh->SetBufferIndex(VertexBufferList.size() - 1);
	//		}
	//	}
	//}
};

