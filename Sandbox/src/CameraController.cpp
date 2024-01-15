#include <Daybreak.h>

#include "CameraController.h"

CameraController::CameraController(const glm::mat4 projection)
{
	m_Camera = Daybreak::Camera(projection);
	UpdateView();
}

void CameraController::Update(Daybreak::DeltaTime dt)
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
	float m_CameraTranslationSpeed = -10.0f;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::A))
	{
		cameraPos.x -= cos(glm::radians(cameraRot.z)) * m_CameraTranslationSpeed * dt;
		cameraPos.y -= sin(glm::radians(cameraRot.z)) * m_CameraTranslationSpeed * dt;
	}
	else if (Daybreak::Input::IsKeyPressed(Daybreak::Key::D))
	{
		cameraPos.x += cos(glm::radians(cameraRot.z)) * m_CameraTranslationSpeed * dt;
		cameraPos.y += sin(glm::radians(cameraRot.z)) * m_CameraTranslationSpeed * dt;
	}

	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::W))
	{
		cameraPos.x += -sin(glm::radians(cameraRot.z)) * m_CameraTranslationSpeed * dt;
		cameraPos.y += cos(glm::radians(cameraRot.z)) * m_CameraTranslationSpeed * dt;
	}
	else if (Daybreak::Input::IsKeyPressed(Daybreak::Key::S))
	{
		cameraPos.x -= -sin(glm::radians(cameraRot.z)) * m_CameraTranslationSpeed * dt;
		cameraPos.y -= cos(glm::radians(cameraRot.z)) * m_CameraTranslationSpeed * dt;
	}

	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::Q))
	{
		cameraPos.z += m_CameraTranslationSpeed * dt;
	}
	else if (Daybreak::Input::IsKeyPressed(Daybreak::Key::E))
	{
		cameraPos.z -= m_CameraTranslationSpeed * dt;
	}

	float m_CameraRotationSpeed = 150.0f;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::D1))
		cameraRot.z += m_CameraRotationSpeed * dt;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::D3))
		cameraRot.z -= m_CameraRotationSpeed * dt;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::D2))
		cameraRot.z = 0.0f;
}

void CameraController::UpdateView()
{
	glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(cameraRot.z), { 0.0f,0.0f,1.0f });
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), cameraPos);
	m_View = rotate * translate;
}