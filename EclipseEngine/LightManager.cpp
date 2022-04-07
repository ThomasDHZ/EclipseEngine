#include "LightManager.h"
 std::vector<std::shared_ptr<DirectionalLight>> LightManager::DirectionalLightList;
 std::vector<std::shared_ptr<PointLight>> LightManager::PointLightList;
 std::vector<std::shared_ptr<SpotLight>> LightManager::SpotLightList;