#pragma once
#include <GameObject3D.h>
#include "GLTF_Temp_Model.h"

class ModelRenderer : public GameObject3D
{
private:
	std::shared_ptr<GLTF_Temp_Model> model;

public:
	ModelRenderer();
	ModelRenderer(const std::string Name, const std::string FilePath);
	ModelRenderer(const std::string Name, const std::string FilePath, const glm::vec3& position);
	ModelRenderer(const std::string Name, const std::string FilePath, const glm::vec3& position, const glm::vec3& rotation);
	ModelRenderer(const std::string Name, const std::string FilePath, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
	virtual ~ModelRenderer();

	virtual void Draw(VkCommandBuffer& commandBuffer, VkPipelineLayout ShaderPipelineLayout) override;
	virtual void Update(float DeltaTime) override;
	virtual void Destroy() override;

	std::shared_ptr<GLTF_Temp_Model> GetModel() { return model; }
	std::vector<std::shared_ptr<GLTFMaterial>> GetMaterialList() { return model->MaterialList; }
	std::vector<VkDescriptorBufferInfo> GetMeshPropertiesBuffer() { return model->GetMeshPropertiesBuffer(); }
	std::vector<VkDescriptorBufferInfo> GetTransformMatrixBuffer() { return model->MeshList[0]->TransformMatrixBuffer; }
	std::vector<VkDescriptorImageInfo> GetTexturePropertiesBuffer() { return model->GetTexturePropertiesBuffer(); }
	std::vector<VkDescriptorBufferInfo> GetMaterialPropertiesBuffer() { return model->GetMaterialPropertiesBuffer(); }
	std::vector<VkDescriptorBufferInfo> GetSunLightPropertiesBuffer() { return model->GetSunLightPropertiesBuffer(); }
	std::vector<VkDescriptorBufferInfo> GetDirectionalLightPropertiesBuffer() { return model->GetDirectionalLightPropertiesBuffer(); }
	std::vector<VkDescriptorBufferInfo> GetPointLightPropertiesBuffer() { return model->GetPointLightPropertiesBuffer(); }
	std::vector<VkDescriptorBufferInfo> GetSpotLightPropertiesBuffer() { return model->GetSpotLightPropertiesBuffer(); }
};