#pragma once
#include "GameObject.h"
#include "LineMesh3D.h"

class GridRenderer3D : public GameObject
{
private:
	std::shared_ptr<LineMesh3D> GridObject;

public:
	GridRenderer3D();
	GridRenderer3D(const std::string Name, int GridSize, float GridSpacing);
	GridRenderer3D(const std::string Name, int GridSizeX, int GridSizeY, int GridSizeZ, float GridSpacingX, float GridSpacingY, float GridSpacingZ);
	~GridRenderer3D();

	virtual void Update(float DeltaTime) override;
};

