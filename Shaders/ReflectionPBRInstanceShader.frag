#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable

#include "VertexLayout.glsl"
#include "MeshProperties.glsl"
#include "Lights.glsl"

layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec2 UV;
layout(location = 2) in vec3 Normal;
layout(location = 3) in vec3 Tangent;
layout(location = 4) in vec3 BiTangent;
layout(location = 5) in vec3 Color;
layout(location = 6) in flat int MaterialID;

layout(location = 0) out vec4 outColor;
//layout(location = 1) out vec4 outBloom;

layout(binding = 1) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
layout(binding = 2) buffer MaterialPropertiesBuffer { MaterialProperties materialProperties; } materialBuffer[];
layout(binding = 3) buffer DirectionalLightBuffer { DirectionalLight directionalLight; } DLight[];
layout(binding = 4) buffer PointLightBuffer { PointLight pointLight; } PLight[];
layout(binding = 5) buffer SpotLightBuffer { SpotLight spotLight; } SLight[];
layout(binding = 6) uniform sampler2D TextureMap[];
layout(binding = 7) uniform samplerCube IrradianceMap;
layout(binding = 8) uniform samplerCube PrefilterMap;
layout(binding = 9) uniform sampler2D BRDFMap;
layout(binding = 10) uniform sampler2D ShadowMap[];
layout(binding = 11) uniform samplerCube PointShadowMap[];

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

const float PI = 3.14159265359;

const mat4 LightBiasMatrix = mat4(
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.5, 0.5, 0.0, 1.0);

vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float ShadowCalculation(vec4 fragPosLightSpace, vec2 offset, int index)
{
    float shadow = 1.0f;
	if ( fragPosLightSpace.z > -1.0 && fragPosLightSpace.z < 1.0 ) 
	{
		float dist = texture( ShadowMap[index], fragPosLightSpace.st + offset ).r;
		if ( fragPosLightSpace.w > 0.0 && dist < fragPosLightSpace.z ) 
		{
			shadow = 0.1f;
		}
	}
    return shadow;
}

float filterPCF(vec4 sc, int index)
{
	ivec2 texDim = textureSize( ShadowMap[index], 0);
	float scale = 1.5;
	float dx = scale * 1.0 / float(texDim.x);
	float dy = scale * 1.0 / float(texDim.y);

	float shadowFactor = 0.0;
	int count = 0;
	int range = 1;
	
	for (int x = -range; x <= range; x++)
	{
		for (int y = -range; y <= range; y++)
		{
			shadowFactor += ShadowCalculation(sc, vec2(dx*x, dy*y), index);
			count++;
		}
	
	}
	return shadowFactor / count;
}

float CubeShadowCalculation(vec3 fragPos, vec3 viewPos, int index)
{
    float far_plane = 500.0f;
    vec3 fragToLight = fragPos - PLight[index].pointLight.position;
    float currentDepth = length(fragToLight);

    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(PointShadowMap[index], fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);

    return shadow;
}

mat3 getTBNFromMap();
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);
vec3 CalcDirectionalLight(vec3 F0, vec3 V, vec3 N, vec3 albedo, float roughness, float metallic);
vec3 CalcPointLight(vec3 F0, vec3 V, vec3 N, vec3 albedo, float roughness, float metallic);
vec3 CalcSpotLight(vec3 F0, vec3 V, vec3 N, vec3 albedo, float roughness, float metallic);
vec2 ParallaxMapping(uint depthMapID, vec2 texCoords, vec3 viewDir);

void main()
{  
   MaterialProperties material = materialBuffer[MaterialID].materialProperties;
   vec2 FinalUV = UV + meshBuffer[sceneData.MeshIndex].meshProperties.UVOffset;
        FinalUV *= meshBuffer[sceneData.MeshIndex].meshProperties.UVScale;

   if(texture(TextureMap[material.AlphaMapID], FinalUV).r == 0.0f ||
      texture(TextureMap[material.AlbedoMapID], FinalUV).a == 0.0f)
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

   vec3 albedo = pow(material.Albedo, vec3(2.2));
   if (material.AlbedoMapID != 0)
   {
       albedo = pow(texture(TextureMap[material.AlbedoMapID], FinalUV).rgb, vec3(2.2));
   }

   float metallic =  material.Matallic;
   if (material.MetallicMapID != 0)
   {
     metallic = texture(TextureMap[material.MetallicMapID], FinalUV).r;
   }

   float roughness =  material.Roughness;
   if (material.RoughnessMapID != 0)
   {
     roughness = texture(TextureMap[material.RoughnessMapID], FinalUV).r;
   }

   float ao = material.AmbientOcclusion;
   if (material.AmbientOcclusionMapID != 0)
   {
     ao = texture(TextureMap[material.AmbientOcclusionMapID], FinalUV).r;
   }

      vec3 emission = material.Emission;
   if (material.EmissionMapID != 0)
   {
       emission = texture(TextureMap[material.EmissionMapID], FinalUV).rgb;
   }

   mat3 TBN = getTBNFromMap();
   vec3 N = Normal;

   vec3 ViewPos  = sceneData.CameraPos;
   vec3 FragPos2  = FragPos;
   vec3 viewDir = normalize(ViewPos - FragPos2);
   if (material.NormalMapID != 0)
   {
        ViewPos  = TBN * sceneData.CameraPos;
        FragPos2  = TBN * FragPos;

        if(material.DepthMapID != 0)
        {
            FinalUV = ParallaxMapping(material.DepthMapID, FinalUV,  viewDir);       
            if(FinalUV.x > 1.0 || FinalUV.y > 1.0 || FinalUV.x < 0.0 || FinalUV.y < 0.0)
            {
              discard;
            }
        }
        N = texture(TextureMap[material.NormalMapID], FinalUV).rgb;
        N = normalize(N * 2.0 - 1.0);
        N = normalize(TBN * N);
   }

    vec3 V = normalize(sceneData.CameraPos - FragPos);
    vec3 R = reflect(-V, N); 

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);
    Lo += CalcDirectionalLight(F0, V, N, albedo, roughness, metallic);
    Lo += CalcPointLight(F0, V, N, albedo, roughness, metallic);
    Lo += CalcSpotLight(F0, V, N, albedo, roughness, metallic);

    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;	  
    
    vec3 irradiance = texture(IrradianceMap, N).rgb;
    vec3 diffuse      = irradiance * albedo;

    vec3 prefilteredColor = textureLod(PrefilterMap, R,  roughness * sceneData.PBRMaxMipLevel).rgb;    
    vec2 brdf  = texture(BRDFMap, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = emission + ((kD * diffuse + specular) * ao);
    
    vec3 color = ambient + Lo;
    outColor = vec4(color, 1.0f);
}

