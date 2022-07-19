#pragma once
#include "ComponentRenderer.h"
#include "LineMesh2D.h"
#include "Vertex.h"
#include "MeshRendererManager.h"

class LineRenderer2D : public Component
{
private:
	std::shared_ptr<Mesh> Line;
public:
	LineRenderer2D();
	LineRenderer2D(std::vector<LineVertex2D>& VertexList, uint64_t GameObjectID);
	LineRenderer2D(glm::vec2& StartPoint, glm::vec2& EndPoint, uint64_t GameObjectID);
	LineRenderer2D(glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec3& LineColor, uint64_t GameObjectID);
	LineRenderer2D(glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec4& LineColor, uint64_t GameObjectID);
	~LineRenderer2D();

	void Update(const glm::mat4& GameObjectMatrix, float DeltaTime);
	void Destroy() override;

	std::shared_ptr<Mesh> GetLine() { return Line; }
};

