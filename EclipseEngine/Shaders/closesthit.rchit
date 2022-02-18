#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable
#include "MeshProperties.glsl"
layout(push_constant) uniform SceneData
{
    uint MeshIndex;
    mat4 proj;
    mat4 view;
    vec3 CameraPos;
    float Timer;
} sceneData;

struct RayPayload {
	vec3 color;
	uint seed;
	vec3 normal;
    int reflectCount;
	bool hitGeo;
};

layout(location = 0) rayPayloadInEXT RayPayload rayHitInfo;


layout(binding = 0, set = 0) uniform accelerationStructureEXT topLevelAS;
layout(binding = 1, set = 0, rgba8) uniform image2D RayTracedTexture;
layout(binding = 2) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
layout(binding = 3) uniform sampler2D TextureMap[];

void main()
{
    rayHitInfo.color = vec3(0.0f, 1.0f, 0.0f);
}
