#pragma once

#include "Daybreak/Core/DeltaTime.h"
#include "Daybreak/Scene/Entity.h"
#include "Daybreak/Physics/PhysicsSim2D.h"

namespace Daybreak
{
    class ScriptableEntity
    {
    public:
        virtual ~ScriptableEntity() {}

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