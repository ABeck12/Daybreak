#include "StressTesting.h"
#include <imgui.h>
#include <glad/glad.h>

#include <cstdlib>

StressTestLayer::StressTestLayer()
{
	m_Scene = Daybreak::CreateRef<Daybreak::Scene>();
	Daybreak::SceneSerializer serializer(m_Scene);
	serializer.Deserialize(Daybreak::AssetManager::Get()->GetAssetDir() / "scenes/SceneLayer.scene");

	Daybreak::Application::Get().GetWindow().SetVSync(false);

	srand(1);

	for (int i = 0; i < 750; i++)
	{
		DB_LOG("Spawning entity {}", i);
		Daybreak::Entity entity = m_Scene->CreateEntity(std::to_string(i));
		auto& rb = entity.AddComponent<Daybreak::Rigidbody2DComponent>();
		rb.Type = Daybreak::Rigidbody2DComponent::BodyType::Dynamic;
		rb.AllowSleep = false;
		auto& sr = entity.AddComponent<Daybreak::SpriteRendererComponent>();
		// float randColor = rand();
		float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		float g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		float b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		sr.TintColor = { r, g, b, 1 };
		if (i % 2 == 1)
		{
			entity.AddComponent<Daybreak::BoxCollider2DComponent>();
		}
		else
		{
			entity.AddComponent<Daybreak::CircleCollider2DComponent>();
		}
	}
	// Daybreak::FrameBufferSpecifications spec;
	// spec.Height = 720;
	// spec.Width = 1280;
	// spec.AttachmentTypes = {
	// 	Daybreak::FrameBufferAttachmentTypes::RGBA,
	// 	// Daybreak::FrameBufferAttachmentTypes::RGBA,
	// 	Daybreak::FrameBufferAttachmentTypes::RedInteger,
	// 	Daybreak::FrameBufferAttachmentTypes::Depth,
	// };
	// m_FB = Daybreak::FrameBuffer::Create(spec);
	// // Daybreak::RenderCommand::SetDepthTesting(false);

	// m_FBShader = Daybreak::AssetManager::Get()->LoadShader("shaders/Renderer2D_DefaultFrameBufferDraw.glsl");
}

void StressTestLayer::OnAttach()
{
	m_Scene->OnRuntimeStart();
}

void StressTestLayer::OnDetach()
{
	m_Scene->OnRuntimeStop();
}

void StressTestLayer::OnUpdate(Daybreak::DeltaTime dt)
{
	// m_FB->Bind();
	Daybreak::RenderCommand::Clear();
	Daybreak::RenderCommand::SetClearColor({ -1, 0, 0, 1 });
	// uint32_t id = m_FB->GetAttachmentRendererID(1);
	// glDrawBuffer(GL_COLOR_ATTACHMENT1);
	// Daybreak::RenderCommand::SetClearColor({ -1, 0, 0, 1 });
	// Daybreak::RenderCommand::Clear();
	// m_FB->Bind();

	m_Scene->OnRuntimeUpdate(dt);
	// m_FB->Unbind();

	// Daybreak::RenderCommand::Clear();
	// m_FB->BindAttachmentAsTexture(0, 0);
	// Daybreak::Renderer::DrawFrameBuffer(m_FB, m_FBShader);
}

void StressTestLayer::OnEvent(Daybreak::Event& event) {}

void StressTestLayer::OnImGuiRender()
{
	ImGui::Begin("Log");
	const ImGuiIO& io = ImGui::GetIO();
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::End();
}
