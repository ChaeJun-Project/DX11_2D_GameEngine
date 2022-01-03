#pragma once

static void ShowInt(const std::string& label_tag, const char* label_name, int& value, const float& size, const float& indent = 0.0f, ImGuiInputTextFlags flags = 0);

static void ShowInt2(const char* label_name, int& value_x, int& value_y, const float& size, const float& indent, ImGuiInputTextFlags flags = 0);

static void ShowFloat(const std::string& label_tag, const char* label_name, float& value, const float& size, const float& indent = 0.0f, ImGuiInputTextFlags flags = 0);

static void ShowFloat2(const char* label_name, Vector2& value, const float& size, const float& indent,  ImGuiInputTextFlags flags = 0);

static void ShowFloat3(const char* label_name, Vector3& value, const float& size, const float& indent,  ImGuiInputTextFlags flags = 0);

static void ShowFloat4(const char* label_name, Vector4& value, const float& size, const float& indent, ImGuiInputTextFlags flags = 0);

static const bool CheckMousePositionInRect(const ImVec2& mouse_position, const ImVec2& rect_left_top, const ImVec2& rect_right_bottom);

static void SaveFile(const std::string& path, const FileType& file_type);

static void SaveScene(const std::string& path);

static void LoadFile(const std::string& path, const FileType& file_type);

static void LoadScene(const std::string& path);