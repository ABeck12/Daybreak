#pragma once

#include "Daybreak/Scene/Entity.h"
#include "Daybreak/Core/DeltaTime.h"

//#include "box2d/box2d.h"
//class b2World;

namespace Daybreak
{
	class Physics2D
	{
	public:
		void InitSimulation();
		void StepSimulation(DeltaTime dt); // TODO: Remove the DeltaTime
		void ShutdownSimulation();

		void AddEntity(Entity& entity);

	private:
		b2World* m_PhysicsWorld = nullptr;
	};
}

