#pragma once

//Editor�� ���¸� ����(Editor ���)
enum EditorState : UINT
{
	EditorState_Edit = 1U << 0,  //0001 ���� ���
	EditorState_Play = 1U << 1, //0010 ��� ���
	EditorState_Pause = 1U << 2, //0100 ���� ���(��� ��忡���� ��ȿ)
};

//Event Type(EventManager ���)
enum class EventType : UINT
{
	Create_Object = 0,
	Delete_Object = 1,
};

//Resource Type(IResource ���)
enum class ResourceType : UINT
{
    NONE		= 0,
	Mesh		= 1,
	Material	= 2,
	Shader		= 3,
	Texture		= 4,
	Animation	= 5,
	Sound		= 6,
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

//Rasterizer State Type(Shader ���)
enum class RasterizerType : UINT
{

};

//Blend State Type(Shader ���)
enum class BlendType : UINT
{
	Default = 0,
};

//Depth Stencil Type(Shader ���)
enum class DepthStencilType : UINT
{
   Less = 0,
   Less_Equal = 1,
};

//Shader Resource Type(ShaderManager ���)
enum class ShaderResourceType : UINT
{
	Standard = 0,
};

enum PipelineStage : UINT
{
	VS = 1U << 0, //0001, VertexShader
	HS = 1U << 1, //0010, Hull Shader
	DS = 1U << 2, //0100, Domain Shader
	GS = 1U << 3, //1000, Geometry Shader
	PS = 1U << 4, //1 0000, Pixel Shader

	ALL = VS | HS | DS | GS | PS, 
};

//Component Type(IComponent ���)
enum class ComponentType : UINT
{
	NONE		  = 0,
	Transform	  = 1,
	Camera		  = 2,
	Renderer	  = 3,
	Animator	  = 4,
	Script		  = 5,
	RigidBody2D	  = 6,
	BoxCollider2D = 7,
};

//Projection Type(Camera ���)
enum class ProjectionType : UINT
{
	Orthographic = 0,//��������
	Perspective = 1, //��������
};

//GameObject SideType(IObject ���)
enum class GameObjectSideState : UINT
{
	Right = 0,
	Left = 1,
};
