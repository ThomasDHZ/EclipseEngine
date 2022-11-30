#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable
#extension GL_EXT_multiview : enable

#include "PBRBindingLayout.glsl"

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

layout(binding = 11) uniform CubeMapViewSampler 
{
    mat4 CubeMapFaceMatrix[6];
} cubeMapViewSampler;


layout(push_constant) uniform SceneData
{
    uint MeshIndex;
    mat4 proj;
    mat4 view;
    vec3 CameraPos;
    vec3 MeshColorID;
    float Timer;
} sceneData;

void main() {

// if(gl_VertexIndex == 0)
//	{
//		debugPrintfEXT(": %i \n", sceneData.MeshIndex);
//	}
    FragPos = vec3(meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform * vec4(inPosition.xyz, 1.0));    
    Color = aColor;
    UV = aUV;
    Normal = mat3(meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform) * aNormal;
	Tangent = aTangent;
	BiTangent = aBitangent;
    gl_Position = cubeMapViewSampler.CubeMapFaceMatrix[gl_ViewIndex] *              
                  IntanceMatrix * 
                  vec4(inPosition, 1.0);
    MaterialID = aMaterialID;
}