#include "GameObject2D.h"
#include "GameObjectManager.h"
#include "LineMesh2D.h"

class LineRenderer2D : public GameObject
{
private:
	std::shared_ptr<LineMesh2D> LineObject;

public:
	LineRenderer2D();

	LineRenderer2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint);
	LineRenderer2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec2& position);
	LineRenderer2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec2& position, glm::vec2& rotation);
	LineRenderer2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec2& position, glm::vec2& rotation, glm::vec2& scale);

	LineRenderer2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec3& Color);
	LineRenderer2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec3& Color, glm::vec2& position);
	LineRenderer2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec3& Color, glm::vec2& position, glm::vec2& rotation);
	LineRenderer2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec3& Color, glm::vec2& position, glm::vec2& rotation, glm::vec2& scale);

	LineRenderer2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec4& Color);
	LineRenderer2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec4& Color, glm::vec2& position);
	LineRenderer2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec4& Color, glm::vec2& position, glm::vec2& rotation);
	LineRenderer2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec4& Color, glm::vec2& position, glm::vec2& rotation, glm::vec2& scale);

	LineRenderer2D(const std::string Name, std::vector<LineVertex2D>& VertexList);
	LineRenderer2D(const std::string Name, std::vector<LineVertex2D>& VertexList, glm::vec2& position);
	LineRenderer2D(const std::string Name, std::vector<LineVertex2D>& VertexList, glm::vec2& position, glm::vec2& rotation);
	LineRenderer2D(const std::string Name, std::vector<LineVertex2D>& VertexList, glm::vec2& position, glm::vec2& rotation, glm::vec2& scale);

	~LineRenderer2D();

	virtual void Update(float DeltaTime) override;
};
