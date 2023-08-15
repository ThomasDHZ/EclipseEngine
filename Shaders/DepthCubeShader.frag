#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable

#include "VertexLayout.glsl"
#include "MeshProperties.glsl"
#include "MaterialProperties.glsl"
#include "Lights.glsl"

layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec2 UV;

layout(binding = 0) uniform ViewSampler
{
    mat4 CubeMapFaceMatrix[6];
} viewSampler;
layout(binding = 1) buffer TransformBuffer { mat4 transform; } transformBuffer[];
layout(binding = 2) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
layout(binding = 3) buffer MaterialPropertiesBuffer { MaterialProperties materialProperties; } materialBuffer[];
layout(binding = 4) uniform sampler2D TextureMap[];

layout(push_constant) uniform DepthSceneData
{
    uint MeshIndex;
    uint LightIndex;
} sceneData;

void main()
{		
   const uint materialID = meshBuffer[sceneData.MeshIndex].meshProperties.MaterialBufferIndex;
   MaterialProperties material = materialBuffer[materialID].materialProperties;

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
