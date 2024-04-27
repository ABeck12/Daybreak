#include "LightingScene.h"

LightingScene::LightingScene()
{
	m_Scene = Daybreak::CreateRef<Daybreak::Scene>();
	Daybreak::SceneSerializer serializer(m_Scene);
	serializer.Deserialize(Daybreak::AssetManager::Get()->GetAssetDir() / "scenes/SceneLayer.scene");
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

	Daybreak::Renderer2D::DrawCircle({ 0, 1 }, 1.0f, { 0, 1, 0, 1 }, 0.1f, 0.1f);
	Daybreak::Renderer2D::EndScene();
}

void LightingScene::OnEvent(Daybreak::Event& event)
{
}

void LightingScene::OnImGuiRender()
{
}