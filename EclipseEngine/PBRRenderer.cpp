#include "PBRRenderer.h"
#include "SpriteGameObject3D.h"


PBRRenderer::PBRRenderer()
{
	GLTFSceneManager::StartUp();
	GLTFSceneManager::ActiveCamera = std::make_shared<PerspectiveCamera>(PerspectiveCamera("DefaultCamera", VulkanRenderer::GetSwapChainResolutionVec2(), glm::vec3(0.0f, 0.0f, 5.0f)));

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
	IronMaterial->AlbedoMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/bricks2.jpg"), TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_SRGB)));
	IronMaterial->MetallicMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("//texture/pbr/rusted_iron/metallic.png"), TextureTypeEnum::kMetallicTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	IronMaterial->RoughnessMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("//texture/pbr/rusted_iron/roughness.png"), TextureTypeEnum::kRoughnessTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	IronMaterial->AmbientOcclusionMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/rusted_iron/ao.png"), TextureTypeEnum::kAmbientOcclusionTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	IronMaterial->NormalMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/rusted_iron/normal.png"), TextureTypeEnum::kNormalTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	//IronMaterial->EmissionMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/matrix.jpg", TextureTypeEnum::kEmissionTextureMap, VK_FORMAT_R8G8B8A8_SRGB)));
	//IronMaterial->HeightMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/EclipseEngine/texture/bricks2_disp.jpg", TextureTypeEnum::kDepthTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));

	GLTFSceneManager::UpdateBufferIndex();
	IronMaterial->UpdateBuffer();
	GLTFSceneManager::AddMaterial(IronMaterial);

	GLTFSceneManager::AddMeshGameObject3D("sponza", VulkanRenderer::OpenFile("/Models/GLTFSponza/Sponza.gltf"));
	GLTFSceneManager::AddMeshGameObject3D("Sphere", VulkanRenderer::OpenFile("/Models/GLTFGold/Gold.gltf"), IronMaterial);
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

	std::shared_ptr<Material> PlasticMaterial = std::make_shared<Material>(Material("PlasticMaterial"));
	PlasticMaterial->AlbedoMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/plastic/albedo.png"), TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_SRGB)));
	PlasticMaterial->MetallicMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/plastic/metallic.png"), TextureTypeEnum::kMetallicTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	PlasticMaterial->RoughnessMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/plastic/roughness.png"), TextureTypeEnum::kRoughnessTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	PlasticMaterial->AmbientOcclusionMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/plastic/ao.png"), TextureTypeEnum::kAmbientOcclusionTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	PlasticMaterial->NormalMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/plastic/normal.png"), TextureTypeEnum::kNormalTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	PlasticMaterial->EmissionMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/matrix.jpg"), TextureTypeEnum::kEmissionTextureMap, VK_FORMAT_R8G8B8A8_SRGB)));
	GLTFSceneManager::UpdateBufferIndex();
	PlasticMaterial->UpdateBuffer();
	GLTFSceneManager::AddMaterial(PlasticMaterial);

	std::shared_ptr<Material> WallMaterial = std::make_shared<Material>(Material("WallMaterial"));
	WallMaterial->AlbedoMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/wall/albedo.png"), TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_SRGB)));
	WallMaterial->MetallicMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/wall/metallic.png"), TextureTypeEnum::kMetallicTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	WallMaterial->RoughnessMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/wall/roughness.png"), TextureTypeEnum::kRoughnessTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	WallMaterial->AmbientOcclusionMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/wall/ao.png"), TextureTypeEnum::kAmbientOcclusionTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	WallMaterial->NormalMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/wall/normal.png"), TextureTypeEnum::kNormalTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	WallMaterial->EmissionMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/matrix.jpg"), TextureTypeEnum::kEmissionTextureMap, VK_FORMAT_R8G8B8A8_SRGB)));
	GLTFSceneManager::UpdateBufferIndex();
	WallMaterial->UpdateBuffer();
	GLTFSceneManager::AddMaterial(WallMaterial);

	std::shared_ptr<Material> GoldMaterial = std::make_shared<Material>(Material("GoldMaterial"));
	GoldMaterial->AlbedoMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/gold/albedo.png"), TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_SRGB)));
	GoldMaterial->MetallicMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/gold/metallic.png"), TextureTypeEnum::kMetallicTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	GoldMaterial->RoughnessMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/gold/roughness.png"), TextureTypeEnum::kRoughnessTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	GoldMaterial->AmbientOcclusionMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/gold/ao.png"), TextureTypeEnum::kAmbientOcclusionTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	GoldMaterial->NormalMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/gold/normal.png"), TextureTypeEnum::kNormalTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	GoldMaterial->EmissionMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/matrix.jpg"), TextureTypeEnum::kEmissionTextureMap, VK_FORMAT_R8G8B8A8_SRGB)));
	GLTFSceneManager::UpdateBufferIndex();
	GoldMaterial->UpdateBuffer();
	GLTFSceneManager::AddMaterial(GoldMaterial);

	std::shared_ptr<Material> GrassMaterial = std::make_shared<Material>(Material("GrassMaterial"));
	GrassMaterial->AlbedoMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/grass/albedo.png"), TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_SRGB)));
	GrassMaterial->MetallicMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/grass/metallic.png"), TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	GrassMaterial->RoughnessMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/grass/roughness.png"), TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	GrassMaterial->AmbientOcclusionMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/grass/ao.png"), TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	GrassMaterial->NormalMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/grass/normal.png"), TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_UNORM)));
	GrassMaterial->EmissionMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/matrix.jpg"), TextureTypeEnum::kEmissionTextureMap, VK_FORMAT_R8G8B8A8_SRGB)));
	GLTFSceneManager::UpdateBufferIndex();
	GrassMaterial->UpdateBuffer();
	GLTFSceneManager::AddMaterial(GrassMaterial);


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

