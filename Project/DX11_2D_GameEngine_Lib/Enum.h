#pragma once

//Editor의 상태를 정의(Editor 사용)
enum EditorState : UINT
{
	EditorState_Edit = 1U << 0,  //0001 편집 모드
	EditorState_Play = 1U << 1, //0010 재생 모드
	EditorState_Pause = 1U << 2, //0100 정지 모드(재생 모드에서만 유효)
};

//Event Type(EventManager 사용)
enum class EventType : UINT
{
	Create_Object = 0,
	Delete_Object = 1,
};

//Resource Type(IResource 사용)
enum class ResourceType : UINT
{
	NONE = 0,
	Mesh = 1,
	Material = 2,
	Shader = 3,
	Texture = 4,
	Animation = 5,
	Sound = 6,
};

//Mesh Type(Mesh 사용)
enum class MeshType : UINT
{
	Triangle = 0,  //삼각형
	Rectangle = 1, //사각형
	Circle = 2,    //원
};

//Shader Type(Shader 사용)
enum class ShaderType : UINT
{
	NONE = 0,
	VS = 1, //VertexShader
	HS = 2, //Hull Shader
	DS = 3, //Domain Shader
	GS = 4, //Geometry Shader
	PS = 5, //Pixel Shader
};

//Rasterizer State Type(Shader 사용)
enum class RasterizerType : UINT
{
	Cull_Back_Solid = 0, //기본 옵션
	Cull_Front_Solid = 1,
	Cull_None_Solid = 2, //2D 엔진에서 기본 옵션
	Cull_None_WireFrame = 3
};

//Blend State Type(Shader 사용)
enum class BlendType : UINT
{
	Default = 0,
	Alpha_Blend = 1,
	One_One = 2,
};

//Depth Stencil Type(Shader 사용)
enum class DepthStencilType : UINT
{
	Less = 0,
	Less_Equal = 1,
	Grater = 2,
	Grater_Equal = 3,
	No_Test = 4,
	No_Wire = 5,
	No_Test_No_Wire = 6,
};

//Shader Resource Type(ShaderManager 사용)
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

//Component Type(IComponent 사용)
enum class ComponentType : UINT
{
	NONE = 0,
	Transform = 1,
	Camera = 2,
	Renderer = 3,
	Animator = 4,
	Script = 5,
	RigidBody2D = 6,
	BoxCollider2D = 7,
};

//Projection Type(Camera 사용)
enum class ProjectionType : UINT
{
	Orthographic = 0,//직교투영
	Perspective = 1, //원근투영
};

//GameObject SideType(IObject 사용)
enum class GameObjectSideState : UINT
{
	Right = 0,
	Left = 1,
};
