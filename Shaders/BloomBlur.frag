#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable

#include "VertexLayout.glsl"
#include "MeshProperties.glsl"
#include "Lights.glsl"

layout(location = 0) in vec2 UV;
layout(location = 0) out vec4 outColor;
layout(binding = 0) uniform sampler2D BloomTexture;

layout(push_constant) uniform BloomSettings
{
    uint horizontal;
    uint MipLevel;
} bloomSettings;

float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main()
{  
     vec2 tex_offset = 1.0 / textureSize(BloomTexture, 0); 
     vec3 result = texture(BloomTexture, UV).rgb * weight[0];
     if(bloomSettings.horizontal == 1)
     {
         for(int i = 1; i < 5; ++i)
         {
            result += textureLod(BloomTexture, UV + vec2(tex_offset.x * i, 0.0), bloomSettings.MipLevel).rgb * weight[i];
            result += textureLod(BloomTexture, UV - vec2(tex_offset.x * i, 0.0), bloomSettings.MipLevel).rgb * weight[i];
         }
     }
     else
     {
         for(int i = 1; i < 5; ++i)
         {
             result += textureLod(BloomTexture, UV + vec2(0.0, tex_offset.y * i), bloomSettings.MipLevel).rgb * weight[i];
             result += textureLod(BloomTexture, UV - vec2(0.0, tex_offset.y * i), bloomSettings.MipLevel).rgb * weight[i];
         }
     }
     outColor = vec4(1.0f, 0.0f, 0.0f, 1.0);
}