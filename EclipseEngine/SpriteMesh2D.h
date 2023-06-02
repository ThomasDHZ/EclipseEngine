#pragma once
#include "Mesh.h"
class SpriteMesh2D : public Mesh
{
private:
public:
	SpriteMesh2D();
	SpriteMesh2D(const std::string& SpriteName, std::shared_ptr<Material> material, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID);
	~SpriteMesh2D();

	virtual void DrawSprite(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout) override;
	virtual void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix) override;

	void SetSpritePosition(float x, float y, int drawLayer);
	void SetSpriteRotation(float x, float y);
	void SetSpriteScale(float x, float y);
	void SetSpritePosition(const glm::vec2& position, int drawLayer);
	void SetSpriteRotation(const glm::vec2& rotation);
	void SetSpriteScale(const glm::vec2& scale);

	glm::vec2 GetSpritePosition() { return MeshPosition; }
	glm::vec2 GetSpriteRotation() { return MeshRotation; }
	glm::vec2 GetSpriteScale() { return MeshScale; }
	int GetDrawLayer() { return MeshPosition.z; }
};

