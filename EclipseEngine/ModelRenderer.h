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
		/*	Component::from_json(json);
			model = std::make_shared<Model>(Model(json.at("Model"), ParentGameObjectID));*/
			//	ModelManager::AddModel(model);
	}

public:
	ModelRenderer();
	ModelRenderer(const std::string Name, std::shared_ptr<Mesh> mesh);
	ModelRenderer(const std::string Name, std::shared_ptr<Mesh> mesh, glm::vec3 position);
	ModelRenderer(const std::string Name, std::shared_ptr<Mesh> mesh, glm::vec3 position, glm::vec3 rotation);
	ModelRenderer(const std::string Name, std::shared_ptr<Mesh> mesh, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	ModelRenderer(const std::string Name, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);
	ModelRenderer(const std::string Name, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, glm::vec3 position);
	ModelRenderer(const std::string Name, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, glm::vec3 position, glm::vec3 rotation);
	ModelRenderer(const std::string Name, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	ModelRenderer(const std::string Name, std::vector<Mesh>& mesh);
	ModelRenderer(const std::string Name, std::vector<Mesh>& mesh, glm::vec3 position);
	ModelRenderer(const std::string Name, std::vector<Mesh>& mesh, glm::vec3 position, glm::vec3 rotation);
	ModelRenderer(const std::string Name, std::vector<Mesh>& mesh, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	ModelRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList);
	ModelRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, glm::vec3 position);
	ModelRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, glm::vec3 position, glm::vec3 rotation);
	ModelRenderer(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	ModelRenderer(const std::string Name, const std::string& FilePath);
	ModelRenderer(const std::string Name, const std::string& FilePath, glm::vec3 position);
	ModelRenderer(const std::string Name, const std::string& FilePath, glm::vec3 position, glm::vec3 rotation);
	ModelRenderer(const std::string Name, const std::string& FilePath, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	ModelRenderer(nlohmann::json json, uint64_t GameObjectID);
	virtual ~ModelRenderer();

	void Update(float DeltaTime) override;
	void Destroy() override;

	std::shared_ptr<Model> GetModel() { return model; }

	//virtual void to_json(nlohmann::json& json) override
	//{
	///*	Component::to_json(json);
	//	model->to_json(json["Model"]);*/
	//}
};