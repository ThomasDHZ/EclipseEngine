#include "GLTFSunLight.h"

GLTFSunLight::GLTFSunLight() : Light<GLTFSunLightBuffer>()
{

}

GLTFSunLight::GLTFSunLight(const std::string name, glm::vec3 Position, glm::vec3 DiffuseColor, float Intesity)
{
	LightName = name;
	LightBuffer.UniformDataInfo.diffuse = DiffuseColor;
	LightBuffer.UniformDataInfo.position = Position;
	LightBuffer.UniformDataInfo.intensity = Intesity;
	LightBuffer.Update();
}


GLTFSunLight::GLTFSunLight(GLTFSunLightBuffer light) : Light<GLTFSunLightBuffer>(light)
{
	LightBuffer.Update(light);

	//ModelLoader loader{};
	//loader.FilePath = "../Models/cube.obj";
	//loader.MeshType = MeshTypeEnum::kLightDebug;

	//debugMesh = std::make_shared<ModelRenderer>(ModelRenderer("PLight1", loader, LightBuffer.UniformDataInfo.position));
	//GameObjectManager::AddGameObject(debugMesh);
}

GLTFSunLight::~GLTFSunLight()
{
}

void GLTFSunLight::Update()
{
	Light::Update();
	//debugMesh->GameObjectPosition = LightBuffer.UniformDataInfo.position;
}

void GLTFSunLight::Destroy()
{
	Light::Destroy();
}

void GLTFSunLight::SetLightViewTexture(std::shared_ptr<RenderedCubeMapDepthTexture> lightViewTexture)
{
	LightViewTexture = lightViewTexture;
}
