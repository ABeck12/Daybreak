#pragma once

#include <Daybreak.h>
#include "CameraController.h"

class GameLayer : public Daybreak::Layer
{
public:
	GameLayer();

	void OnUpdate() override;
	void OnEvent(Daybreak::Event& event) override;
	void RenderTest();
	void RenderTest2();

private:
	Daybreak::Ref<Daybreak::VertexArray> va;
	Daybreak::Ref<Daybreak::VertexBuffer> vb;
	Daybreak::Ref<Daybreak::IndexBuffer> ib;
	Daybreak::Ref<Daybreak::Shader> shader;
	Daybreak::Ref<Daybreak::Texture2D> texture;
	glm::mat4 mvp;

	glm::vec3 obj1Pos = glm::vec3(0.0f);
	glm::vec3 obj2Pos = glm::vec3(2.5f,1.0f, -5.0f);

	glm::vec3 obj1Rot = glm::vec3(0.0f);
	glm::vec3 obj2Rot = glm::vec3(0.0f);

	glm::vec3 obj1Scale = glm::vec3(10.0f);
	glm::vec3 obj2Scale = glm::vec3(10.0f);

	const glm::mat4 GetModelMat(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale);

	float r_color = 0;
	float g_color = 0;
	float b_color = 0;


	CameraController m_CameraController;

};