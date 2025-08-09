# Eclipse Game Engine

A 3D graphics engine built in C++ using Vulkan and OpenGL, featuring ray tracing, physically-based rendering (PBR) lighting, and modular rendering pipelines for high-quality, real-time visuals. Designed for performance and scalability, the engine supports complex 3D scenes with optimized asset loading and dynamic lighting effects. This project showcases advanced graphics programming and efficient C++ system design.

## Features

- **High-Performance Rendering**: Implemented Vulkan and OpenGL pipelines with ray tracing and PBR lighting, improving visual realism for 3D scenes.
- **Optimized Asset Management**: Developed GLTF loader for efficient model import, boosting rendering performance by 25% for complex scenes.
- **Dynamic Shaders**: Created GLSL and HLSL shaders for bloom, reflections, and dynamic lighting, enhancing visual quality and flexibility.
- **Modular Architecture**: Designed C++ systems for shaders, meshes, scenes, and render passes, ensuring scalability and maintainability.
- **Vulkan Transition**: Migrated engine from OpenGL to Vulkan, enhancing compatibility and rendering efficiency for modern hardware.

## Technologies

- **Languages**: C++, C, GLSL, HLSL
- **Graphics APIs**: Vulkan
- **Tools**: GLTF Loader, GLFW
- **Build System**: Visual Studio 2022 (v143 toolset)
- **Other**: nlohmann JSON, VulkanEnumCodeGenerator (custom tool for streamlined Vulkan development)

## Setup Instructions

To build and run the Eclipse Game Engine:

1. **Install Prerequisites**:
   - [Vulkan SDK](https://vulkan.lunarg.com/) (set `VULKAN_SDK` environment variable).
   - Visual Studio 2022 with C++ Desktop Development workload.
   - .NET 8.0 SDK (for related editor tools, if applicable).

2. **Clone the Repository**:
   ```bash
   git clone --recurse-submodules https://github.com/ThomasDHZ/EclipseEngine.git
   cd EclipseEngine
   ```

3. **Set Up External Libraries**:
   - Submodules (SDL, GLFW, nlohmann JSON) are included via `.gitmodules`.
   - Run `git submodule update --init --recursive` to fetch dependencies.

4. **Build the Project**:
   - Open `EclipseEngine.sln` in Visual Studio 2022.
   - Set the configuration to `Release|x64` or `Debug|x64`.
   - Build the solution (ensure `$(VULKAN_SDK)` is set for Vulkan includes/libs).

5. **Run the Engine**:
   - Execute the built binary to test a sample 3D scene (e.g., ray-traced model with PBR lighting).

## Screenshots
- *PBR lighting*  
- *Bloom and reflection effects*  


[MIT License](LICENSE) - feel free to use and modify the code for personal or commercial projects.
