#include "GLTFPointLight.h"

GLTFPointLight::GLTFPointLight() : Light<GLTFPointLightBuffer>()
{

}

GLTFPointLight::GLTFPointLight(const std::string name, glm::vec3 Position, glm::vec3 DiffuseColor, float Intesity, float Radius)
{
	LightName = name;
	LightBuffer.UniformDataInfo.diffuse = DiffuseColor;
	LightBuffer.UniformDataInfo.position = Position;
	LightBuffer.UniformDataInfo.intensity = Intesity;
	LightBuffer.UniformDataInfo.radius = Radius;
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
