#include "stdafx.h"
#include "GUI_Camera.h"

#include "Helper/IconProvider.h"

GUI_Camera::GUI_Camera(const std::string& camera_gui_name)
	:GUI_Component(camera_gui_name)
{
}

void GUI_Camera::Render()
{
	if (BeginComponent(m_component_gui_name, ComponentType::Camera, IconType::Component_Camera))
	{

		DrawComponentEnd();
	}
}
