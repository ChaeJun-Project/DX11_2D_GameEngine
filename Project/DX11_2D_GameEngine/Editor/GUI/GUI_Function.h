#pragma once

static void ShowInt(const std::string& label_tag, const char* label_name, int& value, const float& indent = 0.0f);

static void ShowFloat(const std::string& label_tag, const char* label_name, float& value, const float& indent = 0.0f);

static void ShowFloat2(const char* label_name, Vector2& value, const float& indent);

static void ShowFloat3(const char* label_name, Vector3& value, const float& indent);