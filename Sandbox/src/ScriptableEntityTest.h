#pragma once

#include <Daybreak.h>

class MoveableComponent : public Daybreak::ScriptableEntity
{
public:
    void OnCreate()
    {
        m_Transform = GetComponent<Daybreak::TransformComponent>();
		m_RB = GetComponent<Daybreak::Rigidbody2DComponent>();

		auto& anim = GetComponent<Daybreak::AnimatorComponent>();
		const Daybreak::Ref<Daybreak::Texture2D> spriteSheet = Daybreak::Texture2D::Create({ 3, 3, Daybreak::ImageFormat::RGBA, Daybreak::TextureFilterType::Point }, "../Sandbox/assets/adventurer-Sheet.png");
        float width = 50.;
        float height = 37.;
        auto& subtexture1 = Daybreak::SubTexture2D::Create(spriteSheet, { 0, 10 }, { width, height });
        auto& subtexture2 = Daybreak::SubTexture2D::Create(spriteSheet, { 1, 10 }, { width, height });
        auto& subtexture3 = Daybreak::SubTexture2D::Create(spriteSheet, { 2, 10 }, { width, height });
		auto& subtexture4 = Daybreak::SubTexture2D::Create(spriteSheet, { 3, 10 }, { width, height });

	    Daybreak::Ref<Daybreak::AnimationSource> animSource = Daybreak::CreateRef<Daybreak::AnimationSource>();
		animSource->AddKeyFrame(subtexture1, 10);
        animSource->AddKeyFrame(subtexture2, 10);
        animSource->AddKeyFrame(subtexture3, 10);
		animSource->AddKeyFrame(subtexture4, 10);
		auto action = [&]() {AnimTest();};
	    animSource->AddKeyFrame(subtexture1, 10, action);

		anim.Source = animSource;
    }

    void OnUpdate(Daybreak::DeltaTime dt)
    {
        auto& m_Transform = GetComponent<Daybreak::TransformComponent>();
		auto& m_RB = GetComponent<Daybreak::Rigidbody2DComponent>();

        float velamount = 150.0f * dt;
        if (Daybreak::Input::IsKeyPressed(Daybreak::Key::Space) && m_IsGrounded)
        {
            m_RB.Velocity.y += 2.5f * velamount;
            m_IsGrounded = false;
        }
        if (Daybreak::Input::IsKeyPressed(Daybreak::Key::D))
        {
            m_RB.Velocity.x = velamount;
            GetComponent<Daybreak::TransformComponent>().Scale.x = 1;
        }
        if (Daybreak::Input::IsKeyPressed(Daybreak::Key::A))
        {
            m_RB.Velocity.x = -velamount;
            GetComponent<Daybreak::TransformComponent>().Scale.x = -1;
        }
        // if (Daybreak::Input::IsKeyPressed(Daybreak::Key::LeftShift))
        // {
            // m_RB.Velocity.y = -velamount;
        // }
        // auto test = m_Entity->m_Scene->GetPhysicsSim2D().Raycast({m_Transform.Position.x, m_Transform.Position.y}, {m_Transform.Position.x, m_Transform.Position.y - 0.5f});
        // DB_LOG(test);

    }

    void OnCollisionEnter(Daybreak::Entity& entity)
    {
        m_IsGrounded = true;
        // DB_LOG("enter {}", m_IsGrounded);

        auto clip = Daybreak::AudioClip::Create("../Sandbox/assets/bonk.mp3");
        // Daybreak::AudioEngine::Play(clip);
    }

    void OnCollisionExit(Daybreak::Entity& entity)
    {
        m_IsGrounded = false;
	}

	void AnimTest()
	{
        // DB_LOG(test);
		if (test)
			test = false;
		else
			test = true;
    }

private:
    Daybreak::TransformComponent m_Transform;
    Daybreak::Rigidbody2DComponent m_RB;
	bool m_IsGrounded = false;
	bool test = false;
};