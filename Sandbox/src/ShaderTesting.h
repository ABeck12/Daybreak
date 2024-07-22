#pragma once

#include <Daybreak.h>


class ShaderTesting : public Daybreak::Layer
{
public:
	ShaderTesting();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Daybreak::DeltaTime dt) override;
	virtual void OnEvent(Daybreak::Event& event) override;
	virtual void OnImGuiRender() override;

private:
	Daybreak::Ref<Daybreak::Shader> m_Shader;
	Daybreak::Ref<Daybreak::FrameBuffer> m_FrameBuffer;
};
