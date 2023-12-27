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
};