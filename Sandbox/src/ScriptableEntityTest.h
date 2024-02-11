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
        auto clip = Daybreak::AudioClip::Create("../Sandbox/assets/bonk.mp3");
        // Daybreak::AudioEngine::Play(clip);
    }

    void OnCollisionExit(Daybreak::Entity& entity)
    {

    }
};