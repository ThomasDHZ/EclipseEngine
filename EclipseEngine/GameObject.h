#pragma once
#include "VulkanRenderer.h"
#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Model.h"
#include "LineMesh3D.h"

enum GameObjectRenderType
{
	kNoRender,
	kModelRenderer,
	kInstanceRenderer,
	kSpriteRenderer,
	kLineRenderer2D,
	kLineRenderer3D,
	kGridRenderer2D,
	kGridRenderer3D,
	kDebugRenderer
};

class GameObject
{
private:
	static uint64_t GameObjectIDCounter;

protected:
	uint64_t GameObjectID;
	std::shared_ptr<Model> GameObjectRenderer;
	std::vector<std::shared_ptr<Component>> ComponentList;

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
	std::string ObjectName;
	GameObjectRenderType  RenderType;

	void GenerateID();
	GameObject();

	GameObject(const std::string& objectName, GameObjectRenderType renderType)
	{
		GenerateID();
		ObjectName = objectName;
		RenderType = renderType;
	}

	GameObject(const std::string& objectName, GameObjectRenderType renderType, const glm::vec3& Position)
	{
		GenerateID();
		ObjectName = objectName;
		RenderType = renderType;

		GameObjectPosition = Position;

		GameObjectTransform = glm::mat4(1.0f);
		GameObjectTransform = glm::translate(GameObjectTransform, GameObjectPosition);
		GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		GameObjectTransform = glm::scale(GameObjectTransform, GameObjectScale);
	}

	GameObject(const std::string& objectName, GameObjectRenderType renderType, const glm::vec3& Position, const glm::vec3& Rotation)
	{
		GenerateID();
		ObjectName = objectName;
		RenderType = renderType;

		GameObjectPosition = Position;
		GameObjectRotation = Rotation;

		GameObjectTransform = glm::mat4(1.0f);
		GameObjectTransform = glm::translate(GameObjectTransform, GameObjectPosition);
		GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		GameObjectTransform = glm::scale(GameObjectTransform, GameObjectScale);
	}

	GameObject(const std::string& objectName, GameObjectRenderType renderType, const glm::vec3& Position, const glm::vec3& Rotation, const glm::vec3& Scale)
	{
		GenerateID();
		ObjectName = objectName;
		RenderType = renderType;

		GameObjectPosition = Position;
		GameObjectRotation = Rotation;
		GameObjectScale = Scale;

		GameObjectTransform = glm::mat4(1.0f);
		GameObjectTransform = glm::translate(GameObjectTransform, GameObjectPosition);
		GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		GameObjectTransform = glm::scale(GameObjectTransform, GameObjectScale);
	}

	template <class T>
	void LoadRenderObject(const std::string& fileName)
	{
		GameObjectRenderer = std::make_shared<Model>(Model());
		GameObjectRenderer->LoadModel<T>(fileName, GameObjectTransform, GameObjectID);
		VulkanRenderer::UpdateRendererFlag = true;
	}

	template <class T>
	void LoadRenderObject(const std::string& fileName, std::shared_ptr<Material> material)
	{
		GameObjectRenderer = std::make_shared<Model>(Model());
		GameObjectRenderer->LoadModel<T>(fileName, material, GameObjectTransform, GameObjectID);
		VulkanRenderer::UpdateRendererFlag = true;
	}

	template <class T>
	void LoadRenderObject(const std::string& fileName, GLTFInstancingDataStruct& instanceData)
	{
		GameObjectRenderer = std::make_shared<Model>(Model());
		GameObjectRenderer->LoadModel<T>(fileName, instanceData, GameObjectTransform, GameObjectID);
		VulkanRenderer::UpdateRendererFlag = true;
	}

	template <class T>
	void LoadRenderObject(const std::vector<T>& vertexList)
	{
		GameObjectRenderer = std::make_shared<Model>(Model());
		GameObjectRenderer->LoadModel<T>(ObjectName, vertexList, GameObjectTransform, GameObjectID);
		VulkanRenderer::UpdateRendererFlag = true;
	}

	template <class T>
	void LoadRenderObject(const std::vector<T>& vertexList, const std::vector<uint32_t>& indexList)
	{
		GameObjectRenderer = std::make_shared<Model>(Model());
		GameObjectRenderer->LoadModel<T>(ObjectName, vertexList, indexList, GameObjectTransform, GameObjectID);
		VulkanRenderer::UpdateRendererFlag = true;
	}

