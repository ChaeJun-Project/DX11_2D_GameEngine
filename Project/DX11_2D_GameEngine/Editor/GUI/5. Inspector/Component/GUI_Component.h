#pragma once

#include "Helper/IconProvider.h"

//<summary>
//��� Component GUI�� GUI_Component�� ���
//</summary>
class GUI_Component
{
public:
	GUI_Component(const std::string& component_gui_name);
	virtual ~GUI_Component() = default;

	virtual void Render() = 0;

protected:
	const bool BeginComponent
	(
		const std::string& component_name,
		const ComponentType& component_type,
		bool& is_active,
		const IconType& icon_type,
		const std::string& script_name = std::string() //�ڽ� Ŭ������ GUI_Script�� ��쿡�� �Ű����� ���
	);
	void ShowComponentSettingPopup(const std::string& component_popup_id, const ComponentType& component_type);
	void ShowScriptSettingPopup(const std::string& script_name);
	void DrawComponentEnd();

public:
	void SetGameObject(GameObject* select_game_object)
	{
		m_p_selected_game_object = select_game_object;
	}

protected:
	std::string m_component_gui_name;

	GameObject* m_p_selected_game_object = nullptr;
};