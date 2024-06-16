#include "LightingScene.h"
#include <imgui.h>

LightingScene::LightingScene()
{
	// Daybreak::Application::Get().GetWindow().SetVSync(false);
	m_Scene = Daybreak::CreateRef<Daybreak::Scene>();
	Daybreak::SceneSerializer serializer(m_Scene);
	serializer.Deserialize(Daybreak::AssetManager::Get()->GetAssetDir() / "scenes/SceneLayer.scene");
	Daybreak::Entity playerEntity = m_Scene->GetEntityByName("Player");

	auto& pl = playerEntity.AddComponent<Daybreak::PointLight2DComponent>();


	Daybreak::Entity parent = m_Scene->CreateEntity("parent");
	auto& parentTr = parent.GetComponent<Daybreak::TransformComponent>();
	parentTr.Position.x = -3;
	auto& parentRb = parent.AddComponent<Daybreak::Rigidbody2DComponent>();
	parentRb.Type = Daybreak::Rigidbody2DComponent::BodyType::Dynamic;
	auto& parentBc = parent.AddComponent<Daybreak::BoxCollider2DComponent>();
	auto& parentSr = parent.AddComponent<Daybreak::SpriteRendererComponent>();
	parentSr.TintColor = { 0, 0, 1, 1 };
	auto& parentPl = parent.AddComponent<Daybreak::PointLight2DComponent>();
	parentPl.Color = { 0.0f, 1.0f, 0.0f };
	parentPl.OuterRadius = 2.0f;
	parentPl.InnerRadius = 0.0f;

	// parentPl.Intensity = 10;
	Daybreak::Entity child = m_Scene->CreateEntity(parent, "child");
	auto& childBc = child.AddComponent<Daybreak::BoxCollider2DComponent>();
	auto& childSr = child.AddComponent<Daybreak::SpriteRendererComponent>();
	childSr.TintColor = { 0, 0, 1, 1 };
	auto& childTr = child.GetComponent<Daybreak::TransformComponent>();
	childTr.Position = { 1, 0, 0 };

	Daybreak::Entity child2 = m_Scene->CreateEntity(child, "child2");
	auto& child2Cc = child2.AddComponent<Daybreak::CircleCollider2DComponent>();
	child2Cc.Radius = sqrt(2) / 2;
	auto& child2Tr = child2.GetComponent<Daybreak::TransformComponent>();
	child2Tr.Position = { -1, 1, 0 };


	Daybreak::Entity child3 = m_Scene->CreateEntity(child2, "child3");
	auto& child3Pc = child3.AddComponent<Daybreak::PolygonCollider2DComponent>();
	child3Pc.Count = 3;
	auto& child3Tr = child3.GetComponent<Daybreak::TransformComponent>();
	child3Tr.Position = { -1, 1, 0 };
}

void LightingScene::OnAttach()
{
	m_Scene->OnRuntimeStart();
}

void LightingScene::OnDetach()
{
	m_Scene->OnRuntimeStop();
}

void LightingScene::OnUpdate(Daybreak::DeltaTime dt)
{
	Daybreak::RenderCommand::Clear();
	Daybreak::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.2f, 1.0f)); // Blue-Gray

	m_Scene->OnRuntimeUpdate(dt);

	auto sc = m_Scene->GetEntityByName("Player").GetComponent<Daybreak::ScriptComponent>().Instance;
	// auto font = Daybreak::Font::GetDefault();
	glm::vec3 coords[4] = { { 0.0f, -3.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } };
	// Daybreak::Renderer2D::DrawQuad(coords[0], coords[1], coords[2], coords[3]);
	// Daybreak::Renderer2D::DrawString("STRING of te\nxt 1234;fds>?'", font, { -3, 3, 0 }, { 1, 1 });
	// Daybreak::Renderer2D::DrawString("text", font, { 0, 0, 0 }, { 10, 10 }, { 0, 1, 0, 1 });

	// Daybreak::Renderer2D::DrawQuad({ -1, -3 }, { 10, 10 }, font->GetAtlasTexture());
	// Daybreak::Renderer2D::DrawCircle({ 0, 1 }, 1.0f, { 0, 1, 0, 1 }, 0.1f, 0.1f);
	Daybreak::Renderer2D::EndScene();
}

