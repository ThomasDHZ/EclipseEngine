struct VSInput
{
    [[vk::location(0)]] float3 Position : POSITION0;
};

struct VSOutput
{
    float4 Pos : SV_POSITION;
    [[vk::location(0)]] float3 UV : TEXCOORD0;
};

struct SceneDataProperties
{
    float4x4 proj;
    float4x4 view;
};

[[vk::push_constant]] SceneDataProperties sceneDataProperties;

VSOutput main(VSInput input)
{
    float4 pos = mul(sceneDataProperties.proj, mul(sceneDataProperties.view, float4(input.Position.xyz, 1.0)));
    
    VSOutput output = (VSOutput) 0;
     
    output.UV = input.Position;
    output.Pos = pos.xyww;
    return output;
}