#ifndef ParticleStruct
#define ParticleStruct

struct ParticleInfo
{
    float3 world_position; //���� ��ǥ�� ���� ��ġ��
    float3 view_rotation;  //�� ��ǥ�� ���� ȸ����
    float3 view_scale;     //�� ��ǥ�� ���� �����ϰ�
    
    float4 color;          //��ƼŬ ����
    
    float speed;           //��ƼŬ �ӵ�
    float3 direction;      //��ƼŬ �̵� ����
    
    float current_time;    //��ƼŬ�� �����ǰ� ��� �ð�
    float life_time;       //��ƼŬ ���� �ð�
    int is_active;         //��ƼŬ Ȱ��ȭ ����
};

struct ParticleShared
{
    int activable_count;      //Ȱ��ȭ �� �� �ִ� ��ƼŬ ����
    int current_active_count; //���� Ȱ��ȭ �Ǿ��ִ� ��ƼŬ ����
    float3 create_range;     //��ƼŬ ���� ����
};

#endif