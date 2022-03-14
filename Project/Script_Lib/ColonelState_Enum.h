#pragma once

enum class ColonelState : UINT
{
     Start   = 0,
     Idle    = 1,
     Attack  = 2,
     Guard   = 3,
     Stealth = 4,
     Die     = 5,
};