#pragma once
#include <vector>
#include <memory>
#include "RenderedColorTexture.h"
#include "RenderedCubeMapTexture.h"

class SceneManager
{
private:
public:
	static std::shared_ptr<RenderedColorTexture>   BRDFTexture;
	static std::shared_ptr<RenderedCubeMapTexture> IrradianceCubeMap;
	static std::shared_ptr<RenderedCubeMapTexture> PrefilterCubeMap;
	static std::shared_ptr<RenderedCubeMapTexture> CubeMap;
};

