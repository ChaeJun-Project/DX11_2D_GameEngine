#pragma once
#include "../GUI_Component.h"

class GUI_AudioSource : public GUI_Component
{
public:
	GUI_AudioSource(const std::string& audio_source_gui_name);
	~GUI_AudioSource() = default;

	void Render() override;
};

