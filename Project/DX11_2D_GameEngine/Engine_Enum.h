#pragma once

enum class ClientState : UINT
{
    Title = 0,
    Game = 1,
    Editor = 2,
};

extern ClientState client_state;