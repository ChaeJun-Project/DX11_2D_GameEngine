#include "stdafx.h"
#include "RockManZ.h"
#include "IObject.h"

RockManZ::RockManZ()
	:IObject(GameObjectType::RockManZ)
{
	Initialize();
}

RockManZ::~RockManZ()
{
}

void RockManZ::Initialize()
{
	//Transform 컴포넌트 추가
	__super::AddAndCreateComponent<Transform>();

	//Animator 컴포넌트 추가
	__super::AddAndCreateComponent<Animator>();
	auto animator = __super::GetComponent<Animator>();
	auto animation_idle = std::make_shared<Animation>("RockManZ_Idle");
	auto result = animation_idle->LoadFromFile("Texture/RockManZ/Animation/Z03_Idle/");
	assert(result);
	if (result)
	{
		animation_idle->SetAnimationTime(3.0f);
		animation_idle->SetIsLoop(true);
		animator->AddAnimation(animation_idle->GetResourceName(), animation_idle);
	}

	animator->SetCurrentAnimation("RockManZ_Idle");
}

void RockManZ::Update()
{
}

void RockManZ::FinalUpdate()
{
	for(auto& component : m_component_un_map)
		component.second->FinalUpdate();
}

void RockManZ::Render()
{
	__super::GetComponent<Animator>()->Render();
}
