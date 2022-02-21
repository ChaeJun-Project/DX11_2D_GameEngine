#include "stdafx.h"
#include "RigidBody2D.h"

RigidBody2D::RigidBody2D()
	:IComponent(ComponentType::RigidBody2D)
{
}

RigidBody2D::RigidBody2D(const RigidBody2D& origin)
	: IComponent(origin.m_component_type)
{
}

RigidBody2D::~RigidBody2D()
{
}

void RigidBody2D::Initialize()
{
}

void RigidBody2D::Start()
{
}

void RigidBody2D::FinalUpdate()
{
}

void RigidBody2D::SaveToScene(FILE* p_file)
{
}

void RigidBody2D::LoadFromScene(FILE* p_file)
{
}
