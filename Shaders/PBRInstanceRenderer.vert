#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in vec3 aColor;
layout (location = 6) in mat4 IntanceMatrix;
layout (location = 10) in int aMaterialID;

layout(location = 0) out vec3 FragPos;
layout(location = 1) out vec2 UV;
layout(location = 2) out vec3 Normal;
layout(location = 3) out vec3 Tangent;
layout(location = 4) out vec3 BiTangent;
layout(location = 5) out vec3 Color;
layout(location = 6) out int MaterialID;

#include "vertex.glsl"
#include "MeshProperties.glsl"
#include "MaterialProperties.glsl"
#include "LightProperties.glsl"
#include "ConstSceneData.glsl"

layout(binding = 1) buffer TransformBuffer { mat4 transform; } transformBuffer[];

void main() {

    FragPos = vec3(IntanceMatrix * vec4(inPosition.xyz, 1.0));    
    Color = aColor;
    UV = aUV;
    Normal = mat3(IntanceMatrix) * aNormal;
	Tangent = aTangent;
	BiTangent = aBitangent;
    gl_Position = sceneData.proj * 
                  sceneData.view *                
                  IntanceMatrix * 
                  vec4(inPosition, 1.0);
   MaterialID = aMaterialID;
}