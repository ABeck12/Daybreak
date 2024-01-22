#include "dbpch.h"

#include "Daybreak/Scene/Entity.h"

namespace Daybreak
{
	Entity::Entity(entt::entity handle, Scene* scene) : m_EntityHandle(handle), m_Scene(scene) {}

}