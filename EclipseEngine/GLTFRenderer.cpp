#include "GLTFRenderer.h"
#include "SpriteGameObject3D.h"

GLTFRenderer::GLTFRenderer()
{
}

GLTFRenderer::~GLTFRenderer()
{
}

void GLTFRenderer::BuildRenderer()
{
	GLTFSceneManager::StartUp();
	GLTFSceneManager::ActiveCamera = std::make_shared<PerspectiveCamera>(PerspectiveCamera("DefaultCamera", VulkanRenderer::GetSwapChainResolutionVec2(), glm::vec3(0.0f, 0.0f, 5.0f)));

	/// <summary>
	/// 
	/// </summary>


	auto a = "C:/Users/dotha/source/repos/EclipseEngine/Models/GLTFSponza/Sponza.gltf";
	auto b = "C:/Users/dotha/source/repos/EclipseEngine/Models/GLTFIron/Iron.gltf";
	auto d = "C:/Users/dotha/source/repos/EclipseEngine/Models/GLTFGold/Gold.gltf";
	auto c = "C:/Users/dotha/source/repos/EclipseEngine/Models/glTF-Sample-Models-master/2.0/SciFiHelmet/glTF/SciFiHelmet.gltf";


	std::shared_ptr<Material> GoldMaterial = std::make_shared<Material>(Material("GoldMaterial"));
	GoldMaterial->AlbedoMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/pbr/gold/albedo.png", TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_SRGB)));
	GoldMaterial->MetallicMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/pbr/gold/metallic.png", TextureTypeEnum::kMetallicTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	GoldMaterial->RoughnessMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/pbr/gold/roughness.png", TextureTypeEnum::kRoughnessTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	GoldMaterial->AmbientOcclusionMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/pbr/gold/ao.png", TextureTypeEnum::kAmbientOcclusionTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	GoldMaterial->NormalMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/pbr/gold/normal.png", TextureTypeEnum::kNormalTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	GLTFSceneManager::UpdateBufferIndex();
	GoldMaterial->UpdateBuffer();
	GLTFSceneManager::AddMaterial(GoldMaterial);

	GLTFSceneManager::AddMeshGameObject3D("sponza", a);
	GLTFSceneManager::AddMeshGameObject3D("Sphere", d, GoldMaterial);
	//GLTFSceneManager::AddMeshGameObject3D("Sci-fi", c);

	//	/// <summary>
	///// 
	///// </summary>


	//std::shared_ptr<Material> IronMaterial = std::make_shared<Material>(Material("IronMaterial"));
	//IronMaterial->AlbedoMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/pbr/rusted_iron/albedo.png", TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_SRGB)));
	//IronMaterial->MetallicMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/pbr/rusted_iron/metallic.png", TextureTypeEnum::kMetallicTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	//IronMaterial->RoughnessMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/pbr/rusted_iron/roughness.png", TextureTypeEnum::kRoughnessTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	//IronMaterial->AmbientOcclusionMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/pbr/rusted_iron/ao.png", TextureTypeEnum::kAmbientOcclusionTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	//IronMaterial->NormalMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/pbr/rusted_iron/normal.png", TextureTypeEnum::kNormalTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	//GLTFSceneManager::UpdateBufferIndex();
	//IronMaterial->UpdateBuffer();
	//GLTFSceneManager::AddMaterial(IronMaterial);

	///**/std::shared_ptr<Material> PlasticMaterial = std::make_shared<Material>(Material("PlasticMaterial"));
	//PlasticMaterial->AlbedoMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/pbr/plastic/albedo.png", TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_SRGB)));
	//PlasticMaterial->MetallicMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/pbr/plastic/metallic.png", TextureTypeEnum::kMetallicTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	//PlasticMaterial->RoughnessMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/pbr/plastic/roughness.png", TextureTypeEnum::kRoughnessTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	//PlasticMaterial->AmbientOcclusionMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/pbr/plastic/ao.png", TextureTypeEnum::kAmbientOcclusionTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	//PlasticMaterial->NormalMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/pbr/plastic/normal.png", TextureTypeEnum::kNormalTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	//GLTFSceneManager::UpdateBufferIndex();
	//PlasticMaterial->UpdateBuffer();
	//GLTFSceneManager::AddMaterial(PlasticMaterial);

	//std::shared_ptr<Material> WallMaterial = std::make_shared<Material>(Material("WallMaterial"));
	//WallMaterial->AlbedoMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/pbr/wall/albedo.png", TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_SRGB)));
	//WallMaterial->MetallicMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/pbr/wall/metallic.png", TextureTypeEnum::kMetallicTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	//WallMaterial->RoughnessMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/pbr/wall/roughness.png", TextureTypeEnum::kRoughnessTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	//WallMaterial->AmbientOcclusionMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/pbr/wall/ao.png", TextureTypeEnum::kAmbientOcclusionTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	//WallMaterial->NormalMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/pbr/wall/normal.png", TextureTypeEnum::kNormalTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	//GLTFSceneManager::UpdateBufferIndex();
	//WallMaterial->UpdateBuffer();
	//GLTFSceneManager::AddMaterial(WallMaterial);

	//std::shared_ptr<Material> GrassMaterial = std::make_shared<Material>(Material("GrassMaterial"));
	//GrassMaterial->AlbedoMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/pbr/grass/albedo.png", TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_SRGB)));
	//GrassMaterial->MetallicMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/pbr/grass/metallic.png", TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	//GrassMaterial->RoughnessMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/pbr/grass/roughness.png", TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	//GrassMaterial->AmbientOcclusionMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/pbr/grass/ao.png", TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	//GrassMaterial->NormalMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/pbr/grass/normal.png", TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	//GLTFSceneManager::UpdateBufferIndex();
	//GrassMaterial->UpdateBuffer();
	//GLTFSceneManager::AddMaterial(GrassMaterial);

	//GLTFInstancingDataStruct instance = {};
	//std::vector<std::shared_ptr<Material>> instanceMaterialList;
	//instanceMaterialList.emplace_back(IronMaterial);
	//instanceMaterialList.emplace_back(PlasticMaterial);
	//instanceMaterialList.emplace_back(WallMaterial);
	//instanceMaterialList.emplace_back(GoldMaterial);
	//instanceMaterialList.emplace_back(GrassMaterial);
	//for (int x = 0; x < 5; x++)
	//{
	//	for (int y = 0; y < 5; y++)
	//	{
	//		for (int z = 0; z < 5; z++)
	//		{
	//			GLTFInstanceMeshDataStruct instanceMeshDataStruct = {};
	//			instanceMeshDataStruct.InstancePosition = glm::vec3(float(x * 3.0f), float(y * 3.0f), float(z * 3.0f));
	//			instance.InstanceMeshDataList.emplace_back(instanceMeshDataStruct);
	//			instance.MaterialList = instanceMaterialList;
	//		}
	//	}
	//}
	//GLTFSceneManager::AddInstancedGameObject3D("InstanceTest", b, instance);


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

	//GLTFSceneManager::AddDirectionalLight(std::make_shared<GLTFDirectionalLight>(GLTFDirectionalLight("sdf", glm::vec3(0.01f), glm::vec3(1.0f), 30.8f)));

	GLTFSceneManager::sceneProperites.PBRMaxMipLevel = static_cast<uint32_t>(std::floor(std::log2(std::max(GLTFSceneManager::GetPreRenderedMapSize(), GLTFSceneManager::GetPreRenderedMapSize())))) + 1;
	GLTFSceneManager::EnvironmentTexture = std::make_shared<EnvironmentTexture>("../texture/hdr/newport_loft.hdr", VK_FORMAT_R32G32B32A32_SFLOAT);

	environmentToCubeRenderPass.OneTimeDraw(4096.0f / 4);
	brdfRenderPass.OneTimeDraw(GLTFSceneManager::GetPreRenderedMapSize());

	//submitList.DirectionalLightTextureShadowMaps = DepthPassRenderPass.DepthTextureList;
	//submitList.PointLightShadowMaps = DepthCubeMapRenderPass.DepthCubeMapTextureList;

	{
		//SkyBox Reflection Pass
		{

			skyBoxReflectionIrradianceRenderPass.OneTimeDraw(environmentToCubeRenderPass.RenderedCubeMap, GLTFSceneManager::GetPreRenderedMapSize());
			skyBoxReflectionPrefilterRenderPass.OneTimeDraw(environmentToCubeRenderPass.RenderedCubeMap, GLTFSceneManager::GetPreRenderedMapSize());


			PBRRenderPassTextureSubmitList submitList;
			submitList.CubeMapTexture = environmentToCubeRenderPass.RenderedCubeMap;
			submitList.IrradianceTexture = skyBoxReflectionIrradianceRenderPass.IrradianceCubeMap;
			submitList.PrefilterTexture = skyBoxReflectionPrefilterRenderPass.PrefilterCubeMap;

			skyBoxReflectionRenderPass.PreRenderPass(submitList, GLTFSceneManager::GetPreRenderedMapSize());
		}
		//Mesh Reflection Pass
		{
			meshReflectionIrradianceRenderPass.OneTimeDraw(skyBoxReflectionRenderPass.RenderedReflectionCubeMap, GLTFSceneManager::GetPreRenderedMapSize());
			meshReflectionPrefilterRenderPass.OneTimeDraw(skyBoxReflectionRenderPass.RenderedReflectionCubeMap, GLTFSceneManager::GetPreRenderedMapSize());

			PBRRenderPassTextureSubmitList submitList;
			submitList.CubeMapTexture = environmentToCubeRenderPass.RenderedCubeMap;
			submitList.IrradianceTexture = meshReflectionIrradianceRenderPass.IrradianceCubeMap;
			submitList.PrefilterTexture = meshReflectionPrefilterRenderPass.PrefilterCubeMap;

			meshReflectionRenderPass.PreRenderPass(submitList, GLTFSceneManager::GetPreRenderedMapSize());
		}
		//Mesh Reflection Pass
		{
			irradianceRenderPass.OneTimeDraw(environmentToCubeRenderPass.RenderedCubeMap, GLTFSceneManager::GetPreRenderedMapSize());
			prefilterRenderPass.OneTimeDraw(environmentToCubeRenderPass.RenderedCubeMap, GLTFSceneManager::GetPreRenderedMapSize());

			PBRRenderPassTextureSubmitList submitList;
			submitList.CubeMapTexture = environmentToCubeRenderPass.RenderedCubeMap;
			submitList.IrradianceTexture = irradianceRenderPass.IrradianceCubeMap;
			submitList.PrefilterTexture = prefilterRenderPass.PrefilterCubeMap;

			gLTFRenderPass.BuildRenderPass(submitList);
		}
	}

	frameBufferRenderPass.BuildRenderPass(gLTFRenderPass.RenderedTexture, gLTFRenderPass.RenderedTexture);
	VulkanRenderer::UpdateRendererFlag = true;
	GLTFSceneManager::Update();
}

