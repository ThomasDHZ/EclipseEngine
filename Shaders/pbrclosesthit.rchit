#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable

#include "RTXPayload.glsl"
layout(location = 0) rayPayloadInEXT RayPayload rayHitInfo;
layout(location = 1) rayPayloadEXT bool shadowed;
hitAttributeEXT vec2 attribs;

#include "RTXPBRBindingLayout.glsl"
#include "PBRMaterial.glsl"
#include "PBRFunctions.glsl"
#include "PBRRTXLight.glsl"
#include "RTXVertexBuilder.glsl"
#include "RTXRandom.glsl"

void main()
{		
   Vertex vertex = BuildVertexInfo();
    const uint materialID = meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MaterialBufferIndex;
    MaterialProperties material = materialBuffer[materialID].materialProperties;
    PBRMaterial pbrMaterial = BuildPBRMaterial(material, vertex.UV);

   mat3 TBN = getTBNFromMap(vertex);
   vec3 N = vertex.Normal;

   vec3 ViewPos  = sceneData.CameraPos;
   vec3 FragPos  = vertex.Position;
   vec3 viewDir = normalize(ViewPos - FragPos);
   if (material.NormalMapID != 0)
   {
        ViewPos  = TBN * sceneData.CameraPos;
        FragPos  = TBN * vertex.Position;

        if(material.DepthMapID != 0)
        {
            vertex.UV = ParallaxMapping(material, vertex.UV, viewDir);       
        }
        N = texture(TextureMap[material.NormalMapID], vertex.UV).rgb;
        N = normalize(N * 2.0 - 1.0);
        N = normalize(TBN * N);
   }

    vec3 V = normalize(sceneData.CameraPos - FragPos);
    vec3 R = reflect(-V, N); 

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, pbrMaterial.Albedo, pbrMaterial.Metallic);

    vec3 Lo = vec3(0.0);
    Lo += CalcDirectionalLight(F0, V, N, vertex, pbrMaterial);
    Lo += CalcPointLight(F0, V, N, vertex, pbrMaterial);
    Lo += CalcSpotLight(F0, V, N, vertex, pbrMaterial);

    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, pbrMaterial.Roughness);
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - pbrMaterial.Metallic;	  
    
    vec3 irradiance = vec3(0.0f);
    if(rayHitInfo.reflectCount < sceneData.MaxRefeflectCount)
    {
       uint seed = tea(gl_LaunchIDEXT.y * gl_LaunchSizeEXT.x + gl_LaunchIDEXT.x, sceneData.frame);
       float r1        = rnd(seed) - 0.5f;
       float r2        = rnd(seed) - 0.5f;
       float r3        = rnd(seed) - 0.5f;

        vec3 hitPos = gl_WorldRayOriginEXT + gl_WorldRayDirectionEXT * gl_RayTmaxEXT;
        vec3 origin   = hitPos.xyz + N * 0.001f;
        vec3 rayDir   = reflect(origin, N + vec3(r1, r2, r3));

        rayHitInfo.reflectCount++;
        traceRayEXT(topLevelAS, gl_RayFlagsNoneEXT, 0xff, 0, 0, 0, origin, 0.001f, rayDir, 10000.0f, 0);
		irradiance += rayHitInfo.color; 
    }
    irradiance /= rayHitInfo.reflectCount;
    irradiance = clamp(irradiance, 0.0f, 1.0f);
    rayHitInfo.reflectCount = 1;

    vec3 specular = vec3(0.0f);    
    if(pbrMaterial.Metallic > 0.0f &&
       rayHitInfo.reflectCount2 < sceneData.MaxRefeflectCount)
    {
       uint seed = tea(gl_LaunchIDEXT.y * gl_LaunchSizeEXT.x + gl_LaunchIDEXT.x, sceneData.frame);
       float r1        = rnd(seed) - 0.5f;
       float r2        = rnd(seed) - 0.5f;
       float r3        = rnd(seed) - 0.5f;

        vec3 hitPos = gl_WorldRayOriginEXT + gl_WorldRayDirectionEXT * gl_RayTmaxEXT;
        vec3 origin   = hitPos.xyz + N * 0.001f;
        vec3 rayDir   = reflect(origin, N + material.Roughness * vec3(r1, r2, r3));

        rayHitInfo.reflectCount2++;
        traceRayEXT(topLevelAS, gl_RayFlagsNoneEXT, 0xff, 0, 0, 0, origin, 0.001f, rayDir, 10000.0f, 0);
		specular += rayHitInfo.color; 
	}
    specular /= rayHitInfo.reflectCount2;
    specular = clamp(specular, 0.0f, 1.0f);

    vec3 diffuse = irradiance * pbrMaterial.Albedo;
    vec3 ambient = pbrMaterial.Emission + ((kD * diffuse + specular) * pbrMaterial.AmbientOcclusion);
    rayHitInfo.color = ambient + Lo;
}