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

void SaveFile(const std::string& path, const FileType& file_type);

void SaveScene(const std::string& path);

void LoadFile(const std::string& path, const FileType& file_type);

void LoadScene(const std::string& path);