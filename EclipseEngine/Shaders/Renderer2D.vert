#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable

#include "MeshProperties.glsl"

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

layout(binding = 0) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];

layout(push_constant) uniform SceneData
{
    uint MeshIndex;
    mat4 proj;
    mat4 view;
    vec3 CameraPos;
    float Timer;
} sceneData;

void main() {

// if(gl_VertexIndex == 0)
//	{
//		debugPrintfEXT(": %i \n", sceneData.MeshIndex);
//	}
    gl_Position = sceneData.proj * sceneData.view * meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform * vec4(inPosition, 0.0, 1.0);
    fragColor = inColor;
}