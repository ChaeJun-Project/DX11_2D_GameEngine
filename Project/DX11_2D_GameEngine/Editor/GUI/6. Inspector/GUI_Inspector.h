#pragma once
#include "GUI/IGUI.h"

class GameObject;
class IResource;
class GUI_Component;

class GUI_Inspector final : public IGUI
{
public:
	GUI_Inspector(const std::string& inspector_title);
	~GUI_Inspector();

public:
	void Initialize() override;
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
	IResource* m_select_resource = nullptr;

	//Component GUI list
	std::list<std::pair<ComponentType, std::shared_ptr<GUI_Component>>> m_component_gui_list;
};

