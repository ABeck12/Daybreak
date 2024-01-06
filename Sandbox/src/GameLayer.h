#pragma once

#include <Daybreak.h>
#include "Daybreak/Renderer/Camera.h"

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
	Daybreak::Camera camera;
	glm::mat4 mvp;
	glm::vec3 cameraPos = glm::vec3(1.0f);
	glm::vec3 cameraRot = glm::vec3(0.0f);

	float r_color = 0;
	float g_color = 0;
	float b_color = 0;

};