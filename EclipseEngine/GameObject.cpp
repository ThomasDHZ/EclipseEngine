#include "GameObject.h"
#include "LineRenderer.h"

uint64_t GameObject::GameObjectIDCounter = 0;

GameObject::GameObject()
{
}

GameObject::GameObject(const std::string Name, std::vector<LineVertex>& VertexList, int a)
{
	ObjectName = Name;
	AddComponent(std::make_shared<Transform3D>(Transform3D(glm::vec3(0.0f))));
	AddComponent(std::make_shared<LineRenderer>(LineRenderer(VertexList)));
	GenerateID();
}

GameObject::GameObject(const std::string Name, glm::vec3 StartLine, glm::vec3 EndLine, int a)
{
	ObjectName = Name;
	AddComponent(std::make_shared<Transform3D>(Transform3D(glm::vec3(0.0f))));
	AddComponent(std::make_shared<LineRenderer>(LineRenderer(StartLine, EndLine)));
	GenerateID();
}

GameObject::GameObject(const std::string Name)
{
	ObjectName = Name;
	GenerateID();
}

GameObject::GameObject(const std::string Name, glm::vec2 position, uint32_t zIndex)
{
	ObjectName = Name;
	AddComponent(std::make_shared<Transform2D>(Transform2D(position, zIndex)));
	AddComponent(std::make_shared<SpriteRenderer>(SpriteRenderer()));
	GenerateID();
}

GameObject::GameObject(const std::string Name, glm::vec2 position, glm::vec2 rotation, uint32_t zIndex)
{
	ObjectName = Name;
	AddComponent(std::make_shared<Transform2D>(Transform2D(position, rotation, zIndex)));
	AddComponent(std::make_shared<SpriteRenderer>(SpriteRenderer()));
	GenerateID();
}

GameObject::GameObject(const std::string Name, glm::vec2 position, glm::vec2 rotation, glm::vec2 scale, uint32_t zIndex)
{
	ObjectName = Name;
	AddComponent(std::make_shared<Transform2D>(Transform2D(position, rotation, scale, zIndex)));
	AddComponent(std::make_shared<SpriteRenderer>(SpriteRenderer()));
	GenerateID();
}

GameObject::GameObject(const std::string Name, glm::vec3 position)
{
	ObjectName = Name;
	AddComponent(std::make_shared<Transform3D>(Transform3D(position)));
	AddComponent(std::make_shared<MeshRenderer>(MeshRenderer()));

	GenerateID();
}

GameObject::GameObject(const std::string Name, glm::vec3 position, glm::vec3 rotation)
{
	ObjectName = Name;
	AddComponent(std::make_shared<Transform3D>(Transform3D(position, rotation)));
	AddComponent(std::make_shared<MeshRenderer>(MeshRenderer()));
	GenerateID();
}

GameObject::GameObject(const std::string Name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	ObjectName = Name;
	AddComponent(std::make_shared<Transform3D>(Transform3D(position, rotation, scale)));
	AddComponent(std::make_shared<MeshRenderer>(MeshRenderer()));
	GenerateID();
}

GameObject::GameObject(nlohmann::json& json)
{
	std::cout << json << std::endl;

	json.at("ObjectName").get_to(ObjectName);

	for (int x = 0; x <= json.size(); x++)
	{
		ComponentType type = ComponentType::kNullComponent;
		json["ComponentList"][x].at("componentType").get_to(type);

		switch (type)
		{
		case ComponentType::kTransform2D: { AddComponent(std::make_shared<Transform2D>(Transform2D(json["ComponentList"][x]))); break; }
		case ComponentType::kTransform3D: { AddComponent(std::make_shared<Transform3D>(Transform3D(json["ComponentList"][x]))); break; }
		case ComponentType::kSpriteRenderer: {break; }
		case ComponentType::kMeshRenderer:
		{

			MeshRenderer a = MeshRenderer(json["ComponentList"][x]);
			break;
		}
		}
	}
	auto abs = 34;
}

GameObject::~GameObject()
{
}

