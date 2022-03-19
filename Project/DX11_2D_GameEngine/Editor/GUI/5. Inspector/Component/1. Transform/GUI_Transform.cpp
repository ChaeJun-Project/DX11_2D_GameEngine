#include "stdafx.h"
#include "GUI_Transform.h"

#include "Helper/IconProvider.h"

#include <DX11_2D_GameEngine_Lib/SceneManager.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Transform.h>
#include <DX11_2D_GameEngine_Lib/RectTransform.h>

GUI_Transform::GUI_Transform(const std::string& transform_gui_name)
	:GUI_Component(transform_gui_name)
{

}

void GUI_Transform::Render()
{
	auto p_component = m_p_selected_game_object->GetComponent(ComponentType::Transform);

	switch (p_component->GetComponentType())
	{
	case ComponentType::Transform:
		RenderTransform(p_component);
		break;
	case ComponentType::RectTransform:
		RenderRectTransform(p_component);
		break;
	}
}

void GUI_Transform::RenderTransform(IComponent* p_component)
{
	auto p_transform = dynamic_cast<Transform*>(p_component); //다운 캐스팅
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

void GUI_Transform::RenderRectTransform(IComponent* p_component)
{
	auto p_rect_transform = dynamic_cast<RectTransform*>(p_component); //다운 캐스팅
	if (p_rect_transform == nullptr)
		return;

	auto is_active = p_rect_transform->GetIsActive();
	if (BeginComponent("Rect Transform", ComponentType::RectTransform, is_active, IconType::Component_RectTransform))
	{
		auto position = p_rect_transform->GetLocalTranslation();
		auto widget_size = p_rect_transform->GetWidgetSize();
		auto rotation = p_rect_transform->GetLocalRotation().ToEulerAngle();
		auto scale = p_rect_transform->GetLocalScale();

		ShowFloat3("Position", position, 70.0f, 80.0f);
		ShowFloat2("Size", widget_size, 70.0f, 80.0f);
		ShowFloat3("Rotation", rotation, 70.0f, 80.0f);
		ShowFloat3("Scale", scale, 70.0f, 80.0f);

		if (CAN_EDIT)
		{
			p_rect_transform->SetLocalTranslation(position);
			p_rect_transform->SetWidgetSize(widget_size);
			p_rect_transform->SetLocalRotation(Quaternion::QuaternionFromEulerAngle(rotation));
			p_rect_transform->SetLocalScale(scale);
		}

		DrawComponentEnd();
	}
}
