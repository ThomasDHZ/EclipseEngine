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


struct Vertex
{
    vec3 Position;
    float PositionPadding;
    vec3 Normal;
    float NormalPadding;
    vec2 UV;
    vec2 UVPadding;
    vec3 Tangant;
    float TangantPadding;
    vec3 BiTangant;
    float BiTangentPadding;
    vec3 Color;
    float ColorPadding;
};

struct MaterialProperties
{
    vec3 Albedo;
    float Metallic;
    float Roughness;
    float AmbientOcclusion;
    vec3 Emission;
    float Alpha;
    uint AlbedoMap;
    uint MetallicRoughnessMap;
    uint MetallicMap;
    uint RoughnessMap;
    uint AmbientOcclusionMap;
    uint NormalMap;
    uint DepthMap;
    uint AlphaMap;
    uint EmissionMap;
};

struct MeshProperties
{
    uint VertexBufferIndex;
    uint IndexBufferIndex;
    uint MaterialBufferIndex;
    uint ReflectionIndex;
    uint AlbedoMapIndex;
    uint MetallicRoughnessMapIndex;
    uint AmbientOcclusionMapIndex;
    uint NormalMapIndex;
    uint DepthMapIndex;
    uint AlphaMapIndex;
    uint EmissionMapIndex;
    uint SkyBoxIndex;
    mat4 MeshTransform;
    vec2 UVOffset;
    vec2 UVScale;
    vec2 UVFlip;
    int SelectedMesh;
    float heightScale;
    float minLayers;
    float maxLayers;
};

struct DirectionalLight
{
    vec3 diffuse;
    vec3 direction;
    mat4 LightSpaceMatrix;
    float intensity;
};

struct PointLight
{
    vec3 diffuse;
    vec3 position;
    mat4 LightSpaceMatrix;
    float intensity;
    float radius;
};

struct SunLight
{
    vec3 diffuse;
    vec3 position;
    mat4 LightSpaceMatrix;
    float intensity;
};

struct SpotLight
{
    vec3 diffuse;
    vec3 position;
    vec3 direction;
    mat4 LightSpaceMatrix;
    float intensity;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
    mat4 lightSpaceMatrix;
};

layout(push_constant) uniform SceneData
{
    uint MeshIndex;
    uint PrimitiveIndex;
    uint MaterialIndex;
    mat4 proj;
    mat4 view;
    vec3 CameraPos;
    vec3 MeshColorID;
    vec3 AmbientLight;
    uint SunLightCount;
    uint DirectionalLightCount;
    uint PointLightCount;
    uint SpotLightCount;
    float Timer;
    float PBRMaxMipLevel;
    uint frame;
    int MaxRefeflectCount;
} sceneData;

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
    if (fragPosLightSpace.z > -1.0 && fragPosLightSpace.z < 1.0)
    {
        float dist = texture(ShadowMap[index], fragPosLightSpace.st + offset).r;
        if (fragPosLightSpace.w > 0.0 && dist < fragPosLightSpace.z)
        {
            shadow = 0.1f;
        }
    }
    return shadow;
}

float filterPCF(vec4 sc, int index)
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
            shadowFactor += ShadowCalculation(sc, vec2(dx * x, dy * y), index);
            count++;
        }

    }
    return shadowFactor / count;
}

