#pragma once
#include "GameObject.h"
class MeshRendererManager
{
private:
	static std::vector<std::shared_ptr<Model>> ModelList;
	static std::vector<std::shared_ptr<Mesh>> MeshList;

public:

	static std::shared_ptr<Mesh> AddMesh(glm::vec3& StartPoint, glm::vec3& EndPoint)
	{
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(StartPoint, EndPoint));
		MeshList.emplace_back(mesh);

		return mesh;
	}

	static std::shared_ptr<Mesh> AddMesh(std::vector<LineVertex>& vertices)
	{
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(vertices));
		MeshList.emplace_back(mesh);

		return mesh;
	}

	static std::shared_ptr<Mesh> AddMesh(std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices)
	{
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(vertices, indices));
		MeshList.emplace_back(mesh);

		return mesh;
	}

	static std::shared_ptr<Mesh> AddMesh(std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices, std::shared_ptr<Material> materialPtr)
	{
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(vertices, indices, materialPtr));
		MeshList.emplace_back(mesh);

		return mesh;
	}

	static std::shared_ptr<Mesh> AddMesh(MeshLoadingInfo meshLoader)
	{
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(meshLoader));
		MeshList.emplace_back(mesh);

		return mesh;
	}

	static void Update(float DeltaTime)
	{
		for (auto& mesh : MeshList)
		{
			mesh->UpdateMeshProperties();
		}
	}

	static void GetMeshPropertiesBuffer(std::vector<VkDescriptorBufferInfo>& MeshPropertiesBufferList)
	{
		for (auto& mesh : MeshList)
		{
			mesh->GetMeshPropertiesBuffer(MeshPropertiesBufferList);
		}
	}

	static void GetMeshVertexBuffer(std::vector<VkDescriptorBufferInfo>& VertexBufferList)
	{
		for (auto& mesh : MeshList)
		{
			mesh->GetMeshVertexBuffer(VertexBufferList);
		}
	}

	static void GetMeshIndexBuffer(std::vector<VkDescriptorBufferInfo>& IndexBufferList)
	{
		for (auto& mesh : MeshList)
		{
			mesh->GetMeshIndexBuffer(IndexBufferList);
		}
	}
};

