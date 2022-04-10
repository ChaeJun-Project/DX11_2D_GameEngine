#include "stdafx.h"
#include "Loading_Script.h"

#include <DX11_2D_GameEngine_Lib/Scene.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>

Loading_Script::Loading_Script()
	:Script("Loading_Script")
{

}

Loading_Script::Loading_Script(const Loading_Script& origin)
	: Script(origin.m_script_name)
{
	m_is_active = origin.m_is_active;
}

#include <thread>
void Loading_Script::OnEnable()
{
	std::thread loading_thread(&Loading_Script::LoadScene, this);

	//로딩 스레드가 종료될 때까지 기다리지 않음(메인 스레드는 계속 작동)
	loading_thread.detach();
}

void Loading_Script::LoadScene()
{
	std::cout<<"[Game Stage]"<< std::endl;
	std::cout<<"Scene Load Thread Start"<<std::endl;

	auto next_scene = SCENE_MANAGER->LoadScene((FILE_MANAGER->GetAbsoluteContentPath() + "Asset/Scene/Game Stage.scene"));

	//Change Scene
	EventStruct event_struct;
	ZeroMemory(&event_struct, sizeof(EventStruct));

	event_struct.event_type = EventType::Scene_Change;
	event_struct.object_address_1 = next_scene;

	EVENT_MANAGER->AddEvent(event_struct);

	std::cout << "Scene Load Thread End" << std::endl;
}

void Loading_Script::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //Script
}