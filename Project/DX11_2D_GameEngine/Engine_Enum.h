#pragma once

enum class ClientState : UINT
{
    Title = 0,
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