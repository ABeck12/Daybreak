#include "DaybreakEditor/Pannels/HierarchyPannel.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>
// #include <glm/gtc/type_ptr.hpp>

static void DragVec3(const std::string& label, glm::vec3& vec)
{
	ImGui::PushID(label.c_str());
	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 { 4, 4 });

	ImGui::DragFloat("##X", &vec.x, 0.1f, 0.0f, 0.0f, "X: %.3f");
	ImGui::SameLine();
	ImGui::PopItemWidth();

	ImGui::DragFloat("##Y", &vec.y, 0.1f, 0.0f, 0.0f, "Y: %.3f");
	ImGui::SameLine();
	ImGui::PopItemWidth();

	ImGui::DragFloat(label.c_str(), &vec.z, 0.1f, 0.0f, 0.0f, "Z: %.3f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();
	ImGui::PopID();
}

static void DragVec2(const std::string& label, glm::vec2& vec)
{
	ImGui::PushID(label.c_str());
	ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 { 4, 4 });

	ImGui::DragFloat("##X", &vec.x, 0.1f, 0.0f, 0.0f, "X: %.3f");
	ImGui::SameLine();
	ImGui::PopItemWidth();

	ImGui::DragFloat(label.c_str(), &vec.y, 0.1f, 0.0f, 0.0f, "Y: %.3f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();
	ImGui::PopID();
}

namespace Daybreak
{
	HierarchyPannel::HierarchyPannel(const Ref<Scene> scene)
		: m_ActiveScene(scene)
	{
	}

