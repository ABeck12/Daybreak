#pragma once

#include <Daybreak.h>

namespace Daybreak::DrawField
// TODO: Rename to fields?
{
	void DragVec2(const std::string& label, glm::vec2& vec, float min = 0.0f, float max = 0.0f, int flags = 0);
	void DragVec3(const std::string& label, glm::vec3& vec, float min = 0.0f, float max = 0.0f, int flags = 0);
	void DragVec4(const std::string& label, glm::vec4& vec, float min = 0.0f, float max = 0.0f, int flags = 0);

	void DragFloat(const std::string& label, float& value, float min = 0.0f, float max = 0.0f, int flags = 0);

	void DragInt(const std::string& label, int& value);
	void InputInt(const std::string& label, int& value);

	void Checkbox(const std::string& label, bool& value);
	void Combobox(const std::string& label, int& currentItem, const char* items);

	void InputText(const std::string& label, std::string& text);
	void Text(const std::string& label, const std::string& text);
}