mat3 getTBNFromMap()
{
   vec3 T = normalize(mat3(meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform) * vec3(Tangent));
   vec3 B = normalize(mat3(meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform) * vec3(BiTangent));
   vec3 N = normalize(mat3(meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform) * Normal);
   return mat3(T, B, N);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}   

vec3 CalcDirectionalLight(vec3 F0, vec3 V, vec3 N, vec3 albedo, float roughness, float metallic)
{
   vec3 Lo = vec3(0.0);

   for(int x = 0; x < sceneData.DirectionalLightCount; x++)
   {
        vec3 L = normalize(-DLight[x].directionalLight.direction);
        vec3 H = normalize(V + L);
        vec3 radiance = DLight[x].directionalLight.diffuse;

        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);    
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);        
        
        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;	                
            
        float NdotL = max(dot(N, L), 0.0);        

        vec4 LightSpace = (LightBiasMatrix *  DLight[x].directionalLight.lightSpaceMatrix * meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform) * vec4(FragPos, 1.0);
        float shadow = filterPCF(LightSpace/ LightSpace.w, x);  
        Lo += (kD * albedo / PI + specular) * radiance * NdotL * shadow;
    }
    return Lo;
}

vec3 CalcPointLight(vec3 F0, vec3 V, vec3 N, vec3 albedo, float roughness, float metallic)
{
   vec3 Lo = vec3(0.0);
     for(int x = 0; x < sceneData.PointLightCount; x++)
   {
        vec3 L = normalize(PLight[x].pointLight.position - FragPos);
        vec3 H = normalize(V + L);
        float distance = length(PLight[x].pointLight.position - FragPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = PLight[x].pointLight.diffuse * attenuation;

        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);    
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);        
        
        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; 
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;	                
            
        float NdotL = max(dot(N, L), 0.0);        

     // float shadow = CubeShadowCalculation(FragPos, V, x);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;// * shadow;
   }

    return Lo;
}

vec3 CalcSpotLight(vec3 F0, vec3 V, vec3 N, vec3 albedo, float roughness, float metallic)
{
   vec3 Lo = vec3(0.0);
   for(int x = 0; x < sceneData.SpotLightCount; x++)
   {
        vec3 L = normalize(SLight[x].spotLight.position - FragPos);
        vec3 H = normalize(V + L);

        float theta = dot(L, normalize(-SLight[x].spotLight.direction)); 
        float epsilon = SLight[x].spotLight.cutOff - SLight[x].spotLight.outerCutOff;
        float intensity = clamp((theta - SLight[x].spotLight.outerCutOff) / epsilon, 0.0, 1.0);

        float distance = length(SLight[x].spotLight.position - FragPos);
        float attenuation = 1.0 / (distance * distance) ;
        attenuation *= intensity;
        vec3 radiance = SLight[x].spotLight.diffuse * attenuation;

        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);    
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);        
        
        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; 
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;	                
            
        float NdotL = max(dot(N, L), 0.0);        

        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
   }

    return Lo;
}


vec2 ParallaxMapping(uint depthMapID, vec2 texCoords, vec3 viewDir)
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
    float currentDepthMapValue = texture(TextureMap[depthMapID], currentTexCoords).r;

    while (currentLayerDepth < currentDepthMapValue)
    {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = texture(TextureMap[depthMapID], currentTexCoords).r;
        currentLayerDepth += layerDepth;
    }

    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    float afterDepth = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(TextureMap[depthMapID], prevTexCoords).r - currentLayerDepth + layerDepth;

    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}