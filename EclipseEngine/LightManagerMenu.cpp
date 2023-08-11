#include "LightManagerMenu.h"

LightManagerMenu::LightManagerMenu()
{
	Update();
}

LightManagerMenu::~LightManagerMenu()
{
}

void LightManagerMenu::Update()
{
	for (int x = 0; x < GLTFSceneManager::GetAllLights().size(); x++)
	{
		std::string light = GLTFSceneManager::GetAllLights()[x]->GetLightName();
		lightcharstring.emplace_back(light);
	}
	for (auto& list : lightcharstring)
	{
		lightListchar.emplace_back(list.c_str());
	}
}

void LightManagerMenu::ImGuiUpdate()
{
	ImGui::Begin("Light Manager");
	ImGui::Separator();

	ImGui::ListBox("Lights", &lightSelection, lightListchar.data(), lightListchar.size());
	if (lightSelection != -1)
	{
		for (int x = 0; x < GLTFSceneManager::GetAllLights().size(); x++)
		{
			if (x == lightSelection)
			{
				GLTFSceneManager::GetAllLights()[x]->SetSelectedLight(true);
			}
			else
			{
				GLTFSceneManager::GetAllLights()[x]->SetSelectedLight(false);
			}
		}

		if (GLTFSceneManager::GetAllLights()[lightSelection]->GetLightType() == LightTypeEnum::kSunLight)
		{
			uint32_t sunLightOffset = 0;
			uint32_t sunLightIndex = lightSelection - sunLightOffset;

			ImGui::LabelText(GLTFSceneManager::GetPointLights()[sunLightIndex]->GetLightName().c_str(), "");
			ImGui::Checkbox("Static Light", GLTFSceneManager::GetSunLights()[sunLightIndex]->GetStaticLightStatusPtr());

			if (!GLTFSceneManager::GetSunLights()[sunLightIndex]->GetStaticLightStatus())
			{
				ImGui::SliderFloat3(("Light position " + std::to_string(1)).c_str(), &GLTFSceneManager::GetSunLights()[sunLightIndex]->GetPositionPtr()->x, -1.0f, 1.0f);
			}
			ImGui::SliderFloat3(("Light Diffuse " + std::to_string(1)).c_str(), &GLTFSceneManager::GetSunLights()[sunLightIndex]->GetDiffusePtr()->x, -100.0f, 100.0f);
			ImGui::SliderFloat(("Light Intensity " + std::to_string(1)).c_str(), &GLTFSceneManager::GetSunLights()[sunLightIndex]->GetIntensityPtr()[0], 0.0f, 100.0f);

			//GLTFSceneManager::GetSunLights()[sunLightIndex]->CubeMapSide[0]->ImGuiShowTexture(ImVec2(100.0f, 100.0f));
			//ImGui::SameLine();
			//GLTFSceneManager::GetSunLights()[sunLightIndex]->CubeMapSide[1]->ImGuiShowTexture(ImVec2(100.0f, 100.0f));
			//ImGui::SameLine();
			//GLTFSceneManager::GetSunLights()[sunLightIndex]->CubeMapSide[2]->ImGuiShowTexture(ImVec2(100.0f, 100.0f));
			//ImGui::SameLine();
			//GLTFSceneManager::GetSunLights()[sunLightIndex]->CubeMapSide[3]->ImGuiShowTexture(ImVec2(100.0f, 100.0f));
			//ImGui::SameLine();
			//GLTFSceneManager::GetSunLights()[sunLightIndex]->CubeMapSide[4]->ImGuiShowTexture(ImVec2(100.0f, 100.0f));
			//ImGui::SameLine();
			//GLTFSceneManager::GetSunLights()[sunLightIndex]->CubeMapSide[5]->ImGuiShowTexture(ImVec2(100.0f, 100.0f));
		}
		if (GLTFSceneManager::GetAllLights()[lightSelection]->GetLightType() == LightTypeEnum::kDirectionalLight)
		{
			uint32_t directionLightOffset = GLTFSceneManager::GetSunLightCount();
			uint32_t directionLightIndex = lightSelection - directionLightOffset;

			ImGui::LabelText(GLTFSceneManager::GetDirectionalLights()[directionLightIndex]->GetLightName().c_str(), "");
			ImGui::Checkbox("Static Light", GLTFSceneManager::GetDirectionalLights()[directionLightIndex]->GetStaticLightStatusPtr());

			if (!GLTFSceneManager::GetDirectionalLights()[directionLightIndex]->GetStaticLightStatus())
			{
				ImGui::SliderFloat3("Direction ", &GLTFSceneManager::GetDirectionalLights()[directionLightIndex]->GetDirectionPtr()->x, -1.0f, 1.0f);
			}

			ImGui::SliderFloat3("Diffuse ", &GLTFSceneManager::GetDirectionalLights()[directionLightIndex]->GetDiffusePtr()->x, 0.0f, 1.0f);
			ImGui::SliderFloat("Intensity ", &GLTFSceneManager::GetDirectionalLights()[directionLightIndex]->GetIntensityPtr()[0], 0.0f, 100.0f);

			ImGui::SliderFloat2("LeftRight ", &GLTFSceneManager::GetDirectionalLights()[directionLightIndex]->GetLeftRightPtr()->x, -100.0f, 100.0f);
			ImGui::SliderFloat2("NearFar ", &GLTFSceneManager::GetDirectionalLights()[directionLightIndex]->GetNearFarPtr()->x, -100.0f, 100.0f);
			ImGui::SliderFloat2("TopBottom ", &GLTFSceneManager::GetDirectionalLights()[directionLightIndex]->GetTopBottomPtr()->x, -100.0f, 100.0f);

			GLTFSceneManager::GetDirectionalLights()[directionLightIndex]->LightViewTexture->ImGuiShowTexture(ImVec2(100.0f, 100.0f));
		}
		if (GLTFSceneManager::GetAllLights()[lightSelection]->GetLightType() == LightTypeEnum::kPointLight)
		{
			uint32_t pointLightOffset = GLTFSceneManager::GetSunLightCount() + GLTFSceneManager::GetDirectionalCount();
			uint32_t pointLightIndex = lightSelection - pointLightOffset;

			ImGui::LabelText(GLTFSceneManager::GetPointLights()[pointLightIndex]->GetLightName().c_str(), "");
			ImGui::Checkbox("Static Light", GLTFSceneManager::GetPointLights()[pointLightIndex]->GetStaticLightStatusPtr());

			if (!GLTFSceneManager::GetPointLights()[pointLightIndex]->GetStaticLightStatus())
			{
				ImGui::SliderFloat3("Position", &GLTFSceneManager::GetPointLights()[pointLightIndex]->GetPositionPtr()->x, -50.0f, 50.0f);
			}

			ImGui::SliderFloat3("Diffuse", &GLTFSceneManager::GetPointLights()[pointLightIndex]->GetDiffusePtr()->x, 0.0f, 1.0f);
			ImGui::SliderFloat("Radius", GLTFSceneManager::GetPointLights()[pointLightIndex]->GetRadiusPtr(), 0.0f, 15.0f);
			ImGui::SliderFloat("Intensity", &GLTFSceneManager::GetPointLights()[pointLightIndex]->GetIntensityPtr()[0], 0.0f, 100.0f);

			GLTFSceneManager::GetPointLights()[pointLightIndex]->CubeMapSide[0]->ImGuiShowTexture(ImVec2(100.0f, 100.0f));
			ImGui::SameLine();
			GLTFSceneManager::GetPointLights()[pointLightIndex]->CubeMapSide[1]->ImGuiShowTexture(ImVec2(100.0f, 100.0f));
			ImGui::SameLine();
			GLTFSceneManager::GetPointLights()[pointLightIndex]->CubeMapSide[2]->ImGuiShowTexture(ImVec2(100.0f, 100.0f));
			ImGui::SameLine();
			GLTFSceneManager::GetPointLights()[pointLightIndex]->CubeMapSide[3]->ImGuiShowTexture(ImVec2(100.0f, 100.0f));
			ImGui::SameLine();
			GLTFSceneManager::GetPointLights()[pointLightIndex]->CubeMapSide[4]->ImGuiShowTexture(ImVec2(100.0f, 100.0f));
			ImGui::SameLine();
			GLTFSceneManager::GetPointLights()[pointLightIndex]->CubeMapSide[5]->ImGuiShowTexture(ImVec2(100.0f, 100.0f));
		}
		if (GLTFSceneManager::GetAllLights()[lightSelection]->GetLightType() == LightTypeEnum::kSpotLight)
		{
			uint32_t spotLightOffset = GLTFSceneManager::GetSunLightCount() + GLTFSceneManager::GetDirectionalCount() + GLTFSceneManager::GetPointLightCount();
			uint32_t spotLightIndex = lightSelection - spotLightOffset;

			ImGui::LabelText(GLTFSceneManager::GetSpotLights()[spotLightIndex]->GetLightName().c_str(), "");
			ImGui::Checkbox("Static Light", GLTFSceneManager::GetSpotLights()[spotLightIndex]->GetStaticLightStatusPtr());

			if (!GLTFSceneManager::GetSpotLights()[spotLightIndex]->GetStaticLightStatus())
			{
				ImGui::SliderFloat3("Position", &GLTFSceneManager::GetSpotLights()[spotLightIndex]->GetPositionPtr()->x, -1.0f, 1.0f);
				ImGui::SliderFloat3("Direction", &GLTFSceneManager::GetSpotLights()[spotLightIndex]->GetDirectionPtr()->x, -50.0f, 50.0f);
			}
			ImGui::SliderFloat("Radius", GLTFSceneManager::GetSpotLights()[spotLightIndex]->GetRadiusPtr(), 0.0f, 15.0f);
			ImGui::SliderFloat("Intensity", &GLTFSceneManager::GetSpotLights()[spotLightIndex]->GetIntensityPtr()[0], 0.0f, 100.0f);
			ImGui::SliderFloat("cutOff", GLTFSceneManager::GetSpotLights()[spotLightIndex]->GetCutOffPtr(), 0.0f, 15.0f);
			ImGui::SliderFloat("outerCutOff", &GLTFSceneManager::GetSpotLights()[spotLightIndex]->GetOuterCutOffPtr()[0], 0.0f, 100.0f);
			ImGui::SliderFloat("constant", GLTFSceneManager::GetSpotLights()[spotLightIndex]->GetConstantPtr(), 0.0f, 15.0f);
			ImGui::SliderFloat("linear", &GLTFSceneManager::GetSpotLights()[spotLightIndex]->GetLinearPtr()[0], 0.0f, 100.0f);
			ImGui::SliderFloat("quadratic", &GLTFSceneManager::GetSpotLights()[spotLightIndex]->GetQuadraticPtr()[0], 0.0f, 100.0f);

			GLTFSceneManager::GetSpotLights()[spotLightIndex]->LightViewTexture->ImGuiShowTexture(ImVec2(100.0f, 100.0f));
		}
	}
	ImGui::End();
}
