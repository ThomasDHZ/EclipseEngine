#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform samplerCube cubeMap;

layout(location = 0) in vec2 UV;

layout(location = 0) out vec4 FragColor;

const float PI = 3.14159265359;

const vec2 invAtan = vec2(0.1591, 0.3183);

void main() 
{
     vec3 cubmapTexCoords = vec3(0.0f);
     cubmapTexCoords.x = -sin(UV.x * PI * 2.0f) * sin(UV.y * PI);
     cubmapTexCoords.y = cos(UV.y * PI);
     cubmapTexCoords.z = -cos(UV.x * PI * 2.0f) * sin(UV.y * PI);
     FragColor =  vec4(texture(cubeMap, cubmapTexCoords).rgb, 1.0f);
}