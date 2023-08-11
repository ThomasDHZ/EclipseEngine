#include "PointLight.h"

PointLight::PointLight() : Light<GLTFPointLightBuffer>()
{

}

PointLight::PointLight(const std::string name, glm::vec3 Position, glm::vec3 DiffuseColor, float Intesity, float Radius) : Light<GLTFPointLightBuffer>(LightTypeEnum::kPointLight)
{
	LightName = name;
	LightBuffer.UniformDataInfo.diffuse = DiffuseColor;
	LightBuffer.UniformDataInfo.position = Position;
	LightBuffer.UniformDataInfo.intensity = Intesity;
	LightBuffer.UniformDataInfo.radius = Radius;
	LightBuffer.Update();
}


PointLight::PointLight(GLTFPointLightBuffer light) : Light<GLTFPointLightBuffer>(LightTypeEnum::kPointLight, light)
{
	LightBuffer.Update(light);

	//ModelLoader loader{};
	//loader.FilePath = "../Models/cube.obj";
	//loader.MeshType = MeshTypeEnum::kLightDebug;

	//debugMesh = std::make_shared<ModelRenderer>(ModelRenderer("PLight1", loader, LightBuffer.UniformDataInfo.position));
	//GameObjectManager::AddGameObject(debugMesh);
}

PointLight::~PointLight()
{
}

void PointLight::Update()
{
	glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10000.0f);

	cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[0] = ProjectionMatrix * glm::lookAt(LightBuffer.UniformDataInfo.position, LightBuffer.UniformDataInfo.position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[1] = ProjectionMatrix * glm::lookAt(LightBuffer.UniformDataInfo.position, LightBuffer.UniformDataInfo.position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[2] = ProjectionMatrix * glm::lookAt(LightBuffer.UniformDataInfo.position, LightBuffer.UniformDataInfo.position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[3] = ProjectionMatrix * glm::lookAt(LightBuffer.UniformDataInfo.position, LightBuffer.UniformDataInfo.position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[4] = ProjectionMatrix * glm::lookAt(LightBuffer.UniformDataInfo.position, LightBuffer.UniformDataInfo.position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[5] = ProjectionMatrix * glm::lookAt(LightBuffer.UniformDataInfo.position, LightBuffer.UniformDataInfo.position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	cubeMapSampler.Update();

	Light::Update();
}

void PointLight::Destroy()
{
	Light::Destroy();
	cubeMapSampler.Destroy();
	for (int x = 0; x < 6; x++)
	{
		CubeMapSide[x]->Destroy();
	}
}

void PointLight::SetLightViewTexture(std::shared_ptr<RenderedCubeMapDepthTexture> lightViewTexture)
{
	LightViewTexture = lightViewTexture;
}

