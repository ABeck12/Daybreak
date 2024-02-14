#include "SceneLayer.h"

#include <Daybreak.h>
#include <imgui.h>

#include "ScriptableEntityTest.h"

SceneLayer::SceneLayer() : Layer("SceneLayer")
{
	// Daybreak::Application::Get().GetWindow().SetVSync(false);
	m_Scene = Daybreak::CreateRef<Daybreak::Scene>();

	playerEntity = m_Scene->CreateEntity("Entity Test");
	playerEntity.AddComponent<Daybreak::NativeScriptComponent>().Bind<MoveableComponent>();
	//auto& texture = Daybreak::Texture2D::Create({ 3, 3, Daybreak::ImageFormat::RGBA, Daybreak::TextureFilterType::Bilinear }, "../Resources/DaybreakLogo.png");
	auto& rb2d = playerEntity.AddComponent<Daybreak::Rigidbody2DComponent>();
	rb2d.Type = Daybreak::Rigidbody2DComponent::BodyType::Dynamic;
	rb2d.FixedRotation = true;
	rb2d.Restitution = 0.00f;
	//rb2d.RestitutionThreshold = 2.0f;
	auto& bc2d = playerEntity.AddComponent<Daybreak::BoxCollider2DComponent>();

	auto& spriteSheet = Daybreak::Texture2D::Create({ 3, 3, Daybreak::ImageFormat::RGBA, Daybreak::TextureFilterType::Point }, "../Sandbox/assets/adventurer-Sheet.png");
	float width = 50.;
	float height = 37.;
	auto& subtexture1 = Daybreak::SubTexture2D::Create(spriteSheet, { 0, 10 }, { width, height });
	auto& subtexture2 = Daybreak::SubTexture2D::Create(spriteSheet, { 1, 10 }, { width, height });
	auto& subtexture3 = Daybreak::SubTexture2D::Create(spriteSheet, { 2, 10 }, { width, height });
	auto& subtexture4 = Daybreak::SubTexture2D::Create(spriteSheet, { 3, 10 }, { width, height });
	Daybreak::Ref<Daybreak::AnimationSource> animSource = Daybreak::CreateRef<Daybreak::AnimationSource>();
	auto& anim = playerEntity.AddComponent<Daybreak::AnimatorComponent>();
	float step = 0.16f;
	animSource->AddFrame({ subtexture1, 0 * step });
	animSource->AddFrame({ subtexture2, 1 * step });
	animSource->AddFrame({ subtexture3, 2 * step });
	animSource->AddFrame({ subtexture4, 3 * step });
	animSource->AddFrame({ subtexture1, 4 * step });
	anim.Source = animSource;



	
	cameraEntity = m_Scene->CreateEntity("Camera");
	auto& cameraComp = cameraEntity.AddComponent<Daybreak::CameraComponent>();
	cameraComp.Primary = true;
	cameraComp.Camera.SetProjection(glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 75.0f));
	auto& cameraTransformComp = cameraEntity.GetComponent<Daybreak::TransformComponent>();
	cameraTransformComp.Position = glm::vec3(0.0f, 0.0f, -10.0f);

	lastMousePos = Daybreak::Input::GetMousePosition();

	floorEntity = m_Scene->CreateEntity("Floor");
	auto& floorsr = floorEntity.AddComponent<Daybreak::SpriteRendererComponent>();
	auto& floorrb2d = floorEntity.AddComponent<Daybreak::Rigidbody2DComponent>();
	auto& floorbc2d = floorEntity.AddComponent<Daybreak::BoxCollider2DComponent>();
	auto& floortransform = floorEntity.GetComponent<Daybreak::TransformComponent>();
	floortransform.Position = { 0.0f,-4.0f,0.0f };
	floortransform.Scale = { 15.0f, 1.0f,1.0f };
	auto& texture = Daybreak::Texture2D::Create({ 3, 3, Daybreak::ImageFormat::RGBA, Daybreak::TextureFilterType::Bilinear }, "../Sandbox/assets/Test.png");
	floorsr.Sprite = texture;


	// floorEntity.GetComponent<Daybreak::RelationshipComponent>().ParentEntity = &cameraEntity;
	// m_Scene->CreateEntity(floorEntity, "TestChild");
}

