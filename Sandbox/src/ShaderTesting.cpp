#include "ShaderTesting.h"

#include <imgui.h>

ShaderTesting::ShaderTesting()
{
	Daybreak::FrameBufferSpecifications spec;
	spec.Width = 1280;
	spec.Height = 720;
	// spec.AttachmentTypes
	m_FrameBuffer = Daybreak::FrameBuffer::Create(spec);
	Daybreak::Application::Get().GetWindow().SetVSync(false);


	m_Shader = Daybreak::AssetManager::Get()->LoadShader("shaders/test_shader_geo.glsl");
}

void ShaderTesting::OnAttach()
{
}

void ShaderTesting::OnDetach()
{
}

void ShaderTesting::OnUpdate(Daybreak::DeltaTime dt)
{
	m_Shader->Bind();
	m_Shader->SetFloat1("u_Time", Daybreak::Time::GetTime());
	Daybreak::RenderCommand::SetClearColor({ 0, 0, 0, 1 });
	Daybreak::RenderCommand::Clear();
	Daybreak::Renderer::DrawFrameBuffer(m_FrameBuffer, m_Shader);
}

void ShaderTesting::OnEvent(Daybreak::Event& event)
{
}

void ShaderTesting::OnImGuiRender()
{
	const ImGuiIO& io = ImGui::GetIO();
	ImGui::Begin("ImGui Layer");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::End();
	// ImGui::Begin("Test");
	// float width = 1280.0;
	// float height = 720.0;
	// ImGui::Image((void*)(intptr_t)m_FrameBuffer->GetAttachmentRendererID(), { width, height }, { 0, 1 }, { 1, 0 });
	// ImGui::End();
}
