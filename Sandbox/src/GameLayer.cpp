#include <Daybreak.h>

#include <glm/gtc/matrix_transform.hpp>
#include "GameLayer.h"
#include <glad/glad.h> //Temporary for RenderTest() and RenderTest2()

#include <imgui.h>
#include <glm/gtx/string_cast.hpp>


GameLayer::GameLayer() : Layer("GameLayer")
{
	m_CameraController = CameraController(glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 75.0f));
	//m_CameraController = CameraController(glm::ortho(0,1280,0,720));
	m_CameraController.SetCameraPosition(glm::vec3(0.0f, 0.0f, -10.0f));
	//camera = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.0f, 100.0f);


	texture1 = Daybreak::Texture2D::Create({ 128, 128, Daybreak::ImageFormat::RGBA, Daybreak::TextureFilterType::Point }, "../Sandbox/assets/Test.png");
	texture2 = Daybreak::Texture2D::Create({ 128, 128, Daybreak::ImageFormat::RGBA, Daybreak::TextureFilterType::Point }, "../Sandbox/assets/TestTexture.png");
	tileTexture = Daybreak::Texture2D::Create({ 3, 3, Daybreak::ImageFormat::RGBA, Daybreak::TextureFilterType::Point }, "../Sandbox/assets/TileGrid.png");

	
	//DrawableObject gridObject1 = { texture2, glm::vec3(0.0f,0.0f,-0.5f), glm::vec2(1.0f), "object1"};
	//DrawableObject gridObject2 = { texture1, glm::vec3(0.0f), glm::vec2(1.0f), "object2"};
	//ObjectsList.push_back(gridObject1);
	//ObjectsList.push_back(gridObject2);
}

void GameLayer::OnUpdate(Daybreak::DeltaTime dt)
{
	//Daybreak::RenderCommand::SetClearColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)); // Pink
	Daybreak::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.2f, 1.0f)); // Blue-Gray
	Daybreak::RenderCommand::Clear();

	//{
	//	glDisable(GL_DEPTH_TEST);
	//	m_CameraController.UpdateProj(glm::ortho(0.f,16.0f,0.f,9.f, -1.f, 100.f));
	//	Daybreak::Renderer2D::BeginScene(m_CameraController.GetCamera(), m_CameraController.GetView());
	//	auto transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,-10.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(90.0f));
	//	Daybreak::Renderer2D::DrawQuad(transform, tileTexture, glm::vec4(1.0f), 100.0f);
	//	Daybreak::Renderer2D::EndScene();
	//	glEnable(GL_DEPTH_TEST);
	//	m_CameraController.UpdateProj(glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 75.0f));
	//}

	Daybreak::Renderer2D::BeginScene(m_CameraController.GetCamera(), m_CameraController.GetView());
	
	for (int x = 0; x < 3 ; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			glm::vec3 pos(1.11f * x, 1.11f * y, 0.0f);
			gridDepth = pos.z;
			if (x % 2 == 1 || y % 2 == 1)
				Daybreak::Renderer2D::DrawQuad(pos, { 1.0f, 1.0f }, texture1);
			else
				Daybreak::Renderer2D::DrawQuad(pos, { 1.0f, 1.0f }, texture2);
		}
	}

	//std::sort(ObjectsList.begin(), ObjectsList.end());
	//for (int i = 0; i < ObjectsList.size(); i++)
	//{
	//	if (ObjectsList[i].name == "object1")
	//		ObjectsList[i].position = obj1Pos;
	//	Daybreak::Renderer2D::DrawQuad(ObjectsList[i].position, ObjectsList[i].size, ObjectsList[i].texture);
	//}

	//Daybreak::Renderer2D::DrawQuad(obj1Pos, glm::vec2(3.0f), glm::vec4(1.0f, 0.5f, 0.5f, 0.75f));
	Daybreak::Renderer2D::DrawQuad(obj1Pos, glm::vec2(3.0f), texture1, glm::vec4(0.5f,0.2f,0.95f,1.0f));


	Daybreak::Renderer2D::EndScene();

	m_CameraController.Update(dt);

	float amount = 10.0f * dt;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::I))
		obj1Pos.y += amount;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::K))
		obj1Pos.y -= amount;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::L))
		obj1Pos.x += amount;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::J))
		obj1Pos.x -= amount;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::O))
		obj1Pos.z += amount;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::U))
		obj1Pos.z -= amount;
}

void GameLayer::OnEvent(Daybreak::Event& event)
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
			m_CameraController.UpdateProj(glm::perspective(glm::radians(45.0f), (float)e.GetWidth() / (float)e.GetHeight(), 0.0f, 100.0f));
	}
}

void GameLayer::OnImGuiRender()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::Begin("ImGui Layer");
	ImGui::Text("Camera Position");
	ImGui::Text((const char*)glm::to_string(m_CameraController.GetCameraPosition()).c_str());
	ImGui::Text("Big Square Position");
	ImGui::Text((const char*)glm::to_string(obj1Pos).c_str());
	ImGui::Text("Grid Depth %.1f", gridDepth);

	ImGui::Checkbox("Use Vsync", &vsyncBox);
	Daybreak::Application::Get().GetWindow().SetVSync(vsyncBox);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::End();
}
