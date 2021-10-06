#pragma once

//Editor�� ���¸� ����(Editor ���)
enum EditorState : UINT
{
	EditorState_Edit = 0U,      //0000 ���� ���
	EditorState_Play = 1U << 0, //0001 ��� ���
	EditorState_Pause = 1U << 1, //0010 ���� ���(��� ��忡���� ��ȿ)
};

//Resource Type(IResource ���)
enum class ResourceType : UINT
{
	Mesh = 0,
	Material = 1,
	Shader = 2,
	Texture = 3,
	Sound = 4,
	Animation = 5,
};

//Mesh Type(Mesh ���)
enum class MeshType : UINT
{
	Triangle = 0,  //�ﰢ��
	Rectangle = 1, //�簢��
	Circle = 2,    //��
};

//Shader Type(Shader ���)
enum class ShaderType : UINT
{
	NONE = 0,
	VS = 1, //VertexShader
	HS = 2, //Hull Shader
	DS = 3, //Domain Shader
	GS = 4, //Geometry Shader
	PS = 5, //Pixel Shader
};

//Shader Resource Type(ShaderManager ���)
enum class ShaderResourceType : UINT
{
	//TODO
};

enum TexturePipelineStage : UINT
{
	VS = 0U,      //0000, VertexShader
	HS = 1U << 0, //0001, Hull Shader
	DS = 1U << 1, //0010, Domain Shader
	GS = 1U << 2, //0100, Geometry Shader
	PS = 1U << 3  //1000, Pixel Shader
};

//Component Type(IComponent ���)
enum class ComponentType : UINT
{
	NONE = 0,
	Transform = 1,
	Camera = 2,
	Image = 3,
	Animator = 4,
	RigidBody2D = 5,
	BoxCollider2D = 6,
};

//Projection Type(Camera ���)
enum class ProjectionType : UINT
{
	Orthographic = 0,//��������
	Perspective = 1, //��������
};

//GameObject Type(IObject ���)
enum class GameObjectType : UINT
{
    MainCamera = 0, //�ӽ�
	Background = 1,
	RockManX = 2,
	RockManZ = 3,
};

//GameObject SideType(IObject ���)
enum class GameObjectSideState : UINT
{
	Right = 0,
	Left = 1,
};
