#pragma once

#include "DaybreakEditor/Pannels/InspectorPannel.h"
#include "DaybreakEditor/Draw/Drawables.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

namespace Daybreak
{
	template<typename T>
	static void DrawComponentFields(Entity& entity);

	template<>
	void DrawComponentFields<TransformComponent>(Entity& entity)
	{
		auto& transform = entity.GetComponent<TransformComponent>();
		DrawField::DragVec3("Position", transform.Position);
		glm::vec3 degRotation = glm::degrees(transform.Rotation);
		DrawField::DragVec3("Rotation", degRotation);
		transform.Rotation = glm::radians(degRotation);
		DrawField::DragVec3("Scale", transform.Scale);
	}


	template<>
	void DrawComponentFields<SpriteRendererComponent>(Entity& entity)
	{
		auto& sr = entity.GetComponent<SpriteRendererComponent>();
		ImGui::ColorEdit4("Tint Color", &sr.TintColor.x);
		DrawField::DragFloat("Tiling Factor", sr.TilingFactor, 0.0f, FLT_MAX, ImGuiSliderFlags_AlwaysClamp);

		int input = sr.RenderLayer;
		DrawField::InputInt("Render Layer", input);
		sr.RenderLayer = (uint8_t)std::clamp<int>(input, 0, 32);

		ImGui::Text("TODO: Add sprite filepath");
	}

	template<>
	void DrawComponentFields<CameraComponent>(Entity& entity)
	{
		auto& camera = entity.GetComponent<CameraComponent>();
		DrawField::Checkbox("Primary", camera.Primary);
		ImGui::Text("TODO: Add projection type");
	}

	template<>
	void DrawComponentFields<BoxCollider2DComponent>(Entity& entity)
	{
		auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
		DrawField::DragVec2("Size", bc2d.Size, 0.0f, FLT_MAX, ImGuiSliderFlags_AlwaysClamp);

		DrawField::DragVec2("Offset", bc2d.Offset);
		int input = bc2d.CollisionLayer;
		DrawField::InputInt("Collision Layer", input);

		bc2d.CollisionLayer = (uint8_t)std::clamp<int>(input, 0, 32);
		DrawField::Checkbox("IsTrigger", bc2d.IsTrigger);
		DrawField::Checkbox("Enabled", bc2d.Enabled);
	}

	template<>
	void DrawComponentFields<CircleCollider2DComponent>(Entity& entity)
	{
		auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
		DrawField::DragFloat("Radius", cc2d.Radius, 0.0f, FLT_MAX, ImGuiSliderFlags_AlwaysClamp);
		DrawField::DragVec2("Offset", cc2d.Offset);
		int input = cc2d.CollisionLayer;
		DrawField::InputInt("Collision Layer", input);
		cc2d.CollisionLayer = (uint32_t)std::clamp<int>(input, 0, 32);
		DrawField::Checkbox("IsTrigger", cc2d.IsTrigger);
		DrawField::Checkbox("Enabled", cc2d.Enabled);
	}

	template<>
	void DrawComponentFields<Rigidbody2DComponent>(Entity& entity)
	{
		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		int intType = (int)rb2d.Type;
		DrawField::Combobox("Body Type", intType, "Static\0Dynamic\0Kinematic\0\0");
		rb2d.Type = (Rigidbody2DComponent::BodyType)intType;
		switch (intType)
		{
			case 0: rb2d.Type = Rigidbody2DComponent::BodyType::Static; break;
			case 1: rb2d.Type = Rigidbody2DComponent::BodyType::Dynamic; break;
			case 2: rb2d.Type = Rigidbody2DComponent::BodyType::Kinematic; break;
		}

		DrawField::Checkbox("Continuous Detection", rb2d.ContinuousDetection);
		DrawField::Checkbox("Allow Sleep", rb2d.AllowSleep);
		DrawField::DragVec2("Velocity", rb2d.Velocity);
		DrawField::DragFloat("Mass", rb2d.Mass, 0.0f, FLT_MAX, ImGuiSliderFlags_AlwaysClamp);
		DrawField::DragFloat("Gravity Scale", rb2d.GravityScale, 0.0f, FLT_MAX, ImGuiSliderFlags_AlwaysClamp);
		DrawField::DragFloat("Angular Drag", rb2d.AngularDrag, 0.0f, FLT_MAX, ImGuiSliderFlags_AlwaysClamp);
		DrawField::DragFloat("Linear Drag", rb2d.LinearDrag, 0.0f, FLT_MAX, ImGuiSliderFlags_AlwaysClamp);
		DrawField::DragFloat("Density", rb2d.Density, 0.0f, FLT_MAX, ImGuiSliderFlags_AlwaysClamp);
		DrawField::DragFloat("Friction", rb2d.Friction, 0.0f, FLT_MAX, ImGuiSliderFlags_AlwaysClamp);
		DrawField::DragFloat("Restitution", rb2d.Restitution, 0.0f, FLT_MAX, ImGuiSliderFlags_AlwaysClamp);
		DrawField::DragFloat("Restitution Threshold", rb2d.RestitutionThreshold, 0.0f, FLT_MAX, ImGuiSliderFlags_AlwaysClamp);
	}

