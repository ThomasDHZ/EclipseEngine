#include "GLTFRenderer2D.h"
#include "SpriteGameObject2D.h"
#include "OrthographicCamera.h"

GLTFRenderer2D::GLTFRenderer2D()
{
}

GLTFRenderer2D::~GLTFRenderer2D()
{
}

void GLTFRenderer2D::BuildRenderer()
{
	GLTFSceneManager::StartUp();
	GLTFSceneManager::ActiveCamera = std::make_shared<OrthographicCamera>(OrthographicCamera("camera", VulkanRenderer::GetSwapChainResolutionVec2().x, VulkanRenderer::GetSwapChainResolutionVec2().y, 10.5f));

	std::string mario = "C:/Users/dotha/source/repos/VulkanGraphics/texture/Brick_diffuseOriginal.bmp";
	std::string mario2 = "C:/Users/dotha/source/repos/VulkanGraphics/texture/Brick_diffuseOriginal.bmp";

	std::shared_ptr<Material> material = std::make_shared<Material>(Material("TestMaterial"));
	material->AlbedoMap = std::make_shared<Texture2D>(Texture2D(mario, TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_SRGB));
	//material->AlphaMap = std::make_shared<Texture2D>(Texture2D(mario2, TextureTypeEnum::kAlphaTextureMap, VK_FORMAT_R8G8B8A8_UNORM));

	GLTFSceneManager::AddSpriteGameObject2D("Testobject", material, glm::vec2(0.0f), 0);
	GLTFSceneManager::AddSpriteGameObject2D("Testobject2", material, glm::vec2(1.0f, 0.0f), 0);
	GLTFSceneManager::AddSpriteGameObject2D("Testobject3", material, glm::vec2(2.0f, 0.0f), 0);
	GLTFSceneManager::AddSpriteGameObject2D("Testobject4", material, glm::vec2(3.0f, 0.0f), 0);

	renderPass2D.BuildRenderPass();
	frameBufferRenderPass.BuildRenderPass(renderPass2D.renderedTexture);
}

void GLTFRenderer2D::Update()
{
	GLTFSceneManager::Update();
}

void GLTFRenderer2D::ImGuiUpdate()
{
}

void GLTFRenderer2D::Draw(std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	CommandBufferSubmitList.emplace_back(renderPass2D.Draw(GLTFSceneManager::GameObjectList));
	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.Draw());
}

void GLTFRenderer2D::Destroy()
{
	GLTFSceneManager::Destroy();
	renderPass2D.Destroy();
	frameBufferRenderPass.Destroy();
}
