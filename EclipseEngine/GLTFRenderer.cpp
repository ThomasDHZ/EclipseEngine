#include "GLTFRenderer.h"

GLTFRenderer::GLTFRenderer()
{
}

GLTFRenderer::~GLTFRenderer()
{
}

void GLTFRenderer::BuildRenderer()
{
	GLTFSceneManager::sceneProperites.PBRMaxMipLevel = static_cast<uint32_t>(std::floor(std::log2(std::max(GLTFSceneManager::GetPreRenderedMapSize(), GLTFSceneManager::GetPreRenderedMapSize())))) + 1;
	GLTFSceneManager::EnvironmentTexture = std::make_shared<EnvironmentTexture>("../texture/hdr/alps_field_4k.hdr", VK_FORMAT_R32G32B32A32_SFLOAT);

	//GLTFSceneManager::ActiveCamera = std::make_shared<PerspectiveCamera>(PerspectiveCamera("DefaultCamera", VulkanRenderer::GetSwapChainResolutionVec2(), glm::vec3(0.0f, 0.0f, 5.0f)));

	environmentToCubeRenderPass.BuildRenderPass(1024.0f);
	GLTF_BRDFRenderPass.BuildRenderPass(GLTFSceneManager::GetPreRenderedMapSize());

	std::vector<std::shared_ptr<RenderedCubeMapTexture>> cubemap = { SceneManager::CubeMap };
	irradianceRenderPass.OneTimeDraw(cubemap, GLTFSceneManager::GetPreRenderedMapSize());
	GLTFSceneManager::IrradianceMap = irradianceRenderPass.IrradianceCubeMapList[0];
	prefilterRenderPass.OneTimeDraw(cubemap, GLTFSceneManager::GetPreRenderedMapSize());
	GLTFSceneManager::PrefilterMap = prefilterRenderPass.PrefilterCubeMapList[0];

	auto a = "C:/Users/dotha/source/repos/EclipseEngine/Models/GLTFIron/Iron.gltf";
	model = std::make_shared<ModelRenderer>(ModelRenderer("Testobject", a, glm::vec3(0.0f)));

	gLTFRenderPass.BuildRenderPass(model);
	frameBufferRenderPass.BuildRenderPass(gLTFRenderPass.RenderedTexture, gLTFRenderPass.RenderedTexture);
}

void GLTFRenderer::Update()
{
	//sceneProperites.CameraPos = SceneManager::activeCamera->GetPosition();
	//sceneProperites.view = SceneManager::activeCamera->GetViewMatrix();
	//sceneProperites.proj = SceneManager::activeCamera->GetProjectionMatrix();
	//sceneProperites.SunLightCount = 1;
	//sceneProperites.DirectionalLightCount = 1;
	//sceneProperites.PointLightCount = 7;
	//sceneProperites.SpotLightCount = LightManager::GetSpotLightCount();
	//sceneProperites.Timer = (float)glfwGetTime();
	//sceneProperites.frame++;
	//if (sceneProperites.frame == UINT32_MAX)
	//{
	//	sceneProperites.frame = 0;
	//}
	//

	//GLTFSceneManager::Update();
	model->Update(0.0f);
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

	ImGui::SliderFloat3("Position ", &model->GetGameObjectPositionPtr()->x, 0.0f, 1.0f);
	ImGui::SliderFloat3("Rotation ", &model->GetGameObjectRotationPtr()->x, 0.0f, 360.0f);
	ImGui::SliderFloat3("Scale ", &model->GetGameObjectScalePtr()->x, 0.0f, 1.0f);
	//if (GLTFSceneManager::EditorModeFlag)
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

	model->Destroy();
}
