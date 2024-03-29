#include "DaybreakEditor/Panels/HierarchyPanel.h"

#include <imgui.h>

namespace Daybreak
{
	HierarchyPannel::HierarchyPannel(const Ref<Scene>& scene)
		: m_ActiveScene(scene)
	{
	}

	void HierarchyPannel::Render()
	{
		ImGui::Begin("Hierarchy");

		ImGui::Text("Entities:");
		ImGui::Separator();
		m_ActiveScene->m_Registry.each([&](auto entityID)
									   {Entity entity{ entityID ,m_ActiveScene.get()};
							DrawEntityRow(entity); });

		ImGui::Separator();
		if (ImGui::Button("Add Entity"))
		{
			m_ActiveScene->CreateEntity("Entity");
		}

		ImGui::End();

		ImGui::Begin("Inspector");
		if (m_SelectionContext)
		{
			DrawInspectorComponents(m_SelectionContext);
		}
		ImGui::End();
	}

	void HierarchyPannel::DrawEntityRow(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			if (m_SelectionContext != entity)
			{
				m_SelectionContext = entity;
			}
			else
			{
				m_SelectionContext = {};
			}
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_ActiveScene->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
	}

	void HierarchyPannel::DrawInspectorComponents(Entity entity)
	{
		ImGui::Text(entity.GetName().c_str());
		ImGui::Separator();

		ImGui::Text("TransformComponent");
		auto& transform = entity.GetComponent<TransformComponent>();
		ImGui::InputFloat3("Position", &transform.Position.x);
		ImGui::InputFloat3("Rotation", &transform.Rotation.x);
		ImGui::InputFloat3("Scale", &transform.Scale.x);
		ImGui::Separator();

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			ImGui::Text("SpriteRendererComponent");
			auto& sr = entity.GetComponent<SpriteRendererComponent>();
			ImGui::Separator();
		}
		if (entity.HasComponent<CameraComponent>())
		{
			ImGui::Text("CameraComponent");
			auto& camera = entity.GetComponent<CameraComponent>();
			ImGui::Separator();
		}
		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			ImGui::Text("BoxCollider2DComponent");
			auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
			ImGui::InputFloat2("Size", &bc2d.Size.x);
			ImGui::InputFloat2("Offset", &bc2d.Offset.x);
			ImGui::Text("Add Collision Layer here TODO");
			// ImGui::InputInt("Collision Layer", &((int)bc2d.CollisionLayer));
			ImGui::Checkbox("IsTriggr", &bc2d.IsTrigger);
			ImGui::Checkbox("Enabled", &bc2d.Enabled);
			ImGui::Separator();
		}
		if (entity.HasComponent<CircleCollider2DComponent>())
		{
			ImGui::Text("CircleCollider2DComponent");
			auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
			ImGui::InputFloat("Radius", &cc2d.Radius);
			ImGui::InputFloat2("Offset", &cc2d.Offset.x);
			ImGui::Text("Add Collision Layer here TODO");
			// ImGui::InputInt("Collision Layer", &((int)cc2d.CollisionLayer));
			ImGui::Checkbox("IsTriggr", &cc2d.IsTrigger);
			ImGui::Checkbox("Enabled", &cc2d.Enabled);
			ImGui::Separator();
		}
		if (entity.HasComponent<Rigidbody2DComponent>())
		{
			ImGui::Text("Rigidbody2DComponent");
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
			ImGui::Checkbox("Continuous Detection", &rb2d.ContinuousDetection);
			ImGui::Checkbox("Allow Sleep", &rb2d.AllowSleep);
			ImGui::InputFloat2("Velocity", &rb2d.Velocity.x);
			ImGui::InputFloat("Mass", &rb2d.Mass);
			ImGui::InputFloat("Gravity Scale", &rb2d.GravityScale);
			ImGui::InputFloat("Angular Drag", &rb2d.AngularDrag);
			ImGui::InputFloat("Linear Drag", &rb2d.LinearDrag);
			ImGui::InputFloat("Density", &rb2d.Density);
			ImGui::InputFloat("Friction", &rb2d.Friction);
			ImGui::InputFloat("Restitution", &rb2d.Restitution);
			ImGui::InputFloat("Restitution Threshold", &rb2d.RestitutionThreshold);
			ImGui::Separator();
		}
		if (entity.HasComponent<AnimatorComponent>())
		{
			ImGui::Text("AnimatorComponent");
			auto& anim = entity.GetComponent<AnimatorComponent>();
			ImGui::Separator();
		}
		if (entity.HasComponent<NativeScriptComponent>())
		{
			ImGui::Text("NativeScriptComponent");
			auto& nsc = entity.GetComponent<NativeScriptComponent>();
			ImGui::Separator();
		}

		ImGui::Button("Add Component");
	}
}