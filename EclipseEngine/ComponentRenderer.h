#pragma once
#include "Component.h"
#include "Mesh.h"

class ComponentRenderer : public Component
{
private:

protected:
    Mesh mesh;

public:
    ComponentRenderer();
    ComponentRenderer(ComponentType componentType);

    void Draw(VkCommandBuffer& commandBuffer);
    void Update(float DeltaTime) override;
    void UpdateMeshProperties(MeshProperties& meshProperties);
    void Destroy() override;

    void SetBufferIndex(int bufferIndex);
    void SetMaterial(std::shared_ptr<Material> material);

    uint64_t GetMeshID() { return mesh.GetMeshID(); }
    uint32_t GetMeshBufferIndex() { return mesh.GetMeshBufferIndex(); }
    uint64_t GetBLASBufferDeviceAddress() { return mesh.GetBLASBufferDeviceAddress(); }
    VkBuffer GetMeshPropertiesBuffer() { return mesh.GetMeshPropertiesBuffer(); }
    VkBuffer GetMeshVertexBuffer() { return mesh.GetMeshVertexBuffer(); }
    VkBuffer GetMeshIndiceBuffer() { return mesh.GetMeshIndiceBuffer(); }
    std::shared_ptr<Material> GetMaterial() { return mesh.GetMaterial(); }

    virtual nlohmann::json ToJson() override
    {
        nlohmann::json json = Component::ToJson();

        return json;
    }
};

