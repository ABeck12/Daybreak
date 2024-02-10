#pragma once

#include <Daybreak.h>

class MoveableComponent : public Daybreak::ScriptableEntity
{
public:
    void OnCreate()
    {

    }

    void OnUpdate(Daybreak::DeltaTime dt)
    {
        float velamount = 75.0f * dt;
        if (Daybreak::Input::IsKeyPressed(Daybreak::Key::Space))
        {
            auto& velocity = GetComponent<Daybreak::Rigidbody2DComponent>().Velocity;
            velocity.y += velamount;
        }
        if (Daybreak::Input::IsKeyPressed(Daybreak::Key::D))
        {
            auto& velocity = GetComponent<Daybreak::Rigidbody2DComponent>().Velocity;
            velocity.x += velamount;
        }
        if (Daybreak::Input::IsKeyPressed(Daybreak::Key::A))
        {
            auto& velocity = GetComponent<Daybreak::Rigidbody2DComponent>().Velocity;
            velocity.x -= velamount;
        }
        if (Daybreak::Input::IsKeyPressed(Daybreak::Key::LeftShift))
        {
            auto& velocity = GetComponent<Daybreak::Rigidbody2DComponent>().Velocity;
            velocity.y -= velamount;
        }
    }

    void OnCollisionEnter(Daybreak::Entity& entity)
    {
        DB_LOG("Collided with {}", entity.GetName());
    }

    void OnCollisionExit(Daybreak::Entity& entity)
    {
        DB_LOG("Exited collision with {}", entity.GetName());
    }
};