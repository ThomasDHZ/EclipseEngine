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

layout(location = 0) out vec4 outColor;

layout(binding = 0) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
layout(binding = 1) buffer MaterialPropertiesBuffer { MaterialProperties materialProperties; } materialBuffer[];
layout(binding = 2) buffer DirectionalLightBuffer { DirectionalLight directionalLight; } DLight[];
layout(binding = 3) buffer PointLightBuffer { PointLight pointLight; } PLight[];
layout(binding = 4) buffer SpotLightBuffer { SpotLight spotLight; } SLight[];
layout(binding = 5) uniform sampler2D TextureMap[];
//layout(binding = 6) uniform sampler2D ShadowMap[];

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

const mat4 LightBiasMatrix = mat4(
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.5, 0.5, 0.0, 1.0);

vec3 CalcNormalDirLight(MaterialProperties material, mat3 TBN, vec3 normal, vec2 uv, int index);
vec3 CalcNormalPointLight(MaterialProperties material, mat3 TBN, vec3 normal, vec2 uv, int index);
vec3 CalcNormalSpotLight(MaterialProperties material, mat3 TBN, vec3 normal, vec2 uv, int index);
vec2 ParallaxMapping(MaterialProperties material, vec2 texCoords, vec3 viewDir);

/*float ShadowCalculation(vec4 fragPosLightSpace, vec2 offset, int index)
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

/*float filterPCF(vec4 sc, int index)
{
	ivec2 texDim = textureSize(ShadowMap[index], 0);
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
*/

void main() {

//debugPrintfEXT(": %i \n", sceneData.MeshIndex);
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

   vec3 result = vec3(0.0f);
   for(int x = 0; x < sceneData.DirectionalLightCount; x++)
   {
        result += CalcNormalDirLight(material, TBN, normal, FinalUV, x);
   }
//   for(int x = 0; x < sceneData.PointLightCount; x++)
//   {
//        result += CalcNormalPointLight(material, TBN, normal, FinalUV, x);   
//   }
//   for(int x = 0; x < sceneData.SpotLightCount; x++)
//   {
//        result += CalcNormalSpotLight(material, TBN, normal, FinalUV, x);   
//   }
//    vec3 I = normalize(FragPos2 - ViewPos);
//    vec3 R = reflect(I, normalize(normal));
//    vec3 Reflection = texture(CubeMap[0], R).rgb;
//    vec3 finalMix = mix(result, Reflection, material.Reflectivness);

    vec3 finalResult = vec3(1.0) - exp(-result * 1.0f);
    finalResult = pow(finalResult, vec3(1.0 / 2.2f));
    outColor = vec4(finalResult, material.AlphaMapID);

}

vec3 CalcNormalDirLight(MaterialProperties material, mat3 TBN, vec3 normal, vec2 uv, int index)
{
    vec3 LightPos = DLight[index].directionalLight.direction;

    vec3 ViewPos = sceneData.CameraPos;
    vec3 FragPos2 = FragPos;
    if (material.NormalMapID != 0)
    {
        LightPos = TBN * DLight[index].directionalLight.direction;
        ViewPos = TBN * sceneData.CameraPos;
        FragPos2 = TBN * FragPos;
    }
    vec3 ViewDir = normalize(ViewPos - FragPos2);

    const vec3 lightDir = normalize(-LightPos);
    const float diff = max(dot(normal, lightDir), 0.0);

    const vec3 halfwayDir = normalize(lightDir + ViewDir);
    const float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0f);

    vec3 ambient = sceneData.AmbientLight * material.Diffuse.rgb;
    vec3 diffuse = DLight[index].directionalLight.diffuse * diff * material.Diffuse.rgb;
    vec3 specular = DLight[index].directionalLight.specular * spec * material.Specular;
    if (material.DiffuseMapID != 0)
    {
        ambient = sceneData.AmbientLight * vec3(texture(TextureMap[material.DiffuseMapID], uv));
        diffuse = DLight[index].directionalLight.diffuse * diff * vec3(texture(TextureMap[material.DiffuseMapID], uv));
    }
    if (material.SpecularMapID != 0)
    {
        specular = DLight[index].directionalLight.specular * spec * vec3(texture(TextureMap[material.SpecularMapID], uv));
    }

    float LightDistance = length(LightPos - FragPos2);


    vec4 LightSpace = (LightBiasMatrix *  DLight[index].directionalLight.lightSpaceMatrix * meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform) * vec4(FragPos, 1.0);
   //float shadow = filterPCF(LightSpace/ LightSpace.w, index);  
    return (ambient + diffuse + specular);
}

