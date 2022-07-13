#pragma once
#include "Component.h"
#include "Mesh.h"
class GridRendererComponent : public Component
{
private:
	std::shared_ptr<Mesh> Grid;
public:
	GridRendererComponent();
	GridRendererComponent(int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, uint64_t parentGameObjectID);
	~GridRendererComponent();

	void Update(const glm::mat4& GameObjectMatrix, float DeltaTime);
	void Destroy() override;
};

