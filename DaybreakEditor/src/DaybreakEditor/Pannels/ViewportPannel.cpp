#pragma once

#include "DaybreakEditor/Pannels/ViewportPannel.h"

#include <imgui.h>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "DaybreakEditor/EditorLayer.h"

#include "DaybreakEditor/Pannels/SelectionContext.h"

namespace Daybreak
{
	ViewportPannel::ViewportPannel(Ref<Scene> scene)
		: m_Scene(scene)
	{
		m_CameraScene = CreateRef<Scene>();
		m_EditorCameraEntity = m_CameraScene->CreateEntity("EditorCamera");
		CameraComponent& camera = m_EditorCameraEntity.AddComponent<CameraComponent>();
		camera.Camera.SetProjection(glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 75.0f));
		camera.Primary = true;
		m_EditorCameraEntity.GetComponent<TransformComponent>().Position = { 0, 0, -10 };
	}

	void ViewportPannel::OnUpdate(DeltaTime dt)
	{
		if (m_IsPlaying)
		{
			m_Scene->m_ClearColor = m_RuntimeClearColor;
			m_Scene->OnRenderScene(m_Scene->GetActiveCameraEntity());
		}
		else if (!m_IsPlaying)
		{
			m_Scene->m_ClearColor = m_EditorClearColor;
			m_Scene->OnRenderScene(m_EditorCameraEntity);
		}
		if (m_DrawGrid)
		{
			m_Scene->m_ScreenBuffer->Bind();
			DrawGrid();
			m_Scene->m_ScreenBuffer->Unbind();
		}
	}

	void ViewportPannel::OnAttach()
	{
	}

	void ViewportPannel::OnDetach()
	{
	}

	void ViewportPannel::OnImGuiRender()
	{
		ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_NoScrollbar);

		// if (!m_IsPlaying && ImGui::IsWindowHovered())

		if (ImGui::Button("Toggle Gizmos"))
		{
			m_Scene->ToggleDebugDraw();
		}
		ImGui::SameLine();
		if (ImGui::Button("Toggle Grid"))
		{
			m_DrawGrid = !m_DrawGrid;
		}
		ImGui::SameLine();

		std::string text = m_IsPlaying ? "Pause" : "Play";
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2);
		if (ImGui::Button(text.c_str()))
		{
			m_IsPlaying = !m_IsPlaying;
		}
		ImGui::Separator();

		float cursorYBeforeImage = ImGui::GetCursorPos().y;

		uint32_t textureID = m_Scene->m_ScreenBuffer->GetAttachmentRendererID(0);
		const ImVec2 viewportSize = ImGui::GetContentRegionAvail();

		const float windowWidth = ImGui::GetWindowWidth();
		m_ImageSize = { windowWidth, windowWidth / m_AspectRatio };
		ImGui::SetCursorPos({ 0.5f * (windowWidth - m_ImageSize.x),
							  0.5f * (viewportSize.y - m_ImageSize.y) + ImGui::GetCursorPosY() });
		ImGui::Image((void*)(intptr_t)textureID, { m_ImageSize.x, m_ImageSize.y },
					 { 0, 1 }, { 1, 0 });
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_SCENE"))
			{
				const char* cString = (const char*)payload->Data;
				std::filesystem::path path(cString);
				m_IsPlaying = false;
				SceneSerializer serializer = SceneSerializer(m_Scene);
				serializer.Deserialize(AssetManager::Get()->GetAssetDir() / path);
				SelectionContext::ResetContext();
			}
			ImGui::EndDragDropTarget();
		}

		if (SelectionContext::GetContext()->GetContextType() == SelectionContextType::Entity)
		{
			Entity selectedEntity = std::static_pointer_cast<EntityContext>(SelectionContext::GetContext())->GetEntity();

			if (selectedEntity && !m_IsPlaying)
			{
				m_GuizmoOffset = (viewportSize.y - m_ImageSize.y) / 2 + cursorYBeforeImage;
				DrawTransformGuizmo(selectedEntity);
			}
		}
		if (!m_IsPlaying && ImGui::IsWindowHovered())
		{
			UpdateEditorCameraPosition();
		}

		ImGui::End();
	}

	void ViewportPannel::DrawTransformGuizmo(Entity& entity) const
	{
		ImGuizmo::SetOrthographic(true);
		ImGuizmo::SetDrawlist();

		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + m_GuizmoOffset,
						  m_ImageSize.x, m_ImageSize.y);

		Entity cameraEntity = m_EditorCameraEntity;
		const Camera& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
		const glm::mat4& cameraProjection = camera.GetProjection();
		// glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());
		// glm::mat4 cameraView = cameraEntity.GetComponent<TransformComponent>().GetTransform();

		glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(-1, -1, 1) * (cameraEntity.GetComponent<TransformComponent>().Position));
		glm::mat4 rotation = glm::toMat4(glm::quat(cameraEntity.GetComponent<TransformComponent>().Rotation));
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), cameraEntity.GetComponent<TransformComponent>().Scale);
		glm::mat4 cameraView = scale * rotation * translation;

		auto& tc = entity.GetComponent<TransformComponent>();
		glm::mat4 transform = tc.GetTransform();

		ImGuizmo::OPERATION operation = ImGuizmo::OPERATION();
		if (m_Guizmos2D)
		{
			operation = operation | ImGuizmo::OPERATION::TRANSLATE_X | ImGuizmo::OPERATION::TRANSLATE_Y;
			operation = operation | ImGuizmo::OPERATION::ROTATE_Z | ImGuizmo::OPERATION::SCALE_X | ImGuizmo::OPERATION::SCALE_Y;
		}
		else
		{
			operation = ImGuizmo::OPERATION::TRANSLATE | ImGuizmo::OPERATION::SCALE | ImGuizmo::OPERATION::ROTATE;
		}

		ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
							 operation, ImGuizmo::LOCAL, glm::value_ptr(transform));

		if (ImGuizmo::IsUsing())
		{
			glm::vec3 scale;
			glm::quat rotation;
			glm::vec3 translation;
			glm::vec3 skew;
			glm::vec4 perspective;
			glm::decompose(transform, scale, rotation, translation, skew, perspective);
			tc.Position = translation;
			tc.Scale = scale;
			tc.Rotation = glm::eulerAngles(rotation);
		}
	}

	void ViewportPannel::OnEvent(Event& event)
	{
	}

	void ViewportPannel::DrawGrid() const
	{
		Renderer2D::NextBatch();
		// Needs to be cleaned up somewhat
		glm::vec3 cameraCenter = m_EditorCameraEntity.GetComponent<TransformComponent>().Position;

		constexpr int numberLines = 30;
		constexpr glm::vec4 lineColor = { 1, 1, 1, 1 };

		for (int x = int(cameraCenter.x) - numberLines / 2; x < numberLines; x++)
		{
			for (int y = int(cameraCenter.y) - numberLines / 2; y < numberLines; y++)
			{
				Renderer2D::DrawLine({ x, y }, { x + numberLines, y }, lineColor);
				Renderer2D::DrawLine({ x, y }, { x, y + numberLines }, lineColor);
			}
		}
		Renderer2D::EndScene();
	}

	void ViewportPannel::UpdateEditorCameraPosition()
	{
		if (!(Input::IsKeyPressed(Key::LeftAlt) || Input::IsKeyPressed(Key::RightAlt)))
		{
			return;
		}

		glm::vec3& cameraPos = m_EditorCameraEntity.GetComponent<TransformComponent>().Position;
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
		{
			ImVec2 delta = ImGui::GetMouseDragDelta();

			cameraPos.x -= delta.x / 50;
			cameraPos.y += delta.y / 50;
			ImGui::ResetMouseDragDelta(ImGuiMouseButton_Left);
		}

		if (Input::IsKeyPressed(Key::Up))
		{
			cameraPos.z += 1.0f;
		}
		if (Input::IsKeyPressed(Key::Down))
		{
			cameraPos.z -= 1.0f;
		}
		if (Input::IsKeyPressed(Key::Right))
		{
			cameraPos.z = -10.0f;
		}
	}
}