#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable

layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec2 UV;

#include "Vertex.glsl"
#include "MeshProperties.glsl"
#include "MaterialProperties.glsl"
#include "LightProperties.glsl"

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

#include "RasterDepthVertexBuilder.glsl"
#include "MaterialBuilder.glsl"

void main()
{		
    Vertex vertex = RasterDepthVertexBuilder();
	MaterialProperties material = BuildDepthMaterial(sceneData.MaterialIndex, vertex.UV);

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

