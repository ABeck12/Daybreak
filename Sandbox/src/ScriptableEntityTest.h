#pragma once

#include <Daybreak.h>

class MoveableComponent : public Daybreak::ScriptableEntity
{
public:
    void OnCreate()
    {
        m_Transform = GetComponent<Daybreak::TransformComponent>();
		m_RB = GetComponent<Daybreak::Rigidbody2DComponent>();
		// m_Anim = GetComponent<Daybreak::AnimatorComponent>();

		const Daybreak::Ref<Daybreak::Texture2D> spriteSheet = Daybreak::Texture2D::Create({ 3, 3, Daybreak::ImageFormat::RGBA, Daybreak::TextureFilterType::Point }, "../Sandbox/assets/adventurer-Sheet.png");
        float width = 50.;
        float height = 37.;
        auto& idle1 = Daybreak::SubTexture2D::Create(spriteSheet, { 0, 10 }, { width, height });
        auto& idle2 = Daybreak::SubTexture2D::Create(spriteSheet, { 1, 10 }, { width, height });
        auto& idle3 = Daybreak::SubTexture2D::Create(spriteSheet, { 2, 10 }, { width, height });
		auto& idle4 = Daybreak::SubTexture2D::Create(spriteSheet, { 3, 10 }, { width, height });

		auto& run1 = Daybreak::SubTexture2D::Create(spriteSheet, { 1, 9 }, { width, height });
		auto& run2 = Daybreak::SubTexture2D::Create(spriteSheet, { 2, 9 }, { width, height });
		auto& run3 = Daybreak::SubTexture2D::Create(spriteSheet, { 3, 9 }, { width, height });
		auto& run4 = Daybreak::SubTexture2D::Create(spriteSheet, { 4, 9 }, { width, height });
		auto& run5 = Daybreak::SubTexture2D::Create(spriteSheet, { 5, 9 }, { width, height });
		auto& run6 = Daybreak::SubTexture2D::Create(spriteSheet, { 6, 9 }, { width, height });
		
		uint32_t framesPerAnimationFrame = 10;
	    idleAnimation = Daybreak::CreateRef<Daybreak::AnimationSource>();
		idleAnimation->AddKeyFrame(idle1, framesPerAnimationFrame);
        idleAnimation->AddKeyFrame(idle2, framesPerAnimationFrame);
        idleAnimation->AddKeyFrame(idle3, framesPerAnimationFrame);
		idleAnimation->AddKeyFrame(idle4, framesPerAnimationFrame);
		auto action = [&]() {AnimTest();};
	    idleAnimation->AddKeyFrame(idle1, framesPerAnimationFrame, action);

		GetComponent<Daybreak::AnimatorComponent>().Source = idleAnimation;
		GetComponent<Daybreak::AnimatorComponent>().PixelsPerUnit = 50;
		

	    runAnimation = Daybreak::CreateRef<Daybreak::AnimationSource>();
        runAnimation->AddKeyFrame(run1, framesPerAnimationFrame);
        runAnimation->AddKeyFrame(run2, framesPerAnimationFrame);
        runAnimation->AddKeyFrame(run3, framesPerAnimationFrame);
        runAnimation->AddKeyFrame(run4, framesPerAnimationFrame);
        runAnimation->AddKeyFrame(run5, framesPerAnimationFrame);
        runAnimation->AddKeyFrame(run6, framesPerAnimationFrame);
        runAnimation->AddKeyFrame(run1, framesPerAnimationFrame);
		
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

		if (abs(m_RB.Velocity.x) > 0.02f)
		{
            GetComponent<Daybreak::AnimatorComponent>().Source = runAnimation;
		}
		else
        {
            GetComponent<Daybreak::AnimatorComponent>().Source = idleAnimation;
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
    Daybreak::AnimatorComponent m_Anim;
	bool m_IsGrounded = false;
	bool test = false;


	Daybreak::Ref<Daybreak::AnimationSource> idleAnimation;
	Daybreak::Ref<Daybreak::AnimationSource> runAnimation;
};