#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_scalar_block_layout : enable

#include "RTXPayload.glsl"
layout(location = 2) rayPayloadInEXT bool shadowed;

#include "RTXPBRBindingLayout.glsl"

void main()
{
	shadowed = true;
}