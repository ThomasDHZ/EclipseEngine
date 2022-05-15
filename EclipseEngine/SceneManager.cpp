#include "SceneManager.h"

std::shared_ptr<RenderedColorTexture>   SceneManager::BRDFTexture = nullptr;
std::shared_ptr<RenderedCubeMapTexture> SceneManager::IrradianceCubeMap = nullptr;
std::shared_ptr<RenderedCubeMapTexture> SceneManager::PrefilterCubeMap = nullptr;
std::shared_ptr<RenderedCubeMapTexture> SceneManager::CubeMap = nullptr;