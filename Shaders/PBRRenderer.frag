#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable

layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec2 UV;
layout(location = 2) in vec3 Normal;
layout(location = 3) in vec3 Tangent;
layout(location = 4) in vec3 BiTangent;
layout(location = 5) in vec3 Color;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outBloom;

#include "vertex.glsl"
#include "MeshProperties.glsl"
#include "MaterialProperties.glsl"
#include "LightProperties.glsl"
#include "ConstSceneData.glsl"

layout(binding = 0) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
layout(binding = 1) buffer TransformBuffer { mat4 transform; } transformBuffer[];
layout(binding = 2) buffer MaterialPropertiesBuffer { MaterialProperties materialProperties; } materialBuffer[];
layout(binding = 3) uniform sampler2D TextureMap[];
layout(binding = 4) uniform sampler2D BRDFMap;
layout(binding = 5) uniform samplerCube IrradianceMap;
layout(binding = 6) uniform samplerCube PrefilterMap;
layout(binding = 7) buffer SunLightBuffer { SunLight sunLight; } SULight[];
layout(binding = 8) buffer DirectionalLightBuffer { DirectionalLight directionalLight; } DLight[];
layout(binding = 9) buffer PointLightBuffer { PointLight pointLight; } PLight[];
layout(binding = 10) buffer SpotLightBuffer { SpotLight spotLight; } SLight[];
layout(binding = 11) uniform sampler2D ShadowMap[];
layout(binding = 12) uniform samplerCube PointShadowMap[];

#include "RasterVertexBuilder.glsl"
#include "MaterialBuilder.glsl"
#include "PBRFunctions.glsl"
#include "LightFunctions.glsl"

vec2 ParallaxMapping(MaterialProperties material, vec2 texCoords, vec3 viewDir)
{
    const float heightScale = 0.6f;
    const float minLayers = 3;
    const float maxLayers = 6;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;

    viewDir.y = -viewDir.y;
    vec2 P = viewDir.xy / viewDir.z * heightScale;
    vec2 deltaTexCoords = P / numLayers;

    vec2  currentTexCoords = texCoords;
    float currentDepthMapValue = texture(TextureMap[material.HeightMap], currentTexCoords).r;

    while (currentLayerDepth < currentDepthMapValue)
    {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = texture(TextureMap[material.HeightMap], currentTexCoords).r;
        currentLayerDepth += layerDepth;
    }

    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    float afterDepth = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(TextureMap[material.HeightMap], prevTexCoords).r - currentLayerDepth + layerDepth;

    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

void main()
{ 
    Vertex vertex = RasterVertexBuilder();
    MaterialProperties materialProp = materialBuffer[sceneData.MaterialIndex].materialProperties;
    
   mat3 TBN = getTBNFromMap(vertex);
   vec2 UV = vertex.UV;
   vec3 N = vertex.Normal;
   vec3 V = normalize(sceneData.CameraPos - vertex.Position);

    vec3 ViewPos  = TBN * sceneData.CameraPos;
    vec3 FragPos2  = TBN * vertex.Position;
    vec3 viewDir = normalize(ViewPos - FragPos2);

   if (materialProp.NormalMap != 0)
   {
        ViewPos  = TBN * sceneData.CameraPos;
        FragPos2  = TBN * vertex.Position;
        vec3 viewDir = normalize(ViewPos - FragPos2);

        if(materialProp.DepthMap != 0)
        {
            UV = ParallaxMapping(materialProp, UV,  viewDir);   
           // if(UV.x > 1.0 || UV.y > 1.0 || UV.x < 0.0 || UV.y < 0.0)
       // discard;
        }
        N = texture(TextureMap[materialProp.NormalMap], UV).rgb;
        N = normalize(N * 2.0 - 1.0);
        N = normalize(TBN * N);
   }
    vec3 R = reflect(-V, N); 

    vec3 Albedo = texture(TextureMap[materialProp.AlbedoMap], UV).rgb;
    float Metallic = texture(TextureMap[materialProp.MetallicMap], UV).r;
    float Roughness = texture(TextureMap[materialProp.RoughnessMap], UV).r;
    float AmbientOcclusion = 0.34f;
    vec3 Emission = texture(TextureMap[materialProp.EmissionMap], UV).rgb;

    vec3 F0 = vec3(0.04f); 
    F0 = mix(F0, Albedo, Metallic);

    vec3 Lo = vec3(0.0);
        for (int x = 0; x < sceneData.DirectionalLightCount; x++)
    {
        vec3 L = normalize(-DLight[x].directionalLight.direction);
        vec3 H = normalize(V + L);
        float watts = DLight[x].directionalLight.intensity;
        vec3 radiance = DLight[x].directionalLight.diffuse * watts;

        float NDF = DistributionGGX(N, H, Roughness);
        float G = GeometrySmith(N, V, L, Roughness);
        vec3 F = fresnelSchlickRoughness(max(dot(H, V), 0.0), F0, Roughness);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - Metallic;

        float NdotL = max(dot(N, L), 0.0);

        vec4 LightSpace = (LightBiasMatrix * DLight[x].directionalLight.LightSpaceMatrix * meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform) * vec4(FragPos, 1.0);
        float shadow = filterPCF(LightSpace / LightSpace.w, x);


        Lo += (kD * Albedo / PI + specular) * radiance * NdotL;
    }

    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, Roughness);
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - Metallic;	  
    
    vec3 irradiance = texture(IrradianceMap, N).rgb;
    vec3 diffuse      = irradiance * Albedo;

    vec3 prefilteredColor = textureLod(PrefilterMap, R,  Roughness * sceneData.PBRMaxMipLevel).rgb;    
    vec2 brdf  = texture(BRDFMap, vec2(max(dot(N, V), 0.0), Roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = Emission + ((kD * diffuse + specular) * AmbientOcclusion);
    
    vec3 color = ambient + Lo;
    color = color / (color + vec3(1.0f));
    color = pow(color, vec3(1.0f/2.2f)); 

    if(meshBuffer[sceneData.MeshIndex].meshProperties.SelectedMesh == 1)
    {
        color = mix(color, vec3(1.0f, 0.0f, 0.0f), .35f);
    }

     outColor = vec4(color, 1.0f);

     vec3 bloomColor = Emission;
     if(color.r >= 1.0f)
     {
        bloomColor.r = color.r;
     }
     if(color.g >= 1.0f)
     {
        bloomColor.g = color.g;
     }
     if(color.b >= 1.0f)
     {
        bloomColor.b = color.b;
     }

     outBloom = vec4(bloomColor, 1.0f);
}