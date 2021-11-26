#pragma once

#include "../GUI_Component.h"

class IResource;

class SpriteRenderer;

class GUI_ItemList;

class GUI_SpriteRenderer final : public GUI_Component
{
public:
	GUI_SpriteRenderer(const std::string& sprite_renderer_gui_name);
	~GUI_SpriteRenderer();

	void Render() override;

private:
    void SelectResource(SpriteRenderer* p_sprite_renderer, const std::shared_ptr<IResource>& p_resource, const std::string& selected_item);

private:
	GUI_ItemList* m_p_item_list = nullptr;
};