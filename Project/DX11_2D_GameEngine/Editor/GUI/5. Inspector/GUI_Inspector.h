#pragma once
#include "GUI/IGUI.h"

class GameObject;
class IResource;
class GUI_ItemList;
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
	void ShowComboTags();
	void ShowComboLayers();
	void ShowTagAndLayerList();

	void ShowResourceInfo();

	void ShowAddComponent();
	void ShowAddComponentPopup();

private:
	void InitializeTag();
	void SaveTag();
	void LoadTag();

	void InitializeLayer();
	void SaveLayer();
	void LoadLayer();

private:
	std::shared_ptr<GUI_Component> GetComponentGUI(const ComponentType& component_type) const;

private:
    GameObject* m_p_selected_game_object = nullptr;
	IResource* m_p_select_resource = nullptr;

	//Tag&Layer
	bool m_is_show_tag_and_layer_list = false;

	//Tag
	std::unique_ptr<GUI_ItemList> m_p_tag_list = nullptr;
	std::string m_folder_path = "Engine/";
	std::string m_tag_file_path = "Tag.txt";
	std::deque<std::string> m_tag_deque;

	//Layer
	std::unique_ptr<GUI_ItemList> m_p_layer_list = nullptr;
	std::string m_tag_layer_path = "Layer.txt";
	std::map<UINT, std::string> m_layer_map;

	//Component GUI list
	std::list<std::pair<ComponentType, std::shared_ptr<GUI_Component>>> m_component_gui_list;
};

