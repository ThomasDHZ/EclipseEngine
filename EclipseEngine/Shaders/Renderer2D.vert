#version 460
#extension GL_EXT_debug_printf : enable

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

layout(push_constant) uniform SceneData
{
    mat4 proj;
    mat4 view;
    vec3 CameraPos;
    float Timer;
} sceneData;

void main() {

    gl_Position = sceneData.proj * sceneData.view * vec4(inPosition, 0.0, 1.0);
    fragColor = inColor;
}