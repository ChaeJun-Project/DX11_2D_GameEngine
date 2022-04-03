#pragma once

enum class ColonelState : UINT
{
     Start   = 0, //시작
     Idle    = 1, //우선순위 5
     Attack  = 2, //우선순위 2
     Guard   = 3, //우선순위 3
     Stealth = 4, //우선순위 4
     Die     = 5, //우선순위 1
};