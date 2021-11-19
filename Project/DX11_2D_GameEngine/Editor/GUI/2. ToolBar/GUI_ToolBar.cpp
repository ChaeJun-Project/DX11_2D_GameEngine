#include "stdafx.h"
#include "GUI_ToolBar.h"

GUI_ToolBar::GUI_ToolBar(const std::string& toolbar_title)
:IGUI(toolbar_title)
{
}

void GUI_ToolBar::Update()
{
	if (KEY_PRESS(KeyCode::KEY_CONTROL) && KEY_DOWN(KeyCode::KEY_T))
	{
		m_is_active = !m_is_active;
	}
}

void GUI_ToolBar::Begin()
{
}

void GUI_ToolBar::Render()
{
}
