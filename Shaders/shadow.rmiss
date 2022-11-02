#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_scalar_block_layout : enable

#include "MeshProperties.glsl"
#include "Lights.glsl"
#include "RTXPayload.glsl"
#include "SceneData.glsl"

layout(location = 2) rayPayloadInEXT bool shadowed;

void main()
{
	shadowed = true;
}