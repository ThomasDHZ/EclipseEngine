#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable

layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec2 UV;
layout(location = 2) in vec3 Normal;
layout(location = 3) in vec3 Tangent;
layout(location = 4) in vec3 BiTangent;
layout(location = 5) in vec3 Color;

layout(location = 0) out vec4 outColor;

#include "VertexLayout.glsl"
#include "MeshProperties.glsl"
#include "MaterialProperties.glsl"
#include "SceneData.glsl"
#include "Lights.glsl"

layout(binding = 0) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
layout(binding = 1) buffer TransformBuffer { mat4 transform; } transformBuffer[];
layout(binding = 2) buffer MaterialPropertiesBuffer { MaterialProperties materialProperties; } materialBuffer[];
layout(binding = 3) uniform sampler2D TextureMap[];
layout(binding = 4) buffer DirectionalLightBuffer { DirectionalLight directionalLight; } DLight[];
layout(binding = 5) buffer PointLightBuffer { PointLight pointLight; } PLight[];
layout(binding = 6) buffer SpotLightBuffer { SpotLight spotLight; } SLight[];

#include "PBRMaterial.glsl"
#include "RasterVertexBuilder.glsl"

void main()
{ 
   Vertex vertex = RasterVertexBuilder();
   const uint materialID = meshBuffer[sceneData.MeshIndex].meshProperties.MaterialBufferIndex;
   const MaterialProperties material = materialBuffer[materialID].materialProperties;

   PBRMaterial pbrMaterial = BuildPBRMaterial(materialBuffer[materialID].materialProperties, vertex.UV);

    vec3 N = normalize(Normal);
	vec3 T = normalize(Tangent.xyz);
	vec3 B = cross(Normal, Tangent.xyz);
	mat3 TBN = mat3(T, B, N);
	N = TBN * normalize(texture(TextureMap[material.NormalMapID], vertex.UV).xyz * 2.0 - vec3(1.0));

	const float ambient = 0.1;
	vec3 L = normalize(DLight[0].directionalLight.direction);
	vec3 V = normalize(sceneData.CameraPos - vertex.Position);
	vec3 R = reflect(-L, N);
	vec3 diffuse = max(dot(N, L), ambient).rrr;
	float specular = pow(max(dot(R, V), 0.0), 32.0f);
	outColor = vec4(diffuse * texture(TextureMap[material.AlbedoMapID], vertex.UV).rgb + specular, 1.0f);
}