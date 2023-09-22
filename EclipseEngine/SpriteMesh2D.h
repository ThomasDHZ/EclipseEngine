#pragma once
#include "Mesh.h"
#include "Animation2D.h"
#include "AnimationPlayer2D.h"

class SpriteMesh2D : public Mesh
{
private:
protected:
	AnimationPlayer2D AnimationPlayer;
	glm::vec2 Velocity = glm::vec2(0.0f);
	glm::ivec2 FlipSprite = glm::ivec2(0);

	void AddAnimation(std::shared_ptr<Animation2D> animation);
	void AddAnimation(std::vector<std::shared_ptr<Animation2D>> AnimationList);
public:
	SpriteMesh2D();
	SpriteMesh2D(const std::string& SpriteName, std::shared_ptr<Material> material, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID);
	~SpriteMesh2D();



	virtual void DrawSprite(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout, SceneProperties& sceneProperites) override;
	virtual void Update(float DeltaTime, const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix) override;

	void SetAnimation(uint32_t AnimationIndex);
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

