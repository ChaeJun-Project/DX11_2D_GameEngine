#pragma once

enum class CBuffer_BindSlot : UINT
{
	WVPMatrix = 0,
	Material = 1,
	Program = 2,
	Light2D = 3,
	SpriteAnimation = 4,
	WidgetWVPMatrix = 5,
};

//======================================
//WVPMatrix[0]
//======================================
struct CBuffer_WVPMatrix
{
	Matrix world;		//월드 행렬
	Matrix view;		//뷰 행렬
	Matrix projection;  //투영 행렬

	Matrix identity;  //항등 행렬
};

extern CBuffer_WVPMatrix g_cbuffer_wvpmatrix;

//======================================
//Material[1]
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
	VEC4_4 = 16,
	VEC4_5 = 17,
	VEC4_6 = 18,
	VEC4_7 = 19,

	MAT_0 = 20,
	MAT_1 = 21,
	MAT_2 = 22,
	MAT_3 = 23,

	TEX_0 = 24,
	TEX_1 = 25,
	TEX_2 = 26,
	TEX_3 = 27,
	TEX_4 = 28,
	TEX_5 = 29,
	TEX_6 = 30,
	TEX_7 = 31,
};

struct CBuffer_Material
{
	int i_array[4];
	float f_array[4];
	Vector2 v2_array[4];
	Vector4 v4_array[8];
	Matrix mx_array[4];
};

//======================================
//Program[2]
//======================================
struct CBuffer_Program
{
	Vector2 client_resolution;
	Vector2 game_resolution;

	Vector2 noise_resolution; //Shader에서 랜덤값으로 사용
	float delta_time;
	float accumulate_time;

	Vector3 view_position; //Main Camera Position
	float padding;
};

extern CBuffer_Program g_cbuffer_program;

//======================================
//Light2D[3]
//======================================
struct Light2D_Info
{
	Vector4 color;

	Light2DType type;
	Vector3 position;

	float range;
	Vector3 padding;
};

struct CBuffer_Light2D
{
	Light2D_Info light2D_array[50];
	UINT light_count;
	Vector3 padding;
};

//======================================
//SpriteAnimation[4]
//======================================
struct SpriteAnimation_Frame
{
	Vector2 left_top;
	Vector2 frame_size;
	float duration;
};

struct CBuffer_SpriteAnimation
{
	SpriteAnimation_Frame sprite_animation_frame;
	Vector3 padding;
};

extern CBuffer_SpriteAnimation g_cbuffer_sprite_animation;