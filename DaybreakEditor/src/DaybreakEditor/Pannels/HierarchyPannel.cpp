#include "DaybreakEditor/Pannels/HierarchyPannel.h"

#include <imgui.h>

#include "DaybreakEditor/Pannels/SelectionContext.h"

namespace Daybreak
{
	HierarchyPannel::HierarchyPannel(Ref<Scene> scene)
		: m_Scene(scene)
	{
	}

	void HierarchyPannel::OnUpdate(DeltaTime dt)
	{
	}

	void HierarchyPannel::OnAttach()
	{
	}

	void HierarchyPannel::OnDetach()
	{
	}

	void HierarchyPannel::OnImGuiRender()
	{
		ImGui::Begin("Hierarchy Pannel");

		ImVec2 textWidth = ImGui::CalcTextSize(m_Scene->GetName().c_str());
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - textWidth.x) / 2);
		ImGui::Text("%s", m_Scene->GetName().c_str());
		ImGui::Separator();
		ImGui::Text("Entities:");
		ImGui::Separator();
		m_Scene->m_Registry.each([&](auto entityID)
								 {
			Entity entity { entityID, m_Scene.get() };
				DrawEntityRow(entity); });


		ImGui::Separator();
		ImGui::SetNextItemWidth(40.0f);
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - 40.0f);
		if (ImGui::Button("Add Entity"))
		{
			// m_SelectedEntity = m_Scene->CreateEntity("Entity");
			// m_SelectedEntityID = m_SelectedEntity.GetUUID();
			SelectionContext::SetContext(CreateRef<EntityContext>(m_Scene->CreateEntity("Entity")));
		}
		ImGui::End();
	}

	void HierarchyPannel::DrawEntityRow(Entity entity, bool drawRowAsChild)
	{
		std::string& tag = entity.GetComponent<TagComponent>().Tag;
		RelationshipComponent& rc = entity.GetComponent<RelationshipComponent>();
		if (rc.ParentID && !drawRowAsChild)
		{
			return;
		}

		Entity selectedEntity;
		if (SelectionContext::GetContext()->GetContextType() == SelectionContextType::Entity)
		{
			selectedEntity = std::static_pointer_cast<EntityContext>(SelectionContext::GetContext())->GetEntity();
		}

		ImGuiTreeNodeFlags flags = ((selectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		if (rc.AmountOfChildren == 0)
		{
			flags |= ImGuiTreeNodeFlags_Leaf;
		}

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, "%s", tag.c_str());
		if (ImGui::IsItemClicked())
		{
			SelectionContext::SetContext(CreateRef<EntityContext>(entity));
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("HIERARCHY_ENTITY", (void*)&entity, sizeof(Entity));
			ImGui::Text("%s", entity.GetName().c_str());
			ImGui::EndDragDropSource();
		}
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY"))
			{
				Entity payloadEntity = *(Entity*)payload->Data;

				if (payloadEntity.HasParent())
				{
					RemoveEntityAsChild(m_Scene->GetEntityByUUID(payloadEntity.GetParentUUID()), payloadEntity);
				}
				AddEntityAsChild(entity, payloadEntity);
			}
			ImGui::EndDragDropTarget();
		}
		if (drawRowAsChild)
		{
			ImGui::SameLine();
			float currentXPos = ImGui::GetCursorPosX();
			ImGui::SetCursorPosX(0);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
			ImGui::Button("##RemoveParent", { currentXPos / 2, 15 });
			ImGui::PopStyleColor(3);

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY"))
				{
					Entity payloadEntity = *(Entity*)payload->Data;

					RemoveEntityAsChild(m_Scene->GetEntityByUUID(payloadEntity.GetParentUUID()), payloadEntity);
				}
			}
		}

		if (opened)
		{
			for (UUID child : rc.ChildrenIDs)
			{
				DrawEntityRow(m_Scene->GetEntityByUUID(child), true);
			}
			ImGui::TreePop();
		}


		if (entityDeleted)
		{
			m_Scene->DestroyEntity(entity);
			SelectionContext::ResetContext();
		}
	}

	void HierarchyPannel::OnEvent(Event& event)
	{
	}


	void HierarchyPannel::RemoveEntityAsChild(Entity parent, Entity child)
	{
		RelationshipComponent& parentRc = parent.GetComponent<RelationshipComponent>();
		RelationshipComponent& childRc = child.GetComponent<RelationshipComponent>();

		childRc.ParentID = 0;
		std::vector<UUID>::iterator position = std::find(parentRc.ChildrenIDs.begin(), parentRc.ChildrenIDs.end(), child.GetUUID());
		if (position != parentRc.ChildrenIDs.end())
		{
			parentRc.ChildrenIDs.erase(position);
			parentRc.AmountOfChildren--;
		}
	}


	void HierarchyPannel::AddEntityAsChild(Entity parent, Entity child)
	{
		RelationshipComponent& parentRc = parent.GetComponent<RelationshipComponent>();
		RelationshipComponent& childRc = child.GetComponent<RelationshipComponent>();

		childRc.ParentID = parent.GetUUID();
		parentRc.ChildrenIDs.emplace_back(child.GetUUID());
		parentRc.AmountOfChildren++;
	}
}