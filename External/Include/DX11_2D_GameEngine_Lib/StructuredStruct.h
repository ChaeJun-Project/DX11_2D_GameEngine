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
	Vector3 world_position; //���� ��ǥ�� ���� ��ġ��
	Vector3 view_rotation;	//�� ��ǥ�� ���� ȸ����
	Vector3 view_scale;		//�� ��ǥ�� ���� �����ϰ�

	Vector4 color;           //��ƼŬ ����

	float speed;           //��ƼŬ �ӵ�
	Vector3 direction;		//��ƼŬ �̵� ����

	float current_time;     //��ƼŬ�� �����ǰ� ��� �ð�
	float life_time;		//��ƼŬ ���� �ð�
	int is_active;			//��ƼŬ Ȱ��ȭ ����
};

struct ParticleShared
{
	int activable_count;      //Ȱ��ȭ �� �� �ִ� ��ƼŬ ����
	int current_active_count; //���� Ȱ��ȭ �Ǿ��ִ� ��ƼŬ ����
	Vector3 create_range;      //��ƼŬ ���� ����
};