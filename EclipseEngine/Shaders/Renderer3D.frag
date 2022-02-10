#version 460

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragUV;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outBloom;

layout(binding = 1) uniform sampler2D TextureMap[];

layout(push_constant) uniform SceneData
{
    mat4 proj;
    mat4 view;
    vec3 CameraPos;
    float Timer;
} sceneData;

void main() {
    outColor = vec4(fragColor, 1.0f);
    outBloom = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}