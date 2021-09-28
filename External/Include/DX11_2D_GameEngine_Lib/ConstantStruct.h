#pragma once

#include "stdafx.h"

enum CBuffer_BindSlot : UINT
{
	WVPMatrix = 0,
};

struct CBuffer_WVPMatrix
{
   Matrix world;		//월드 행렬
   Matrix view;			//뷰 행렬
   Matrix projection;   //투영 행렬
};