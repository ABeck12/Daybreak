#pragma once

#include <Daybreak.h>

class TestLayer : public Daybreak::Layer
{
public:
	TestLayer();

	void OnUpdate() override;
	void OnEvent(Daybreak::Event& event) override;
	void RenderTest();
	void RenderTest2();

private:
	Daybreak::Ref<Daybreak::VertexArray> va;
	Daybreak::Ref<Daybreak::VertexBuffer> vb;
	Daybreak::Ref<Daybreak::IndexBuffer> ib;
	Daybreak::Ref<Daybreak::Shader> shader;
	Daybreak::Ref<Daybreak::Texture> texture;

	float r_color = 0;
	float g_color = 0;
	float b_color = 0;

};