void SceneLayer::OnAttach()
{
	m_Scene->OnRuntimeStart();
}

void SceneLayer::OnDetach()
{
	m_Scene->OnRuntimeEnd();
}

void SceneLayer::OnUpdate(Daybreak::DeltaTime dt)
{
	Daybreak::RenderCommand::Clear();

	Daybreak::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.2f, 1.0f)); // Blue-Gray
	auto playerPos = playerEntity.GetComponent<Daybreak::TransformComponent>().Position;
	bool hit = m_Scene->GetPhysicsSim2D().Raycast({ playerPos.x, playerPos.y }, { playerPos.x, playerPos.y - 1 });

	// Camera Update
	float amount = 10.0f * dt;
	auto& cameraPos = cameraEntity.GetComponent<Daybreak::TransformComponent>().Position;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::K))
		cameraPos.y += amount;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::I))
		cameraPos.y -= amount;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::J))
		cameraPos.x += amount;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::L))
		cameraPos.x -= amount;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::O))
		cameraPos.z += amount;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::U))
		cameraPos.z -= amount;
	auto& cameraRot = cameraEntity.GetComponent<Daybreak::TransformComponent>().Rotation;
	float amountRot = 10.0f * dt;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::D7))
		cameraRot.z += amountRot;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::D9))
		cameraRot.z -= amountRot;
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::D8))
		cameraRot.z = 0.0f;

	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::P))
	{
		cameraPos = glm::vec3(0.0f, 0.0f, -10.0f);
		auto& playerEntityPos = playerEntity.GetComponent<Daybreak::TransformComponent>().Position;
		playerEntityPos = glm::vec3(0.0f);
	}


	m_Scene->OnRuntimeUpdate(dt);
	// DrawColliders();
}

void SceneLayer::OnEvent(Daybreak::Event& event)
{
	if (event.GetEventType() == Daybreak::EventType::KeyPressed)
	{
		Daybreak::KeyPressedEvent& e = (Daybreak::KeyPressedEvent&)event;
		if (e.GetKeyCode() == Daybreak::Key::Escape)
		{
			Daybreak::Application::Get().Close();
		}
		if (e.GetKeyCode() == Daybreak::Key::E)
		{
			// DB_LOG("Playing clip");
			// auto clip = Daybreak::AudioClip::Create("../Sandbox/assets/bonk.mp3");
			// Daybreak::AudioEngine::Play(clip);
		}
	}
	if (event.GetEventType() == Daybreak::EventType::WindowResize)
	{
		Daybreak::WindowResizeEvent& e = (Daybreak::WindowResizeEvent&)event;
		if (e.GetWidth() != 0 && e.GetHeight() != 0) //TEMPORARY!!!
			cameraEntity.GetComponent<Daybreak::CameraComponent>().Camera.SetProjection(glm::perspective(glm::radians(45.0f), (float)e.GetWidth() / (float)e.GetHeight(), 0.0f, 100.0f));
	}

}

void SceneLayer::OnImGuiRender()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::Begin("ImGui Layer");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::End();

	ImGui::Begin("Box Entity");
	ImGui::SliderFloat("Roation", &playerEntity.GetComponent<Daybreak::TransformComponent>().Rotation.z,-3.14f,3.14f);
	//ImGui::SliderFloat3("Position", )
	ImGui::End();
}

void SceneLayer::MoveCamera(Daybreak::Entity& entity, Daybreak::DeltaTime dt)
{
	float rotationSensitivity = 100.0f;
	float translationAmount = 10.0f * dt;
	glm::vec2 mousePos = Daybreak::Input::GetMousePosition();
	auto& transform = entity.GetComponent<Daybreak::TransformComponent>();
	//transform.Rotation.x += (mousePos.x - lastMousePos.x) / rotationSensitivity;
	//transform.Rotation.y += (-mousePos.y + lastMousePos.y) / rotationSensitivity;

	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::A))
	{
		transform.Position.x -= cos(glm::radians(transform.Rotation.z)) * translationAmount;
		transform.Position.y -= sin(glm::radians(transform.Rotation.z)) * translationAmount;
	}
	else if (Daybreak::Input::IsKeyPressed(Daybreak::Key::D))
	{
		transform.Position.x += cos(glm::radians(transform.Rotation.z)) * translationAmount;
		transform.Position.y += sin(glm::radians(transform.Rotation.z)) * translationAmount;
	}

	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::W))
	{
		transform.Position.x += -sin(glm::radians(transform.Rotation.z)) * translationAmount;
		transform.Position.y += cos(glm::radians(transform.Rotation.z)) * translationAmount;
	}
	else if (Daybreak::Input::IsKeyPressed(Daybreak::Key::S))
	{
		transform.Position.x -= -sin(glm::radians(transform.Rotation.z)) * translationAmount;
		transform.Position.y -= cos(glm::radians(transform.Rotation.z)) * translationAmount;
	}

	lastMousePos = mousePos;
}

