#include "stdafx.h"
#include "GUI_RectTransform.h"

#include "Helper/IconProvider.h"

#include <DX11_2D_GameEngine_Lib/SceneManager.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/RectTransform.h>

GUI_RectTransform::GUI_RectTransform(const std::string& rect_transform_gui_name)
	:GUI_Component(rect_transform_gui_name)
{
}

void GUI_RectTransform::Render()
{
	auto p_rect_transform = m_p_selected_game_object->GetComponent<RectTransform>();
	if (p_rect_transform == nullptr)
		return;

	auto is_active = p_rect_transform->GetIsActive();
	if (BeginComponent(m_component_gui_name, ComponentType::RectTransform, is_active, IconType::Component_Rect_Transform))
	{
		//Property
		auto position = p_rect_transform->GetLocalTranslation();
		auto size = p_rect_transform->GetWidgetSize();
		auto rotation = p_rect_transform->GetLocalRotation().ToEulerAngle();
		auto scale = p_rect_transform->GetLocalScale();

		ShowFloat3("Position", position, 70.0f, 80.0f);
		ShowFloat2("Size", size, 70.0f, 80.0f);
		ShowFloat3("Rotation", rotation, 70.0f, 80.0f);
		ShowFloat3("Scale", scale, 70.0f, 80.0f);

		if (CAN_EDIT)
		{
			p_rect_transform->SetLocalTranslation(position);
			p_rect_transform->SetWidgetSize(size);
			p_rect_transform->SetLocalRotation(Quaternion::QuaternionFromEulerAngle(rotation));
			p_rect_transform->SetLocalScale(scale);
		}

		DrawComponentEnd();
	}
}
