#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aColor;

layout(location = 0) out vec3 FragPos;
layout(location = 1) out vec2 UV;
layout(location = 2) out vec3 Color;

#include "VertexLayout.glsl"
#include "MeshProperties.glsl"
#include "MaterialProperties.glsl"
#include "Lights.glsl"
#include "SceneData.glsl"

layout(binding = 0) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
layout(binding = 1) buffer TransformBuffer { mat4 transform; } transformBuffer[];

void main() {

// if(gl_VertexIndex == 0)
//	{
//		debugPrintfEXT(": %i \n", sceneData.MeshIndex);
//	}
    mat4 MeshTransform = transformBuffer[sceneData.MeshIndex].transform;
    FragPos = vec3(MeshTransform * vec4(inPosition.xyz, 1.0));    
    Color = aColor;
    UV = aUV;
    gl_Position = sceneData.proj * 
                  sceneData.view *                
                  MeshTransform * 
                  vec4(inPosition, 1.0);
}