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
	void MoveCamera(Daybreak::Entity& entity, Daybreak::DeltaTime dt);
private:
	glm::vec2 lastMousePos = glm::vec2(1.0f);
	Daybreak::Ref<Daybreak::Scene> m_Scene;

	Daybreak::Entity entityTest;
	Daybreak::Entity cameraEntity;
};
