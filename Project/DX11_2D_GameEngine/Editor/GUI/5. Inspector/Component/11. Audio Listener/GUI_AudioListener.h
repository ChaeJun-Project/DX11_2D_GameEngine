#pragma once
#include "../GUI_Component.h"

class GUI_AudioListener final : public GUI_Component
{
public:
	GUI_AudioListener(const std::string& audio_listener_gui_name);
	~GUI_AudioListener() = default;

	void Render() override;
};
