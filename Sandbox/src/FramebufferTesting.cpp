#include "FramebufferTesting.h"
#include <imgui.h>
#include <glad/glad.h>

FBTesting::FBTesting()
{
	m_Scene = Daybreak::CreateRef<Daybreak::Scene>();
	Daybreak::SceneSerializer serializer(m_Scene);
	serializer.Deserialize(Daybreak::AssetManager::Get()->GetAssetDir() / "scenes/SceneLayer.scene");

	Daybreak::FrameBufferSpecifications spec;
	spec.Height = 720;
	spec.Width = 1280;
	spec.AttachmentTypes = {
		Daybreak::FrameBufferAttachmentTypes::RGBA,
		// Daybreak::FrameBufferAttachmentTypes::RGBA,
		Daybreak::FrameBufferAttachmentTypes::RedInteger,
		Daybreak::FrameBufferAttachmentTypes::Depth,
	};
	m_FB = Daybreak::FrameBuffer::Create(spec);
	// Daybreak::RenderCommand::SetDepthTesting(false);

	m_FBShader = Daybreak::AssetManager::Get()->LoadShader("shaders/Renderer2D_DefaultFrameBufferDraw.glsl");
}

void FBTesting::OnAttach()
{
	m_Scene->OnRuntimeStart();
}

void FBTesting::OnDetach()
{
	m_Scene->OnRuntimeStop();
}

void FBTesting::OnUpdate(Daybreak::DeltaTime dt)
{
	m_FB->Bind();
	Daybreak::RenderCommand::Clear();
	Daybreak::RenderCommand::SetClearColor({ -1, 0, 0, 1 });
	// uint32_t id = m_FB->GetAttachmentRendererID(1);
	// glDrawBuffer(GL_COLOR_ATTACHMENT1);
	// Daybreak::RenderCommand::SetClearColor({ -1, 0, 0, 1 });
	// Daybreak::RenderCommand::Clear();
	m_FB->Bind();

	// m_FB->Bind();
	// int value[] = { -1, -1 - 1 - 1 };
	// glClearBufferiv(m_FB->GetAttachmentRendererID(1), GL_COLOR_ATTACHMENT1, &value);
	// glClearBufferiv(GL_COLOR, 1, value);


	m_Scene->OnRuntimeUpdate(dt);
	m_FB->Unbind();

	Daybreak::RenderCommand::Clear();
	m_FB->BindAttachmentAsTexture(0, 0);
	Daybreak::Renderer::DrawFrameBuffer(m_FB, m_FBShader);
}

void FBTesting::OnEvent(Daybreak::Event& event) {}

void FBTesting::OnImGuiRender()
{
	float width = 1280;
	float height = 720;
	// ImGui::Begin("Image0");
	// for (int i = 0; i < m_FB->GetSpecification().AttachmentTypes.size(); i++)
	// {
	// 	ImGui::Text("%i", i);
	// 	ImGui::Image((void*)(intptr_t)(m_FB->GetAttachmentRendererID(i)), { width, height }, { 0, 1 }, { 1, 0 });
	// }
	// ImGui::End();
	// return;

	ImGui::Begin("Out");
	m_FB->Bind();
	glm::vec2 mousePos = Daybreak::Input::GetMousePosition();
	int val = m_FB->ReadPixel1I(1, int(mousePos.x), 720 - int(mousePos.y));
	// glm::vec4 val2;
	// val2 = m_FB->ReadPixel4F(0, int(mousePos.x), int(mousePos.y));
	m_FB->Unbind();

	ImGui::InputInt("Entity ID", &val);
	// ImGui::InputFloat4("Color", &val2.r);
	// DB_LOG(val);
	Daybreak::Entity entity;
	std::string name = "";
	if (val > 5)
	{
		DB_LOG(val);
	}
	if (val >= 0 && val < 5)
	{
		entity = { (entt::entity)val, m_Scene.get() };
		if (entity.HasComponent<Daybreak::TagComponent>())
		{
			name = entity.GetName();
		}
	}
	ImGui::Text("%s", name.c_str());
	ImGui::End();

	// // ImGui::Begin("Image1");
	// ImGui::Image((void*)(intptr_t)(m_FB->GetAttachmentRendererID(1)), { width, height }, { 0, 1 }, { 1, 0 });
	// // ImGui::End();

	// // ImGui::Begin("Image2");
	// ImGui::Image((void*)(intptr_t)(m_FB->GetAttachmentRendererID(2)), { width, height }, { 0, 1 }, { 1, 0 });
	// ImGui::End();
}
