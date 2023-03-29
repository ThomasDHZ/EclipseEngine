#include "SceneManager.h"

SceneType								SceneManager::sceneType;
//float                                   SceneManager::PBRCubeMapSize = 256.0f;
//float									SceneManager::PreRenderedMapSize = 256.0f;
std::shared_ptr<SkyboxMesh>             SceneManager::skyboxMesh;
//std::shared_ptr<EnvironmentTexture>     SceneManager::environmentTexture = nullptr;
//std::shared_ptr<RenderedColorTexture>   SceneManager::BRDFTexture = nullptr;
//std::shared_ptr<RenderedCubeMapTexture> SceneManager::IrradianceMap = nullptr;
//std::shared_ptr<RenderedCubeMapTexture> SceneManager::PrefilterMap = nullptr;
//std::shared_ptr<RenderedCubeMapTexture> SceneManager::CubeMap = nullptr;
SkyBoxView								SceneManager::cubeMapInfo;
//SceneProperties							SceneManager::sceneProperites;
MusicPlayer								SceneManager::musicPlayer;
GaussianBlurSettings					SceneManager::bloomsettings;
bool									SceneManager::EditorModeFlag = true;
bool									SceneManager::RayTracingActive = false;
bool									SceneManager::HybridRendererActive = false;
 VkSampler SceneManager::NullSampler = VK_NULL_HANDLE;
 VkDescriptorImageInfo SceneManager::nullBufferInfo{};