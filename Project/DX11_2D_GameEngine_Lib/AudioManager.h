#pragma once

#include <FMOD/fmod.h>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_errors.h>
#include <FMOD/fmod_codec.h>

#ifdef _DEBUG
#pragma comment(lib, "FMOD/Debug/FMODx64_Debug.lib")
#else
#pragma comment(lib, "FMOD/Release/FMODx64_Release.lib")
#endif

class Transform;

class AudioManager final : public Singleton<AudioManager>
{
	SINGLETON(AudioManager);

	AudioManager() = default;
	~AudioManager();

public:
	void Initialize();
	void Update();

public:
	FMOD::System* GetAudioSystem() { SAFE_GET_POINTER(m_p_system); }

public:
    void SetListenerTransform(Transform* p_listener_transform);
	void SetListenerAttributes();

private:
    FMOD::System* m_p_system = nullptr;
	UINT m_version = 0;
	UINT m_max_channels = 32;
	float m_distance_factor = 50.0f; //Engine에서 1미터당 상대적인 거리 비율

	Transform* m_p_listener_transform = nullptr; //주로 해당 Scene의 Main Camera의 Transform 설정
	UINT m_interface_update_time = 50;
};

