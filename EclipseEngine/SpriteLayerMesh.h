#pragma once
#include "Mesh.h"
#include "Sprite.h"

class SpriteLayerMesh : public Mesh
{
private:
	std::vector<Sprite> SpriteList;
	std::vector<InstancedVertexData2D> InstancedVertex2DDataList;
	GLTFInstancingDataStruct InstanceData;

	void Instancing2DStartUp(GLTFInstancingDataStruct& instanceData);
public:
	SpriteLayerMesh();
	SpriteLayerMesh(const std::string& SpriteName, std::vector<Sprite>& spriteList, std::vector<std::shared_ptr<Material>> materialList, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID);
	~SpriteLayerMesh();

	void AddSprite(Sprite sprite);
	void AddSprite(std::vector<Sprite> sprite);

	virtual void Update(float DeltaTime, const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix) override;
	virtual void DrawLevelLayer(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout, SceneProperties& sceneProperites);

	std::vector<Sprite> GetLevelSprite() { return SpriteList; }

};

