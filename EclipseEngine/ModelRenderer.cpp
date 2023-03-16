#include "ModelRenderer.h"

ModelRenderer::ModelRenderer()
{
}

ModelRenderer::ModelRenderer(const std::string Name, const std::string FilePath) : GameObject3D(Name, kModelRenderer)
{
	model = std::make_shared<GLTF_Temp_Model>(GLTF_Temp_Model(FilePath, GetGameObjectMatrix(), GameObjectID));
}

ModelRenderer::ModelRenderer(const std::string Name, const std::string FilePath, const glm::vec3& position) : GameObject3D(Name, kModelRenderer, position)
{
	model = std::make_shared<GLTF_Temp_Model>(GLTF_Temp_Model(FilePath, GetGameObjectMatrix(), GameObjectID));
}

ModelRenderer::ModelRenderer(const std::string Name, const std::string FilePath, const glm::vec3& position, const glm::vec3& rotation) : GameObject3D(Name, kModelRenderer, position, rotation)
{
	model = std::make_shared<GLTF_Temp_Model>(GLTF_Temp_Model(FilePath, GetGameObjectMatrix(), GameObjectID));
}

ModelRenderer::ModelRenderer(const std::string Name, const std::string FilePath, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : GameObject3D(Name, kModelRenderer, position, rotation, scale)
{
	model = std::make_shared<GLTF_Temp_Model>(GLTF_Temp_Model(FilePath, GetGameObjectMatrix(), GameObjectID));
}

ModelRenderer::~ModelRenderer()
{
}

void ModelRenderer::Draw(VkCommandBuffer& commandBuffer, VkPipelineLayout ShaderPipelineLayout)
{
	model->Draw(commandBuffer, ShaderPipelineLayout);
}

void ModelRenderer::Update(float DeltaTime)
{
	GameObject::Update(DeltaTime);
	model->Update(GameObjectTransform);
}

void ModelRenderer::Destroy()
{
	model->Destroy();
}