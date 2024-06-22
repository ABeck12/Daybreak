#include "DaybreakEditor/Pannels/InspectorPannel.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include <ImGuizmo.h>

#include "DaybreakEditor/Draw/Drawables.h"
#include "DaybreakEditor/Pannels/InspectorComponents.h"
#include "DaybreakEditor/Pannels/SelectionContext.h"

namespace Daybreak
{
	InspectorPannel::InspectorPannel()
	{
	}

	void InspectorPannel::OnUpdate(DeltaTime dt)
	{
	}

	void InspectorPannel::OnAttach()
	{
	}

	void InspectorPannel::OnDetach()
	{
	}

	void InspectorPannel::OnImGuiRender()
	{
		ImGui::Begin("Insepector");

		SelectionContextType type = SelectionContext::GetContext()->GetContextType();
		switch (type)
		{
			case SelectionContextType::Entity:
			{
				Entity selectedEntity = std::static_pointer_cast<EntityContext>(SelectionContext::GetContext())->GetEntity();
				DrawEntityInspector(selectedEntity);
				m_LastEntityName = selectedEntity.GetName();
				break;
			}
			case SelectionContextType::File:
			{
				std::filesystem::path filepath = std::static_pointer_cast<FileContext>(SelectionContext::GetContext())->GetPath();
				DrawFileInspector(filepath);
				break;
			}
			case SelectionContextType::None:
			{
				break;
			}
		}
		ImGui::End();
	}

	template<typename T>
	static void DisplayAddComponentEntry(Entity entity, const std::string& entryName)
	{
		if (!entity.HasComponent<T>())
		{
			if (ImGui::MenuItem(entryName.c_str()))
			{
				entity.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}

	void InspectorPannel::DrawEntityInspector(Entity& entity)
	{
		bool& active = entity.GetComponent<ActiveComponent>().Active;
		ImGui::Checkbox("##Active", &active);
		ImGui::SameLine();
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		float width = window->InnerRect.GetWidth();

		float widthx = ImGui::GetCursorPosX();
		ImGui::SetNextItemWidth(width - widthx);

		auto& tag = entity.GetComponent<TagComponent>().Tag;
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strncpy_s(buffer, sizeof(buffer), tag.c_str(), sizeof(buffer));
		if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
		{
			tag = std::string(buffer);
		}
		ImGui::Separator();

		DrawComponent<TransformComponent>(entity, "Transform", false);
		DrawComponent<SpriteRendererComponent>(entity, "Sprite Renderer");
		DrawComponent<TextRendererComponent>(entity, "Text Renderer");
		DrawComponent<CameraComponent>(entity, "Camera");
		DrawComponent<Rigidbody2DComponent>(entity, "Rigidbody 2D");
		DrawComponent<BoxCollider2DComponent>(entity, "Box Collider 2D");
		DrawComponent<CircleCollider2DComponent>(entity, "Circle Collider 2D");
		DrawComponent<PolygonCollider2DComponent>(entity, "Polygon Collider 2D");
		DrawComponent<AnimatorComponent>(entity, "Animator");
		DrawComponent<ScriptComponent>(entity, "Script");
		DrawComponent<PointLight2DComponent>(entity, "Point Light 2D");
		DrawComponent<GlobalLight2DComponent>(entity, "Global Light");


		ImGui::SetNextItemWidth(40.0f);
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - 40.0f);
		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("Add Component");
		}
		if (ImGui::BeginPopup("Add Component"))
		{
			ImGui::SeparatorText("Components");
			DisplayAddComponentEntry<SpriteRendererComponent>(entity, "Sprite Renderer");
			DisplayAddComponentEntry<TextRendererComponent>(entity, "Text Renderer");
			DisplayAddComponentEntry<CameraComponent>(entity, "Camera");
			DisplayAddComponentEntry<Rigidbody2DComponent>(entity, "Rigidbody 2D");
			DisplayAddComponentEntry<BoxCollider2DComponent>(entity, "Box Collider 2D");
			DisplayAddComponentEntry<CircleCollider2DComponent>(entity, "Circle Collider 2D");
			DisplayAddComponentEntry<PolygonCollider2DComponent>(entity, "Polygon Collider 2D");
			DisplayAddComponentEntry<AnimatorComponent>(entity, "Animator");
			DisplayAddComponentEntry<ScriptComponent>(entity, "Script");
			DisplayAddComponentEntry<PointLight2DComponent>(entity, "Point Light 2D");
			DisplayAddComponentEntry<GlobalLight2DComponent>(entity, "Global Light 2D");

			ImGui::EndPopup();
		}
	}

	void InspectorPannel::OnEvent(Event& event)
	{
		if (event.GetEventType() == EventType::KeyPressed)
		{
			KeyPressedEvent& e = (KeyPressedEvent&)event;
			if (e.GetKeyCode() == Key::Escape)
			{
				SelectionContext::ResetContext();
			}
		}
	}


	void InspectorPannel::DrawFileInspector(const std::filesystem::path& path)
	{
		ImGui::Text("%s", path.filename().string().c_str());
		ImGui::Separator();

		std::ifstream file(path);
		std::string text;
		while (getline(file, text))
		{
			ImGui::Text("%s", text.c_str());
		}
	}

}