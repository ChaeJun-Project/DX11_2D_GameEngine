#include "stdafx.h"
#include "EditorHelper.h"

#include <DX11_2D_GameEngine_Lib/SceneManager.h>
#include <DX11_2D_GameEngine_Lib./Scene.h>


EditorHelper::~EditorHelper()
{
	m_selected_game_object = nullptr;
}