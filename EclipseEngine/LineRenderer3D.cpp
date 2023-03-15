//#include "LineRenderer3D.h"
//
//LineRenderer3D::LineRenderer3D()
//{
//}
//
//LineRenderer3D::LineRenderer3D(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint) : GameObject(Name, kLineRenderer3D)
//{
//	LineObject = std::make_shared<LineMesh3D>(LineMesh3D(StartPoint, EndPoint));
//}
//
//LineRenderer3D::LineRenderer3D(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec3& position) : GameObject(Name, kLineRenderer3D, position)
//{
//	LineObject = std::make_shared<LineMesh3D>(LineMesh3D(StartPoint, EndPoint));
//}
//
//LineRenderer3D::LineRenderer3D(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec3& position, const glm::vec3& rotation) : GameObject(Name, kLineRenderer3D, position, rotation)
//{
//	LineObject = std::make_shared<LineMesh3D>(LineMesh3D(StartPoint, EndPoint));
//}
//
//LineRenderer3D::LineRenderer3D(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : GameObject(Name, kLineRenderer3D, position, rotation, scale)
//{
//	LineObject = std::make_shared<LineMesh3D>(LineMesh3D(StartPoint, EndPoint));
//}
//
//
//LineRenderer3D::LineRenderer3D(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec4& Color) : GameObject(Name, kLineRenderer3D)
//{
//	LineObject = std::make_shared<LineMesh3D>(LineMesh3D(StartPoint, EndPoint, Color));
//}
//
//LineRenderer3D::LineRenderer3D(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec4& Color, const glm::vec3& position) : GameObject(Name, kLineRenderer3D, position)
//{
//	LineObject = std::make_shared<LineMesh3D>(LineMesh3D(StartPoint, EndPoint, Color));
//}
//
//LineRenderer3D::LineRenderer3D(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec4& Color, const glm::vec3& position, const glm::vec3& rotation) : GameObject(Name, kLineRenderer3D, position, rotation)
//{
//	LineObject = std::make_shared<LineMesh3D>(LineMesh3D(StartPoint, EndPoint, Color));
//}
//
//LineRenderer3D::LineRenderer3D(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec4& Color, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : GameObject(Name, kLineRenderer3D, position, rotation, scale)
//{
//	LineObject = std::make_shared<LineMesh3D>(LineMesh3D(StartPoint, EndPoint, Color));
//}
//
//LineRenderer3D::LineRenderer3D(const std::string Name, std::vector<LineVertex3D> VertexList)
//{
//	LineObject = std::make_shared<LineMesh3D>(LineMesh3D(VertexList));
//}
//
//LineRenderer3D::LineRenderer3D(const std::string Name, std::vector<LineVertex3D> VertexList, const glm::vec3 position) : GameObject(Name, kLineRenderer3D, position)
//{
//	LineObject = std::make_shared<LineMesh3D>(LineMesh3D(VertexList));
//}
//
//LineRenderer3D::LineRenderer3D(const std::string Name, std::vector<LineVertex3D> VertexList, const glm::vec3 position, const glm::vec3 rotation) : GameObject(Name, kLineRenderer3D, position, rotation)
//{
//	LineObject = std::make_shared<LineMesh3D>(LineMesh3D(VertexList));
//}
//
//LineRenderer3D::LineRenderer3D(const std::string Name, std::vector<LineVertex3D> VertexList, const glm::vec3 position, const glm::vec3 rotation, const glm::vec3 scale) : GameObject(Name, kLineRenderer3D, position, rotation, scale)
//{
//	LineObject = std::make_shared<LineMesh3D>(LineMesh3D(VertexList));
//}
//
//LineRenderer3D::~LineRenderer3D()
//{
//}
//
//void LineRenderer3D::Update(float DeltaTime)
//{
//	GameObject::Update(DeltaTime);
//	LineObject->Update(GameObjectTransform, glm::mat4(1.0f));
//}