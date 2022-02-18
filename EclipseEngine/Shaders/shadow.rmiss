#version 460
#extension GL_EXT_ray_tracing : require
#include "MeshProperties.glsl"
layout(push_constant) uniform SceneData
{
    uint MeshIndex;
    mat4 proj;
    mat4 view;
    vec3 CameraPos;
    float Timer;
} sceneData;

layout(location = 2) rayPayloadInEXT bool shadowed;

void main()
{
	shadowed = false;
}