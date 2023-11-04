#include "Vertex.hlsli"
#include "SceneProperties.hlsli"
#include "MeshProperties.hlsli"
#include "MaterialProperties.hlsli"
#include "LightProperties.hlsli"

struct RayPayload
{
    float3 color;
    uint seed;
    int reflectCount;
    int reflectCount2;
};



struct ModelMatrix
{
    float4x4 model;
};

struct Attributes
{
    float2 bary;
};

[[vk::push_constant]] SceneProperties sceneDataProperties;
[[vk::binding(0)]]  RaytracingAccelerationStructure topLevelAS;
[[vk::binding(1)]]  RWTexture2D<float4> image;
[[vk::binding(2)]] StructuredBuffer<Vertex> VertexBuffer[];
[[vk::binding(3)]] StructuredBuffer<uint> IndexBuffer[];
[[vk::binding(4)]]  ConstantBuffer<MeshProperties> MeshPropertiesBuffer[];
[[vk::binding(5)]]  ConstantBuffer<ModelMatrix> ModelTransformBuffer[];
[[vk::binding(6)]]  ConstantBuffer<MaterialProperties> MaterialPropertiesBuffer[];
[[vk::binding(7)]]  Texture2D TextureMap[];
[[vk::binding(7)]]  SamplerState TextureMapSampler[];
[[vk::binding(8)]]  Texture2D BRDFMap;
[[vk::binding(8)]]  SamplerState BRDFMapSampler;
[[vk::binding(9)]]  TextureCube IrradianceMap;
[[vk::binding(9)]]  SamplerState IrradianceMapSampler;
[[vk::binding(10)]]  TextureCube PrefilterMap;
[[vk::binding(10)]]  SamplerState PrefilterMapSampler;
[[vk::binding(11)]]  ConstantBuffer<SunLight> SunLightBuffer[];
[[vk::binding(12)]]  ConstantBuffer<DirectionalLight> DirectionalLightBuffer[];
[[vk::binding(13)]]  ConstantBuffer<PointLight> PointLightBuffer[];
[[vk::binding(14)]] ConstantBuffer<SpotLight> SpotLightBuffer[];

Vertex BuildVertexInfo(Attributes attribs)
{
    Vertex vertex;
    float a = IndexBuffer[InstanceIndex()][3 * PrimitiveIndex()];
    float b = IndexBuffer[InstanceIndex()][3 * PrimitiveIndex() + 1];
    float c = IndexBuffer[InstanceIndex()][3 * PrimitiveIndex() + 2];
    const int3 index = float3(a, b, c);
    
    const Vertex v0 = VertexBuffer[InstanceIndex()][index.x];
    const Vertex v1 = VertexBuffer[InstanceIndex()][index.y];
    const Vertex v2 = VertexBuffer[InstanceIndex()][index.z];
    
    const float3 barycentricCoords = float3(1.0f - attribs.bary.x - attribs.bary.y, attribs.bary.x, attribs.bary.y);

    float4x4 mat = float4x4(1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f);
    
    vertex.Position = v0.Position * barycentricCoords.x + v1.Position * barycentricCoords.y + v2.Position * barycentricCoords.z;
    vertex.Position = mul(mat, float4(vertex.Position, 1.0)).xyz;
    
    vertex.Normal = v0.Normal * barycentricCoords.x + v1.Normal * barycentricCoords.y + v2.Normal * barycentricCoords.z;
   // printf("Vertex position: (%.2f, %.2f, %.2f, %.2f)\n", ModelTransformBuffer[InstanceIndex()].model[0][0], ModelTransformBuffer[InstanceIndex()].model[0][1], ModelTransformBuffer[InstanceIndex()].model[0][2], ModelTransformBuffer[InstanceIndex()].model[0][3]);
   // printf("Vertex position 2: (%.2f, %.2f, %.2f, %.2f)\n", ModelTransformBuffer[InstanceIndex()].model[1][0], ModelTransformBuffer[InstanceIndex()].model[1][1], ModelTransformBuffer[InstanceIndex()].model[1][2], ModelTransformBuffer[InstanceIndex()].model[1][3]);
   // printf("Vertex position 3: (%.2f, %.2f, %.2f, %.2f)\n", mat[2][0], mat[2][1], mat[2][2], mat[2][3]);
  //  printf("Vertex position 4: (%.2f, %.2f, %.2f, %.2f)\n", ModelTransformBuffer[InstanceIndex()].model[3][0], ModelTransformBuffer[InstanceIndex()].model[3][1], ModelTransformBuffer[InstanceIndex()].model[3][2], ModelTransformBuffer[InstanceIndex()].model[3][3]);
        
    
    vertex.UV = v0.UV * barycentricCoords.x + v1.UV * barycentricCoords.y + v2.UV * barycentricCoords.z;
    vertex.UV += MeshPropertiesBuffer[InstanceIndex()].UVOffset;

    vertex.Tangant = v0.Tangant * barycentricCoords.x + v1.Tangant * barycentricCoords.y + v2.Tangant * barycentricCoords.z;
    vertex.BiTangant = v0.BiTangant * barycentricCoords.x + v1.BiTangant * barycentricCoords.y + v2.BiTangant * barycentricCoords.z;

    return vertex;
}

