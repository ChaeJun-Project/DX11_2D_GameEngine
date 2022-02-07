#include "stdafx.h"
#include "Animator.h"

Animator::Animator()
	:IComponent(ComponentType::Animator)
{

}

Animator::Animator(const Animator& origin)
	: IComponent(origin.GetComponentType())
{
	
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