#pragma once

#include "stdafx.h"

enum class CBuffer_BindSlot : UINT
{
	WVPMatrix = 0,
	Material = 1,
};

//======================================
//WVPMatrix
//======================================
struct CBuffer_WVPMatrix
{
   Matrix world;		//월드 행렬
   Matrix view;			//뷰 행렬
   Matrix projection;   //투영 행렬
};

extern CBuffer_WVPMatrix g_cbuffer_wvpmatrix;

//======================================
//Material
//======================================
enum class Material_Parameter : UINT
{
	INT_0 = 0,
	INT_1 = 1,
	INT_2 = 2,
	INT_3 = 3,

	FLOAT_0 = 4,
	FLOAT_1 = 5,
	FLOAT_2 = 6,
	FLOAT_3 = 7,

	VEC2_0 = 8,
	VEC2_1 = 9,
	VEC2_2 = 10,
	VEC2_3 = 11,

	VEC4_0 = 12,
	VEC4_1 = 13,
	VEC4_2 = 14,
	VEC4_3 = 15,

	MAT_0 = 16,
	MAT_1 = 17,
	MAT_2 = 18,
	MAT_3 = 19,

	TEX_0 = 20,
	TEX_1 = 21,
	TEX_2 = 22,
	TEX_3 = 23,
};

struct CBuffer_Material
{
    int i_array[4];
	float f_array[4];
	Vector2 v2_array[4];
	Vector4 v4_array[4];
	Matrix mx_array[4];
};

