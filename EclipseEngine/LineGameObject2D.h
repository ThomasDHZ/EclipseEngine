#pragma once
#include "GameObject2D.h"

class LineGameObject2D : public GameObject2D
{
public:
	LineGameObject2D();
	LineGameObject2D(const std::string Name, int drawLayer);
	LineGameObject2D(const std::string Name, const glm::vec2& position, int drawLayer);
	LineGameObject2D(const std::string Name, const glm::vec2& position, const glm::vec2& rotation, int drawLayer);
	LineGameObject2D(const std::string Name, const glm::vec2& position, const glm::vec2& rotation, const glm::vec2& scale, int drawLayer);
	~LineGameObject2D();

	void LoadLineGameObject2D(const std::string Name, const glm::vec2& StartPoint, const glm::vec2& EndPoint, int drawLayer);
	void LoadLineGameObject2D(const std::string Name, const glm::vec2& StartPoint, const glm::vec2& EndPoint, const glm::vec4& Color, int drawLayer);
	void LoadLineGameObject2D(const std::string Name, const glm::vec2& StartPoint, const glm::vec2& EndPoint, const glm::vec4& StartColor, const glm::vec4& EndColor, int drawLayer);
	void LoadLineGameObject2D(const std::string Name, std::vector<LineVertex2D> VertexList, int drawLayer);
};

