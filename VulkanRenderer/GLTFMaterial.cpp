#include "GLTFMaterial.h"

uint64_t GLTFMaterial::MaterialIDCounter = 0;

GLTFMaterial::GLTFMaterial()
{
}

GLTFMaterial::GLTFMaterial(const std::string& materialName)
{
	MaterialName = materialName;
}

GLTFMaterial::~GLTFMaterial()
{
}

void GLTFMaterial::GenerateID()
{
	MaterialIDCounter++;
	MaterialID = MaterialIDCounter;
}
