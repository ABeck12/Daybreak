#pragma once

#ifdef DB_SCRIPT_BUILD_DLL
#ifdef DB_PLATFORM_WINDOWS
#define DB_SCRIPT(className) extern "C" __declspec(dllexport)
#endif
#else
#define DB_SCRIPT(className) __declspec(dllimport)
#endif

#include "Daybreak/Core/DeltaTime.h"
#include "Daybreak/Scene/Entity.h"
#include "Daybreak/Physics/PhysicsSim2D.h"

namespace Daybreak
{
	class Script
	{
	public:
		virtual ~Script() {}

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	protected:
		virtual void OnCreate() {}
		virtual void OnUpdate(DeltaTime dt) {}
		virtual void OnDestroy() {}
		virtual void OnCollisionEnter(Entity& entity) {}
		virtual void OnCollisionExit(Entity& entity) {}

	private:
		Entity m_Entity;

		friend class ContactListener;
		friend class Scene;
	};
}