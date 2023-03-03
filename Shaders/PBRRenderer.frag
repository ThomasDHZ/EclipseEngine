#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable

layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec2 UV;
layout(location = 2) in vec3 Normal;
layout(location = 3) in vec3 Tangent;
layout(location = 4) in vec3 BiTangent;
layout(location = 5) in vec3 Color;

layout(location = 0) out vec4 outColor;

#include "VertexLayout.glsl"
#include "MeshProperties.glsl"
#include "MaterialProperties.glsl"
#include "Lights.glsl"
#include "SceneData.glsl"

layout(binding = 0) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
layout(binding = 1) buffer MaterialPropertiesBuffer { MaterialProperties materialProperties; } materialBuffer[];
layout(binding = 2) buffer DirectionalLightBuffer { DirectionalLight directionalLight; } DLight[];
layout(binding = 3) buffer PointLightBuffer { PointLight pointLight; } PLight[];
layout(binding = 4) buffer SpotLightBuffer { SpotLight spotLight; } SLight[];
layout(binding = 5) uniform sampler2D TextureMap[];
layout(binding = 6) uniform samplerCube IrradianceMap[];
layout(binding = 7) uniform samplerCube PrefilterMap[];
layout(binding = 8) uniform sampler2D BRDFMap;
layout(binding = 9) uniform sampler2D ShadowMap[];
layout(binding = 10) uniform samplerCube PointShadowMap[];

#include "PBRMaterial.glsl"
#include "PBRFunctions.glsl"
#include "PBRLight.glsl"
#include "RasterVertexBuilder.glsl"

void main()
{ 
   Vertex vertex = RasterVertexBuilder();
   const uint materialID = meshBuffer[sceneData.MeshIndex].meshProperties.MaterialBufferIndex;
   const MaterialProperties material = materialBuffer[materialID].materialProperties;

   PBRMaterial pbrMaterial = BuildPBRMaterial(materialBuffer[materialID].materialProperties, vertex.UV);

   mat3 TBN = getTBNFromMap(vertex);
   vec3 N = vertex.Normal;

   vec3 ViewPos  = sceneData.CameraPos;
   vec3 FragPos2  = vertex.Position;
   vec3 viewDir = normalize(ViewPos - FragPos2);
   if (pbrMaterial.NormalMapID != 0)
   {
        ViewPos  = TBN * sceneData.CameraPos;
        FragPos2  = TBN * vertex.Position;

        if(pbrMaterial.DepthMapID != 0)
        {
            vertex.UV = ParallaxMapping(material.DepthMapID, vertex.UV,  viewDir);       
            if(vertex.UV.x > 1.0 || vertex.UV.y > 1.0 || vertex.UV.x < 0.0 || vertex.UV.y < 0.0)
            {
              discard;
            }
        }
        N = texture(TextureMap[material.NormalMapID], vertex.UV).rgb;
        N = normalize(N * 2.0 - 1.0);
        N = normalize(TBN * N);
   }

    vec3 V = normalize(sceneData.CameraPos - vertex.Position);
    vec3 R = reflect(-V, N); 

    vec3 F0 = vec3(0.04f); 
    F0 = mix(F0, pbrMaterial.Albedo, pbrMaterial.Metallic);

    vec3 Lo = vec3(0.0);
    Lo += CalcDirectionalLight(F0, V, N, pbrMaterial);
    Lo += CalcPointLight(F0, V, N, vertex, pbrMaterial);
    Lo += CalcSpotLight(F0, V, N, vertex, pbrMaterial);

    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, pbrMaterial.Roughness);
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - pbrMaterial.Metallic;	  
    
    vec3 irradiance = texture(IrradianceMap[0], N).rgb;
    vec3 diffuse      = irradiance * pbrMaterial.Albedo;

    vec3 prefilteredColor = textureLod(PrefilterMap[0], R,  pbrMaterial.Roughness * sceneData.PBRMaxMipLevel).rgb;    
    vec2 brdf  = texture(BRDFMap, vec2(max(dot(N, V), 0.0), pbrMaterial.Roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = pbrMaterial.Emission + ((kD * diffuse + specular) * pbrMaterial.AmbientOcclusion);
    
    vec3 color = ambient + Lo;
    color = color / (color + vec3(1.0f));
    color = pow(color, vec3(1.0f/2.2f)); 

    if(meshBuffer[sceneData.MeshIndex].meshProperties.SelectedMesh == 1)
    {
        color = mix(color, vec3(1.0f, 0.0f, 0.0f), .35f);
    }
    outColor = vec4(color, 1.0f);
}