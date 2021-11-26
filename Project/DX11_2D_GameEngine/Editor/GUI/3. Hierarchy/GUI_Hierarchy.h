#pragma once
#include "GUI/IGUI.h"

class Scene;
class GameObject;

class GUI_Hierarchy final : public IGUI
{
public:
	GUI_Hierarchy(const std::string& hierarchy_title);
	~GUI_Hierarchy();

public:
	void Update() override;
	void Render() override;

private:
	void ShowHierarchy();
	void AddGameObject(GameObject* game_object);

private:
    void ClickedGameObject();
	void ClickedCheck();
	void ShowMenuPopup();
	void SelectedGameObject(GameObject* game_object);

private:
	std::shared_ptr<Scene> m_p_current_scene = nullptr;

	GameObject* m_hovered_game_object = nullptr;
	GameObject* m_clicked_game_object = nullptr;
	GameObject* m_empty_game_object = nullptr;
};