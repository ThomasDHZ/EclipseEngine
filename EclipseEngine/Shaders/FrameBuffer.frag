#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_debug_printf : enable

layout(location = 0) in vec2 TexCoords;
layout(location = 0) out vec4 outColor;

const float Gamma = 2.2f;
const float Exposure = 1.0f;

void main() 
{
    outColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
}