void SceneLayer::DrawColliders()
{
	auto view = m_Scene->GetAllEntitiesWith<Daybreak::BoxCollider2DComponent, Daybreak::TransformComponent>();

	auto& camera = m_Scene->GetActiveCameraEntity();
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), camera.GetComponent<Daybreak::TransformComponent>().Position);
	glm::mat4 rotation = glm::toMat4(glm::quat(camera.GetComponent<Daybreak::TransformComponent>().Rotation));
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), camera.GetComponent<Daybreak::TransformComponent>().Scale);

	Daybreak::Renderer2D::BeginScene(cameraEntity.GetComponent<Daybreak::CameraComponent>().Camera, scale * rotation * translation);
	glm::vec4 lineColor = { 0.239f, 1.0f, 0.0f, 1.0f };


	for (auto e : view)
	{
		Daybreak::Entity entity = { e, m_Scene.get()};
		auto transform = entity.GetComponent<Daybreak::TransformComponent>();
		auto bc2d = entity.GetComponent<Daybreak::BoxCollider2DComponent>();


		glm::vec3 upperRight = {
			((transform.Position.x + bc2d.Offset.x) + bc2d.Size.x) * transform.Scale.x,
			((transform.Position.y + bc2d.Offset.y) + bc2d.Size.y) * transform.Scale.y,
			transform.Position.z
		};

		glm::vec3 upperLeft = {
			((transform.Position.x + bc2d.Offset.x) - bc2d.Size.x) * transform.Scale.x,
			((transform.Position.y + bc2d.Offset.y) + bc2d.Size.y) * transform.Scale.y,
			transform.Position.z
		};

		glm::vec3 lowerRight = {
			((transform.Position.x + bc2d.Offset.x) + bc2d.Size.x) * transform.Scale.x,
			((transform.Position.y + bc2d.Offset.y) - bc2d.Size.y) * transform.Scale.y,
			transform.Position.z
		};

		glm::vec3 lowerLeft = {
			((transform.Position.x + bc2d.Offset.x) - bc2d.Size.x) * transform.Scale.x,
			((transform.Position.y + bc2d.Offset.y) - bc2d.Size.y) * transform.Scale.y,
			transform.Position.z
		};

		Daybreak::Renderer2D::DrawLine(
			RotatePoint(upperRight-transform.Position, transform.Rotation.z)+transform.Position, 
			RotatePoint(upperLeft-transform.Position, transform.Rotation.z)+transform.Position, 
			lineColor);

		Daybreak::Renderer2D::DrawLine(
			RotatePoint(upperRight - transform.Position, transform.Rotation.z) + transform.Position,
			RotatePoint(lowerRight - transform.Position, transform.Rotation.z) + transform.Position,
			lineColor);

		Daybreak::Renderer2D::DrawLine(
			RotatePoint(lowerLeft - transform.Position, transform.Rotation.z) + transform.Position,
			RotatePoint(upperLeft - transform.Position, transform.Rotation.z) + transform.Position,
			lineColor);

		Daybreak::Renderer2D::DrawLine(
			RotatePoint(lowerLeft - transform.Position, transform.Rotation.z) + transform.Position,
			RotatePoint(lowerRight - transform.Position, transform.Rotation.z) + transform.Position,
			lineColor);
	}

	Daybreak::Renderer2D::EndScene();
}

glm::vec3 SceneLayer::RotatePoint(glm::vec3 vec, float theta)
{
	glm::vec3 out;

	out.x = vec.x * cos(theta) - vec.y * sin(theta);
	out.y = vec.x * sin(theta) + vec.y * cos(theta);
	out.z = vec.z;
	return out;
}
