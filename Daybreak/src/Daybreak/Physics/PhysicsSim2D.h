#pragma once

#include "Daybreak/Core/DeltaTime.h"
#include "Daybreak/Scene/Components.h"

#include "box2d/box2d.h"

namespace Daybreak
{
	class Entity;
	class Scene;

	class ContactListener : public b2ContactListener
	{
	public:
		void BeginContact(b2Contact* contact);
		void EndContact(b2Contact* contact);

		void SetScene(Scene* scene) { m_Scene = scene; }
		Scene* GetScene() { return m_Scene; }
	private:
		Scene* m_Scene;
	};

	class PhysicsSim2D
	{
	public:
		void InitSimulation(Scene* scene);
		void FixedStepSimulation();
		void ShutdownSimulation();

		void AddEntity(Entity& entity);

		//bool OverlapCollider(BoxCollider2DComponent bc2d);
		
		// Use colliderfilter type like on the component
		// bool RayCast(const glm::vec2& origin, const glm::vec2& direction, const float& depth, bool triggerCollidersOnHit = false); //TODO: Put in layer mask option

	private:
		// bool ContactFilter()

	private:
		float m_LastUpdateTime = 0.0f;
		b2World* m_PhysicsWorld;
		ContactListener* m_ContactLitener;
	};
}

