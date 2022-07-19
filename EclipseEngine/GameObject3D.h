#pragma once
#include "GameObject.h"

class GameObject3D : public GameObject
{
private:
public:
	GameObject3D();

	GameObject3D(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList);
	GameObject3D(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, glm::vec3 position);
	GameObject3D(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, glm::vec3 position, glm::vec3 rotation);
	GameObject3D(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	GameObject3D(const std::string Name, const std::string filePath);
	GameObject3D(const std::string Name, const std::string filePath, glm::vec3 position);
	GameObject3D(const std::string Name, const std::string filePath, glm::vec3 position, glm::vec3 rotation);
	GameObject3D(const std::string Name, const std::string filePath, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	GameObject3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint);
	GameObject3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec3& position);
	GameObject3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec3& position, glm::vec3& rotation);
	GameObject3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale);

	GameObject3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec4& Color);
	GameObject3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec4& Color, glm::vec3& position);
	GameObject3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec4& Color, glm::vec3& position, glm::vec3& rotation);
	GameObject3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec4& Color, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale);

	GameObject3D(const std::string Name, std::vector<LineVertex3D> VertexList);
	GameObject3D(const std::string Name, std::vector<LineVertex3D> VertexList, glm::vec3 position);
	GameObject3D(const std::string Name, std::vector<LineVertex3D> VertexList, glm::vec3 position, glm::vec3 rotation);
	GameObject3D(const std::string Name, std::vector<LineVertex3D> VertexList, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	GameObject3D(const std::string Name, int GridSize, float GridSpacing);
	GameObject3D(const std::string Name, int GridSizeX, int GridSizeY, int GridSizeZ, float GridSpacingX, float GridSpacingY, float GridSpacingZ);

	~GameObject3D();

	virtual void Update(float DeltaTime) override;

	void SetGameObjectPosition(float x, float y, float z);
	void SetGameObjectRotation(float x, float y, float z);
	void SetGameObjectScale(float x, float y, float z);
	void SetGameObjectPosition(glm::vec3 gameObjectPosition);
	void SetGameObjectRotation(glm::vec3 gameObjectRotation);
	void SetGameObjectScale(glm::vec3 gameObjectScale);

	glm::mat4 GetGameObjectMatrix() { return GameObject::GetGameObjectMatrix(); }
	glm::vec3 GetGameObjectPosition() { return GameObject::GetGameObjectPosition(); }
	glm::vec3 GetGameObjectRotation() { return GameObject::GetGameObjectRotation(); }
	glm::vec3 GetGameObjectScale() { return GameObject::GetGameObjectScale(); }

	glm::vec3* GetGameObjectPositionPtr() { return &GameObjectPosition; }
	glm::vec3* GetGameObjectRotationPtr() { return &GameObjectRotation; }
	glm::vec3* GetGameObjectScalePtr() { return &GameObjectScale; }
};

