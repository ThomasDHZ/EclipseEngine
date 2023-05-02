#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_scalar_block_layout : enable

#include "RTXPayload.glsl"
#include "Vertex.glsl"
#include "MeshProperties.glsl"
#include "MaterialProperties.glsl"
#include "GLTFLights.glsl"

layout(location = 2) rayPayloadInEXT bool shadowed;


void main()
{
}