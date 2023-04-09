#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable

#include "MeshProperties.glsl"
#include "MaterialProperties.glsl"

layout(location = 0) in vec2 Position;
layout(location = 1) in vec2 UV;
layout(location = 2) in vec3 Color;

layout(location = 0) out vec4 outColor;

struct SunLight
{
	vec3 diffuse;
	vec3 position;
	mat4 LightSpaceMatrix;
	float intensity;
};

struct DirectionalLight
{
	vec3 diffuse;
	vec3 direction;
	mat4 LightSpaceMatrix;
	float intensity;
};

struct PointLight
{
	vec3 diffuse;
	vec3 position;
	mat4 LightSpaceMatrix;
	float intensity;
	float radius;
};

struct SpotLight
{
	vec3 diffuse;
	vec3 position;
	vec3 direction;
	mat4 LightSpaceMatrix;
	float intensity;

    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
    mat4 lightSpaceMatrix;
};

struct PBRMaterial
{
	vec3 Albedo;
	float Metallic;
	float Roughness;
	float AmbientOcclusion;
	vec3 Emission;
	float Alpha;

	uint NormalMapID;
	uint DepthMapID;
};

layout(binding = 0) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
layout(binding = 1) buffer TransformBuffer { mat4 transform; } transformBuffer[];
layout(binding = 2) uniform sampler2D AlbedoMap;
layout(binding = 3) uniform sampler2D NormalMap;
layout(binding = 4) uniform sampler2D MetallicRoughnessMap;
layout(binding = 5) uniform sampler2D AmbientOcclusionMap;
layout(binding = 6) uniform sampler2D AlphaMap;
layout(binding = 7) uniform sampler2D DepthMap;
layout(binding = 8) uniform sampler2D BRDFMap;
layout(binding = 9) uniform samplerCube IrradianceMap;
layout(binding = 10) uniform samplerCube PrefilterMap;
layout(binding = 11) buffer SunLightBuffer { SunLight sunLight; } SULight[];
layout(binding = 12) buffer DirectionalLightBuffer { DirectionalLight directionalLight; } DLight[];
layout(binding = 13) buffer PointLightBuffer { PointLight pointLight; } PLight[];
layout(binding = 14) buffer SpotLightBuffer { SpotLight spotLight; } SLight[];

layout(push_constant) uniform SceneData
{
    uint MeshIndex;
    mat4 proj;
    mat4 view;
    vec3 CameraPos;
    vec3 MeshColorID;
    vec3 AmbientLight;
    uint SunLightCount;
    uint DirectionalLightCount;
    uint PointLightCount;
    uint SpotLightCount;
    float Timer;
    float PBRMaxMipLevel;
    uint frame;
    int MaxRefeflectCount;
} sceneData;

PBRMaterial BuildPBRMaterial(vec2 UV)
{
	PBRMaterial material;
    material.Albedo = texture(AlbedoMap, UV).rgb;
	material.Metallic = texture(MetallicRoughnessMap, UV).b;
	material.Roughness = texture(MetallicRoughnessMap, UV).g;
    material.AmbientOcclusion = texture(MetallicRoughnessMap, UV).r;
	//material.Emission = .4f;
	material.Alpha = texture(AlphaMap, UV).r;
    return material;
}

void main() {
    PBRMaterial pbrMaterial = BuildPBRMaterial(UV);

   vec3 result = pbrMaterial.Albedo;

   vec3 finalResult = vec3(1.0) - exp(-result * 1.0f);
		finalResult = pow(finalResult, vec3(1.0 / 2.2f));

   outColor = vec4(finalResult, pbrMaterial.Alpha);
}
