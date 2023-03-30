#include "SceneManager.h"

SceneType								SceneManager::sceneType;
MusicPlayer								SceneManager::musicPlayer;
GaussianBlurSettings					SceneManager::bloomsettings;
bool									SceneManager::EditorModeFlag = true;
bool									SceneManager::RayTracingActive = false;
bool									SceneManager::HybridRendererActive = false;
 VkSampler SceneManager::NullSampler = VK_NULL_HANDLE;
 VkDescriptorImageInfo SceneManager::nullBufferInfo{};