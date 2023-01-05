#pragma once
#include <tinygltf/tiny_gltf.h>

class GLTFFileLoader
{
public:
	static tinygltf::Model Loader(const char* filename);
};

