#include "stdafx.h"
#include "GUI_Transform.h"

#include "Helper/IconProvider.h"

#include <DX11_2D_GameEngine_Lib/FileManager.h>

#include <DX11_2D_GameEngine_Lib/SceneManager.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Transform.h>

GUI_Transform::GUI_Transform(const std::string& transform_gui_name)
	:GUI_Component(transform_gui_name)
{

}

void GUI_Transform::Render()
{
	if (BeginComponent(m_component_gui_name, ComponentType::Transform, IconType::Component_Transform))
	{
		auto transform = m_select_game_object->GetComponent<Transform>();

		auto position = transform->GetLocalTranslation();
		auto rotation = transform->GetLocalRotation().ToEulerAngle();
		auto scale = transform->GetLocalScale();

		ShowFloat3("Position", position, 70.0f, 80.0f);
		ShowFloat3("Rotation", rotation, 70.0f, 80.0f);
		ShowFloat3("Scale", scale, 70.0f, 80.0f);

		//Set Data
		if (SceneManager::GetInstance()->GetEditorState() == EditorState::EditorState_Stop)
		{
			transform->SetLocalTranslation(position);
			transform->SetLocalRotation(Quaternion::QuaternionFromEulerAngle(rotation));
			transform->SetLocalScale(scale);
		}

		DrawComponentEnd();
	}
}
