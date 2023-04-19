//#include "TextureBaker.h"
//#include <STBIImport.cpp>
//
//void TextureBaker::BakeCubeTextureAtlus(const std::string& FilePath, std::vector<std::shared_ptr<RenderedCubeMapTexture>> ReflectionCubeMapList, uint32_t cubeMapSizePerCell, uint32_t bakedTextureAtlusSize)
//{
//    std::vector<std::shared_ptr<BakedTexture>> BakeTextureList;
//    for (int TextureLayer = 0; TextureLayer < 6; TextureLayer++)
//    {
//        BakeTextureList.emplace_back(std::make_shared<BakedTexture>(BakedTexture(Pixel(255, 0, 0, 255), glm::vec2(bakedTextureAtlusSize))));
//    }
//
//    VkCommandBuffer commandBuffer = VulkanRenderer::BeginSingleTimeCommands();
//    for (auto& reflectionCubeMap : ReflectionCubeMapList)
//    {
//        reflectionCubeMap->UpdateCubeMapLayout(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
//    }
//    for (int layer = 0; layer < 6; layer++)
//    {
//        BakeTextureList[layer]->UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL);
//        BakeTextureList[layer]->UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
//    }
//    for (int layer = 0; layer < 6; layer++)
//    {
//        int x = 0;
//        int y = 0;
//        for (const auto& reflectionCubeMap : ReflectionCubeMapList)
//        {
//            if (x == bakedTextureAtlusSize / cubeMapSizePerCell)
//            {
//                x = 0;
//                y++;
//            }
//
//            VkImageCopy copyImage{};
//            copyImage.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//            copyImage.srcSubresource.layerCount = 1;
//            copyImage.srcSubresource.baseArrayLayer = layer;
//
//            copyImage.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//            copyImage.dstSubresource.layerCount = 1;
//
//            copyImage.dstOffset.x = x * cubeMapSizePerCell;
//            copyImage.dstOffset.y = y * cubeMapSizePerCell;
//            copyImage.dstOffset.z = 0;
//
//            copyImage.extent.width = reflectionCubeMap->GetWidth();
//            copyImage.extent.height = reflectionCubeMap->GetHeight();
//            copyImage.extent.depth = 1;
//
//            vkCmdCopyImage(commandBuffer, reflectionCubeMap->Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, BakeTextureList[layer]->Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyImage);
//            x++;
//        }
//    }
//    for (int layer = 0; layer < 6; layer++)
//    {
//        BakeTextureList[layer]->UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL);
//    }
//    for (const auto& reflectionCubeMap : ReflectionCubeMapList)
//    {
//        reflectionCubeMap->UpdateImageLayout(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
//    }
//    VulkanRenderer::EndSingleTimeCommands(commandBuffer);
//
//    for (int TextureLayer = 0; TextureLayer < 6; TextureLayer++)
//    {
//        VkImageSubresource subResource{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 0 };
//        VkSubresourceLayout subResourceLayout;
//        vkGetImageSubresourceLayout(VulkanRenderer::GetDevice(), BakeTextureList[TextureLayer]->Image, &subResource, &subResourceLayout);
//
//        const char* data;
//        vkMapMemory(VulkanRenderer::GetDevice(), BakeTextureList[TextureLayer]->Memory, 0, VK_WHOLE_SIZE, 0, (void**)&data);
//
//        std::string textureloc = FilePath;
//        textureloc.append(std::to_string(TextureLayer));
//        textureloc.append(".bmp");
//        stbi_write_bmp(textureloc.c_str(), BakeTextureList[TextureLayer]->GetWidth(), BakeTextureList[TextureLayer]->GetHeight(), STBI_rgb_alpha, data);
//
//        BakeTextureList[TextureLayer]->Destroy();
//    }
//}
