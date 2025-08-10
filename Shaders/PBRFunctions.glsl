const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0)) + 1.0;
    denom = (3.1415927410125732421875 * denom) * denom;
    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    float nom = NdotV;
    float denom = (NdotV * (1.0 - k)) + k;
    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float param = NdotV;
    float param_1 = roughness;
    float ggx2 = GeometrySchlickGGX(param, param_1);
    float param_2 = NdotL;
    float param_3 = roughness;
    float ggx1 = GeometrySchlickGGX(param_2, param_3);
    return ggx1 * ggx2;
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + ((max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0));
}


mat3 getTBNFromMap(Vertex vertex)
{
    vec3 T = normalize(mat3(meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform[0].xyz, 
                            meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform[1].xyz, 
                            meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform[2].xyz) * 
                            vec3(vertex.Tangant));
    vec3 B = normalize(mat3(meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform[0].xyz, 
                            meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform[1].xyz, 
                            meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform[2].xyz) * 
                            vec3(vertex.BiTangant));
    vec3 N = normalize(mat3(meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform[0].xyz, 
                            meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform[1].xyz, 
                            meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform[2].xyz) * 
                            vertex.Normal);
    return mat3(vec3(T), vec3(B), vec3(N));
}