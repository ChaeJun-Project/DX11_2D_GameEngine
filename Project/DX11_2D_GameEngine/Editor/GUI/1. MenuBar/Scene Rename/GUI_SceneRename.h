#pragma once

#include "GUI/IGUI_Popup.h"

class GUI_SceneRename : public IGUI_Popup
{
public:
	GUI_SceneRename() = default;
	virtual ~GUI_SceneRename() = default;

public:
	virtual void Render() override;

	friend class GUI_MenuBar;
};

