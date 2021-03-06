#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable

#include "MeshProperties.glsl"

layout(location = 0) in vec2 Position;
layout(location = 1) in vec2 UV;
layout(location = 2) in vec3 Color;

layout(location = 0) out vec4 outColor;

layout(binding = 0) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
layout(binding = 1) uniform sampler2D TextureMap[];

layout(push_constant) uniform SceneData
{
    uint MeshIndex;
    mat4 proj;
    mat4 view;
    vec3 CameraPos;
    vec3 MeshColorID;
    vec3 AmbientLight;
    uint DirectionalLightCount;
    uint PointLightCount;
    uint SpotLightCount;
    float Timer;
} sceneData;

void main() {
   uint diffuse = meshBuffer[sceneData.MeshIndex].meshProperties.materialProperties.DiffuseMapID;
   uint alpha = meshBuffer[sceneData.MeshIndex].meshProperties.materialProperties.AlphaMapID;

   vec3 result = texture(TextureMap[diffuse], UV).rgb;
   	 vec3 finalResult = vec3(1.0) - exp(-result * 1.0f);
		  finalResult = pow(finalResult, vec3(1.0 / 2.2f));
   outColor = vec4(finalResult, texture(TextureMap[alpha], UV).r);
}