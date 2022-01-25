#include "stdafx.h"
#include "GUI_Collider2D.h"

#include "Helper/IconProvider.h"

#include <DX11_2D_GameEngine_Lib/FileManager.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Collider2D.h>

GUI_Collider2D::GUI_Collider2D(const std::string& collider2D_name)
	:GUI_Component(collider2D_name)
{
}

void GUI_Collider2D::Render()
{
	if (BeginComponent(m_component_gui_name, ComponentType::Collider2D, IconType::Component_Collider2D))
	{
		auto collider2D = m_select_game_object->GetComponent<Collider2D>();
		if (collider2D == nullptr)
			return;

		auto offest_position = collider2D->GetOffsetPosition();
		auto offest_scale = collider2D->GetOffsetScale();

		ShowFloat2("Offset", offest_position, 70.0f, 80.0f);
		ShowFloat2("Size", offest_scale, 70.0f, 80.0f);

		if (CAN_EDIT)
		{
			collider2D->SetOffsetPosition(offest_position);
			collider2D->SetOffsetScale(offest_scale);
		}

		DrawComponentEnd();
	}
}
