#pragma once
#include "../GUI_Component.h"

class GUI_ItemList;

class GUI_AudioSource final : public GUI_Component
{
public:
	GUI_AudioSource(const std::string& audio_source_gui_name);
	~GUI_AudioSource();

public:
	void Render() override;

private:
	//Audio Clip List
	std::unique_ptr<GUI_ItemList> m_p_audio_clip_list = nullptr;
};

