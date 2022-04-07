#pragma once
#include <DX11_2D_GameEngine_Lib/Script.h>

class StageEvent_Script final : public Script
{
public:
	StageEvent_Script();
	explicit StageEvent_Script(const StageEvent_Script& origin);
	virtual ~StageEvent_Script();

public:
    void SetEventFunc(std::function<void(void)> p_event_func) { m_p_event_func = p_event_func; }

public:
	void OnCollisionExit(GameObject* p_other_game_object) override;

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override {}

public:
	CLONE(StageEvent_Script);

private:
	std::function<void(void)> m_p_event_func = nullptr;
};

