#pragma once

#include <Daybreak.h>


class StressTestLayer : public Daybreak::Layer
{
public:
	StressTestLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Daybreak::DeltaTime dt) override;
	virtual void OnEvent(Daybreak::Event& event) override;
	virtual void OnImGuiRender() override;

private:
	Daybreak::Ref<Daybreak::Scene> m_Scene;
	// Daybreak::Ref<Daybreak::FrameBuffer> m_FB;

	// Daybreak::Ref<Daybreak::Shader> m_FBShader;
};
