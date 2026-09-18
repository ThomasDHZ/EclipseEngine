# Eclipse Engine

Earlier **C++ / Vulkan** renderer (2020–2024). PBR, modular render passes, GLTF, instancing.

This repo is the predecessor. The current system is a hybrid **.NET 8 + native C++** stack:

- [VulkanEngineCore](https://github.com/ThomasDHZ/VulkanEngineCore) — interop, pools, C# wrappers
- [VulkanGameEngine](https://github.com/ThomasDHZ/VulkanGameEngine) — runtime on that core
- [VulkanGameEngineLevelEditor](https://github.com/ThomasDHZ/VulkanGameEngineLevelEditor) — C# WinForms host

Eclipse informed the native side of that split. It is not the active editor/runtime.

## Screenshots

**Cathedral**

<img width="400" alt="Cathedral interior" src="https://github.com/user-attachments/assets/dfe383a4-7e48-40a8-940e-a27a3aafb4aa" />
<img width="400" alt="Cathedral view" src="https://github.com/user-attachments/assets/aaca38c7-1bae-4605-9131-62800e013615" />

**Materials, lighting, instancing**

<img width="400" alt="Instancing far" src="https://github.com/user-attachments/assets/7e66d48a-5afc-4a08-a69d-c254daab50a4" />
<img width="400" alt="Instancing near" src="https://github.com/user-attachments/assets/8071d670-6ba2-4e37-befb-7812c72170b3" />

**PBR reflection test**

<img width="400" alt="Sphere reflection" src="https://github.com/user-attachments/assets/28145e1c-6b50-450d-887d-2315f2d01eac" />

**Line rendering**

<img width="400" alt="Line rendering" src="https://github.com/user-attachments/assets/8ad545ac-8f42-4664-8618-ef73fd52473d" />
<img width="400" alt="Line rendering debug" src="https://github.com/user-attachments/assets/54e1d1e7-e67e-483b-ac3c-52fc7257d345" />

## What it includes

- Vulkan renderer (older OpenGL path exists in history)
- PBR materials and lighting
- Modular render passes, pipelines, materials, scenes
- GLTF load
- Instancing and line/debug views
- Wireframe / debug views (test scenes in the 700–860+ FPS range on the hardware used at the time)

## Stack

| | |
|---|---|
| Language | C++ |
| API | Vulkan (earlier OpenGL) |
| Shaders | GLSL / HLSL |
| Assets | GLTF |
| Other | GLFW, nlohmann/json |

## Build

**Prerequisites:** Vulkan SDK, Visual Studio 2022 (Desktop C++).

```bash
git clone --recurse-submodules https://github.com/ThomasDHZ/EclipseEngine.git
```

Open `EclipseEngine.sln` → **Release | x64**.

## Layout (high level)

```text
EclipseEngine/     # engine
Pipelines/
RenderPasses/
Shaders/
GLTFLoader/
Materials/
Scenes/
Levels/
VulkanEnumCodeGenerator/
```

## Related (current work)

- [VulkanEngineCore](https://github.com/ThomasDHZ/VulkanEngineCore)
- [VulkanGameEngine](https://github.com/ThomasDHZ/VulkanGameEngine)
- [VulkanGameEngineLevelEditor](https://github.com/ThomasDHZ/VulkanGameEngineLevelEditor)
- [ListPtr](https://github.com/ThomasDHZ/ListPtr)
