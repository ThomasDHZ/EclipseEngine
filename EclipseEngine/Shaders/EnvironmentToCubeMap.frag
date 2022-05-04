#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable
#extension GL_EXT_ray_tracing : enable
#extension GL_EXT_ray_query : enable

#include "VertexLayout.glsl"
#include "MeshProperties.glsl"
#include "Lights.glsl"

layout(location = 0) in vec2 TexCoords;
layout(location = 0) out vec4 outColor;

layout(binding = 0) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
layout(binding = 1) buffer DirectionalLightBuffer { DirectionalLight directionalLight; } DLight[];
layout(binding = 2) buffer PointLightBuffer { PointLight pointLight; } PLight[];
layout(binding = 3) buffer SpotLightBuffer { SpotLight spotLight; } SLight[];
layout(binding = 4) uniform sampler2D TextureMap[];
layout(binding = 5) uniform sampler2D PositionTexture;
layout(binding = 6) uniform sampler2D TangantTexture;
layout(binding = 7) uniform sampler2D BiTangantTexture;
layout(binding = 8) uniform sampler2D TBNormalTexture;
layout(binding = 9) uniform sampler2D NormalTexture;
layout(binding = 10) uniform sampler2D AlbedoTexture;
layout(binding = 11) uniform sampler2D SpecularTexture;
layout(binding = 12) uniform sampler2D BloomTexture;
layout(binding = 13) uniform sampler2D ShadowTexture;

layout(push_constant) uniform SceneData
{
    uint MeshIndex;
    mat4 proj;
    mat4 view;
    vec3 CameraPos;
    vec3 MeshColorID;
    vec3 AmbientLight;
    uint DirectionalLightCount;
    uint PointLightCount;
    uint SpotLightCount;
    float Timer;
} sceneData;

vec3 CalcNormalDirLight(vec3 normal, int index);
//vec3 CalcNormalPointLight(vec3 normal, int index);
//vec3 CalcNormalSpotLight(vec3 normal, int index);

mat3 TBN;

void main() 
{
    const vec3 Position = texture(PositionTexture, TexCoords).rgb;
     vec3 Normal = texture(NormalTexture, TexCoords).rgb;
    const vec3 Diffuse = texture(AlbedoTexture, TexCoords).rgb;
    const float Specular = texture(SpecularTexture, TexCoords).r;
    const float Shadow = texture(ShadowTexture, TexCoords).r;

   const vec3 T = texture(TangantTexture, TexCoords).rgb;
   const vec3 B = texture(BiTangantTexture, TexCoords).rgb;
   const vec3 N = texture(TBNormalTexture, TexCoords).rgb;
   TBN = transpose(mat3(T, B, N));

     vec3 ViewPos  = sceneData.CameraPos;
    vec3 FragPos  = Position;
 
        ViewPos  = TBN * sceneData.CameraPos;
        FragPos  = TBN * Position;
    

    const vec3 viewDir = normalize(ViewPos - FragPos);

    vec3 result = vec3(0.0f);

    vec3 LightPos = DLight[0].directionalLight.direction;
         LightPos = TBN * DLight[0].directionalLight.direction;
    vec3 lightDir = normalize(-LightPos);
    float diff = max(dot(Normal.rgb, lightDir), 0.0);
    vec3 ViewDir = normalize(ViewPos - FragPos);
       const vec3 halfwayDir = normalize(lightDir + ViewDir);
    const float spec = pow(max(dot(Normal, halfwayDir), 0.0), 32.0f);

    vec3 ambient = sceneData.AmbientLight * Diffuse.rgb;
    vec3 LDiffuse = DLight[0].directionalLight.diffuse * diff * Diffuse.rgb;
    vec3 specular = DLight[0].directionalLight.specular * spec * Specular;
    
    result = ambient;
    if(Shadow == 1.0f)
    {
       result += LDiffuse + specular;
    }
    outColor = vec4(result, 1.0f);
}

vec3 CalcNormalDirLight(vec3 normal, int index)
{
    vec3 FragPos = texture(PositionTexture, TexCoords).rgb;
    vec3 Normal = texture(NormalTexture, TexCoords).rgb;
    vec4 Diffuse = texture(AlbedoTexture, TexCoords);
    float Specular = texture(SpecularTexture, TexCoords).a;



    vec3 ViewPos = sceneData.CameraPos;
    vec3 FragPos2 = FragPos;
 
//        LightPos = TBN * DLight[index].directionalLight.direction;
//        ViewPos = TBN * sceneData.CameraPos;
//        FragPos2 = TBN * FragPos;
    
    vec3 ViewDir = normalize(ViewPos - FragPos2);

    vec3 LightPos = DLight[index].directionalLight.direction;
    const vec3 lightDir = normalize(-LightPos);
    const float diff = max(dot(normal, lightDir), 0.0);

return LightPos;
}

