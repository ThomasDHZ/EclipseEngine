struct TextureCreatorProperties
{
    float time;
};
[[vk::push_constant]] TextureCreatorProperties textureCreatorProperties;
[[vk::binding(0)]]  TextureCube CubeMap;
[[vk::binding(0)]]  SamplerState CubeMapSampler;

static float PI = 3.14159265359;
static float2 invAtan = float2(0.1591, 0.3183);

float4 main(float2 UV : TEXCOORD0) : SV_TARGET
{
    float3 cubemapTexCoords = float3(0.0f.rrr);
    cubemapTexCoords.x = -sin(UV.x * PI * 2.0f) * sin(UV.y * PI);
    cubemapTexCoords.y = cos(UV.y * PI);
    cubemapTexCoords.z = -cos(UV.x * PI * 2.0f) * sin(UV.y * PI);
    
    return float4(CubeMap.Sample(CubeMapSampler, cubemapTexCoords));
}
