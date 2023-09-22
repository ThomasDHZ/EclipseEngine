#pragma once
#include "Mesh.h"
class SpriteMesh3D : public Mesh
{
private:
public:
	SpriteMesh3D();
	SpriteMesh3D(const std::string& SpriteName, std::shared_ptr<Material> material, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID);
	~SpriteMesh3D();

	virtual void DrawSprite(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout, SceneProperties& sceneProperties) override;
	virtual void Update(float DeltaTime, const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix) override;
	virtual void Destroy() override;

	void SetSpritePosition(float x, float y, float z);
	void SetSpriteRotation(float x, float y, float z);
	void SetSpriteScale(float x, float y, float z);
	void SetSpritePosition(const glm::vec3& position);
	void SetSpriteRotation(const glm::vec3& rotation);
	void SetSpriteScale(const glm::vec3& scale);

	glm::vec3 GetSpritePosition() { return MeshPosition; }
	glm::vec3 GetSpriteRotation() { return MeshRotation; }
	glm::vec3 GetSpriteScale() { return MeshScale; }
};

