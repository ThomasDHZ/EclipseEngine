#include "Scene.h"
#include "EnvironmentTexture.h"
#include "MusicPlayer.h"
#include "LineMesh2D.h"
#include "BillBoardMeshRenderer.h"
#include "ReadableTexture.h"
#include "ParticalSystemRenderer.h"
#include "Model.h"

//std::vector<std::shared_ptr<GameObject>> GameObjectManager::objList;

Scene::Scene()
{
    SceneManager::sceneType = SceneType::kPBR;
   
    //MeshRendererManager::Update();
    //TopLevelAccelerationStructureManager::Update();

    StartUp();
    BuildRenderers();
}

Scene::~Scene()
{
}

void Scene::StartUp()
{
    GLTFSceneManager::StartUp();
    GLTFSceneManager::ActiveCamera = std::make_shared<PerspectiveCamera>(PerspectiveCamera("DefaultCamera", VulkanRenderer::GetSwapChainResolutionVec2(), glm::vec3(0.0f, 0.0f, 5.0f)));
	GLTFSceneManager::EnvironmentTexture = std::make_shared<EnvironmentTexture>(VulkanRenderer::OpenFile("/texture/hdr/Field-Path-Steinbacher-Street-Georgensgmünd-4K.hdr"), VK_FORMAT_R32G32B32A32_SFLOAT);

	environmentToCubeRenderPass.OneTimeDraw(4096.0f / 4);
	//GLTFSceneManager::ActiveCamera = std::make_shared<OrthographicCamera>(OrthographicCamera("DefaultCamera", VulkanRenderer::GetSwapChainResolutionVec2(), glm::vec3(0.0f, 0.0f, 5.0f)));


	/*std::shared_ptr<Material> GoldMaterial = std::make_shared<Material>(Material("GoldMaterial"));
	GoldMaterial->AlbedoMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/pbr/gold/albedo.png", TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_SRGB)));
	GoldMaterial->MetallicMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/pbr/gold/metallic.png", TextureTypeEnum::kMetallicTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	GoldMaterial->RoughnessMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/pbr/gold/roughness.png", TextureTypeEnum::kRoughnessTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	GoldMaterial->AmbientOcclusionMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/pbr/gold/ao.png", TextureTypeEnum::kAmbientOcclusionTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	GoldMaterial->NormalMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/pbr/gold/normal.png", TextureTypeEnum::kNormalTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	GLTFSceneManager::UpdateBufferIndex();
	GoldMaterial->UpdateBuffer();
	GLTFSceneManager::AddMaterial(GoldMaterial);*/

	std::shared_ptr<Material> IronMaterial = std::make_shared<Material>(Material("IronMaterial"));
	IronMaterial->SetIndexOfRefraction(1.31f);
	IronMaterial->SetTransmission(1.0f);
	IronMaterial->SetAlbedoMap(GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/rusted_iron/albedo.png"), TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_SRGB))));
	IronMaterial->SetMetallicMap(GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/rusted_iron/metallic.png"), TextureTypeEnum::kMetallicTextureMap, VK_FORMAT_R8G8B8A8_UNORM))));
	IronMaterial->SetRoughnessMap(GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/rusted_iron/roughness.png"), TextureTypeEnum::kRoughnessTextureMap, VK_FORMAT_R8G8B8A8_UNORM))));
	IronMaterial->SetAmbientOcclusionMap(GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/rusted_iron/ao.png"), TextureTypeEnum::kAmbientOcclusionTextureMap, VK_FORMAT_R8G8B8A8_UNORM))));
	IronMaterial->SetNormalMap(GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/rusted_iron/normal.png"), TextureTypeEnum::kNormalTextureMap, VK_FORMAT_R8G8B8A8_UNORM))));
	GLTFSceneManager::AddMaterial(IronMaterial);

	std::shared_ptr<Material> GoldMaterial = std::make_shared<Material>(Material("GoldMaterial"));
	GoldMaterial->SetAlbedoMap(GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/gold/albedo.png"), TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_SRGB))));
	GoldMaterial->SetMetallicMap(GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/gold/metallic.png"), TextureTypeEnum::kMetallicTextureMap, VK_FORMAT_R8G8B8A8_UNORM))));
	GoldMaterial->SetRoughnessMap(GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/gold/roughness.png"), TextureTypeEnum::kRoughnessTextureMap, VK_FORMAT_R8G8B8A8_UNORM))));
	GoldMaterial->SetAmbientOcclusionMap(GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/gold/ao.png"), TextureTypeEnum::kAmbientOcclusionTextureMap, VK_FORMAT_R8G8B8A8_UNORM))));
	GoldMaterial->SetNormalMap(GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/gold/normal.png"), TextureTypeEnum::kNormalTextureMap, VK_FORMAT_R8G8B8A8_UNORM))));
	GLTFSceneManager::AddMaterial(GoldMaterial);
	
	GLTFSceneManager::AddMeshGameObject3D("sponza", VulkanRenderer::OpenFile("/Models/sponza/sponza.gltf"));
	GLTFSceneManager::AddMeshGameObject3D("Sphere", "C:/Users/DHZ/source/repos/EclipseEngine/Models/GLTFIron/Iron.gltf", GoldMaterial);
	//GLTFSceneManager::AddMeshGameObject3D("Sci-fi", "C:/Users/DHZ/source/repos/EclipseEngine/Models/glTF-Sample-Models-master/2.0/SciFiHelmet/glTF/SciFiHelmet.gltf");

	//	/// <summary>
	///// 
	///// </summary>




	std::shared_ptr<Material> PlasticMaterial = std::make_shared<Material>(Material("PlasticMaterial"));
	PlasticMaterial->SetAlbedoMap(GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/plastic/albedo.png"), TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_SRGB))));
	PlasticMaterial->SetMetallicMap(GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/plastic/metallic.png"), TextureTypeEnum::kMetallicTextureMap, VK_FORMAT_R8G8B8A8_UNORM))));
	PlasticMaterial->SetRoughnessMap(GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/plastic/roughness.png"), TextureTypeEnum::kRoughnessTextureMap, VK_FORMAT_R8G8B8A8_UNORM))));
	PlasticMaterial->SetAmbientOcclusionMap(GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/plastic/ao.png"), TextureTypeEnum::kAmbientOcclusionTextureMap, VK_FORMAT_R8G8B8A8_UNORM))));
	PlasticMaterial->SetNormalMap(GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/plastic/normal.png"), TextureTypeEnum::kNormalTextureMap, VK_FORMAT_R8G8B8A8_UNORM))));
	GLTFSceneManager::UpdateBufferIndex();
	GLTFSceneManager::AddMaterial(PlasticMaterial);

	std::shared_ptr<Material> WallMaterial = std::make_shared<Material>(Material("WallMaterial"));
	WallMaterial->SetAlbedoMap(GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/wall/albedo.png"), TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_SRGB))));
	WallMaterial->SetMetallicMap(GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/wall/metallic.png"), TextureTypeEnum::kMetallicTextureMap, VK_FORMAT_R8G8B8A8_UNORM))));
	WallMaterial->SetRoughnessMap(GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/wall/roughness.png"), TextureTypeEnum::kRoughnessTextureMap, VK_FORMAT_R8G8B8A8_UNORM))));
	WallMaterial->SetAmbientOcclusionMap(GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/wall/ao.png"), TextureTypeEnum::kAmbientOcclusionTextureMap, VK_FORMAT_R8G8B8A8_UNORM))));
	WallMaterial->SetNormalMap(GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/wall/normal.png"), TextureTypeEnum::kNormalTextureMap, VK_FORMAT_R8G8B8A8_UNORM))));
	GLTFSceneManager::UpdateBufferIndex();
	GLTFSceneManager::AddMaterial(WallMaterial);

	std::shared_ptr<Material> GrassMaterial = std::make_shared<Material>(Material("GrassMaterial"));
	GrassMaterial->SetAlbedoMap(GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/grass/albedo.png"), TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_SRGB))));
	GrassMaterial->SetMetallicMap(GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/grass/metallic.png"), TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_UNORM))));
	GrassMaterial->SetRoughnessMap(GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/grass/roughness.png"), TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_UNORM))));
	GrassMaterial->SetAmbientOcclusionMap(GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/grass/ao.png"), TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_UNORM))));
	GrassMaterial->SetNormalMap(GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/grass/normal.png"), TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_UNORM))));
	GLTFSceneManager::UpdateBufferIndex();
	GLTFSceneManager::AddMaterial(GrassMaterial);


	auto b = "C:/Users/DHZ/source/repos/EclipseEngine/Models/GLTFIron/Iron.gltf";

	GLTFInstancingDataStruct instance = {};
	std::vector<std::shared_ptr<Material>> instanceMaterialList;
	instanceMaterialList.emplace_back(IronMaterial);
	instanceMaterialList.emplace_back(PlasticMaterial);
	instanceMaterialList.emplace_back(WallMaterial);
	instanceMaterialList.emplace_back(GoldMaterial);
	instanceMaterialList.emplace_back(GrassMaterial);
	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 5; y++)
		{
			for (int z = 0; z < 5; z++)
			{
				GLTFInstanceMeshDataStruct instanceMeshDataStruct = {};
				instanceMeshDataStruct.InstancePosition = glm::vec3(float(x * 3.0f), float(y * 3.0f), float(z * 3.0f));
				instance.InstanceMeshDataList.emplace_back(instanceMeshDataStruct);
				instance.MaterialList = instanceMaterialList;
			}
		}
	}
	GLTFSceneManager::AddInstancedGameObject3D("InstanceTest", b, instance);


	/// <summary>
	/// 
	/// </summary>

	//std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>(Material("TestMaterial"));
	//GLTFSceneManager::UpdateBufferIndex();
	//spriteMaterial->UpdateBuffer();
	//GLTFSceneManager::AddMaterial(spriteMaterial);

	//spriteMaterial->AlbedoMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/VulkanGraphics/texture/Brick_diffuseOriginal.bmp", TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_SRGB)));

	//const std::string asdf = "sprite";
	//GLTFSceneManager::AddSpriteGameObject3D(asdf, spriteMaterial);

	/// <summary>
	/// 
	/// </summary>

	//int width = 500;
	//int height = 500;
	//float length = 10.0f;
	//float radius = 0.5f;
	//std::vector<LineVertex3D> VertexList;
	//for (uint32_t y = 0; y < height; y++)
	//{
	//	for (uint32_t x = 0; x < width; x++)
	//	{
	//		glm::vec2 coord = { (float)x / width, (float)y / height };
	//		coord = coord * 2.0f - 1.0f;

	//		uint8_t r = (uint8_t)(coord.x * 255.0f);
	//		uint8_t g = (uint8_t)(coord.y * 255.0f);

	//		glm::vec3 rayOrigin(0.0f, 0.0f, 2.0f);
	//		glm::vec3 rayDirection(coord.x, coord.y, -1.0f);

	//		float a = glm::dot(rayDirection, rayDirection);
	//		float b = 2.0f * glm::dot(rayOrigin, rayDirection);
	//		float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

	//		//Quadratic forumla discriminat
	//		//b^2 - 4ac;

	//		float discriminant = (b * b) - 4.0f * a * c;

	//		if (discriminant >= 0.0f)
	//		{
	//			VertexList.emplace_back(LineVertex3D(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec4(1.0f, 0.0f, 0.8f, 1.0f)));
	//			VertexList.emplace_back(LineVertex3D(glm::vec3(coord.x, coord.y, -1.0f), glm::vec4(1.0f, 0.0f, 0.8, 1.0f)));
	//		}
	//		else
	//		{
	//			VertexList.emplace_back(LineVertex3D(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec4(0.0f, 0.0f, 01.0f, 0.02f)));
	//			VertexList.emplace_back(LineVertex3D(glm::vec3(coord.x, coord.y, -1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 0.02f)));
	//		}
	//	}
	//}
	//GLTFSceneManager::AddLineGameObject3D("Lines", VertexList);

	//std::vector<LineVertex3D> LightView;
	//LightView.emplace_back(LineVertex3D(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	//LightView.emplace_back(LineVertex3D(glm::vec3(3.0f, 3.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	//LightView.emplace_back(LineVertex3D(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
	//LightView.emplace_back(LineVertex3D(glm::vec3(3.0f, -3.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));

	//LightView.emplace_back(LineVertex3D(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
	//LightView.emplace_back(LineVertex3D(glm::vec3(0.0f, 3.0f, 3.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));

	//LightView.emplace_back(LineVertex3D(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));
	//LightView.emplace_back(LineVertex3D(glm::vec3(0.0f, -3.0f, 3.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));

	//GLTFSceneManager::AddLineGameObject3D("Lines", LightView);

//GLTFSceneManager::AddDirectionalLight(std::make_shared<DirectionalLight>(DirectionalLight("sdf", glm::vec3(0.01f), glm::vec3(1.0f), 30.8f)));
	/*GLTFSceneManager::AddPointLight(std::make_shared<GLTFPointLight>(GLTFPointLight("sdf", glm::vec3(0.01f), glm::vec3(1.0f), 30.8f, 1.0f))); */
}

void Scene::Update()
{
    if (VulkanRenderer::UpdateRendererFlag)
    {
        BuildRenderers();
    }

    SceneManager::Update();
	GLTFSceneManager::Update();
	if (GLTFSceneManager::RaytraceModeFlag)
	{
		rayTraceRenderer.Update();
	}
	else
	{
		pbrRenderer.Update();
	}
   
    //spriteRenderer.Update();

}

void Scene::ImGuiUpdate()
{
    SceneManager::bloomsettings.blurScale;
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::Checkbox("Wireframe Mode", &GLTFSceneManager::WireframeModeFlag);
    if (GraphicsDevice::IsRayTracingFeatureActive())
    {
		ChangeRendererFlag = ImGui::Checkbox("RayTrace Mode", &SceneManager::RayTracingActive);
        ImGui::Checkbox("Hybrid Mode", &SceneManager::HybridRendererActive);

		if (ChangeRendererFlag)
		{
			GLTFSceneManager::RaytraceModeFlag = !GLTFSceneManager::RaytraceModeFlag;
			ChangeRendererFlag = false;
			VulkanRenderer::UpdateRendererFlag = true;
		}
    }

   // spriteRenderer.ImGuiUpdate();
	if (GLTFSceneManager::RaytraceModeFlag)
	{
		rayTraceRenderer.ImGuiUpdate();
	}
	else
	{
		pbrRenderer.ImGuiUpdate();
	}


    SceneManager::ImGuiSceneHierarchy();
   // MeshRendererManager::GUIUpdate();
    VulkanRenderer::ImGUILayerActive = ImGui::IsAnyItemHovered() || ImGui::IsWindowHovered() || ImGui::IsWindowFocused();
}

void Scene::BuildRenderers()
{
    //MeshRendererManager::Update();
    //spriteRenderer.BuildRenderer();
	if (GLTFSceneManager::RaytraceModeFlag)
	{
		rayTraceRenderer.BuildRenderer();
	}
	else
	{
		pbrRenderer.BuildRenderer();
	}

    InterfaceRenderPass::RebuildSwapChain();
    VulkanRenderer::UpdateRendererFlag = false;
}

void Scene::Draw()
{
    std::vector<VkCommandBuffer> CommandBufferSubmitList;

    VkResult result = VulkanRenderer::StartDraw();
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        BuildRenderers();
        return;
    }

	if (GLTFSceneManager::RaytraceModeFlag)
	{
		rayTraceRenderer.Draw(CommandBufferSubmitList);
	}
	else
	{
		pbrRenderer.Draw(CommandBufferSubmitList);
	}
    //spriteRenderer.Draw(CommandBufferSubmitList);
   
    InterfaceRenderPass::Draw();
    CommandBufferSubmitList.emplace_back(InterfaceRenderPass::ImGuiCommandBuffers[VulkanRenderer::GetCMDIndex()]);

    result = VulkanRenderer::SubmitDraw(CommandBufferSubmitList);
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        BuildRenderers();
        return;
    }
}

void Scene::Destroy()
{
	GLTFSceneManager::Destroy();
	environmentToCubeRenderPass.Destroy();
   // GameObjectManager::Destroy();
    rayTraceRenderer.Destroy();
    pbrRenderer.Destroy();
    //spriteRenderer.Destroy();
}