	template<>
	void DrawComponentFields<PolygonCollider2DComponent>(Entity& entity)
	{
		auto& pc2d = entity.GetComponent<PolygonCollider2DComponent>();
		int count = pc2d.Count;
		DrawField::InputInt("Count", count);
		pc2d.Count = count;
		ImGui::Indent();
		for (int i = 0; i < count; i++)
		{
			DrawField::DragVec2("Vertex " + std::to_string(i), pc2d.Vertices[i]);
		}
		ImGui::Unindent();
		int input = pc2d.CollisionLayer;
		DrawField::InputInt("Collision Layer", input);
		pc2d.CollisionLayer = (uint32_t)std::clamp<int>(input, 0, 32);
		DrawField::Checkbox("IsTrigger", pc2d.IsTrigger);
		DrawField::Checkbox("Enabled", pc2d.Enabled);
	}

	template<>
	void DrawComponentFields<AnimatorComponent>(Entity& entity)
	{
		auto& anim = entity.GetComponent<AnimatorComponent>();
		DrawField::Checkbox("IsPlaying", anim.IsPlaying);
		ImGui::ColorEdit4("Tint Color", &anim.TintColor.x);
		int input = anim.RenderLayer;
		DrawField::InputInt("Render Layer", input);
		anim.RenderLayer = (uint8_t)std::clamp<int>(input, 0, 32);

		ImGui::Text("Add anim controller filepath");
	}

	template<>
	void DrawComponentFields<TextRendererComponent>(Entity& entity)
	{
		auto& tr = entity.GetComponent<TextRendererComponent>();
		DrawField::InputText("Text", tr.Text);
		DrawField::Text("Font", tr.Font->GetName());
		ImGui::ColorEdit4("Tint Color", &tr.Color.x);
		DrawField::DragFloat("Kerning", tr.Kerning, 0.0f, FLT_MAX, ImGuiSliderFlags_AlwaysClamp);
		DrawField::DragFloat("Line Spacing", tr.LineSpacing, 0.0f, FLT_MAX, ImGuiSliderFlags_AlwaysClamp);

		int input = tr.RenderLayer;
		DrawField::InputInt("Render Layer", input);
		tr.RenderLayer = (uint8_t)std::clamp<int>(input, 0, 32);
	}

	template<>
	void DrawComponentFields<PointLight2DComponent>(Entity& entity)
	{
		auto& pl = entity.GetComponent<PointLight2DComponent>();
		DrawField::DragFloat("Inner Radius", pl.InnerRadius, 0.0f, pl.OuterRadius, ImGuiSliderFlags_AlwaysClamp);
		DrawField::DragFloat("Outer Radius", pl.OuterRadius, pl.InnerRadius, FLT_MAX, ImGuiSliderFlags_AlwaysClamp);
		DrawField::DragFloat("Intensity", pl.Intensity, 0.0f, FLT_MAX, ImGuiSliderFlags_AlwaysClamp);
		ImGui::ColorEdit3("Color", &pl.Color.r);
	}

	template<>
	void DrawComponentFields<GlobalLight2DComponent>(Entity& entity)
	{
		auto& gl = entity.GetComponent<GlobalLight2DComponent>();
		DrawField::DragFloat("Intensity", gl.Intensity, 0.0f, FLT_MAX, ImGuiSliderFlags_AlwaysClamp);
		ImGui::ColorEdit3("Color", &gl.Color.r);
	}

	template<>
	void DrawComponentFields<ScriptComponent>(Entity& entity)
	{
		auto& sc = entity.GetComponent<ScriptComponent>();
		auto registry = ScriptRegistry::GetRegistry();
		int currentItem = 0;
		int index = 0;
		std::vector<char> chars;
		std::vector<std::string> strings;
		for (auto& item : registry)
		{
			strings.emplace_back(item.first);
			for (auto& character : item.first)
			{
				chars.emplace_back(character);
			}
			chars.emplace_back(0);
			if (item.first == sc.TypeName)
			{
				currentItem = index;
			}
			index++;
		}
		DrawField::Combobox("Scripts", currentItem, &chars[0]);
		sc.TypeName = strings[currentItem];
	}

	template<typename T>
	void DrawComponent(Entity& entity, const std::string& name, bool removeable = true)
	{
		if (!entity.HasComponent<T>())
		{
			return;
		}
		int flags = ImGuiTreeNodeFlags_DefaultOpen;
		ImGui::AlignTextToFramePadding();
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, name.c_str());
		bool remove;
		if (removeable)
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			float width = window->InnerRect.GetWidth();

			ImGui::SameLine(width - 60);
			remove = ImGui::Button("Remove");
		}

		if (open)
		{
			DrawComponentFields<T>(entity);
			ImGui::TreePop();
		}

		if (removeable && remove)
		{
			entity.RemoveComponent<T>();
		}
		ImGui::Separator();
	}

}