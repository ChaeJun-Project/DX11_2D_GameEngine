#pragma once

//Editor�� ���¸� ����(Editor ���)
enum EditorState : UINT
{
	EditorState_Stop = 0U,			//0000, ����_�������(��������) ���
	EditorState_Play = 1U << 0,		//0001, ����_��� ���
	EditorState_Pause = 1U << 1,	//0010, ����_���� ���
};

//Event Type(EventManager ���)
enum class EventType : UINT
{
	Create_Object = 0,
	Delete_Object = 1,
	Add_Child_Object = 2,
};

//RenderTexture Type
enum class RenderTextureType : UINT
{
	GameScene = 0,
	EditorScene = 1
};

//Resource Type(IResource ���)
enum class ResourceType : UINT
{
	NONE = 0,
	Mesh = 1,
	Material = 2,
	Shader = 3,
	Texture = 4,
	Animation = 5,
	Prefab = 6,
	Sound = 7,
};

//Mesh Type(Mesh ���)
enum class MeshType : UINT
{
	Point = 0,     //��
	Triangle = 1,  //�ﰢ��
	Rectangle = 2, //�簢��
	Circle = 3,    //��
	Grid = 4      //�׸���
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
	CS = 6, //Compute Shader
};

//Texture Type(Resource Manager ���)
enum class TextureType : UINT
{
	Standard = 0,
	Atlas = 1,
	Tile_Atlas = 2,
};

//Rasterizer State Type(Shader ���)
enum class RasterizerType : UINT
{
	Cull_Back_Solid = 0, //�⺻ �ɼ�
	Cull_Front_Solid = 1,
	Cull_None_Solid = 2, //2D �������� �⺻ �ɼ�
	Cull_None_WireFrame = 3
};

//Blend State Type(Shader ���)
enum class BlendType : UINT
{
	Default = 0,
	Alpha_Blend = 1,
	One_One = 2,
};

//Depth Stencil Type(Shader ���)
enum class DepthStencilType : UINT
{
	Less = 0,
	Less_Equal = 1,
	Grater = 2,
	No_Test = 3,
	No_Write = 4,
	No_Test_No_Write = 5,
};

//������ ���� Type(Shader ���)
enum class RenderTimePointType : UINT
{
	NONE = 0,
	Forward = 1,
	Particle = 2,
	PostEffect = 3,
};

//Shader Resource Type(ShaderManager ���)
enum class ShaderResourceType : UINT
{
	Standard = 0,		  //Draw Texture
	Line = 1,			  //Draw Line
	Grid = 2,             //Draw Grid
	Light2D = 3,		  //Draw Texture & Light2D
	Particle = 4,		  //Draw Particle
	PostEffect = 5,	      //Draw Post Effect
	TileMap = 6           //Draw Tile Map
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

//Component Type(IComponent ���)
enum class ComponentType : UINT
{
	NONE = 0,
	Transform = 1,
	Camera = 2,
	SpriteRenderer = 3,
	Animator2D = 4,
	Animator = 5,
	Collider2D = 6,
	Light2D = 7,
	ParticleSystem = 8,
	TileMap = 9,
	RigidBody2D = 10,
	END = 11,

	Script = 12,
};

//Projection Type(Camera ���)
enum class ProjectionType : UINT
{
	Orthographic = 0,//��������
	Perspective = 1, //��������
};

//GameObject SideType(Transform ���)
enum class GameObjectSideState : UINT
{
	Right = 0,
	Left = 1,
};

//Light Type(Light2D, Light3D ���)
enum class LightType : UINT
{
	Directional = 0,
	Point = 1,
	Spot = 2,
};