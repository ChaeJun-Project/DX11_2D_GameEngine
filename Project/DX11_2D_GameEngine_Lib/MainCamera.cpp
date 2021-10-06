#include "stdafx.h"
#include "MainCamera.h"
#include "IObject.h"

MainCamera::MainCamera()
	:IObject(GameObjectType::MainCamera)
{
	Initialize();
}

MainCamera::~MainCamera()
{
}

void MainCamera::Initialize()
{
	//Transform 컴포넌트 추가
	__super::AddAndCreateComponent<Transform>();

	//Camera 컴포넌트 추가
	__super::AddAndCreateComponent<Camera>();
}

void MainCamera::Update()
{
}

void MainCamera::FinalUpdate()
{
	for (auto& component : m_component_un_map)
		component.second->FinalUpdate();
}

void MainCamera::Render()
{
}