	template <class T>
	void LoadRenderObject(const std::vector<T>& vertexList, const std::vector<uint32_t>& indexList, const std::shared_ptr<Material> material)
	{
		GameObjectRenderer = std::make_shared<Model>(Model());
		GameObjectRenderer->LoadModel<T>(ObjectName, vertexList, indexList, material, GameObjectTransform, GameObjectID);
		VulkanRenderer::UpdateRendererFlag = true;
	}

	virtual ~GameObject();

	glm::vec3 GameObjectPosition = glm::vec3(0.0f);
	glm::vec3 GameObjectRotation = glm::vec3(0.0f);
	glm::vec3 GameObjectScale = glm::vec3(1.0f);


	template<class T>
	void DrawMesh(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout shaderPipelineLayout, T& constBuffer)
	{
		GameObjectRenderer->DrawMesh<T>(commandBuffer, descriptorset, shaderPipelineLayout, constBuffer);
	}

	virtual void DrawMesh(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout shaderPipelineLayout, SceneProperties& constBuffer)
	{
		GameObjectRenderer->DrawMesh(commandBuffer, descriptorset, shaderPipelineLayout, constBuffer);
	}

	virtual void DrawMesh(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout shaderPipelineLayout, LightViewSceneData& constBuffer)
	{
		GameObjectRenderer->DrawMesh(commandBuffer, descriptorset, shaderPipelineLayout, constBuffer);
	}

	template<class T>
	void DrawInstancedMesh(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout shaderPipelineLayout, T& constBuffer)
	{
		GameObjectRenderer->DrawInstancedMesh<T>(commandBuffer, descriptorset, shaderPipelineLayout);
	}

	virtual void DrawInstancedMesh(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout shaderPipelineLayout, SceneProperties& constBuffer)
	{
		GameObjectRenderer->DrawInstancedMesh(commandBuffer, descriptorset, shaderPipelineLayout, constBuffer);
	}

	template<class T>
	void DrawSprite(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout shaderPipelineLayout, T& constBuffer)
	{
		GameObjectRenderer->DrawSprite<T>(commandBuffer, descriptorset, shaderPipelineLayout, constBuffer);
	}
	virtual void DrawSprite(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout, SceneProperties& constBuffer)
	{
		GameObjectRenderer->DrawSprite(commandBuffer, descriptorSet, shaderPipelineLayout, constBuffer);
	}

	virtual void DrawLine(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout);
	virtual void Update(float DeltaTime);
	virtual void Destroy();

	void AddComponent(std::shared_ptr<Component> component);
	void RemoveComponent(std::shared_ptr<Component> component);

	std::string GetObjectName() { return ObjectName; }
	uint64_t GetGameObjectID() { return GameObjectID; }
	GameObjectRenderType GetRenderType() { return RenderType; }

	std::shared_ptr<Model> GetGameObjectRenderer() { return GameObjectRenderer; }

	std::shared_ptr<Component> GetComponentBySubType(ComponentSubType componentType);
	std::shared_ptr<Component> GetComponentByType(ComponentType componentType);
	std::shared_ptr<Component> GetComponentByID(uint64_t ComponentID);
	std::vector<std::shared_ptr<Component>> GetComponentList() { return GetComponentList(); };

	std::vector<std::shared_ptr<Mesh>> GetMeshList() { return GameObjectRenderer->GetMeshList(); }
	std::vector<std::shared_ptr<Material>> GetMaterialList() { return GameObjectRenderer->GetMaterialList(); }

	std::vector<VkDescriptorBufferInfo> GetGameObjectPropertiesBuffer() { return GameObjectRenderer->GetMeshPropertiesBuffer(); }
	std::vector<VkDescriptorBufferInfo> GetGameObjectTransformMatrixBuffer() { return GameObjectRenderer->GetTransformMatrixBuffer(); }
	std::vector<VkDescriptorImageInfo> GetTexturePropertiesBuffer() { return GameObjectRenderer->GetTexturePropertiesBuffer(); }
	std::vector<VkDescriptorBufferInfo> GetMaterialPropertiesBuffer() { return GameObjectRenderer->GetMaterialPropertiesBuffer(); }

	bool operator==(const GameObject& rhs) const
	{
		return this->GameObjectID == rhs.GameObjectID;
	}
};

