#pragma once

#include "../GUI_Component.h"

class GUI_ItemList;

class IResource;

class ImageRenderer;

class GUI_ImageRenderer final : public GUI_Component
{
public:
	GUI_ImageRenderer(const std::string& image_renderer_gui_name);
	~GUI_ImageRenderer();

	void Render() override;

private:
    void ShowTexture(const std::string& label_name, ImageRenderer* p_image_renderer);
	void ShowMaterialCombo(const std::string& label_name, ImageRenderer* p_image_renderer);
	void ShowMeshCombo(const std::string& label_name, ImageRenderer* p_image_renderer);

    void ShowImagePropertyCombo(ImageRenderer* p_image_renderer);
	void ShowFillMethod(ImageRenderer* p_image_renderer);
	void ShowDirection(ImageRenderer* p_image_renderer);

private:
	GUI_ItemList* m_p_item_list = nullptr;

	GUI_ItemList* m_p_image_type_list = nullptr;
	GUI_ItemList* m_p_fill_method_type_list = nullptr;
	GUI_ItemList* m_p_direction_type_list = nullptr;
};
