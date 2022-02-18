#version 460
#extension GL_EXT_ray_tracing : enable
#include "MeshProperties.glsl"
layout(push_constant) uniform SceneData
{
    uint MeshIndex;
    mat4 proj;
    mat4 view;
    vec3 CameraPos;
    float Timer;
} sceneData;

struct RayHitInfo
{
	vec3 color;
	uint seed;
	vec3 normal;
    int reflectCount;
    bool hitGeo;
};

layout(location = 0) rayPayloadInEXT RayHitInfo rayPayload;
layout(binding = 1, set = 0, rgba8) uniform image2D RayTracedTexture;
layout(binding = 2) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
layout(binding = 3) uniform sampler2D TextureMap[];
void main()
{
	rayPayload.color = vec3(0.0f, 0.0f, 1.0f);
	rayPayload.normal = vec3(0.0f);
}