#pragma once

#include <Daybreak.h>


class SceneLayer : public Daybreak::Layer
{
public:
	SceneLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Daybreak::DeltaTime dt) override;
	virtual void OnEvent(Daybreak::Event& event) override;
	virtual void OnImGuiRender() override;

private:
	void MoveCamera(Daybreak::Entity& entity, Daybreak::DeltaTime dt);

	void DrawColliders();
	glm::vec3 RotatePoint(glm::vec3 vec, float theta);
private:
	glm::vec2 lastMousePos = glm::vec2(1.0f);
	Daybreak::Ref<Daybreak::Scene> m_Scene;
	Daybreak::Ref<Daybreak::FrameBuffer> m_FrameBuffer;

	Daybreak::Entity playerEntity;
	Daybreak::Entity floorEntity;
	Daybreak::Entity cameraEntity;
};
