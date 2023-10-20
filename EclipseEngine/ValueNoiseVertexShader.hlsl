struct VSInput
{
    uint VertexID : SV_VertexID;
};

struct VSOutput
{
    float4 Pos : SV_POSITION;
    [[vk::location(0)]] float2 UV : TEXCOORD0;
};

VSOutput main(VSInput input)
{
    VSOutput output = (VSOutput) 0;
    
    output.UV = float2((input.VertexID << 1) & 2, input.VertexID & 2);
    output.Pos = float4(output.UV * 2.0f - 1.0f, 0.0f, 1.0f);
    
    return output;
}