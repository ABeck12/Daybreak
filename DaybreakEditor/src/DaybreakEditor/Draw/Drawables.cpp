#include "DaybreakEditor/Draw/Drawables.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

namespace Daybreak::DrawField
{
	void DragVec2(const std::string& label, glm::vec2& vec, float min, float max, int flags)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		float fieldNameWidth = window->InnerRect.GetWidth() * 0.35f;
		ImGui::PushID(label.c_str());
		ImGui::Columns(2, NULL, false);
		ImGui::SetColumnWidth(0, fieldNameWidth);
		float padding = 4;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 { padding, padding });

		ImGui::PushTextWrapPos();
		ImGui::Text("%s", label.c_str());
		ImGui::PopTextWrapPos();
		ImGui::NextColumn();
		float totalWidth = window->InnerRect.GetWidth() - fieldNameWidth;
		float textWidth = ImGui::CalcTextSize("X").x;
		float buttonWidth = (totalWidth - (2 * textWidth)) / 2 - (4 * padding);

		ImGui::SetNextItemWidth(textWidth);
		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(buttonWidth);
		ImGui::DragFloat("##X", &vec.x, 0.1f, min, max, "%.3f", flags);
		ImGui::SameLine();

		ImGui::SetNextItemWidth(textWidth);
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(buttonWidth);
		ImGui::DragFloat("##Y", &vec.y, 0.1f, min, max, "%.3f", flags);

		ImGui::PopStyleVar();
		ImGui::PopID();
		ImGui::Columns(1);
	}

	void DragVec3(const std::string& label, glm::vec3& vec, float min, float max, int flags)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		float fieldNameWidth = window->InnerRect.GetWidth() * 0.35f;
		ImGui::PushID(label.c_str());
		ImGui::Columns(2, NULL, false);
		ImGui::SetColumnWidth(0, fieldNameWidth);
		float padding = 4;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 { padding, padding });

		ImGui::PushTextWrapPos();
		ImGui::Text("%s", label.c_str());
		ImGui::PopTextWrapPos();
		ImGui::NextColumn();

		float totalWidth = window->InnerRect.GetWidth() - fieldNameWidth;
		float textWidth = ImGui::CalcTextSize("X").x;
		float buttonWidth = (totalWidth - (3 * textWidth)) / 3 - (4 * padding);

		ImGui::SetNextItemWidth(textWidth);
		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(buttonWidth);
		ImGui::DragFloat("##X", &vec.x, 0.1f, min, max, "%.3f", flags);
		ImGui::SameLine();

		ImGui::SetNextItemWidth(textWidth);
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(buttonWidth);
		ImGui::DragFloat("##Y", &vec.y, 0.1f, min, max, "%.3f", flags);
		ImGui::SameLine();

		ImGui::SetNextItemWidth(textWidth);
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(buttonWidth);
		ImGui::DragFloat("##Z", &vec.z, 0.1f, min, max, "%.3f", flags);

		ImGui::PopStyleVar();
		ImGui::PopID();
		ImGui::Columns(1);
	}

	void DragVec4(const std::string& label, glm::vec4& vec, float min, float max, int flags)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		float fieldNameWidth = window->InnerRect.GetWidth() * 0.35f;
		ImGui::PushID(label.c_str());
		ImGui::Columns(2, NULL, false);
		ImGui::SetColumnWidth(0, fieldNameWidth);
		float padding = 4;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 { padding, padding });

		ImGui::PushTextWrapPos();
		ImGui::Text("%s", label.c_str());
		ImGui::PopTextWrapPos();
		ImGui::NextColumn();

		float totalWidth = window->InnerRect.GetWidth() - fieldNameWidth;
		float textWidth = ImGui::CalcTextSize("X").x;
		float buttonWidth = (totalWidth - (4 * textWidth)) / 4 - (4 * padding);

		ImGui::SetNextItemWidth(textWidth);
		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(buttonWidth);
		ImGui::DragFloat("##X", &vec.x, 0.1f, min, max, "%.3f", flags);
		ImGui::SameLine();

		ImGui::SetNextItemWidth(textWidth);
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(buttonWidth);
		ImGui::DragFloat("##Y", &vec.y, 0.1f, min, max, "%.3f", flags);
		ImGui::SameLine();

		ImGui::SetNextItemWidth(textWidth);
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(buttonWidth);
		ImGui::DragFloat("##Z", &vec.z, 0.1f, min, max, "%.3f", flags);
		ImGui::SameLine();

		ImGui::SetNextItemWidth(textWidth);
		ImGui::Text("W");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(buttonWidth);
		ImGui::DragFloat("##W", &vec.w, 0.1f, min, max, "%.3f", flags);

		ImGui::PopStyleVar();
		ImGui::PopID();
		ImGui::Columns(1);
	}

	void DragFloat(const std::string& label, float& value, float min, float max, int flags)
	{
		ImGui::PushID(label.c_str());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 { 4, 4 });
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		float width = window->InnerRect.GetWidth();

		ImGui::Text("%s", label.c_str());
		ImGui::SameLine();
		ImGui::SetCursorPosX(width / 2);
		ImGui::SetNextItemWidth(width / 2);
		ImGui::DragFloat("##X", &value, 0.1f, min, max, "%.3f", flags);

		ImGui::PopStyleVar();
		ImGui::PopID();
	}

	void DragInt(const std::string& label, int& value)
	{
		ImGui::PushID(label.c_str());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 { 4, 4 });
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		float width = window->InnerRect.GetWidth();

		ImGui::Text("%s", label.c_str());
		ImGui::SameLine();
		ImGui::SetNextItemWidth(width / 2);
		ImGui::SetCursorPosX(width / 2);
		ImGui::DragInt("##X", &value, 0.1f, 0, 0);

		ImGui::PopStyleVar();
		ImGui::PopID();
	}

	void InputInt(const std::string& label, int& value)
	{
		ImGui::PushID(label.c_str());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 { 4, 4 });
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		float width = window->InnerRect.GetWidth();

		ImGui::Text("%s", label.c_str());
		ImGui::SameLine();
		ImGui::SetNextItemWidth(width / 2);
		ImGui::SetCursorPosX(width / 2);
		ImGui::InputInt("##X", &value);

		ImGui::PopStyleVar();
		ImGui::PopID();
	}

	void Checkbox(const std::string& label, bool& value)
	{
		ImGui::PushID(label.c_str());
		ImGui::Text("%s", label.c_str());
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		float width = window->InnerRect.GetWidth();
		ImGui::SameLine();
		ImGui::SetCursorPosX(width / 2);

		ImGui::Checkbox("##", &value);
		ImGui::PopID();
	}

	void Combobox(const std::string& label, int& currentItem, const char* items)
	{
		ImGui::PushID(label.c_str());
		ImGui::Text("%s", label.c_str());
		ImGui::SameLine();
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		float width = window->InnerRect.GetWidth();

		ImGui::SetCursorPosX(width / 2);
		ImGui::SetNextItemWidth(width / 2);

		ImGui::Combo(label.c_str(), &currentItem, items);

		ImGui::PopID();
	}

	void InputText(const std::string& label, std::string& text)
	{
		ImGui::PushID(label.c_str());
		ImGui::Text("%s", label.c_str());
		ImGui::SameLine();
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		float width = window->InnerRect.GetWidth();
		ImGui::SetCursorPosX(width / 2);
		ImGui::SetNextItemWidth(width / 2);

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strncpy_s(buffer, sizeof(buffer), text.c_str(), sizeof(buffer));
		if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
		{
			text = std::string(buffer);
		}
		ImGui::PopID();
	}

	void Text(const std::string& label, const std::string& text)
	{
		ImGui::PushID(label.c_str());
		ImGui::Text("%s", label.c_str());
		ImGui::SameLine();
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		float width = window->InnerRect.GetWidth();
		ImGui::SetCursorPosX(width / 2);
		ImGui::SetNextItemWidth(width / 2);
		ImGui::Text("%s", text.c_str());
		ImGui::PopID();
	}

}