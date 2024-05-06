#include "LightingScene.h"
#include <imgui.h>

#include "Daybreak/Renderer/Font.h"

LightingScene::LightingScene()
{
	m_Scene = Daybreak::CreateRef<Daybreak::Scene>();
	Daybreak::SceneSerializer serializer(m_Scene);
	serializer.Deserialize(Daybreak::AssetManager::Get()->GetAssetDir() / "scenes/SceneLayer.scene");

	{
		Daybreak::Entity e = m_Scene->CreateEntity("test");
		auto& transform = e.GetComponent<Daybreak::TransformComponent>();
		transform.Position = { -2, -3, 0 };
		auto& sr = e.AddComponent<Daybreak::SpriteRendererComponent>();
		sr.TintColor = { 1, 1, 1, 0.5 };
	}

	{
		Daybreak::Entity e = m_Scene->CreateEntity("test2");
		auto& transform = e.GetComponent<Daybreak::TransformComponent>();
		transform.Position = { -1.5, -3, 0 };
		auto& sr = e.AddComponent<Daybreak::SpriteRendererComponent>();
		sr.TintColor = { 1, 1, 0, 0.5 };
	}
	{
		Daybreak::Entity e = m_Scene->CreateEntity("test2");
		auto& transform = e.GetComponent<Daybreak::TransformComponent>();
		transform.Position = { -1, -2.5, -.5 };
		auto& sr = e.AddComponent<Daybreak::SpriteRendererComponent>();
		sr.TintColor = { 1, 0, 0, 0.5 };
	}
	{
		Daybreak::Entity e = m_Scene->CreateEntity("test3");
		auto& transform = e.GetComponent<Daybreak::TransformComponent>();
		transform.Position = { 0, 3, 1 };
		// auto& sr = e.AddComponent<Daybreak::SpriteRendererComponent>();
		// sr.TintColor = { 1, 1, 0, 0.5 };
	}


	// for (int i = 0; i < 5000; i++)
	// {
	// 	Daybreak::Entity e = m_Scene->CreateEntity(std::to_string(i));
	// 	e.AddComponent<Daybreak::SpriteRendererComponent>();
	// }
}

void LightingScene::OnAttach()
{
	m_Scene->OnRuntimeStart();
}

void LightingScene::OnDetach()
{
	m_Scene->OnRuntimeEnd();
}

void LightingScene::OnUpdate(Daybreak::DeltaTime dt)
{
	Daybreak::RenderCommand::Clear();
	Daybreak::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.2f, 1.0f)); // Blue-Gray

	m_Scene->OnRuntimeUpdate(dt);
	auto font = Daybreak::Font::GetDefault();
	Daybreak::Renderer2D::DrawString("STRING of te\nxt 1234;fds>?'", font, { -3, 3, 0 }, { 1, 1 });
	Daybreak::Renderer2D::DrawString("text", font, { 0, 0, 0 }, { 10, 10 }, { 0, 1, 0, 1 });

	// Daybreak::Renderer2D::DrawQuad({ -1, -3 }, { 10, 10 }, font->GetAtlasTexture());
	// Daybreak::Renderer2D::DrawCircle({ 0, 1 }, 1.0f, { 0, 1, 0, 1 }, 0.1f, 0.1f);
	Daybreak::Renderer2D::EndScene();
}

void LightingScene::OnEvent(Daybreak::Event& event) {}

void LightingScene::OnImGuiRender()
{
	const ImGuiIO& io = ImGui::GetIO();
	ImGui::Begin("ImGui Layer");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::End();
}
