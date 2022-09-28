#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable

layout(location = 0) in vec2 UV;
layout(location = 0) out vec4 outColor;
layout(binding = 0) uniform sampler2D Texture[];

layout(push_constant) uniform TextureCountData
{
    uint textureCount;
} textureCountData;

void main()
{  
     vec3 result = vec3(0.0f);
     for(int x = 0; x < textureCountData.textureCount; x++)
     {
        result += texture(Texture[x], UV).rgb;
     }
     outColor = vec4(result, 1.0);
}