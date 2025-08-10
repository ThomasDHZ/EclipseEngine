  #version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform samplerCube CubeMap;

layout(location = 0) in vec3 TexCoords;

layout(location = 0) out vec4 FragColor;

void main() 
{
    vec3 color = texture(CubeMap, TexCoords).rgb;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 
    FragColor = vec4(color, 1.0f);
}