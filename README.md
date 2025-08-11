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
- *Scene Rendering*
<img width="642" height="384" alt="Eclipse2" src="https://github.com/user-attachments/assets/dfe383a4-7e48-40a8-940e-a27a3aafb4aa" />
<img width="640" height="384" alt="Eclipse" src="https://github.com/user-attachments/assets/aaca38c7-1bae-4605-9131-62800e013615" />

- *Instancing Test*
 ![490296796_10222917695685520_3843289632798826724_n](https://github.com/user-attachments/assets/7e66d48a-5afc-4a08-a69d-c254daab50a4)
![489419744_10222917698965602_7496271288393276553_n](https://github.com/user-attachments/assets/8071d670-6ba2-4e37-befb-7812c72170b3)

- *PBR lighting and reflection test*  
![488926980_10222909203153212_5999367957420881329_n](https://github.com/user-attachments/assets/28145e1c-6b50-450d-887d-2315f2d01eac)

- Line Rendering*
![488745782_10222868154567023_1954215942717174832_n](https://github.com/user-attachments/assets/8ad545ac-8f42-4664-8618-ef73fd52473d)
![488005838_10222868140286666_7386628907948823544_n](https://github.com/user-attachments/assets/54e1d1e7-e67e-483b-ac3c-52fc7257d345)

