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

layout(binding = 0) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
layout(binding = 1) buffer MaterialPropertiesBuffer { MaterialProperties materialProperties; } materialBuffer[];
layout(binding = 2) uniform sampler2D TextureMap[];

#include "PBRMaterial.glsl"

#include "RasterVertexBuilder.glsl"

void main()
{ 
   Vertex vertex = RasterVertexBuilder();
   const uint materialID = meshBuffer[sceneData.MeshIndex].meshProperties.MaterialBufferIndex;
   const MaterialProperties material = materialBuffer[materialID].materialProperties;

   PBRMaterial pbrMaterial = BuildPBRMaterial(materialBuffer[materialID].materialProperties, vertex.UV);

   outColor = vec4(texture(TextureMap[material.NormalMapID], vertex.UV).rgb, 1.0f);
}