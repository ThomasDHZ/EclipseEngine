#pragma once
#include "ComponentRenderer.h"
#include "LineMesh2D.h"
#include "Vertex.h"
#include "MeshRendererManager.h"

class LineRenderer2DComponent : public Component
{
private:
	std::shared_ptr<Mesh> Line;
public:
	LineRenderer2DComponent();
	LineRenderer2DComponent(std::vector<LineVertex2D>& VertexList, uint64_t GameObjectID);
	LineRenderer2DComponent(glm::vec2& StartPoint, glm::vec2& EndPoint, uint64_t GameObjectID);
	LineRenderer2DComponent(glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec3& LineColor, uint64_t GameObjectID);
	LineRenderer2DComponent(glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec4& LineColor, uint64_t GameObjectID);
	~LineRenderer2DComponent();

	void Update(const glm::mat4& GameObjectMatrix, float DeltaTime);
	void Destroy() override;

	std::shared_ptr<Mesh> GetLine() { return Line; }
};

