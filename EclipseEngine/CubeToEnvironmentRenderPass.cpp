#include "CubeToEnvironmentRenderPass.h"

CubeToEnvironmentRenderPass::CubeToEnvironmentRenderPass() : GenerateImageRenderPass()
{
}

CubeToEnvironmentRenderPass::~CubeToEnvironmentRenderPass()
{
}

void CubeToEnvironmentRenderPass::BuildRenderPass(std::shared_ptr<RenderedCubeMapTexture> cubeMap, glm::ivec2 textureSize)
{
    GenerateImageRenderPass::BuildRenderPass("/Pipelines/EnvironmentToCubeMapPipeline.txt", cubeMap, textureSize);
}

void CubeToEnvironmentRenderPass::OneTimeDraw(std::shared_ptr<RenderedCubeMapTexture> cubeMap, glm::ivec2 textureSize)
{
    GenerateImageRenderPass::OneTimeDraw("/Pipelines/EnvironmentToCubeMapPipeline.txt", cubeMap, textureSize);
}