PBRRenderer::~PBRRenderer()
{
}

void PBRRenderer::BuildRenderer()
{
	GLTFSceneManager::sceneProperites.PBRMaxMipLevel = static_cast<uint32_t>(std::floor(std::log2(std::max(GLTFSceneManager::GetPreRenderedMapSize(), GLTFSceneManager::GetPreRenderedMapSize())))) + 1;
	GLTFSceneManager::EnvironmentTexture = std::make_shared<EnvironmentTexture>(VulkanRenderer::OpenFile("/texture/hdr/alps_field_4k.hdr"), VK_FORMAT_R32G32B32A32_SFLOAT);

	environmentToCubeRenderPass.OneTimeDraw(4096.0f / 4);

	brdfRenderPass.OneTimeDraw(GLTFSceneManager::GetPreRenderedMapSize());
	GLTFSceneManager::BRDFTexture = brdfRenderPass.GetImageTexture();
	perlinNoise.BuildRenderPass(glm::ivec2(512));
	voronoiNoiseRenderPass.BuildRenderPass(glm::ivec2(512));
	auto AlbedoMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/grass/albedo.png"), TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_SRGB)));
	multiplyRenderPass.BuildRenderPass(glm::ivec2(512), AlbedoMap, voronoiNoiseRenderPass.GetImageTexture());
	{
		//Depth Pass
		{
			depthRenderPass.BuildRenderPass(glm::vec2(512.0f));
			depthCubeMapRenderPass.BuildRenderPass(glm::vec2(512.0f));
			depthSpotLightRenderPass.BuildRenderPass(glm::vec2(512.0f));
		}

		PBRRenderPassTextureSubmitList submitList;
		submitList.DirectionalLightTextureShadowMaps = depthRenderPass.DepthTextureList;
		submitList.PointLightShadowMaps = depthCubeMapRenderPass.DepthCubeMapTextureList;

		//SkyBox Reflection Pass
	/*	{
			skyBoxReflectionIrradianceRenderPass.OneTimeDraw(GLTFSceneManager::CubeMap, GLTFSceneManager::GetPreRenderedMapSize());
			skyBoxReflectionPrefilterRenderPass.OneTimeDraw(GLTFSceneManager::CubeMap, GLTFSceneManager::GetPreRenderedMapSize());


			PBRRenderPassTextureSubmitList submitList;
			submitList.CubeMapTexture = GLTFSceneManager::CubeMap;
			submitList.IrradianceTextureList.emplace_back(skyBoxReflectionIrradianceRenderPass.IrradianceCubeMap);
			submitList.PrefilterTextureList.emplace_back(skyBoxReflectionPrefilterRenderPass.PrefilterCubeMap);
			submitList.DirectionalLightTextureShadowMaps.emplace_back(depthRenderPass.RenderPassDepthTexture);
			for (int x = 0; x < GLTFSceneManager::GetPointLights().size(); x++)
			{
				submitList.PointLightShadowMaps.emplace_back(depthCubeMapRenderPass.DepthCubeMapTextureList[x]);
			}
			submitList.SpotLightTextureShadowMaps.emplace_back(depthSpotLightRenderPass.RenderPassDepthTexture);

			skyBoxReflectionRenderPass.PreRenderPass(submitList, GLTFSceneManager::GetPreRenderedMapSize(), glm::vec3(0.0f, 20.0f, 0.0f));
		}*/
		//Mesh Reflection Pass
	/*	{
			meshReflectionIrradianceRenderPass.OneTimeDraw(skyBoxReflectionRenderPass.RenderedReflectionCubeMap, GLTFSceneManager::GetPreRenderedMapSize());
			meshReflectionPrefilterRenderPass.OneTimeDraw(skyBoxReflectionRenderPass.RenderedReflectionCubeMap, GLTFSceneManager::GetPreRenderedMapSize());

			PBRRenderPassTextureSubmitList submitList;
			submitList.CubeMapTexture = GLTFSceneManager::CubeMap;
			submitList.IrradianceTextureList.emplace_back(meshReflectionIrradianceRenderPass.IrradianceCubeMap);
			submitList.PrefilterTextureList.emplace_back(meshReflectionPrefilterRenderPass.PrefilterCubeMap);
			submitList.DirectionalLightTextureShadowMaps.emplace_back(depthRenderPass.RenderPassDepthTexture);
			for (int x = 0; x < GLTFSceneManager::GetPointLights().size(); x++)
			{
				submitList.PointLightShadowMaps.emplace_back(depthCubeMapRenderPass.DepthCubeMapTextureList[x]);
			}
			submitList.SpotLightTextureShadowMaps.emplace_back(depthSpotLightRenderPass.RenderPassDepthTexture);

			meshReflectionRenderPass.PreRenderPass(submitList, GLTFSceneManager::GetPreRenderedMapSize(), glm::vec3(0.0f, 20.0f, 0.0f));
		}*/
		//Mesh Reflection Pass
		{
			irradianceRenderPass.OneTimeDraw(GLTFSceneManager::CubeMap, GLTFSceneManager::GetPreRenderedMapSize());
			prefilterRenderPass.OneTimeDraw(GLTFSceneManager::CubeMap, GLTFSceneManager::GetPreRenderedMapSize());

			PBRRenderPassTextureSubmitList submitList;
			submitList.CubeMapTexture = GLTFSceneManager::CubeMap;
			submitList.IrradianceTextureList.emplace_back(irradianceRenderPass.IrradianceCubeMap);
			submitList.PrefilterTextureList.emplace_back(prefilterRenderPass.PrefilterCubeMap);
			submitList.DirectionalLightTextureShadowMaps.emplace_back(depthRenderPass.RenderPassDepthTexture);
			for (int x = 0; x < GLTFSceneManager::GetPointLights().size(); x++)
			{
				submitList.PointLightShadowMaps.emplace_back(depthCubeMapRenderPass.DepthCubeMapTextureList[x]);
			}
			submitList.SpotLightTextureShadowMaps.emplace_back(depthSpotLightRenderPass.RenderPassDepthTexture);

			std::string a = "PBRRenderPass.txt";
			gLTFRenderPass.BuildRenderPass(a, submitList);
		}

		//Post Process Pass
		{
			std::vector<std::shared_ptr<RenderedColorTexture>> textureList;
			textureList.emplace_back(gLTFRenderPass.RenderedBloomTexture);

			BloomRenderPass.BuildRenderPass(textureList);
			bloomCombineRenderPass.BuildRenderPass(BloomRenderPass.BlurredTextureList);
			frameBufferRenderPass.BuildRenderPass(gLTFRenderPass.RenderedTexture, bloomCombineRenderPass.BloomTexture);
		}
	}

	GLTFSceneManager::Update();
	lightManagerMenu.Update();

	VulkanRenderer::UpdateRendererFlag = true;
}

