#pragma once
#include "Component.h"
#include "Mesh.h"

class ComponentRenderer : public Component
{
private:

protected:
    Mesh mesh;
    void FromJson(const nlohmann::json& json) override
    {

    }

public:
    ComponentRenderer();
    ComponentRenderer(ComponentType componentType);

    void Draw(VkCommandBuffer& commandBuffer);
    void Update(float DeltaTime) override;
    void UpdateMeshProperties(MeshProperties& meshProperties);
    void Destroy() override;

    void SetBufferIndex(int bufferIndex);
    void SetMaterial(std::shared_ptr<Material> material);

    uint32_t GetMeshBufferIndex() { return mesh.GetMeshBufferIndex(); }
    uint64_t GetBLASBufferDeviceAddress() { return mesh.GetBLASBufferDeviceAddress(); }
    VkBuffer GetMeshPropertiesBuffer() { return mesh.GetMeshPropertiesBuffer(); }
    std::shared_ptr<Material> GetMaterial() { return mesh.GetMaterial(); }

    void ToJson(nlohmann::json& j) override
    {

    }
};

