#pragma once

enum class ColonelState : UINT
{
     Start   = 0, //����
     Idle    = 1, //�켱���� 5
     Attack  = 2, //�켱���� 2
     Guard   = 3, //�켱���� 3
     Stealth = 4, //�켱���� 4
     Die     = 5, //�켱���� 1
};