void GLTFRenderer::Update()
{
	GLTFSceneManager::Update();
}

void GLTFRenderer::ImGuiUpdate()
{
	//pipelineEditor.Update();
	//for (int x = 0; x < model.GetSunLightPropertiesBuffer().size(); x++)
	//{
	//	ImGui::SliderFloat3(("Sun Light direction " + std::to_string(x)).c_str(), &model.SunLightList[x]->GetPositionPtr()->x, -1000.0f, 1000.0f);
	//	ImGui::SliderFloat3(("Sun Light Diffuse " + std::to_string(x)).c_str(), &model.SunLightList[x]->GetDiffusePtr()->x, 0.0f, 1.0f);
	//	ImGui::SliderFloat(("Sun Light Intensity " + std::to_string(x)).c_str(), &model.SunLightList[x]->GetIntensityPtr()[0], 0.0f, 100.0f);
	//}
	for (int x = 0; x < GLTFSceneManager::GetDirectionalLights().size(); x++)
	{
		ImGui::SliderFloat3(("DLight direction " + std::to_string(1)).c_str(), &GLTFSceneManager::GetDirectionalLights()[x]->GetDirectionPtr()->x, -1.0f, 1.0f);
		ImGui::SliderFloat3(("DLight Diffuse " + std::to_string(1)).c_str(), &GLTFSceneManager::GetDirectionalLights()[x]->GetDiffusePtr()->x, 0.0f, 1.0f);
		ImGui::SliderFloat(("DLight Intensity " + std::to_string(1)).c_str(), &GLTFSceneManager::GetDirectionalLights()[x]->GetIntensityPtr()[0], 0.0f, 100.0f);
	}
	//for (int x = 0; x < model.GetPointLightPropertiesBuffer().size(); x++)
	//{
	//	ImGui::SliderFloat3(("PLight direction " + std::to_string(x)).c_str(), &model.PointLightList[x]->GetPositionPtr()->x, -1.0f, 100.0f);
	//	ImGui::SliderFloat3(("PLight Diffuse " + std::to_string(x)).c_str(), &model.PointLightList[x]->GetDiffusePtr()->x, 0.0f, 1.0f);
	//	ImGui::SliderFloat(("PLight Intensity " + std::to_string(x)).c_str(), &model.PointLightList[x]->GetIntensityPtr()[0], 0.0f, 100.0f);
	//	ImGui::SliderFloat(("PLight Radius " + std::to_string(x)).c_str(), &model.PointLightList[x]->GetRadiusPtr()[0], 0.0f, 100.0f);
	//}
	//for (int x = 0; x < model.GetSpotLightPropertiesBuffer().size(); x++)
	//{
	//	ImGui::SliderFloat3(("SLight Position " + std::to_string(x)).c_str(), &model.SpotLightList[x]->GetPositionPtr()->x, -1.0f, 1.0f);
	//	ImGui::SliderFloat3(("SLight direction " + std::to_string(x)).c_str(), &model.SpotLightList[x]->GetDirectionPtr()->x, -1.0f, 1.0f);
	//	ImGui::SliderFloat3(("SLight Diffuse " + std::to_string(x)).c_str(), &model.SpotLightList[x]->GetDiffusePtr()->x, 0.0f, 1.0f);
	//	ImGui::SliderFloat(("SLight Intensity " + std::to_string(x)).c_str(), &model.SpotLightList[x]->GetIntensityPtr()[0], 0.0f, 1.0f);
	//}

	//ImGui::SliderFloat3("Position ", &gameObjectList[0]->GameObjectPosition.x, 0.0f, 100.0f);
	//ImGui::SliderFloat3("Rotation ", &gameObjectList[0]->GameObjectRotation.x, 0.0f, 360.0f);
	//ImGui::SliderFloat3("Scale ", &gameObjectList[0]->GameObjectScale.x, 0.0f, 1.0f);
	//if (SceneManager::EditorModeFlag)
	//{
	//	if (ImGui::Button("Play Mode"))
	//	{
	//		SceneManager::EditorModeFlag = false;
	//	}
	//	if (ImGui::Button("Update Renderer"))
	//	{
	//		UpdateRenderer = true;
	//	}
	//	if (ImGui::Button("Bake"))
	//	{
	//		//BakeTextures("TestBake.bmp");
	//	}
	//}
	//else
	//{
	//	if (ImGui::Button("Editor Mode"))
	//	{
	//		SceneManager::EditorModeFlag = true;
	//	}
	//}
}

void GLTFRenderer::Draw(std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	CommandBufferSubmitList.emplace_back(gLTFRenderPass.Draw());
	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.Draw());
}

void GLTFRenderer::Destroy()
{
	GLTFSceneManager::Destroy();

	environmentToCubeRenderPass.Destroy();
	brdfRenderPass.Destroy();
	skyBoxReflectionIrradianceRenderPass.Destroy();
	skyBoxReflectionPrefilterRenderPass.Destroy();
	skyBoxReflectionRenderPass.Destroy();
	//meshReflectionIrradianceRenderPass.Destroy();
	//meshReflectionPrefilterRenderPass.Destroy();
	//meshReflectionRenderPass.Destroy();
	irradianceRenderPass.Destroy();
	prefilterRenderPass.Destroy();
	gLTFRenderPass.Destroy();
	frameBufferRenderPass.Destroy();
}
