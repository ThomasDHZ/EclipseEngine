#pragma once
#include "ComponentRenderer.h"
#include "Model.h"

class MeshRendererComponent : public Component
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
	MeshRendererComponent(std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, uint64_t GameObjectID);
	MeshRendererComponent(const std::string& FilePath, uint64_t GameObjectID);
	MeshRendererComponent(nlohmann::json json, uint64_t GameObjectID);
	virtual ~MeshRendererComponent();

	void Update(const glm::mat4& GameObjectMatrix, float DeltaTime);
	void Destroy() override;

	std::shared_ptr<Model> GetModel() { return model; }

	//virtual void to_json(nlohmann::json& json) override
	//{
	///*	Component::to_json(json);
	//	model->to_json(json["Model"]);*/
	//}
};