MaterialProperties MaterialBuilder(float2 UV, uint MaterialIndex)
{
    MaterialProperties material = MaterialPropertiesBuffer[sceneDataProperties.MaterialIndex];
    
    if (material.AlbedoMap != -1)
    {
        material.Albedo = TextureMap[material.AlbedoMap].Sample(TextureMapSampler[material.AlbedoMap], UV).rgb;
    }
    if (material.MetallicRoughnessMap != -1)
    {
        material.Roughness = TextureMap[material.MetallicRoughnessMap].Sample(TextureMapSampler[material.MetallicRoughnessMap], UV).g;
        material.Metallic = TextureMap[material.MetallicRoughnessMap].Sample(TextureMapSampler[material.MetallicRoughnessMap], UV).b;
    }
    if (material.MetallicMap != -1)
    {
        material.Metallic = TextureMap[material.MetallicMap].Sample(TextureMapSampler[material.MetallicMap], UV).r;
    }
    if (material.RoughnessMap != -1)
    {
        material.Roughness = TextureMap[material.RoughnessMap].Sample(TextureMapSampler[material.RoughnessMap], UV).r;
    }
    if (material.AmbientOcclusionMap != -1)
    {
        material.AmbientOcclusion = TextureMap[material.AmbientOcclusionMap].Sample(TextureMapSampler[material.AmbientOcclusionMap], UV).r;
    }
    if (material.EmissionMap != -1)
    {
        material.Emission = TextureMap[material.EmissionMap].Sample(TextureMapSampler[material.EmissionMap], UV).rgb;
    }
    if (material.TransmissionMap != -1)
    {
        material.Transmission = TextureMap[material.TransmissionMap].Sample(TextureMapSampler[material.TransmissionMap], UV).r;
    }
    if (material.IndexOfRefractionMap != -1)
    {
        material.IndexofRefraction = TextureMap[material.IndexOfRefractionMap].Sample(TextureMapSampler[material.IndexOfRefractionMap], UV).r;
    }
    if (TextureMap[material.AlbedoMap].Sample(TextureMapSampler[material.AlbedoMap], UV).a == 0.0f)
    {
        discard;
    }
    
    return material;
}

[shader("closesthit")]
void main(inout RayPayload rayPayload, in Attributes attribs)
{
    Vertex vertex = BuildVertexInfo(attribs);
    //const uint materialID = MeshPropertiesBuffer[InstanceIndex()];
    //MaterialProperties material = MaterialPropertiesBuffer[materialID];
    //PBRMaterial pbrMaterial = BuildPBRMaterial(material, vertex.UV);

    MaterialProperties material = MaterialPropertiesBuffer[sceneDataProperties.MaterialIndex];
    material.Albedo = TextureMap[material.AlbedoMap].SampleLevel(TextureMapSampler[material.AlbedoMap], vertex.UV, 0.0f).rgb;
    
    const float3 barycentricCoords = float3(1.0f - attribs.bary.x - attribs.bary.y, attribs.bary.x, attribs.bary.y);
    rayPayload.color = barycentricCoords;
}
