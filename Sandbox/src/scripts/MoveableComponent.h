#pragma once

#include <Daybreak.h>

DB_SCRIPT class MoveableComponent : public Daybreak::Script
{
public:
	void OnCreate();
	void OnUpdate(Daybreak::DeltaTime dt);
	void OnCollisionEnter(Daybreak::Entity& entity);
	void OnCollisionExit(Daybreak::Entity& entity);

	void AnimTest();

private:
	Daybreak::TransformComponent m_Transform;
	Daybreak::Rigidbody2DComponent m_RB;
	Daybreak::AnimatorComponent m_Anim;
	bool m_IsGrounded = false;
	bool test = false;


	Daybreak::Ref<Daybreak::Animation> idleAnimation;
	Daybreak::Ref<Daybreak::Animation> runAnimation;
};