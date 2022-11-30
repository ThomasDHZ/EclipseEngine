#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable

#include "MeshProperties.glsl"
#include "MaterialProperties.glsl"

layout (location = 0) in vec2 InPosition;
layout (location = 1) in vec2 InUV;
layout (location = 2) in vec3 InColor;

layout(location = 0) out vec2 FragPos;
layout(location = 1) out vec2 UV;
layout(location = 2) out vec3 Color;

layout(binding = 0) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];

layout(push_constant) uniform SceneData
{
    uint MeshIndex;
    mat4 proj;
    mat4 view;
    vec3 CameraPos;
    vec3 MeshColorID;
    float Timer;
} sceneData;
void main() {
    gl_Position = sceneData.proj * 
                  sceneData.view *                
                  meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform * 
                  vec4(InPosition, 0.0f, 1.0);
    Color = InColor;
    UV = InUV;
}