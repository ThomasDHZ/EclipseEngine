#pragma once
#include "Component.h"
#include "Mesh.h"
class GridRenderer2DComponent : public Component
{
private:
	std::shared_ptr<Mesh> Grid;
public:
	GridRenderer2DComponent();
	GridRenderer2DComponent(int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, uint64_t parentGameObjectID);
	GridRenderer2DComponent(int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, glm::vec3& Color, uint64_t parentGameObjectID);
	GridRenderer2DComponent(int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, glm::vec4& Color, uint64_t parentGameObjectID);
	~GridRenderer2DComponent();

	void Update(const glm::mat4& GameObjectMatrix, float DeltaTime);
	void Destroy() override;
};

