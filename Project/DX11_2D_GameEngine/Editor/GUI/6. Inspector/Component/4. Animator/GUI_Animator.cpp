#include "stdafx.h"
#include "GUI_Animator.h"

#include "Helper/IconProvider.h"

GUI_Animator::GUI_Animator(const std::string& animator_gui_name)
	:GUI_Component(animator_gui_name)
{
}

void GUI_Animator::Render()
{
	if (BeginComponent(m_component_gui_name, ComponentType::Animator, IconType::Component_Animator))
	{


		DrawComponentEnd();
	}
}
