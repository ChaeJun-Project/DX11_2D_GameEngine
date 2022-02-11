#pragma once
#include "IComponent.h"

class AudioListener final : public IComponent
{
public:
	AudioListener();
	explicit AudioListener(const AudioListener& origin) = default;
	~AudioListener() = default;

public:
	void Start() override;
	void FinalUpdate() override; //최종 업데이트 => 오브젝트가 움직이면 안됨

public:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override {};

public:
	CLONE(AudioListener);
};

