static float PI = 3.14159265359f;

static float4x4 LightBiasMatrix = float4x4(
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.5, 0.5, 0.0, 1.0);

static float3 gridSamplingDisk[20] =
{
    float3(1, 1, 1), float3(1, -1, 1), float3(-1, -1, 1), float3(-1, 1, 1),
    float3(1, 1, -1), float3(1, -1, -1), float3(-1, -1, -1), float3(-1, 1, -1),
    float3(1, 1, 0), float3(1, -1, 0), float3(-1, -1, 0), float3(-1, 1, 0),
    float3(1, 0, 1), float3(-1, 0, 1), float3(1, 0, -1), float3(-1, 0, -1),
    float3(0, 1, 1), float3(0, -1, 1), float3(0, -1, -1), float3(0, 1, -1)
};

float ShadowCalculation(float4 fragPosLightSpace, float2 offset, int index)
{
    float shadow = 1.0f;
    if (fragPosLightSpace.z > -1.0 && fragPosLightSpace.z < 1.0)
    {
        float dist = ShadowMap[index].Sample(ShadowMapSampler[index], fragPosLightSpace.xy + offset).r;
        if (fragPosLightSpace.w > 0.0 && dist < fragPosLightSpace.z)
        {
            shadow = 0.1f;
        }
    }
    return shadow;
}

float filterPCF(float4 sc, int index)
{
    float2 texDim;
    ShadowMap[index].GetDimensions(texDim.x, texDim.y);
    
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
            shadowFactor += ShadowCalculation(sc, float2(dx * x, dy * y), index);
            count++;
        }

    }
    return shadowFactor / count;
}

float CubeShadowCalculation(float3 fragPos, float3 viewPos, int index)
{
    float near_plane = 0.1f;
    float far_plane = PointLightBuffer[index].radius;

    float3 fragToLight = fragPos - PointLightBuffer[index].position;
    float currentDepth = length(fragToLight);

    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    for (int i = 0; i < samples; ++i)
    {
        float closestDepth = PointShadowMap[index].Sample(PointShadowSampler[index], fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane; // undo mapping [0;1]
        if (currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);

    return shadow;
}

float DistributionGGX(float3 N, float3 H, float roughness)
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

float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
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

float3 fresnelSchlickRoughness(float cosTheta, float3 F0, float roughness)
{
    return F0 + ((max(float3(1.0 - roughness, 1.0 - roughness, 1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0));
}

float3 DirectionalLightCalc(float3 V, float3 N, float3 F0, float3 Pos, MaterialProperties material)
{
    float3 Lo = float3(0.0.rrr);
    for (int x = 0; x < sceneDataProperties.DirectionalLightCount; x++)
    {
        float3 L = normalize(-DirectionalLightBuffer[x].direction);
        float3 H = normalize(V + L);
        float watts = DirectionalLightBuffer[x].intensity;
        float3 radiance = DirectionalLightBuffer[x].diffuse * watts;

        float NDF = DistributionGGX(N, H, material.Roughness);
        float G = GeometrySmith(N, V, L, material.Roughness);
        float3 F = fresnelSchlickRoughness(max(dot(H, V), 0.0), F0, material.Roughness);

        float3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        float3 specular = numerator / denominator;

        float3 kS = F;
        float3 kD = float3(1.0.rrr) - kS;
        kD *= 1.0 - material.Metallic;

        float NdotL = max(dot(N, L), 0.0);
        
        float4 LightSpace = mul(float4(Pos, 1.0), mul(MeshPropertiesBuffer[sceneDataProperties.MeshIndex].MeshTransform, mul(transpose(DirectionalLightBuffer[0].LightSpaceMatrix), LightBiasMatrix)));
        float shadow = filterPCF(LightSpace / LightSpace.w, x);

        Lo += (kD * material.Albedo.rgb / PI + specular) * radiance * NdotL;
        Lo *= shadow;
    }
    return Lo;
}

float3 PointLightCalc(float3 V, float3 N, float3 F0, float3 Pos, MaterialProperties material)
{
    float3 Lo = float3(0.0.rrr);
    for (int x = 0; x < sceneDataProperties.PointLightCount; x++)
    {
        float3 L = normalize(PointLightBuffer[x].position - Pos);
        float3 H = normalize(V + L);

        float distance = length(PointLightBuffer[x].position - Pos);
        float Kc = 1.0f;
        float Kl = 2 / PointLightBuffer[x].radius;
        float Kq = 1 / (PointLightBuffer[x].radius * PointLightBuffer[x].radius);
        float attenuation = 1.0f / (Kc + Kl * distance + Kq * (distance * distance));

        float watts = PointLightBuffer[x].intensity;
        float3 radiance = PointLightBuffer[x].diffuse * watts * attenuation;

        float NDF = DistributionGGX(N, H, material.Roughness);
        float G = GeometrySmith(N, V, L, material.Roughness);
        float3 F = fresnelSchlickRoughness(max(dot(H, V), 0.0), F0, material.Roughness);

        float3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        float3 specular = numerator / denominator;

        float3 kS = F;
        float3 kD = float3(1.0.rrr) - kS;
        kD *= 1.0 - material.Metallic;

        float NdotL = max(dot(N, L), 0.0);

        float EPSILON = 0.15;
        float SHADOW_OPACITY = 0.005;

        float3 lightVec = Pos - PointLightBuffer[x].position;
        float dist = length(lightVec);

        float shadow = CubeShadowCalculation(Pos, V, x);

        Lo += (kD * material.Albedo / PI + specular) * radiance * NdotL;
        Lo *= 1.0f - float3(shadow.rrr);
    }
    return Lo;
}