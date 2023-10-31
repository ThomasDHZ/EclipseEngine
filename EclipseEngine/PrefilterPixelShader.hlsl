struct VSOutput
{
    [[vk::location(0)]] float3 WorldPos : POSITION0;
};

struct PrefilterSamplerProperties
{
    uint CubeMapId;
    uint res;
    float Roughness;
};

[[vk::push_constant]] PrefilterSamplerProperties prefilterSamplerProperties;
[[vk::binding(0)]] TextureCube CubeMap[] : register(t0);
[[vk::binding(0)]] SamplerState CubeMapSampler[] : register(s0);

static float PI = 3.14159265359f;

float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
 
float RadicalInverse_VdC(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

float2 Hammersley(uint i, uint N)
{
    return float2(float(i) / float(N), RadicalInverse_VdC(i));
}

float3 ImportanceSampleGGX(float2 Xi, float3 N, float roughness)
{
    float a = roughness * roughness;
	
    float phi = 2.0 * PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a * a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
	
    float3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;
	
    float3 up = abs(N.z) < 0.999 ? float3(0.0, 0.0, 1.0) : float3(1.0, 0.0, 0.0);
    float3 tangent = normalize(cross(up, N));
    float3 bitangent = cross(N, tangent);
	
    float3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);
}


float4 main(VSOutput input) : SV_TARGET
{
    float3 N = normalize(input.WorldPos);
    
    float3 R = N;
    float3 V = R;

    const uint SAMPLE_COUNT = 1024u;
    float3 prefilteredColor = float3(0.0f, 0.0f, 0.0f);
    float totalWeight = 0.0;
    
    //int2 CubeMapResolution;
   // CubeMap[prefilterSamplerProperties.CubeMapId].GetDimensions(CubeMapResolution.x, CubeMapResolution.y);
    
    for (uint x = 0u; x < SAMPLE_COUNT; ++x)
    {
        float2 Xi = Hammersley(x, SAMPLE_COUNT);
        float3 H = ImportanceSampleGGX(Xi, N, prefilterSamplerProperties.Roughness);
        float3 L = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(dot(N, L), 0.0);
        if (NdotL > 0.0)
        {
            float D = DistributionGGX(N, H, prefilterSamplerProperties.Roughness);
            float NdotH = max(dot(N, H), 0.0);
            float HdotV = max(dot(H, V), 0.0);
            float pdf = D * NdotH / (4.0 * HdotV) + 0.0001;

            float saTexel = 4.0 * PI / (6.0 * prefilterSamplerProperties.res * prefilterSamplerProperties.res);
            float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);

            float mipLevel = prefilterSamplerProperties.Roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);
            
            prefilteredColor += clamp(CubeMap[prefilterSamplerProperties.CubeMapId].SampleLevel(CubeMapSampler[prefilterSamplerProperties.CubeMapId], L, mipLevel).rgb, 0.0f, 1.0) * NdotL;
            totalWeight += NdotL;
        }
    }

    prefilteredColor = prefilteredColor / totalWeight;

    return float4(prefilteredColor, 1.0);
}