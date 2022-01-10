#include "stdafx.h"
#include "AudioManager.h"

#include "SceneManager.h"

#include "Transform.h"

AudioManager::~AudioManager()
{
	m_p_listener_transform = nullptr;

	//Release FMOD System
	m_p_system->close();
	m_p_system->release();
}

void AudioManager::Initialize()
{
	//Create FMOD System
	auto result = FMOD::System_Create(&m_p_system);
	assert(result == FMOD_OK);

	//Check FMOD Version
	result = m_p_system->getVersion(&m_version);
	assert(result == FMOD_OK);

	//현재 사용하고 있는 FMOD 라이브러리 버전보다 낮은 경우 사용 불가
	if (m_version < FMOD_VERSION)
		return;

	//Initialize FMOD System(Channel을 32개까지 사용, FMOD 설정은 기본)
	result = m_p_system->init(m_max_channels, FMOD_INIT_NORMAL, nullptr);
	assert(result == FMOD_OK);

	//FMOD 3D 환경 설정
	result = m_p_system->set3DSettings(1.0, m_distance_factor, 1.0f);
	assert(result == FMOD_OK);
}

void AudioManager::Update()
{
	auto scene_manager = SceneManager::GetInstance();

	//Editor Mode에 정지 상태일 경우
	if (scene_manager->GetClientState() == 2 && scene_manager->GetEditorState() == EditorState::EditorState_Stop)
		return;

	m_p_system->update();

	//Listener가 설정되어 있다면
	if (m_p_listener_transform != nullptr)
		SetListenerAttributes();
}

void AudioManager::SetListenerTransform(Transform* p_listener_transform)
{
	if (p_listener_transform == nullptr)
		return;

	m_p_listener_transform = p_listener_transform;
}

void AudioManager::SetListenerAttributes()
{
    static Vector3 listener_last_position = Vector3::Zero; //Main Camera의 지난 프레임 위치

	auto listener_position = m_p_listener_transform->GetTranslation(); //Main Camera의 현재 위치
	auto velocity = Vector3::Zero;
	auto forward_vector = m_p_listener_transform->GetForwardVector();  //Main Camera의 바라보는 방향 벡터(전면 벡터)
	auto up_vector = m_p_listener_transform->GetUpVector();			//Main Camera의 업 벡터

	//velocity = how far we moved last FRAME (m/f), then time compensate it to SECONDS (m/s).
	velocity.x = (listener_position.x - listener_last_position.x) * (1000 / m_interface_update_time);
	velocity.y = (listener_position.y - listener_last_position.y) * (1000 / m_interface_update_time);
	velocity.z = (listener_position.z - listener_last_position.z) * (1000 / m_interface_update_time);

	listener_last_position = listener_position;

	m_p_system->set3DListenerAttributes
	(
		0,
		reinterpret_cast<FMOD_VECTOR*>(&listener_position),
		reinterpret_cast<FMOD_VECTOR*>(&velocity),
		reinterpret_cast<FMOD_VECTOR*>(&forward_vector),
		reinterpret_cast<FMOD_VECTOR*>(&up_vector)
	);
}
