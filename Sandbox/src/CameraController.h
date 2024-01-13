#pragma once

#include <Daybreak.h>

class CameraController
{
public:
	CameraController() = default;
	CameraController(const glm::mat4 projection);

	void Update(Daybreak::DeltaTime dt);
	const glm::mat4& GetView() const { return m_View; }
	const glm::mat4& GetProj() const { return m_Camera.GetProjection(); }
	void UpdateProj(const glm::mat4& projection) { m_Camera.SetProjection(projection); }
	void SetCameraPosition(const glm::vec3& position) { cameraPos = position; }
	const glm::vec3& GetCameraPosition() const { return cameraPos; }
	const Daybreak::Camera& GetCamera() const { return m_Camera; }
private:
	void UpdateView();
	glm::mat4 m_View;
	Daybreak::Camera m_Camera;
	glm::vec3 cameraPos = glm::vec3(1.0f);
	glm::vec3 cameraRot = glm::vec3(0.0f);
};