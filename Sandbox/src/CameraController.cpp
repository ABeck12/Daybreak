#include <Daybreak.h>

#include "CameraController.h"

CameraController::CameraController(const glm::mat4 projection)
{
	m_Camera = Daybreak::Camera(projection);
	UpdateView();
}

void CameraController::Update()
{
	float amount = 0.5f;

	//if (Daybreak::Input::IsKeyPressed(DB_KEY_S)) { cameraPos.y += amount; }
	//if (Daybreak::Input::IsKeyPressed(DB_KEY_W)) { cameraPos.y -= amount; }
	//if (Daybreak::Input::IsKeyPressed(DB_KEY_A)) { cameraPos.x += amount; }
	//if (Daybreak::Input::IsKeyPressed(DB_KEY_D)) { cameraPos.x -= amount; }
	//if (Daybreak::Input::IsKeyPressed(DB_KEY_E)) { cameraPos.z += amount; }
	//if (Daybreak::Input::IsKeyPressed(DB_KEY_Q)) { cameraPos.z -= amount; }
	UpdateView();

	//r = glm::mat4(1.0f);
	//r = glm::rotate(r, glm::radians(cameraRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
	//r = glm::rotate(r, glm::radians(cameraRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
	//r = glm::rotate(r, glm::radians(cameraRot.z), glm::vec3(0.0f, 0.0f, 1.0f));
	float m_CameraTranslationSpeed = 0.1f;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::A))
	{
		cameraPos.x -= cos(glm::radians(cameraRot.z)) * m_CameraTranslationSpeed;
		cameraPos.y -= sin(glm::radians(cameraRot.z)) * m_CameraTranslationSpeed;
	}
	else if (Daybreak::Input::IsKeyPressed(Daybreak::Key::D))
	{
		cameraPos.x += cos(glm::radians(cameraRot.z)) * m_CameraTranslationSpeed;
		cameraPos.y += sin(glm::radians(cameraRot.z)) * m_CameraTranslationSpeed;
	}

	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::W))
	{
		cameraPos.x += -sin(glm::radians(cameraRot.z)) * m_CameraTranslationSpeed;
		cameraPos.y += cos(glm::radians(cameraRot.z)) * m_CameraTranslationSpeed;
	}
	else if (Daybreak::Input::IsKeyPressed(Daybreak::Key::S))
	{
		cameraPos.x -= -sin(glm::radians(cameraRot.z)) * m_CameraTranslationSpeed;
		cameraPos.y -= cos(glm::radians(cameraRot.z)) * m_CameraTranslationSpeed;
	}
	float m_CameraRotationSpeed = 10.0f;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::Q))
		cameraRot.z += m_CameraRotationSpeed;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::E))
		cameraRot.z -= m_CameraRotationSpeed;
}

void CameraController::UpdateView()
{
	m_View = glm::translate(glm::mat4(1.0f), cameraPos);
	m_View = glm::rotate(m_View, glm::radians(cameraRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_View = glm::rotate(m_View, glm::radians(cameraRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
	m_View = glm::rotate(m_View, glm::radians(cameraRot.z), glm::vec3(0.0f, 0.0f, 1.0f));
}