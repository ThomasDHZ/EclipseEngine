#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable

layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec2 UV;

#include "VertexLayout.glsl"
struct MeshProperties
{
	uint VertexBufferIndex;
	uint IndexBufferIndex;
	uint MaterialBufferIndex;
	uint AlbedoMapIndex;
	uint MetallicRoughnessMapIndex;
	uint AmbientOcclusionMapIndex;
	uint NormalMapIndex;
	uint DepthMapIndex;
	uint AlphaMapIndex;
	uint EmissionMapIndex;
	uint SkyBoxIndex;
	mat4 MeshTransform;
	vec2 UVOffset;
	vec2 UVScale;
	vec2 UVFlip;
	int SelectedMesh;
	float heightScale;
	float minLayers;
	float maxLayers;
};

struct MaterialProperties
{
	vec3 Albedo;
	float Metallic;
	float Roughness;
	float AmbientOcclusion;
	vec3 Emission;
	float Alpha;

	uint AlbedoMap;
	uint MetallicRoughnessMap;
	uint AmbientOcclusionMap;
	uint NormalMap;
	uint DepthMap;
	uint AlphaMap;
	uint EmissionMap;
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

layout(binding = 0) buffer TransformBuffer { mat4 transform; } transformBuffer[];
layout(binding = 1) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
layout(binding = 2) buffer MaterialPropertiesBuffer { MaterialProperties materialProperties; } materialBuffer[];
layout(binding = 3) uniform sampler2D TextureMap[];

layout(push_constant) uniform DirectionalLightProjection
{
    uint MeshIndex;
	uint MaterialIndex;
    uint LightIndex;
} sceneData;

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

Vertex RasterVertexBuilder()
{
	Vertex vertex;
	vertex.Position = FragPos;
	vertex.UV = UV;
	vertex.Normal = vec3(0.0f);

	vertex.UV = vertex.UV + meshBuffer[sceneData.MeshIndex].meshProperties.UVOffset;
	vertex.UV *= meshBuffer[sceneData.MeshIndex].meshProperties.UVScale;

	if (meshBuffer[sceneData.MeshIndex].meshProperties.UVFlip.y == 1.0f)
	{
		vertex.UV.y = 1.0f - vertex.UV.y;
	}
	if (meshBuffer[sceneData.MeshIndex].meshProperties.UVFlip.x == 1.0f)
	{
		vertex.UV.x = 1.0f - vertex.UV.x;
	}

	return vertex;
}

MaterialProperties BuildPBRMaterial(uint materialId, vec2 UV)
{
	MaterialProperties material = materialBuffer[materialId].materialProperties;

		if (texture(TextureMap[material.AlphaMap], UV).r == 0.0f ||
			texture(TextureMap[material.AlphaMap], UV).a == 0.0f)
	{
		discard;
	}
	return material;
}

void main()
{		
    Vertex vertex = RasterVertexBuilder();
	MaterialProperties material = BuildPBRMaterial(sceneData.MaterialIndex, vertex.UV);

   vec2 FinalUV = UV + meshBuffer[sceneData.MeshIndex].meshProperties.UVOffset;
        FinalUV *= meshBuffer[sceneData.MeshIndex].meshProperties.UVScale;

   if(texture(TextureMap[material.AlphaMap], FinalUV).r == 0.0f ||
      texture(TextureMap[material.AlbedoMap], FinalUV).a == 0.0f)
   {
	 discard;
   }
   if(meshBuffer[sceneData.MeshIndex].meshProperties.UVFlip.y == 1.0f)
   {
        FinalUV.y = 1.0f - FinalUV.y;
   }
   if(meshBuffer[sceneData.MeshIndex].meshProperties.UVFlip.x == 1.0f)
   {
        FinalUV.x = 1.0f - FinalUV.x;
   }
}

