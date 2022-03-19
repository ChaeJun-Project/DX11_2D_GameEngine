#pragma once

#include "../GUI_Component.h"

class IComponent;

class GUI_Transform final : public GUI_Component
{
public:
	GUI_Transform(const std::string& transform_gui_name);
	~GUI_Transform() = default;

	void Render() override;

private:
    void RenderTransform(IComponent* p_component);
	void RenderRectTransform(IComponent* p_component);
};