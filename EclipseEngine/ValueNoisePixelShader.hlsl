struct TextureCreatorProperties
{
    float time;
};
[[vk::push_constant]] TextureCreatorProperties textureCreatorProperties;

float whiteNoise2x1(float2 p)
{
    float random = dot(p, float2(12., 78.));
    random = sin(random);
    random = random * 43758.5453;
    random = frac(random);
    return random;
}

float2 quintic(float2 p)
{
    return p * p * p * (10.0 + p * (-15.0 + p * 6.0));
}

float valueNoiseFn(float2 uv)
{
    float2 gridUv = frac(uv);
    float2 gridId = floor(uv);

    gridUv = quintic(gridUv);

    float botLeft = whiteNoise2x1(gridId);
    float botRight = whiteNoise2x1(gridId + float2(1.0, 0.0));
    float b = lerp(botLeft, botRight, gridUv.x);

    float topLeft = whiteNoise2x1(gridId + float2(0.0, 1.0));
    float topRight = whiteNoise2x1(gridId + float2(1.0, 1.0));
    float t = lerp(topLeft, topRight, gridUv.x);

    float noise = lerp(b, t, gridUv.y);

    return noise;
}

float4 main(float2 UV : TEXCOORD0) : SV_TARGET
{
    float2 uv = UV;
    uv.y = 1 - UV.y;
    
    float3 color = float3(1.0.rrr);
    
    uv += textureCreatorProperties.time / 10.0;
    float vn = valueNoiseFn(uv * 4.0) * 1.0;
    vn += valueNoiseFn(uv * 8.0) * 0.5;
    vn += valueNoiseFn(uv * 16.0) * 0.25;
    vn += valueNoiseFn(uv * 32.0) * 0.125;
    vn += valueNoiseFn(uv * 64.0) * 0.0625;
    vn /= 2.0;
    color = float3(vn.rrr);
    
    return float4(color.rgb, 1.0f);
}