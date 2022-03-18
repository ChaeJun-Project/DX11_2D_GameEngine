#include "stdafx.h"
#include "GUI_Canvas.h"

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Canvas.h>

GUI_Canvas::GUI_Canvas(const std::string& canvas_gui_name)
	:GUI_Component(canvas_gui_name)
{
}

void GUI_Canvas::Render()
{
	//Canvas
	auto p_canvas = m_p_selected_game_object->GetComponent<Canvas>();
	if (p_canvas == nullptr)
		return;

	auto is_active = p_canvas->GetIsActive();
	if (BeginComponent(m_component_gui_name, ComponentType::Canvas, is_active, IconType::Component_Canvas))
	{
		auto p_canvas = m_p_selected_game_object->GetComponent<Canvas>();
		if (p_canvas == nullptr)
			return;

		p_canvas->SetIsActive(is_active);

		//Property
		auto cavnas_resolution = p_canvas->GetCanvasResolution();

		//Canvas Resolution
		ShowFloat2("Canvas Resolution", cavnas_resolution, 60.0f, 100.0f);

		if (CAN_EDIT)
		{
			p_canvas->SetCanvasResolution(cavnas_resolution);
		}

		DrawComponentEnd();
	}
}
