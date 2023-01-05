#include "GLTFFileLoader.h"
#include <iostream>

tinygltf::Model GLTFFileLoader::Loader(const char* filename)
{
	auto file = "C:/Users/dotha/Desktop/Vulkan-master/Vulkan/data/models/FlightHelmet/glTF/FlightHelmet.gltf";
	tinygltf::TinyGLTF loader;
	tinygltf::Model model;
	std::string error;
	std::string warning;

	loader.LoadASCIIFromFile(&model, &error, &warning, file);

	if (!error.empty())
	{
		std::cout << "Error: " << error << std::endl;
	}

	if (!warning.empty())
	{
		std::cout << "Warning: " << warning << std::endl;
	}

	return model;
}
