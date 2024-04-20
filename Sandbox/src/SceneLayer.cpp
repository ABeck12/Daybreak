#include "SceneLayer.h"

#include <Daybreak.h>
#include <imgui.h>

#include "Daybreak/Renderer/Texture.h"
#include "ScriptableEntityTest.h"
#include "Daybreak/Scene/SceneSerializer.h"

SceneLayer::SceneLayer()
	: Layer("SceneLayer")
{
	// Daybreak::Application::Get().GetWindow().SetVSync(false);
	m_Scene = Daybreak::CreateRef<Daybreak::Scene>("Test Scene");
	{
		Daybreak::SceneSerializer serializer(m_Scene);
		serializer.Deserialize(Daybreak::AssetManager::Get()->GetAssetDir() / "scenes/SceneLayer.scene");
		return;
	}

	playerEntity = m_Scene->CreateEntity("Player");
	playerEntity.AddComponent<Daybreak::NativeScriptComponent>().Bind<MoveableComponent>();
	// auto& texture = Daybreak::Texture2D::Create({ 3, 3, Daybreak::ImageFormat::RGBA, Daybreak::TextureFilterType::Bilinear }, "../Resources/DaybreakLogo.png");
	auto& rb2d = playerEntity.AddComponent<Daybreak::Rigidbody2DComponent>();
	rb2d.Type = Daybreak::Rigidbody2DComponent::BodyType::Dynamic;
	rb2d.FixedRotation = true;
	rb2d.AllowSleep = false;
	rb2d.Restitution = 0.00f;
	// rb2d.RestitutionThreshold = 2.0f;
	auto& bc2d = playerEntity.AddComponent<Daybreak::BoxCollider2DComponent>();
	bc2d.Size = { 0.1f, 0.48f };
	// bc2d.Offset.y = 0.04f;

	const Daybreak::Ref<Daybreak::Texture2D> spriteSheet = Daybreak::Texture2D::Create({ 3, 3, Daybreak::ImageFormat::RGBA, Daybreak::TextureFilterType::Point }, "../Sandbox/assets/sprites/adventurer-Sheet.png");
	float width = 50.;
	float height = 37.;
	auto& subtexture1 = Daybreak::SubTexture2D::Create(spriteSheet, { 0, 10 }, { width, height });
	auto& subtexture2 = Daybreak::SubTexture2D::Create(spriteSheet, { 1, 10 }, { width, height });
	auto& subtexture3 = Daybreak::SubTexture2D::Create(spriteSheet, { 2, 10 }, { width, height });
	auto& subtexture4 = Daybreak::SubTexture2D::Create(spriteSheet, { 3, 10 }, { width, height });
	// Daybreak::Ref<Daybreak::AnimationSource> animSource = Daybreak::CreateRef<Daybreak::AnimationSource>();
	auto& anim = playerEntity.AddComponent<Daybreak::AnimatorComponent>();
	// animSource->AddKeyFrame(subtexture1, 10);
	// animSource->AddKeyFrame(subtexture2, 10);
	// animSource->AddKeyFrame(subtexture3, 10);
	// animSource->AddKeyFrame(subtexture4, 10);
	// animSource->AddKeyFrame(subtexture1, 10);
	// float step = 0.016f;
	// animSource->AddKeyFrame({ subtexture1, 0 * step });
	// animSource->AddKeyFrame({ subtexture2, 1 * step });
	// animSource->AddKeyFrame({ subtexture3, 2 * step });
	// animSource->AddKeyFrame({ subtexture4, 3 * step });
	// animSource->AddKeyFrame({ subtexture1, 4 * step });
	// anim.Source = animSource;


	cameraEntity = m_Scene->CreateEntity(playerEntity, "Camera");
	auto& cameraComp = cameraEntity.AddComponent<Daybreak::CameraComponent>();
	cameraComp.Primary = true;
	cameraComp.Camera.SetProjection(glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 75.0f));
	// cameraComp.Camera.SetProjection(glm::ortho(0.0f,1280.0f,0.0f,720.0f,0.1f,75.0f));
	float swidth = 1280.0f / 100;
	float sheight = 720.0f / 100;
	// float swidth = 16.0f;
	// float sheight = 9.0f;
	// cameraComp.Camera.SetProjection(glm::ortho(-swidth/2,swidth/2,-sheight/2,sheight/2,0.1f,75.0f));
	auto& cameraTransformComp = cameraEntity.GetComponent<Daybreak::TransformComponent>();
	cameraTransformComp.Position = glm::vec3(0.0f, 0.0f, -10.0f);

	lastMousePos = Daybreak::Input::GetMousePosition();

	floorEntity = m_Scene->CreateEntity("Floor");
	// auto texture = Daybreak::AssetSerializer::DeserializeSprite("sprites/Test.sprite");
	auto& texture = Daybreak::Texture2D::Create({ 3, 3, Daybreak::ImageFormat::RGBA, Daybreak::TextureFilterType::Point, 128 }, "../Sandbox/assets/sprites/Test.png");
	auto& floorsr = floorEntity.AddComponent<Daybreak::SpriteRendererComponent>(texture);
	auto& floorrb2d = floorEntity.AddComponent<Daybreak::Rigidbody2DComponent>();
	auto& floorbc2d = floorEntity.AddComponent<Daybreak::BoxCollider2DComponent>();
	auto& floortransform = floorEntity.GetComponent<Daybreak::TransformComponent>();
	floortransform.Position = { 0.0f, -4.0f, 0.0f };
	floortransform.Scale = { 15.0f, 1.0f, 1.0f };
	// floorsr.Sprite = texture;

	Daybreak::FrameBufferSpecifications fbspec;
	fbspec.Width = 1280;
	fbspec.Height = 720;
	m_FrameBuffer = Daybreak::FrameBuffer::Create(fbspec);


	// DB_LOG(floorsr.Sprite->GetFilepath());
	// floorEntity.GetComponent<Daybreak::RelationshipComponent>().ParentEntity = &cameraEntity;
	// m_Scene->CreateEntity(floorEntity, "TestChild");
	// auto& tileTexture = Daybreak::Texture2D::Create({ 3, 3, Daybreak::ImageFormat::RGBA, Daybreak::TextureFilterType::Point }, "../Sandbox/assets/TileGrid.png");

	auto background = m_Scene->CreateEntity("Background");
	auto backgroundSprite = Daybreak::Texture2D::Create({ 1280, 720, Daybreak::ImageFormat::RGBA, Daybreak::TextureFilterType::Point }, "../Sandbox/assets/sprites/Background.png");
	auto& backgroundTransform = background.GetComponent<Daybreak::TransformComponent>();
	backgroundTransform.Position.z = -1;
	auto& backgroundSr = background.AddComponent<Daybreak::SpriteRendererComponent>();
	backgroundSr.Sprite = backgroundSprite;
	// Daybreak::AssetManager::AddAssetRef(backgroundSr.Sprite, "sprites/Background.sprite");

	// backgroundSr.PixelsPerUnit = 1064;

	auto test = m_Scene->CreateEntity("Box");

	auto& testsr = test.AddComponent<Daybreak::SpriteRendererComponent>();
	auto& testtrans = test.GetComponent<Daybreak::TransformComponent>();
	testtrans.Position.y = -2;
	auto& testbc2d = test.AddComponent<Daybreak::BoxCollider2DComponent>();


	// Daybreak::SceneSerializer serializer(m_Scene);
	// serializer.Serialize("../Sandbox/assets/scenes/SceneLayer.dbscn");


	// auto testscn = Daybreak::CreateRef<Daybreak::Scene>();
	// m_Scene = Daybreak::CreateRef<Daybreak::Scene>();

	// auto serializer2 = Daybreak::SceneSerializer(m_Scene);
	// serializer2.Deserialize("../Sandbox/assets/scenes/SceneLayer.scene");
	// serializer2.Serialize("../Sandbox/assets/scenes/SceneLayer2.dbscn");
	// Daybreak::ScriptableEntityRegistry::RegisterType<MoveableComponent>();
	for (const auto& pair : Daybreak::ScriptRegistry::GetRegistry())
	{
		DB_LOG(pair.first);
	}
	// Daybreak::AssetSerializer::SerializeSprite(texture, "sprites/Test.sprite");
	// auto testanim = Daybreak::AssetSerializer::DeserializeAnimation("animations/idle.anim");
	// auto testanim2 = Daybreak::AssetSerializer::DeserializeAnimation("animations/idle.anim");

	// auto cont1 = Daybreak::AssetSerializer::DeserializeAnimationController("animations/playerController.controller");
	// auto cont2 = Daybreak::AssetSerializer::DeserializeAnimationController("animations/playerController.controller");
	// playerEntity.GetComponent<Daybreak::AnimatorComponent>().Controller = cont1;

	// auto tex1 = Daybreak::AssetSerializer::DeserializeSprite("sprites/adventurer-Sheet.sprite");

	// DB_LOG(Daybreak::AssetManager::GetFilepathOfRef<Daybreak::Texture2D>(tex1));
}

