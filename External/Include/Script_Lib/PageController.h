#pragma once

class AudioSource;

class PageController
{
public:
	PageController() = default;
	virtual ~PageController();

public:
	void SetActiveMenuPageCallBack(std::function<void(void)> p_active_menu_page_func)
	{
		m_p_active_menu_page_func = p_active_menu_page_func;
	}

	void SetActivePlayerSelectPageCallBack(std::function<void(void)> p_active_player_select_page_func)
	{
		m_p_active_player_select_page_func = p_active_player_select_page_func;
	}

	void SetActiveLoadingPageCallBack(std::function<void(void)> p_active_loading_page_func)
	{
		m_p_active_loading_page_func = p_active_loading_page_func;
	}

protected:
	void SetCurrentAudioClip(const std::string& audio_clip_name, const float& volume, const bool& is_loop = false);

protected:
#pragma region Page Event CallBack
	std::function<void(void)> m_p_active_menu_page_func = nullptr;
	std::function<void(void)> m_p_active_player_select_page_func = nullptr;
	std::function<void(void)> m_p_active_loading_page_func = nullptr;
#pragma endregion

#pragma region Audio Source
	AudioSource* m_p_audio_source = nullptr;
#pragma endregion

    bool m_is_focus = false;
};

