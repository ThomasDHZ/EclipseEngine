#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable
#extension GL_EXT_multiview : enable

#include "MeshProperties.glsl"

layout(push_constant) uniform MeshInfo
{
	uint MeshIndex;
    mat4 proj;
    mat4 view;
    vec3 CameraPos;
} sceneData;


layout(binding = 0) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in vec3 aColor;

layout(location = 0) out vec3 FragPos;
layout(location = 1) out vec2 TexCoords;
layout(location = 2) out vec3 Normal;
layout(location = 3) out vec3 Tangent;
layout(location = 4) out vec3 BiTangent;
layout(location = 5) out vec3 Color;

void main() 
{
    FragPos = vec3(meshBuffer[sceneData.MeshIndex].meshProperties.GameObjectTransform * meshBuffer[sceneData.MeshIndex].meshProperties.ModelTransform * meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform * vec4(inPosition.xyz, 1.0));    
    Color = aColor;
    TexCoords = aTexCoords;
    Normal = mat3(meshBuffer[sceneData.MeshIndex].meshProperties.GameObjectTransform * meshBuffer[sceneData.MeshIndex].meshProperties.ModelTransform * meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform) * aNormal;
	Tangent = aTangent;
	BiTangent = aBitangent;
    gl_Position = sceneData.proj * 
                  sceneData.view *                
                  meshBuffer[sceneData.MeshIndex].meshProperties.GameObjectTransform * 
                  meshBuffer[sceneData.MeshIndex].meshProperties.ModelTransform * 
                  meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform * 
                  vec4(inPosition, 1.0);
}