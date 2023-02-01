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
#include "SceneData.glsl"
#include "Lights.glsl"

layout(binding = 0) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
layout(binding = 1) buffer TransformBuffer { mat4 transform; } transformBuffer[];
layout(binding = 2) uniform sampler2D AlbedoMap;
layout(binding = 3) uniform sampler2D NormalMap;
layout(binding = 4) uniform sampler2D MetallicRoughnessMap;
layout(binding = 5) uniform sampler2D AmbientOcclusionMap;
layout(binding = 6) uniform sampler2D AlphaMap;
layout(binding = 7) buffer DirectionalLightBuffer { DirectionalLight directionalLight; } DLight[];
layout(binding = 8) buffer PointLightBuffer { PointLight pointLight; } PLight[];
layout(binding = 9) buffer SpotLightBuffer { SpotLight spotLight; } SLight[];

#include "RasterVertexBuilder.glsl"

void main()
{ 
   Vertex vertex = RasterVertexBuilder();

    vec3 Albedo = texture(AlbedoMap, vertex.UV).rgb;
	vec3 Normal = texture(NormalMap, vertex.UV).rgb;
	float Metalic = texture(MetallicRoughnessMap, vertex.UV).r;
	float Roughness = texture(MetallicRoughnessMap, vertex.UV).g;
	float AmbientOcclusion = texture(AmbientOcclusionMap, vertex.UV).r;
	float Alpha = texture(AlphaMap, vertex.UV).r;

   	vec3 N = texture(NormalMap, vertex.UV).rgb;
	vec3 L = normalize(DLight[0].directionalLight.direction);
	vec3 V = normalize(sceneData.CameraPos - vertex.Position);
	vec3 R = reflect(-L, N);
	vec3 diffuse = max(dot(N, L), 0.15) * Albedo;
	vec3 specular = pow(max(dot(R, V), 0.0), 16.0) * vec3(0.75);

   outColor = vec4(diffuse * Albedo.rgb + specular, 1.0);	
}