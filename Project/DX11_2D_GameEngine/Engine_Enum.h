#pragma once

enum class ClientSceneState : UINT
{
    Game = 0,
    Editor = 1,
};

extern ClientSceneState client_scene_state;