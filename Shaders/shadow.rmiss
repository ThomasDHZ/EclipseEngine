#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_scalar_block_layout : enable

#include "VertexLayout.glsl"
#include "MeshProperties.glsl"
#include "MaterialProperties.glsl"
#include "Lights.glsl"
#include "SceneData.glsl"
#include "RTXPayload.glsl"

layout(location = 2) rayPayloadInEXT bool shadowed;

void main()
{
	shadowed = true;
}