//vec3 CalcNormalPointLight(vec3 normal, int index)
//{
//    vec3 FragPos = texture(PositionTexture, TexCoords).rgb;
//    vec3 LightPos = PLight[index].pointLight.position;
//    vec3 ViewPos = sceneData.CameraPos;
//    vec3 FragPos2 = FragPos;
//
//    vec3 ViewDir = normalize(ViewPos - FragPos2);
//
//    vec3 lightDir = normalize(LightPos - FragPos2);
//    float diff = max(dot(normal, lightDir), 0.0);
//
//    vec3 halfwayDir = normalize(lightDir + ViewDir);
//    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.Shininess);
//
//    vec3 ambient = sceneData.AmbientLight * material.Diffuse.rgb;
//    vec3 diffuse = PLight[index].pointLight.diffuse * diff * material.Diffuse.rgb;
//    vec3 specular = PLight[index].pointLight.specular * spec * material.Specular;
//    if (material.DiffuseMapID != 0)
//    {
//        ambient = sceneData.AmbientLight * vec3(texture(TextureMap[material.DiffuseMapID], uv));
//        diffuse = PLight[index].pointLight.diffuse * diff * vec3(texture(TextureMap[material.DiffuseMapID], uv));
//    }
//    if (material.SpecularMapID != 0)
//    {
//        specular = PLight[index].pointLight.specular * spec * vec3(texture(TextureMap[material.SpecularMapID], uv));
//    }
//
//    float LightDistance = length(LightPos - FragPos2);
//    float attenuation = 1.0 / (1.0f + PLight[index].pointLight.linear * LightDistance + PLight[index].pointLight.quadratic * (LightDistance * LightDistance));
//
//    return (ambient + diffuse + specular) * attenuation;
//}
//
//vec3 CalcNormalSpotLight(vec3 normal, int index)
//{
//    vec3 FragPos = texture(PositionTexture, TexCoords).rgb;
//    vec3 LightPos = SLight[index].spotLight.position;
//    vec3 ViewPos = sceneData.CameraPos;
//    vec3 FragPos2 = FragPos;
//    if (material.NormalMapID != 0)
//    {
//        LightPos = normal * SLight[index].spotLight.direction;
//        ViewPos = normal * sceneData.CameraPos;
//    }
//    vec3 ViewDir = normalize(ViewPos - FragPos2);
//
//    vec3 lightDir = normalize(LightPos - FragPos2);
//    float diff = max(dot(normal, lightDir), 0.0);
//
//    vec3 halfwayDir = normalize(lightDir + ViewDir);
//    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.Shininess);
//
//    vec3 ambient = sceneData.AmbientLight * material.Diffuse.rgb;
//    vec3 diffuse = SLight[index].spotLight.diffuse * diff * material.Diffuse.rgb;
//    vec3 specular = SLight[index].spotLight.specular * spec * material.Specular;
//    if (material.DiffuseMapID != 0)
//    {
//        ambient = sceneData.AmbientLight * vec3(texture(TextureMap[material.DiffuseMapID], uv));
//        diffuse = SLight[index].spotLight.diffuse * diff * vec3(texture(TextureMap[material.DiffuseMapID], uv));
//    }
//    if (material.SpecularMapID != 0)
//    {
//        specular = SLight[index].spotLight.specular * spec * vec3(texture(TextureMap[material.SpecularMapID], uv));
//    }
//
//    float theta = dot(lightDir, normalize(-SLight[index].spotLight.direction)); 
//    float epsilon = SLight[index].spotLight.cutOff - SLight[index].spotLight.outerCutOff;
//    float intensity = clamp((theta - SLight[index].spotLight.outerCutOff) / epsilon, 0.0, 1.0);
//
//    float LightDistance = length(LightPos - FragPos2);
//    float attenuation = 1.0 / (1.0f + SLight[index].spotLight.linear * LightDistance + SLight[index].spotLight.quadratic * (LightDistance * LightDistance));
//
//    return (ambient + diffuse + specular) * attenuation;
//}
//
//