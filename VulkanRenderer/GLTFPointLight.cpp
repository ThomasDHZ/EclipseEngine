#include "GLTFPointLight.h"

GLTFPointLight::GLTFPointLight() : Light<GLTFPointLightBuffer>()
{

}

GLTFPointLight::GLTFPointLight(const std::string name, glm::vec3 Position, glm::vec3 DiffuseColor, float Intesity)
{
	LightName = name;
	LightBuffer.UniformDataInfo.diffuse = DiffuseColor;
	LightBuffer.UniformDataInfo.position = Position;
	LightBuffer.UniformDataInfo.intensity = Intesity;
	LightBuffer.Update();
}


GLTFPointLight::GLTFPointLight(GLTFPointLightBuffer light) : Light<GLTFPointLightBuffer>(light)
{
	LightBuffer.Update(light);

	//ModelLoader loader{};
	//loader.FilePath = "../Models/cube.obj";
	//loader.MeshType = MeshTypeEnum::kLightDebug;

	//debugMesh = std::make_shared<ModelRenderer>(ModelRenderer("PLight1", loader, LightBuffer.UniformDataInfo.position));
	//GameObjectManager::AddGameObject(debugMesh);
}

GLTFPointLight::~GLTFPointLight()
{
}

void GLTFPointLight::Update()
{
	Light::Update();
	//debugMesh->GameObjectPosition = LightBuffer.UniformDataInfo.position;
}

void GLTFPointLight::Destroy()
{
	Light::Destroy();
}

void GLTFPointLight::SetLightViewTexture(std::shared_ptr<RenderedCubeMapDepthTexture> lightViewTexture)
{
	LightViewTexture = lightViewTexture;
}
