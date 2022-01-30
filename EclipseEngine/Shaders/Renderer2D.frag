#version 460

layout(location = 0) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

layout(push_constant) uniform SceneData
{
    mat4 proj;
    mat4 view;
    vec3 CameraPos;
    float Timer;
} sceneData;

void main() {
    outColor = vec4(fragColor, 1.0);
}