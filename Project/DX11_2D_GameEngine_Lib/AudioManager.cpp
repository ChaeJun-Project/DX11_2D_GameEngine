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

	//���� ����ϰ� �ִ� FMOD ���̺귯�� �������� ���� ��� ��� �Ұ�
	if (m_version < FMOD_VERSION)
		return;

	//Initialize FMOD System(Channel�� 32������ ���, FMOD ������ �⺻)
	result = m_p_system->init(m_max_channels, FMOD_INIT_NORMAL, nullptr);
	assert(result == FMOD_OK);

	//FMOD 3D ȯ�� ����
	result = m_p_system->set3DSettings(1.0, m_distance_factor, 1.0f);
	assert(result == FMOD_OK);
}

void AudioManager::Update()
{
	auto scene_manager = SceneManager::GetInstance();

	//Editor Mode�� ���� ������ ���
	if (scene_manager->GetClientState() == 2 && scene_manager->GetEditorState() == EditorState::EditorState_Stop)
		return;

	m_p_system->update();

	//Listener�� �����Ǿ� �ִٸ�
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
    static Vector3 listener_last_position = Vector3::Zero; //Main Camera�� ���� ������ ��ġ

	auto listener_position = m_p_listener_transform->GetTranslation(); //Main Camera�� ���� ��ġ
	auto velocity = Vector3::Zero;
	auto forward_vector = m_p_listener_transform->GetForwardVector();  //Main Camera�� �ٶ󺸴� ���� ����(���� ����)
	auto up_vector = m_p_listener_transform->GetUpVector();			//Main Camera�� �� ����

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
