#include "GLTFRenderer.h"

GLTFRenderer::GLTFRenderer()
{
}

GLTFRenderer::~GLTFRenderer()
{
}

void GLTFRenderer::BuildRenderer()
{
	SceneManager::sceneProperites.PBRMaxMipLevel = static_cast<uint32_t>(std::floor(std::log2(std::max(SceneManager::GetPreRenderedMapSize(), SceneManager::GetPreRenderedMapSize())))) + 1;
	SceneManager::environmentTexture = std::make_shared<EnvironmentTexture>("../texture/hdr/alps_field_4k.hdr", VK_FORMAT_R32G32B32A32_SFLOAT);

	environmentToCubeRenderPass.BuildRenderPass(4096.0f / 4);
	GLTF_BRDFRenderPass.BuildRenderPass(SceneManager::GetPreRenderedMapSize());

	std::vector<std::shared_ptr<RenderedCubeMapTexture>> cubemap = { SceneManager::CubeMap };
	irradianceRenderPass.OneTimeDraw(cubemap, SceneManager::GetPreRenderedMapSize());
	SceneManager::IrradianceMap = irradianceRenderPass.IrradianceCubeMapList[0];
	prefilterRenderPass.OneTimeDraw(cubemap, SceneManager::GetPreRenderedMapSize());
	SceneManager::PrefilterMap = prefilterRenderPass.PrefilterCubeMapList[0];

	auto a = "C:/Users/dotha/source/repos/EclipseEngine/Models/GLTFSponza/Sponza.gltf";
	model = GLTF_Temp_Model(a, glm::mat4(1.0f), 0);

	gLTFRenderPass.BuildRenderPass(model);
	frameBufferRenderPass.BuildRenderPass(gLTFRenderPass.RenderedTexture, gLTFRenderPass.RenderedTexture);
}

void GLTFRenderer::Update()
{
	sceneProperites.CameraPos = SceneManager::activeCamera->GetPosition();
	sceneProperites.view = SceneManager::activeCamera->GetViewMatrix();
	sceneProperites.proj = SceneManager::activeCamera->GetProjectionMatrix();
	sceneProperites.SunLightCount = 1;
	sceneProperites.DirectionalLightCount = 1;
	sceneProperites.PointLightCount = 7;
	sceneProperites.SpotLightCount = LightManager::GetSpotLightCount();
	sceneProperites.Timer = (float)glfwGetTime();
	sceneProperites.frame++;
	if (sceneProperites.frame == UINT32_MAX)
	{
		sceneProperites.frame = 0;
	}
	sceneProperites.MaxReflectCount = 2;

	model.Update(glm::mat4(1.0f));
}

void GLTFRenderer::ImGuiUpdate()
{
	//for (int x = 0; x < model.GetSunLightPropertiesBuffer().size(); x++)
	//{
	//	ImGui::SliderFloat3(("Sun Light direction " + std::to_string(x)).c_str(), &model.SunLightList[x]->GetPositionPtr()->x, -1000.0f, 1000.0f);
	//	ImGui::SliderFloat3(("Sun Light Diffuse " + std::to_string(x)).c_str(), &model.SunLightList[x]->GetDiffusePtr()->x, 0.0f, 1.0f);
	//	ImGui::SliderFloat(("Sun Light Intensity " + std::to_string(x)).c_str(), &model.SunLightList[x]->GetIntensityPtr()[0], 0.0f, 100.0f);
	//}
	//for (int x = 0; x < model.GetDirectionalLightPropertiesBuffer().size(); x++)
	//{
	//	ImGui::SliderFloat3(("DLight direction " + std::to_string(x)).c_str(), &model.DirectionalLightList[x]->GetDirectionPtr()->x, -1.0f, 1.0f);
	//	ImGui::SliderFloat3(("DLight Diffuse " + std::to_string(x)).c_str(), &model.DirectionalLightList[x]->GetDiffusePtr()->x, 0.0f, 1.0f);
	//	ImGui::SliderFloat(("DLight Intensity " + std::to_string(x)).c_str(), &model.DirectionalLightList[x]->GetIntensityPtr()[0], 0.0f, 100.0f);
	//}
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

	//ImGui::SliderFloat3("Position ", &model.MeshList[0]->MeshPosition.x, 0.0f, 1.0f);
	//ImGui::SliderFloat3("Rotation ", &model.MeshList[0]->MeshRotation.x, 0.0f, 360.0f);
	//ImGui::SliderFloat3("Scale ", &model.MeshList[0]->MeshScale.x, 0.0f, 1.0f);
	if (SceneManager::EditorModeFlag)
	{
		if (ImGui::Button("Play Mode"))
		{
			SceneManager::EditorModeFlag = false;
		}
		if (ImGui::Button("Update Renderer"))
		{
			UpdateRenderer = true;
		}
		if (ImGui::Button("Bake"))
		{
			//BakeTextures("TestBake.bmp");
		}
	}
	else
	{
		if (ImGui::Button("Editor Mode"))
		{
			SceneManager::EditorModeFlag = true;
		}
	}
}

void GLTFRenderer::Draw(std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	CommandBufferSubmitList.emplace_back(gLTFRenderPass.Draw(model));
	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.Draw());
}

void GLTFRenderer::Destroy()
{
	environmentToCubeRenderPass.Destroy();
	GLTF_BRDFRenderPass.Destroy();
	irradianceRenderPass.Destroy();
	prefilterRenderPass.Destroy();
	gLTFRenderPass.Destroy();
	frameBufferRenderPass.Destroy();

	model.Destroy();
}
