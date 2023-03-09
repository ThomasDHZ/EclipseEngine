#pragma once
#include "GLTFImporter.h"

class GLTFModel : public GLTFImporter
{
private:
	void LoadModel(tinygltf::Model model);

public: 
	GLTFModel();
	GLTFModel(const char* filename);
	~GLTFModel();

	GLTFModelData GetModelData() { return data; }
};

