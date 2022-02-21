#include "stdafx.h"
#include "GUI_Function.h"

void ShowInt(const std::string& label_tag, const char* label_name, int& value, const float& size, const float& indent, ImGuiInputTextFlags flags)
{
	ImGui::Text(label_name);
	ImGui::SameLine(indent);
	ImGui::PushItemWidth(size);
	std::string label_str = "##" + label_tag + "_" + FILE_MANAGER->ConvertCharToString(label_name);
	ImGui::InputInt(label_str.c_str(), &value, flags);
	ImGui::PopItemWidth();
}

void ShowInt2(const char* label_name, int& value_x, int& value_y, const float& size, const float& indent, ImGuiInputTextFlags flags)
{
	ImGui::BeginGroup(); //�ϳ��� �׷����� ����
	std::string label_str = FILE_MANAGER->ConvertCharToString(label_name);
	ImGui::Text(label_name);
	ImGui::SameLine(indent); //���� ���ο��� 80��ŭ �������� ����

	//Draw X
	ShowInt(label_str, "X", value_x, size, 0.0f, flags); ImGui::SameLine(); //���� ���ο� ����
	//Draw Y
	ShowInt(label_str, "Y", value_y, size, 0.0f, flags);

	ImGui::EndGroup(); //�ϳ��� �׷����� ���� ����
}

void ShowFloat(const std::string& label_tag, const char* label_name, float& value, const float& size, const float& indent, ImGuiInputTextFlags flags)
{
	ImGui::Text(label_name);
	ImGui::SameLine(indent);
	ImGui::PushItemWidth(size);
	std::string label_str = "##" + label_tag + "_" + FILE_MANAGER->ConvertCharToString(label_name);
	ImGui::InputFloat(label_str.c_str(), &value);
	ImGui::PopItemWidth();
}

void ShowFloat2(const char* label_name, Vector2& value, const float& size, const float& indent, ImGuiInputTextFlags flags)
{
	ImGui::BeginGroup(); //�ϳ��� �׷����� ����
	std::string label_str = FILE_MANAGER->ConvertCharToString(label_name);
	ImGui::Text(label_name);
	ImGui::SameLine(indent); //���� ���ο��� 80��ŭ �������� ����

	//Draw X
	ShowFloat(label_str, "X", value.x, size, 0.0f, flags); ImGui::SameLine(); //���� ���ο� ����
	//Draw Y
	ShowFloat(label_str, "Y", value.y, size, 0.0f, flags);

	ImGui::EndGroup(); //�ϳ��� �׷����� ���� ����
}

void ShowFloat3(const char* label_name, Vector3& value, const float& size, const float& indent, ImGuiInputTextFlags flags)
{
	ImGui::BeginGroup(); //�ϳ��� �׷����� ����
	ImGui::Text(label_name);
	std::string label_str = FILE_MANAGER->ConvertCharToString(label_name);
	ImGui::SameLine(indent); //���� ���ο��� 80��ŭ �������� ����

	//Draw X
	ShowFloat(label_str, "X", value.x, size, 0.0f, flags); ImGui::SameLine(); //���� ���ο� ����
	//Draw Y
	ShowFloat(label_str, "Y", value.y, size, 0.0f, flags); ImGui::SameLine(); //���� ���ο� ����
	//Draw Z
	ShowFloat(label_str, "Z", value.z, size, 0.0f, flags);

	ImGui::EndGroup(); //�ϳ��� �׷����� ���� ����
}

void ShowFloat4(const char* label_name, Vector4& value, const float& size, const float& indent, ImGuiInputTextFlags flags)
{
	ImGui::BeginGroup(); //�ϳ��� �׷����� ����
	ImGui::Text(label_name);
	std::string label_str = FILE_MANAGER->ConvertCharToString(label_name);
	ImGui::SameLine(indent); //���� ���ο��� 80��ŭ �������� ����

	//Draw X
	ShowFloat(label_str, "X", value.x, size, 0.0f, flags); ImGui::SameLine(); //���� ���ο� ����
	//Draw Y
	ShowFloat(label_str, "Y", value.y, size, 0.0f, flags); ImGui::SameLine(); //���� ���ο� ����
	//Draw Z
	ShowFloat(label_str, "Z", value.z, size, 0.0f, flags); ImGui::SameLine(); //���� ���ο� ����
	//Draw W
	ShowFloat(label_str, "W", value.z, size, 0.0f, flags);

	ImGui::EndGroup(); //�ϳ��� �׷����� ���� ����
}

void ShowColorPicker3(const std::string& label_name, float* data, const ImGuiColorEditFlags& flags)
{
	ImGui::Text(label_name.c_str());
	ImGui::SameLine(100.0f);

	std::string label_tag = "##ColorPicker_" + label_name;
	ImGui::ColorEdit3(label_tag.c_str(), data, flags);
}

void ShowColorPicker4(const std::string& label_name, float* data, const ImGuiColorEditFlags& flags)
{
	ImGui::Text(label_name.c_str());
	ImGui::SameLine(100.0f);

	std::string label_tag = "##ColorPicker_" + label_name;
	ImGui::ColorEdit4(label_tag.c_str(), data, flags);
}

const bool CheckMousePositionInRect(const ImVec2& mouse_position, const ImVec2& rect_left_top, const ImVec2& rect_right_bottom)
{
	//���� ���콺 Ŀ���� ��ġ�� Rect ���ο� �ִ� ���
	if (rect_left_top.x < mouse_position.x && rect_left_top.y < mouse_position.y &&
		rect_right_bottom.x > mouse_position.x && rect_right_bottom.y > mouse_position.y)
		return true;

	else
		return false;
}

void DataInputInt(const std::string& data_name, int* p_data)
{
}

void DataInputFloat(const std::string& data_name, float* p_data)
{
}

void DataInputVector2(const std::string& data_name, Vector2* p_data)
{
}

void DataInputVector3(const std::string& data_name, Vector3* p_data)
{
}

void DataInputVector4(const std::string& data_name, Vector4* p_data)
{
}

void DataInputTexture(const std::string& data_name, Texture* p_data)
{
}

void DataInputGameObject(const std::string& data_name, GameObject* p_data)
{
}
