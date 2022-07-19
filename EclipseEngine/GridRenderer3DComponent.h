#pragma once
#include "Component.h"
#include "Model.h"
class GridRenderer3DComponent : public Component
{
private:
	std::shared_ptr<Model> Grid;
public:
	GridRenderer3DComponent();
	GridRenderer3DComponent(int GridSize, float GridSpacingX, uint64_t parentGameObjectID);
	GridRenderer3DComponent(int GridSizeX, int GridSizeY, int GridSizeZ, float GridSpacingX, float GridSpacingY, float GridSpacingZ, uint64_t parentGameObjectID);
	~GridRenderer3DComponent();

	void Update(const glm::mat4& GameObjectMatrix, float DeltaTime);
	void Destroy() override;
};