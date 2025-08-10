#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable

layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec2 UV;
layout(location = 2) in vec3 Normal;
layout(location = 3) in vec3 Tangent;
layout(location = 4) in vec3 BiTangent;
layout(location = 5) in vec3 Color;
layout(location = 6) in flat int MaterialID;

layout(location = 0) out vec4 outColor;

#include "SceneData.glsl"

void main()
{ 
   outColor = vec4(1.0f);
}