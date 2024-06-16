#include "ArrowKeyMoveable.h"

void ArrowKeyMoveable::OnCreate()
{
	m_Position = &GetComponent<Daybreak::TransformComponent>().Position;
}

void ArrowKeyMoveable::OnUpdate(Daybreak::DeltaTime dt)
{
	float amount = 5 * dt;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::Up))
	{
		(*m_Position).y += amount;
	}
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::Down))
	{
		(*m_Position).y -= amount;
	}
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::Right))
	{
		(*m_Position).x += amount;
	}
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::Left))
	{
		(*m_Position).x -= amount;
	}
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::Comma))
	{
		(*m_Position).z += amount;
	}
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::Period))
	{
		(*m_Position).z -= amount;
	}
}

void ArrowKeyMoveable::OnCollisionEnter(Daybreak::Entity& entity)
{
}

void ArrowKeyMoveable::OnCollisionExit(Daybreak::Entity& entity)
{
}
