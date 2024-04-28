#pragma once

#include <Daybreak.h>


class LightingScene : public Daybreak::Layer
{
public:
	LightingScene();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Daybreak::DeltaTime dt) override;
	virtual void OnEvent(Daybreak::Event& event) override;
	virtual void OnImGuiRender() override;

private:
	Daybreak::Ref<Daybreak::Scene> m_Scene;
};
