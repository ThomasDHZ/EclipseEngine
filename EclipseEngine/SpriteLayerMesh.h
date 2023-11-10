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
	SpriteLayerMesh(const std::string& SpriteName, std::vector<LevelTile>& levelTiles, std::vector<std::shared_ptr<Material>> materialList, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID);
	~SpriteLayerMesh();

	void AddTile(LevelTile tile);
	void AddTiles(std::vector<LevelTile> tile);

	virtual void Update(float DeltaTime, const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix) override;
	virtual void DrawLevelLayer(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout, SceneProperties& sceneProperites);

	std::vector<LevelTile> GetLevelTile() { return TileList; }

};

