C:/VulkanSDK/1.3.204.0/Bin/glslc.exe FrameBuffer.vert -o FrameBufferVert.spv
C:/VulkanSDK/1.3.204.0/Bin/glslc.exe FrameBuffer.frag -o FrameBufferFrag.spv
C:/VulkanSDK/1.3.204.0/Bin/glslc.exe Renderer2D.vert -o Renderer2DVert.spv
C:/VulkanSDK/1.3.204.0/Bin/glslc.exe Renderer2D.frag -o Renderer2DFrag.spv
C:/VulkanSDK/1.3.204.0/Bin/glslc.exe Renderer3D.vert -o Renderer3DVert.spv
C:/VulkanSDK/1.3.204.0/Bin/glslc.exe Renderer3D.frag -o Renderer3DFrag.spv
C:/VulkanSDK/1.3.204.0/Bin/glslc.exe --target-env=vulkan1.3 --target-spv=spv1.4 closesthit.rchit -o closesthit.rchit.spv
C:/VulkanSDK/1.3.204.0/Bin/glslc.exe --target-env=vulkan1.3 --target-spv=spv1.4 anyhit.rahit -o anyhit.rahit.spv
C:/VulkanSDK/1.3.204.0/Bin/glslc.exe --target-env=vulkan1.3 --target-spv=spv1.4 anyhit1.rahit -o anyhit1.rahit.spv
C:/VulkanSDK/1.3.204.0/Bin/glslc.exe --target-env=vulkan1.3 --target-spv=spv1.4 raygen.rgen -o raygen.rgen.spv
C:/VulkanSDK/1.3.204.0/Bin/glslc.exe --target-env=vulkan1.3 --target-spv=spv1.4 miss.rmiss -o miss.rmiss.spv
C:/VulkanSDK/1.3.204.0/Bin/glslc.exe --target-env=vulkan1.3 --target-spv=spv1.4 shadow.rmiss -o shadow.rmiss.spv
pause