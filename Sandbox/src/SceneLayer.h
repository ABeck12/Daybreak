#pragma once

#include <Daybreak.h>


class SceneLayer : public Daybreak::Layer
{
public:
	SceneLayer();

	virtual void OnUpdate(Daybreak::DeltaTime dt) override;
	virtual void OnEvent(Daybreak::Event& event) override;
	virtual void OnImGuiRender() override;

private:
	Daybreak::Ref<Daybreak::Scene> m_Scene;

	Daybreak::Entity entityTest;
	Daybreak::Entity cameraEntity;
};
