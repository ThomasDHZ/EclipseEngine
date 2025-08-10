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
layout(binding = 11) uniform CubeMapViewSampler 
{
    mat4 CubeMapFaceMatrix[6];
} cubeMapViewSampler;
layout(binding = 12) uniform sampler2D ShadowMap[];
layout(binding = 13) uniform samplerCube PointShadowMap[];

#include "RasterVertexBuilder.glsl"
#include "MaterialBuilder.glsl"
#include "PBRFunctions.glsl"
#include "LightFunctions.glsl"

void main()
{ 
    Vertex vertex = RasterVertexBuilder();
    MaterialProperties material = BuildMaterial(sceneData.MaterialIndex, vertex.UV);
    
   mat3 TBN = getTBNFromMap(vertex);
   vec3 N = vertex.Normal;

   vec3 ViewPos  = sceneData.CameraPos;
   vec3 FragPos2  = vertex.Position;
   vec3 viewDir = normalize(ViewPos - FragPos2);
   if (material.NormalMap != 0)
   {
        ViewPos  = TBN * sceneData.CameraPos;
        FragPos2  = TBN * vertex.Position;

        /*if(material.DepthMap != 0)
        {
            vertex.UV = ParallaxMapping(material.DepthMap, vertex.UV,  viewDir);       
            if(vertex.UV.x > 1.0 || vertex.UV.y > 1.0 || vertex.UV.x < 0.0 || vertex.UV.y < 0.0)
            {
              discard;
            }
        }*/
        N = texture(TextureMap[material.NormalMap], vertex.UV).rgb;
        N = normalize(N * 2.0 - 1.0);
        N = normalize(TBN * N);
   }

    vec3 V = normalize(sceneData.CameraPos - vertex.Position);
    vec3 R = reflect(-V, N); 

    vec3 F0 = vec3(0.04f); 
    F0 = mix(F0, material.Albedo, material.Metallic);

    vec3 Lo = vec3(0.0);
 // Lo += CalcSunLight(F0, V, N, vertex, pbrMaterial);
  Lo += CalcDirectionalLight(F0, V, N, material);
   Lo += CalcPointLight(F0, V, N, vertex, material);
  //Lo += CalcSpotLight(F0, V, N, vertex, pbrMaterial);

    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, material.Roughness);
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - material.Metallic;	  
    
    vec3 irradiance = texture(IrradianceMap, N).rgb;
    vec3 diffuse      = irradiance * material.Albedo;

    vec3 prefilteredColor = textureLod(PrefilterMap, R,  material.Roughness * sceneData.PBRMaxMipLevel).rgb;    
    vec2 brdf  = texture(BRDFMap, vec2(max(dot(N, V), 0.0), material.Roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = ((kD * diffuse + specular) * material.AmbientOcclusion);
    
    vec3 color = ambient + Lo;
    color = color / (color + vec3(1.0f));
    color = pow(color, vec3(1.0f/2.2f)); 

    if(meshBuffer[sceneData.MeshIndex].meshProperties.SelectedMesh == 1)
    {
        color = mix(color, vec3(1.0f, 0.0f, 0.0f), .35f);
    }
    outColor = vec4(color, 1.0f);
}