#include "dbpch.h"

#include "Daybreak/Scene/Entity.h"
#include "Daybreak/Scene/Components.h"

namespace Daybreak
{
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene) {}

	const std::string& Entity::GetName()
	{
		return GetComponent<TagComponent>().Tag;
	}

	const UUID Entity::GetUUID()
	{
		return GetComponent<IDComponent>().ID;
	}
}