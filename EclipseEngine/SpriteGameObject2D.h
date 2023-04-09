#pragma once
#include "GameObject2D.h"
#include "Mesh2D.h"

class SpriteGameObject2D : public GameObject2D
{
private:
	const std::vector<Vertex2D> SpriteVertexList =
	{
	  { {-0.5f, -0.5f},  {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f} },
	  { { 0.5f, -0.5f},  {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },
	  { { 0.5f,  0.5f},  {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f} },
	  { {-0.5f,  0.5f},  {1.0f, 1.0f}, {1.0f, 1.0f, 0.0f, 1.0f} }
	};
	const std::vector<uint32_t> SpriteIndexList = {
	   0, 1, 2,
	   2, 3, 0
	};

public:
	SpriteGameObject2D();
	SpriteGameObject2D(const std::string Name);
	SpriteGameObject2D(const std::string Name, const glm::vec2& position, uint32_t Depth);
	SpriteGameObject2D(const std::string Name, const glm::vec2& position, const glm::vec2& rotation, uint32_t Depth);
	SpriteGameObject2D(const std::string Name, const glm::vec2& position, const glm::vec2& rotation, const glm::vec2& scale, uint32_t Depth);

	SpriteGameObject2D(const std::string Name, std::shared_ptr<GLTFMaterial> material);
	SpriteGameObject2D(const std::string Name, std::shared_ptr<GLTFMaterial> material, const glm::vec2& position, uint32_t Depth);
	SpriteGameObject2D(const std::string Name, std::shared_ptr<GLTFMaterial> material, const glm::vec2& position, const glm::vec2& rotation, uint32_t Depth);
	SpriteGameObject2D(const std::string Name, std::shared_ptr<GLTFMaterial> material, const glm::vec2& position, const glm::vec2& rotation, const glm::vec2& scale, uint32_t Depth);
	virtual ~SpriteGameObject2D();

	virtual void Update(float DeltaTime) override;
	virtual void Destroy() override;

	void SetSpriteMaterial(std::shared_ptr<Material> SpriteMaterial);
};