	void HierarchyPannel::Render()
	{
		ImGui::Begin("Hierarchy");
		ImVec2 textWidth = ImGui::CalcTextSize(m_ActiveScene->GetName().c_str());
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - textWidth.x) / 2);
		ImGui::Text(m_ActiveScene->GetName().c_str());
		ImGui::Separator();
		ImGui::Text("Entities:");
		ImGui::Separator();
		m_ActiveScene->m_Registry.each([&](auto entityID)
									   {Entity entity{ entityID ,m_ActiveScene.get()};
							DrawEntityRow(entity); });

		ImGui::Separator();
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("Add Entity").x) / 2);

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
			m_SelectionContext = entity;
			m_ActiveEntityName = entity.GetName();
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
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strncpy_s(buffer, sizeof(buffer), tag.c_str(), sizeof(buffer));
		if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
		{
			tag = std::string(buffer);
		}

		ImGui::SameLine();

		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("Add Component");
		}
		if (ImGui::BeginPopup("Add Component"))
		{
			ImGui::SeparatorText("Components");
			DisplayAddComponentEntry<CameraComponent>("Camera");
			DisplayAddComponentEntry<SpriteRendererComponent>("SpriteRenderer");
			DisplayAddComponentEntry<Rigidbody2DComponent>("Rigidbody2D");
			DisplayAddComponentEntry<BoxCollider2DComponent>("BoxCollider2D");
			DisplayAddComponentEntry<CircleCollider2DComponent>("CircleCollider2D");

			ImGui::EndPopup();
		}
		ImGui::Separator();

		// ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;

		ImGui::AlignTextToFramePadding();
		if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), flags, "Transform"))
		{
			auto& transform = entity.GetComponent<TransformComponent>();
			DragVec3("Position", transform.Position);
			DragVec3("Rotation", transform.Rotation);
			DragVec3("Scale", transform.Scale);
			ImGui::Separator();
			ImGui::TreePop();
		}
		if (entity.HasComponent<SpriteRendererComponent>())
		{
			ImGui::AlignTextToFramePadding();
			bool open = ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), flags, "SpriteRenderer");
			ImGui::SameLine(ImGui::GetWindowWidth() - 60);
			bool remove = ImGui::Button("Remove");

			if (open)
			{
				auto& sr = entity.GetComponent<SpriteRendererComponent>();
				ImGui::ColorEdit4("Tint Color", &sr.TintColor.x);
				ImGui::InputFloat("Tiling Factor", &sr.TilingFactor);
				// if (sr.TilingFactor < 0)
				// {
				// 	sr.TilingFactor = 0;
				// }
				ImGui::Text("Add sprite filepath");
				ImGui::Separator();
				ImGui::TreePop();
			}
			if (remove)
			{
				entity.RemoveComponent<SpriteRendererComponent>();
			}
		}
		if (entity.HasComponent<CameraComponent>())
		{
			ImGui::AlignTextToFramePadding();
			bool open = ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), flags, "Camera");
			ImGui::SameLine(ImGui::GetWindowWidth() - 60);
			bool remove = ImGui::Button("Remove");

			if (open)
			{
				auto& camera = entity.GetComponent<CameraComponent>();
				ImGui::Checkbox("Primary", &camera.Primary);
				ImGui::Text("Add projection type");
				ImGui::Separator();
				ImGui::TreePop();
			}
			if (remove)
			{
				entity.RemoveComponent<CameraComponent>();
			}
		}
		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			ImGui::AlignTextToFramePadding();
			bool open = ImGui::TreeNodeEx((void*)typeid(BoxCollider2DComponent).hash_code(), flags, "BoxCollider2D");
			ImGui::SameLine(ImGui::GetWindowWidth() - 60);
			bool remove = ImGui::Button("Remove");

			if (open)
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
				DragVec2("Size", bc2d.Size);
				DragVec2("Offset", bc2d.Offset);
				int input = bc2d.CollisionLayer;
				ImGui::InputInt("Collision Layer", &input);
				bc2d.CollisionLayer = (uint32_t)std::clamp<int>(input, 0, 32);
				ImGui::Checkbox("IsTrigger", &bc2d.IsTrigger);
				ImGui::Checkbox("Enabled", &bc2d.Enabled);
				ImGui::Separator();
				ImGui::TreePop();
			}
			if (remove)
			{
				entity.RemoveComponent<BoxCollider2DComponent>();
			}
		}
		if (entity.HasComponent<CircleCollider2DComponent>())
		{
			ImGui::AlignTextToFramePadding();
			bool open = ImGui::TreeNodeEx((void*)typeid(CircleCollider2DComponent).hash_code(), flags, "CircleCollider2D");
			ImGui::SameLine(ImGui::GetWindowWidth() - 60);
			bool remove = ImGui::Button("Remove");

			if (open)
			{
				auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
				ImGui::DragFloat("Radius", &cc2d.Radius, 0.005f, 0.0f, FLT_MAX, "%.3f", ImGuiSliderFlags_AlwaysClamp);
				DragVec2("Offset", cc2d.Offset);
				int input = cc2d.CollisionLayer;
				ImGui::InputInt("Collision Layer", &input);
				cc2d.CollisionLayer = (uint32_t)std::clamp<int>(input, 0, 32);
				ImGui::Checkbox("IsTrigger", &cc2d.IsTrigger);
				ImGui::Checkbox("Enabled", &cc2d.Enabled);
				ImGui::Separator();
				ImGui::TreePop();
			}

			if (remove)
			{
				entity.RemoveComponent<CircleCollider2DComponent>();
			}
		}
		if (entity.HasComponent<Rigidbody2DComponent>())
		{
			ImGui::AlignTextToFramePadding();
			bool open = ImGui::TreeNodeEx((void*)typeid(Rigidbody2DComponent).hash_code(), flags, "Rigidbody2D");
			ImGui::SameLine(ImGui::GetWindowWidth() - 60);
			bool remove = ImGui::Button("Remove");

			if (open)
			{
				auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
				int intType = (int)rb2d.Type;
				ImGui::Combo("Body Type", &intType, "Static\0Dynamic\0Kinematic\0\0");
				rb2d.Type = (Rigidbody2DComponent::BodyType)intType;
				switch (intType)
				{
					case 0: rb2d.Type = Rigidbody2DComponent::BodyType::Static; break;
					case 1: rb2d.Type = Rigidbody2DComponent::BodyType::Dynamic; break;
					case 2: rb2d.Type = Rigidbody2DComponent::BodyType::Kinematic; break;
				}

				ImGui::Checkbox("Continuous Detection", &rb2d.ContinuousDetection);
				ImGui::Checkbox("Allow Sleep", &rb2d.AllowSleep);
				DragVec2("Velocity", rb2d.Velocity);
				ImGui::InputFloat("Mass", &rb2d.Mass);
				ImGui::InputFloat("Gravity Scale", &rb2d.GravityScale);
				ImGui::InputFloat("Angular Drag", &rb2d.AngularDrag);
				ImGui::InputFloat("Linear Drag", &rb2d.LinearDrag);
				ImGui::InputFloat("Density", &rb2d.Density);
				ImGui::InputFloat("Friction", &rb2d.Friction);
				ImGui::InputFloat("Restitution", &rb2d.Restitution);
				ImGui::InputFloat("Restitution Threshold", &rb2d.RestitutionThreshold);
				ImGui::Separator();
				ImGui::TreePop();
			}

			if (remove)
			{
				entity.RemoveComponent<Rigidbody2DComponent>();
			}
		}
		if (entity.HasComponent<AnimatorComponent>())
		{
			ImGui::AlignTextToFramePadding();
			bool open = ImGui::TreeNodeEx((void*)typeid(AnimatorComponent).hash_code(), flags, "Animator");
			ImGui::SameLine(ImGui::GetWindowWidth() - 60);
			bool remove = ImGui::Button("Remove");

			if (open)
			{
				auto& anim = entity.GetComponent<AnimatorComponent>();
				ImGui::Checkbox("IsPlaying", &anim.IsPlaying);
				ImGui::ColorEdit4("Tint Color", &anim.TintColor.x);
				ImGui::Text("Add anim controller filepath");
				ImGui::Separator();
				ImGui::TreePop();
			}

			if (remove)
			{
				entity.RemoveComponent<AnimatorComponent>();
			}
		}
		if (entity.HasComponent<NativeScriptComponent>())
		{
			ImGui::AlignTextToFramePadding();
			bool open = ImGui::TreeNodeEx((void*)typeid(NativeScriptComponent).hash_code(), flags, "NativeScript");
			ImGui::SameLine(ImGui::GetWindowWidth() - 60);
			bool remove = ImGui::Button("Remove");

			if (open)
			{
				auto& nsc = entity.GetComponent<NativeScriptComponent>();
				ImGui::Text(nsc.TypeName.c_str());
				ImGui::Separator();
				ImGui::TreePop();
			}

			if (remove)
			{
				entity.RemoveComponent<AnimatorComponent>();
			}
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

	glm::vec2 HierarchyPannel::GetSelectedEntityPosition()
	{
		DB_CORE_ASSERT(HasSelectedEntity(), "No entity is selected");
		auto pos = m_SelectionContext.GetComponent<TransformComponent>().Position;
		return { pos.x, pos.y };
	}

	void HierarchyPannel::SetScene(const Ref<Scene> scene)
	{
		m_ActiveScene = scene;

		Entity activeEntity = m_ActiveScene->GetEntityByName(m_ActiveEntityName);

		if (activeEntity)
		{
			m_SelectionContext = activeEntity;
		}
		else
		{
			m_SelectionContext = {};
		}
	}
}