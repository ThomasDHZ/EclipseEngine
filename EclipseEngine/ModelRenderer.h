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
	ModelRenderer(const std::string Name, std::shared_ptr<Mesh> mesh);
	ModelRenderer(const std::string Name, std::shared_ptr<Mesh> mesh, const glm::vec3& position);
	ModelRenderer(const std::string Name, std::shared_ptr<Mesh> mesh, const glm::vec3& position, const glm::vec3& rotation);
	ModelRenderer(const std::string Name, std::shared_ptr<Mesh> mesh, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

	ModelRenderer(const std::string Name, std::vector<std::shared_ptr<Mesh>>& mesh);
	ModelRenderer(const std::string Name, std::vector<std::shared_ptr<Mesh>>& mesh, glm::vec3 position);
	ModelRenderer(const std::string Name, std::vector<std::shared_ptr<Mesh>>& mesh, glm::vec3 position, glm::vec3 rotation);
	ModelRenderer(const std::string Name, std::vector<std::shared_ptr<Mesh>>& mesh, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	ModelRenderer(const std::string Name, std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, InstancingDataStruct& instanceData);
	ModelRenderer(const std::string Name, std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, InstancingDataStruct& instanceData, glm::vec3 position);
	ModelRenderer(const std::string Name, std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, InstancingDataStruct& instanceData, glm::vec3 position, glm::vec3 rotation);
	ModelRenderer(const std::string Name, std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, InstancingDataStruct& instanceData, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	ModelRenderer(const std::string Name, std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, InstancingDataStruct& instanceData, std::shared_ptr<Material> materialPtr);
	ModelRenderer(const std::string Name, std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, InstancingDataStruct& instanceData, std::shared_ptr<Material> materialPtr, glm::vec3 position);
	ModelRenderer(const std::string Name, std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, InstancingDataStruct& instanceData, std::shared_ptr<Material> materialPtr, glm::vec3 position, glm::vec3 rotation);
	ModelRenderer(const std::string Name, std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, InstancingDataStruct& instanceData, std::shared_ptr<Material> materialPtr, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	ModelRenderer(const std::string Name, const std::string& FilePath);
	ModelRenderer(const std::string Name, const std::string& FilePath, const glm::vec3& position);
	ModelRenderer(const std::string Name, const std::string& FilePath, const glm::vec3& position, const glm::vec3& rotation);
	ModelRenderer(const std::string Name, const std::string& FilePath, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

	ModelRenderer(const std::string Name, const std::string& FilePath, InstancingDataStruct& instanceData);
	ModelRenderer(const std::string Name, const std::string& FilePath, InstancingDataStruct& instanceData, glm::vec3 position);
	ModelRenderer(const std::string Name, const std::string& FilePath, InstancingDataStruct& instanceData, glm::vec3 position, glm::vec3 rotation);
	ModelRenderer(const std::string Name, const std::string& FilePath, InstancingDataStruct& instanceData, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

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