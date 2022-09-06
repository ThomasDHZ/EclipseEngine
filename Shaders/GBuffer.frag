#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable
#extension GL_EXT_ray_tracing : enable
#extension GL_EXT_ray_query : enable

#include "VertexLayout.glsl"
#include "MeshProperties.glsl"
#include "Lights.glsl"

layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec2 UV;
layout(location = 2) in vec3 Normal;
layout(location = 3) in vec3 Tangent;
layout(location = 4) in vec3 BiTangent;
layout(location = 5) in vec3 Color;

layout(location = 0) out vec4 outPosition;
layout(location = 1) out vec4 outTangent;
layout(location = 2) out vec4 outBiTangent;
layout(location = 3) out vec4 outTBNormal;
layout(location = 4) out vec4 outNormal;
layout(location = 5) out vec4 outAlbedo;
layout(location = 6) out vec4 outSpecular;
layout(location = 7) out vec4 outBloom;

layout(binding = 0) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
layout(binding = 1) buffer MaterialPropertiesBuffer { MaterialProperties materialProperties; } materialBuffer[];
layout(binding = 2) buffer DirectionalLightBuffer { DirectionalLight directionalLight; } DLight[];
layout(binding = 3) buffer PointLightBuffer { PointLight pointLight; } PLight[];
layout(binding = 4) buffer SpotLightBuffer { SpotLight spotLight; } SLight[];
layout(binding = 5) uniform sampler2D TextureMap[];

layout(push_constant) uniform SceneData
{
    uint MeshIndex;
    mat4 proj;
    mat4 view;
    vec3 CameraPos;
    vec3 MeshColorID;
    vec3 AmbientLight;
    uint DirectionalLightCount;
    uint PointLightCount;
    uint SpotLightCount;
    float Timer;
    float PBRMaxMipLevel;
} sceneData;

vec2 ParallaxMapping(MaterialProperties material, vec2 texCoords, vec3 viewDir);

void main() 
{
   const uint materialID = meshBuffer[sceneData.MeshIndex].meshProperties.MaterialBufferIndex;
   MaterialProperties material = materialBuffer[materialID].materialProperties;

   vec2 FinalUV = UV + meshBuffer[sceneData.MeshIndex].meshProperties.UVOffset;
        FinalUV *= meshBuffer[sceneData.MeshIndex].meshProperties.UVScale;

   if(texture(TextureMap[material.AlphaMapID], FinalUV).r == 0.0f ||
      texture(TextureMap[material.DiffuseMapID], FinalUV).a == 0.0f)
   {
	 discard;
   }
   if(meshBuffer[sceneData.MeshIndex].meshProperties.UVFlip.y == 1.0f)
   {
        FinalUV.y = 1.0f - FinalUV.y;
   }
   if(meshBuffer[sceneData.MeshIndex].meshProperties.UVFlip.x == 1.0f)
   {
        FinalUV.x = 1.0f - FinalUV.x;
   }

   vec3 T = normalize(mat3(meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform) * vec3(Tangent));
   vec3 B = normalize(mat3(meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform) * vec3(BiTangent));
   vec3 N = normalize(mat3(meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform) * Normal);
   mat3 TBN = transpose(mat3(T, B, N));
   outTangent = vec4(T, 1.0f);
   outBiTangent = vec4(B, 1.0f);
   outTBNormal = vec4(N, 1.0f);

   vec3 normal = Normal;
   vec3 ViewPos  = sceneData.CameraPos;
   vec3 FragPos2  = FragPos;
   vec3 viewDir = normalize(ViewPos - FragPos2);
    if(material.NormalMapID != 0)
    {
        ViewPos  = TBN * sceneData.CameraPos;
        FragPos2  = TBN * FragPos;
    }

    if(material.NormalMapID != 0)
    {
        if(material.DepthMapID != 0)
        {
            FinalUV = ParallaxMapping(material, FinalUV,  viewDir);       
            if(FinalUV.x > 1.0 || FinalUV.y > 1.0 || FinalUV.x < 0.0 || FinalUV.y < 0.0)
            {
              discard;
            }
        }
        normal = texture(TextureMap[material.NormalMapID], FinalUV).rgb;
        normal = normalize(normal * 2.0 - 1.0);
     }

    outPosition = vec4(FragPos2, 1.0f);
    outNormal = vec4(normal, 1.0f);

    outAlbedo = vec4(material.Diffuse, 1.0f);
    outSpecular = vec4(material.Specular, 1.0f);
    if (material.DiffuseMapID != 0)
    {
        outAlbedo = texture(TextureMap[material.DiffuseMapID], FinalUV);
    }
    if (material.SpecularMapID != 0)
    {
        outSpecular = texture(TextureMap[material.SpecularMapID], FinalUV);
    }

    outBloom = vec4(0.0f, 0.0f, 0.0f, 1.0f);

}

vec2 ParallaxMapping(MaterialProperties material, vec2 texCoords, vec3 viewDir)
{
    const float heightScale = meshBuffer[sceneData.MeshIndex].meshProperties.heightScale;
    const float minLayers = meshBuffer[sceneData.MeshIndex].meshProperties.minLayers;
    const float maxLayers = meshBuffer[sceneData.MeshIndex].meshProperties.maxLayers;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;

    viewDir.y = -viewDir.y;
    vec2 P = viewDir.xy / viewDir.z * heightScale;
    vec2 deltaTexCoords = P / numLayers;

    vec2  currentTexCoords = texCoords;
    float currentDepthMapValue = texture(TextureMap[material.DepthMapID], currentTexCoords).r;

    while (currentLayerDepth < currentDepthMapValue)
    {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = texture(TextureMap[material.DepthMapID], currentTexCoords).r;
        currentLayerDepth += layerDepth;
    }

    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    float afterDepth = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(TextureMap[material.DepthMapID], prevTexCoords).r - currentLayerDepth + layerDepth;

    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}