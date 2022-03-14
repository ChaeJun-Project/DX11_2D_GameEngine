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
	ImGui::BeginGroup(); //하나의 그룹으로 묶기
	std::string label_str = FILE_MANAGER->ConvertCharToString(label_name);
	ImGui::Text(label_name);
	ImGui::SameLine(indent); //같은 라인에서 80만큼 떨어져서 시작

	//Draw X
	ShowInt(label_str, "X", value_x, size, 0.0f, flags); ImGui::SameLine(); //같은 라인에 시작
	//Draw Y
	ShowInt(label_str, "Y", value_y, size, 0.0f, flags);

	ImGui::EndGroup(); //하나의 그룹으로 묶기 해제
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
	ImGui::BeginGroup(); //하나의 그룹으로 묶기
	std::string label_str = FILE_MANAGER->ConvertCharToString(label_name);
	ImGui::Text(label_name);
	ImGui::SameLine(indent); //같은 라인에서 80만큼 떨어져서 시작

	//Draw X
	ShowFloat(label_str, "X", value.x, size, 0.0f, flags); ImGui::SameLine(); //같은 라인에 시작
	//Draw Y
	ShowFloat(label_str, "Y", value.y, size, 0.0f, flags);

	ImGui::EndGroup(); //하나의 그룹으로 묶기 해제
}

void ShowFloat3(const char* label_name, Vector3& value, const float& size, const float& indent, ImGuiInputTextFlags flags)
{
	ImGui::BeginGroup(); //하나의 그룹으로 묶기
	ImGui::Text(label_name);
	std::string label_str = FILE_MANAGER->ConvertCharToString(label_name);
	ImGui::SameLine(indent); //같은 라인에서 80만큼 떨어져서 시작

	//Draw X
	ShowFloat(label_str, "X", value.x, size, 0.0f, flags); ImGui::SameLine(); //같은 라인에 시작
	//Draw Y
	ShowFloat(label_str, "Y", value.y, size, 0.0f, flags); ImGui::SameLine(); //같은 라인에 시작
	//Draw Z
	ShowFloat(label_str, "Z", value.z, size, 0.0f, flags);

	ImGui::EndGroup(); //하나의 그룹으로 묶기 해제
}

void ShowFloat4(const char* label_name, Vector4& value, const float& size, const float& indent, ImGuiInputTextFlags flags)
{
	ImGui::BeginGroup(); //하나의 그룹으로 묶기
	ImGui::Text(label_name);
	std::string label_str = FILE_MANAGER->ConvertCharToString(label_name);
	ImGui::SameLine(indent); //같은 라인에서 80만큼 떨어져서 시작

	//Draw X
	ShowFloat(label_str, "X", value.x, size, 0.0f, flags); ImGui::SameLine(); //같은 라인에 시작
	//Draw Y
	ShowFloat(label_str, "Y", value.y, size, 0.0f, flags); ImGui::SameLine(); //같은 라인에 시작
	//Draw Z
	ShowFloat(label_str, "Z", value.z, size, 0.0f, flags); ImGui::SameLine(); //같은 라인에 시작
	//Draw W
	ShowFloat(label_str, "W", value.z, size, 0.0f, flags);

	ImGui::EndGroup(); //하나의 그룹으로 묶기 해제
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
	//현재 마우스 커서의 위치가 Rect 내부에 있는 경우
	if (rect_left_top.x < mouse_position.x && rect_left_top.y < mouse_position.y &&
		rect_right_bottom.x > mouse_position.x && rect_right_bottom.y > mouse_position.y)
		return true;

	else
		return false;
}

void DataInputInt(const std::string& data_name, int* p_data, const float& size, const float& indent, ImGuiInputTextFlags flags)
{
	ShowInt("Script", data_name.c_str(), *p_data, size, indent, flags);
}

void DataInputFloat(const std::string& data_name, float* p_data, const float& size, const float& indent, ImGuiInputTextFlags flags)
{
	ShowFloat("Script", data_name.c_str(), *p_data, size, indent, flags);
}

void DataInputVector2(const std::string& data_name, Vector2* p_data, const float& size, const float& indent, ImGuiInputTextFlags flags)
{
	ShowFloat2(data_name.c_str(), *p_data, size, indent, flags);
}

void DataInputVector3(const std::string& data_name, Vector3* p_data, const float& size, const float& indent, ImGuiInputTextFlags flags)
{
	ShowFloat3(data_name.c_str(), *p_data, size, indent, flags);
}

void DataInputVector4(const std::string& data_name, Vector4* p_data, const float& size, const float& indent, ImGuiInputTextFlags flags)
{
	ShowFloat4(data_name.c_str(), *p_data, size, indent, flags);
}

void DataInputResource(const std::string& data_name, const IResource* p_resource, const float& size, const float& indent)
{
	ImGui::Text(data_name.c_str());
	ImGui::SameLine(indent);

	auto label_str = data_name;
	label_str = "##" + label_str;

	std::string resource_name;
	if (p_resource != nullptr)
		resource_name = p_resource->GetResourceName();

	ImGui::PushItemWidth(size);
	ImGui::InputText(label_str.c_str(), &resource_name, ImGuiInputTextFlags_ReadOnly);
	ImGui::PopItemWidth();
}

#include <DX11_2D_GameEngine_Lib/Texture.h>
void DataInputTexture(const std::string& data_name, Texture** pp_texture, const float& size, const float& indent)
{
	DataInputResource(data_name, *pp_texture, size, indent);

	if (auto pay_load = DragDropEvent::ReceiveDragDropPayLoad(PayLoadType::Texture))
	{
		auto p_texture = RESOURCE_MANAGER->LoadFromFile<Texture>(std::get<std::string>(pay_load->data));
		(*pp_texture) = p_texture.get();
	}
}

#include <DX11_2D_GameEngine_Lib/Prefab.h>
void DataInputPrefab(const std::string& data_name, Prefab** pp_prefab, const float& size, const float& indent)
{
	DataInputResource(data_name, *pp_prefab, size, indent);

	if (auto pay_load = DragDropEvent::ReceiveDragDropPayLoad(PayLoadType::Prefab))
	{
		auto p_prefab = RESOURCE_MANAGER->LoadFromFile<Prefab>(std::get<std::string>(pay_load->data));
		(*pp_prefab) = p_prefab.get();
	}
}