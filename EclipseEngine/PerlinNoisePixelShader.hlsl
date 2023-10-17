struct TextureCreatorProperties
{
    float time;
};
[[vk::push_constant]] TextureCreatorProperties textureCreatorProperties;

float2 randomGradient(float2 p)
{
    
    p = p + 0.02;
    float x = dot(p, float2(123.4, 234.5));
    float y = dot(p, float2(234.5, 345.6));
    float2 gradient = float2(x, y);
    gradient = sin(gradient);
    gradient = gradient * 43758.5453;
    
    gradient = sin(gradient + textureCreatorProperties.time);
    return gradient;
}

float2 quintic(float2 p)
{
    return p * p * p * (10.0 + p * (-15.0 + p * 6.0));
}

float4 main(float2 UV : TEXCOORD0) : SV_TARGET
{  
    float2 uv = UV;
    
    float3 black = float3(0.0, 0.0f, 0.0f);
    float3 white = float3(1.0, 1.0f, 1.0f);
    float3 color = black;
    
    uv = uv * 4.0;
    float2 gridId = floor(uv);
    float2 gridUv = frac(uv);
    color = float3(gridId, 0.0);
    color = float3(gridUv, 0.0);
    
    float2 bl = gridId + float2(0.0, 0.0);
    float2 br = gridId + float2(1.0, 0.0);
    float2 tl = gridId + float2(0.0, 1.0);
    float2 tr = gridId + float2(1.0, 1.0);
    
    float2 gradBl = randomGradient(bl);
    float2 gradBr = randomGradient(br);
    float2 gradTl = randomGradient(tl);
    float2 gradTr = randomGradient(tr);
    
    float2 distFromPixelToBl = gridUv - float2(0.0, 0.0);
    float2 distFromPixelToBr = gridUv - float2(1.0, 0.0);
    float2 distFromPixelToTl = gridUv - float2(0.0, 1.0);
    float2 distFromPixelToTr = gridUv - float2(1.0, 1.0);
    
    float dotBl = dot(gradBl, distFromPixelToBl);
    float dotBr = dot(gradBr, distFromPixelToBr);
    float dotTl = dot(gradTl, distFromPixelToTl);
    float dotTr = dot(gradTr, distFromPixelToTr);

    gridUv = quintic(gridUv);
    
    float b = lerp(dotBl, dotBr, gridUv.x);
    float t = lerp(dotTl, dotTr, gridUv.x);
    float perlin = lerp(b, t, gridUv.y);

    color = float3(perlin + 0.2, perlin + 0.2, perlin + 0.2);
    return float4(color, 1.0f);
}