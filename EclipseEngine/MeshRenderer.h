#pragma once
#include "ComponentRenderer.h"
#include "Model.h"

class MeshRenderer : public Component
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
	MeshRenderer(const std::string& FilePath, uint64_t GameObjectID);
	MeshRenderer(const std::string& FilePath, glm::vec3 position, uint64_t GameObjectID);
	MeshRenderer(const std::string& FilePath, glm::vec3 position, glm::vec3 rotation, uint64_t GameObjectID);
	MeshRenderer(const std::string& FilePath, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, uint64_t GameObjectID);
	MeshRenderer(nlohmann::json json, uint64_t GameObjectID);
	virtual ~MeshRenderer();

	void Update(float DeltaTime) override;
	void Destroy() override;

	std::shared_ptr<Model> GetModel() { return model; }

	//virtual void to_json(nlohmann::json& json) override
	//{
	///*	Component::to_json(json);
	//	model->to_json(json["Model"]);*/
	//}
};

