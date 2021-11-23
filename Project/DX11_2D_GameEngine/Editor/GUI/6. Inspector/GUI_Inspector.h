#pragma once
#include "GUI/IGUI.h"

class GameObject;
class IResource;
class GUI_Component;

class GUI_Inspector : public IGUI
{
public:
	GUI_Inspector(const std::string& inspector_title);
	~GUI_Inspector();

public:
	void Update() override;
	void Render() override;

private:
    void ShowGameObjectInfo();
	void ShowResourceInfo();

	void ShowAddComponent();
	void ShowAddComponentPopup();

private:
	std::shared_ptr<GUI_Component> GetComponentGUI(const ComponentType& component_type) const;

private:
	GameObject* m_select_game_object = nullptr;

	IResource* m_select_resource = nullptr;

	//Component GUI list
	std::list<std::pair<ComponentType, std::shared_ptr<GUI_Component>>> m_component_gui_list;
};

