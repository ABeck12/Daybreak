#include "SceneLayer.h"

#include <Daybreak.h>

SceneLayer::SceneLayer() : Layer("SceneLayer")
{
	m_Scene = Daybreak::CreateRef<Daybreak::Scene>();

	entityTest = m_Scene->CreateEntity("Entity Test");
	auto& texture = Daybreak::Texture2D::Create({ 3, 3, Daybreak::ImageFormat::RGBA, Daybreak::TextureFilterType::Bilinear }, "../Resources/DaybreakLogo.png");
	auto& sr = entityTest.AddComponent<Daybreak::SpriteRendererComponent>();
	sr.Sprite = texture;
	
	cameraEntity = m_Scene->CreateEntity("Camera");
	auto& cameraComp = cameraEntity.AddComponent<Daybreak::CameraComponent>();
	cameraComp.Primary = true;
	cameraComp.Camera.SetProjection(glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 75.0f));
	auto& cameraTransformComp = cameraEntity.GetComponent<Daybreak::TransformComponent>();
	cameraTransformComp.Position = glm::vec3(0.0f, 0.0f, -10.0f);

	lastMousePos = Daybreak::Input::GetMousePosition();
}

void SceneLayer::OnUpdate(Daybreak::DeltaTime dt)
{
	Daybreak::RenderCommand::Clear();
	Daybreak::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.2f, 1.0f)); // Blue-Gray

	// entityTest Update
	auto& entityTestPos = entityTest.GetComponent<Daybreak::TransformComponent>().Position;
	float amount = 10.0f * dt;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::W))
		entityTestPos.y += amount;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::S))
		entityTestPos.y -= amount;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::D))
		entityTestPos.x += amount;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::A))
		entityTestPos.x -= amount;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::E))
		entityTestPos.z += amount;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::Q))
		entityTestPos.z -= amount;

	
	// Camera Update
	auto& cameraPos = cameraEntity.GetComponent<Daybreak::TransformComponent>().Position;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::K))
		cameraPos.y += amount;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::I))
		cameraPos.y -= amount;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::J))
		cameraPos.x += amount;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::L))
		cameraPos.x -= amount;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::O))
		cameraPos.z += amount;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::U))
		cameraPos.z -= amount;
	auto& cameraRot = cameraEntity.GetComponent<Daybreak::TransformComponent>().Rotation;
	float amountRot = 10.0f * dt;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::D7))
		cameraRot.z += amountRot;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::D9))
		cameraRot.z -= amountRot;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::D8))
		cameraRot.z = 0.0f;
	


	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::P))
	{
		cameraPos = glm::vec3(0.0f,0.0f,-10.0f);
		entityTestPos = glm::vec3(0.0f);
	}
	//MoveCamera(entityTest);
	//MoveCamera(cameraEntity, dt);

	m_Scene->OnSceneUpdate(dt);
}

void SceneLayer::OnEvent(Daybreak::Event& event)
{
	if (event.GetEventType() == Daybreak::EventType::KeyPressed)
	{
		Daybreak::KeyPressedEvent& e = (Daybreak::KeyPressedEvent&)event;
		if (e.GetKeyCode() == Daybreak::Key::Escape)
		{
			Daybreak::Application::Get().Close();
		}
	}
	if (event.GetEventType() == Daybreak::EventType::WindowResize)
	{
		Daybreak::WindowResizeEvent& e = (Daybreak::WindowResizeEvent&)event;
		if (e.GetWidth() != 0 && e.GetHeight() != 0) //TEMPORARY!!!
			cameraEntity.GetComponent<Daybreak::CameraComponent>().Camera.SetProjection(glm::perspective(glm::radians(45.0f), (float)e.GetWidth() / (float)e.GetHeight(), 0.0f, 100.0f));
	}
}

void SceneLayer::OnImGuiRender()
{
	
}

void SceneLayer::MoveCamera(Daybreak::Entity& entity, Daybreak::DeltaTime dt)
{
	float rotationSensitivity = 100.0f;
	float translationAmount = 10.0f * dt;
	glm::vec2 mousePos = Daybreak::Input::GetMousePosition();
	auto& transform = entity.GetComponent<Daybreak::TransformComponent>();
	//transform.Rotation.x += (mousePos.x - lastMousePos.x) / rotationSensitivity;
	//transform.Rotation.y += (-mousePos.y + lastMousePos.y) / rotationSensitivity;

	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::A))
	{
		transform.Position.x -= cos(glm::radians(transform.Rotation.z)) * translationAmount;
		transform.Position.y -= sin(glm::radians(transform.Rotation.z)) * translationAmount;
	}
	else if (Daybreak::Input::IsKeyPressed(Daybreak::Key::D))
	{
		transform.Position.x += cos(glm::radians(transform.Rotation.z)) * translationAmount;
		transform.Position.y += sin(glm::radians(transform.Rotation.z)) * translationAmount;
	}

	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::W))
	{
		transform.Position.x += -sin(glm::radians(transform.Rotation.z)) * translationAmount;
		transform.Position.y += cos(glm::radians(transform.Rotation.z)) * translationAmount;
	}
	else if (Daybreak::Input::IsKeyPressed(Daybreak::Key::S))
	{
		transform.Position.x -= -sin(glm::radians(transform.Rotation.z)) * translationAmount;
		transform.Position.y -= cos(glm::radians(transform.Rotation.z)) * translationAmount;
	}

	//if (Daybreak::Input::IsKeyPressed(Daybreak::Key::W))
	//	transform.Position.y += translationAmount;
	//if (Daybreak::Input::IsKeyPressed(Daybreak::Key::S))
	//	transform.Position.y -= translationAmount;
	//if (Daybreak::Input::IsKeyPressed(Daybreak::Key::D))
	//	transform.Position.x += translationAmount;
	//if (Daybreak::Input::IsKeyPressed(Daybreak::Key::A))
	//	transform.Position.x -= translationAmount;
	
	//if (Daybreak::Input::IsKeyPressed(Daybreak::Key::E))
	//	entity.z += translationAmount;
	//if (Daybreak::Input::IsKeyPressed(Daybreak::Key::Q))
	//	entity.z -= translationAmount;

	lastMousePos = mousePos;
}
