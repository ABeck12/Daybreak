#pragma once

#include <Daybreak.h>

class AnimLayer : public Daybreak::Layer
{
public:
	AnimLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Daybreak::DeltaTime dt) override;
	virtual void OnEvent(Daybreak::Event& event) override;
	virtual void OnImGuiRender() override;

private:
	Daybreak::Entity m_AnimatedEntity;
	Daybreak::Entity m_CameraEntity;
	Daybreak::Ref<Daybreak::Scene> m_Scene;
};