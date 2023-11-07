#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable

#include "RTXPayload.glsl"
#include "Vertex.glsl"
#include "MeshProperties.glsl"
#include "MaterialProperties.glsl"
#include "GLTFLights.glsl"
#include "ConstSceneData.glsl"

layout(location = 0) rayPayloadInEXT RayPayload rayHitInfo;
layout(location = 1) rayPayloadEXT bool shadowed;
hitAttributeEXT vec2 attribs;

layout(binding = 0, set = 0) uniform accelerationStructureEXT topLevelAS;
layout(binding = 1, set = 0, rgba8) uniform image2D RayTracedTexture;
layout(binding = 2, scalar) buffer Vertices { Vertex v[]; } vertices[];
layout(binding = 3) buffer Indices { uint i[]; } indices[];
layout(binding = 4) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
layout(binding = 5) buffer TransformBuffer { mat4 transform; } transformBuffer[];
layout(binding = 6) buffer MaterialPropertiesBuffer { MaterialProperties materialProperties; } materialBuffer[];
layout(binding = 7) uniform sampler2D TextureMap[];
layout(binding = 8) uniform sampler2D BRDFMap;
layout(binding = 9) uniform samplerCube IrradianceMap;
layout(binding = 10) uniform samplerCube PrefilterMap;
layout(binding = 11) buffer SunLightBuffer { SunLight sunLight; } SULight[];
layout(binding = 12) buffer DirectionalLightBuffer { DirectionalLight directionalLight; } DLight[];
layout(binding = 13) buffer PointLightBuffer { PointLight pointLight; } PLight[];
layout(binding = 14) buffer SpotLightBuffer { SpotLight spotLight; } SLight[];

struct PBRMaterial
{
	vec3 Albedo;
	float Metallic;
	float Roughness;
	float AmbientOcclusion;
	vec3 Emission;
	float Alpha;

	uint NormalMapID;
	uint DepthMapID;
};

 mat3 aces_input_matrix =
{
    vec3(0.59719f, 0.35458f, 0.04823f),
    vec3(0.07600f, 0.90834f, 0.01566f),
    vec3(0.02840f, 0.13383f, 0.83777f)
};

 mat3 aces_output_matrix =
{
    vec3(1.60475f, -0.53108f, -0.07367f),
   vec3(-0.10208f,  1.10813f, -0.00605f),
   vec3(-0.00327f, -0.07276f,  1.07602f)
};

vec3 rtt_and_odt_fit(vec3 v)
{
    vec3 a = v * (v + 0.0245786f) - 0.000090537f;
    vec3 b = v * (0.983729f * v + 0.4329510f) + 0.238081f;
    return a / b;
}

vec3 aces_fitted(vec3 v)
{
    v = aces_input_matrix * v;
    v = rtt_and_odt_fit(v);
    return aces_output_matrix * v;
}

Vertex BuildVertexInfo()
{
	Vertex vertex;
	const ivec3 index = ivec3(indices[gl_InstanceCustomIndexEXT].i[3 * gl_PrimitiveID],
						      indices[gl_InstanceCustomIndexEXT].i[3 * gl_PrimitiveID + 1],
							  indices[gl_InstanceCustomIndexEXT].i[3 * gl_PrimitiveID + 2]);

	const Vertex v0 = vertices[gl_InstanceCustomIndexEXT].v[index.x];
	const Vertex v1 = vertices[gl_InstanceCustomIndexEXT].v[index.y];
	const Vertex v2 = vertices[gl_InstanceCustomIndexEXT].v[index.z];

	const vec3 barycentricCoords = vec3(1.0f - attribs.x - attribs.y, attribs.x, attribs.y);

	vertex.Position = v0.Position * barycentricCoords.x + v1.Position * barycentricCoords.y + v2.Position * barycentricCoords.z;
	vertex.Position = vec3(meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MeshTransform * vec4(vertex.Position, 1.0));
			debugPrintfEXT(": %f, %f, %f, %f \n", meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MeshTransform[3][0], meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MeshTransform[3][1], meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MeshTransform[3][2], meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MeshTransform[3][3]);
	vertex.Normal = v0.Normal * barycentricCoords.x + v1.Normal * barycentricCoords.y + v2.Normal * barycentricCoords.z;

	vertex.UV = v0.UV * barycentricCoords.x + v1.UV * barycentricCoords.y + v2.UV * barycentricCoords.z;
	vertex.UV += meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.UVOffset;

	vertex.Tangant = v0.Tangant * barycentricCoords.x + v1.Tangant * barycentricCoords.y + v2.Tangant * barycentricCoords.z;
	vertex.BiTangant = v0.BiTangant * barycentricCoords.x + v1.BiTangant * barycentricCoords.y + v2.BiTangant * barycentricCoords.z;

	return vertex;
}

