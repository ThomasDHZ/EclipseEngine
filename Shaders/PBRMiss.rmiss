#version 460
#extension GL_EXT_ray_tracing : enable
#extension GL_EXT_scalar_block_layout : enable

struct RayHitInfo
{
	vec3 color;
	uint seed;
    int reflectCount;
};

layout(location = 0) rayPayloadInEXT RayHitInfo rayPayload;

#include "RTXPBRBindingLayout.glsl"

void main()
{
	rayPayload.color = texture(CubeMap, gl_WorldRayDirectionEXT).rgb;
}