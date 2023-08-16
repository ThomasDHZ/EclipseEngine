#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable
#extension GL_EXT_multiview : enable

#include "Vertex.glsl"
#include "MeshProperties.glsl"
#include "MaterialProperties.glsl"
#include "LightProperties.glsl"

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in vec3 aColor;

layout(location = 0) out vec3 FragPos;
layout(location = 1) out vec2 UV;

layout(push_constant) uniform SceneData
{
  uint MeshIndex;
  uint MaterialIndex;
  uint LightIndex;
} sceneData;

layout(binding = 0) uniform ViewSampler
{
    mat4 CubeMapFaceMatrix[6];
} viewSampler[];
layout(binding = 1) buffer TransformBuffer { mat4 transform; } transformBuffer[];
layout(binding = 2) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];

void main() 
{
    mat4 MeshTransform = transformBuffer[sceneData.MeshIndex].transform;
    FragPos = vec3(MeshTransform * vec4(inPosition.xyz, 1.0));    

    UV = aUV;

    gl_Position = viewSampler[sceneData.LightIndex].CubeMapFaceMatrix[gl_ViewIndex] *               
                  MeshTransform * 
                  vec4(inPosition, 1.0);
}