vec3 CalcNormalPointLight(MaterialProperties material, mat3 TBN, vec3 normal, vec2 uv, int index)
{
    vec3 LightPos = PLight[index].pointLight.position;
    vec3 ViewPos = sceneData.CameraPos;
    vec3 FragPos2 = FragPos;
    if (material.NormalMapID != 0)
    {
        LightPos = TBN * PLight[index].pointLight.position;
        ViewPos = TBN * sceneData.CameraPos;
        FragPos2 = TBN * FragPos;
    }
    vec3 ViewDir = normalize(ViewPos - FragPos2);

    vec3 lightDir = normalize(LightPos - FragPos2);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + ViewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.Shininess);

    vec3 ambient = sceneData.AmbientLight * material.Diffuse.rgb;
    vec3 diffuse = PLight[index].pointLight.diffuse * diff * material.Diffuse.rgb;
    vec3 specular = PLight[index].pointLight.specular * spec * material.Specular;
    if (material.DiffuseMapID != 0)
    {
        ambient = sceneData.AmbientLight * vec3(texture(TextureMap[material.DiffuseMapID], uv));
        diffuse = PLight[index].pointLight.diffuse * diff * vec3(texture(TextureMap[material.DiffuseMapID], uv));
    }
    if (material.SpecularMapID != 0)
    {
        specular = PLight[index].pointLight.specular * spec * vec3(texture(TextureMap[material.SpecularMapID], uv));
    }

    float LightDistance = length(LightPos - FragPos2);
    float attenuation = 1.0 / (1.0f + PLight[index].pointLight.linear * LightDistance + PLight[index].pointLight.quadratic * (LightDistance * LightDistance));

    return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcNormalSpotLight(MaterialProperties material, mat3 TBN, vec3 normal, vec2 uv, int index)
{
    vec3 LightPos = SLight[index].spotLight.position;
    vec3 ViewPos = sceneData.CameraPos;
    vec3 FragPos2 = FragPos;
    if (material.NormalMapID != 0)
    {
        LightPos = TBN * SLight[index].spotLight.direction;
        ViewPos = TBN * sceneData.CameraPos;
        FragPos2 = TBN * FragPos;
    }
    vec3 ViewDir = normalize(ViewPos - FragPos2);

    vec3 lightDir = normalize(LightPos - FragPos2);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + ViewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.Shininess);

    vec3 ambient = sceneData.AmbientLight * material.Diffuse.rgb;
    vec3 diffuse = SLight[index].spotLight.diffuse * diff * material.Diffuse.rgb;
    vec3 specular = SLight[index].spotLight.specular * spec * material.Specular;
    if (material.DiffuseMapID != 0)
    {
        ambient = sceneData.AmbientLight * vec3(texture(TextureMap[material.DiffuseMapID], uv));
        diffuse = SLight[index].spotLight.diffuse * diff * vec3(texture(TextureMap[material.DiffuseMapID], uv));
    }
    if (material.SpecularMapID != 0)
    {
        specular = SLight[index].spotLight.specular * spec * vec3(texture(TextureMap[material.SpecularMapID], uv));
    }

    float theta = dot(lightDir, normalize(-SLight[index].spotLight.direction)); 
    float epsilon = SLight[index].spotLight.cutOff - SLight[index].spotLight.outerCutOff;
    float intensity = clamp((theta - SLight[index].spotLight.outerCutOff) / epsilon, 0.0, 1.0);

    float LightDistance = length(LightPos - FragPos2);
    float attenuation = 1.0 / (1.0f + SLight[index].spotLight.linear * LightDistance + SLight[index].spotLight.quadratic * (LightDistance * LightDistance));

    return (ambient + diffuse + specular) * attenuation;
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