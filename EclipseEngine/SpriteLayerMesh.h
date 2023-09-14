#pragma once
#include "Mesh.h"
#include "LevelTile.h"

class SpriteLayerMesh : public Mesh
{
private:
	std::vector<LevelTile> TileList;
	std::vector<InstancedVertexData2D> InstancedVertex2DDataList;
	GLTFInstancingDataStruct InstanceData;

	void Instancing2DStartUp(GLTFInstancingDataStruct& instanceData);
public:
	SpriteLayerMesh();
	SpriteLayerMesh(const std::string& SpriteName, glm::ivec2 levelBounds, std::vector<std::shared_ptr<Material>> materialList, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID);
	~SpriteLayerMesh();

	virtual void DrawLevelLayer(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout, SceneProperties& sceneProperites);
	virtual void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix) override;
};

