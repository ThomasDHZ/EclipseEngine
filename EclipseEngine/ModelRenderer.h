#pragma once
#include "GameObject3D.h"
#include "Model.h"
#include "GameObjectManager.h"

class ModelRenderer : public GameObject3D
{
private:
	std::shared_ptr<Model> model;

	void from_json(nlohmann::json& json)
	{
		model = std::make_shared<Model>(Model(json.at("Model"), GameObjectID));
	}

public:
	ModelRenderer();

	ModelRenderer(const std::string Name, MeshLoader3D& meshLoader);
	ModelRenderer(const std::string Name, MeshLoader3D& meshLoader, const glm::vec3& position);
	ModelRenderer(const std::string Name, MeshLoader3D& meshLoader, const glm::vec3& position, const glm::vec3& rotation);
	ModelRenderer(const std::string Name, MeshLoader3D& meshLoader, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

	ModelRenderer(const std::string Name, ModelLoader& modelLoader);
	ModelRenderer(const std::string Name, ModelLoader& modelLoader, const glm::vec3& position);
	ModelRenderer(const std::string Name, ModelLoader& modelLoader, const glm::vec3& position, const glm::vec3& rotation);
	ModelRenderer(const std::string Name, ModelLoader& modelLoader, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

	ModelRenderer(const std::string Name, std::shared_ptr<Mesh> mesh);
	ModelRenderer(const std::string Name, std::shared_ptr<Mesh> mesh, const glm::vec3& position);
	ModelRenderer(const std::string Name, std::shared_ptr<Mesh> mesh, const glm::vec3& position, const glm::vec3& rotation);
	ModelRenderer(const std::string Name, std::shared_ptr<Mesh> mesh, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

	ModelRenderer(const std::string Name, std::vector<std::shared_ptr<Mesh>>& mesh);
	ModelRenderer(const std::string Name, std::vector<std::shared_ptr<Mesh>>& mesh, const glm::vec3& position);
	ModelRenderer(const std::string Name, std::vector<std::shared_ptr<Mesh>>& mesh, const glm::vec3& position, const glm::vec3& rotation);
	ModelRenderer(const std::string Name, std::vector<std::shared_ptr<Mesh>>& mesh, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

	ModelRenderer(const std::string Name, const std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices);
	ModelRenderer(const std::string Name, const std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices, const glm::vec3& position);
	ModelRenderer(const std::string Name, const std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices, const glm::vec3& position, const glm::vec3& rotation);
	ModelRenderer(const std::string Name, const std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

	ModelRenderer(const std::string Name, const std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices, std::shared_ptr<Material> materialPtr);
	ModelRenderer(const std::string Name, const std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices, std::shared_ptr<Material> materialPtr, const glm::vec3& position);
	ModelRenderer(const std::string Name, const std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices, std::shared_ptr<Material> materialPtr, const glm::vec3& position, const glm::vec3& rotation);
	ModelRenderer(const std::string Name, const std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices, std::shared_ptr<Material> materialPtr, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

	ModelRenderer(const std::string Name, const std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices, const InstancingDataStruct& instanceData);
	ModelRenderer(const std::string Name, const std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices, const InstancingDataStruct& instanceData, const glm::vec3& position);
	ModelRenderer(const std::string Name, const std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices, const InstancingDataStruct& instanceData, const glm::vec3& position, const glm::vec3& rotation);
	ModelRenderer(const std::string Name, const std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices, const InstancingDataStruct& instanceData, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

	ModelRenderer(const std::string Name, const std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices, const InstancingDataStruct& instanceData, std::shared_ptr<Material> materialPtr);
	ModelRenderer(const std::string Name, const std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices, const InstancingDataStruct& instanceData, std::shared_ptr<Material> materialPtr, const glm::vec3& position);
	ModelRenderer(const std::string Name, const std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices, const InstancingDataStruct& instanceData, std::shared_ptr<Material> materialPtr, const glm::vec3& position, const glm::vec3& rotation);
	ModelRenderer(const std::string Name, const std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& indices, const InstancingDataStruct& instanceData, std::shared_ptr<Material> materialPtr, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

	ModelRenderer(const std::string Name, const std::string& FilePath);
	ModelRenderer(const std::string Name, const std::string& FilePath, const glm::vec3& position);
	ModelRenderer(const std::string Name, const std::string& FilePath, const glm::vec3& position, const glm::vec3& rotation);
	ModelRenderer(const std::string Name, const std::string& FilePath, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

	ModelRenderer(const std::string Name, const std::string& FilePath, const InstancingDataStruct& instanceData);
	ModelRenderer(const std::string Name, const std::string& FilePath, const InstancingDataStruct& instanceData, const glm::vec3& position);
	ModelRenderer(const std::string Name, const std::string& FilePath, const InstancingDataStruct& instanceData, const glm::vec3& position, const glm::vec3& rotation);
	ModelRenderer(const std::string Name, const std::string& FilePath, const InstancingDataStruct& instanceData, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

	ModelRenderer(nlohmann::json json);
	virtual ~ModelRenderer();

	virtual void LoadPrefab(nlohmann::json& json) override;
	virtual void SaveAsPrefab(nlohmann::json& json) override;
	virtual void Update(float DeltaTime) override;
	virtual void Destroy() override;

	std::shared_ptr<Model> GetModel() { return model; }

	virtual void to_json(nlohmann::json& json) override
	{
		GameObject3D::to_json(json);
		model->to_json(json["Model"]);
	}
};