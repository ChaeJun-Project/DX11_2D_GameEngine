#include "stdafx.h"
#include "GUI_Transform.h"

#include "Helper/IconProvider.h"

#include <DX11_2D_GameEngine_Lib/SceneManager.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Transform.h>

GUI_Transform::GUI_Transform(const std::string& transform_gui_name)
	:GUI_Component(transform_gui_name)
{

}

void GUI_Transform::Render()
{
	auto p_transform = m_p_selected_game_object->GetComponent<Transform>();
	if (p_transform == nullptr)
		return;

	auto is_active = p_transform->GetIsActive();
	if (BeginComponent(m_component_gui_name, ComponentType::Transform, is_active, IconType::Component_Transform))
	{
		auto position = p_transform->GetLocalTranslation();
		auto rotation = p_transform->GetLocalRotation().ToEulerAngle();
		auto scale = p_transform->GetLocalScale();

		ShowFloat3("Position", position, 70.0f, 80.0f);
		ShowFloat3("Rotation", rotation, 70.0f, 80.0f);
		ShowFloat3("Scale", scale, 70.0f, 80.0f);

		if (CAN_EDIT)
		{
			p_transform->SetLocalTranslation(position);
			p_transform->SetLocalRotation(Quaternion::QuaternionFromEulerAngle(rotation));
			p_transform->SetLocalScale(scale);
		}

		DrawComponentEnd();
	}
}
