#pragma once

enum class SBufferType : UINT
{
	Read_Only = 0,
	Read_Write = 1,
};

//======================================
//Particle
//======================================
struct ParticleInfo
{
	Vector3 world_position; //월드 좌표계 기준 위치값
	Vector3 view_rotation;	//뷰 좌표계 기준 회전값
	Vector3 view_scale;		//뷰 좌표계 기준 스케일값

	Vector4 color;           //파티클 색상

	float speed;           //파티클 속도
	Vector3 direction;		//파티클 이동 방향

	float current_time;     //파티클이 생성되고 경과 시간
	float life_time;		//파티클 생존 시간
	int is_active;			//파티클 활성화 여부
};

struct ParticleShared
{
	int activable_count;      //활성화 할 수 있는 파티클 개수
	int current_active_count; //현재 활성화 되어있는 파티클 개수
	Vector3 create_range;      //파티클 생성 범위
};