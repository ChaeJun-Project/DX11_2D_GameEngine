#pragma once

#include "../GUI_Component.h"

class GUI_Script final : public GUI_Component
{
public:
	GUI_Script(const std::string& script_gui_name);
	~GUI_Script() = default;

	void Render() override;

public:
    void SetScriptName(const std::string& script_name) { m_script_name = script_name; }

private:
	std::string m_script_name;
};