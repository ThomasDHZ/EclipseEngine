#pragma once
#include "GLTFFileLoader.h"
#include "GLTFModel.h"

class GLTFScene
{
private:
	GLTFModel model;

public:

	GLTFScene(const char* filename, uint32_t vertexSize);
	~GLTFScene();

	GLTFModelData GetModelData() { return model.GetModelData(); }
};

