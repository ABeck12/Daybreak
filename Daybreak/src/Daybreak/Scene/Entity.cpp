#include "dbpch.h"

#include "Daybreak/Scene/Entity.h"
#include "Daybreak/Scene/Components.h"

namespace Daybreak
{
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene) {}

	const std::string& Entity::GetName() const
	{
		return GetComponent<TagComponent>().Tag;
	}

	const UUID Entity::GetUUID() const
	{
		return GetComponent<IDComponent>().ID;
	}

	const bool Entity::IsActive() const
	{
		return GetComponent<ActiveComponent>().Active;
	}

	const UUID Entity::GetParentUUID() const
	{
		return GetComponent<RelationshipComponent>().ParentID;
	}

	const bool Entity::HasParent() const
	{
		return GetComponent<RelationshipComponent>().ParentID != 0;
	}
}