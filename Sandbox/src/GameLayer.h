#pragma once

#include <Daybreak.h>
#include "CameraController.h"
//#include "DrawableObjects.h"

class GameLayer : public Daybreak::Layer
{
public:
	GameLayer();

	virtual void OnUpdate(Daybreak::DeltaTime dt) override;
	virtual void OnEvent(Daybreak::Event& event) override;
	virtual void OnImGuiRender() override;
	//void RenderTest();
	//void RenderTest2();

private:
	Daybreak::Ref<Daybreak::Texture2D> texture1;
	Daybreak::Ref<Daybreak::Texture2D> texture2;
	Daybreak::Ref<Daybreak::Texture2D> tileTexture;


	//std::vector<DrawableObject> ObjectsList;

	CameraController m_CameraController;

	float gridDepth = 0.0f;

	glm::vec3 obj1Pos = glm::vec3(0.0f);
	glm::vec3 obj1Rot = glm::vec3(0.0f);
	glm::vec3 obj1Scale = glm::vec3(10.0f);

	float zrot = 0.0f;

	bool vsyncBox = true;

};