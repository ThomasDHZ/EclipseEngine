#version 460
//#extension GL_ARB_separate_shader_objects : enable
//#extension GL_EXT_nonuniform_qualifier : enable
//
//#include "VertexLayout.glsl"
//#include "MeshProperties.glsl"
//#include "MaterialProperties.glsl"
//#include "Lights.glsl"
//
//layout(location = 0) in vec3 FragPos;
//layout(location = 1) in vec2 UV;
//layout(location = 2) in flat int MaterialID;
//
//layout(binding = 0) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
//layout(binding = 2) buffer MaterialPropertiesBuffer { MaterialProperties materialProperties; } materialBuffer[];
//layout(binding = 3) uniform sampler2D TextureMap[];
//
//layout(push_constant) uniform DepthSceneData
//{
//    uint MeshIndex;
//    uint LightIndex;
//} sceneData;
//
//
//void main()
//{		
//   MaterialProperties material = materialBuffer[MaterialID].materialProperties;
//   vec2 FinalUV = UV + meshBuffer[sceneData.MeshIndex].meshProperties.UVOffset;
//        FinalUV *= meshBuffer[sceneData.MeshIndex].meshProperties.UVScale;
//
//   if(texture(TextureMap[material.AlphaMapID], FinalUV).r == 0.0f ||
//      texture(TextureMap[material.AlbedoMapID], FinalUV).a == 0.0f)
//   {
//	 discard;
//   }
//   if(meshBuffer[sceneData.MeshIndex].meshProperties.UVFlip.y == 1.0f)
//   {
//        FinalUV.y = 1.0f - FinalUV.y;
//   }
//   if(meshBuffer[sceneData.MeshIndex].meshProperties.UVFlip.x == 1.0f)
//   {
//        FinalUV.x = 1.0f - FinalUV.x;
//   }
//}
//