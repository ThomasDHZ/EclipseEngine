#include "PointLight.h"
PointLight::PointLight() : Light<PointLightBuffer>()
{

}

PointLight::PointLight(PointLightBuffer light) : Light<PointLightBuffer>(light)
{
	LightBuffer.Update(light);

 //   ModelLoader loader{};
 //   loader.FilePath = "../Models/cube.obj";
 //   loader.MeshType = MeshTypeEnum::kMeshLightDebug;

	//debugMesh = std::make_shared<ModelRenderer>(ModelRenderer("PLight1", loader, LightBuffer.UniformDataInfo.position));
	//GameObjectManager::AddGameObject(debugMesh);
}
PointLight::~PointLight()
{
}

void PointLight::Update()
{
	Light::Update();
	//debugMesh->GameObjectPosition = LightBuffer.UniformDataInfo.position;
}

void PointLight::Destroy()
{
	Light::Destroy();
}

void PointLight::SetLightViewTexture(std::shared_ptr<RenderedCubeMapDepthTexture> lightViewTexture)
{
	LightViewTexture = lightViewTexture;
}
