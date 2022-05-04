  #version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D EnvironmentMap;

layout(location = 0) in vec3 pos;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BloomColor;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main() 
{
    vec2 uv = SampleSphericalMap(normalize(pos));
    
    vec3 color = texture(EnvironmentMap, uv).rgb;
    FragColor = vec4(color, 1.0f);
    BloomColor = vec4(vec3(0.0f, 0.0f, 0.0f), 1.0f);
}