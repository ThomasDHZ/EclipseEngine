struct DirectionalLight
{
    float3 diffuse;
    float3 direction;
    float4x4 LightSpaceMatrix;
    float intensity;
};

struct PointLight
{
    float3 diffuse;
    float3 position;
    float4x4 LightSpaceMatrix;
    float intensity;
    float radius;
};

struct SunLight
{
    float3 diffuse;
    float3 position;
    float4x4 LightSpaceMatrix;
    float intensity;
};

struct SpotLight
{
    float3 diffuse;
    float3 position;
    float3 direction;
    float4x4 lightSpaceMatrix;
    float intensity;
    float radius;
    float cutOff;
    float outerCutOff;
    float constant;
    float Linear;
    float quadratic;
};