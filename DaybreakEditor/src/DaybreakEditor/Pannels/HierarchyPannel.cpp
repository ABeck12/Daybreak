#include "DaybreakEditor/Pannels/HierarchyPannel.h"

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
			m_SelectionContext = m_ActiveScene->CreateEntity("Entity");
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
			// ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			// bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			// if (opened)
			// 	ImGui::TreePop();
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

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;

		if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), flags, "TransformComponent"))
		{
			auto& transform = entity.GetComponent<TransformComponent>();
			ImGui::InputFloat3("Position", &transform.Position.x);
			ImGui::InputFloat3("Rotation", &transform.Rotation.x);
			ImGui::InputFloat3("Scale", &transform.Scale.x);
			ImGui::Separator();
			ImGui::TreePop();
		}
		if (entity.HasComponent<SpriteRendererComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), flags, "SpriteRendererComponent"))
			{
				auto& sr = entity.GetComponent<SpriteRendererComponent>();
				ImGui::InputFloat4("Tint Color", &sr.TintColor.x);
				ImGui::InputFloat("Tiling Factor", &sr.TilingFactor);
				ImGui::Text("Add sprite filepath");
				if (ImGui::Button("Remove"))
				{
					entity.RemoveComponent<SpriteRendererComponent>();
				}
				ImGui::Separator();
				ImGui::TreePop();
			}
		}
		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), flags, "CameraComponent"))
			{
				auto& camera = entity.GetComponent<CameraComponent>();
				ImGui::Checkbox("Primary", &camera.Primary);
				ImGui::Text("Add projection type");
				if (ImGui::Button("Remove"))
				{
					entity.RemoveComponent<CameraComponent>();
				}
				ImGui::Separator();
				ImGui::TreePop();
			}
		}
		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(BoxCollider2DComponent).hash_code(), flags, "BoxCollider2DComponent"))
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
				ImGui::InputFloat2("Size", &bc2d.Size.x);
				ImGui::InputFloat2("Offset", &bc2d.Offset.x);
				ImGui::Text("Add Collision Layer here TODO");
				// ImGui::InputInt("Collision Layer", &((int)bc2d.CollisionLayer));
				ImGui::Checkbox("IsTriggr", &bc2d.IsTrigger);
				ImGui::Checkbox("Enabled", &bc2d.Enabled);
				if (ImGui::Button("Remove"))
				{
					entity.RemoveComponent<BoxCollider2DComponent>();
				}
				ImGui::Separator();
				ImGui::TreePop();
			}
		}
		if (entity.HasComponent<CircleCollider2DComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CircleCollider2DComponent).hash_code(), flags, "CircleCollider2DComponent"))
			{
				auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
				ImGui::InputFloat("Radius", &cc2d.Radius);
				ImGui::InputFloat2("Offset", &cc2d.Offset.x);
				ImGui::Text("Add Collision Layer here TODO");
				// ImGui::InputInt("Collision Layer", &((int)cc2d.CollisionLayer));
				ImGui::Checkbox("IsTriggr", &cc2d.IsTrigger);
				ImGui::Checkbox("Enabled", &cc2d.Enabled);
				if (ImGui::Button("Remove"))
				{
					entity.RemoveComponent<CircleCollider2DComponent>();
				}
				ImGui::Separator();
				ImGui::TreePop();
			}
		}
		if (entity.HasComponent<Rigidbody2DComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(Rigidbody2DComponent).hash_code(), flags, "Rigidbody2DComponent"))
			{
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
				if (ImGui::Button("Remove"))
				{
					entity.RemoveComponent<Rigidbody2DComponent>();
				}

				ImGui::Separator();
				ImGui::TreePop();
			}
		}
		if (entity.HasComponent<AnimatorComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(AnimatorComponent).hash_code(), flags, "AnimatorComponent"))
			{
				auto& anim = entity.GetComponent<AnimatorComponent>();
				ImGui::Checkbox("IsPlaying", &anim.IsPlaying);
				ImGui::InputFloat4("Tint Color", &anim.TintColor.x);
				ImGui::Text("Add anim controller filepath");
				if (ImGui::Button("Remove"))
				{
					entity.RemoveComponent<AnimatorComponent>();
				}
				ImGui::Separator();
				ImGui::TreePop();
			}
		}
		if (entity.HasComponent<NativeScriptComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(NativeScriptComponent).hash_code(), flags, "NativeScriptComponent"))
			{
				auto& nsc = entity.GetComponent<NativeScriptComponent>();
				ImGui::Text(nsc.TypeName.c_str());
				if (ImGui::Button("Remove"))
				{
					entity.RemoveComponent<NativeScriptComponent>();
				}
				ImGui::Separator();
				ImGui::TreePop();
			}
		}


		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("Add Component");
		}
		if (ImGui::BeginPopup("Add Component"))
		{
			DisplayAddComponentEntry<CameraComponent>("Camera");
			DisplayAddComponentEntry<SpriteRendererComponent>("SpriteRenderer");
			DisplayAddComponentEntry<Rigidbody2DComponent>("Rigidbody2D");
			DisplayAddComponentEntry<BoxCollider2DComponent>("BoxCollider2D");
			DisplayAddComponentEntry<CircleCollider2DComponent>("CircleCollider2D");

			ImGui::EndPopup();
		}
	}

	template<typename T>
	void HierarchyPannel::DisplayAddComponentEntry(const std::string& entryName)
	{
		if (!m_SelectionContext.HasComponent<T>())
		{
			if (ImGui::MenuItem(entryName.c_str()))
			{
				m_SelectionContext.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}
}