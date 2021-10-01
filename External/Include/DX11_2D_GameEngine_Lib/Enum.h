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

//Component Type(IComponent ���)
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
