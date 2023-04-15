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
	auto a = "C:/Users/dotha/source/repos/EclipseEngine/Models/GLTFSponza/Sponza.gltf";
	auto b = "C:/Users/dotha/source/repos/EclipseEngine/Models/GLTFIron/Iron.gltf";
	auto d = "C:/Users/dotha/source/repos/EclipseEngine/Models/GLTFGold/Gold.gltf";
	auto c = "C:/Users/dotha/source/repos/EclipseEngine/Models/glTF-Sample-Models-master/2.0/SciFiHelmet/glTF/SciFiHelmet.gltf";

	GLTFSceneManager::ActiveCamera = std::make_shared<PerspectiveCamera>(PerspectiveCamera("DefaultCamera", VulkanRenderer::GetSwapChainResolutionVec2(), glm::vec3(0.0f, 0.0f, 5.0f)));

	//modelList.emplace_back(std::make_shared<GLTF_Temp_Model>(GLTF_Temp_Model(a, glm::mat4(1.0f), 0)));
	//if(gameObjectList.size() == 0)
	gameObjectList.emplace_back(std::make_shared<GameObject3D>(GameObject3D("Sphere", GameObjectRenderType::kModelRenderer)));
	gameObjectList.back()->LoadRenderObject<Vertex3D>(a);

	gameObjectList.emplace_back(std::make_shared<GameObject3D>(GameObject3D("Sphere", GameObjectRenderType::kModelRenderer)));
	gameObjectList.back()->LoadRenderObject<Vertex3D>(b);

	gameObjectList.emplace_back(std::make_shared<GameObject3D>(GameObject3D("Sphere", GameObjectRenderType::kModelRenderer)));
	gameObjectList.back()->LoadRenderObject<Vertex3D>(c);


	std::shared_ptr<GLTFMaterial> material = std::make_shared<GLTFMaterial>(GLTFMaterial("TestMaterial"));
	TinyGltfTextureSamplerLoader SamplerLoader{};

	std::string mario = "C:/Users/dotha/source/repos/VulkanGraphics/texture/Brick_diffuseOriginal.bmp";
	std::string mario2 = "C:/Users/dotha/source/repos/VulkanGraphics/texture/Brick_diffuseOriginal.bmp";
	material->AlbedoMap = std::make_shared<Texture2D>(Texture2D(mario, TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_SRGB));
    material->AlphaMap = std::make_shared<Texture2D>(Texture2D(mario2, TextureTypeEnum::kAlphaTextureMap, VK_FORMAT_R8G8B8A8_UNORM));
	
	//const std::string asdf = "sprite";
	//std::shared_ptr<SpriteGameObject3D> sprite = std::make_shared<SpriteGameObject3D>(SpriteGameObject3D(asdf, material));
	//gameObjectList.emplace_back(sprite);

	//int width = 500;
	//int height = 500;
	//float length = 10.0f;
	//float radius = 0.5f;
	//std::vector<LineVertex3D> VertexList;
	//for (uint32_t y = 0; y < height; y++)
	//{
	//    for (uint32_t x = 0; x < width; x++)
	//    {
	//        glm::vec2 coord = { (float)x / width, (float)y / height };
	//        coord = coord * 2.0f - 1.0f;

	//        uint8_t r = (uint8_t)(coord.x * 255.0f);
	//        uint8_t g = (uint8_t)(coord.y * 255.0f);

	//        glm::vec3 rayOrigin(0.0f, 0.0f, 2.0f);
	//        glm::vec3 rayDirection(coord.x, coord.y, -1.0f);

	//        float a = glm::dot(rayDirection, rayDirection);
	//        float b = 2.0f * glm::dot(rayOrigin, rayDirection);
	//        float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

	//        //Quadratic forumla discriminat
	//        //b^2 - 4ac;

	//        float discriminant = (b * b) - 4.0f * a * c;

	//        if (discriminant >= 0.0f)
	//        {
	//            VertexList.emplace_back(LineVertex3D(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec4(1.0f, 0.0f, 0.8f, 1.0f)));
	//            VertexList.emplace_back(LineVertex3D(glm::vec3(coord.x, coord.y, -1.0f), glm::vec4(1.0f, 0.0f, 0.8, 1.0f)));
	//        }
	//        else
	//        {
	//            VertexList.emplace_back(LineVertex3D(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec4(0.0f, 0.0f, 01.0f, 0.02f)));
	//            VertexList.emplace_back(LineVertex3D(glm::vec3(coord.x, coord.y, -1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 0.02f)));
	//        }
	//    }
	//}

	//gameObjectList.emplace_back(std::make_shared<LineGameObject>(LineGameObject("Sphere", VertexList)));

	//gameObjectList.emplace_back(std::make_shared<LineRenderer3D>(LineRenderer3D("Line", VertexList)));


	GLTFSceneManager::AddDirectionalLight(std::make_shared<GLTFDirectionalLight>(GLTFDirectionalLight("sdf", glm::vec3(0.01f), glm::vec3(1.0f), 30.8f)));

	GLTFSceneManager::sceneProperites.PBRMaxMipLevel = static_cast<uint32_t>(std::floor(std::log2(std::max(GLTFSceneManager::GetPreRenderedMapSize(), GLTFSceneManager::GetPreRenderedMapSize())))) + 1;
	GLTFSceneManager::EnvironmentTexture = std::make_shared<EnvironmentTexture>("../texture/hdr/alps_field_4k.hdr", VK_FORMAT_R32G32B32A32_SFLOAT);

	environmentToCubeRenderPass.OneTimeDraw(4096.0f / 4);
	GLTF_BRDFRenderPass.OneTimeDraw(GLTFSceneManager::GetPreRenderedMapSize());

	std::vector<std::shared_ptr<RenderedCubeMapTexture>> cubemap = { GLTFSceneManager::CubeMap };
	irradianceRenderPass.OneTimeDraw(cubemap, GLTFSceneManager::GetPreRenderedMapSize());
	GLTFSceneManager::IrradianceMap = irradianceRenderPass.IrradianceCubeMapList[0];
	prefilterRenderPass.OneTimeDraw(cubemap, GLTFSceneManager::GetPreRenderedMapSize());
	GLTFSceneManager::PrefilterMap = prefilterRenderPass.PrefilterCubeMapList[0];

	gLTFRenderPass.BuildRenderPass(gameObjectList);
	frameBufferRenderPass.BuildRenderPass(gLTFRenderPass.RenderedTexture, gLTFRenderPass.RenderedTexture);
}

void GLTFRenderer::Update()
{
	GLTFSceneManager::Update();
	for (auto& obj : gameObjectList)
	{
		obj->Update(VulkanRenderer::GetFrameTimeDurationMilliseconds());
	}
}

void GLTFRenderer::ImGuiUpdate()
{
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

	ImGui::SliderFloat3("Position ", &gameObjectList[0]->GameObjectPosition.x, 0.0f, 100.0f);
	ImGui::SliderFloat3("Rotation ", &gameObjectList[0]->GameObjectRotation.x, 0.0f, 360.0f);
	ImGui::SliderFloat3("Scale ", &gameObjectList[0]->GameObjectScale.x, 0.0f, 1.0f);
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
	CommandBufferSubmitList.emplace_back(gLTFRenderPass.Draw(gameObjectList));
	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.Draw());
}

void GLTFRenderer::Destroy()
{
	GLTFSceneManager::Destroy();
	for (auto& obj : gameObjectList)
	{
		obj->Destroy();
	}

	environmentToCubeRenderPass.Destroy();
	GLTF_BRDFRenderPass.Destroy();
	irradianceRenderPass.Destroy();
	prefilterRenderPass.Destroy();
	gLTFRenderPass.Destroy();
	frameBufferRenderPass.Destroy();
}
