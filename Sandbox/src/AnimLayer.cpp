#include <Daybreak.h>

#include "AnimLayer.h"

#include <imgui.h>

AnimLayer::AnimLayer() : Layer("AnimLayer")
{
	m_Scene = Daybreak::CreateRef<Daybreak::Scene>();

	m_CameraEntity = m_Scene->CreateEntity("Camera");
	auto& cameraComp = m_CameraEntity.AddComponent<Daybreak::CameraComponent>();
	cameraComp.Primary = true;
	cameraComp.Camera.SetProjection(glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 75.0f));
	auto& cameraTransformComp = m_CameraEntity.GetComponent<Daybreak::TransformComponent>();
	cameraTransformComp.Position = glm::vec3(0.0f, 0.0f, -10.0f);

	m_AnimatedEntity = m_Scene->CreateEntity("AnimatedEntity");
	auto& texture1 = Daybreak::Texture2D::Create({ 3, 3, Daybreak::ImageFormat::RGBA, Daybreak::TextureFilterType::Point }, "../Sandbox/assets/AnimSprite1.png");
	auto& texture2 = Daybreak::Texture2D::Create({ 3, 3, Daybreak::ImageFormat::RGBA, Daybreak::TextureFilterType::Point }, "../Sandbox/assets/AnimSprite2.png");
	auto& texture3 = Daybreak::Texture2D::Create({ 3, 3, Daybreak::ImageFormat::RGBA, Daybreak::TextureFilterType::Point }, "../Sandbox/assets/AnimSprite3.png");
	// Daybreak::Ref<Daybreak::AnimationSource> animSource = Daybreak::CreateRef<Daybreak::AnimationSource>();
	// auto& anim = m_AnimatedEntity.AddComponent<Daybreak::AnimatorComponent>();
	// animSource->AddFrame({ texture1,0.0f });
	// animSource->AddFrame({ texture2,1.0f });
	// animSource->AddFrame({ texture3,2.0f });
	// animSource->AddFrame({ texture1,3.0f });

	// anim.Source = animSource;

	
	// auto& texture = Daybreak::Texture2D::Create({ 3, 3, Daybreak::ImageFormat::RGBA, Daybreak::TextureFilterType::Bilinear }, "../Sandbox/assets/Test.png");
	// auto& sr = m_AnimatedEntity.AddComponent<Daybreak::SpriteRendererComponent>();
	// sr.Sprite = texture;
}

void AnimLayer::OnAttach()
{
	m_Scene->OnRuntimeStart();
}

void AnimLayer::OnDetach()
{
	m_Scene->OnRuntimeEnd();
}

void AnimLayer::OnUpdate(Daybreak::DeltaTime dt)
{
	Daybreak::RenderCommand::Clear();
	Daybreak::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.2f, 1.0f)); // Blue-Gray
	m_Scene->OnRuntimeUpdate(dt);
	// Daybreak::Renderer2D::BeginScene(m_CameraEntity.GetComponent<Daybreak::CameraComponent>().Camera, m_CameraEntity.GetComponent<Daybreak::TransformComponent>().GetTransform());
	auto& texturetemp = Daybreak::Texture2D::Create({ 0, 0, Daybreak::ImageFormat::RGBA, Daybreak::TextureFilterType::Point }, "../Sandbox/assets/adventurer-Sheet.png");
	auto subtexture = Daybreak::SubTexture2D::Create(texturetemp, { 0, 0 }, { 50, 37 });

	// Daybreak::Renderer2D::DrawQuad({0,0},{10,10}, subtexture);
	Daybreak::Renderer2D::DrawRotatedQuad({0,0},{10,10}, 45.0f,subtexture);
	// Daybreak::Renderer2D::DrawQuad(m_AnimatedEntity.GetComponent<Daybreak::TransformComponent>().GetTransform(), subtexture);
	Daybreak::Renderer2D::EndScene();
}

void AnimLayer::OnEvent(Daybreak::Event& event)
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
			m_CameraEntity.GetComponent<Daybreak::CameraComponent>().Camera.SetProjection(glm::perspective(glm::radians(45.0f), (float)e.GetWidth() / (float)e.GetHeight(), 0.0f, 100.0f));
	}

}

void AnimLayer::OnImGuiRender()
{
	
}
