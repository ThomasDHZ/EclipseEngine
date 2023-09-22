#pragma once
#include "Mesh.h"
#include "LevelTile.h"
#include "SpriteLayerMesh.h"

class Level2D : public Mesh
{
private:
	std::string LevelName;

	glm::ivec2 LevelSizeInPixels;
	glm::ivec2 TileSizeInPixels;
	glm::ivec2 LevelTileCount;
	glm::ivec2 LevelBounds;
	glm::vec2 TileUVSize;

	std::vector<SpriteLayerMesh> DrawLevelLayerMeshList;
	std::vector<SpriteLayerMesh> DrawAnimatedLayerMeshList;
	std::vector<SpriteLayerMesh> DrawSpriteLayerMeshList;
public:
	Level2D();
	Level2D(const std::string& levelName, glm::ivec2 tileSizeInPixels, glm::ivec2 levelBounds, std::vector<std::shared_ptr<Material>> materialList, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelID);
	~Level2D();

	void LoadLevel();
	virtual void Update(float DeltaTime, const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix) override;
	virtual void DrawLevelLayer(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout, SceneProperties& constBuffer) override;
};

