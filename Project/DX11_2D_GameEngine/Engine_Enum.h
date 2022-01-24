#pragma once

enum class ClientState : UINT
{
    Game = 1,
    Editor = 2,
};

extern ClientState client_state;

enum class FileType : UINT
{
    Scene = 0,
    Tile = 1,
    Animation = 2,
};