void GameObject::Update(float DeltaTime)
{
	for (auto& comp : ComponentList)
	{
		comp->Update(DeltaTime);
	}

	//auto objRenderer = GetComponentBySubType(ComponentSubType::kRenderedObject);
	//auto componentTransform = GetComponentBySubType(ComponentSubType::kTransform);
	//if (objRenderer != nullptr &&
	//	componentTransform != nullptr)
	//{
	//	ComponentRenderer* objRendererPtr = static_cast<ComponentRenderer*>(objRenderer.get());
	//	for (auto& mesh : objRendererPtr->GetModel()->GetMeshList())
	//	{
	//		Transform* transformPtr = static_cast<Transform*>(componentTransform.get());

	//		MeshProperties meshProps = {};
	//		meshProps.MeshTransform = transformPtr->TransformMatrix;
	//		meshProps.materialBufferData = mesh->GetMaterial()->GetMaterialTextureData();
	//		objRendererPtr->UpdateMeshProperties(meshProps);
	//	}
	//}

	auto spriteRenderer = GetComponentByType(ComponentType::kSpriteRenderer);
	auto transform2D = GetComponentByType(ComponentType::kTransform2D);
	if (spriteRenderer != nullptr &&
		transform2D != nullptr)
	{
		ComponentRenderer* sprite = static_cast<ComponentRenderer*>(spriteRenderer.get());
		Transform2D* transform = static_cast<Transform2D*>(transform2D.get());

		MeshProperties meshProps = {};
		meshProps.MeshTransform = transform->TransformMatrix;
		meshProps.materialBufferData = sprite->GetMaterial()->GetMaterialTextureData();
		sprite->UpdateMeshProperties(meshProps);
	}

	auto meshRenderer = GetComponentByType(ComponentType::kMeshRenderer);
	auto transform3D = GetComponentByType(ComponentType::kTransform3D);
	if (meshRenderer != nullptr &&
		transform3D != nullptr)
	{
		ComponentRenderer* mesh = static_cast<ComponentRenderer*>(meshRenderer.get());
		Transform3D* transform = static_cast<Transform3D*>(transform3D.get());

		MeshProperties meshProps = {};
		meshProps.MeshTransform = transform->TransformMatrix;
		meshProps.materialBufferData = mesh->GetMaterial()->GetMaterialTextureData();
		mesh->UpdateMeshProperties(meshProps);
	}

	auto lineRenderer = GetComponentByType(ComponentType::kLineRenderer);
	if (lineRenderer != nullptr &&
		transform3D != nullptr)
	{
		ComponentRenderer* line = static_cast<ComponentRenderer*>(lineRenderer.get());
		Transform3D* transform = static_cast<Transform3D*>(transform3D.get());

		MeshProperties meshProps = {};
		meshProps.MeshTransform = transform->TransformMatrix;
		meshProps.materialBufferData = line->GetMaterial()->GetMaterialTextureData();
		line->UpdateMeshProperties(meshProps);
	}
}

void GameObject::Draw(VkCommandBuffer& commandBuffer)
{
	auto objRenderer = GetComponentBySubType(ComponentSubType::kRenderedObject);
	if (objRenderer != nullptr)
	{
		static_cast<ComponentRenderer*>(objRenderer.get())->Draw(commandBuffer);
	}
}

void GameObject::Destory()
{
	for (auto& comp : ComponentList)
	{
		comp->Destroy();
	}
}

void GameObject::AddComponent(std::shared_ptr<Component> component)
{
	component->GenerateID();
	ComponentList.emplace_back(component);
}

void GameObject::RemoveComponent(std::shared_ptr<Component> component)
{
	for (int x = ComponentList.size() - 1; x > 0; x--)
	{
		if (component->GetComponentType() == ComponentList[x]->GetComponentType())
		{
			ComponentList[x]->Destroy();
			ComponentList.erase(ComponentList.begin() + x);
			break;
		}
	}
}

void GameObject::GenerateID()
{
	GameObjectIDCounter++;
	GameObjectID = GameObjectIDCounter;
}

std::shared_ptr<Component> GameObject::GetComponentByType(ComponentType componentType)
{
	for (auto& comp : ComponentList)
	{
		if (comp->GetComponentType() == componentType)
		{
			return comp;
		}
	}

	return nullptr;
}

std::shared_ptr<Component> GameObject::GetComponentBySubType(ComponentSubType componentSubType)
{
	for (auto& comp : ComponentList)
	{
		if (comp->GetComponentSubType() == componentSubType)
		{
			return comp;
		}
	}

	return nullptr;
}

std::shared_ptr<Component> GameObject::GetComponentByID(uint64_t ComponentID)
{
	for (auto& comp : ComponentList)
	{
		if (comp->GetComponentID() == ComponentID)
		{
			return comp;
		}
	}

	return nullptr;
}

