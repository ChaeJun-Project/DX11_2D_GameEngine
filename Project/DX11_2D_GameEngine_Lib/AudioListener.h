#pragma once
#include "IComponent.h"

class AudioListener final : public IComponent
{
public:
	AudioListener();
	explicit AudioListener(const AudioListener& origin);
	~AudioListener() = default;

public:
	void Start() override;
	void FinalUpdate() override; //���� ������Ʈ => ������Ʈ�� �����̸� �ȵ�

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

private:
	CLONE(AudioListener);
};

