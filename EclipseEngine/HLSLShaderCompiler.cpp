#include "HLSLShaderCompiler.h"
#include <algorithm>

Microsoft::WRL::ComPtr<IDxcUtils> HLSLShaderCompiler::dxc_utils;
Microsoft::WRL::ComPtr<IDxcCompiler3> HLSLShaderCompiler::dxc_compiler;
Microsoft::WRL::ComPtr<IDxcIncludeHandler> HLSLShaderCompiler::DefaultIncludeHandler;

std::string ConvertLPCWSTRToString(LPCWSTR lpcwszStr)
{
    // Determine the length of the converted string 
    int strLength
        = WideCharToMultiByte(CP_UTF8, 0, lpcwszStr, -1,
            nullptr, 0, nullptr, nullptr);

    // Create a std::string with the determined length 
    std::string str(strLength, 0);

    // Perform the conversion from LPCWSTR to std::string 
    WideCharToMultiByte(CP_UTF8, 0, lpcwszStr, -1, &str[0],
        strLength, nullptr, nullptr);

    // Return the converted std::string 
    return str;
}

void HLSLShaderCompiler::SetUpCompiler()
{
    DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(dxc_utils.ReleaseAndGetAddressOf()));
    DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxc_compiler));
    dxc_utils->CreateDefaultIncludeHandler(&DefaultIncludeHandler);
}

std::vector<uint32_t> HLSLShaderCompiler::BuildShader(const std::string filename, VkShaderStageFlagBits stage)
{
    std::ifstream ifs(File::OpenFile(filename));
    std::string str;
    ifs.seekg(0, std::ios::end);
    str.reserve(ifs.tellg());
    ifs.seekg(0, std::ios::beg);
    str.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());

    Microsoft::WRL::ComPtr<IDxcUtils> dxc_utils{};
    Microsoft::WRL::ComPtr<IDxcCompiler3> dxc_compiler{};
    Microsoft::WRL::ComPtr<IDxcIncludeHandler> DefaultIncludeHandler;

    DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(dxc_utils.ReleaseAndGetAddressOf()));
    DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxc_compiler));
    dxc_utils->CreateDefaultIncludeHandler(&DefaultIncludeHandler);

    DxcBuffer src_buffer = {
        .Ptr = str.c_str(),
        .Size = static_cast<uint32_t>(str.size()),
        .Encoding = 0
    };

    std::vector<LPCWSTR> args;
    args.emplace_back(L"-spirv");
    args.emplace_back(L"-fspv-target-env=vulkan1.3");
    switch (stage)
    {
    case VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT: args.emplace_back(L"-T vs_6_5"); break;
    case VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT: args.emplace_back(L"-T ps_6_5"); break;
    case VkShaderStageFlagBits::VK_SHADER_STAGE_COMPUTE_BIT: args.emplace_back(L"-T cs_6_5"); break;
    }

    for (int x = 0; x < args.size(); x++)
    {

        std::cout << ConvertLPCWSTRToString(args[x]) << std::endl;
    }

    Microsoft::WRL::ComPtr<IDxcResult> result;
    dxc_compiler->Compile(&src_buffer, args.data(), static_cast<uint32_t>(args.size()), DefaultIncludeHandler.Get(), IID_PPV_ARGS(&result));

    Microsoft::WRL::ComPtr<IDxcBlob> shader_obj;
    result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shader_obj), nullptr);

    Microsoft::WRL::ComPtr<IDxcBlobUtf8> error_message;
    result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&error_message), nullptr);
    if (error_message && error_message->GetStringLength() > 0)
    {
        auto string = std::string();
        string.resize(error_message->GetBufferSize());
        for (size_t x = 0; x < string.size(); x++)
        {
            string[x] = static_cast<const char*>(error_message->GetBufferPointer())[x];
        }

        std::cout << string << std::endl;
        throw std::runtime_error("Error found in: " + filename);
    }

    std::vector<uint32_t> spriv_buffer;
    spriv_buffer.resize(shader_obj->GetBufferSize() / sizeof(uint32_t));

    for (int x = 0; x < spriv_buffer.size(); x++)
    {
        uint32_t spv_uint = static_cast<std::uint32_t*>(shader_obj->GetBufferPointer())[x];
        spriv_buffer[x] = spv_uint;
    }

    return spriv_buffer;
}
