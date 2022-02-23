#pragma once

#include "Helper/IconProvider.h"

//<summary>
//모든 Component GUI는 GUI_Component를 상속
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
		const std::string& script_name = std::string() //자식 클래스가 GUI_Script인 경우에만 매개변수 사용
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