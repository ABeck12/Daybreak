#include "RendererTesting.h"
#include <imgui.h>
#include <glad/glad.h>
#include "Daybreak/Renderer/SceneRenderer.h"

RendererTesting::RendererTesting()
{
	m_Scene = Daybreak::CreateRef<Daybreak::Scene>();
	Daybreak::SceneSerializer serializer(m_Scene);
	serializer.Deserialize(Daybreak::AssetManager::Get()->GetAssetDir() / "scenes/SceneLayer.scene");
	Daybreak::Application::Get().GetWindow().SetVSync(false);

	auto test = m_Scene->CreateEntity("Test");
	auto& gl = test.AddComponent<Daybreak::GlobalLight2DComponent>();
	gl.Intensity = 0;
	// gl.Color = { 0, 1, 0 };
	auto& plTest = test.AddComponent<Daybreak::PointLight2DComponent>();
	plTest.Color = { 1, 0, 0 };
	plTest.OuterRadius = 2.5;
	plTest.Intensity = 2;

	auto player = m_Scene->GetEntityByName("Player");
	auto& pl = player.AddComponent<Daybreak::PointLight2DComponent>();
	// pl.Color = { 1, 0, 0 };
	pl.OuterRadius = 5;
	// pl.Intensity = 2;

	// Daybreak::Ref<Daybreak::Shader> computeTest = Daybreak::Shader::Create("test compute", "C:\\dev\\Daybreak\\Sandbox\\assets\\shaders\\testCompute.glsl");
}

void RendererTesting::OnAttach()
{
	m_Scene->OnRuntimeStart();
}

void RendererTesting::OnDetach()
{
	m_Scene->OnRuntimeStop();
}

void RendererTesting::OnUpdate(Daybreak::DeltaTime dt)
{
	// m_FB->Bind();
	Daybreak::RenderCommand::Clear();
	Daybreak::RenderCommand::SetClearColor({ 0, 0, 0, 1 });
	// uint32_t id = m_FB->GetAttachmentRendererID(1);
	// glDrawBuffer(GL_COLOR_ATTACHMENT1);
	// Daybreak::RenderCommand::SetClearColor({ -1, 0, 0, 1 });
	// Daybreak::RenderCommand::Clear();
	// m_FB->Bind();

	// m_FB->Bind();
	// int value[] = { -1, -1 - 1 - 1 };
	// glClearBufferiv(m_FB->GetAttachmentRendererID(1), GL_COLOR_ATTACHMENT1, &value);
	// glClearBufferiv(GL_COLOR, 1, value);


	m_Scene->OnRuntimeUpdate(dt);
	// m_FB->Unbind();

	// Daybreak::RenderCommand::Clear();
	// m_FB->BindAttachmentAsTexture(0, 0);
	// Daybreak::Renderer::DrawFrameBuffer(m_FB, m_FBShader);
}

void RendererTesting::OnEvent(Daybreak::Event& event)
{
	if (event.GetEventType() == Daybreak::EventType::KeyPressed)
	{
		Daybreak::KeyPressedEvent& e = (Daybreak::KeyPressedEvent&)event;
		if (e.GetKeyCode() == Daybreak::Key::Escape)
		{
			Daybreak::Application::Get().Close();
		}
	}
}