float CubeShadowCalculation(vec3 fragPos, vec3 viewPos, int index)
{
    float near_plane = 1.0f;
    float far_plane = 25.0f;

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

Vertex RasterVertexBuilder()
{
    Vertex vertex;
    vertex.Position = FragPos;
    vertex.UV = UV;
    vertex.Normal = Normal;
    vertex.Tangant = Tangent;
    vertex.BiTangant = BiTangent;
    vertex.Color = Color;
    vertex.UV += meshBuffer[sceneData.MeshIndex].meshProperties.UVOffset;
    vertex.UV *= meshBuffer[sceneData.MeshIndex].meshProperties.UVScale;
    if (meshBuffer[sceneData.MeshIndex].meshProperties.UVFlip.y == 1.0)
    {
        vertex.UV.y = 1.0 - vertex.UV.y;
    }
    if (meshBuffer[sceneData.MeshIndex].meshProperties.UVFlip.x == 1.0)
    {
        vertex.UV.x = 1.0 - vertex.UV.x;
    }
    return vertex;
}

MaterialProperties BuildPBRMaterial(uint materialId, vec2 UV_1)
{
    MaterialProperties material = materialBuffer[materialId].materialProperties;

 //  material.Albedo = material.Albedo;
//	if (material.AlbedoMap != 0)
//	{
	    material.Albedo = texture(TextureMap[material.AlbedoMap], UV).rgb;
	//}

   // material.Metallic = material.Metallic;
	if (material.MetallicRoughnessMap != 0)
	{
        material.Metallic = texture(TextureMap[material.MetallicMap], UV).r;
        material.Roughness = texture(TextureMap[material.RoughnessMap], UV).r;
	}
    else
    {
        material.Metallic = texture(TextureMap[material.MetallicRoughnessMap], UV).b;
        material.Roughness = texture(TextureMap[material.MetallicRoughnessMap], UV).g;
    }

   // material.Roughness = material.Roughness;
	//if (material.MetallicRoughnessMap != 0)
	//{
	    
//	}

//material.AmbientOcclusion = material.AmbientOcclusion;
	//if (material.AmbientOcclusionMap != 0)
	//{
		material.AmbientOcclusion = 0.1f;
	//}

	//material.Emission = material.Emission;
	//if (material.EmissionMap != 0)
	//{
		material.Emission = texture(TextureMap[material.EmissionMap], UV).rgb;
	//}

    if(texture(TextureMap[material.AlbedoMap], UV).a == 0.0f)
	{
		discard;
	}

    return material;
}

mat3 getTBNFromMap(Vertex vertex)
{
    vec3 T = normalize(mat3(meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform[0].xyz, meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform[1].xyz, meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform[2].xyz) * vec3(vertex.Tangant));
    vec3 B = normalize(mat3(meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform[0].xyz, meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform[1].xyz, meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform[2].xyz) * vec3(vertex.BiTangant));
    vec3 N = normalize(mat3(meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform[0].xyz, meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform[1].xyz, meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform[2].xyz) * vertex.Normal);
    return mat3(vec3(T), vec3(B), vec3(N));
}

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

vec3 CalcDirectionalLight(vec3 F0, vec3 V, vec3 N, MaterialProperties material)
{
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

        vec4 LightSpace = (LightBiasMatrix * DLight[x].directionalLight.LightSpaceMatrix * meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform) * vec4(FragPos, 1.0);
        float shadow = filterPCF(LightSpace / LightSpace.w, x);

        Lo += (kD * material.Albedo / PI + specular) * radiance * NdotL;
        Lo *= shadow;
    }
    return Lo;
}

vec3 CalcPointLight(vec3 F0, vec3 V, vec3 N, vec3 viewPos, Vertex vertex, MaterialProperties material)
{
    vec3 Lo = vec3(0.0);
    for (int x = 0; x < sceneData.PointLightCount; x++)
    {
        vec3 L = normalize(PLight[x].pointLight.position - vertex.Position);
        vec3 H = normalize(V + L);

        float distance = length(PLight[x].pointLight.position - vertex.Position);
        float Kc = 1.0f;
        float Kl = 2 / PLight[x].pointLight.radius;
        float Kq = 1 / (PLight[x].pointLight.radius * PLight[x].pointLight.radius);
        float attenuation = 1.0f / (Kc + Kl * distance + Kq * (distance * distance));

        float watts = PLight[x].pointLight.intensity;
        vec3 radiance = PLight[x].pointLight.diffuse * watts * attenuation;

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
       // vec3 fragPos, vec3 viewPos, int index
       float EPSILON = 0.15;
       float SHADOW_OPACITY = 0.005;

       vec3 lightVec = vertex.Position - PLight[x].pointLight.position;
       float dist = length(lightVec);

        float shadow = (dist <= texture(PointShadowMap[x], lightVec).r + EPSILON) ? 1.0 : SHADOW_OPACITY;

        Lo += (kD * material.Albedo / PI + specular) * radiance * NdotL;
        Lo *= vec3(shadow);
    }
    return Lo;
}

void main()
{ 
    Vertex vertex = RasterVertexBuilder();
    MaterialProperties material = BuildPBRMaterial(sceneData.MaterialIndex, vertex.UV);
    
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
  // Lo += CalcDirectionalLight(F0, V, N, material);
   Lo += CalcPointLight(F0, V, N, ViewPos, vertex, material);
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
    
    vec3 color = Lo;
    color = color / (color + vec3(1.0f));
    color = pow(color, vec3(1.0f/2.2f)); 

    if(meshBuffer[sceneData.MeshIndex].meshProperties.SelectedMesh == 1)
    {
        color = mix(color, vec3(1.0f, 0.0f, 0.0f), .35f);
    }

     outColor = vec4(color, 1.0f);
}