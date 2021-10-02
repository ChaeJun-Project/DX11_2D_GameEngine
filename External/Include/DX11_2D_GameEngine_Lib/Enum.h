#pragma once

//Editor의 상태를 정의(Editor 사용)
enum EditorState : UINT
{
	EditorState_Edit = 0U,      //0000 편집 모드
	EditorState_Play = 1U << 0, //0001 재생 모드
	EditorState_Pause = 1U << 1, //0010 정지 모드(재생 모드에서만 유효)
};

//Resource Type(IResource 사용)
enum class ResourceType : UINT
{
	Mesh = 0,
	Material = 1,
	Shader = 2,
	Texture = 3,
	Sound = 4,
	Animation = 5,
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

//Shader Resource Type(ShaderManager 사용)
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

//Component Type(IComponent 사용)
enum class ComponentType : UINT
{
	Transform = 0,
	Camera = 1,
	Image = 2,
	Sprite = 3,
	Animator = 4,
	RigidBody2D = 5,
	BoxCollider2D = 6,
};

//Projection Type(Camera 사용)
enum class ProjectionType : UINT
{
	Orthographic = 0,//직교투영
	Perspective = 1, //원근투영
};

//Object Type(IObject 사용)
enum class ObjectType : UINT
{
	Background = 0,
	RockManX = 1,
	RockManZ = 2,
};