void SceneLayer::OnAttach()
{
	m_Scene->OnRuntimeStart();
	Daybreak::SceneSerializer serializer(m_Scene);
	// Daybreak::AssetManager::AddAssetRef(playerEntity.GetComponent<Daybreak::AnimatorComponent>().Controller, "animations/playerController.controller");
	// Daybreak::AssetManager::AddAssetRef(floorEntity.GetComponent<Daybreak::SpriteRendererComponent>().Sprite, "sprites/Test.sprite");
	// serializer.Serialize("../Sandbox/assets/scenes/SceneLayer.scene");
	// serializer.Serialize(Daybreak::AssetManager::Get()->GetAssetDir() / "scenes/SceneLayer.scene");
	// Daybreak::AssetSerializer::SerializeAnimationController(playerEntity.GetComponent<Daybreak::AnimatorComponent>().Controller, "animations/playerController.controller");
}

void SceneLayer::OnDetach()
{
	m_Scene->OnRuntimeEnd();
}

void SceneLayer::OnUpdate(Daybreak::DeltaTime dt)
{

	// std::cout << "Type: " << typeInfo.name() << std::endl;
	playerEntity = m_Scene->GetEntityByName("Player");
	cameraEntity = m_Scene->GetEntityByName("Camera");
	floorEntity = m_Scene->GetEntityByName("Floor");
	// DB_LOG(glm::mat4(1.0f)[0]);
	// m_FrameBuffer->Bind();
	// m_Scene->GetEntityByName("Background").GetComponent<Daybreak::TransformComponent>().Position.z = -1;
	// DB_LOG("player {} background {}", playerEntity.GetComponent<Daybreak::TransformComponent>().Position, m_Scene->GetEntityByName("Background").GetComponent<Daybreak::TransformComponent>().Position);

	Daybreak::RenderCommand::Clear();

	Daybreak::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.2f, 1.0f)); // Blue-Gray
	auto playerPos = playerEntity.GetComponent<Daybreak::TransformComponent>().Position;
	// bool hit = m_Scene->GetPhysicsSim2D().RayCast({ playerPos.x, playerPos.y }, { playerPos.x, playerPos.y - 1 });

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
	if (Daybreak::Input::IsKeyPressed(Daybreak::Key::T))
	{
		auto& rb2d = playerEntity.GetComponent<Daybreak::Rigidbody2DComponent>();
		auto& transform = playerEntity.GetComponent<Daybreak::TransformComponent>();
		// m_Scene->GetPhysicsSim2D().AddForce(rb2d, {0,100}, {transform.Position.x,transform.Position.y});
		// m_Scene->GetPhysicsSim2D().AddTorque(rb2d, 100.0f);
	}

	m_Scene->OnRuntimeUpdate(dt);
	DrawGrid();
	// m_FrameBuffer->Unbind();
	DrawColliders();
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
		if (e.GetWidth() != 0 && e.GetHeight() != 0) // TEMPORARY!!!
		{
			m_Scene->GetActiveCameraEntity().GetComponent<Daybreak::CameraComponent>().Camera.SetProjection(glm::perspective(glm::radians(45.0f), (float)e.GetWidth() / (float)e.GetHeight(), 0.1f, 75.0f));
			// m_Scene->GetEntityByName("Background").GetComponent<Daybreak::TransformComponent>().Position.z *= -1;
			m_FrameBuffer->Resize(e.GetWidth(), e.GetHeight());
		}
	}
}

