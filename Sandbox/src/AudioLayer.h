#pragma once

#include <Daybreak.h>

class AudioLayer : public Daybreak::Layer
{
public:
	AudioLayer();

	virtual void OnUpdate(Daybreak::DeltaTime dt) override;
	virtual void OnEvent(Daybreak::Event& event) override;
	virtual void OnImGuiRender() override;

private:

};