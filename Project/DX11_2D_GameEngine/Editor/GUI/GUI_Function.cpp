#include "stdafx.h"
#include "GUI_Function.h"

void ShowInt(const std::string& label_tag, const char* label_name, int& value, const float& indent, ImGuiInputTextFlags flags)
{
	ImGui::Text(label_name);
	ImGui::SameLine(indent);
	ImGui::PushItemWidth(100.0f);
	std::string label_str = "##" + label_tag + "_" + FileManager::ConvertCharToString(label_name);
	ImGui::InputInt(label_str.c_str(), &value, flags);
	ImGui::PopItemWidth();
}

void ShowInt2(const char* label_name, int& value_x, int& value_y, const float& indent, ImGuiInputTextFlags flags)
{
	ImGui::BeginGroup(); //�ϳ��� �׷����� ����
	std::string label_str = FileManager::ConvertCharToString(label_name);
	ImGui::Text(label_name);
	ImGui::SameLine(indent); //���� ���ο��� 80��ŭ �������� ����

	//Draw X
	ShowInt(label_str, "X", value_x, 0.0f, flags); ImGui::SameLine(); //���� ���ο� ����
	//Draw Y
	ShowInt(label_str, "Y", value_y, 0.0f, flags);

	ImGui::EndGroup(); //�ϳ��� �׷����� ���� ����
}

void ShowFloat(const std::string& label_tag, const char* label_name, float& value, const float& indent, ImGuiInputTextFlags flags)
{
	ImGui::Text(label_name);
	ImGui::SameLine(indent);
	ImGui::PushItemWidth(70.0f);
	std::string label_str = "##" + label_tag + "_" + FileManager::ConvertCharToString(label_name);
	ImGui::InputFloat(label_str.c_str(), &value);
	ImGui::PopItemWidth();
}

void ShowFloat2(const char* label_name, Vector2& value, const float& indent, ImGuiInputTextFlags flags)
{
	ImGui::BeginGroup(); //�ϳ��� �׷����� ����
	std::string label_str = FileManager::ConvertCharToString(label_name);
	ImGui::Text(label_name);
	ImGui::SameLine(indent); //���� ���ο��� 80��ŭ �������� ����

	//Draw X
	ShowFloat(label_str, "X", value.x, 0.0f, flags); ImGui::SameLine(); //���� ���ο� ����
	//Draw Y
	ShowFloat(label_str, "Y", value.y, 0.0f, flags);

	ImGui::EndGroup(); //�ϳ��� �׷����� ���� ����
}

void ShowFloat3(const char* label_name, Vector3& value, const float& indent, ImGuiInputTextFlags flags)
{
	ImGui::BeginGroup(); //�ϳ��� �׷����� ����
	ImGui::Text(label_name);
	std::string label_str = FileManager::ConvertCharToString(label_name);
	ImGui::SameLine(indent); //���� ���ο��� 80��ŭ �������� ����

	//Draw X
	ShowFloat(label_str, "X", value.x, 0.0f, flags); ImGui::SameLine(); //���� ���ο� ����
	//Draw Y
	ShowFloat(label_str, "Y", value.y, 0.0f, flags); ImGui::SameLine(); //���� ���ο� ����
	//Draw Z
	ShowFloat(label_str, "Z", value.z, 0.0f, flags);

	ImGui::EndGroup(); //�ϳ��� �׷����� ���� ����
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
