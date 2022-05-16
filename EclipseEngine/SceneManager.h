#pragma once
#include <vector>
#include <memory>
#include "RenderedColorTexture.h"
#include "RenderedCubeMapTexture.h"
#include "Skybox.h"

class SceneManager
{
private:
	static float PBRCubeMapSize;

	static std::shared_ptr<Skybox> SkyboxMesh;

public:
	static std::shared_ptr<RenderedColorTexture>   BRDFTexture;
	static std::shared_ptr<RenderedCubeMapTexture> IrradianceCubeMap;
	static std::shared_ptr<RenderedCubeMapTexture> PrefilterCubeMap;
	static std::shared_ptr<RenderedCubeMapTexture> CubeMap;

	static void StartUp()
	{
		SkyboxMesh = std::make_shared<Skybox>();
		SkyboxMesh->StartUp();
	}

	static void Destory()
	{
		SkyboxMesh->Destory();

		if (BRDFTexture != nullptr)
		{
			BRDFTexture->Destroy();
		}
		if (IrradianceCubeMap != nullptr)
		{
			IrradianceCubeMap->Destroy();
		}
		if (PrefilterCubeMap != nullptr)
		{
			PrefilterCubeMap->Destroy();
		}
		if (CubeMap != nullptr)
		{
			CubeMap->Destroy();
		}
	}

	static std::shared_ptr<Skybox> GetSkyboxMesh() { return SkyboxMesh; };
	static float GetPBRCubeMapSize() { return PBRCubeMapSize; }
};