void SceneLayer::OnImGuiRender()
{
	ImGuiIO& io = ImGui::GetIO();

	// ImGuiIO& io = ImGui::GetIO();
	// static bool opt_fullscreen_persistant = true;
	// bool opt_fullscreen = opt_fullscreen_persistant;
	// ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	// if (opt_fullscreen)
	// {
	// 	ImGuiViewport* viewport = ImGui::GetMainViewport();
	// 	ImGui::SetNextWindowPos(viewport->Pos);
	// 	ImGui::SetNextWindowSize(viewport->Size);
	// 	ImGui::SetNextWindowViewport(viewport->ID);
	// 	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	// 	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	// 	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 1.0f, 0.0f });
	// 	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	// 	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	// }

	// if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	// {
	// 	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	// 	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
	// }

	// ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	// ImGui::Begin("Image Test");
	// uint32_t textureID = m_FrameBuffer->GetAttachmentRendererID(0);
	// uint32_t width = Daybreak::Application::Get().GetWindow().GetWidth();
	// uint32_t height = Daybreak::Application::Get().GetWindow().GetHeight();
	// ImGui::Image((void*)textureID, ImVec2({ (float)width, (float)height }), { 0, 1 }, { 1, 0 });
	// ImGui::End();
	// // ImGui::PopStyleVar();
	// ImGui::PopStyleVar();
	// ImGui::PopStyleVar();
	// ImGui::PopStyleVar();


	ImGui::Begin("ImGui Layer");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::End();

	// ImGui::Begin("Box Entity");
	// ImGui::SliderFloat("Roation", &playerEntity.GetComponent<Daybreak::TransformComponent>().Rotation.z,-3.14f,3.14f);
	// //ImGui::SliderFloat3("Position", )
	// ImGui::End();
}

