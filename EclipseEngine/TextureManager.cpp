#include "TextureManager.h"

VkSampler TextureManager::NullSampler = VK_NULL_HANDLE;
std::vector<std::shared_ptr<Texture2D>> TextureManager::Texture2DList;