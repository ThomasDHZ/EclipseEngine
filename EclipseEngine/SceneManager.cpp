#include "SceneManager.h"

float                                   SceneManager::PBRCubeMapSize = 512.0f;
float                                   SceneManager::PBRMaxMipLevel = 4.0f;
std::shared_ptr<Skybox>                 SceneManager::SkyboxMesh;
std::shared_ptr<RenderedColorTexture>   SceneManager::BRDFTexture = nullptr;
std::shared_ptr<RenderedCubeMapTexture> SceneManager::IrradianceCubeMap = nullptr;
std::shared_ptr<RenderedCubeMapTexture> SceneManager::PrefilterCubeMap = nullptr;
std::shared_ptr<RenderedCubeMapTexture> SceneManager::CubeMap = nullptr;