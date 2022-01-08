#include "stdafx.h"
#include "FontManager.h"

FontManager::~FontManager()
{
	SAFE_RELEASE(m_p_fw1_font_wrapper);
	SAFE_RELEASE(m_p_fw1_factory);
}

void FontManager::Initialize()
{
	auto result = FW1CreateFactory(FW1_VERSION, &m_p_fw1_factory);
	assert(result == S_OK);

	result = m_p_fw1_factory->CreateFontWrapper(DEVICE, L"Arial", &m_p_fw1_font_wrapper);
	assert(result == S_OK);
}

void FontManager::DrawFont(const std::string& font, const float& font_size, const UINT& font_color, const float& window_pos_x, const float& window_pos_y)
{
	if (m_p_fw1_font_wrapper != nullptr)
		m_p_fw1_font_wrapper->DrawString
		(
			DEVICE_CONTEXT,
			FileManager::ConvertStringToWString(font).c_str(),
			font_size,
			window_pos_x,
			window_pos_y,
			font_color,
			0
		);
}

