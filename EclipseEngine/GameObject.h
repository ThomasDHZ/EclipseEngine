#pragma once
#include "VulkanRenderer.h"
#include "Component.h"
#include <glm/glm.hpp>
#include "GLTF_Temp_Model.h"

enum GameObjectRenderType
{
	kNoRender,
	kMeshRenderer,
	kModelRenderer,
	kSpriteRenderer,
	kLineRenderer2D,
	kLineRenderer3D,
	kGridRenderer2D,
	kGridRenderer3D
};

class GameObject
{
private:
	static uint64_t GameObjectIDCounter;

protected:
	std::string ObjectName;
	uint64_t GameObjectID;
	std::shared_ptr<GLTF_Temp_Model> model;
	std::vector<std::shared_ptr<Component>> ComponentList;
	GameObjectRenderType  RenderType;

	glm::mat4 GameObjectTransform = glm::mat4(1.0f);

	void SetGameObjectPosition(float x, float y, float z);
	void SetGameObjectRotation(float x, float y, float z);
	void SetGameObjectScale(float x, float y, float z);
	void SetGameObjectPosition(const glm::vec3& gameObjectPosition);
	void SetGameObjectRotation(const glm::vec3& gameObjectRotation);
	void SetGameObjectScale(const glm::vec3& gameObjectScale);

	glm::mat4 GetGameObjectMatrix() { return GameObjectTransform; }
	glm::vec3 GetGameObjectPosition() { return GameObjectPosition; }
	glm::vec3 GetGameObjectRotation() { return GameObjectRotation; }
	glm::vec3 GetGameObjectScale() { return GameObjectScale; }

	glm::vec3* GetGameObjectPositionPtr() { return &GameObjectPosition; }
	glm::vec3* GetGameObjectRotationPtr() { return &GameObjectRotation; }
	glm::vec3* GetGameObjectScalePtr() { return &GameObjectScale; }

public:

	GameObject();
	GameObject(const std::string Name, GameObjectRenderType renderType);
	GameObject(const std::string Name, GameObjectRenderType renderType, const glm::vec3& Position);
	GameObject(const std::string Name, GameObjectRenderType renderType, const glm::vec3& Position, const glm::vec3& Rotation);
	GameObject(const std::string Name, GameObjectRenderType renderType, const glm::vec3& Position, const glm::vec3& Rotation, const glm::vec3& Scale);
	GameObject(const std::string Name, const std::string fileName);
	GameObject(const std::string Name, const std::string fileName, const glm::vec3& Position);
	GameObject(const std::string Name, const std::string fileName, const glm::vec3& Position, const glm::vec3& Rotation);
	GameObject(const std::string Name, const std::string fileName, const glm::vec3& Position, const glm::vec3& Rotation, const glm::vec3& Scale);
	virtual ~GameObject();

	glm::vec3 GameObjectPosition = glm::vec3(0.0f);
	glm::vec3 GameObjectRotation = glm::vec3(0.0f);
	glm::vec3 GameObjectScale = glm::vec3(1.0f);

	virtual void Draw(VkCommandBuffer& commandBuffer, VkPipelineLayout shaderPipelineLayout);
	virtual void Update(float DeltaTime);
	virtual void Destroy();

	void AddComponent(std::shared_ptr<Component> component);
	void RemoveComponent(std::shared_ptr<Component> component);

	void GenerateID();

	std::string GetObjectName() { return ObjectName; }
	uint64_t GetGameObjectID() { return GameObjectID; }
	GameObjectRenderType GetRenderType() { return RenderType; }

	std::shared_ptr<Component> GetComponentBySubType(ComponentSubType componentType);
	std::shared_ptr<Component> GetComponentByType(ComponentType componentType);
	std::shared_ptr<Component> GetComponentByID(uint64_t ComponentID);
	std::vector<std::shared_ptr<Component>> GetComponentList() { return GetComponentList(); };

	std::vector<std::shared_ptr<Temp_GLTFMesh>> GetMeshList() { return model->GetMeshList(); }
	std::vector<std::shared_ptr<GLTFMaterial>> GetMaterialList() { return model->GetMaterialList(); }

	std::vector<VkDescriptorBufferInfo> GetMeshPropertiesBuffer() { return model->GetMeshPropertiesBuffer(); }
	std::vector<VkDescriptorBufferInfo> GetTransformMatrixBuffer() { return model->MeshList[0]->GetTransformMatrixBuffer(); }
	std::vector<VkDescriptorImageInfo> GetTexturePropertiesBuffer() { return model->GetTexturePropertiesBuffer(); }
	std::vector<VkDescriptorBufferInfo> GetMaterialPropertiesBuffer() { return model->GetMaterialPropertiesBuffer(); }
	ModelTypeEnum GetModelType() { return model->GetModelType(); }

	bool operator==(const GameObject& rhs) const
	{ 
		return this->GameObjectID == rhs.GameObjectID; 
	}
};

