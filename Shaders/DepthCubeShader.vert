#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable
#extension GL_EXT_multiview : enable

#include "MeshProperties.glsl"

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in vec3 aColor;

layout(location = 0) out vec3 FragPos;
layout(location = 1) out vec2 UV;

layout(binding = 0) uniform ViewSampler
{
    mat4 CubeMapFaceMatrix;
} viewSampler;
layout(binding = 1) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];

layout(push_constant) uniform SceneData
{
  uint MeshIndex;
  uint LightIndex;
} sceneData;


void main() {
    FragPos = vec3(viewSampler.CubeMapFaceMatrix[sceneData.LightIndex] * 
                   meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform * 
                   vec4(inPosition.xyz, 1.0));    
    UV = aUV;
    gl_Position = viewSampler.CubeMapFaceMatrix[sceneData.LightIndex] * 
                  meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform * 
                  vec4(inPosition, 1.0);
}