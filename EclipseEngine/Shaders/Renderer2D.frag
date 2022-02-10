#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable

#include "MeshProperties.glsl"

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

layout(binding = 0) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
layout(binding = 1) uniform sampler2D TextureMap[];

layout(push_constant) uniform SceneData
{
    uint MeshIndex;
    mat4 proj;
    mat4 view;
    vec3 CameraPos;
    float Timer;
} sceneData;

void main() {
   uint material = meshBuffer[sceneData.MeshIndex].meshProperties.materialProperties.DiffuseMapID;
   outColor = vec4(texture(TextureMap[material], fragUV).rgb, 1.0f);
}