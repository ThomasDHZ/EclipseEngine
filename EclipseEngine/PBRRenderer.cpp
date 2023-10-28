#include "PBRRenderer.h"
#include "SpriteGameObject3D.h"


PBRRenderer::PBRRenderer()
{
}

PBRRenderer::~PBRRenderer()
{
}

void PBRRenderer::BuildRenderer()
{
	GLTFSceneManager::sceneProperites.PBRMaxMipLevel = static_cast<uint32_t>(std::floor(std::log2(std::max(GLTFSceneManager::GetPreRenderedMapSize(), GLTFSceneManager::GetPreRenderedMapSize())))) + 1;
	GLTFSceneManager::EnvironmentTexture = std::make_shared<EnvironmentTexture>(VulkanRenderer::OpenFile("/texture/hdr/alps_field_4k.hdr"), VK_FORMAT_R32G32B32A32_SFLOAT);

	environmentToCubeRenderPass.BuildRenderPass(4096.0f / 4);
	brdfRenderPass.OneTimeDraw(GLTFSceneManager::GetPreRenderedMapSize());
	GLTFSceneManager::BRDFTexture = brdfRenderPass.GetImageTexture();
	//perlinNoise.BuildRenderPass(glm::ivec2(512));
	//voronoiNoiseRenderPass.BuildRenderPass(glm::ivec2(512));
	//valueNoiseRenderPass.BuildRenderPass(glm::ivec2(512));
	//auto AlbedoMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/pbr/gold/albedo.png"), TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_SRGB)));
	//multiplyRenderPass.BuildRenderPass(glm::ivec2(512), AlbedoMap, valueNoiseRenderPass.GetImageTexture());

	//GLTFSceneManager::UpdateBufferIndex();
	//GLTFSceneManager::GetMaterialList()[0]->SetNormalMap(multiplyRenderPass.GetImageTexture());
	//GLTFSceneManager::UpdateBufferIndex();
	auto asdf = GLTFSceneManager::GetMaterialList();
	//GLTFSceneManager::UpdateBufferIndex();
	//GLTFSceneManager::GetMaterialList()[0]->UpdateBuffer();
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
		{
			skyBoxReflectionIrradianceRenderPass.BuildRenderPass(GLTFSceneManager::CubeMap, GLTFSceneManager::GetPreRenderedMapSize());
			skyBoxReflectionPrefilterRenderPass.BuildRenderPass(GLTFSceneManager::CubeMap, GLTFSceneManager::GetPreRenderedMapSize());


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

			skyBoxReflectionRenderPass.BuildRenderPass(submitList, GLTFSceneManager::GetPreRenderedMapSize());
		}
		//Mesh Reflection Pass
		{
			meshReflectionIrradianceRenderPass.BuildRenderPass(skyBoxReflectionRenderPass.RenderedReflectionCubeMap, GLTFSceneManager::GetPreRenderedMapSize());
			meshReflectionPrefilterRenderPass.BuildRenderPass(skyBoxReflectionRenderPass.RenderedReflectionCubeMap, GLTFSceneManager::GetPreRenderedMapSize());

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

			meshReflectionRenderPass.BuildRenderPass(submitList, GLTFSceneManager::GetPreRenderedMapSize());
		}
		//Mesh Reflection Pass
		{
			irradianceRenderPass.BuildRenderPass(meshReflectionRenderPass.RenderedReflectionCubeMap, GLTFSceneManager::GetPreRenderedMapSize());
			prefilterRenderPass.BuildRenderPass(meshReflectionRenderPass.RenderedReflectionCubeMap, GLTFSceneManager::GetPreRenderedMapSize());

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
	cubeMapToEnvironmentRenderPass.BuildRenderPass(meshReflectionRenderPass.RenderedReflectionCubeMap, glm::ivec2(4096.0f / 2, 1024));
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
	cubeMapToEnvironmentRenderPass.GetImageTexture()->ImGuiShowTexture(ImVec2(256, 128));
	if(ImGui::Button("Save Texture", ImVec2(50, 25)))
	{
		cubeMapToEnvironmentRenderPass.SaveTexture("../adsfasdf24535.HDR");
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

	/*if (SelectedMesh)
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
	}*/
	
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
	//CommandBufferSubmitList.emplace_back(perlinNoise.Draw((float)glfwGetTime()));
	//CommandBufferSubmitList.emplace_back(voronoiNoiseRenderPass.Draw((float)glfwGetTime()));
	//CommandBufferSubmitList.emplace_back(multiplyRenderPass.Draw((float)glfwGetTime()));
	//CommandBufferSubmitList.emplace_back(valueNoiseRenderPass.Draw((float)glfwGetTime()));

	//CommandBufferSubmitList.emplace_back(brdfRenderPass.Draw());
	
	CommandBufferSubmitList.emplace_back(environmentToCubeRenderPass.Draw());
	CommandBufferSubmitList.emplace_back(cubeMapToEnvironmentRenderPass.Draw((float)glfwGetTime()));

	CommandBufferSubmitList.emplace_back(depthRenderPass.Draw());
	CommandBufferSubmitList.emplace_back(depthCubeMapRenderPass.Draw());
	CommandBufferSubmitList.emplace_back(depthSpotLightRenderPass.Draw());

	CommandBufferSubmitList.emplace_back(skyBoxReflectionIrradianceRenderPass.Draw());
	CommandBufferSubmitList.emplace_back(skyBoxReflectionPrefilterRenderPass.Draw());
	CommandBufferSubmitList.emplace_back(skyBoxReflectionRenderPass.Draw(glm::vec3(0.245790839f, 3.02915239f, -0.0890803784f)));

	CommandBufferSubmitList.emplace_back(meshReflectionIrradianceRenderPass.Draw());
	CommandBufferSubmitList.emplace_back(meshReflectionPrefilterRenderPass.Draw());
	CommandBufferSubmitList.emplace_back(meshReflectionRenderPass.Draw(glm::vec3(0.245790839f, 3.02915239f, -0.0890803784f)));

	CommandBufferSubmitList.emplace_back(irradianceRenderPass.Draw());
	CommandBufferSubmitList.emplace_back(prefilterRenderPass.Draw());
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