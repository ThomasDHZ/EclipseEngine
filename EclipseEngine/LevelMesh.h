#pragma once
#include "Mesh.h"

class LevelMesh : public Mesh
{
private:
public:
	LevelMesh();
	LevelMesh(const std::string& SpriteName, std::shared_ptr<Material> material, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID);
	~LevelMesh();

	virtual void DrawSprite(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout, SceneProperties& sceneProperites) override;
	virtual void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix) override;
};

