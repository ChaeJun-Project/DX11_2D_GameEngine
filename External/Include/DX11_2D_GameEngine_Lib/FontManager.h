#pragma once

#include <FW1FontWrapper/FW1FontWrapper.h>
#include <FW1FontWrapper/FW1CompileSettings.h>

#ifdef _DEBUG
#pragma comment(lib, "FW1FontWrapper/Debug/FW1FontWrapper_Debug.lib")
#else
#pragma comment(lib, "FW1FontWrapper/Release/FW1FontWrapper_Release.lib")
#endif

//32��Ʈ�� 8��Ʈ�� ������ RGBA���� �Ҵ� (0 ~ 255)
#define FONT_RGBA(r, g, b, a) (((((BYTE)a << 24 ) | (BYTE)b << 16) | (BYTE)g << 8) | (BYTE)r)

//<summary>
//https://www.gamedev.net/forums/topic/597917-d3d11-drawing-text/ (������ ���� �ڵ� ����)
//</summary>
class FontManager final : public Singleton<FontManager>
{
	SINGLETON(FontManager);

	FontManager() = default;
	~FontManager();

public:
	void Initialize();

public:
	void DrawFont
	(
		const std::string& font,
		const float& font_size,
		const UINT& font_color,
		const float& window_pos_x,
		const float& window_pos_y
	);

private:
	IFW1Factory* m_p_fw1_factory = nullptr;
	IFW1FontWrapper* m_p_fw1_font_wrapper = nullptr;
};

