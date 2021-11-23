#pragma once

class GUI_Component
{
public:
	GUI_Component(const std::string& component_gui_name);
	virtual ~GUI_Component() = default;

	virtual void Render() = 0;

protected:
	const bool BeginComponent(const std::string& component_name, const ComponentType& component_type, const IconType& icon_type);
	void ShowComponentSettingPopup(const std::string& component_popup_id, const ComponentType& component_type);
	void DrawComponentEnd();

public:
	void SetGameObject(GameObject* select_game_object)
	{
		m_select_game_object = select_game_object;
	}

protected:
    std::string m_component_gui_name;
 
	GameObject* m_select_game_object = nullptr;
};