void GameObject::GetGameObjectPropertiesBuffer(std::vector<VkDescriptorBufferInfo>& MeshPropertiesBufferList)
{

	//auto objRenderer = GetComponentBySubType(ComponentSubType::kRenderedObject);
	//if (objRenderer != nullptr)
	//{
	//	ComponentRenderer* objRendererPtr = static_cast<ComponentRenderer*>(objRenderer.get());
	//	for (auto& mesh : objRendererPtr->GetModel()->GetMeshList())
	//	{
	//		MeshProperties meshProps = {};
	//		objRendererPtr->UpdateMeshProperties(meshProps);
	//		VkBuffer buffer = mesh->GetMeshPropertiesBuffer();

	//		VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
	//		MeshPropertiesmBufferBufferInfo.buffer = buffer;
	//		MeshPropertiesmBufferBufferInfo.offset = 0;
	//		MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
	//		MeshPropertiesBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);

	//		objRendererPtr->SetBufferIndex(MeshPropertiesBufferList.size() - 1);
	//	}
	//}

	auto objRenderer = GetComponentBySubType(ComponentSubType::kRenderedObject);
	if (objRenderer != nullptr)
	{
		ComponentRenderer* objRendererPtr = static_cast<ComponentRenderer*>(objRenderer.get());

			MeshProperties meshProps = {};
			objRendererPtr->UpdateMeshProperties(meshProps);
			VkBuffer buffer = objRendererPtr->GetMeshPropertiesBuffer();

			VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
			MeshPropertiesmBufferBufferInfo.buffer = buffer;
			MeshPropertiesmBufferBufferInfo.offset = 0;
			MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
			MeshPropertiesBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);

			objRendererPtr->SetBufferIndex(MeshPropertiesBufferList.size() - 1);
	}
}

void GameObject::GetMeshIndexBuffer(std::vector<VkDescriptorBufferInfo>& IndexBufferList)
{
//	auto objRenderer = GetComponentBySubType(ComponentSubType::kRenderedObject);
//if (objRenderer != nullptr)
//{
//	ComponentRenderer* objRendererPtr = static_cast<ComponentRenderer*>(objRenderer.get());
//	for (auto& mesh : objRendererPtr->GetModel()->GetMeshList())
//	{
//		MeshProperties meshProps = {};
//		objRendererPtr->UpdateMeshProperties(meshProps);
//		VkBuffer buffer = mesh->GetMeshPropertiesBuffer();
//
//		VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
//		MeshPropertiesmBufferBufferInfo.buffer = buffer;
//		MeshPropertiesmBufferBufferInfo.offset = 0;
//		MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
//		IndexBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);
//
//		objRendererPtr->SetBufferIndex(IndexBufferList.size() - 1);
//	}
//}

	auto objRenderer = GetComponentBySubType(ComponentSubType::kRenderedObject);
	if (objRenderer != nullptr)
	{
		ComponentRenderer* objRendererPtr = static_cast<ComponentRenderer*>(objRenderer.get());

		MeshProperties meshProps = {};
		VkBuffer buffer = objRendererPtr->GetMeshIndiceBuffer();

		VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
		MeshPropertiesmBufferBufferInfo.buffer = buffer;
		MeshPropertiesmBufferBufferInfo.offset = 0;
		MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
		IndexBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);

		objRendererPtr->SetBufferIndex(IndexBufferList.size() - 1);
	}
}

void GameObject::GetMeshVertexBuffer(std::vector<VkDescriptorBufferInfo>& VertexBufferList)
{
//	auto objRenderer = GetComponentBySubType(ComponentSubType::kRenderedObject);
//if (objRenderer != nullptr)
//{
	//ComponentRenderer* objRendererPtr = static_cast<ComponentRenderer*>(objRenderer.get());
	//for (auto& mesh : objRendererPtr->GetModel()->GetMeshList())
	//{
	//	MeshProperties meshProps = {};
	//	objRendererPtr->UpdateMeshProperties(meshProps);
	//	VkBuffer buffer = mesh->GetMeshPropertiesBuffer();

	//	VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
	//	MeshPropertiesmBufferBufferInfo.buffer = buffer;
	//	MeshPropertiesmBufferBufferInfo.offset = 0;
	//	MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
	//	VertexBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);

	//	objRendererPtr->SetBufferIndex(VertexBufferList.size() - 1);
	//}
//}

	auto objRenderer = GetComponentBySubType(ComponentSubType::kRenderedObject);
	if (objRenderer != nullptr)
	{
		ComponentRenderer* objRendererPtr = static_cast<ComponentRenderer*>(objRenderer.get());

		MeshProperties meshProps = {};
		VkBuffer buffer = objRendererPtr->GetMeshVertexBuffer();

		VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
		MeshPropertiesmBufferBufferInfo.buffer = buffer;
		MeshPropertiesmBufferBufferInfo.offset = 0;
		MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
		VertexBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);

		objRendererPtr->SetBufferIndex(VertexBufferList.size() - 1);
	}
}