void PBRRenderer::Update()
{
	GLTFSceneManager::Update();
	if (VulkanRenderer::UpdateRendererFlag == true)
	{
		lightManagerMenu.Update();
	}
	
}

void PBRRenderer::ImGuiUpdate()
{
	pipelineEditor.Update();
	lightManagerMenu.ImGuiUpdate();
	ImGui::ShowDemoWindow();

	ImGui::Begin("Scene Hierarchy Manager");
	multiplyRenderPass.GetImageTexture()->ImGuiShowTexture(ImVec2(512, 512));
	if(ImGui::Button("Save Texture", ImVec2(50, 25)))
	{
		voronoiNoiseRenderPass.SaveTexture("../adsfasdf2.BMP", BakeTextureFormat::Bake_BMP);
	}
	int index = 0;
	for (auto& obj : GLTFSceneManager::GameObjectList)
	{
		index++;
		ImGui::PushID(index);
		bool selectedModel = ImGui::TreeNodeEx(&obj->ObjectName, ImGuiTreeNodeFlags_DefaultOpen ||
																			ImGuiTreeNodeFlags_FramePadding ||
																			ImGuiTreeNodeFlags_OpenOnArrow ||
																			ImGuiTreeNodeFlags_SpanAvailWidth,
																obj->ObjectName.c_str());
		ImGui::PopID();
		if (selectedModel)
		{
			SelectedGameObject = obj;
			for (auto& mesh : obj->GetMeshList())
			{
					bool selectedMesh = ImGui::TreeNodeEx(&mesh->MeshName, ImGuiTreeNodeFlags_DefaultOpen ||
																			ImGuiTreeNodeFlags_FramePadding ||
																			ImGuiTreeNodeFlags_OpenOnArrow ||
																			ImGuiTreeNodeFlags_SpanAvailWidth ,
																			mesh->MeshName.c_str());
				if (selectedMesh)
				{
					SelectedMesh = mesh;
					for (auto& material : mesh->GetMaterialList())
					{
						bool selectedMaterial = ImGui::TreeNodeEx(&material->MaterialName, ImGuiTreeNodeFlags_DefaultOpen ||
							ImGuiTreeNodeFlags_FramePadding ||
							ImGuiTreeNodeFlags_OpenOnArrow ||
							ImGuiTreeNodeFlags_SpanAvailWidth,
							material->MaterialName.c_str());
						if (selectedMaterial)
						{
							SelectedMaterial = material;
							ImGui::TreePop();
						}
					}

					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
	}
	ImGui::End();

	if (SelectedGameObject)
	{
		ImGui::Begin("Model Properties");
		ImGui::SliderFloat3("Position ", &SelectedGameObject->GameObjectPosition.x, 0.0f, 100.0f);
		ImGui::SliderFloat3("Rotation ", &SelectedGameObject->GameObjectRotation.x, 0.0f, 360.0f);
		ImGui::SliderFloat3("Scale ", &SelectedGameObject->GameObjectScale.x, 0.0f, 1.0f);
		ImGui::End();
	}

	if (SelectedMesh)
	{
		ImGui::Begin("Mesh Properties");
		ImGui::SliderFloat3("Position ", &SelectedMesh->MeshPosition.x, 0.0f, 100.0f);
		ImGui::SliderFloat3("Rotation ", &SelectedMesh->MeshRotation.x, 0.0f, 360.0f);
		ImGui::SliderFloat3("Scale ", &SelectedMesh->MeshScale.x, 0.0f, 1.0f);
		if (SelectedMaterial)
		{
			ImGui::Separator();
			ImGui::LabelText(SelectedMaterial->MaterialName.c_str(), "");
			ImGui::Separator();

			if (SelectedMaterial->AlbedoMap)
			{
				ImGui::LabelText("Alebdo", "");
				ImGui::SameLine();
				SelectedMaterial->AlbedoMap->ImGuiShowTexture(ImVec2(100.0f, 100.0f));
				ImGui::Separator();
			}
			else
			{
				ImGuiColorEditFlags misc_flags = ImGuiColorEditFlags_DisplayRGB || ImGuiColorEditFlags_InputRGB || ImGuiColorEditFlags_NoAlpha;
				ImGui::ColorEdit3("Alebdo", (float*)&SelectedMaterial->Albedo.x, misc_flags);
				ImGui::Separator();
			}

			if (SelectedMaterial->MetallicRoughnessMap)
			{
				ImGui::LabelText("Metallic/Roughness", "");
				ImGui::SameLine();
				SelectedMaterial->MetallicRoughnessMap->ImGuiShowTexture(ImVec2(100.0f, 100.0f));
				ImGui::Separator();
			}

			if (SelectedMaterial->MetallicMap)
			{
				ImGui::LabelText("Metallic", "");
				ImGui::SameLine();
				SelectedMaterial->MetallicMap->ImGuiShowTexture(ImVec2(100.0f, 100.0f));
				ImGui::Separator();
			}
			else
			{
				ImGui::SliderFloat("Metallic ", &SelectedMaterial->Metallic, 0.0f, 1.0f);
				ImGui::Separator();
			}

			if (SelectedMaterial->RoughnessMap)
			{
				ImGui::LabelText("Roughness", "");
				ImGui::SameLine();
				SelectedMaterial->RoughnessMap->ImGuiShowTexture(ImVec2(100.0f, 100.0f));
				ImGui::Separator();
			}
			else
			{
				ImGui::SliderFloat("Roughness ", &SelectedMaterial->Roughness, 0.0f, 1.0f);
				ImGui::Separator();
			}

			if (SelectedMaterial->EmissionMap)
			{
				ImGui::LabelText("Emission", "");
				ImGui::SameLine();
				SelectedMaterial->EmissionMap->ImGuiShowTexture(ImVec2(100.0f, 100.0f));
				ImGui::Separator();
			}
			else
			{
				ImGuiColorEditFlags misc_flags = ImGuiColorEditFlags_DisplayRGB || ImGuiColorEditFlags_InputRGB || ImGuiColorEditFlags_NoAlpha;
				ImGui::ColorEdit3("Emission", (float*)&SelectedMaterial->EmissionMap, misc_flags);
				ImGui::Separator();
			}

			if (SelectedMaterial->AmbientOcclusionMap)
			{
				ImGui::LabelText("Ambient Occlusion", "");
				ImGui::SameLine();
				SelectedMaterial->AmbientOcclusionMap->ImGuiShowTexture(ImVec2(100.0f, 100.0f));
				ImGui::Separator();
			}
			else
			{
				ImGui::SliderFloat("Ambient Occlusion ", &SelectedMaterial->AmbientOcclusion, 0.0f, 1.0f);
				ImGui::Separator();
			}

			if (SelectedMaterial->AlphaMap)
			{
				ImGui::LabelText("Alpha", "");
				ImGui::SameLine();
				SelectedMaterial->AlphaMap->ImGuiShowTexture(ImVec2(100.0f, 100.0f));
				ImGui::Separator();
			}
			else
			{
				ImGuiColorEditFlags misc_flags = ImGuiColorEditFlags_DisplayRGB || ImGuiColorEditFlags_InputRGB || ImGuiColorEditFlags_NoAlpha;
				ImGui::SliderFloat("Alpha ", &SelectedMaterial->Alpha, 0.0f, 1.0f);
				ImGui::Separator();
			}
		}

		ImGui::End();
	}
	
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

void PBRRenderer::Draw(std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	CommandBufferSubmitList.emplace_back(perlinNoise.Draw((float)glfwGetTime()));
	CommandBufferSubmitList.emplace_back(voronoiNoiseRenderPass.Draw((float)glfwGetTime()));
	CommandBufferSubmitList.emplace_back(multiplyRenderPass.Draw((float)glfwGetTime()));
	//CommandBufferSubmitList.emplace_back(brdfRenderPass.Draw());

	CommandBufferSubmitList.emplace_back(depthRenderPass.Draw());
	CommandBufferSubmitList.emplace_back(depthCubeMapRenderPass.Draw());
	CommandBufferSubmitList.emplace_back(depthSpotLightRenderPass.Draw());

	//CommandBufferSubmitList.emplace_back(skyBoxReflectionIrradianceRenderPass.Draw());
	//CommandBufferSubmitList.emplace_back(skyBoxReflectionPrefilterRenderPass.Draw());
	//CommandBufferSubmitList.emplace_back(skyBoxReflectionRenderPass.Draw(GLTFSceneManager::ActiveCamera->GetPosition()));

	//CommandBufferSubmitList.emplace_back(meshReflectionIrradianceRenderPass.Draw());
	//CommandBufferSubmitList.emplace_back(meshReflectionPrefilterRenderPass.Draw());
	//CommandBufferSubmitList.emplace_back(meshReflectionRenderPass.Draw(GLTFSceneManager::ActiveCamera->GetPosition()));

	//CommandBufferSubmitList.emplace_back(irradianceRenderPass.Draw());
	//CommandBufferSubmitList.emplace_back(prefilterRenderPass.Draw());
	CommandBufferSubmitList.emplace_back(gLTFRenderPass.Draw());

	CommandBufferSubmitList.emplace_back(BloomRenderPass.Draw());
	CommandBufferSubmitList.emplace_back(bloomCombineRenderPass.Draw());
	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.Draw());
}

void PBRRenderer::Destroy()
{
	GLTFSceneManager::Destroy();

	environmentToCubeRenderPass.Destroy();
	brdfRenderPass.Destroy();

	depthRenderPass.Destroy();
	depthCubeMapRenderPass.Destroy();
	depthSpotLightRenderPass.Destroy();

	//skyBoxReflectionIrradianceRenderPass.Destroy();
	//skyBoxReflectionPrefilterRenderPass.Destroy();
	//skyBoxReflectionRenderPass.Destroy();
	//
	//meshReflectionIrradianceRenderPass.Destroy();
	//meshReflectionPrefilterRenderPass.Destroy();
	//meshReflectionRenderPass.Destroy();
	
	irradianceRenderPass.Destroy();
	prefilterRenderPass.Destroy();
	gLTFRenderPass.Destroy();
	
	BloomRenderPass.Destroy();
	bloomCombineRenderPass.Destroy();
	frameBufferRenderPass.Destroy();
}