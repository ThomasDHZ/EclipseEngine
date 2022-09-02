#pragma once
#include "Mesh.h"
#include "MeshRendererManager.h"

struct SpriteLoader
{
	uint64_t ModelID = -1;
	uint64_t GameObjectID = -1;

	glm::mat4 GameObjectTransform = glm::mat4(1.0f);
	glm::mat4 MeshTransform = glm::mat4(1.0f);
	std::shared_ptr<Material> materialPtr = MaterialManager::GetDefaultMaterial();

	SpriteLoader() {}
};

class Mesh2D : public Mesh
{
private:
	const std::vector<Vertex2D> SpriteVertexList = {
			{ {-0.5f, -0.5f},  {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f} },
			{ { 0.5f, -0.5f},  {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} },
			{ { 0.5f,  0.5f},  {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f} },
			{ {-0.5f,  0.5f},  {1.0f, 1.0f}, {1.0f, 1.0f, 0.0f} }
	};

	const std::vector<uint32_t> SpriteIndexList = {
		0, 1, 2, 2, 3, 0
	};

	std::vector<Vertex2D> VertexList;
	std::vector<uint32_t> IndexList;

	int MeshDepth = 0;
	glm::vec2 MeshPosition = glm::vec3(0.0f);
	glm::vec2 MeshRotation = glm::vec3(0.0f);
	glm::vec2 MeshScale = glm::vec3(1.0f);


public:
	Mesh2D();
	Mesh2D(SpriteLoader& spriteLoader);
	~Mesh2D();

	void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix) override;

	void SetPosition(float x, float y);
	void SetPosition(glm::vec2 position);
	void SetDepth(float Depth);
	void SetRotation(float x, float y);
	void SetRotation(glm::vec2 rotation);
	void SetScale(float x, float y);
	void SetScale(glm::vec2 Scale);

	float GetDepth() { return MeshDepth; }
	glm::vec2 GetPosition() { return MeshPosition; }
	glm::vec2 GetRotation() { return MeshRotation; }
	glm::vec2 GetScale() { return MeshScale; }

	virtual void to_json(nlohmann::json& json) override
	{
		JsonConverter::to_json(json["MeshPosition"], MeshPosition);
		JsonConverter::to_json(json["MeshRotation"], MeshRotation);
		JsonConverter::to_json(json["MeshScale"], MeshScale);

		Mesh::to_json(json);
	}
};

