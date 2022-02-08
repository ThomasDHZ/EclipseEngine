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
    ComponentRenderer(ComponentType componentType);

    void Draw(VkCommandBuffer& commandBuffer);
    void Update(float DeltaTime) override;
    void UpdateMeshProperties(MeshProperties& meshProperties);
    void Destroy() override;

    void SetBufferIndex(int bufferIndex);

    VkBuffer GetMeshPropertiesBuffer() { return mesh.GetMeshPropertiesBuffer(); }

    void ToJson(nlohmann::json& j) override
    {

    }
};

