#pragma once

static void ShowInt(const std::string& label_tag, const char* label_name, int& value, const float& indent = 0.0f, ImGuiInputTextFlags flags = 0);

static void ShowInt2(const char* label_name, int& value_x, int& value_y, const float& indent, ImGuiInputTextFlags flags = 0);

static void ShowFloat(const std::string& label_tag, const char* label_name, float& value, const float& indent = 0.0f, ImGuiInputTextFlags flags = 0);

static void ShowFloat2(const char* label_name, Vector2& value, const float& indent, ImGuiInputTextFlags flags = 0);

static void ShowFloat3(const char* label_name, Vector3& value, const float& indent, ImGuiInputTextFlags flags = 0);

static const bool CheckMousePositionInRect(const ImVec2& mouse_position, const ImVec2& rect_left_top, const ImVec2& rect_right_bottom);
