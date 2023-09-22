#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable

#include "MeshProperties.glsl"
#include "MaterialProperties.glsl"

layout(location = 0) in vec2 Position;
layout(location = 1) in vec2 VertexUV;
layout(location = 2) in vec3 Color;
layout(location = 3) in vec2 UVOffset;
layout(location = 4) in flat int MaterialID;
layout(location = 5) in vec2 SpriteFlip;
//layout(location = 6) in flat int PixelOffset;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outBloom;

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
layout(binding = 2) buffer MaterialPropertiesBuffer { MaterialProperties materialProperties; } materialBuffer[];
layout(binding = 3) uniform sampler2D TextureMap[];

layout(push_constant) uniform SceneData
{
    uint MeshIndex;
    uint PrimitiveIndex;
    uint MaterialIndex;
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

void main() {
	vec2 UV = VertexUV + UVOffset;
	if (SpriteFlip.x == 1.0f)
    {
        UV.x = 1.0 - UV.x;
    }
	if (SpriteFlip.y == 1.0f)
    {
        UV.y = 1.0 - UV.y;
    }

	MaterialProperties material = materialBuffer[MaterialID].materialProperties;
	material.Albedo = texture(TextureMap[material.AlbedoMap], UV).rgb;
	//material.Alpha = texture(TextureMap[material.AlbedoMap], UV).a;
	material.CheckPixelForPaletteSwapColor = texture(TextureMap[material.CheckPixelForPaletteSwapColorMap], UV).rgb;
	material.PaletteSwapPixelColor = texture(TextureMap[material.PaletteSwapPixelColorMap], UV).rgb;

	if(material.CheckPixelForPaletteSwapColor == texelFetch(TextureMap[material.PaletteSwapPixelColorMap], ivec2(0), 0).rgb)
	{
		int palettePixel = (int(sceneData.Timer * 5) % 6) + 1;
		material.PaletteSwapPixelColor = texelFetch(TextureMap[material.PaletteSwapPixelColorMap], ivec2(palettePixel, 0), 0).rgb;
	}
	else
	{
		material.PaletteSwapPixelColor = vec3(0.0f, 0.0f, 0.0f);
	}


	/*if(material.Alpha != 1.0f)
	{
		discard;
	}*/

   vec3 result = material.Albedo;
      if(material.PaletteSwapPixelColor.r != 0.0f &&
   material.PaletteSwapPixelColor.g != 0.0f &&
   material.PaletteSwapPixelColor.b != 0.0f)
   {
		result = material.PaletteSwapPixelColor;
   }

   vec3 finalResult = vec3(1.0) - exp(-result * 1.0f);
		finalResult = pow(finalResult, vec3(1.0 / 2.2f));

   outColor = vec4(finalResult,1.0f);
   if(material.PaletteSwapPixelColor.r != 0.0f &&
   material.PaletteSwapPixelColor.g != 0.0f &&
   material.PaletteSwapPixelColor.b != 0.0f)
   {
		outBloom = vec4(material.PaletteSwapPixelColor, 1.0f);
   }
   else
   {
	outBloom = vec4(0.0f, 0.0f, 0.0f, 1.0f);
   }
}
