#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable

#include "MeshProperties.glsl"
#include "Lights.glsl"

layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec2 UV;
layout(location = 2) in vec3 Normal;
layout(location = 3) in vec3 Tangent;
layout(location = 4) in vec3 BiTangent;
layout(location = 5) in vec3 Color;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outBloom;

layout(binding = 0) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
layout(binding = 1) buffer DirectionalLightBuffer { DirectionalLight directionalLight; } DLight[];
layout(binding = 2) buffer PointLightBuffer { PointLight pointLight; } PLight[];
layout(binding = 3) buffer SpotLightBuffer { SpotLight spotLight; } SLight[];
layout(binding = 4) uniform sampler2D TextureMap[];

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
   uint diffuse = meshBuffer[sceneData.MeshIndex].meshProperties.materialProperties.DiffuseMapID;
   uint alpha = meshBuffer[sceneData.MeshIndex].meshProperties.materialProperties.AlphaMapID;
   outColor = vec4(texture(TextureMap[diffuse], UV).rgb, texture(TextureMap[alpha], UV).r);
   outBloom = vec4(1.0f);
}