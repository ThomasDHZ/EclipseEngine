#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable
#extension GL_EXT_ray_tracing : enable
#extension GL_EXT_ray_query : enable

#include "VertexLayout.glsl"
#include "MeshProperties.glsl"
#include "Lights.glsl"

layout(location = 0) in vec2 TexCoords;
layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform sampler2D PositionTexture;
layout(binding = 1) uniform sampler2D NormalTexture;
layout(binding = 2) uniform sampler2D AlbedoTexture;
layout(binding = 3) uniform sampler2D SpecularTexture;
layout(binding = 4) uniform sampler2D BloomTexture;
layout(binding = 5) uniform sampler2D ShadowTexture;

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

vec2 ParallaxMapping(MaterialProperties material, vec2 texCoords, vec3 viewDir);

void main() 
{
    vec3 FragPos = texture(PositionTexture, TexCoords).rgb;
    vec3 Normal = texture(NormalTexture, TexCoords).rgb;
    vec3 Diffuse = texture(AlbedoTexture, TexCoords).rgb;
    float Specular = texture(SpecularTexture, TexCoords).r;

    outColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}