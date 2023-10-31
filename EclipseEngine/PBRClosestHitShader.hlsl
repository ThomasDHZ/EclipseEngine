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

struct VertexBuffer
{
    ConstantBuffer<Vertex> vertex[];
};

struct IndiceBuffer
{
    float Indice;
};

struct IndicesBuffer
{
    ConstantBuffer<IndiceBuffer> IndiceBuffer[];
};

[[vk::push_constant]] SceneProperties sceneDataProperties;
[[vk::binding(0)]]  RaytracingAccelerationStructure topLevelAS;
[[vk::binding(1)]]  RWTexture2D<float4> image;
[[vk::binding(2)]]  ConstantBuffer<Vertex> Vertices[][];
[[vk::binding(3)]]  ConstantBuffer<float> Indices[][];
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
    float a = Indices[InstanceIndex()][3 * PrimitiveIndex()];
    float b = Indices[InstanceIndex()][3 * PrimitiveIndex() + 1];
    float c = Indices[InstanceIndex()][3 * PrimitiveIndex() + 2];
    const int3 index = float3(a, b, c);

    const Vertex v0 = Vertices[InstanceIndex()][index.x];
    const Vertex v1 = Vertices[InstanceIndex()][index.y];
    const Vertex v2 = Vertices[InstanceIndex()][index.z];

    const float3 barycentricCoords = float3(1.0f - attribs.bary.x - attribs.bary.y, attribs.bary.x, attribs.bary.y);

    vertex.Position = v0.Position * barycentricCoords.x + v1.Position * barycentricCoords.y + v2.Position * barycentricCoords.z;
    vertex.Position = mul(MeshPropertiesBuffer[InstanceIndex()].MeshTransform, float4(vertex.Position, 1.0));

    vertex.Normal = v0.Normal * barycentricCoords.x + v1.Normal * barycentricCoords.y + v2.Normal * barycentricCoords.z;

    vertex.UV = v0.UV * barycentricCoords.x + v1.UV * barycentricCoords.y + v2.UV * barycentricCoords.z;
    vertex.UV += MeshPropertiesBuffer[InstanceIndex()].UVOffset;

    vertex.Tangant = v0.Tangant * barycentricCoords.x + v1.Tangant * barycentricCoords.y + v2.Tangant * barycentricCoords.z;
    vertex.BiTangant = v0.BiTangant * barycentricCoords.x + v1.BiTangant * barycentricCoords.y + v2.BiTangant * barycentricCoords.z;

    return vertex;
}

//PBRMaterial BuildPBRMaterial(MaterialProperties properties, vec2 UV)
//{
//    PBRMaterial material;

//    material.Albedo = pow(properties.Albedo, vec3(2.2f));
//    if (properties.AlbedoMap != 0)
//    {
//        material.Albedo = pow(texture(TextureMap[properties.AlbedoMap], UV).rgb, vec3(2.2f));
//    }

//    material.Metallic = properties.Metallic;
//    if (properties.MetallicMap != 0)
//    {
//        material.Metallic = texture(TextureMap[properties.MetallicMap], UV).r;
//    }

//    material.Roughness = properties.Roughness;
//    if (properties.RoughnessMap != 0)
//    {
//        material.Roughness = texture(TextureMap[properties.RoughnessMap], UV).r;
//    }

//    material.AmbientOcclusion = properties.AmbientOcclusion;
//    if (properties.AmbientOcclusionMap != 0)
//    {
//        material.AmbientOcclusion = texture(TextureMap[properties.AmbientOcclusionMap], UV).r;
//    }

//    material.Emission = properties.Emission;
//    if (properties.EmissionMap != 0)
//    {
//        material.Emission = texture(TextureMap[properties.EmissionMap], UV).rgb;
//    }

//    if (texture(TextureMap[properties.AlphaMap], UV).r == 0.0f ||
//		texture(TextureMap[properties.AlbedoMap], UV).a == 0.0f)
//    {
//		//discard;
//    }

//    material.NormalMapID = properties.NormalMap;
//    material.DepthMapID = properties.DepthMap;

//    return material;
//}

[shader("closesthit")]
void main(inout RayPayload rayPayload, in Attributes attribs)
{
    Vertex vertex = BuildVertexInfo(attribs);
    //const uint materialID = MeshPropertiesBuffer[InstanceIndex()];
    //MaterialProperties material = MaterialPropertiesBuffer[materialID];
    //PBRMaterial pbrMaterial = BuildPBRMaterial(material, vertex.UV);

    const float3 barycentricCoords = float3(1.0f - attribs.bary.x - attribs.bary.y, attribs.bary.x, attribs.bary.y);

    RayPayload rayHitInfo;
    rayHitInfo.color = barycentricCoords;
}
