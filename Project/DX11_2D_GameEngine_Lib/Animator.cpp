#include "stdafx.h"
#include "Animator.h"

Animator::Animator()
	:IComponent(ComponentType::Animator)
{

}

Animator::Animator(const Animator& origin)
	: IComponent(origin.m_component_type)
{
	m_is_active = origin.m_is_active;
}

Animator::~Animator()
{

}

void Animator::Update()
{
	
}

void Animator::FinalUpdate()
{

}

void Animator::SaveToScene(FILE* p_file)
{
}

void Animator::LoadFromScene(FILE* p_file)
{
}
