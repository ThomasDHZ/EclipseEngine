#pragma once
#include <GameObject3D.h>
#include "Model.h"
#include "GameObjectManager.h"

class ModelRenderer : public GameObject3D
{
private:
	std::shared_ptr<Model> model;

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
	virtual ~ModelRenderer();

	virtual void Update(float DeltaTime) override;
	virtual void Destroy() override;

	std::shared_ptr<Model> GetModel() { return model; }

	//virtual void to_json(nlohmann::json& json) override
	//{
	//	GameObject3D::to_json(json);
	//	model->to_json(json["Model"]);
	//}
};