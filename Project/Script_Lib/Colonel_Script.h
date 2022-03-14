#pragma once

class Transform;
class Animator2D;

#include <DX11_2D_GameEngine_Lib/Script.h>
#include "GameObjectController.h"
#include "ColonelState_Enum.h"

class Colonel_Script final : public Script, public GameObjectController
{
public:
	Colonel_Script();
	explicit Colonel_Script(const Colonel_Script& origin);
	virtual ~Colonel_Script();

	void Start() override;
	void Update() override;

private:
	void Update_Move() override;
	void Update_State() override;
	void Update_Animation() override;

private:
	void RegisterScriptParamData() override;

	//<summary>
	//Animation의 특정 Frame에 이벤트를 추가할 필요가 있는 경우 추가
	//</summary>
private:
	void AddAnimationEvent();

	void TriggerIdleState();

public:
	void OnCollisionEnter(GameObject* other_game_object) override;
	void OnCollisionStay(GameObject* other_game_object) override;
	void OnCollisionExit(GameObject* other_game_object) override;

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

public:
	CLONE(Colonel_Script);

private:
	Transform* m_p_transform = nullptr;

	UINT m_current_state = static_cast<UINT>(ColonelState::Start);
	UINT m_pre_state;
};

