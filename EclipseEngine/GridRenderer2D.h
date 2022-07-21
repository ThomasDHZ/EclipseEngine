#pragma once
#include "GameObject.h"
#include "LineMesh2D.h"

class GridRenderer2D : public GameObject
{
private:
	std::shared_ptr<LineMesh2D> LineObject;

public:
	GridRenderer2D();
	GridRenderer2D(const std::string Name, int GridSize, float GridSpacing);
	GridRenderer2D(const std::string Name, int GridSize, float GridSpacing, glm::vec3& Color);
	GridRenderer2D(const std::string Name, int GridSize, float GridSpacing, glm::vec4& Color);
	GridRenderer2D(const std::string Name, int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY);
	GridRenderer2D(const std::string Name, int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, glm::vec3& Color);
	GridRenderer2D(const std::string Name, int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, glm::vec4& Color);
	~GridRenderer2D();

	virtual void Update(float DeltaTime) override;
};

