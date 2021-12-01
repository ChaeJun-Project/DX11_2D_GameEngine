#include "stdafx.h"
#include "GUI_Function.h"

void ShowInt(const std::string& label_tag, const char* label_name, int& value, const float& indent)
{
	ImGui::Text(label_name);
	ImGui::SameLine(indent);
	ImGui::PushItemWidth(100.0f);
	std::string label_str = "##" + label_tag + "_" + FileManager::ConvertCharToString(label_name);
	ImGui::InputInt(label_str.c_str(), &value);
	ImGui::PopItemWidth();
}

void ShowFloat(const std::string& label_tag, const char* label_name, float& value, const float& indent)
{
	ImGui::Text(label_name);
	ImGui::SameLine(indent);
	ImGui::PushItemWidth(70.0f);
	std::string label_str = "##" + label_tag + "_" + FileManager::ConvertCharToString(label_name);
	ImGui::InputFloat(label_str.c_str(), &value);
	ImGui::PopItemWidth();
}

void ShowFloat2(const char* label_name, Vector2& value, const float& indent)
{
	ImGui::BeginGroup(); //하나의 그룹으로 묶기
	std::string label_str = FileManager::ConvertCharToString(label_name);
	ImGui::Text(label_name);
	ImGui::SameLine(indent); //같은 라인에서 80만큼 떨어져서 시작

	//Draw X
	ShowFloat(label_str, "X", value.x); ImGui::SameLine(); //같은 라인에 시작
	//Draw Y
	ShowFloat(label_str, "Y", value.y);

	ImGui::EndGroup(); //하나의 그룹으로 묶기 해제
}

void ShowFloat3(const char* label_name, Vector3& value, const float& indent)
{
	ImGui::BeginGroup(); //하나의 그룹으로 묶기
	ImGui::Text(label_name);
	std::string label_str = FileManager::ConvertCharToString(label_name);
	ImGui::SameLine(indent); //같은 라인에서 80만큼 떨어져서 시작

	//Draw X
	ShowFloat(label_str, "X", value.x); ImGui::SameLine(); //같은 라인에 시작
	//Draw Y
	ShowFloat(label_str, "Y", value.y); ImGui::SameLine(); //같은 라인에 시작
	//Draw Z
	ShowFloat(label_str, "Z", value.z);

	ImGui::EndGroup(); //하나의 그룹으로 묶기 해제
}