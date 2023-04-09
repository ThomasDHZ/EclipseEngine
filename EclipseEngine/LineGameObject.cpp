#include "LineGameObject.h"

LineGameObject::LineGameObject()
{
}

LineGameObject::LineGameObject(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint) : GameObject(Name, kLineRenderer3D)
{
	std::vector<LineVertex3D> LineVertexList =
	{
		{StartPoint, glm::vec4(1.0f)},
		{EndPoint, glm::vec4(1.0f)}
	};
	std::vector<uint32_t> LineIndexList = {
		0, 1
	};

	LoadRenderObject<LineVertex3D>(LineVertexList, LineIndexList);
}

LineGameObject::LineGameObject(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec3& position) : GameObject(Name, kLineRenderer3D, position)
{
	std::vector<LineVertex3D> LineVertexList =
	{
		{StartPoint, glm::vec4(1.0f)},
		{EndPoint, glm::vec4(1.0f)}
	};
	std::vector<uint32_t> LineIndexList = {
		0, 1
	};

	LoadRenderObject<LineVertex3D>(LineVertexList, LineIndexList);
}

LineGameObject::LineGameObject(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec3& position, const glm::vec3& rotation) : GameObject(Name, kLineRenderer3D, position, rotation)
{
	std::vector<LineVertex3D> LineVertexList =
	{
		{StartPoint, glm::vec4(1.0f)},
		{EndPoint, glm::vec4(1.0f)}
	};
	std::vector<uint32_t> LineIndexList = {
		0, 1
	};

	LoadRenderObject<LineVertex3D>(LineVertexList, LineIndexList);
}

LineGameObject::LineGameObject(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : GameObject(Name, kLineRenderer3D, position, rotation, scale)
{
	std::vector<LineVertex3D> LineVertexList =
	{
		{StartPoint, glm::vec4(1.0f)},
		{EndPoint, glm::vec4(1.0f)}
	};
	std::vector<uint32_t> LineIndexList = {
		0, 1
	};

	LoadRenderObject<LineVertex3D>(LineVertexList, LineIndexList);
}


LineGameObject::LineGameObject(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec4& Color) : GameObject(Name, kLineRenderer3D)
{
	std::vector<LineVertex3D> LineVertexList =
	{
		{StartPoint, Color},
		{EndPoint, Color}
	};
	std::vector<uint32_t> LineIndexList = {
		0, 1
	};

	LoadRenderObject<LineVertex3D>(LineVertexList, LineIndexList);
}

LineGameObject::LineGameObject(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec4& Color, const glm::vec3& position) : GameObject(Name, kLineRenderer3D, position)
{
	std::vector<LineVertex3D> LineVertexList =
	{
		{StartPoint, Color},
		{EndPoint, Color}
	};
	std::vector<uint32_t> LineIndexList = {
		0, 1
	};

	LoadRenderObject<LineVertex3D>(LineVertexList, LineIndexList);
}

LineGameObject::LineGameObject(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec4& Color, const glm::vec3& position, const glm::vec3& rotation) : GameObject(Name, kLineRenderer3D, position, rotation)
{
	std::vector<LineVertex3D> LineVertexList =
	{
		{StartPoint, Color},
		{EndPoint, Color}
	};
	std::vector<uint32_t> LineIndexList = {
		0, 1
	};

	LoadRenderObject<LineVertex3D>(LineVertexList, LineIndexList);
}

LineGameObject::LineGameObject(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec4& Color, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : GameObject(Name, kLineRenderer3D, position, rotation, scale)
{
	std::vector<LineVertex3D> LineVertexList =
	{
		{StartPoint, Color},
		{EndPoint, Color}
	};
	std::vector<uint32_t> LineIndexList = {
		0, 1
	};

	LoadRenderObject<LineVertex3D>(LineVertexList, LineIndexList);
}

LineGameObject::LineGameObject(const std::string Name, std::vector<LineVertex3D> VertexList) : GameObject(Name, kLineRenderer3D)
{
	LoadRenderObject<LineVertex3D>(VertexList);
}

LineGameObject::LineGameObject(const std::string Name, std::vector<LineVertex3D> VertexList, const glm::vec3 position) : GameObject(Name, kLineRenderer3D, position)
{
	LoadRenderObject<LineVertex3D>(VertexList);
}

LineGameObject::LineGameObject(const std::string Name, std::vector<LineVertex3D> VertexList, const glm::vec3 position, const glm::vec3 rotation) : GameObject(Name, kLineRenderer3D, position, rotation)
{
	LoadRenderObject<LineVertex3D>(VertexList);
}

LineGameObject::LineGameObject(const std::string Name, std::vector<LineVertex3D> VertexList, const glm::vec3 position, const glm::vec3 rotation, const glm::vec3 scale) : GameObject(Name, kLineRenderer3D, position, rotation, scale)
{
	LoadRenderObject<LineVertex3D>(VertexList);
}

LineGameObject::~LineGameObject()
{
}

void LineGameObject::DrawLine(VkCommandBuffer& commandBuffer, VkPipelineLayout shaderPipelineLayout, VkDescriptorSet descriptorSet)
{
	GameObject::DrawLine(commandBuffer, shaderPipelineLayout, descriptorSet);
}
