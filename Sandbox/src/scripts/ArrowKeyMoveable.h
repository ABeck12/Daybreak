#pragma once

#pragma once

#include <Daybreak.h>

DB_SCRIPT class ArrowKeyMoveable : public Daybreak::Script
{
public:
	void OnCreate();
	void OnUpdate(Daybreak::DeltaTime dt);
	void OnCollisionEnter(Daybreak::Entity& entity);
	void OnCollisionExit(Daybreak::Entity& entity);

private:
	glm::vec3* m_Position;
};