PBRMaterial BuildPBRMaterial(MaterialProperties properties, vec2 UV)
{
	PBRMaterial material;

	material.Albedo = pow(properties.Albedo, vec3(2.2f));
	if (properties.AlbedoMap != 0)
	{
		material.Albedo = pow(texture(TextureMap[properties.AlbedoMap], UV).rgb, vec3(2.2f));
	}

	material.Metallic = properties.Metallic;
	if (properties.MetallicMap != 0)
	{
		material.Metallic = texture(TextureMap[properties.MetallicMap], UV).r;
	}

	material.Roughness = properties.Roughness;
	if (properties.RoughnessMap != 0)
	{
		material.Roughness = texture(TextureMap[properties.RoughnessMap], UV).r;
	}

	material.AmbientOcclusion = properties.AmbientOcclusion;
	if (properties.AmbientOcclusionMap != 0)
	{
		material.AmbientOcclusion = texture(TextureMap[properties.AmbientOcclusionMap], UV).r;
	}

	material.Emission = properties.Emission;
	if (properties.EmissionMap != 0)
	{
		material.Emission = texture(TextureMap[properties.EmissionMap], UV).rgb;
	}

	if (texture(TextureMap[properties.AlphaMap], UV).r == 0.0f ||
		texture(TextureMap[properties.AlbedoMap], UV).a == 0.0f)
	{
		//discard;
	}

	material.NormalMapID = properties.NormalMap;
	material.DepthMapID = properties.DepthMap;

	return material;
}

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0)) + 1.0;
    denom = (3.1415927410125732421875 * denom) * denom;
    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    float nom = NdotV;
    float denom = (NdotV * (1.0 - k)) + k;
    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float param = NdotV;
    float param_1 = roughness;
    float ggx2 = GeometrySchlickGGX(param, param_1);
    float param_2 = NdotL;
    float param_3 = roughness;
    float ggx1 = GeometrySchlickGGX(param_2, param_3);
    return ggx1 * ggx2;
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + ((max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0));
}


mat3 getTBNFromMap(Vertex vertex)
{
    vec3 T = normalize(mat3(meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MeshTransform[0].xyz, 
                            meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MeshTransform[1].xyz, 
                            meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MeshTransform[2].xyz) * 
                            vec3(vertex.Tangant));
    vec3 B = normalize(mat3(meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MeshTransform[0].xyz, 
                            meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MeshTransform[1].xyz, 
                            meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MeshTransform[2].xyz) * 
                            vec3(vertex.BiTangant));
    vec3 N = normalize(mat3(meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MeshTransform[0].xyz, 
                            meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MeshTransform[1].xyz, 
                            meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MeshTransform[2].xyz) * 
                            vertex.Normal);
    return mat3(vec3(T), vec3(B), vec3(N));
}

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

    Vertex vertex = BuildVertexInfo();
    const uint materialID = meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MaterialBufferIndex;
    MaterialProperties materialProp = materialBuffer[materialID].materialProperties;
    
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

    PBRMaterial material = BuildPBRMaterial(materialProp, vertex.UV);

    vec3 F0 = vec3(0.04f); 
    F0 = mix(F0, material.Albedo, material.Metallic);

    vec3 Lo = vec3(0.0);
    for (int x = 0; x < sceneData.DirectionalLightCount; x++)
    {
        vec3 L = normalize(-DLight[x].directionalLight.direction);
        vec3 H = normalize(V + L);
        float watts = DLight[x].directionalLight.intensity;
        vec3 radiance = DLight[x].directionalLight.diffuse * watts;

        float NDF = DistributionGGX(N, H, material.Roughness);
        float G = GeometrySmith(N, V, L, material.Roughness);
        vec3 F = fresnelSchlickRoughness(max(dot(H, V), 0.0), F0, material.Roughness);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - material.Metallic;

        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * material.Albedo / PI + specular) * radiance * NdotL;
    }

    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, material.Roughness);
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - material.Metallic;	  
    
    vec3 irradiance = texture(IrradianceMap, N).rgb;
    vec3 diffuse      = irradiance * material.Albedo;

    vec3 prefilteredColor = textureLod(PrefilterMap, R,  material.Roughness * sceneData.PBRMaxMipLevel).rgb;    
    vec2 brdf  = texture(BRDFMap, vec2(max(dot(N, V), 0.0), material.Roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = material.Emission + ((kD * diffuse + specular) * material.AmbientOcclusion);
    
    vec3 color = ambient + Lo;
    color = aces_fitted(color);
    color = pow(color, vec3(1.0f / 2.2f));


    rayHitInfo.color = color;
	

	debugPrintfEXT(": %i \n", meshBuffer[2].meshProperties.MaterialBufferIndex);

}