#include "stdafx.h"
#include "GUI_Light2D.h"

#include "Helper/IconProvider.h"

GUI_Light2D::GUI_Light2D(const std::string& light2D_gui_name)
	:GUI_Component(light2D_gui_name)
{
}

void GUI_Light2D::Render()
{
	if (BeginComponent(m_component_gui_name, ComponentType::Light2D, IconType::Component_Light2D))
	{

		DrawComponentEnd();
	}
}
