#pragma once

void ShowInt(const std::string& label_tag, const char* label_name, int& value, const float& size, const float& indent = 0.0f, ImGuiInputTextFlags flags = 0);

void ShowInt2(const char* label_name, int& value_x, int& value_y, const float& size, const float& indent, ImGuiInputTextFlags flags = 0);

void ShowFloat(const std::string& label_tag, const char* label_name, float& value, const float& size, const float& indent = 0.0f, ImGuiInputTextFlags flags = 0);

void ShowFloat2(const char* label_name, Vector2& value, const float& size, const float& indent,  ImGuiInputTextFlags flags = 0);

void ShowFloat3(const char* label_name, Vector3& value, const float& size, const float& indent,  ImGuiInputTextFlags flags = 0);

void ShowFloat4(const char* label_name, Vector4& value, const float& size, const float& indent, ImGuiInputTextFlags flags = 0);

void ShowColorPicker3(const std::string& label_name, float* data, const ImGuiColorEditFlags& flags);

void ShowColorPicker4(const std::string& label_name, float* data, const ImGuiColorEditFlags& flags);

const bool CheckMousePositionInRect(const ImVec2& mouse_position, const ImVec2& rect_left_top, const ImVec2& rect_right_bottom);

//DataInput
void DataInputInt(const std::string& data_name, int* p_data, const float& size, const float& indent, ImGuiInputTextFlags flags = 0);
void DataInputFloat(const std::string& data_name, float* p_data, const float& size, const float& indent, ImGuiInputTextFlags flags = 0);
void DataInputVector2(const std::string& data_name, Vector2* p_data, const float& size, const float& indent, ImGuiInputTextFlags flags = 0);
void DataInputVector3(const std::string& data_name, Vector3* p_data, const float& size, const float& indent, ImGuiInputTextFlags flags = 0);
void DataInputVector4(const std::string& data_name, Vector4* p_data, const float& size, const float& indent, ImGuiInputTextFlags flags = 0);
void DataInputResource(const std::string& data_name, const IResource* p_resource, const float& size, const float& indent);
void DataInputString(const std::string& data_name, std::string* p_data, const float& size, const float& indent, ImGuiInputTextFlags flags = 0);
void DataInputTexture(const std::string& data_name, Texture** pp_texture, const float& size, const float& indent);
void DataInputPrefab(const std::string& data_name, Prefab** pp_prefab, const float& size, const float& indent);