#pragma once

#include "stdafx.h"

enum CBuffer_BindSlot : UINT
{
	WVPMatrix = 0,
};

struct CBuffer_WVPMatrix
{
   Matrix world;		//���� ���
   Matrix view;			//�� ���
   Matrix projection;   //���� ���
};