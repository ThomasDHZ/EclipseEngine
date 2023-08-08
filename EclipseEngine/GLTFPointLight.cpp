#include "GLTFPointLight.h"

GLTFPointLight::GLTFPointLight() : Light<GLTFPointLightBuffer>()
{

}

GLTFPointLight::GLTFPointLight(const std::string name, glm::vec3 Position, glm::vec3 DiffuseColor, float Intesity, float Radius) : Light<GLTFPointLightBuffer>(LightTypeEnum::kPointLight)
{
	LightName = name;
	LightBuffer.UniformDataInfo.diffuse = DiffuseColor;
	LightBuffer.UniformDataInfo.position = Position;
	LightBuffer.UniformDataInfo.intensity = Intesity;
	LightBuffer.UniformDataInfo.radius = Radius;
	LightBuffer.Update();
}


GLTFPointLight::GLTFPointLight(GLTFPointLightBuffer light) : Light<GLTFPointLightBuffer>(LightTypeEnum::kPointLight, light)
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
	glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10000.0f);

	cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[0] = ProjectionMatrix * glm::lookAt(LightBuffer.UniformDataInfo.position, LightBuffer.UniformDataInfo.position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[1] = ProjectionMatrix * glm::lookAt(LightBuffer.UniformDataInfo.position, LightBuffer.UniformDataInfo.position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[2] = ProjectionMatrix * glm::lookAt(LightBuffer.UniformDataInfo.position, LightBuffer.UniformDataInfo.position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[3] = ProjectionMatrix * glm::lookAt(LightBuffer.UniformDataInfo.position, LightBuffer.UniformDataInfo.position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[4] = ProjectionMatrix * glm::lookAt(LightBuffer.UniformDataInfo.position, LightBuffer.UniformDataInfo.position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[5] = ProjectionMatrix * glm::lookAt(LightBuffer.UniformDataInfo.position, LightBuffer.UniformDataInfo.position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	cubeMapSampler.Update();

	Light::Update();

	//ProjectionMatrix = glm::ortho(LeftRight.x, LeftRight.y, TopBottom.x, TopBottom.y, NearFar.x, NearFar.y);
	//ProjectionMatrix[1][1] *= -1;

	//ViewMatrix = glm::lookAt(LightBuffer.UniformDataInfo.position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	//LightBuffer.UniformDataInfo.LightSpaceMatrix = ProjectionMatrix * ViewMatrix;
}

void GLTFPointLight::Destroy()
{
	Light::Destroy();
}

void GLTFPointLight::SetLightViewTexture(std::shared_ptr<RenderedCubeMapDepthTexture> lightViewTexture)
{
	LightViewTexture = lightViewTexture;
}

