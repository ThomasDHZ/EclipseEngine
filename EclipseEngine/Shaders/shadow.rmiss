#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_scalar_block_layout : enable
#include "MeshProperties.glsl"
layout(push_constant) uniform SceneData
{
    uint MeshIndex;
    mat4 proj;
    mat4 view;
    vec3 CameraPos;
    ivec3 MeshColorID;
    float Timer;
} sceneData;

layout(location = 2) rayPayloadInEXT bool shadowed;

void main()
{
	shadowed = false;
}