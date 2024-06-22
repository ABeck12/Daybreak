#include "MoveableComponent.h"

void MoveableComponent::OnCreate()
{
	m_Transform = GetComponent<Daybreak::TransformComponent>();
	m_RB = GetComponent<Daybreak::Rigidbody2DComponent>();
	m_Anim = GetComponent<Daybreak::AnimatorComponent>();

	// m_Anim.Controller = Daybreak::CreateRef<Daybreak::AnimationController>();
	GetComponent<Daybreak::AnimatorComponent>().Controller = Daybreak::CreateRef<Daybreak::AnimationController>();

	const Daybreak::Ref<Daybreak::Texture2D> spriteSheet = Daybreak::Texture2D::Create({ 3, 3, Daybreak::ImageFormat::RGBA, Daybreak::TextureFilterType::Point, 37 }, "../Sandbox/assets/sprites/adventurer-Sheet.png");
	float width = 50.;
	float height = 37.;
	auto idle1 = Daybreak::SubTexture2D::Create(spriteSheet, { 0, 10 }, { width, height });
	auto idle2 = Daybreak::SubTexture2D::Create(spriteSheet, { 1, 10 }, { width, height });
	auto idle3 = Daybreak::SubTexture2D::Create(spriteSheet, { 2, 10 }, { width, height });
	auto idle4 = Daybreak::SubTexture2D::Create(spriteSheet, { 3, 10 }, { width, height });

	auto run1 = Daybreak::SubTexture2D::Create(spriteSheet, { 1, 9 }, { width, height });
	auto run2 = Daybreak::SubTexture2D::Create(spriteSheet, { 2, 9 }, { width, height });
	auto run3 = Daybreak::SubTexture2D::Create(spriteSheet, { 3, 9 }, { width, height });
	auto run4 = Daybreak::SubTexture2D::Create(spriteSheet, { 4, 9 }, { width, height });
	auto run5 = Daybreak::SubTexture2D::Create(spriteSheet, { 5, 9 }, { width, height });
	auto run6 = Daybreak::SubTexture2D::Create(spriteSheet, { 6, 9 }, { width, height });

	uint32_t framesPerAnimationFrame = 10;
	idleAnimation = Daybreak::CreateRef<Daybreak::Animation>("idle");
	idleAnimation->AddKeyFrame(idle1, framesPerAnimationFrame);
	idleAnimation->AddKeyFrame(idle2, framesPerAnimationFrame);
	idleAnimation->AddKeyFrame(idle3, framesPerAnimationFrame);
	idleAnimation->AddKeyFrame(idle4, framesPerAnimationFrame);
	auto action = [&]()
	{
		AnimTest();
	};
	idleAnimation->AddKeyFrame(idle1, framesPerAnimationFrame, action);

	// GetComponent<Daybreak::AnimatorComponent>().Source = idleAnimation;
	// GetComponent<Daybreak::AnimatorComponent>().PixelsPerUnit = 37;


	runAnimation = Daybreak::CreateRef<Daybreak::Animation>("run");
	runAnimation->AddKeyFrame(run1, framesPerAnimationFrame);
	runAnimation->AddKeyFrame(run2, framesPerAnimationFrame);
	runAnimation->AddKeyFrame(run3, framesPerAnimationFrame);
	runAnimation->AddKeyFrame(run4, framesPerAnimationFrame);
	runAnimation->AddKeyFrame(run5, framesPerAnimationFrame);
	runAnimation->AddKeyFrame(run6, framesPerAnimationFrame);
	runAnimation->AddKeyFrame(run1, framesPerAnimationFrame);

	GetComponent<Daybreak::AnimatorComponent>().Controller->AddAnimation(idleAnimation);
	GetComponent<Daybreak::AnimatorComponent>().Controller->AddAnimation(runAnimation);
	GetComponent<Daybreak::AnimatorComponent>().Controller->ChangeAnimation("idle");
	GetComponent<Daybreak::AnimatorComponent>().Controller->SetStartupAnimation("idle");
}

void MoveableComponent::OnUpdate(Daybreak::DeltaTime dt)
{
	auto& m_Transform = GetComponent<Daybreak::TransformComponent>();
	auto& m_RB = GetComponent<Daybreak::Rigidbody2DComponent>();
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::P))
	{
		m_IsGrounded = true;
	}

	float velamount = 150.0f * dt;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::Space) && m_IsGrounded)
	{
		m_RB.Velocity.y += 2.5f * velamount;
		m_IsGrounded = false;
	}
	if (Daybreak::Input::IsGamepadButtonPressed(Daybreak::Gamepad::Gamepad1, Daybreak::GamepadButton::ButtonDown))
	{
		m_RB.Velocity.y += 2.5f * velamount;
		// m_IsGrounded = false;
	}
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::D))
	{
		m_RB.Velocity.x = 2.5f;
		GetComponent<Daybreak::TransformComponent>().Scale.x = 1;
	}
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::A))
	{
		m_RB.Velocity.x = -2.5f;
		GetComponent<Daybreak::TransformComponent>().Scale.x = -1;
	}
	if (Daybreak::Input::IsGamepadConnected(Daybreak::Gamepad::Gamepad1))
	{
		float xaxis = Daybreak::Input::GetGamepadAxis(Daybreak::Gamepad::Gamepad1, Daybreak::GamepadAxis::LeftX);
		if (abs(xaxis) < 0.2)
		{
			m_RB.Velocity.x = 0.0f;
		}
		else
		{
			m_RB.Velocity.x = 2.5f * xaxis;
			GetComponent<Daybreak::TransformComponent>().Scale.x = glm::sign(xaxis);
		}
	}
	if (abs(m_RB.Velocity.x) > 0.02f)
	{
		// GetComponent<Daybreak::AnimatorComponent>().Source = runAnimation;
		if (GetComponent<Daybreak::AnimatorComponent>().Controller)
			GetComponent<Daybreak::AnimatorComponent>().Controller->ChangeAnimation("run");
	}
	else
	{
		// GetComponent<Daybreak::AnimatorComponent>().Source = idleAnimation;
		if (GetComponent<Daybreak::AnimatorComponent>().Controller)
			GetComponent<Daybreak::AnimatorComponent>().Controller->ChangeAnimation("idle");
	}
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::G))
	{
		// Daybreak::Scene::s_ActiveSim->AddAngularImpulse(m_RB, 10.0f);
		// DB_LOG("G");
		// Daybreak::PhysicsSim2D::GetActiveSim()->AddLinearImpulse(m_RB, { 10, 150 });
		// Daybreak::PhysicsSim2D::GetActiveSim()->AddAngularImpulse(m_RB, 10.0f);
		// m_RB.Velocity.x = -velamount;
		// GetComponent<Daybreak::TransformComponent>().Scale.x = -1;
	}


	// if (Daybreak::Input::IsKeyPressed(Daybreak::Key::LeftShift))
	// {
	// m_RB.Velocity.y = -velamount;
	// }
	// auto test = m_Entity->m_Scene->GetPhysicsSim2D().Raycast({m_Transform.Position.x, m_Transform.Position.y}, {m_Transform.Position.x, m_Transform.Position.y - 0.5f});
	// DB_LOG(test);
}

void MoveableComponent::OnCollisionEnter(Daybreak::Entity& entity)
{
	m_IsGrounded = true;
	// DB_LOG("enter {}", m_IsGrounded);
}

void MoveableComponent::OnCollisionExit(Daybreak::Entity& entity)
{
	m_IsGrounded = false;
}

void MoveableComponent::AnimTest()
{
	// DB_LOG(test);
	if (test)
		test = false;
	else
		test = true;
}