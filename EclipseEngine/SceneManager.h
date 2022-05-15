#pragma once
#include <vector>
#include <memory>
#include "RenderedColorTexture.h"
#include "RenderedCubeMapTexture.h"

class SceneManager
{
private:
	static float PBRCubeMapSize;

public:
	static std::shared_ptr<RenderedColorTexture>   BRDFTexture;
	static std::shared_ptr<RenderedCubeMapTexture> IrradianceCubeMap;
	static std::shared_ptr<RenderedCubeMapTexture> PrefilterCubeMap;
	static std::shared_ptr<RenderedCubeMapTexture> CubeMap;

	static float GetPBRCubeMapSize() { return PBRCubeMapSize; }
};

