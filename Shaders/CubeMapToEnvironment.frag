#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform samplerCube cubeMap;

layout(location = 0) in vec2 UV;

layout(location = 0) out vec4 FragColor;

const float PI = 3.14159265359;

const vec2 invAtan = vec2(0.1591, 0.3183);

void main() 
{
     vec3 cubemapTexCoords = vec3(0.0f);
     cubemapTexCoords.x = -sin(UV.x * PI * 2.0f) * sin(UV.y * PI);
     cubemapTexCoords.y = cos(UV.y * PI);
     cubemapTexCoords.z = -cos(UV.x * PI * 2.0f) * sin(UV.y * PI);
     FragColor =  vec4(texture(cubeMap, cubemapTexCoords).rgb, 1.0f);
}