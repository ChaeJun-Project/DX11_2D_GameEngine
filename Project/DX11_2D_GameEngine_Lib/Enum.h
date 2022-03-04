#pragma once

//Unsigned typedef
typedef unsigned long ULONG;

//Editor의 상태를 정의(Editor 사용)
enum EditorState : UINT
{
	EditorState_Stop = 0U,			//0000, 편집_재생중지(편집가능) 모드
	EditorState_Play = 1U << 0,		//0001, 편집_재생 모드
	EditorState_Pause = 1U << 1,	//0010, 편집_정지 모드
};

//Log Type(LogManager 사용)
enum class LogType : UINT
{
	Info,     //정보
	Warning,  //경고
	Error,    //에러
};

//Event Type(EventManager 사용)
enum class EventType : UINT
{
	Create_Object = 0,
	Delete_Object = 1,
	Add_Child_Object = 2,
	Detach_Child_Object = 3,
	Scene_Change = 4,
	Update = 5,
};

//Resource Type(IResource 사용)
enum class ResourceType : UINT
{
	None = 0,
	Mesh = 1,
	Material = 2,
	Shader = 3,
	Texture = 4,
	AudioClip = 5,
	Prefab = 6,
	SpriteAnimation = 7,
	TileMap = 8,
};

//Mesh Type(Mesh 사용)
enum class MeshType : UINT
{
	Point = 0,     //점
	Triangle = 1,  //삼각형
	Rectangle = 2, //사각형
	Circle = 3,    //원
	Grid = 4      //그리드
};

//Shader Type(Shader 사용)
enum class ShaderType : UINT
{
	None = 0,
	VS = 1, //VertexShader
	HS = 2, //Hull Shader
	DS = 3, //Domain Shader
	GS = 4, //Geometry Shader
	PS = 5, //Pixel Shader
	CS = 6, //Compute Shader
};

//AudioMode Type(Audio Clip 사용)
enum class AudioModeType : UINT
{
	Audio_2D = 0,
	Audio_3D = 1
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
	No_Test = 3,
	No_Write = 4,
	No_Test_No_Write = 5,
};

//렌더링 시점 Type(Shader 사용)
enum class RenderTimePointType : UINT
{
	None = 0,
	Forward = 1,
	Particle = 2,
	PostEffect = 3,
};

enum PipelineStage : UINT
{
	VS = 1U << 0, //0001, VertexShader
	HS = 1U << 1, //0010, Hull Shader
	DS = 1U << 2, //0100, Domain Shader
	GS = 1U << 3, //1000, Geometry Shader
	PS = 1U << 4, //1 0000, Pixel Shader
	CS = 1U << 5, //11 0000, Compute Shader

	Graphics_ALL = VS | HS | DS | GS | PS,
};

//Component Type(IComponent 사용)
enum class ComponentType : UINT
{
	None = 0,
	Transform = 1,
	Camera = 2,
	SpriteRenderer = 3,
	Animator2D = 4,
	Animator = 5,
	Collider2D = 6,
	Light2D = 7,
	ParticleSystem = 8,
	TileMapRenderer = 9,
	RigidBody2D = 10,
	AudioListener = 11,
	AudioSource = 12,
	END = 13,

	Script = 14,
};

//Projection Type(Camera 사용)
enum class ProjectionType : UINT
{
	Orthographic = 0,//직교투영
	Perspective = 1, //원근투영
};

//GameObject SideType(Transform 사용)
enum class GameObjectSideState : UINT
{
	Right = 0,
	Left = 1,
};

//Light Type(Light2D, Light3D 사용)
enum class LightType : UINT
{
	Directional = 0,
	Point = 1,
	Spot = 2,
};

//Script Param Type(Script을 상속받는 자식 클래스 사용)
enum class ScriptParamType : UINT
{
	Int = 0,
	Float = 1,
	Vector2 = 2,
	Vector3 = 3,
	Vector4 = 4,
	Texture = 5,
	Prefab = 6,
};