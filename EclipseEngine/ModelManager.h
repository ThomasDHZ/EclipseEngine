#pragma once
#include "VulkanRenderer.h"
#include "Model.h"
#include "GraphicsPipeline.h"

class ModelManager
{
private: 
	static std::vector<std::shared_ptr<Model>> ModelList;
private:

	static void Update()
	{
		for (auto& model : ModelList)
		{
			model->Update();
		}
	}
};

