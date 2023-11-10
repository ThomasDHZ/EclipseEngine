#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable

#include "MeshProperties.glsl"

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec4 aColor;

layout(location = 0) out vec4 Color;

layout(binding = 0) buffer TransformBuffer { mat4 transform; } transformBuffer[];

layout(push_constant) uniform SceneData
{
    uint MeshIndex;
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

gl_Position = sceneData.proj * 
                  sceneData.view *                
                  transformBuffer[sceneData.MeshIndex].transform * 
                  vec4(inPosition, 1.0);
    Color = aColor;
}