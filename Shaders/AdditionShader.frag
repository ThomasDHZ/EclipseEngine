#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable

layout(location = 0) in vec2 UV;
layout(location = 0) out vec4 outColor;
layout(binding = 0) uniform sampler2D Texture[];

void main()
{  
     vec3 result = texture(Texture[0], UV).rgb + 
                   texture(Texture[1], UV).rgb +
                   texture(Texture[2], UV).rgb +
                   texture(Texture[3], UV).rgb +
                   texture(Texture[4], UV).rgb +
                   texture(Texture[5], UV).rgb +
                   texture(Texture[6], UV).rgb +
                   texture(Texture[7], UV).rgb +
                   texture(Texture[8], UV).rgb +
                   texture(Texture[9], UV).rgb +
                   texture(Texture[10], UV).rgb +
                   texture(Texture[11], UV).rgb;
     outColor = vec4(result, 1.0);
}