void SceneLayer::MoveCamera(Daybreak::Entity& entity, Daybreak::DeltaTime dt)
{
	float rotationSensitivity = 100.0f;
	float translationAmount = 10.0f * dt;
	glm::vec2 mousePos = Daybreak::Input::GetMousePosition();
	auto& transform = entity.GetComponent<Daybreak::TransformComponent>();
	// transform.Rotation.x += (mousePos.x - lastMousePos.x) / rotationSensitivity;
	// transform.Rotation.y += (-mousePos.y + lastMousePos.y) / rotationSensitivity;

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
		Daybreak::Entity entity = { e, m_Scene.get() };
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
			RotatePoint(upperRight - transform.Position, transform.Rotation.z) + transform.Position,
			RotatePoint(upperLeft - transform.Position, transform.Rotation.z) + transform.Position,
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

void SceneLayer::DrawGrid()
{
	float length = 100.0f;
	int numLines = 100;
	glm::vec4 lineColor = { 0.82f, 0.82f, 0.82f, 1.0f };


	for (int i = -numLines; i < numLines; i++)
	{
		Daybreak::Renderer2D::DrawLine({ (float)i + 0.5f, -length, 0.0f }, { (float)i + 0.5f, length, 0.0f }, lineColor);
		Daybreak::Renderer2D::DrawLine({ -length, (float)i + 0.5f, 0.0f }, { length, (float)i + 0.5f, 0.0f }, lineColor);

		// Daybreak::Renderer2D::DrawLine({(float)i, -length,0.0f}, {(float)i, length,0.0f}, {1.0f,1.0f,1.0f,1.0f});
		// Daybreak::Renderer2D::DrawLine({ -length, (float)i, 0.0f }, { length, (float)i, 0.0f }, {1.0f,1.0f,1.0f,0.5f});
	}
	Daybreak::Renderer2D::EndScene();
}
