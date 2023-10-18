#pragma once
#include "WindowsIncludeStuff.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <vulkan/vulkan.h>

class HLSLShaderCompiler
{
private:
	static Microsoft::WRL::ComPtr<IDxcUtils> dxc_utils;
	static Microsoft::WRL::ComPtr<IDxcCompiler3> dxc_compiler;
	static Microsoft::WRL::ComPtr<IDxcIncludeHandler> DefaultIncludeHandler;
public:
	static void SetUpCompiler();
	static std::vector<uint32_t> BuildShader(const std::string& filename, VkShaderStageFlagBits stage);
};

