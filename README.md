# Eclipse Engine

A 3D graphics engine written in **C++** using **Vulkan** (with earlier OpenGL support).

Eclipse Engine focuses on performance, modular architecture, and modern rendering techniques including Physically Based Rendering (PBR), dynamic lighting, and efficient asset loading.

## Key Features

- **Vulkan Rendering** with support for modern graphics pipelines
- **Physically Based Rendering (PBR)** materials and lighting
- **Modular Architecture** — render passes, pipelines, materials, and scenes are cleanly separated
- **GLTF Loading** for efficient model and scene import
- **Performance Focus** — designed with efficiency and scalability in mind
- Debug views including wireframe mode (demonstrated at 700–860+ FPS in test scenes)

## Screenshots

### Cathedral Scene
<img width="400" alt="Cathedral Interior" src="https://github.com/user-attachments/assets/dfe383a4-7e48-40a8-940e-a27a3aafb4aa" />
<img width="400" alt="Cathedral View" src="https://github.com/user-attachments/assets/aaca38c7-1bae-4605-9131-62800e013615" />

### Materials, Lighting, Instancing
<img width="400" alt="Instancing Farview" src="https://github.com/user-attachments/assets/7e66d48a-5afc-4a08-a69d-c254daab50a4" />
<img width="400" alt="Instancing Nearview" src="https://github.com/user-attachments/assets/8071d670-6ba2-4e37-befb-7812c72170b3" />

### PBR lighting and reflection test
<img width="400" alt="Sphere Reflection" src="https://github.com/user-attachments/assets/28145e1c-6b50-450d-887d-2315f2d01eac" />

### Line Rendering
<img width="400" alt="Line Rendering" src="https://github.com/user-attachments/assets/8ad545ac-8f42-4664-8618-ef73fd52473d" />
<img width="400" alt="Line Rendering" src="https://github.com/user-attachments/assets/54e1d1e7-e67e-483b-ac3c-52fc7257d345" />

## Tech Stack

- **Language**: C++
- **Graphics API**: Vulkan (earlier OpenGL support)
- **Shaders**: GLSL / HLSL
- **Asset Loading**: GLTF
- **Other**: GLFW, nlohmann/json, custom tools

## Build Instructions

## Prerequisites
- Vulkan SDK
- Visual Studio 2022 (C++ Desktop workload)

## Build
1. Clone the repository with submodules:
   ```bash
   git clone --recurse-submodules https://github.com/ThomasDHZ/EclipseEngine.git
   ```
2. Open EclipseEngine.sln in Visual Studio 2022
```
Build in Release | x64
```

## Purpose
Eclipse Engine was developed as a deep dive into modern graphics programming, performance optimization, and clean C++ systems design. It later informed the architecture of the hybrid .NET + native engine projects.

## Related Projects
Vulkan Game Engine
VulkanEngineCore
