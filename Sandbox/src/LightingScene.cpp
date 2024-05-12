#include "LightingScene.h"
#include <imgui.h>

#include "Daybreak/Renderer/Font.h"

LightingScene::LightingScene()
{
	// Daybreak::Application::Get().GetWindow().SetVSync(false);
	m_Scene = Daybreak::CreateRef<Daybreak::Scene>();
	Daybreak::SceneSerializer serializer(m_Scene);
	serializer.Deserialize(Daybreak::AssetManager::Get()->GetAssetDir() / "scenes/SceneLayer.scene");


	Daybreak::Entity parent = m_Scene->CreateEntity("parent");
	auto& parentRb = parent.AddComponent<Daybreak::Rigidbody2DComponent>();
	parentRb.Type = Daybreak::Rigidbody2DComponent::BodyType::Dynamic;
	auto& parentBc = parent.AddComponent<Daybreak::BoxCollider2DComponent>();
	auto& parentSr = parent.AddComponent<Daybreak::SpriteRendererComponent>();
	parentSr.TintColor = { 1, 0, 0, 1 };

	// Daybreak::Entity child = m_Scene->CreateEntity(parent, "child");
	// // auto& childRb = child.AddComponent<Daybreak::Rigidbody2DComponent>();
	// // childRb.Type = Daybreak::Rigidbody2DComponent::BodyType::Dynamic;
	// auto& childBc = child.AddComponent<Daybreak::BoxCollider2DComponent>();
	// auto& childSr = child.AddComponent<Daybreak::SpriteRendererComponent>();
	// childSr.TintColor = { 0, 0, 1, 1 };
	// auto& childTr = child.GetComponent<Daybreak::TransformComponent>();
	// childTr.Position = { 1, 1, 0 };

	// Daybreak::Entity child2 = m_Scene->CreateEntity(child, "child2");
	// // auto& childRb = child.AddComponent<Daybreak::Rigidbody2DComponent>();
	// // childRb.Type = Daybreak::Rigidbody2DComponent::BodyType::Dynamic;
	// auto& child2Bc = child2.AddComponent<Daybreak::BoxCollider2DComponent>();
	// auto& child2Sr = child2.AddComponent<Daybreak::SpriteRendererComponent>();
	// child2Sr.TintColor = { 0, 1, 0, 1 };
	// auto& child2Tr = child2.GetComponent<Daybreak::TransformComponent>();
	// child2Tr.Position = { -1, 1, 0 };


	Daybreak::Entity child3 = m_Scene->CreateEntity(parent, "child3");
	// auto& childRb = child.AddComponent<Daybreak::Rigidbody2DComponent>();
	// childRb.Type = Daybreak::Rigidbody2DComponent::BodyType::Dynamic;
	// auto& child3Bc = child3.AddComponent<Daybreak::BoxCollider2DComponent>();
	auto& child3Cc = child3.AddComponent<Daybreak::CircleCollider2DComponent>();
	auto& child3Sr = child3.AddComponent<Daybreak::SpriteRendererComponent>();
	child3Sr.TintColor = { 1, 1, 0, 1 };
	auto& child3Tr = child3.GetComponent<Daybreak::TransformComponent>();
	child3Tr.Position = { -1, 1, 0 };

	// auto playerEntity = m_Scene->GetEntityByName("Player");
	// Daybreak::Entity playerChild = m_Scene->CreateEntity(playerEntity, "PlayerChild");
	// auto& playerChildTr = playerChild.GetComponent<Daybreak::TransformComponent>();
	// playerChildTr.Position = { 0, 1, 0 };
	// auto& playerChildSr = playerChild.AddComponent<Daybreak::SpriteRendererComponent>();
	// auto& playerChildBc2d = playerChild.AddComponent<Daybreak::BoxCollider2DComponent>();

	// {
	// 	Daybreak::Entity e = m_Scene->CreateEntity("test");
	// 	auto& transform = e.GetComponent<Daybreak::TransformComponent>();
	// 	transform.Position = { -2, -3, 0 };
	// 	auto& sr = e.AddComponent<Daybreak::SpriteRendererComponent>();
	// 	sr.TintColor = { 1, 1, 1, 0.5 };
	// }

	// {
	// 	Daybreak::Entity e = m_Scene->CreateEntity("test2");
	// 	auto& transform = e.GetComponent<Daybreak::TransformComponent>();
	// 	transform.Position = { -1.5, -3, 0 };
	// 	auto& sr = e.AddComponent<Daybreak::SpriteRendererComponent>();
	// 	sr.TintColor = { 1, 1, 0, 0.5 };
	// }
	// {
	// 	Daybreak::Entity e = m_Scene->CreateEntity("test2");
	// 	auto& transform = e.GetComponent<Daybreak::TransformComponent>();
	// 	transform.Position = { -1, -2.5, -.5 };
	// 	auto& sr = e.AddComponent<Daybreak::SpriteRendererComponent>();
	// 	sr.TintColor = { 1, 0, 0, 0.5 };
	// }
	// {
	// 	Daybreak::Entity e = m_Scene->CreateEntity("test3");
	// 	auto& transform = e.GetComponent<Daybreak::TransformComponent>();
	// 	transform.Position = { 0, 3, 1 };
	// 	// auto& sr = e.AddComponent<Daybreak::SpriteRendererComponent>();
	// 	// sr.TintColor = { 1, 1, 0, 0.5 };
	// }


	// for (int i = 0; i < 2000; i++)
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
	// auto font = Daybreak::Font::GetDefault();
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
	ImGui::Begin("ImGui Layer");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::End();
}