void LightingScene::OnEvent(Daybreak::Event& event) {}

void LightingScene::OnImGuiRender()
{
	const ImGuiIO& io = ImGui::GetIO();
	// ImGui::Begin("ImGui Layer");
	// ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	// // ImGui::InputFloat3("Player Scale", &(m_Scene->GetEntityByName("Player").GetComponent<Daybreak::TransformComponent>().Scale.x));
	// // ImGui::InputFloat3("Child Scale", &(m_Scene->GetEntityByName("child").GetComponent<Daybreak::TransformComponent>().Scale.x));
	// ImGui::DragFloat("Inner Radius", &(m_Scene->GetEntityByName("Player").GetComponent<Daybreak::PointLight2DComponent>().InnerRadius), 0.1, 0.0, 10.0);
	// ImGui::DragFloat("Outer Radius", &(m_Scene->GetEntityByName("Player").GetComponent<Daybreak::PointLight2DComponent>().OuterRadius), 0.1, 0.0, 10.0);
	// ImGui::DragFloat("Intensity", &(m_Scene->GetEntityByName("Player").GetComponent<Daybreak::PointLight2DComponent>().Intensity), 0.1, 0.0, 10.0);
	// // ImGui::DragFloat("Falloff", &(m_Scene->GetEntityByName("Player").GetComponent<Daybreak::PointLight2DComponent>().Falloff), 0.1, 0.0, 10.0);
	// ImGui::ColorEdit3("Color", &(m_Scene->GetEntityByName("Player").GetComponent<Daybreak::PointLight2DComponent>().Color.r));
	// ImGui::End();
	// float height = 720.0f / 1;
	// float width = 1280.0f / 1;
	// ImGui::Begin("Image0");
	// ImGui::Image((void*)(intptr_t)(m_FB->GetAttachmentRendererID(0)), { width, height }, { 0, 1 }, { 1, 0 });
	// // ImGui::End();

	// // ImGui::Begin("Image1");
	// ImGui::Image((void*)(intptr_t)(m_FB->GetAttachmentRendererID(1)), { width, height }, { 0, 1 }, { 1, 0 });
	// // ImGui::End();

	// // ImGui::Begin("Image2");
	// ImGui::Image((void*)(intptr_t)(m_FB->GetAttachmentRendererID(2)), { width, height }, { 0, 1 }, { 1, 0 });
	// ImGui::End();

	// ImGui::Begin("Rotation");
	// ImGui::InputFloat2("Parent Position", &(m_Scene->GetEntityByName("parent").GetComponent<Daybreak::TransformComponent>().Position.x));
	// ImGui::InputFloat2("Parent Scale", &(m_Scene->GetEntityByName("parent").GetComponent<Daybreak::TransformComponent>().Scale.x));
	// ImGui::InputFloat("Parent Rotation", &(m_Scene->GetEntityByName("parent").GetComponent<Daybreak::TransformComponent>().Rotation.z));

	// ImGui::End();
	/*
		ImGui::Begin("Gamepad Testing");
		Daybreak::Gamepad gamepad = Daybreak::Gamepad::Gamepad1;
		if (Daybreak::Input::IsGamepadConnected(gamepad))
		{
			std::string name = Daybreak::Input::GetGamepadName(gamepad);
			float leftx = Daybreak::Input::GetGamepadAxis(gamepad, Daybreak::GamepadAxis::LeftX);
			float lefty = Daybreak::Input::GetGamepadAxis(gamepad, Daybreak::GamepadAxis::LeftY);
			float rightx = Daybreak::Input::GetGamepadAxis(gamepad, Daybreak::GamepadAxis::RightX);
			float righty = Daybreak::Input::GetGamepadAxis(gamepad, Daybreak::GamepadAxis::RightY);
			float triggerRight = Daybreak::Input::GetGamepadAxis(gamepad, Daybreak::GamepadAxis::RightTrigger);
			float triggerLeft = Daybreak::Input::GetGamepadAxis(gamepad, Daybreak::GamepadAxis::LeftTrigger);
			glm::vec2 leftAxis = { leftx, lefty };
			glm::vec2 rightAxis = { rightx, righty };

			bool buttonUp = Daybreak::Input::IsGamepadButtonPressed(gamepad, Daybreak::GamepadButton::ButtonUp);
			bool buttonDown = Daybreak::Input::IsGamepadButtonPressed(gamepad, Daybreak::GamepadButton::ButtonDown);
			bool buttonLeft = Daybreak::Input::IsGamepadButtonPressed(gamepad, Daybreak::GamepadButton::ButtonLeft);
			bool buttonRight = Daybreak::Input::IsGamepadButtonPressed(gamepad, Daybreak::GamepadButton::ButtonRight);

			bool dpadUp = Daybreak::Input::IsGamepadButtonPressed(gamepad, Daybreak::GamepadButton::DPadUp);
			bool dpadDown = Daybreak::Input::IsGamepadButtonPressed(gamepad, Daybreak::GamepadButton::DPadDown);
			bool dpadLeft = Daybreak::Input::IsGamepadButtonPressed(gamepad, Daybreak::GamepadButton::DPadLeft);
			bool dpadRight = Daybreak::Input::IsGamepadButtonPressed(gamepad, Daybreak::GamepadButton::DPadRight);

			bool start = Daybreak::Input::IsGamepadButtonPressed(gamepad, Daybreak::GamepadButton::Start);
			bool select = Daybreak::Input::IsGamepadButtonPressed(gamepad, Daybreak::GamepadButton::Select);
			// bool menu = Daybreak::Input::IsGamepadButtonPressed(gamepad, Daybreak::GamepadButton::Menu);

			bool leftBumper = Daybreak::Input::IsGamepadButtonPressed(gamepad, Daybreak::GamepadButton::LeftBumper);
			bool rightBumper = Daybreak::Input::IsGamepadButtonPressed(gamepad, Daybreak::GamepadButton::RightBumper);

			bool leftStickIn = Daybreak::Input::IsGamepadButtonPressed(gamepad, Daybreak::GamepadButton::LeftStickIn);
			bool rightStickIn = Daybreak::Input::IsGamepadButtonPressed(gamepad, Daybreak::GamepadButton::RightStickIn);

			ImGui::Text(name.c_str());
			ImGui::Separator();
			ImGui::InputFloat2("Left Axis", &leftAxis.x);
			ImGui::InputFloat2("Right Axis", &rightAxis.x);
			ImGui::InputFloat("Right Trigger", &triggerRight);
			ImGui::InputFloat("Left Trigger", &triggerLeft);
			ImGui::Separator();
			ImGui::Checkbox("Button Up", &buttonUp);
			ImGui::Checkbox("Button Down", &buttonDown);
			ImGui::Checkbox("Button Left", &buttonLeft);
			ImGui::Checkbox("Button Right", &buttonRight);
			ImGui::Separator();
			ImGui::Checkbox("DPad Up", &dpadUp);
			ImGui::Checkbox("DPad Down", &dpadDown);
			ImGui::Checkbox("DPad Left", &dpadLeft);
			ImGui::Checkbox("DPad Right", &dpadRight);
			ImGui::Separator();
			ImGui::Checkbox("Start", &start);
			ImGui::Checkbox("Select", &select);
			// ImGui::Checkbox("Menu", &menu);
			ImGui::Separator();
			ImGui::Checkbox("Left Bumper", &leftBumper);
			ImGui::Checkbox("Right Bumper", &rightBumper);
			ImGui::Separator();
			ImGui::Checkbox("Left Stick In", &leftStickIn);
			ImGui::Checkbox("Right Stick In", &rightStickIn);
		}
		ImGui::End();
	*/
}
