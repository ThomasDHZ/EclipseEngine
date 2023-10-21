struct SceneProperties
{
    uint MeshIndex;
    uint TransformIndex;
    uint MaterialIndex;
    uint ReflectionIndex;
    float4x4 proj;
    float4x4 view;
    float3 CameraPos;
    float3 MeshColorID;
    float3 AmbientLight;
    uint SunLightCount;
    uint DirectionalLightCount;
    uint PointLightCount;
    uint SpotLightCount;
    float Timer;
    float PBRMaxMipLevel;
    uint frame;
    int MaxReflectCount;
};