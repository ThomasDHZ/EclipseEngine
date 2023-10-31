#pragma once
#include "WindowsIncludeStuff.h"
#include "File.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <vulkan/vulkan.h>
#include <fstream>

class HLSLShaderCompiler
{
private:
	static Microsoft::WRL::ComPtr<IDxcUtils> dxc_utils;
	static Microsoft::WRL::ComPtr<IDxcCompiler3> dxc_compiler;
	static Microsoft::WRL::ComPtr<IDxcIncludeHandler> DefaultIncludeHandler;
public:
	static void SetUpCompiler();
	static Microsoft::WRL::ComPtr<IDxcBlob> BuildShader(const std::string filename, VkShaderStageFlagBits stage);
};

