#include "SunLight.h"

SunLight::SunLight() : Light<GLTFSunLightBuffer>()
{

}

SunLight::SunLight(const std::string name, glm::vec3 Position, glm::vec3 DiffuseColor, float Intesity) : Light<GLTFSunLightBuffer>(LightTypeEnum::kSunLight)
{
	LightName = name;
	LightBuffer.UniformDataInfo.diffuse = DiffuseColor;
	LightBuffer.UniformDataInfo.position = Position;
	LightBuffer.UniformDataInfo.intensity = Intesity;
	LightBuffer.Update();
}


SunLight::SunLight(GLTFSunLightBuffer light) : Light<GLTFSunLightBuffer>(LightTypeEnum::kSunLight, light)
{
	LightBuffer.Update(light);

	//ModelLoader loader{};
	//loader.FilePath = "../Models/cube.obj";
	//loader.MeshType = MeshTypeEnum::kLightDebug;

	//debugMesh = std::make_shared<ModelRenderer>(ModelRenderer("PLight1", loader, LightBuffer.UniformDataInfo.position));
	//GameObjectManager::AddGameObject(debugMesh);
}

SunLight::~SunLight()
{
}

void SunLight::Update()
{
	Light::Update();
	//debugMesh->GameObjectPosition = LightBuffer.UniformDataInfo.position;
}

void SunLight::Destroy()
{
	Light::Destroy();
}

void SunLight::SetLightViewTexture(std::shared_ptr<RenderedCubeMapDepthTexture> lightViewTexture)
{
	LightViewTexture = lightViewTexture;
}
