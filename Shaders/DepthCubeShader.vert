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

layout(binding = 0) uniform ViewSampler2
{
    mat4 CubeMapFaceMatrix;
} viewSampler2;
layout(binding = 1) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];

layout(push_constant) uniform SceneData
{
  uint MeshIndex;
  uint LightIndex;
} sceneData;


void main() {

    gl_Position = viewSampler2.CubeMapFaceMatrix[sceneData.LightIndex] * 
                  meshBuffer[sceneData.MeshIndex].meshProperties.GameObjectTransform * 
                  meshBuffer[sceneData.MeshIndex].meshProperties.ModelTransform * 
                  meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform * 
                  vec4(inPosition, 1.0);
}