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
	GLTFSceneManager::ActiveCamera = std::make_shared<OrthographicCamera>(OrthographicCamera("camera", 8.49414635f, 5.79752779f, 4.80515528f));

	std::shared_ptr<Material> material3 = std::make_shared<Material>(Material("MetalManTiles"));
	//material3->AlbedoMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/MetalMan.png", TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_SRGB)));
	//material3->CheckPaletteSwapColorMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/MetalMan-Recovered.png", TextureTypeEnum::kPaletteRotationMap, VK_FORMAT_R8G8B8A8_UNORM)));
	//material3->PaletteSwapPixelColorMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/MetalMapPaletteRotation.bmp", TextureTypeEnum::kPaletteRotationMap, VK_FORMAT_R8G8B8A8_UNORM)));
	//
	std::shared_ptr<Material> material = std::make_shared<Material>(Material("TestMaterial"));
	//material->AlbedoMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/VulkanGraphics/texture/Brick_diffuseOriginal.bmp", TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_SRGB)));

	//std::shared_ptr<Material> material2 = std::make_shared<Material>(Material("Layout0"));
	//material2->AlbedoMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/LayerTestlayer0.png", TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_SRGB)));


	//material->AlphaMap = std::make_shared<Texture2D>(Texture2D(mario2, TextureTypeEnum::kAlphaTextureMap, VK_FORMAT_R8G8B8A8_UNORM));
	
	material->UpdateBuffer();
	GLTFSceneManager::AddMaterial(material);
	//material2->UpdateBuffer();
	//GLTFSceneManager::AddMaterial(material2);
	material3->UpdateBuffer();
	GLTFSceneManager::AddMaterial(material3);
	GLTFSceneManager::UpdateBufferIndex();

	std::vector<std::shared_ptr<Material>> materialList;
	materialList.emplace_back(material3);
	 GLTFSceneManager::AddLevelGameObject("Testobject", glm::ivec2(16, 16), glm::ivec2(500,300), materialList, 0);
	
	 //std::vector<std::shared_ptr<Material>> materialList2;
	 //materialList2.emplace_back(material3);
	 //GLTFSceneManager::AddLevelGameObject("Testobject2", glm::ivec2(1, 1), materialList2, 2);

	//for(int x = 0; x < 100; x++)
	//{
	//	for (int y = 0; y < 100; y++)
	//	{
	//		GLTFSceneManager::AddSpriteGameObject2D("Testobject3", material, glm::vec2((float)x, (float)y), 1);
	//	}
	//}

	GLTFSceneManager::AddSpriteGameObject2D("Testobject4", material, glm::vec2(2.5f, 0.0f), 1);
	//GLTFSceneManager::AddSpriteGameObject2D("Testobject5", material2, glm::vec2(0.0f, 1.0f), 1);
	//GLTFSceneManager::AddSpriteGameObject2D("Testobject6", material3, glm::vec2(1.0f, 1.0f), 1);
	//GLTFSceneManager::AddSpriteGameObject2D("Testobject7", material, glm::vec2(2.0f, 1.0f), 1);
	//GLTFSceneManager::AddSpriteGameObject2D("Testobject8", material2, glm::vec2(3.0f, 1.0f), 1);
	//GLTFSceneManager::AddLineGameObject2D("sponza", glm::vec2(0.0f), glm::vec2(25.0f), 0);
	//GLTFSceneManager::AddGridGameObject2D("sponza", 50, 50, 1.0f, 1.0f, 0);
	//GLTFSceneManager::AddSquareGameObject2D("sponza", 5, glm::vec4(1.0, 0.0f, 0.0f, 1.0f), 0);
	//GLTFSceneManager::AddSquareGameObject2D("sponza", 4, glm::vec4(1.0, 0.0f, 0.0f, 1.0f), 0);



	LevelRenderPass2D.BuildRenderPass();

	std::vector<std::shared_ptr<RenderedColorTexture>> textureList;
	textureList.emplace_back(LevelRenderPass2D.bloomTexture);

	BloomRenderPass.BuildRenderPass(textureList);
	bloomCombineRenderPass.BuildRenderPass(BloomRenderPass.BlurredTextureList);
	frameBufferRenderPass.BuildRenderPass(LevelRenderPass2D.renderedTexture, bloomCombineRenderPass.BloomTexture);

	GLTFSceneManager::Update();
	VulkanRenderer::UpdateRendererFlag = true;
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
	CommandBufferSubmitList.emplace_back(LevelRenderPass2D.Draw(GLTFSceneManager::Get2DGameObjects()));
	CommandBufferSubmitList.emplace_back(BloomRenderPass.Draw());
	CommandBufferSubmitList.emplace_back(bloomCombineRenderPass.Draw());
	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.Draw());
}

void GLTFRenderer2D::Destroy()
{
	GLTFSceneManager::Destroy();
	LevelRenderPass2D.Destroy();
	frameBufferRenderPass.Destroy();
}