void RendererTesting::OnImGuiRender()
{
	const ImGuiIO& io = ImGui::GetIO();
	ImGui::Begin("ImGui Layer");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::End();

	ImGui::Begin("Images");
	float width = 1280;
	float height = 720;
	ImGui::Image((void*)(intptr_t)(m_Scene->m_SceneRenderer->m_BloomBufferFinal->GetAttachmentRendererID()), { width, height }, { 0, 1 }, { 1, 0 });
	ImGui::Image((void*)(intptr_t)(m_Scene->m_SceneRenderer->m_BloomBuffer->GetAttachmentRendererID()), { width, height }, { 0, 1 }, { 1, 0 });
	for (size_t i = 0; i < m_Scene->m_SceneRenderer->m_BloomDownscaleBuffers.size(); i++)
	{
		ImGui::Text("%s", std::to_string(i).c_str());
		width = float(m_Scene->m_SceneRenderer->m_BloomDownscaleBuffers[i]->GetSpecification().Width);
		height = float(m_Scene->m_SceneRenderer->m_BloomDownscaleBuffers[i]->GetSpecification().Height);
		// DB_LOG("width {}, height {}", width, height);
		// ImGui::Image((void*)(intptr_t)(m_Scene->m_SceneRenderer->m_BloomDownscaleBuffers[i]->GetAttachmentRendererID()), { width, height }, { 0, 1 }, { 1, 0 });
		ImGui::Image((void*)(intptr_t)(m_Scene->m_SceneRenderer->m_BloomDownscaleBuffers[i]->GetAttachmentRendererID()), { width, height }, { 0, 1 }, { 1, 0 });
	}
	for (size_t i = 0; i < m_Scene->m_SceneRenderer->m_BloomUpscaleBuffers.size(); i++)
	{
		width = float(m_Scene->m_SceneRenderer->m_BloomDownscaleBuffers[i]->GetSpecification().Width);
		height = float(m_Scene->m_SceneRenderer->m_BloomDownscaleBuffers[i]->GetSpecification().Height);
		ImGui::Text("%s", std::to_string(i).c_str());
		ImGui::Image((void*)(intptr_t)(m_Scene->m_SceneRenderer->m_BloomUpscaleBuffers[i]->GetAttachmentRendererID()), { width, height }, { 0, 1 }, { 1, 0 });
	}
	// ImGui::Image((void*)(intptr_t)(m_Scene->m_SceneRenderer->m_LightBuffer2D->GetAttachmentRendererID()), { width, height }, { 0, 1 }, { 1, 0 });
	// ImGui::Image((void*)(intptr_t)(m_Scene->m_SceneRenderer->m_DrawBuffer2D->GetAttachmentRendererID()), { width, height }, { 0, 1 }, { 1, 0 });
	// ImGui::Image((void*)(intptr_t)(m_Scene->m_SceneRenderer->m_FinalBuffer->GetAttachmentRendererID()), { width, height }, { 0, 1 }, { 1, 0 });
	ImGui::End();

	ImGui::Begin("Final Buffer");
	width = float(m_Scene->m_SceneRenderer->m_FinalBuffer->GetSpecification().Width);
	height = float(m_Scene->m_SceneRenderer->m_FinalBuffer->GetSpecification().Height);
	ImGui::Image((void*)(intptr_t)(m_Scene->m_SceneRenderer->m_FinalBuffer->GetAttachmentRendererID()), { width, height }, { 0, 1 }, { 1, 0 });

	ImGui::End();

	ImGui::Begin("Blur Options");
	ImGui::DragFloat("Color strength", &m_Scene->m_SceneRenderer->m_Strength);
	ImGui::DragFloat("Directions", &m_Scene->m_SceneRenderer->m_BloomBlurDirections);
	ImGui::DragFloat("Quality", &m_Scene->m_SceneRenderer->m_BloomQuality);
	ImGui::DragFloat("Size", &m_Scene->m_SceneRenderer->m_BloomSize);

	ImGui::End();


	// float width = 1280;
	// float height = 720;
	// // ImGui::Begin("Image0");
	// // for (int i = 0; i < m_FB->GetSpecification().AttachmentTypes.size(); i++)
	// // {
	// // 	ImGui::Text("%i", i);
	// // 	ImGui::Image((void*)(intptr_t)(m_FB->GetAttachmentRendererID(i)), { width, height }, { 0, 1 }, { 1, 0 });
	// // }
	// // ImGui::End();
	// // return;

	// ImGui::Begin("Out");
	// m_FB->Bind();
	// glm::vec2 mousePos = Daybreak::Input::GetMousePosition();
	// int val = m_FB->ReadPixel1I(1, int(mousePos.x), 720 - int(mousePos.y));
	// // glm::vec4 val2;
	// // val2 = m_FB->ReadPixel4F(0, int(mousePos.x), int(mousePos.y));
	// m_FB->Unbind();

	// ImGui::InputInt("Entity ID", &val);
	// // ImGui::InputFloat4("Color", &val2.r);
	// // DB_LOG(val);
	// Daybreak::Entity entity;
	// std::string name = "";
	// if (val > 5)
	// {
	// 	DB_LOG(val);
	// }
	// if (val >= 0 && val < 5)
	// {
	// 	entity = { (entt::entity)val, m_Scene.get() };
	// 	if (entity.HasComponent<Daybreak::TagComponent>())
	// 	{
	// 		name = entity.GetName();
	// 	}
	// }
	// ImGui::Text("%s", name.c_str());
	// ImGui::End();

	// // // ImGui::Begin("Image1");
	// // ImGui::Image((void*)(intptr_t)(m_FB->GetAttachmentRendererID(1)), { width, height }, { 0, 1 }, { 1, 0 });
	// // // ImGui::End();

	// // // ImGui::Begin("Image2");
	// // ImGui::Image((void*)(intptr_t)(m_FB->GetAttachmentRendererID(2)), { width, height }, { 0, 1 }, { 1, 0 });
	// // ImGui::End();
}
